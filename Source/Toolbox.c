#include "Toolbox.h"
#include "Keys.h"
#include "Remap.h"
//#include <InternetConfig.h>

#include "Items.h"
#include "Globals.h"
#include "FileIO.h"

//---------------------------------------------------------------------------
extern	WindowPtr						Window;
extern  ModalFilterUPP 		  myGenericFilter;
extern  UniversalProcPtr 		mySplashFilter;

TRect							screenBounds;

TBitmap						fxBuffer;
TBitmap						filterBuffer;
TBitmap						scratchBMP;

GWorldPtr					gw[MaxGWorlds];
TBoolean					gwFree[MaxGWorlds];
TRect							gwRect[MaxGWorlds];
RGBColor					gwForeColor[MaxGWorlds];
RGBColor					gwBackColor[MaxGWorlds];

PixMapHandle			gwPixMapHdl[MaxGWorlds];
Ptr								gwBaseAddress[MaxGWorlds];
UInt16						gwRowBytes[MaxGWorlds];

MenuHandle				theMenus[10];
TBoolean					MenuFree[10];

WindowPtr					win[MaxWindows];
TBoolean					winFree[MaxWindows];
TRect							winRect[MaxWindows];
RGBColor					winForeColor[MaxGWorlds];
RGBColor					winBackColor[MaxGWorlds];

RgnHandle					tbRgn[MaxRegions];
TBoolean					RgnFree[MaxRegions+1];

RGBColor					myWhite;
RGBColor    			myBlack;
RGBColor    			myRed;
RGBColor    			myGray;

CCrsrHandle				cur[MaxCursors];
TBoolean					curID[MaxCursors];

PicHandle					PHandle;
CGrafPtr 					tbsavedPort;
GDHandle 					tbsavedDevice;
StandardFileReply	currentFile;
short							refNum;
TBoolean					tb_SoundOn;
int								FileCheck;

short							AppRefNum;
FSSpec						AppFSSpec;

TPPrPort					printPort;
short							printerStartPage;
short							printeRendPage;
long							pageNumber;
Boolean						doneReport;
short							printerErrNum;
THPrint						hAppPrintRec;
THPrint						hDocPrintRec;
TPrStatus					printStatus;
CGrafPtr 					prSavedPort;
GDHandle 					prSavedDevice;
//EventHandlerUPP 	PrintProgressEventUPP;
short							progressWindowNum;
TBoolean					printerRecInitialized;

short 						restoreOldVRefNum;
long 							restoreOldDirID;
TBoolean					restoreValid;
short 						prefsvRefNum;
long 							prefsDirID;

short 						tbCurrentvRefNum;
long 							tbCurrentDirID;
int								tbFileType;

short							previousCursor=-1;

extern void gr_AddText(char	newLine[kStringLength], TBoolean systemMessage, TBoolean log);
extern void GetKeyEvents(void);

extern TBoolean		nightVision;
extern TBoolean		inDungeon;
extern TBoolean		playerDead;
extern Boolean		applicationSuspended;
extern TBoolean		usingX;
extern TBoolean		backgroundnotification;

extern TBoolean tt;
//---------------------------------------------------------------------------

#include "Globals.h"

// ----------------------------------------------------------------- //
//int RC(int value, int range)
// ----------------------------------------------------------------- //

//{

//	if (value>=range)
//		{
//			fi_WriteToErrorLog("Out of Range.");
//		}

//	return (value);

//}

// ----------------------------------------------------------------- //
void	tb_InitializeFilter(pTFilter filter)
// ----------------------------------------------------------------- //

{
	int	i,j;

	for (i=0; i<5; i++)
	{
		for (j=0; j<5; j++)
		{
			filter->C[i][j] = 0;
		}
	}

	filter->D = 1;

	filter->B = 0;
	filter->r = true;
	filter->g = true;
	filter->b = true;

	filter->rand = false;
	filter->min = 0;
	filter->max = 0;

}

/*******************************************************************
Add this function to your filters file
*******************************************************************/

// This is an optimized version of f_blur specifically written
// to help speed up the blur effect used in lightning special effect
void	f_QuickBlur(int bitmap, TRect *rect)
{

	TFilter	filter;
	int			r,c;

	tb_InitializeFilter(&filter);

	for (r=0; r<5; r++)
		for (c=0; c<5; c++)
			filter.C[r][c] = 1;

	filter.D = 25;

	filter.B = 0;

	tb_QuickApplyFilter(bitmap, rect, &filter);

}

// ----------------------------------------------------------------- //
/*	This function is very similar to tb_RenderLightSource except that
	you can control the color bias, i.e., you can specify the dominant
	color component red, green or blue.

	A "clr" value of 0 emphaizes red, 1 green and 2 blue.

	"level" controls the maximum randomly selected intensity.
*/

// ----------------------------------------------------------------- //
void	tb_RenderColoredLightSource(int bitmap, int x, int y, int radius, int clr, int level, int gameTime)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 					r			=	0;
	unsigned int 					g			=	0;
	unsigned int 					b			=	0;
	int										xi;
	int										yi;
	float									rn;
	float									d;
	float									intensity;
	long										maxDistance;
	float									falloff1;
	float									falloff2;
	float									falloff3;
	TRect									gwRect;

	if (inDungeon)
		gameTime=1;

	if (nightVision)
		gameTime=0;

	if ((bitmap<0) || (bitmap>=MaxGWorlds)) // mod 8/4
	{
		fi_WriteToErrorLog("tb_RenderColoredLightSource BMP out of range");
		return;
	}

	if (gwFree[bitmap]) // mod 8/4
	{
		fi_WriteToErrorLog("tb_RenderColoredLightSource BMP free");
		return;
	}

	tb_GetBitmapRect(bitmap, &gwRect);

	intensity=(float)tb_Rnd(1,level)/100;
	maxDistance		= radius*radius;
	falloff1			= maxDistance*0.375;
	falloff2			= maxDistance*0.625;
	falloff3			= maxDistance*0.875;

	for (xi=(x-radius);xi<(x+radius);xi++)
		for (yi=(y-radius);yi<(y+radius);yi++)
		{
			// if out of bounds then skip
			if (xi<=gwRect.left)		continue;

			if (xi>=gwRect.right)		continue;

			if (yi<=gwRect.top)		continue;

			if (yi>=gwRect.bottom)	continue;

			// calculate brightness
			d=((x-xi)*(x-xi))+((y-yi)*(y-yi));

			if (d>maxDistance) 				continue;

			if ((gameTime==1) || (gameTime==2) || (gameTime==3))
			{
				rn=(0.965+(1/(d/radius))+intensity)*1.8;

				if (rn>2.5) rn=2.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.75;

				if (d>falloff3)	rn=rn*0.65;

				if (rn<1)				rn=1;
			}
			else
			{
				rn=0.965+(1/(d/radius))+intensity;

				if (rn>1.5) rn=1.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.9;

				if (d>falloff3)	rn=rn*0.9;

				if (rn<1) rn=1;
			}

			// the next four line get the (R,G,B) values.  Each ranges from 0-65535.
			myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			r = (*myPixelPtr << 1) & 0xF800;

			g = (*myPixelPtr << 6) & 0xF800;

			b = (*myPixelPtr << 11) & 0xF800;

			//tb_GetPixelRGB(bitmap, xi, yi, &r, &g, &b);

			// increase brightness of each component with emphasis on desired RGB comp.

			if (clr == 0)
			{
				r=r*(0.3+rn);
				g=g*(rn);
				b=b*(rn);

				if (r>65535)	r=65535;

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else if (clr == 1)
			{
				r=r*(rn);
				g=g*(0.3+rn);
				b=b*(rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(65535))	g=65535;

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else
			{
				r=r*(rn);
				g=g*(rn);
				b=b*(0.3+rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>65535)	b=65535;
			}

			// set the pixel.
			*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

			//tb_SetPixelRGB(bitmap, xi, yi, r, g, b);

		}

}



// ----------------------------------------------------------------- //
/*
	This function Renders a lightning bolt.

	x1,y1:	the starting coordinates
	x2,y2:	the }ing coordinates
	r,g,b:	the color components
	thk:	line thickness
	blur:	true if the bolt is to be blurred, false if not (it looks better
			blurred but is slower to draw)
*/

// ----------------------------------------------------------------- //
void	tb_RenderLightning(int bitmap, int x1, int y1, int x2, int y2, unsigned int r, unsigned int g, unsigned int b, int thk, TBoolean blur)
// ----------------------------------------------------------------- //

{
#define	_NUMSEGMENTS	20
#define	_NUMBRANCHES	2
#define	_MAXXOFFSET		10
#define _MAXYOFFSET		10

	typedef struct TFPointTag
	{
		float x;
		float y;
	} TFPoint;

	double fee;
	double angle;
	double length;
	//char	debug[255];
	TFPoint EndPoints[_NUMSEGMENTS];
	TFPoint	Branch[_NUMBRANCHES];
	double c,s;
	double x, y;
	int	i,j;
	int m;
	double	lg;

	//double	slope;
	//double	icept;
	unsigned int	r2=0,g2=0,b2=0;
	TRect	brect;
	//TBitmap	buf;
	int		w,h;
	int		xOffset, yOffset;

	xOffset = _MAXXOFFSET+thk/2;
	yOffset = _MAXYOFFSET+thk/2;

	w = abs(x2-x1)+2*xOffset;
	h = abs(y2-y1)+2*yOffset;

	if (w>560) w=560;

	if (h>560) h=560;

	tb_SetRect(&brect, 0, 0, w, h);

	//buf = tb_CreateBitmap(&brect, 0, 16);
	tb_SetForeColor(fxBuffer, 0, 0, 0);

	tb_PaintRect(fxBuffer, &brect);

	// first get the line angle (ccw, 0 right, 90 up, 180 left, 270 down)
	if (y2 == y1 &&  x2 > x1)
	{
		fee = 0;
		angle = 0;
	}
	else if (y2 == y1 && x2 < x1)
	{
		fee = 180;
		angle = 180;
	}
	else if (x2 == x1 && y2 < y1)
	{
		fee = 90;
		angle = 90;
	}
	else if (x2 == x1 && y2 > y1)
	{
		fee = 270;
		angle = 270;
	}
	else if (x2>x1 && y2<y1)
	{
		fee = atan((double)(y1-y2)/(double)(x2-x1)) * 180 / tbPI;
		angle = fee;
	}
	else if (x2>x1 && y2>y1)
	{
		fee = atan((double)(y2-y1)/(double)(x2-x1)) * 180 / tbPI;
		angle = 360-fee;
	}
	else if (x2<x1 && y2>y1)
	{
		fee = atan((double)(y2-y1)/(double)(x1-x2)) * 180 / tbPI;
		angle = 180+fee;
	}
	else if (x2<x1 && y2<y1)
	{
		fee = atan((double)(y1-y2)/(double)(x1-x2)) * 180 / tbPI;
		angle = 180-fee;
	}
	else
	{
		fee = 0;
		angle = 0;
	}

	//  next, get the length of the line
	length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

	lg = length/(_NUMSEGMENTS-1);

	//  next, set the initial, unrotated, coordinates of the
	//  } points of each line segment
	for (i=0; i<_NUMSEGMENTS; i++)
	{
		EndPoints[i].x = i*(lg);

		if (tb_Rnd(0,1) == 0)
			m = -1;
		else
			m = 1;

		EndPoints[i].y = (m*tb_Rnd(0,_MAXYOFFSET));
	}

	EndPoints[0].y = 0;

	EndPoints[_NUMSEGMENTS-1].y = 0;

	for (i=0; i<_NUMBRANCHES; i++)
	{
		m = tb_Rnd(0, _NUMSEGMENTS-1);
		Branch[i].x = EndPoints[m].x;
		Branch[i].y = EndPoints[m].y;
	}

	for (i=1;i<_NUMBRANCHES; i+=2)
	{
		Branch[i].x = Branch[i-1].x + tb_Rnd(0,_MAXXOFFSET);

		if (tb_Rnd(0,1) == 0)
			m = -1;
		else
			m = 1;

		Branch[i].y = Branch[i-1].y + (m*tb_Rnd(0,_MAXYOFFSET));

	}

	c = cos(angle*tbPI/180.0f);

	s = sin(angle*tbPI/180.0f);

	for (i = 0; i<_NUMSEGMENTS; i++)
	{
		x = EndPoints[i].x;
		y = EndPoints[i].y;

		if ( (x2>=x1) && (y2>=y1) )
		{
			EndPoints[i].x = xOffset+(c * x + s * y);
			EndPoints[i].y = yOffset+(-s * x + c * y);
		}
		else if ( (x2>=x1) && (y2<=y1) )
		{
			EndPoints[i].x = xOffset+(c * x + s * y);
			EndPoints[i].y = h-yOffset+(-s * x + c * y);
		}
		else if ( (x2<=x1) && (y2>=y1) )
		{
			EndPoints[i].x = w-xOffset+(c * x + s * y);
			EndPoints[i].y = yOffset+(-s * x + c * y);
		}
		else if ( (x2<=x1) && (y2<=y1) )
		{
			EndPoints[i].x = w-xOffset+(c * x + s * y);
			EndPoints[i].y = h-yOffset+(-s * x + c * y);
		}
	}

	for (i=0; i<_NUMBRANCHES; i++)
	{
		x = Branch[i].x;
		y = Branch[i].y;

		if ( (x2>=x1) && (y2>=y1) )
		{
			Branch[i].x = xOffset+(c * x + s * y);
			Branch[i].y = yOffset+(-s * x + c * y);
		}
		else if ( (x2>=x1) && (y2<=y1) )
		{
			Branch[i].x = xOffset+(c * x + s * y);
			Branch[i].y = h-yOffset+(-s * x + c * y);
		}
		else if ( (x2<=x1) && (y2>=y1) )
		{
			Branch[i].x = w-xOffset+(c * x + s * y);
			Branch[i].y = yOffset+(-s * x + c * y);
		}
		else if ( (x2<=x1) && (y2<=y1) )
		{
			Branch[i].x = w-xOffset+(c * x + s * y);
			Branch[i].y = h-yOffset+(-s * x + c * y);
		}
	}

	tb_SetForeColor(bitmap, r, g, b);

	tb_SetForeColor(fxBuffer, r, g, b);

	for (i=0; i<(_NUMSEGMENTS-1); i++)
	{
		tb_Line(	fxBuffer,
				 tb_Round(EndPoints[i].x),
				 tb_Round(EndPoints[i].y),
				 tb_Round(EndPoints[i+1].x),
				 tb_Round(EndPoints[i+1].y), thk);
	}

	for (i=0; i<(_NUMBRANCHES-1); i+=2)
	{
		tb_Line(	fxBuffer,
				 tb_Round(Branch[i].x),
				 tb_Round(Branch[i].y),
				 tb_Round(Branch[i+1].x),
				 tb_Round(Branch[i+1].y), thk);

	}

	if (blur)
		f_QuickBlur(fxBuffer, &brect);

	for (i=1; i<(w-1); i++)
		for (j=1; j<(h-1); j++)
		{
			tb_GetPixelRGB(fxBuffer, i, j, &r, &g, &b);

			if (r+g+b > 0)
			{
				if ( (x2>=x1) && (y2>=y1) )
				{
					x = x1 + i- xOffset;
					y = y1 + j- yOffset;
				}
				else if ( (x2>=x1) && (y2<=y1) )
				{
					x = x1 + i- xOffset;
					y = y1 + j+ yOffset-h;
				}
				else if ( (x2<=x1) && (y2>=y1) )
				{
					x = x1 + i+ xOffset-w;
					y = y1 + j- yOffset;
				}
				else if ( (x2<=x1) && (y2<=y1) )
				{
					x = x1 + i+ xOffset-w;
					y = y1 + j+ yOffset-h;
				}

				if (x>560) x=560;

				if (y>560) y=560;

				tb_GetPixelRGB(bitmap, x, y, &r2, &g2, &b2);

				r = r2+tb_Round((float)r*(1+(float)r/65535.0f))/2;

				g = g2+tb_Round((float)g*(1+(float)g/65535.0f))/2;

				b = b2+tb_Round((float)b*(1+(float)b/65535.0f))/2;

				if (r>65535) r=65535;

				if (g>65535) g=65535;

				if (b>65535) b=65535;

				tb_SetPixelRGB(bitmap, x, y, r, g, b);
			}
		}

	// FOR DEBUGGING ONLY:
	//tb_CopyBitmap(buf, bitmap, &brect, &brect, false);

	//tb_DisposeBitmap(buf);

}

/*
// ----------------------------------------------------------------- //
void	tb_RenderLightning(int bitmap, int x1, int y1, int x2, int y2, unsigned int r, unsigned int g, unsigned int b, int thk, TBoolean blur)
// ----------------------------------------------------------------- //

{

#define	_NUMSEGMENTS	20
#define	_NUMBRANCHES	10
#define	_MAXXOFFSET		10
#define _MAXYOFFSET		10

	typedef struct TFPointTag {
		float x;
		float y;
	} TFPoint;

	double fee;
	double angle;
	double length;
	TFPoint EndPoints[_NUMSEGMENTS];
	TFPoint	Branch[_NUMBRANCHES];
	double c,s;
	double x, y;
	int	i,j;
	int m;
	double	lg;

	double	slope;
	double	icept;
	unsigned int	r2,g2,b2;
	TRect	brect;
	TBitmap	buf;
	int		w,h;


	w = abs(x2-x1)+2*_MAXXOFFSET;
	h = abs(y2-y1)+2*_MAXYOFFSET;
	tb_SetRect(&brect, 0, 0, w, h);
	buf = tb_CreateBitmap(&brect, 0, 16);
	tb_SetForeColor(buf, 0, 0, 0);
	tb_PaintRect(buf, &brect);

	// first get the line angle (ccw, 0 right, 90 up, 180 left, 270 down)
	if(y2 == y1 &&  x2 > x1) {
		fee = 0;
		angle = 0;
	} else if(y2 == y1 && x2 < x1) {
		fee = 180;
		angle = 180;
	} else if(x2 == x1 && y2 < y1) {
		fee = 90;
		angle = 90;
	} else if(x2 == x1 && y2 > y1) {
		fee = 270;
		angle = 270;
	} else if(x2>x1 && y2<y1) {
		fee = atan((double)(y1-y2)/(double)(x2-x1)) * 180 / tbPI;
		angle = fee;
	} else if(x2>x1 && y2>y1) {
		fee = atan((double)(y2-y1)/(double)(x2-x1)) * 180 / tbPI;
		angle = 360-fee;
	} else if(x2<x1 && y2>y1) {
		fee = atan((double)(y2-y1)/(double)(x1-x2)) * 180 / tbPI;
		angle = 180+fee;
	} else if(x2<x1 && y2<y1) {
		fee = atan((double)(y1-y2)/(double)(x1-x2)) * 180 / tbPI;
		angle = 180-fee;
	} else {
		fee = 0;
		angle = 0;
	}

	//  next, get the length of the line
	length = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	lg = length/(_NUMSEGMENTS-1);

	//  next, set the initial, unrotated, coordinates of the
	//  } points of each line segment
	for(i=0; i<_NUMSEGMENTS; i++)
	{
		EndPoints[i].x = i*(lg);
		if(tb_Rnd(0,1) == 0)
			m = -1;
		else
			m = 1;
		EndPoints[i].y = (m*tb_Rnd(0,_MAXYOFFSET));
	}
	EndPoints[0].y = 0;
	EndPoints[_NUMSEGMENTS-1].y = 0;

	for(i=0; i<_NUMBRANCHES; i++)
	{
		m = tb_Rnd(0, _NUMSEGMENTS);
		Branch[i].x = EndPoints[m].x;
		Branch[i].y = EndPoints[m].y;
	}

	for(i=1;i<_NUMBRANCHES; i+=2)
	{
		Branch[i].x = Branch[i-1].x + tb_Rnd(0,_MAXXOFFSET);

		if(tb_Rnd(0,1) == 0)
			m = -1;
		else
			m = 1;
		Branch[i].y = Branch[i-1].y + (m*tb_Rnd(0,_MAXYOFFSET));

	}

	c = cos(angle*tbPI/180.0f);
	s = sin(angle*tbPI/180.0f);
	for (i = 0; i<_NUMSEGMENTS; i++) {
		x = EndPoints[i].x;
		y = EndPoints[i].y;
		if( (x2>x1) && (y2>y1) )
		{
			EndPoints[i].x = _MAXXOFFSET+(c * x + s * y);
			EndPoints[i].y = _MAXYOFFSET+(-s * x + c * y);
		} else if( (x2>x1) && (y2<y1) )
		{
			EndPoints[i].x = _MAXXOFFSET+(c * x + s * y);
			EndPoints[i].y = h-_MAXYOFFSET+(-s * x + c * y);
		} else if( (x2<x1) && (y2>y1) )
		{
			EndPoints[i].x = w-_MAXXOFFSET+(c * x + s * y);
			EndPoints[i].y = _MAXYOFFSET+(-s * x + c * y);
		} else if( (x2<x1) && (y2<y1) )
		{
			EndPoints[i].x = w-_MAXXOFFSET+(c * x + s * y);
			EndPoints[i].y = h-_MAXYOFFSET+(-s * x + c * y);
		}
	}

	for(i=0; i<_NUMBRANCHES; i++)
	{
		x = Branch[i].x;
		y = Branch[i].y;
		if( (x2>x1) && (y2>y1) )
		{
			Branch[i].x = _MAXXOFFSET+(c * x + s * y);
			Branch[i].y = _MAXYOFFSET+(-s * x + c * y);
		} else if( (x2>x1) && (y2<y1) )
		{
			Branch[i].x = _MAXXOFFSET+(c * x + s * y);
			Branch[i].y = h-_MAXYOFFSET+(-s * x + c * y);
		} else if( (x2<x1) && (y2>y1) )
		{
			Branch[i].x = w-_MAXXOFFSET+(c * x + s * y);
			Branch[i].y = _MAXYOFFSET+(-s * x + c * y);
		} else if( (x2<x1) && (y2<y1) )
		{
			Branch[i].x = w-_MAXXOFFSET+(c * x + s * y);
			Branch[i].y = h-_MAXYOFFSET+(-s * x + c * y);
		}
	}

	tb_SetForeColor(bitmap, r, g, b);
	tb_SetForeColor(buf, r, g, b);

	for(i=0; i<(_NUMSEGMENTS-1); i++)
	{
		tb_Line(	buf,
					tb_Round(EndPoints[i].x),
					tb_Round(EndPoints[i].y),
					tb_Round(EndPoints[i+1].x),
					tb_Round(EndPoints[i+1].y), thk);
	}

	for(i=0; i<_NUMBRANCHES; i+=2)
	{
		tb_Line(buf, Branch[i].x, Branch[i].y, Branch[i+1].x, Branch[i+1].y, thk);
	}

	if (blur)
		f_QuickBlur(buf, &brect);

	for(i=1; i<(w-1); i++)
		for(j=1; j<(h-1); j++)
		{
			tb_GetPixelRGB(buf, i, j, &r, &g, &b);
			if(r+g+b > 0)
			{
				if( (x2>x1) && (y2>y1) )
				{
					x = x1 + i- _MAXXOFFSET;
					y = y1 + j- _MAXYOFFSET;
				} else if( (x2>x1) && (y2<y1) )
				{
					x = x1 + i- _MAXXOFFSET;
					y = y1 + j+ _MAXYOFFSET-h;
				} else if( (x2<x1) && (y2>y1) )
				{
					x = x1 + i+ _MAXXOFFSET-w;
					y = y1 + j- _MAXYOFFSET;
				} else if( (x2<x1) && (y2<y1) )
				{
					x = x1 + i+ _MAXXOFFSET-w;
					y = y1 + j+ _MAXYOFFSET-h;
				}


				tb_GetPixelRGB(bitmap, x, y, &r2, &g2, &b2);
				r = r2+tb_Round((float)r*(1+(float)r/65535.0f))/2;
				g = g2+tb_Round((float)g*(1+(float)g/65535.0f))/2;
				b = b2+tb_Round((float)b*(1+(float)b/65535.0f))/2;
				if(r>65535) r=65535;
				if(g>65535) g=65535;
				if(b>65535) b=65535;

				tb_SetPixelRGB(bitmap, x, y, r, g, b);
			}
		}

	// FOR DEBUGGING ONLY:
	//tb_CopyBitmap(buf, bitmap, &brect, &brect, false);

	tb_DisposeBitmap(buf);

}
*/
// ----------------------------------------------------------------- //
/*	This function Renders a warping effect that looks as though the
	area being warped is being shaken vigourously.  (I figure that this
	would be a good effect to apply when a character is under a disorient
	curse or spell.)

	x,y:	center of the effect
	radius:	affected radius
	str:	strength of the shaking effect
*/

// ----------------------------------------------------------------- //
void	tb_RenderWarp(int bitmap, int x, int y, int radius, int str)
// ----------------------------------------------------------------- //

{
//UInt16* 							myPixelPtr 	= nil;

	unsigned int 					r			=	0;
	unsigned int 					g			=	0;
	unsigned int 					b			=	0;

	int							xi;
	int							yi;
	float							rn;
	float							d;
	float							intensity;
	long							maxDistance;
	//float							falloff1;
	//float							falloff2;
	//float							falloff3;

	unsigned int 					r2			=	0;
	unsigned int 					g2			=	0;
	unsigned int 					b2			=	0;

	int	m, dx,dy;
	int	fdx, fdy;
	TBitmap	buf;
	TRect		brect;
	TRect		crect;
	int	xl, yl;
	int	dsize;

	TRect							gwRect;

	tb_GetBitmapRect(bitmap, &gwRect);

	intensity=(float)tb_Rnd(1,25)/100;
	maxDistance		= radius;

	if (tb_Rnd(0,1) == 0)
		m = -1;
	else
		m = 1;

	dx = m*tb_Rnd(0,str);

	if (tb_Rnd(0,1) == 0)
		m = -1;
	else
		m = 1;

	dy = m*tb_Rnd(0,str);

	if (abs(dx) > abs(dy))
		dsize = abs(dx);
	else
		dsize = abs(dy);

	tb_SetRect(&brect, 0, 0, 2*radius+2*dsize, 2*radius+2*dsize);

	buf = tb_CreateBitmap(&brect, 0, 16);

	tb_SetRect(&crect, x-radius-dsize, y-radius-dsize, x+radius+dsize, y+radius+dsize);

	tb_CopyBitmap(bitmap, buf, &crect, &brect, false);

	for (xi=(x-radius);xi<(x+radius);xi++)
		for (yi=(y-radius);yi<(y+radius);yi++)
		{
			// if out of bounds then skip
			if (xi<=gwRect.left)		continue;

			if (xi>=gwRect.right)		continue;

			if (yi<=gwRect.top)		continue;

			if (yi>=gwRect.bottom)	continue;

			d=sqrt(((x-xi)*(x-xi))+((y-yi)*(y-yi)));

			if (d>maxDistance) 				continue;

			rn=(d/radius);

			tb_GetPixelRGB(bitmap, xi, yi, &r, &g, &b);

			fdx = dx-tb_Round((float) dx*rn);

			fdy = dy-tb_Round((float) dy*rn);

			xl = xi+radius-x+fdx+dsize;

			yl = yi+radius-y+fdy+dsize;

			if ( (xl<0) || (xl>brect.right-1) || (yl<0) || (yl>brect.bottom-1) )
			{
				r2 = r;
				g2 = g;
				b2 = b;
			}
			else
				tb_GetPixelRGB(buf, xl, yl, &r2, &g2, &b2);

			r2 = (r+r2)/2;

			g2 = (g+g2)/2;

			b2 = (b+b2)/2;

			r = r2*(1+(intensity - intensity*rn));

			g = g2*(1+(intensity - intensity*rn));

			b = b2*(1+(intensity - intensity*rn));

			tb_SetPixelRGB(bitmap, xi, yi, r, g, b);

		}

	tb_DisposeBitmap(buf);
}

// ----------------------------------------------------------------- //
/*	This function is similar to tb_RenderColoredLightSource except that
	a ring of light is drawn. "rfactor" specifies the size of the hole
	in the ring as a percentage of the radius; 100 implies a complete hole,
	while 0 implies no hole at all (a filled circle of light)
*/
// ----------------------------------------------------------------- //
void	tb_RenderLightRing(int bitmap, int x, int y, int radius, int rfactor, int clr, int level, int gameTime)
// ----------------------------------------------------------------- //

{
//UInt16* 							myPixelPtr 	= nil;
	unsigned int 					r			=	0;
	unsigned int 					g			=	0;
	unsigned int 					b			=	0;
	int										xi;
	int										yi;
	float									rn;
	float									d;
	float									intensity;
	long										maxDistance;
	float									falloff1;
	float									falloff2;
	float									falloff3;
	float									minDistance;
	TRect									gwRect;

	if (inDungeon)
		gameTime=1;

	if (nightVision)
		gameTime=0;

	tb_GetBitmapRect(bitmap, &gwRect);

	intensity=(float)tb_Rnd(1,level)/100;

	maxDistance		= radius*radius;

	minDistance		= maxDistance*(float) rfactor/100;

	falloff1			= minDistance + (maxDistance-minDistance)/4;

	falloff2			= minDistance + (maxDistance-minDistance)/4*2;

	falloff3			= minDistance + (maxDistance-minDistance)/4*3;


	for (xi=(x-radius);xi<(x+radius);xi++)
		for (yi=(y-radius);yi<(y+radius);yi++)
		{
			// if out of bounds then skip
			if (xi<=gwRect.left)		continue;

			if (xi>=gwRect.right)		continue;

			if (yi<=gwRect.top)		continue;

			if (yi>=gwRect.bottom)	continue;

			// calculate brightness
			d=((x-xi)*(x-xi))+((y-yi)*(y-yi));

			if (d>maxDistance) 				continue;

			if (d<minDistance)				continue;

			if ((gameTime==1) || (gameTime==2) || (gameTime==3))
			{
				rn=(0.965+(1/(maxDistance/d))+intensity)*1.5;

				if (rn>2.5) rn=2.5;

				if ((d>falloff1) && (d<falloff2))	rn=rn*1.4;

				if ((d>falloff2) && (d<falloff3))	rn=rn*1.8;

				if (d>falloff3)	rn=rn*1.4;

				if (rn<1)				rn=1;
			}
			else
			{
				rn=(0.965+(1/(maxDistance/d))+intensity)*0.5;

				if (rn>1.5) rn=1.5;

				if ((d>falloff1) && (d<falloff2))	rn=rn*1.3;

				if ((d>falloff2) && (d<falloff3))	rn=rn*1.6;

				if (d>falloff3)	rn=rn*1.3;

				if (rn<1) rn=1;
			}

			// the next four line get the (R,G,B) values.  Each ranges from 0-65535.
			//myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			//r = (*myPixelPtr << 1) & 0xF800;
			//g = (*myPixelPtr << 6) & 0xF800;
			//b = (*myPixelPtr << 11) & 0xF800;
			tb_GetPixelRGB(bitmap, xi, yi, &r, &g, &b);

			// increase brightness of each component with emphasis on desired RGB comp.

			if (clr == 0)
			{
				r=r*(0.3+rn);
				g=g*(rn);
				b=b*(rn);

				if (r>65535)	r=65535;

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else if (clr == 1)
			{
				r=r*(rn);
				g=g*(0.3+rn);
				b=b*(rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(65535))	g=65535;

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else
			{
				r=r*(rn);
				g=g*(rn);
				b=b*(0.3+rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>65535)	b=65535;
			}

			// set the pixel.
			//*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);
			tb_SetPixelRGB(bitmap, xi, yi, r, g, b);

		}

}

// ----------------------------------------------------------------- //
/*	This function takes the source bitmap and transparently Renders it
	to the destination, where the source bitmap is used as a color intensity
	map to scale the given color components cr, cg and cb.

	dest:	destination bitmap
	src:	src bitmap (intensity map)
	xc,yc:	center of the area on the destination to apply the effect
	frames:	number of image frames in the src map; each time this function
			is called a frame to be drawn will be randomly selected
	cr, cg, cb:	color components for the resulting effect
	alpha:	controls the level of intensity for the effect (the higher the number
			the more intense the effect, the lower the number the more transparent the effect)
*/
// ----------------------------------------------------------------- //
void	tb_RenderBitmap(TBitmap dest, int	src, int xc, int yc, int frames, int cr, int cg, int cb, int alpha)
// ----------------------------------------------------------------- //
{
	TBitmap		bmp;
	TRect		brect;
	TRect		frect;
	int			dx;
	int			fr;
	int			x;
	int			h;
	int			i,j;
	unsigned int	r,g,b, r2,g2,b2;
	int			xi, yi;
	float		f1;

	bmp = tb_CreateBitmap(&brect, src, 16);
	dx = (brect.right-brect.left)/frames;
	fr = tb_Rnd(0, frames-1);
	h = brect.bottom - brect.top;
	x = fr * dx;
	tb_SetRect(&frect, x, 0, x+dx, h);
	f1 = (float)alpha/100.0f;

	for (i=x; i<x+dx; i++)
		for (j=0; j<h; j++)
		{
			xi = xc+i-x-dx/2;
			yi = yc+j-h/2;
			tb_GetPixelRGB(bmp, i, j, &r, &g, &b);
			tb_GetPixelRGB(dest, xi, yi, &r2, &g2, &b2);

			if (r+g+b > 0)
			{
				r = tb_Round((r2+f1*(float)cr*((float)r/65535.0f)));
				g = tb_Round((g2+f1*(float)cg*((float)g/65535.0f)));
				b = tb_Round((b2+f1*(float)cb*((float)b/65535.0f)));

				if (r>65535) r=65535;

				if (g>65535) g=65535;

				if (b>65535) b=65535;

				tb_SetPixelRGB(dest, xi, yi, r, g, b);
			}
		}

	tb_DisposeBitmap(bmp);
}

// ----------------------------------------------------------------- //
void	tb_RenderBitmapFrame(TBitmap dest, int	src, int xc, int yc, int frames, int fra, int cr, int cg, int cb, int alpha)
// ----------------------------------------------------------------- //
{
	TBitmap		bmp;
	TRect		brect;
	TRect		frect;
	int			dx;
	//int			fr;
	int			x;
	int			h;
	int			i,j;
	unsigned int	r,g,b, r2,g2,b2;
	int			xi, yi;
	float		f1;

	if ((xc<0) || (yc<0) || (xc>560) || (yc>560))
		return;

	bmp = tb_CreateBitmap(&brect, src, 16);

	dx = (brect.right-brect.left)/frames;

	h = brect.bottom - brect.top;

	x = fra * dx;

	tb_SetRect(&frect, x, 0, x+dx, h);

	f1 = (float)alpha/100.0f;

	for (i=x; i<x+dx; i++)
		for (j=0; j<h; j++)
		{
			xi = xc+i-x-dx/2;
			yi = yc+j-h/2;
			tb_GetPixelRGB(bmp, i, j, &r, &g, &b);
			tb_GetPixelRGB(dest, xi, yi, &r2, &g2, &b2);

			if (r+g+b > 0)
			{
				r = tb_Round((r2+f1*(float)cr*((float)r/65535.0f)));
				g = tb_Round((g2+f1*(float)cg*((float)g/65535.0f)));
				b = tb_Round((b2+f1*(float)cb*((float)b/65535.0f)));

				if (r>65535) r=65535;

				if (g>65535) g=65535;

				if (b>65535) b=65535;

				tb_SetPixelRGB(dest, xi, yi, r, g, b);
			}
		}

	tb_DisposeBitmap(bmp);
}

// ----------------------------------------------------------------- //
/*	Like tb_RenderColoredLightSource but makes an ellipse instead of a circle.
*/

// ----------------------------------------------------------------- //
void	tb_RenderLightEllipse(int bitmap, int x, int y, int w, int h, int clr, int /*level*/, int gameTime)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 					r			=	0;
	unsigned int 					g			=	0;
	unsigned int 					b			=	0;
	int										xi;
	int										yi;
	float									rn;
	float									d;
	float									intensity;
	float									maxDistance;
	float									falloff1;
	float									falloff2;
	float									falloff3;
	TRect									gwRect;


	if (inDungeon)
		gameTime=1;

	if (nightVision)
		gameTime=0;

	tb_GetBitmapRect(bitmap, &gwRect);

	intensity=(float)tb_Rnd(1,25)/100;

	maxDistance		= 0.25f;

	falloff1			= maxDistance*0.375;

	falloff2			= maxDistance*0.625;

	falloff3			= maxDistance*0.875;

	for (xi=(x-w/2);xi<(x+w/2);xi++)
		for (yi=(y-h/2);yi<(y+h/2);yi++)
		{
			// if out of bounds then skip
			if (xi<=gwRect.left)		continue;

			if (xi>=gwRect.right)		continue;

			if (yi<=gwRect.top)		continue;

			if (yi>=gwRect.bottom)	continue;

			// calculate brightness
			d = ((float)(x-xi)*(float)(x-xi))/(float)(w*w)+((float)(y-yi)*(float)(y-yi))/(float)(h*h);

			if (d > maxDistance) continue;

			if ((gameTime==1) || (gameTime==2) || (gameTime==3))
			{
				rn=(0.965+(1/(maxDistance/d))+intensity)*1.8;

				if (rn>2.5) rn=2.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.75;

				if (d>falloff3)	rn=rn*0.65;

				if (rn<1)				rn=1;
			}
			else
			{
				rn=0.965+(1/(maxDistance/d))+intensity;

				if (rn>1.5) rn=1.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.9;

				if (d>falloff3)	rn=rn*0.9;

				if (rn<1) rn=1;
			}

			// the next four line get the (R,G,B) values.  Each ranges from 0-65535.
			myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			r = (*myPixelPtr << 1) & 0xF800;

			g = (*myPixelPtr << 6) & 0xF800;

			b = (*myPixelPtr << 11) & 0xF800;

			//tb_GetPixelRGB(bitmap, xi, yi, &r, &g, &b);

			// increase brightness of each component with emphasis on red.
			if (clr == 0)
			{
				r=r*(0.3+rn);
				g=g*(rn);
				b=b*(rn);

				if (r>65535)	r=65535;

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else if (clr == 1)
			{
				r=r*(rn);
				g=g*(0.3+rn);
				b=b*(rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(65535))	g=65535;

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else
			{
				r=r*(rn);
				g=g*(rn);
				b=b*(0.3+rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>65535)	b=65535;
			}

			// set the pixel.
			*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

			//tb_SetPixelRGB(bitmap, xi, yi, r, g, b);

		}

}

// ----------------------------------------------------------------- //
/*	Like tb_RenderLightRing but makes an ellipse instead of a circle.
*/

// ----------------------------------------------------------------- //
void	tb_RenderLightRingEllipse(int bitmap, int x, int y, int w, int h, int rfactor, int clr, int level, int gameTime)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 					r			=	0;
	unsigned int 					g			=	0;
	unsigned int 					b			=	0;
	int										xi;
	int										yi;
	float									rn;
	float									d;
	float									intensity;
	float									maxDistance;
	float									falloff1;
	float									falloff2;
	float									falloff3;
	TRect									gwRect;
	float									minDistance;


	if (inDungeon)
		gameTime=1;

	if (nightVision)
		gameTime=0;

	tb_GetBitmapRect(bitmap, &gwRect);

	intensity=(float)tb_Rnd(1,level)/100;

	maxDistance		= 0.25f;

	minDistance		= maxDistance*(float) rfactor/100;

	falloff1			= minDistance + (maxDistance-minDistance)/4;

	falloff2			= minDistance + (maxDistance-minDistance)/4*2;

	falloff3			= minDistance + (maxDistance-minDistance)/4*3;


	for (xi=(x-w/2);xi<(x+w/2);xi++)
		for (yi=(y-h/2);yi<(y+h/2);yi++)
		{
			// if out of bounds then skip
			if (xi<=gwRect.left)		continue;

			if (xi>=gwRect.right)		continue;

			if (yi<=gwRect.top)		continue;

			if (yi>=gwRect.bottom)	continue;

			// calculate brightness
			d = ((float)(x-xi)*(float)(x-xi))/(float)(w*w)+((float)(y-yi)*(float)(y-yi))/(float)(h*h);

			if (d > maxDistance) continue;

			if (d < minDistance)	continue;

			if ((gameTime==1) || (gameTime==2) || (gameTime==3))
			{
				rn=(0.965+(1/(maxDistance/d))+intensity)*0.8;

				if (rn>2.5) rn=2.5;

				if ((d>falloff1) && (d<falloff2))	rn=rn*1.5;

				if ((d>falloff2) && (d<falloff3))	rn=rn*1.8;

				if (d>falloff3)	rn=rn*1.5;

				if (rn<1)				rn=1;
			}
			else
			{
				rn=(0.965+(1/(maxDistance/d))+intensity)*0.5;

				if (rn>1.5) rn=1.5;

				if ((d>falloff1) && (d<falloff2))	rn=rn*1.3;

				if ((d>falloff2) && (d<falloff3))	rn=rn*1.6;

				if (d>falloff3)	rn=rn*1.3;

				if (rn<1) rn=1;
			}

			// the next four line get the (R,G,B) values.  Each ranges from 0-65535.
			myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			r = (*myPixelPtr << 1) & 0xF800;

			g = (*myPixelPtr << 6) & 0xF800;

			b = (*myPixelPtr << 11) & 0xF800;

//			tb_GetPixelRGB(bitmap, xi, yi, &r, &g, &b);

			// increase brightness of each component with emphasis on red.
			if (clr == 0)
			{
				r=r*(0.3+rn);
				g=g*(rn);
				b=b*(rn);

				if (r>65535)	r=65535;

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else if (clr == 1)
			{
				r=r*(rn);
				g=g*(0.3+rn);
				b=b*(rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(65535))	g=65535;

				if (b>(0.9*65535))	b=(0.9*65535);
			}
			else
			{
				r=r*(rn);
				g=g*(rn);
				b=b*(0.3+rn);

				if (r>(0.9*65535))	r=(0.9*65535);

				if (g>(0.9*65535))	g=(0.9*65535);

				if (b>65535)	b=65535;
			}

			// set the pixel.
			*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

			//tb_SetPixelRGB(bitmap, xi, yi, r, g, b);

		}

}

// ----------------------------------------------------------------- //
// This is an optimized version of tb_ApplyFilter specifically written
// to help speed up the f_blur effect used in lightning effect
void	tb_QuickApplyFilter(int bitmap, TRect *frect, pTFilter filter)
{
	//int		tmp;
	long	x, y;
	long	left, top, right, bottom;
	long	maxX, maxY;
	long	u, v;
	long	rP, gP, bP, sumrP, sumgP, sumbP;
	unsigned int	r, g, b;
	unsigned int	ir, ig, ib;
	long	i,j;
	long	c;
	long	w, h;
	TRect	brect;
	int		fC;
	TRect	irect;
	TBoolean	DoFilter;
	long	sumclr;
	TRect	theRect;

	tb_GetBitmapRect(bitmap, &brect);
	w = tb_GetBitmapWidth(bitmap);
	h = tb_GetBitmapHeight(bitmap);
	maxX = w-2;
	maxY = h-2;

	//tmp = tb_CreateBitmap(&brect, 0, 16);

	DoFilter = tb_IntersectRect(&irect, frect, &brect);

	if (!DoFilter) return;

	left = irect.left;

	top	 = irect.top;

	right = irect.right;

	bottom = irect.bottom;

	y = bottom;

	while ( y > top )
	{
		y--;
		x = right;

		while (x > left)
		{
			x--;
			sumrP = sumgP = sumbP = 0;

			sumclr=0;
			tb_GetPixelRGB(bitmap, x-2, y-2, &ir, &ig, &ib);
			//	ir=ir/256;
			//	ig=ig/256;
			//	ib=ib/256;
//			sumclr += ir+ig+ib;
//			tb_GetPixelRGB(bitmap, x-2, y+2, &ir, &ig, &ib);
			//	ir=ir/256;
			//	ig=ig/256;
			//	ib=ib/256;
//			sumclr += ir+ig+ib;
//			tb_GetPixelRGB(bitmap, x+2, y-2, &ir, &ig, &ib);
			//	ir=ir/256;
			//	ig=ig/256;
			//	ib=ib/256;
//			sumclr += ir+ig+ib;
			tb_GetPixelRGB(bitmap, x+2, y+2, &ir, &ig, &ib);
			//	ir=ir/256;
			//	ig=ig/256;
			//	ib=ib/256;
//			sumclr += ir+ig+ib;

			tb_GetPixelRGB(bitmap, x, y, &ir, &ig, &ib);
			//	ir=ir/256;
			//	ig=ig/256;
			//	ib=ib/256;
			sumclr += ir+ig+ib;

			//	if (sumclr==327680)
			//		tb_Beep();

			sumclr=sumclr/256;

			if (sumclr > 0)
			{

				for (j = -2; j <= 2; j++)
				{
					v = y+j;

					for (i = -2; i <= 2; i++)
					{
						u = x+i;

						if (v>0 && v<=maxY && u>0 && u<=maxX)
						{
							tb_GetPixelRGB(bitmap, u, v, &r, &g, &b);
							r=r/256;
							g=g/256;
							b=b/256;
							fC = filter->C[j+2][i+2];

							if (filter->rand)
								fC = fC * tb_Rnd(filter->min, filter->max);

							if (filter->r)
							{
								rP = (long) r * fC;
								sumrP += rP;
							}

							if (filter->g)
							{
								gP = (long) g * fC;
								sumgP += gP;
							}

							if (filter->b)
							{
								bP = (long) b * fC;
								sumbP += bP;
							}
						}
					}
				}
			}

			c = (sumrP / filter->D) + filter->B;
			if (c>255) c=255; else if (c<0) c=0;
			if (filter->r)	r = (UInt8) c; else r = ir;

			c = (sumgP / filter->D) + filter->B;
			if (c>255) c=255; else if (c<0) c=0;
			if (filter->g)	g = (UInt8) c; else g = ig;

			c = (sumbP / filter->D) + filter->B;
			if (c>255) c=255; else if (c<0) c=0;
			if (filter->b)	b = (UInt8) c; else b = ib;

			r=r*256;

			g=g*256;

			b=b*256;

			tb_SetPixelRGB(filterBuffer, x, y, r, g, b);
		}
	}

	//DIB_CopyBits(DIBs[tmp], left, top, (right-left), (bottom-top), DIBs[bitmap], left, top, 0);
	tb_SetRect(&theRect,left,top,right,bottom);

	tb_CopyBitmap(filterBuffer, bitmap, &theRect, &theRect, false);

	//tb_DisposeBitmap(tmp);
}

// ----------------------------------------------------------------- //
pascal	OSErr	GetVolumeInfoNoName(ConstStr255Param pathname,
								 short vRefNum,
								 HParmBlkPtr pb)
// ----------------------------------------------------------------- //

{
	Str255 tempPathname;
	OSErr error;

	/* Make sure pb parameter is not NULL */

	if ( pb != NULL )
	{
		pb->volumeParam.ioVRefNum = vRefNum;

		if ( pathname == NULL )
		{
			pb->volumeParam.ioNamePtr = NULL;
			pb->volumeParam.ioVolIndex = 0;		/* use ioVRefNum only */
		}
		else
		{
			BlockMoveData(pathname, tempPathname, pathname[0] + 1);	/* make a copy of the string and */
			pb->volumeParam.ioNamePtr = (StringPtr)tempPathname;	/* use the copy so original isn't trashed */
			pb->volumeParam.ioVolIndex = -1;	/* use ioNamePtr/ioVRefNum combination */
		}

		error = PBHGetVInfoSync(pb);

		pb->volumeParam.ioNamePtr = NULL;	/* ioNamePtr may point to local	tempPathname, so don't return it */
	}
	else
	{
		error = paramErr;
	}

	return ( error );
}

// ----------------------------------------------------------------- //
pascal	OSErr	DetermineVRefNum(ConstStr255Param pathname,
							  short vRefNum,
							  short *realVRefNum)
// ----------------------------------------------------------------- //

{
	HParamBlockRec pb;
	OSErr error;

	error = GetVolumeInfoNoName(pathname,vRefNum, &pb);

	if ( error == noErr )
	{
		*realVRefNum = pb.volumeParam.ioVRefNum;
	}

	return ( error );
}


// ----------------------------------------------------------------- //
pascal	OSErr	GetDirItems(short vRefNum,
						 long dirID,
						 ConstStr255Param name,
						 Boolean getFiles,
						 Boolean getDirectories,
						 FSSpecPtr items,
						 short reqItemCount,
						 short *actItemCount,
						 short *itemIndex) /* start with 1, then use what's returned */
// ----------------------------------------------------------------- //

{
	CInfoPBRec pb;
	OSErr error;
	long theDirID;
	Boolean isDirectory;
	FSSpec *endItemsArray;

	if ( *itemIndex > 0 )
	{
		/* NOTE: If I could be sure that the caller passed a real vRefNum and real directory */
		/* to this routine, I could rip out calls to DetermineVRefNum and GetDirectoryID and this */
		/* routine would be much faster because of the overhead of DetermineVRefNum and */
		/* GetDirectoryID and because GetDirectoryID blows away the directory index hint the Macintosh */
		/* file system keeps for indexed calls. I can't be sure, so for maximum throughput, */
		/* pass a big array of FSSpecs so you can get the directory's contents with few calls */
		/* to this routine. */

		/* get the real volume reference number */
		error = DetermineVRefNum(name, vRefNum, &pb.hFileInfo.ioVRefNum);

		if ( error == noErr )
		{
			/* and the real directory ID of this directory (and make sure it IS a directory) */
			error = GetDirectoryID(vRefNum, dirID, name, &theDirID, &isDirectory);

			if ( error == noErr )
			{
				if ( isDirectory )
				{
					*actItemCount = 0;
					endItemsArray = items + reqItemCount;

					while ( (items < endItemsArray) && (error == noErr) )
					{
						pb.hFileInfo.ioNamePtr = (StringPtr) &items->name;
						pb.hFileInfo.ioDirID = theDirID;
						pb.hFileInfo.ioFDirIndex = *itemIndex;
						error = PBGetCatInfoSync(&pb);

						if ( error == noErr )
						{
							items->parID = pb.hFileInfo.ioFlParID;	/* return item's parID */
							items->vRefNum = pb.hFileInfo.ioVRefNum;	/* return item's vRefNum */
							++*itemIndex;	/* prepare to get next item in directory */

							if ( (pb.hFileInfo.ioFlAttrib & ioDirMask) != 0 )
							{
								if ( getDirectories )
								{
									++*actItemCount; /* keep this item */
									++items; /* point to next item */
								}
							}
							else
							{
								if ( getFiles )
								{
									++*actItemCount; /* keep this item */
									++items; /* point to next item */
								}
							}
						}
					}
				}
				else
				{
					/* it wasn't a directory */
					error = dirNFErr;
				}
			}
		}
	}
	else
	{
		/* bad itemIndex */
		error = paramErr;
	}

	return ( error );
}

// ----------------------------------------------------------------- //
pascal	OSErr GetCatInfoNoName(short vRefNum,
							  long dirID,
							  ConstStr255Param name,
							  CInfoPBPtr pb)
// ----------------------------------------------------------------- //

{
	Str31 tempName;
	OSErr error;

	/* Protection against File Sharing problem */

	if ( (name == NULL) || (name[0] == 0) )
	{
		tempName[0] = 0;
		pb->dirInfo.ioNamePtr = tempName;
		pb->dirInfo.ioFDirIndex = -1;	/* use ioDirID */
	}
	else
	{
		pb->dirInfo.ioNamePtr = (StringPtr)name;
		pb->dirInfo.ioFDirIndex = 0;	/* use ioNamePtr and ioDirID */
	}

	pb->dirInfo.ioVRefNum = vRefNum;

	pb->dirInfo.ioDrDirID = dirID;
	error = PBGetCatInfoSync(pb);
	pb->dirInfo.ioNamePtr = NULL;
	return ( error );
}

// ----------------------------------------------------------------- //
pascal	OSErr	GetDirectoryID(short vRefNum,
							long dirID,
							ConstStr255Param name,
							long *theDirID,
							Boolean *isDirectory)
// ----------------------------------------------------------------- //

{
	CInfoPBRec pb;
	OSErr error;

	error = GetCatInfoNoName(vRefNum, dirID, name, &pb);

	if ( error == noErr )
	{
		*isDirectory = (pb.hFileInfo.ioFlAttrib & ioDirMask) != 0;

		if ( *isDirectory )
		{
			*theDirID = pb.dirInfo.ioDrDirID;
		}
		else
		{
			*theDirID = pb.hFileInfo.ioFlParID;
		}
	}

	return ( error );
}

// ----------------------------------------------------------------- //
void tb_RemoveClip(TBitmap dstBitmap)
// ----------------------------------------------------------------- //

{
	TRect				r;

	if (dstBitmap < 5000)
	{
		tb_GetBitmapRect(dstBitmap, &r);
	}
	else
	{
		tb_GetClientRect(dstBitmap, &r);
	}

	tb_SetClip(dstBitmap, &r);

}


// ----------------------------------------------------------------- //
OSStatus tb_LaunchURL(ConstStr255Param /*urlStr*/)
// ----------------------------------------------------------------- //

{
	/*
		OSStatus 			err;
		ICInstance 		inst;
		long 					startSel;
		long 					}Sel;


		err = ICStart(&inst, 'qdCV');
		if (err == noErr) {
			err = ICFindConfigFile(inst, 0, nil);
				if (err == noErr) {
					startSel = 0;
					endSel = urlStr[0];
					err = ICLaunchURL(inst, "\p", (char *) &urlStr[1], urlStr[0], &startSel, &endSel);
				}
			(void) ICStop(inst);
		}
		return (err);
		*/
}

// ----------------------------------------------------------------- //
void tb_DeleteFile(char filename[kStringLength])
// ----------------------------------------------------------------- //

{
	Str255								name;
	short 								vRefNum;
	long 									dirID;


	tb_CtoPStr255(filename,name);
	Error = HGetVol(NULL, &vRefNum, &dirID);
	FSMakeFSSpec(vRefNum, dirID, name, &currentFile.sfFile);
	Error=FSpDelete(&currentFile.sfFile);

}

// ----------------------------------------------------------------- //
void tb_SwithToApplicationFolder(void)
// ----------------------------------------------------------------- //

{
	//int				Error;


	restoreValid=false;

	if (SetDefault(AppFSSpec.vRefNum,AppFSSpec.parID,&restoreOldVRefNum,&restoreOldDirID)==noErr)
		restoreValid=true;

}

// ----------------------------------------------------------------- //
void tb_SwithToPrefsFolder(void)
// ----------------------------------------------------------------- //

{
	//int				Error;


	restoreValid=false;

	if (FindFolder(kOnSystemDisk, 'pref', false, &prefsvRefNum, &prefsDirID)!=noErr)
		return;

	if (SetDefault(prefsvRefNum,prefsDirID,&restoreOldVRefNum,&restoreOldDirID)==noErr)
		restoreValid=true;

}

// ----------------------------------------------------------------- //
void tb_RestorePreviousFolder(void)
// ----------------------------------------------------------------- //

{

	if (restoreValid)
		RestoreDefault(restoreOldVRefNum,restoreOldDirID);

	restoreValid=false;

}

// ----------------------------------------------------------------- //
pascal	OSErr SetDefault(short newVRefNum,long newDirID,short *oldVRefNum,long *oldDirID)
// ----------------------------------------------------------------- //
{
	OSErr	error;

	/* Get the current default volume/directory. */
	error = HGetVol(NULL, oldVRefNum, oldDirID);

	if ( error == noErr )
	{
		/* Set the new default volume/directory */
		error = HSetVol(NULL, newVRefNum, newDirID);
	}

	return ( error );
}

// ----------------------------------------------------------------- //
pascal	OSErr RestoreDefault(short /*oldVRefNum*/,long /*oldDirID*/)
// ----------------------------------------------------------------- //

{
	// gs carbon
	/*
		OSErr	error;
		short	defaultVRefNum;
		long	defaultDirID;
		long	defaultProcID;

		error = GetWDInfo(oldVRefNum, &defaultVRefNum, &defaultDirID, &defaultProcID);
		if ( error == noErr )
		{
			if ( defaultDirID != fsRtDirID )
			{
				error = SetVol(NULL, oldVRefNum);
			}
			else
			{
				error = HSetVol(NULL, oldVRefNum, oldDirID);
			}
		}

		return ( error );
	*/
}

// ----------------------------------------------------------------- //
pascal	OSErr	GetFileLocation(short refNum,short *vRefNum,long *dirID,StringPtr fileName)
// ----------------------------------------------------------------- //

{
	FCBPBRec pb;
	OSErr error;

	pb.ioNamePtr = fileName;
	pb.ioVRefNum = 0;
	pb.ioRefNum = refNum;
	pb.ioFCBIndx = 0;
	error = PBGetFCBInfoSync(&pb);

	if ( error == noErr )
	{
		*vRefNum = pb.ioFCBVRefNum;
		*dirID = pb.ioFCBParID;
	}

	return ( error );
}

// ----------------------------------------------------------------- //
pascal	OSErr	FSpGetFileLocation(short refNum, FSSpec *spec)
// ----------------------------------------------------------------- //

{
	return ( GetFileLocation(refNum, &(spec->vRefNum), &(spec->parID), spec->name) );
}

// ----------------------------------------------------------------- //
//void tb_SetRect(&Intensity(TBitmap Bitmap, TRect r, short intensity)
// ----------------------------------------------------------------- //

//{
//OffPixBase = GetPixBaseAddr(srcBuffer->pixmap) + ( y * ((*srcBuffer->pixmap)->rowBytes & 0x1FFF) + x );

//UInt16* tRGB16Ptr = (UInt16*) baseaddr + (y * rowBytes) + (x * 2);


//   UInt16* tRGB16Ptr = GetPixBaseAddr(srcBuffer->pixmap) + ( y * ((*srcBuffer->pixmap)->rowBytes & 0x1FFF) + x );

//   UInt16 value = *tRGB16Ptr;
//   UInt8 red = (0x1F & (value >> 10)) + intensity;
//   UInt8 green = (0x1F & (value >> 5)) + intensity;
//   UInt8 blue = (0x1F & (value >> 0)) + intensity;
//   if (red > 0x1F) red = 0x1F;
//   if (green > 0x1F) green = 0x1F;
//   if (blue > 0x1F) blue = 0x1F;
//   value = (red << 10) | (green << 5) | (blue << 0);
//   *tRGB16Ptr = value;

//}

// ----------------------------------------------------------------- //
void tb_ShowMessage(int	id,char errMess[kStringLength])
// ----------------------------------------------------------------- //

{

	DialogPtr						d;
	short								itemHit=0;
	Str255							s;

	GetGWorld(&tbsavedPort, &tbsavedDevice);
	tb_CtoPStr255(errMess,s);
	tb_SetCursor(0);
	d = GetNewDialog(id, nil, (WindowPtr)-1L);
	SetPortWindowPort(GetDialogWindow(d));
	ShowWindow(GetDialogWindow(d));
	ParamText(s,"\p","\p","\p");

	while (itemHit!=1)
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(d);

}

// ----------------------------------------------------------------- //
GWorldPtr tb_PtrToGWorld(TBitmap Bitmap)
// ----------------------------------------------------------------- //

{

	return (gw[Bitmap]);

}

// ----------------------------------------------------------------- //
void tb_FlushMouseEvents(void)
// ----------------------------------------------------------------- //

{

	FlushEvents(mDownMask + mUpMask, 0);

}

// ----------------------------------------------------------------- //
TBoolean tb_PointInRegion(TPoint pt, TRegion theRegion)
// ----------------------------------------------------------------- //

{

	if (RgnFree[theRegion]==true) return (false);

	if ((theRegion<1) || (theRegion>MaxRegions)) return (false);

#if (Debug==true)
	if (tbRgn[theRegion]==nil) return (false);

#endif

	if (PtInRgn(pt, tbRgn[theRegion])) return (true);

	return (false);

}

// ----------------------------------------------------------------- //
void tb_DisposeRegion(TRegion theRegion)
// ----------------------------------------------------------------- //

{

#if (Debug==true)

	if (RgnFree[theRegion]==true) return;

	if ((theRegion<1) || (theRegion>MaxRegions)) return;

#endif

	if (tbRgn[theRegion]!=NULL)
		DisposeRgn(tbRgn[theRegion]);

	RgnFree[theRegion]=true;

}

// ----------------------------------------------------------------- //
void tb_PaintRegion(int dstBitmap, TRegion	r)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		PaintRgn(tbRgn[r]);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		PaintRgn(tbRgn[r]);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
TRegion tb_CreateRegion(TPoint RgnArray[tb_MaxRegionsPoints])
// ----------------------------------------------------------------- //

{
	int					i;
	int					found=-1;

	if ((RgnArray[0].h==-1) && (RgnArray[0].v==-1)) return (-1);

	for (i=1; i<=MaxRegions; i++)
		if (RgnFree[i]==true)
		{
			found=i;
			break;
		}

#if (Debug==true)
	if (found==-1) tb_DisplayError("tb_CreateRegion:  Free rgn not found.");

#endif

	if (found==-1) return (-1);

	i=1;

	tbRgn[found]=NewRgn();

	OpenRgn();

	MoveTo(RgnArray[0].h,RgnArray[0].v);

	while ((RgnArray[i].h!=-1) && (RgnArray[i].v!=-1))
	{
		LineTo(RgnArray[i].h,RgnArray[i].v);
		i++;
	}

	LineTo(RgnArray[0].h,RgnArray[0].v);

	CloseRgn(tbRgn[found]);

	RgnFree[found]=false;

	return (found);

	i=1;
	LineTo(RgnArray[i].h,RgnArray[i].v);
	i=1;
	i=1;

}

// ----------------------------------------------------------------- //
void tb_CopyString(char str1[kStringLength],char str2[kStringLength])
// ----------------------------------------------------------------- //

{

	int		i=-1;

	if (str2[0]=='\0')
	{
		str1[0]='\0';
		return;
	}

	do
	{
		i++;
		str1[i] = str2[i];
	}
	while (str2[i]!='\0');

}

// ----------------------------------------------------------------- //
void tb_MakeLowerCase(char str[kStringLength])
// ----------------------------------------------------------------- //

{
	int		i;

	if (strcmp(str,"")==0)
		return;

	for (i=0;i<strlen(str);i++)
		if (((int)str[i]>=65) && ((int)str[i]<=90))
			str[i]=(char)((int)str[i]+32);

}

// ----------------------------------------------------------------- //
void tb_ConcatString(char str1[kStringLength], char str2[kStringLength], char str3[kStringLength])
// ----------------------------------------------------------------- //

{
	int			i;
	int			i1=-1;
	int			i2=-1;
	char		sb1[kStringLength];
	char		sb2[kStringLength];
	char		sb3[kStringLength];

	for (i=0;i<kStringLength;i++)
	{
		sb1[i]=str1[i];
		sb2[i]=str2[i];
		sb3[i]=str3[i];
	}

	do
	{
		i1++;
		i2++;
		sb1[i1] = sb2[i2];
	}
	while (sb2[i2]!='\0');

	i1--;

	i2=-1;

	do
	{
		i1++;
		i2++;

		if (i1>(kStringLength-1)) break;

		sb1[i1] = sb3[i2];
	}
	while (sb3[i2]!='\0');

	for (i=0;i<kStringLength;i++)
		str1[i]=sb1[i];

}

// ----------------------------------------------------------------- //
void tb_StringToNum(char str[kStringLength], long *number)
// ----------------------------------------------------------------- //

{
	Str255			pStr;//, pStr2;

	tb_CtoPStr255(str,pStr);
	StringToNum(pStr,number);

}

// ----------------------------------------------------------------- //
void tb_StringToFloat(char str[kStringLength], float *number)
// ----------------------------------------------------------------- //

{
	Str255			pStr, pStr2;
	char			intg[kStringLength], decm[kStringLength];
	int				i,j;
	long			num1;
	long			num2;
	long			div;
	TBoolean		decimal=false;

	j=0;
	i=0;

	while (str[i]!='\0')
	{
		if (str[i]=='.')
		{
			decimal=true;
			i++;
		}

		if (decimal)
		{
			decm[j]=str[i];
			decm[j+1]='\0';
			j++;
		}
		else
		{
			intg[i]=str[i];
			intg[i+1]='\0';
		}

		i++;
	}


	if (!decimal)
	{
		tb_CtoPStr255(str,pStr);
		StringToNum(pStr,&num1);
		*number=num1;
	}
	else
	{
		tb_CtoPStr255(intg,pStr);
		StringToNum(pStr,&num1);

		tb_CtoPStr255(decm, pStr2);
		StringToNum(pStr2,&num2);

		div=1;
		i=0;

		while (decm[i]!='\0')
		{
			div=div*10;
			i++;
		}

		*number=(float)num2/(float)div;

		*number+=num1;
	}

}

// ----------------------------------------------------------------- //
void tb_NumToString(long number, char str[kStringLength])
// ----------------------------------------------------------------- //

{
	Str255			pStr;

	NumToString(number, pStr);
	tb_PascaltoCStr(pStr,str);

}

// ----------------------------------------------------------------- //
void tb_IntegerToString(long number,char str1[kStringLength])
// ----------------------------------------------------------------- //

{
	long i, j, k, l, z, count;

	if (number>1000000000)  //018
	{
		strcpy(str1,"OOR");
		return;
	}

	i = 1;

	j = 0;

	if (number<0)
		number=fabs(number);

	if (number == 0) { str1[0] = '0';  str1[1] = '\0';  return; }

	while (number >= i)
		{ j++; i*=10; }

	if (j)
	{
		k=1;

		for (z=1; z<j; z++)
			k*=10;

		for (count = 0; count < j ; count++)
		{
			l = number / k;

			if (l==0) str1[count]='0';

			if (l==1) str1[count]='1';

			if (l==2) str1[count]='2';

			if (l==3) str1[count]='3';

			if (l==4) str1[count]='4';

			if (l==5) str1[count]='5';

			if (l==6) str1[count]='6';

			if (l==7) str1[count]='7';

			if (l==8) str1[count]='8';

			if (l==9) str1[count]='9';

			number -= l * k;

			k /= 10;
		}
	}

	str1[j] = '\0';
}

// ----------------------------------------------------------------- //
void tb_GetFontInfo(TBitmap dstBitmap, short* maxAscent, short *maxDescent)
// ----------------------------------------------------------------- //

{
	FontInfo						info;

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
		SetPortWindowPort(win[dstBitmap-5000]);
	else
		SetGWorld(gw[dstBitmap], nil);

	GetFontInfo(&info);

	*maxAscent	=	info.ascent;

	*maxDescent	=	info.descent;

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
int tb_StringWidth(TBitmap dstBitmap, char s[kStringLength])
// ----------------------------------------------------------------- //

{

	Str255			ps;
	int					w=0;

	tb_CtoPStr255(s,ps);
	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		w=StringWidth(ps);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		w=StringWidth(ps);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return (w);

}

// ----------------------------------------------------------------- //
void tb_DrawNumber(TBitmap dstBitmap, int h, int v, long n, int justify)
// ----------------------------------------------------------------- //

{

	Str255			ps;
	char				s[kStringLength];

	//tb_IntegerToString(n,	s);

	tb_NumToString(n, s);

	if (justify==tb_Left)					h=h;

	if (justify==tb_Center)				h=h-(tb_StringWidth(dstBitmap, s)/2);

	if (justify==tb_Right)				h=h-tb_StringWidth(dstBitmap, s);

	tb_CtoPStr255(s,ps);

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		MoveTo(h,v);
		DrawString(ps);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		MoveTo(h,v);
		DrawString(ps);
	}

	RGBForeColor(&myBlack);

	RGBBackColor(&myWhite);
	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_DrawString(TBitmap dstBitmap, int h, int v, char s[kStringLength])
// ----------------------------------------------------------------- //

{

	Str255			ps;

	tb_CtoPStr255(s,ps);
	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		MoveTo(h,v);
		DrawString(ps);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		MoveTo(h,v);
		DrawString(ps);
	}

	RGBForeColor(&myBlack);

	RGBBackColor(&myWhite);
	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetFontTransfer(TBitmap dstBitmap, int	transfer)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);

		if (transfer==tb_TextOr) 		TextMode(srcOr);

		if (transfer==tb_TextCopy) 	TextMode(srcCopy);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);

		if (transfer==tb_TextOr) 		TextMode(srcOr);

		if (transfer==tb_TextCopy) 	TextMode(srcCopy);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetFontStyle(TBitmap dstBitmap, int	style)
// ----------------------------------------------------------------- //

{
	int			theStyle=style;

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		TextFace(0);

		if (theStyle>=tb_Underline)		{ TextFace(underline);	theStyle=theStyle-tb_Underline;	}

		if (theStyle>=tb_Shadow)			{ TextFace(shadow);			theStyle=theStyle-tb_Shadow;		}

		if (theStyle>=tb_Outline)			{ TextFace(outline);		theStyle=theStyle-tb_Outline;		}

		if (theStyle>=tb_Italic)			{ TextFace(italic);			theStyle=theStyle-tb_Italic;		}

		if (theStyle>=tb_Bold)				{ TextFace(bold);				theStyle=theStyle-tb_Bold;			}
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		TextFace(0);

		if (theStyle>=tb_Underline)		{ TextFace(underline);	theStyle=theStyle-tb_Underline;	}

		if (theStyle>=tb_Shadow)			{ TextFace(shadow);			theStyle=theStyle-tb_Shadow;		}

		if (theStyle>=tb_Outline)			{ TextFace(outline);		theStyle=theStyle-tb_Outline;		}

		if (theStyle>=tb_Italic)			{ TextFace(italic);			theStyle=theStyle-tb_Italic;		}

		if (theStyle>=tb_Bold)				{ TextFace(bold);				theStyle=theStyle-tb_Bold;			}
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetFont(TBitmap dstBitmap, int theFont)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		TextFont(theFont);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		TextFont(theFont);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetFontSize(TBitmap dstBitmap, int theSize)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		TextSize(theSize);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		TextSize(theSize);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetForeColor(TBitmap dstBitmap, short red, short green, short blue)
// ----------------------------------------------------------------- //

{

	//GetGWorld(&tbsavedPort, &tbsavedDevice);
	if (dstBitmap>4999)
	{
		winForeColor[dstBitmap-5000].red=red;
		winForeColor[dstBitmap-5000].green=green;
		winForeColor[dstBitmap-5000].blue=blue;
		//SetPort(win[dstBitmap-5000]);
		//RGBForeColor(&winForeColor[dstBitmap-5000]);
	}
	else
	{
		gwForeColor[dstBitmap].red=red;
		gwForeColor[dstBitmap].green=green;
		gwForeColor[dstBitmap].blue=blue;
		//SetGWorld(gw[dstBitmap], nil);
		//RGBForeColor(&gwForeColor[dstBitmap]);
	}

	//SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetBackColor(TBitmap dstBitmap, unsigned int red, unsigned int green, unsigned int blue)
// ----------------------------------------------------------------- //

{

	//GetGWorld(&tbsavedPort, &tbsavedDevice);
	if (dstBitmap>4999)
	{
		winBackColor[dstBitmap-5000].red=red;
		winBackColor[dstBitmap-5000].green=green;
		winBackColor[dstBitmap-5000].blue=blue;
		//SetPort(win[dstBitmap-5000]);
		//RGBBackColor(&winBackColor[dstBitmap-5000]);
	}
	else
	{
		gwBackColor[dstBitmap].red=red;
		gwBackColor[dstBitmap].green=green;
		gwBackColor[dstBitmap].blue=blue;
		//SetGWorld(gw[dstBitmap], nil);
		//RGBBackColor(&gwBackColor[dstBitmap]);
	}

	//SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
int tb_DoDialog(int id)
// ----------------------------------------------------------------- //

{
	DialogPtr			Dialog;
	short					itemHit=0;

	//GetGWorld(&tbsavedPort, &tbsavedDevice);
	GetPort(&(GrafPtr)tbsavedPort);
	tb_SetCursor(0);
	Dialog = GetNewDialog(id, nil, (WindowPtr)-1L);

	if (Dialog!=NULL)
	{
		SetPortWindowPort(GetDialogWindow(Dialog));
		ShowWindow(GetDialogWindow(Dialog));

		while (itemHit<1)
			ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

		SetGWorld(tbsavedPort, tbsavedDevice);

		DisposeDialog(Dialog);

		return (itemHit);
	}

	//SetGWorld(tbsavedPort, tbsavedDevice);
	//SetPortWindowPort((GrafPtr)tbsavedPort);
	SetPortWindowPort(GetWindowFromPort(tbsavedPort));

	return (itemHit);

}

// ----------------------------------------------------------------- //
int tb_OpenPartyFileForOutput(char filename[kStringLength])
// ----------------------------------------------------------------- //

{
	int					Error;
	long				DirID;
	FSSpec			Spec;
	short				vRefNum;
	Str255			name;


	tb_CtoPStr255(filename,name);
	Error = FindFolder(kOnSystemDisk, 'pref', true, &vRefNum, &DirID);

	if (Error!=noErr) return (-1);

	Error = FSMakeFSSpec(vRefNum, DirID, name, &Spec);

	Error = FSpDelete(&Spec);

	Error = FSpCreate(&Spec, Creator, 'pref', iuSystemScript);

	if (Error!=noErr) return (-1);

	Error = FSpOpenDF(&Spec, fsRdWrShPerm, &refNum);

	if (Error!=noErr) { Error = FSpDelete(&Spec);	return (-1); }

	return (0);
}

// ----------------------------------------------------------------- //
int tb_OpenPartyFileForInput(char filename[kStringLength])
// ----------------------------------------------------------------- //

{
	int					Error;
	long				DirID;
	FSSpec			Spec;
	short				vRefNum;
	Str255			name;

	tb_CtoPStr255(filename,name);
	Error = FindFolder(kOnSystemDisk, 'pref', false, &vRefNum, &DirID);
	Error = FSMakeFSSpec(vRefNum, DirID, name, &Spec);
	Error = FSpOpenDF(&Spec, fsRdPerm, &refNum);

	if (Error!=noErr) { Error = FSpDelete(&Spec);	return (-1); }

	return (0);
}

// ----------------------------------------------------------------- //
int tb_OpenPrefsForOutput(void)
// ----------------------------------------------------------------- //

{
	int				Error;
	long			DirID;
	FSSpec		Spec;
	short				vRefNum;

	Error = FindFolder(kOnSystemDisk, 'pref', true, &vRefNum, &DirID);

	if (Error!=noErr) return (-1);

	Error = FSMakeFSSpec(vRefNum, DirID, PrefsName, &Spec);

	Error = FSpDelete(&Spec);

	Error = FSpCreate(&Spec, Creator, 'pref', iuSystemScript);

	if (Error!=noErr) return (-1);

	Error = FSpOpenDF(&Spec, fsRdWrShPerm, &refNum);

	if (Error!=noErr) { Error = FSpDelete(&Spec);	return (-1); }

	return (0);
}

// ----------------------------------------------------------------- //
int tb_OpenPrefsForInput(void)
// ----------------------------------------------------------------- //

{
	int				Error;
	long			DirID;
	FSSpec		Spec;
	short			vRefNum;

	Error = FindFolder(kOnSystemDisk, 'pref', false, &vRefNum, &DirID);
	Error = FSMakeFSSpec(vRefNum, DirID, PrefsName, &Spec);
	Error = FSpOpenDF(&Spec, fsRdPerm, &refNum);

	if (Error!=noErr) { Error = FSpDelete(&Spec);	return (-1); }

	return (0);
}

// ----------------------------------------------------------------- //
void tb_Beep(int sound)
// ----------------------------------------------------------------- //

{
	NMRec	 	theNot;
	Handle		theHnd;

	theNot.qType=nmType;
	theNot.nmMark=1;
	theNot.nmIcon=NULL;

	if (sound==0)
		theNot.nmSound=NULL;
	else
	{
		theHnd=GetResource('snd ', sound);

		if (theHnd==NULL)
			SysBeep(0);

		theNot.nmSound=theHnd;
	}

	theNot.nmStr=NULL;

	theNot.nmResp=NULL;

	if (applicationSuspended && backgroundnotification)
	{
		if (NMInstall(&theNot)==noErr)
		{
			if (sound==0)
				SysBeep(0);

			NMRemove(&theNot);

			if (sound!=0)
				ReleaseResource(theHnd);
		}
	}
	else
	{
		if (sound==0)
			SysBeep(0);
		else
			tb_PlaySound(sound);
	}
}

// ----------------------------------------------------------------- //
void tb_AlertMessage(char alertMess[kStringLength])
// ----------------------------------------------------------------- //

{

	DialogPtr						d;
	short								itemHit=0;
	Str255							s;

	//DrawEverything();
	GetGWorld(&tbsavedPort, &tbsavedDevice);
	tb_CtoPStr255(alertMess,s);
	SysBeep(0);
	tb_SetCursor(0);
	d = GetNewDialog(132, nil, (WindowPtr)-1L);
	SetPortWindowPort(GetDialogWindow(d));
	ShowWindow(GetDialogWindow(d));
	ParamText(s,"\p","\p","\p");

	while (itemHit!=1)
		ModalDialog(nil, &itemHit);

	//ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);
	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(d);

}

// ----------------------------------------------------------------- //
void tb_AlertMessage2(char alertMess1[kStringLength],char alertMess2[kStringLength])
// ----------------------------------------------------------------- //

{

	DialogPtr						d;
	short								itemHit=0;
	Str255							s;
	Str255							t;

	//DrawEverything();

	GetGWorld(&tbsavedPort, &tbsavedDevice);
	tb_CtoPStr255(alertMess1,s);
	tb_CtoPStr255(alertMess2,t);
	tb_SetCursor(0);
	d = GetNewDialog(163, nil, (WindowPtr)-1L);
	SetPortWindowPort(GetDialogWindow(d));
	ShowWindow(GetDialogWindow(d));
	ParamText(s,t,"\p","\p");

	while (itemHit!=1)
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(d);

}

// ----------------------------------------------------------------- //
int tb_AlertMessage3(int id,char alertMess[kStringLength])
// ----------------------------------------------------------------- //

{

	DialogPtr						d;
	short								itemHit=0;
	Str255							s;

	//DrawEverything();
	GetGWorld(&tbsavedPort, &tbsavedDevice);
	tb_CtoPStr255(alertMess,s);
	SysBeep(0);
	tb_SetCursor(0);
	d = GetNewDialog(id, nil, (WindowPtr)-1L);
	SetPortWindowPort(GetDialogWindow(d));
	ShowWindow(GetDialogWindow(d));
	ParamText(s,"\p","\p","\p");

	while (itemHit<1)
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(d);

	return (itemHit);

}

// ----------------------------------------------------------------- //
void tb_SetWindowTitle(int theWindow, char WindowTitle[kStringLength])
// ----------------------------------------------------------------- //

{

	Str255			Title;
	Str255			OldName;

	tb_CtoPStr255(WindowTitle,Title);
	GetWTitle(win[theWindow-5000],OldName);

	if (tb_PascalStringsEqual(OldName,Title)) return;

	SetWTitle(win[theWindow-5000],Title);

}

// ----------------------------------------------------------------- //
int tb_OpenOutputFile(char name[kStringLength], TBoolean PromptForName)
// ----------------------------------------------------------------- //

{

	Str255								filename;
	StandardFileReply			reply;
	FSSpec								temp;

	tb_CtoPStr255(name,filename);

	if ((PromptForName==true) || (tb_PascalStringsEqual(filename,"\pUntitled")))
	{
		//StandardPutFile("\pSave As?", filename, &reply);
		if (reply.sfGood)
		{
			currentFile=reply;

			if (reply.sfReplacing==true)
			{
				temp=currentFile.sfFile;
				Error=FSpDelete(&temp);

				if (Error!=0) { tb_AlertMessage("Sorry, but I was unable to replace the selected file.  Try using a different filename."); return (-1); }
			}

			Error=FSpCreate(&currentFile.sfFile, Creator, FileType, currentFile.sfScript);

			if (Error==dskFulErr) { tb_AlertMessage("Sorry, but I was unable to save the game because the disk is full."); return (-1); }

			if ((Error==wPrErr) || (Error==vLckdErr)) { tb_AlertMessage("Sorry, but I was unable to save the game because the disk is locked."); return (-1); }

			if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to save the game because a disk error occured."); return (-1);	}

			Error=FSpOpenDF(&currentFile.sfFile, fsRdWrPerm, &refNum);

			if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to save the game because a disk error occured."); return (-1);	}

			tb_PascaltoCStr(currentFile.sfFile.name,name);
		}
		else
			return (-1);
	}
	else
		FSMakeFSSpec(0, 0, "\pobie world 2", &temp);

	Error=FSpCreate(&temp, Creator, FileType, smSystemScript);

	if (Error==dskFulErr) { tb_AlertMessage("Sorry, but I was unable to save the game because the disk is full."); return (-1); }

	if ((Error==wPrErr) || (Error==vLckdErr)) { tb_AlertMessage("Sorry, but I was unable to save the game because the disk is locked."); return (-1); }

	if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to save the game because a disk error occured."); return (-1);	}

	Error=FSpOpenDF(&temp, fsRdWrPerm, &refNum);

	if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to save the game because a disk error occured."); return (-1);	}

	return (1);

}

// ----------------------------------------------------------------- //
int tb_OpenNamedInputFile(char filename[kStringLength])
// ----------------------------------------------------------------- //

{
	//SFTypeList						typeList;
	//StandardFileReply			reply;
	Str255								name;
	//short 								vRefNum;
	//long 									dirID;


//	AppRefNum = CurResFile();
//	FSpGetFileLocation(AppRefNum, &AppFSSpec);
//	tbCurrentvRefNum	=	AppFSSpec.vRefNum;
//	tbCurrentDirID		=	AppFSSpec.parID;



	tb_CtoPStr255(filename,name);
	//Error = HGetVol(NULL, &vRefNum, &dirID);
	FSMakeFSSpec(AppFSSpec.vRefNum, AppFSSpec.parID, name, &currentFile.sfFile);
	Error=FSpOpenDF(&currentFile.sfFile, fsRdPerm, &refNum);

	if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to open that game file because a disk error occured."); return (-1);	}

	tb_PascaltoCStr(currentFile.sfFile.name,filename);

	return (1);

}

// ----------------------------------------------------------------- //
int tb_OpenInputFile(char */*filename[kStringLength]*/)
// ----------------------------------------------------------------- //

{
	// gs carbon
	/*
		SFTypeList						typeList;
		StandardFileReply			reply;

		typeList[0] = FileType;
		StandardGetFile(nil, 1, typeList, &reply);
		if (reply.sfGood)
			{
				currentFile=reply;
				Error=FSpOpenDF(&currentFile.sfFile, fsRdPerm, &refNum);
					if (Error!=noErr) { tb_AlertMessage("Sorry, but I was unable to open that game file because a disk error occured."); return (-1);	}
				tb_PascaltoCStr(currentFile.sfFile.name,filename);
			}
		else
			return (-1);

		return (1);
	*/
}

// ----------------------------------------------------------------- //
void tb_CloseFile(void)
// ----------------------------------------------------------------- //

{

	Error = FSClose(refNum);

}

// ----------------------------------------------------------------- //
int tb_WriteToFile(void	*d, long s)
// ----------------------------------------------------------------- //

{

	//if (FileCheck!=-1) return (1);

	Error = FSWrite(refNum, &s, d);

	if (Error==noErr) return (0);

	//FileCheck=0;
	if (Error==dskFulErr)
		{ tb_AlertMessage("Sorry, but I was unable to save the game file because the selected disk is full.");	return (1); }

	tb_AlertMessage("Sorry, but I was unable to save the game file because a disk error occured.");

	return (1);

}

// ----------------------------------------------------------------- //
int tb_ReadFromFile(void	*d, long s)
// ----------------------------------------------------------------- //

{
	OSErr					e;

	e=FSRead(refNum, &s, d);

	if (e!=noErr)
		return(1);

	return (0);

}

// ----------------------------------------------------------------- //
unsigned long tb_CurrentTick(void)
// ----------------------------------------------------------------- //

{

	return (TickCount());

}

// ----------------------------------------------------------------- //
void tb_WaitTick(unsigned long	t)
// ----------------------------------------------------------------- //

{
	unsigned long					dummy;


	Delay(t,&dummy);

}

// ----------------------------------------------------------------- //
unsigned long tb_CurrentMillisecond(void)
// ----------------------------------------------------------------- //

{
	register double    	ms;
	unsigned long				result=0;
	UnsignedWide				myTime;

	Microseconds(&myTime);

	//ms = ((((double) myTime->hi) * (4294967296.0)) + myTime->lo)/1000;
	ms = ((((double) myTime.hi) * (4294967296.0)) + myTime.lo)/1000;
	result=((unsigned long)ms);

	if (result>(ULONG_MAX-4000)) result=0;

	return (result);

}

// ----------------------------------------------------------------- //
void tb_Wait(unsigned long	ms)
// ----------------------------------------------------------------- //

{

	unsigned long myStart	=	tb_CurrentMillisecond();
	unsigned long	myEnd		=	myStart+ms;;

	do
	{
		tb_Idle();
	}
	while (tb_CurrentMillisecond()<myEnd);

}

// ----------------------------------------------------------------- //
void tb_SetClip(TBitmap dstBitmap, TRect *crect)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		ClipRect(crect);
	}
	else
	{
		//LockPixels(GetGWorldPixMap(gw[dstBitmap]));
		SetGWorld(gw[dstBitmap], nil);
		ClipRect(crect);
		//UnlockPixels(GetGWorldPixMap(gw[dstBitmap]));
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
void tb_Rectangle(TBitmap Bitmap, TRect *drect)
// ----------------------------------------------------------------- //

{

#if (Debug==true)

	if ((Bitmap<5000) && (gwFree[Bitmap]==true))
	{
		tb_DisplayError("tb_Rectangle:  Illegal destination bitmap.");
		return;
	}

	if ((Bitmap>4999) && (winFree[Bitmap-5000]==true))
	{
		tb_DisplayError("tb_Rectangle:  Illegal destination window.");
		return;
	}

	if ((Bitmap<5000) && ((Bitmap<0) || (Bitmap>=MaxGWorlds)))
	{
		tb_DisplayError("tb_Rectangle:  Illegal destination bitmap bounds.");
		return;
	}

	if ((Bitmap>=MaxGWorlds) && ((Bitmap<5000) || (Bitmap>=5000+MaxWindows)))
	{
		tb_DisplayError("tb_Rectangle:  Illegal destination windows bounds.");
		return;
	}

#endif

	if (Bitmap>4999)
	{

#if (Debug==true)

		if (win[Bitmap-5000]==NULL)
		{
			tb_DisplayError("tb_Rectangle:  Null window.");
			return;
		}

#endif

		GetGWorld(&tbsavedPort, &tbsavedDevice);

		SetPortWindowPort(win[Bitmap-5000]);

		RGBForeColor(&myWhite);

		PaintRect(drect);

		RGBForeColor(&myBlack);

		RGBBackColor(&myWhite);

		SetGWorld(tbsavedPort, tbsavedDevice);

	}
	else
	{

#if (Debug==true)

		if (gw[Bitmap]==NULL)
		{
			tb_DisplayError("tb_Rectangle:  Null GWorld.");
			return;
		}

#endif

		GetGWorld(&tbsavedPort, &tbsavedDevice);

		//LockPixels(GetGWorldPixMap(gw[Bitmap]));
		SetGWorld(gw[Bitmap], nil);

		RGBForeColor(&myWhite);

		PaintRect(drect);

		//UnlockPixels(GetGWorldPixMap(gw[Bitmap]));
		RGBForeColor(&myBlack);

		RGBBackColor(&myWhite);

		SetGWorld(tbsavedPort, tbsavedDevice);

	}

	return;

}

// ----------------------------------------------------------------- //
void tb_Line(TBitmap dstBitmap, int h1, int v1, int h2, int v2, int width)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		PenSize(width,width);
		MoveTo(h1,v1);
		LineTo(h2,v2);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		PenSize(width,width);
		MoveTo(h1,v1);
		LineTo(h2,v2);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
void tb_EraseRect(TBitmap dstBitmap, TRect *drect)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		EraseRect(drect);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		EraseRect(drect);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
void tb_PaintRect(TBitmap dstBitmap, TRect *drect)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		PaintRect(drect);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}
	else
	{
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		PaintRect(drect);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
void tb_FillRect(TBitmap dstBitmap, short PatternID, TRect *drect)
// ----------------------------------------------------------------- //

{
	PixPatHandle						pat;


	pat = GetPixPat(PatternID);

	if (pat!=nil)
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);

		if (dstBitmap>4999)
		{
			SetPortWindowPort(win[dstBitmap-5000]);
			RGBForeColor(&winForeColor[dstBitmap-5000]);
			RGBBackColor(&winBackColor[dstBitmap-5000]);
			FillCRect(drect, pat);
			RGBForeColor(&myBlack);
			RGBBackColor(&myWhite);
		}
		else
		{
			SetGWorld(gw[dstBitmap], nil);
			RGBForeColor(&gwForeColor[dstBitmap]);
			RGBBackColor(&gwBackColor[dstBitmap]);
			PaintRect(drect);
			FillCRect(drect, pat);
			RGBForeColor(&myBlack);
			RGBBackColor(&myWhite);
		}

		SetGWorld(tbsavedPort, tbsavedDevice);

		DisposePixPat(pat);
	}

	return;

}

// ----------------------------------------------------------------- //
void tb_FrameRect(TBitmap dstBitmap, TRect *drect)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&winForeColor[dstBitmap-5000]);
		RGBBackColor(&winBackColor[dstBitmap-5000]);
		FrameRect(drect);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}
	else
	{
		//LockPixels(GetGWorldPixMap(gw[dstBitmap]));
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&gwForeColor[dstBitmap]);
		RGBBackColor(&gwBackColor[dstBitmap]);
		FrameRect(drect);
		//UnlockPixels(GetGWorldPixMap(gw[dstBitmap]));
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return;

}

// ----------------------------------------------------------------- //
void tb_GetLocalMousePosition(TBitmap theWindow, TPoint *pt)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (theWindow>4999)
	{
		SetPortWindowPort(win[theWindow-5000]);
		GetMouse(pt);
	}
	else
	{
		pt->h=0;
		pt->v=0;
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
TBoolean tb_LocalPointInContent(TBitmap theWindow, TPoint pt)
// ----------------------------------------------------------------- //

{
	WindowPtr         wp;
	short							windowPart;
	int               match=-1;
	TBoolean          mouseInContent=false;
	TPoint            globalPt;

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (theWindow>4999)
	{
		SetPortWindowPort(win[theWindow-5000]);
		globalPt=pt;
		LocalToGlobal(&globalPt);
		windowPart = FindWindow(globalPt, &wp);
		match=tb_GetWindowMatch(wp);

		if (match!=theWindow)
			mouseInContent=false;
		else if (windowPart==inContent)
			mouseInContent=true;
		else
			mouseInContent=false;
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

	return (mouseInContent);

}

// ----------------------------------------------------------------- //
int tb_Trunc(double x)
// ----------------------------------------------------------------- //

{

	long y;

	y = x;
	return(y);

}

// ----------------------------------------------------------------- //
//int tb_Round(double x)
// ----------------------------------------------------------------- //

//{

//	long y;

//	y = x;
//	x = x - y;

//	if(x > .5)
//		return(y + 1);
//	else
//		return(y);

//	return ((int)(x+0.5));

//}

// ----------------------------------------------------------------- //
void tb_Idle(void)
// ----------------------------------------------------------------- //

{
	EventRecord			theEvent;

	if (usingX)
		WaitNextEvent(0, &theEvent, 1, NULL);

}

// ----------------------------------------------------------------- //
void tb_DisplayError(char errMess[kStringLength])
// ----------------------------------------------------------------- //

{

	DialogPtr						d;
	short								itemHit=0;
	Str255							s;

	GetGWorld(&tbsavedPort, &tbsavedDevice);
	tb_CtoPStr255(errMess,s);
	SysBeep(0);
	tb_SetCursor(0);
	d = GetNewDialog(128, nil, (WindowPtr)-1L);
	SetPortWindowPort(GetDialogWindow(d));
	ShowWindow(GetDialogWindow(d));
	ParamText(s,"\p","\p","\p");

	while (itemHit!=1)
		ModalDialog(nil, &itemHit);

	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(d);

	//tb_Halt();

}

// ----------------------------------------------------------------- //
void tb_SetCursor(short id)
// ----------------------------------------------------------------- //

{
	int			i;

//	if (id==previousCursor)
//		return;

	if (id==140)
		tb_Idle();

	if (id==0)
	{
		InitCursor();
		previousCursor=0;
		return;
	}

	for (i=0;i<MaxCursors;i++)
		if (curID[i]==id)
			if (cur[i]!=nil)
			{
				SetCCursor(cur[i]);
				previousCursor=id;
				return;
			};

	for (i=0;i<MaxCursors;i++)
		if (curID[i]==0)
		{
			cur[i]=GetCCursor(id);

			if (cur[i]!=nil)
			{
				curID[i]=id;
				SetCCursor(cur[i]);
				previousCursor=id;
			}

			return;
		}

	previousCursor=-1;

}

// ----------------------------------------------------------------- //
void tb_PlaySound(int id)
// ----------------------------------------------------------------- //

{

	if (applicationSuspended)
		return;

	//if (tb_SoundOn==true)
	if (id!=0)
		PlayAsyncSound(id);

}

// ----------------------------------------------------------------- //
//int tb_Rnd (int min, int max)
// ----------------------------------------------------------------- //

//{

//	return (((abs(Random())%(max-min+1))+min));

//}

// ----------------------------------------------------------------- //
void tb_GetClientRect(TWindow theWindow, TRect */*crect*/)
// ----------------------------------------------------------------- //

{

#if (Debug==true)

	if ((theWindow<5000))
	{
		tb_DisplayError("tb_GetClientRect:  Illegal window reference.");
		return;
	}

	if (winFree[theWindow-5000]==true)
	{
		tb_DisplayError("tb_GetClientRect:  Window is free.");
		return;
	}

#endif

	//*crect= win[theWindow-5000]->portRect; // gs carbon

}

// ----------------------------------------------------------------- //
void tb_GetWindowRect(TWindow theWindow, TRect *wrect)
// ----------------------------------------------------------------- //

{
	OSStatus					Error;
	char							debugStr[kStringLength];

#if (Debug==true)

	if ((theWindow<5000))
	{
		tb_DisplayError("tb_GetWindowRect:  Illegal window reference.");
		return;
	}

	if (winFree[theWindow-5000]==true)
	{
		tb_DisplayError("tb_GetWindowRect:  Window is free.");
		return;
	}

#endif

	Error=GetWindowBounds(win[theWindow-5000],kWindowStructureRgn,wrect);

	if (Error!=noErr)
	{
		tb_IntegerToString(Error,debugStr);
		fi_WriteToErrorLog(debugStr);
	}

	//							 	*wrect = (*(((WindowPeek)win[theWindow-5000])->strucRgn))->rgnBBox;
	//tb_OffsetRect(wrect,-wrect->left,-wrect->top);

	//*wrect = ((((WindowPeek)win[theWindow-5000])->port.portRect));

}

// ----------------------------------------------------------------- //
TBoolean tb_IsButtonDown(void)
// ----------------------------------------------------------------- //

{

	return (Button());

}

// ----------------------------------------------------------------- //
void tb_GetMonitorRect(TRect *r)
// ----------------------------------------------------------------- //

{

	*r = screenBounds;

}

// ----------------------------------------------------------------- //
TBoolean tb_IsKeyDown (short KeyCode)
// ----------------------------------------------------------------- //

{
	KeyMap		myKeyMap;
	short			byteIndex;
	char			theByte, theBit;
	char			*thePointer;

	GetKeys(myKeyMap);
	byteIndex = KeyCode >> 3;
	thePointer =(char*)&myKeyMap[0];
	theByte = *(char*)(thePointer + byteIndex);
	theBit = 1L << (KeyCode & 7);
	return ((theByte & theBit) != 0);

}

// ----------------------------------------------------------------- //
int tb_InsertTheMenu(char *MenuTitle)
// ----------------------------------------------------------------- //

{

	int					i;
	int					found=-1;
	Str255			Title;

	tb_CtoPStr255(MenuTitle,Title);

	for (i=1; i<10; i++)
		if (MenuFree[i]==true)
		{
			found=i;
			break;
		}

	if (found==-1) return(0);

	theMenus[found]=NewMenu(128+found,Title);

	InsertMenu(theMenus[found], 0);

	DrawMenuBar();

	MenuFree[found]=false;

	return (128+found);

}

// ----------------------------------------------------------------- //
int tb_InsertTheMenuItem(int theMenu,char *MenuItemTitle,char Key)
// ----------------------------------------------------------------- //

{

	Str255			Title;

	tb_CtoPStr255(MenuItemTitle,Title);
	InsertMenuItem(theMenus[theMenu-128],Title,1000);

	if (Key!=' ')
		SetItemCmd(theMenus[theMenu-128],CountMenuItems(theMenus[theMenu-128]),(short)Key);

	return ((100*theMenu)+CountMenuItems(theMenus[theMenu-128]));

}

// ----------------------------------------------------------------- //
void tb_Halt(void)
// ----------------------------------------------------------------- //

{
	fi_WriteToErrorLog("tb_halt");

	ExitToShell();

}

// ----------------------------------------------------------------- //
int tb_SetWindowPtr(WindowPtr w)
// ----------------------------------------------------------------- //

{

	int					i;
	int					found=-1;

	for (i=0; i<MaxWindows; i++)
		if (winFree[i])
		{
			found=i;
			break;
		}

	if (found==-1) return (WindowError);

	winFree[found]=false;

	win[found]=w;

	return (found+5000);

}

// ----------------------------------------------------------------- //
int tb_OpenWindow(TRect *r, char *WindowTitle)
// ----------------------------------------------------------------- //

{

	int					i;
	int					found=-1;
	TRect				t=*r;
	Str255			Title;
	TBoolean			DragBar=true;

	tb_CtoPStr255(WindowTitle,Title);

	for (i=0; i<MaxWindows; i++)
		if (winFree[i])
		{
			found=i;
			break;
		}

	if (found==-1) return (WindowError);

	if (t.top<GetMBarHeight())
		OffsetRect(&t,0,GetMBarHeight()+18);

	if (DragBar==true)
		win[found] = NewCWindow(nil, &t, Title, true, noGrowDocProc, (WindowPtr)-1L, false, 0);
	else
		win[found] = NewCWindow(nil, &t, Title, true, plainDBox, (WindowPtr)-1L, false, 0);

	SetPortWindowPort(win[found]);

	ShowWindow(win[found]);

	SelectWindow(win[found]);

	winRect[found]=t;

	winFree[found]=false;

	return (found+5000);

}

// ----------------------------------------------------------------- //
void tb_GetBitmapRect(TBitmap Bitmap, TRect *r)
// ----------------------------------------------------------------- //

{

	*r = gwRect[Bitmap];

}

// ----------------------------------------------------------------- //
int tb_GetBitmapWidth(TBitmap Bitmap)
// ----------------------------------------------------------------- //

{

	if (gwFree[Bitmap]==true) return (-1);

	return (gwRect[Bitmap].right-gwRect[Bitmap].left+1);

}

// ----------------------------------------------------------------- //
int tb_GetBitmapHeight(TBitmap Bitmap)
// ----------------------------------------------------------------- //

{

	if (gwFree[Bitmap]==true) return (-1);

	return (gwRect[Bitmap].bottom-gwRect[Bitmap].top+1);


}

// ----------------------------------------------------------------- //
void tb_CopyORMaskBitmapStd(TBitmap /*srcBitmap*/, TBitmap /*dstBitmap*/, TRect */*srcRect*/, TRect */*dstRect*/)
// ----------------------------------------------------------------- //

{
	/*
	GetGWorld(&tbsavedPort, &tbsavedDevice);
	if (dstBitmap>4999)
		{
			SetPortWindowPort(win[dstBitmap-5000]);
			RGBForeColor(&myBlack);
			RGBBackColor(&myWhite);
			CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )), srcRect, dstRect, srcOr, NULL );
		}
	else
		{
			#if (Debug==true)
				if (gw[srcBitmap]==NULL)
					{
						tb_DisplayError("tb_CopyBitmap:  Null source bitmap.");
						SetGWorld(tbsavedPort, tbsavedDevice);
						return;
					}
				if (gw[dstBitmap]==NULL)
					{
						tb_DisplayError("tb_CopyBitmap:  Null destination bitmap.");
						SetGWorld(tbsavedPort, tbsavedDevice);
						return;
					}
			#endif
			SetGWorld(gw[dstBitmap], nil);
			CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits( gw[dstBitmap] ), srcRect, dstRect, srcOr, NULL );
		}
	SetGWorld(tbsavedPort, tbsavedDevice);
	*/
}

// ----------------------------------------------------------------- //
void	tb_GetPixelRGB(int bitmap, int x, int y, unsigned int* r, unsigned int* g, unsigned int* b)
// ----------------------------------------------------------------- //

{

	/*if (bitmap>4999)
		{
			RGBColor			color;
			
			GetGWorld(&tbsavedPort, &tbsavedDevice);
			SetPortWindowPort(win[bitmap-5000]);
			GetCPixel(x,y, &color);
			*r=color.red;
			*g=color.green;
			*b=color.blue;
			SetGWorld(tbsavedPort, tbsavedDevice);
		}
	else
		{*/
	UInt16				*myPixelPtr;
	//char				debug[255], debug2[255], debug3[255];



	myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (y * gwRowBytes[bitmap]) + (x * 2));

	*r = (*myPixelPtr << 1) & 0xF800;
	*g = (*myPixelPtr << 6) & 0xF800;
	*b = (*myPixelPtr << 11) & 0xF800;


	//}

}

// ----------------------------------------------------------------- //
void	tb_GetPixelGray(int bitmap, int x, int y, unsigned int* g)
// ----------------------------------------------------------------- //

{

	UInt16* 				myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (y * gwRowBytes[bitmap]) + (x * 2));

	*g = (*myPixelPtr << 1) & 0xF800;

}

// ----------------------------------------------------------------- //
void	tb_SetWindowPixelRGB(int bitmap, int x, int y, unsigned int r, unsigned int g, unsigned int b)
// ----------------------------------------------------------------- //

{
	RGBColor			color;

	GetGWorld(&tbsavedPort, &tbsavedDevice);
	SetPortWindowPort(win[bitmap-5000]);
	color.red		=	r;
	color.green	=	g;
	color.blue	=	b;
	SetCPixel(x,y, &color);
	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void	tb_SetPixelRGB(int bitmap, int x, int y, unsigned int r, unsigned int g, unsigned int b)
// ----------------------------------------------------------------- //

{
	UInt16* 				myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (y * gwRowBytes[bitmap]) + (x * 2));

	if (x<=gwRect[bitmap].left)		return;

	if (x>=gwRect[bitmap].right)		return;

	if (y<=gwRect[bitmap].top)			return;

	if (y>=gwRect[bitmap].bottom)	return;

	*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

}

// ----------------------------------------------------------------- //
void	tb_ColorizeWorkSpace(int workBMP, int color, TRect	*r)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 						c						=	0;
//	unsigned int						r;
	unsigned int						g;
	unsigned int						b;
	int											x;
	int											y;
//   	for (x=0;x<64;x++)
//   		for (y=0;y<103;y++)

	GetKeyEvents();

	if (playerDead)
		return;

	if ((workBMP<0) || (workBMP>=MaxGWorlds)) // mod 8/4
	{
		fi_WriteToErrorLog("Work BMP out of range");
		return;
	}

	if (gwFree[workBMP]) // mod 8/4
	{
		fi_WriteToErrorLog("Work BMP free");
		return;
	}

	if (r->right>gwRect[workBMP].right)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace width out of range");
		return;
	}

	if (r->left<0)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height less than zero");
		return;
	}

	if (r->bottom>gwRect[workBMP].bottom)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height out of range");
		return;
	}

	if (r->top<0)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height less than zero");
		return;
	}

	if (color==kLightBlue)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = ((((int)(c*0.42) >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((c >> 11) & 0x1F);
			}

		return;
	}

	if (color==kRed)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((c >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kGreen)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kBlue)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((c >> 11) & 0x1F);
			}

		return;
	}

	if (color==kBrown)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

			//old	//*myPixelPtr = ((((int)(c*0.85) >> 11) & 0x1F) << 10) | ((((int)(c*0.64) >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
				*myPixelPtr = ((((int)(c*1.00) >> 11) & 0x1F) << 10) | ((((int)(c*0.50) >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			//*myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			//*myPixelPtr = ((((int)(c*0.9) >> 11) & 0x1F) << 10)|((((int)(c*0.5) >> 11) & 0x1F) << 10)| ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kBlack)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = ((((int)(c*0.2) >> 11) & 0x1F) << 10) | ((((int)(c*0.2) >> 11) & 0x1F) << 5) | (((int)(c*0.2) >> 11) & 0x1F);
			}

		return;
	}

	if (color==kPurple)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((c >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((c >> 11) & 0x1F);
			}

		return;
	}

	if (color==kYellow)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((c >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}
	
	//Dave color 01-15-2013
	
	if (color==kOrange)
	{
		for (x=r->left;x<r->right;x++)
			for (y=r->top;y<r->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = ((((int)(c*1.0) >> 1) & 0x1F) << 10) | ((((int)(c*0.5) >> 6) & 0x1F) << 5) | (((int)(c*0.0) >> 11) & 0x1F);
			}

		return;
	}
/**/
	//Dave
		

	/*	for (x=0;x<64;x++)
			for (y=0;y<103;y++)
				{
					myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));

					r = (*myPixelPtr << 1) & 0xF800;
					g = (*myPixelPtr << 6) & 0xF800;
					b = (*myPixelPtr << 11) & 0xF800;

					if (!((r>65000) && (g>65000) && (b>65000)))
						{
							if (color==kLightBlue)
								 *myPixelPtr = ((((int)(r*0.42) >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);
							else if (color==kRed)
								 *myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
							else if (color==kGreen)
								 *myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
							else if (color==kBlue)
								 *myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);
							else if (color==kBrown)
								 *myPixelPtr = ((((int)(r*0.85) >> 11) & 0x1F) << 10) | ((((int)(g*0.64) >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
							else if (color==kBlack)
								 *myPixelPtr = ((((int)(r*0.2) >> 11) & 0x1F) << 10) | ((((int)(g*0.2) >> 11) & 0x1F) << 5) | (((int)(b*0.2) >> 11) & 0x1F);
							else if (color==kPurple)
								 *myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);
							else if (color==kYellow)
								 *myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);

						}
				}
	*/
}

// ----------------------------------------------------------------- //
void	tb_ColorizeCreature(int workBMP, int color, TRect *a)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int						r;
	unsigned int						g;
	unsigned int						b;
	unsigned int 						c;
	int											x;
	int											y;


	GetKeyEvents();

	if (playerDead)
		return;

	if (a->right>250)   return;

	if (a->bottom>250)  return;

	if ((workBMP<0) || (workBMP>=MaxGWorlds)) // mod 8/4
	{
		fi_WriteToErrorLog("Creature work BMP out of range");
		return;
	}

	if (gwFree[workBMP]) // mod 8/4
	{
		fi_WriteToErrorLog("Creature work BMP free");
		return;
	}

	if (a->right>gwRect[workBMP].right)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace width out of range");
		return;
	}

	if (a->left<0)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height less than zero");
		return;
	}

	if (a->bottom>gwRect[workBMP].bottom)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height out of range");
		return;
	}

	if (a->top<0)
	{
		fi_WriteToErrorLog("tb_ColorizeWorkSpace height less than zero");
		return;
	}

	if (color==kBlackWolfColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				c=c*0.2;

				*myPixelPtr = (((c >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((c >> 11) & 0x1F);
			}

		return;
	}

	if (color==kBrownWolfColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c*1.212;

				b=c*0.635;

				if (r>65535) r=65535;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);
			}

		return;
	}

	if (color==kBlackDragonColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				//r=c*0.001;
				//r=c;
				r = c >> 1;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((r >> 11) & 0x1F) << 5) | ((r >> 11) & 0x1F);
			}

		return;
	}

	if (color==kRedDragonColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c*1.75;

				if (r>65535) r=65535;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((0 >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kPinkDragonColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c*2.2;

				g=c*1.75;

				if (r>65535) r=65535;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((g >> 11) & 0x1F);
			}

		return;
	}

	if (color==kPoisonElementalColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				*myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kWaterElementalColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c/5;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((c >> 11) & 0x1F);
			}

		return;
	}

	if (color==kFireElementalColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c*1.75;

				if (r>65535) r=65535;

				g=c/3;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kIceDragonColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c/2;

				g=c*1.75;

				if (g>65535) g=65535;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((g >> 11) & 0x1F);
			}

		return;
	}

	if (color==kForestDragonColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				//r=0;
				g=c;

				//if (g>65535) g=65535;
				*myPixelPtr = (((0 >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
	}

	if (color==kSandSpiderColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				//r=c*0.48;
				g=c*0.64;

				b=c*0.8;

				c=c*1.75;

				if (c>65535) c=65535;

				*myPixelPtr = (((c >> 11) & 0x1F) << 10) | (((c >> 11) & 0x1F) << 5) | ((0 >> 11) & 0x1F);
			}

		return;
		
		}
		//Dave03-23-2010
		/*	
		if (color==kCreatureBlueColor)
	{
		for (x=0;x<a->right;x++)
			for (y=0;y<a->bottom;y++)
			{
				myPixelPtr 	= (UInt16*) (gwBaseAddress[workBMP] + (y * gwRowBytes[workBMP]) + (x * 2));
				c = (*myPixelPtr << 1) & 0xF800;

				if (c>63000) continue;

				r=c/2;

				g=c*1.75;

				if (g>65535) g=65535;

				*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((g >> 11) & 0x1F);
			}

		return;
	}
*/
		//Dave^

}

// ----------------------------------------------------------------- //
void	tb_RenderLightSource(int bitmap, int x, int y, int radius, int gameTime)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
//	unsigned int* 					rp					=	nil;
//	unsigned int* 					gp					=	nil;
//	unsigned int* 					bp					=	nil;
	unsigned int 						r						=	0;
	unsigned int 						g						=	0;
	unsigned int 						b						=	0;
	int											xi;
	int											yi;
	float										rn;
	float										d;
	float										intensity;
	long										maxDistance;
	float										falloff1;
	float										falloff2;
	float										falloff3;


// if (inDungeon)
// 	gameTime=1;

	if (nightVision)
		gameTime=0;

	if (inDungeon)
		gameTime=1;

	if ((gameTime==1) || (gameTime==2) || (gameTime==3))
		radius=radius*1.5;

	intensity=(float)tb_Rnd(1,25)/100;

	maxDistance		=	radius*radius;

	falloff1			= maxDistance*0.375;

	falloff2			= maxDistance*0.625;

	falloff3			= maxDistance*0.875;

	for (xi=(x-radius);xi<(x+radius);xi++)
		for (yi=(y-radius);yi<(y+radius);yi++)
		{
			if (xi<=gwRect[bitmap].left)		continue;

			if (xi>=gwRect[bitmap].right)		continue;

			if (yi<=gwRect[bitmap].top)			continue;

			if (yi>=gwRect[bitmap].bottom)	continue;

			d=((x-xi)*(x-xi))+((y-yi)*(y-yi));

			if (d>maxDistance) 							continue;

			if ((gameTime==1) || (gameTime==2) || (gameTime==3))
			{
				rn=(0.965+(1/(d/radius))+intensity)*1.8;

				if (rn>2.5) rn=2.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.75;

				if (d>falloff3)	rn=rn*0.65;

				if (rn<1)				rn=1;
			}
			else
			{
				rn=0.965+(1/(d/radius))+intensity;

				if (rn>1.5) rn=1.5;

				if (d>falloff1)	rn=rn*0.9;

				if (d>falloff2)	rn=rn*0.9;

				if (d>falloff3)	rn=rn*0.9;

				if (rn<1)				rn=1;
			}

			myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			r = (*myPixelPtr << 1) & 0xF800;
			g = (*myPixelPtr << 6) & 0xF800;
			b = (*myPixelPtr << 11) & 0xF800;

			r=r*(0.2+rn);
			g=g*rn;
			b=b*rn;

			if (r>65535)	r=65535;

			if (g>65535)	g=65535;

			if (b>65535)	b=65535;

			*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

		}

}

// ----------------------------------------------------------------- //
void	tb_DarkenBitmap(int bitmap, TRect *s, int level)
// ----------------------------------------------------------------- //

{
	const unsigned long 		kLongMask = (30L << 26) | (30L << 21) | (30L << 16) | (30L << 10) | (30L << 5) | 30L;
	UInt32* 								myPixelPtr 	= nil;
	int											xi;
	int											yi;
	int											left=s->left * 2;
	TBoolean                dungeonLighting=false;


	if (playerDead)
		return;

//  if ((playerRow+13>=598) && (playerRow+13<=615) && (playerCol+13>=79) && (playerCol+13<=94))
//    return;


	if (playerRow>1908 && playerRow<1924 && playerCol>1689 && playerCol<1732)
		return;
	//Dave /forest area
	if (playerRow+13>1939 && playerRow+13<1985 && playerCol+13>961 && playerCol+13<1000)
		return;
	if (playerRow+13>1921 && playerRow+13<1982 && playerCol+13>561 && playerCol+13<598)
		return;
	if (playerRow+13>1922 && playerRow+13<1951 && playerCol+13>603 && playerCol+13<631)
		return;
	if (playerRow+13>1961 && playerRow+13<1984 && playerCol+13>406 && playerCol+13<491)
		return;
	
	if ((playerRow>1842 && playerCol>40) || (playerCol>1842))
		dungeonLighting=true;
	
	

	if ((level==0) && (dungeonLighting==false))	// daylight
		return;

	if ((nightVision) && (dungeonLighting==false))
		return;

	if (dungeonLighting)
		level=1;

#ifdef _DEBUGGING
	level=0;

#endif

	if (s->right>=562)
	{
		tb_AlertMessage("Darken out of bounds.");	// beta
		return;
	}

	if ((level==1) || (level==3))	// dusk/dawn
		for (yi=s->top;yi<=s->bottom;yi++)
		{
			myPixelPtr 	= (UInt32*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + left);

			for (xi=s->left;xi<=s->right;xi=xi+2)
			{
				*myPixelPtr = (*myPixelPtr & kLongMask) >> 1;
				*myPixelPtr++;
			}
		}

	if (level==2)	// darkness
		for (yi=s->top;yi<=s->bottom;yi++)
		{
			myPixelPtr 	= (UInt32*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + left);

			for (xi=s->left;xi<=s->right;xi=xi+2)
			{
				*myPixelPtr = (*myPixelPtr & kLongMask) >> 1;
				*myPixelPtr = (*myPixelPtr & kLongMask) >> 1;
				*myPixelPtr++;
			}
		}

}

// ----------------------------------------------------------------- //
void	tb_GrayBMP(int bitmap, TRect *t)
// ----------------------------------------------------------------- //

{
//  UInt16* 								myPixelPtr 	= nil;
//	unsigned int 						a						=	0;
//	int											xi;
//	int											yi;
	int											rowInc;
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 						a						=	0;
	int											xi;
	int											yi;
//	int											left;

	myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (t->top * gwRowBytes[bitmap]) + (t->left*2));
	rowInc			= (gwRowBytes[bitmap]-(2*(t->right-t->left))-2)/2;

	for (yi=t->top;yi<=t->bottom;yi++)
	{
		for (xi=t->left;xi<=t->right;xi++)
		{
			a=(((*myPixelPtr << 1) & 0xF800) + ((*myPixelPtr << 6) & 0xF800) + ((*myPixelPtr << 11) & 0xF800))>>2;
			*myPixelPtr = (((a >> 11) & 0x1F) << 10) | (((a >> 11) & 0x1F) << 5) | ((a >> 11) & 0x1F);
			myPixelPtr++;
		}

		myPixelPtr = myPixelPtr + rowInc;
	}

	/*
		left=t->left*2;
		for (yi=t->top;yi<=t->bottom;yi++)
			{

				myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + left);

				for (xi=t->left;xi<=t->right;xi++)
					{

						a=(((*myPixelPtr << 1) & 0xF800) + ((*myPixelPtr << 6) & 0xF800) + ((*myPixelPtr << 11) & 0xF800))>>2;

		  			*myPixelPtr = (((a >> 11) & 0x1F) << 10) | (((a >> 11) & 0x1F) << 5) | ((a >> 11) & 0x1F);

						myPixelPtr++;

					}

			}
	*/
}

// ----------------------------------------------------------------- //
void	tb_DarkenTextBackground(int bitmap, TRect *t)
// ----------------------------------------------------------------- //

{
	UInt16* 								myPixelPtr 	= nil;
	unsigned int 						r						=	0;
	unsigned int 						g						=	0;
	unsigned int 						b						=	0;
	int											xi;
	int											yi;
	int											steps;
	float										inc;
	float										intensity;
	float										edge;
	TRect                   z;


	z.left=t->left;
	z.top=t->top;
	z.right=t->right;
	z.bottom=t->bottom;

	z.top=z.top-5;
	z.bottom=z.bottom+5;
	z.left=z.left-4;
	z.right=z.right+4;

	steps=(z.bottom-z.top)/2;
	inc=-((50/(float)steps)/100);

	intensity=0.95;

	for (yi=z.top;yi<=z.bottom;yi++)
	{

		for (xi=z.left;xi<=z.right;xi++)
		{

			if (xi<=gwRect[bitmap].left)		continue;

			if (xi>=gwRect[bitmap].right)		continue;

			if (yi<=gwRect[bitmap].top)			continue;

			if (yi>=gwRect[bitmap].bottom)	continue;

			myPixelPtr 	= (UInt16*) (gwBaseAddress[bitmap] + (yi * gwRowBytes[bitmap]) + (xi * 2));

			r = (*myPixelPtr << 1) & 0xF800;

			g = (*myPixelPtr << 6) & 0xF800;

			b = (*myPixelPtr << 11) & 0xF800;

			if ((xi<z.left+3) || (xi>z.right-3))
			{
				edge=intensity*1.2;

				if (edge>1.0)	edge=1.0;

				r=(float)r*edge;

				g=(float)g*edge;

				b=(float)b*edge;
			}
			else
			{
				r=(float)r*intensity;
				g=(float)g*intensity;
				b=(float)b*intensity;
			}

			*myPixelPtr = (((r >> 11) & 0x1F) << 10) | (((g >> 11) & 0x1F) << 5) | ((b >> 11) & 0x1F);

		}

		intensity=intensity+inc;

		if (intensity<=0.5)
			inc=fabs(inc);

		if (intensity>0.95)
			intensity=0.95;

	}

}

// ----------------------------------------------------------------- //
void tb_CopyMaskBitmap(TBitmap srcBitmap, TBitmap maskBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *maskRect, TRect *dstRect)
// ----------------------------------------------------------------- //

{
	CGrafPtr              thePort;


	GetKeyEvents();

	if (dstBitmap>4999)
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		CopyMask(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[maskBitmap]),GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )),srcRect,maskRect,dstRect);
		thePort = GetWindowPort(win[dstBitmap-5000]);

		if (QDIsPortBuffered(thePort))
		{
			RgnHandle theRgn;
			theRgn = NewRgn();
			SetRectRgn(theRgn, dstRect->left, dstRect->top, dstRect->right, dstRect->bottom);
			QDFlushPortBuffer(thePort, theRgn);
			DisposeRgn(theRgn);
		}

		SetGWorld(tbsavedPort, tbsavedDevice);
	}
	else
	{
		if ((dstBitmap)>=MaxGWorlds)    return;

		if (gwFree[dstBitmap])         return;

		GetGWorld(&tbsavedPort, &tbsavedDevice);

		SetGWorld(gw[dstBitmap], nil);

		RGBForeColor(&myBlack);

		RGBBackColor(&myWhite);

		CopyMask(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[maskBitmap]),GetPortBitMapForCopyBits(gw[dstBitmap]),srcRect,maskRect,dstRect);

		SetGWorld(tbsavedPort, tbsavedDevice);
	}

}

// ----------------------------------------------------------------- //
void tb_SetGWorld(TBitmap theBitmap)
// ----------------------------------------------------------------- //

{

	GetGWorld(&tbsavedPort, &tbsavedDevice);
	SetGWorld(gw[theBitmap], nil);
	RGBForeColor(&myBlack);
	RGBBackColor(&myWhite);

}

// ----------------------------------------------------------------- //
void tb_RestorePort(void)
// ----------------------------------------------------------------- //

{

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_FastCopyMaskBitmap(TBitmap srcBitmap, TBitmap maskBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *maskRect, TRect *dstRect)
// ----------------------------------------------------------------- //

{

	GetKeyEvents();

	if (srcBitmap<5000) // mod 8/4
	{
		if ((srcBitmap)>=MaxGWorlds)          return;

		if (gwFree[srcBitmap])                return;
	}
	else
		return;

	if (maskBitmap<5000) // mod 8/4
	{
		if ((maskBitmap)>=MaxGWorlds)     return;

		if (gwFree[maskBitmap])           return;
	}
	else
		return;

	if (dstBitmap<5000) // mod 8/4
	{
		if ((dstBitmap)>=MaxGWorlds)     return;

		if (gwFree[dstBitmap])           return;
	}

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	SetGWorld(gw[dstBitmap], nil);
	CopyMask(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[maskBitmap]),GetPortBitMapForCopyBits(gw[dstBitmap]),srcRect,maskRect,dstRect);
	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_CopyMaskBitmapClr(TBitmap srcBitmap, TBitmap maskBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *maskRect, TRect *dstRect, unsigned int red, unsigned int green, unsigned int blue)
// ----------------------------------------------------------------- //

{
	RGBColor									backColor;


	backColor.red=red;
	backColor.green=green;
	backColor.blue=blue;

	if (dstBitmap>4999)
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&myBlack);
		RGBBackColor(&backColor);
		CopyMask(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[maskBitmap]),GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )),srcRect,maskRect,dstRect);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		SetGWorld(tbsavedPort, tbsavedDevice);
	}
	else
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&myBlack);
		RGBBackColor(&backColor);
		CopyMask(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[maskBitmap]),GetPortBitMapForCopyBits(gw[dstBitmap]),srcRect,maskRect,dstRect);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		SetGWorld(tbsavedPort, tbsavedDevice);
	}

}

// ----------------------------------------------------------------- //
void tb_CopyBitmapClr(TBitmap srcBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *dstRect, unsigned int red, unsigned int green, unsigned int blue)
// ----------------------------------------------------------------- //

{
	RGBColor									backColor;


	backColor.red=red;
	backColor.green=green;
	backColor.blue=blue;

	if (dstBitmap>4999)
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&myBlack);
		RGBBackColor(&backColor);
		CopyBits(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )),srcRect,dstRect,srcCopy,NULL);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		SetGWorld(tbsavedPort, tbsavedDevice);
	}
	else
	{
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		SetGWorld(gw[dstBitmap], nil);
		RGBForeColor(&myBlack);
		RGBBackColor(&backColor);
		CopyBits(GetPortBitMapForCopyBits(gw[srcBitmap]),GetPortBitMapForCopyBits(gw[dstBitmap]),srcRect,dstRect,srcCopy,NULL );
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		SetGWorld(tbsavedPort, tbsavedDevice);
	}

}

// ----------------------------------------------------------------- //
void tb_SetPort(TBitmap theBitmap)
// ----------------------------------------------------------------- //

{

	if (theBitmap>4999)
	{
#if (Debug==true)

		if (win[theBitmap-5000]==NULL)
		{
			tb_DisplayError("tb_SetPort:  Null window.");
			return;
		}

#endif
		SetPortWindowPort(win[theBitmap-5000]);
	}
	else
	{
#if (Debug==true)

		if (gw[theBitmap]==NULL)
		{
			tb_DisplayError("tb_SetPort:  Null GWorld.");
			return;
		}

#endif
		SetGWorld(gw[theBitmap], nil);
	}

}

// ----------------------------------------------------------------- //
void tb_CopyBitmap(TBitmap srcBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *dstRect, TBoolean /*Transparent*/)
// ----------------------------------------------------------------- //

{
	CGrafPtr              thePort;

	//int t1, t2;
	//Str255 tt, ttt;


	GetKeyEvents();

	if ((srcRect->right-srcRect->left) != (dstRect->right-dstRect->left))
	{
		fi_WriteToErrorLog("Wrong pict width");
		return;
	}

	if ((srcRect->bottom-srcRect->top) != (dstRect->bottom-dstRect->top))
	{
		fi_WriteToErrorLog("Wrong pic height");
		return;
	}

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )), srcRect, dstRect, srcCopy, NULL );
		thePort = GetWindowPort(win[dstBitmap-5000]);

		if (QDIsPortBuffered(thePort))
		{
			RgnHandle theRgn;
			theRgn = NewRgn();
			SetRectRgn(theRgn, dstRect->left, dstRect->top, dstRect->right, dstRect->bottom);
			QDFlushPortBuffer(thePort, theRgn);
			DisposeRgn(theRgn);
		}
	}
	else
	{
		if (dstBitmap>=MaxGWorlds)  { fi_WriteToErrorLog("tb_CopyBitmap 3"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (gwFree[dstBitmap])      { fi_WriteToErrorLog("tb_CopyBitmap 4"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (srcBitmap>=MaxGWorlds)  { fi_WriteToErrorLog("tb_CopyBitmap 5"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (gwFree[srcBitmap])      { fi_WriteToErrorLog("tb_CopyBitmap 6"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		SetGWorld(gw[dstBitmap], nil);

		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits( gw[dstBitmap] ), srcRect, dstRect, srcCopy, NULL );
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_CopyBitmapGray(TBitmap srcBitmap, TBitmap dstBitmap, TRect *srcRect, TRect *dstRect, TBoolean /*Transparent*/)
// ----------------------------------------------------------------- //

{
	if ((srcRect->right-srcRect->left) != (dstRect->right-dstRect->left))
	{
		//fi_WriteToErrorLog("tb_CopyBitmap 1");
		return;
	}

	if ((srcRect->bottom-srcRect->top) != (dstRect->bottom-dstRect->top))
	{
		//fi_WriteToErrorLog("tb_CopyBitmap 2");
		return;
	}

	GetGWorld(&tbsavedPort, &tbsavedDevice);

	if (dstBitmap>4999)
	{
		SetPortWindowPort(win[dstBitmap-5000]);
		RGBForeColor(&myBlack);
		RGBBackColor(&myGray);
		CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )), srcRect, dstRect, srcCopy, NULL );
	}
	else
	{
		if (dstBitmap>=MaxGWorlds)  { fi_WriteToErrorLog("tb_CopyBitmap 3"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (gwFree[dstBitmap])      { fi_WriteToErrorLog("tb_CopyBitmap 4"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (srcBitmap>=MaxGWorlds)  { fi_WriteToErrorLog("tb_CopyBitmap 5"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		if (gwFree[srcBitmap])      { fi_WriteToErrorLog("tb_CopyBitmap 6"); SetGWorld(tbsavedPort, tbsavedDevice); return; }

		SetGWorld(gw[dstBitmap], nil);

		RGBForeColor(&myGray);
		RGBBackColor(&myWhite);
		CopyBits( GetPortBitMapForCopyBits( gw[srcBitmap] ), GetPortBitMapForCopyBits( gw[dstBitmap] ), srcRect, dstRect, srcCopy, NULL );
	}

	SetGWorld(tbsavedPort, tbsavedDevice);

}

// ----------------------------------------------------------------- //
void tb_SetDefaultFolder(void)
// ----------------------------------------------------------------- //

{

	HSetVol(NULL, AppFSSpec.vRefNum, AppFSSpec.parID);

}


// ----------------------------------------------------------------- //
void tb_InitializeToolbox(void)
// ----------------------------------------------------------------- //

{
	int					i;
	//long				csrandom;
	TRect				r;
	BitMap 			screenBits;

	tb_SetCursor(0);

//	GetDateTime((unsigned long*) &qd.randSeed);
//	csrandom = (unsigned long) clock();
//	srand(csrandom);
	AppRefNum = CurResFile();
	FSpGetFileLocation(AppRefNum, &AppFSSpec);

	tbCurrentvRefNum	=	AppFSSpec.vRefNum;
	tbCurrentDirID		=	AppFSSpec.parID;

	FindFolder(kOnSystemDisk, 'pref', false, &prefsvRefNum, &prefsDirID);

	restoreValid=false;

	GetQDGlobalsScreenBits(&screenBits);
	screenBounds = screenBits.bounds;

	myWhite.red=65535;
	myWhite.green=65535;
	myWhite.blue=65535;

	myBlack.red=0;
	myBlack.green=0;
	myBlack.blue=0;

	myRed.red=65535;
	myRed.green=0;
	myRed.blue=0;

	myGray.red=51968;
	myGray.green=51968;
	myGray.blue=51968;

	for (i=0; i<MaxGWorlds; i++)
	{
		gwFree[i]=true;
		gwForeColor[i].red=0;
		gwForeColor[i].green=0;
		gwForeColor[i].blue=0;
		gwBackColor[i].red=65535;
		gwBackColor[i].green=65535;
		gwBackColor[i].blue=65535;
	}

	for (i=0; i<MaxWindows; i++)
	{
		winFree[i]=true;
		winForeColor[i].red=0;
		winForeColor[i].green=0;
		winForeColor[i].blue=0;
		winBackColor[i].red=65535;
		winBackColor[i].green=65535;
		winBackColor[i].blue=65535;
	}

	MenuFree[0]=false;

	for (i=1; i<10; i++)
	{
		MenuFree[i]=true;
	}

	for (i=0; i<MaxCursors; i++)
	{
		curID[i]=0;
	}

	for (i=0; i<MaxRegions; i++)
	{
		RgnFree[i]=true;
	}

	InitializeAsyncSoundPlayer();

	tb_SwithToApplicationFolder();

	tb_SetRect(&r,0,0,560,560);
	fxBuffer = tb_CreateBitmap(&r, 0, kScreenDepth);
	filterBuffer = tb_CreateBitmap(&r, 0, kScreenDepth);

	tb_SetRect(&r,0,0,128,128);
	scratchBMP =	tb_CreateBitmap(&r, kNull, kScreenDepth);

}

// ----------------------------------------------------------------- //
void tb_CloseToolbox(void)
// ----------------------------------------------------------------- //

{
	int				i;

	CleanUpSounds(true);

	for (i=0; i<MaxGWorlds; i++)
		if (gwFree[i]==false)
		{
			if (gw[i]!=nil)
			{
				DisposeGWorld(gw[i]);
				gw[i]=nil;
				gwFree[i]=true;
			}
		}

}


// ----------------------------------------------------------------- //
PicHandle	tb_GetPicture(int i, TBoolean /*convert*/)
// ----------------------------------------------------------------- //

{
	PicHandle				thePict;
	FSSpec					PictFSSpec;
	//FSSpec					xPictFSSpec;
	char						cname[kStringLength];
	//char						xname[kStringLength];
	long						dataLength;
	Str255					name;
	//Str255					namex;
	long						theDirID;
	TBoolean				isDirectory;

#ifndef _DEBUGGING
	tb_IntegerToString(i,cname);
	strcat(cname,".pxt");
	tb_CtoPStr255(cname,name);
#endif

#ifdef _DEBUGGING
	tb_IntegerToString(i,cname);
	strcat(cname,".pct");
	tb_CtoPStr255(cname,name);
#endif

	Error=GetDirectoryID(AppFSSpec.vRefNum,AppFSSpec.parID,"\pOberin Graphics",&theDirID,&isDirectory);
	FSMakeFSSpec(AppFSSpec.vRefNum, theDirID, name, &PictFSSpec);
	Error=FSpOpenDF(&PictFSSpec, fsRdPerm, &refNum);

	Error= GetEOF(refNum,&dataLength);

#ifdef _DEBUGGING
	Error = SetFPos(refNum,fsFromStart,512);
	dataLength=dataLength-512;
#endif

	thePict=(PicHandle)NewHandle(dataLength);

	if (thePict!=nil)
	{
		HLock((Handle)thePict);
		Error = FSRead(refNum,&dataLength,*thePict);
		HUnlock((Handle)thePict);
		Error = FSClose(refNum);

#ifndef _DEBUGGING
		(**(thePict)).picFrame.left = 0;
		(**(thePict)).picFrame.top = 0;
#endif

#ifdef _DEBUGGING

		if (convert)
		{
			(**(thePict)).picFrame.left = (**(thePict)).picFrame.right;
			(**(thePict)).picFrame.top = (**(thePict)).picFrame.bottom;

			tb_IntegerToString(i,xname);
			strcat(xname,".pxt");
			tb_CtoPStr255(xname,namex);
			Error=GetDirectoryID(AppFSSpec.vRefNum,AppFSSpec.parID,"\pOberin Graphics",&theDirID,&isDirectory);
			FSMakeFSSpec(AppFSSpec.vRefNum, theDirID, namex, &xPictFSSpec);
			Error=FSpDelete(&xPictFSSpec);
			Error=FSpCreate(&xPictFSSpec, Creator, FileType, smSystemScript);
			Error=FSpOpenDF(&xPictFSSpec, fsRdWrPerm, &refNum);
			Error = FSWrite(refNum, &dataLength, *thePict);
			Error = FSClose(refNum);
			KillPicture(thePict);
			return (nil);
		}

#endif

		return (thePict);
	}

	fi_WriteToErrorLog("Unable to load graphic");

	return (nil);
}

/*
FUNCTION MyDrawFilePicture(pictFileRefNum: Integer; destRect: Rect): OSErr;

CONST
����cPicFileHeaderSize = 512;

VAR
����myPic:����������PicHandle;
����dataLength:�����LongInt;
����err:������������OSErr;

{�������{This listing assumes the current graphics port is set.}

����err := GetEOF(pictFileRefNum,dataLength);�������������������{get file length}
����IF err = noErr THEN {
��������err := SetFPos(pictFileRefNum,fsFromStart,
����������������������������cPicFileHeaderSize);��������{move past the 512-byte 'PICT' }
��������������������������������������������������������{ file headeRend
��������dataLength := dataLength - cPicFileHeaderSize;������������������{remove 512-byte }
������������������������������������������������{ 'PICT' file header from file length}
��������myPic := PicHandle(NewHandle(dataLength)); {allocate picture handle}
��������IF (err = noErr) & (myPic <> NIL) THEN {
������������HLock(Handle(myPic));�����������{lock picture handle before using FSRead}
������������err := FSRead(pictFileRefNum,dataLength,Ptr(myPic^));�������������������{read file}
������������HUnlock(Handle(myPic));���������{unlock picture handle after using FSRead}
������������MyAdjustDestRect(myPic,destRect);���������������{see Listing 7-7}
������������DrawPicture(myPic,destRect);
������������IF QDError <> noErr THEN
����������������; {likely error is that there is insufficient memory}
������������KillPicture(myPic);
��������};
����};
����MyDrawFilePicture := err;

};
*/
// ----------------------------------------------------------------- //
void tb_DrawPictureToBitmap(TBitmap dstBitmap, int Picture, int h, int v, TRect *r)
// ----------------------------------------------------------------- //

{
	//WindowRef             oldWindow;
	//TRect									s;
	CGrafPtr              thePort;


	SetRect(r, 0, 0, 0, 0);

	//if (dstBitmap>4999)
	//	{
	//		h=h+hOffset;
	//		v=v+vOffset;
	//	}

#if (Debug==true)

	if ((dstBitmap<5000) && (gwFree[dstBitmap]==true))
	{
		tb_DisplayError("tb_DrawPictureToBitmap:  Illegal destination bitmap.");
		return;
	}

	if ((dstBitmap>4999) && (winFree[dstBitmap-5000]==true))
	{
		tb_DisplayError("tb_DrawPictureToBitmap:  Illegal destination window.");
		return;
	}

	if ((dstBitmap<5000) && ((dstBitmap<0) || (dstBitmap>=MaxGWorlds)))
	{
		tb_DisplayError("tb_DrawPictureToBitmap:  Illegal destination bitmap bounds.");
		return;
	}

	if ((dstBitmap>=MaxGWorlds) && ((dstBitmap<5000) || (dstBitmap>=5000+MaxWindows)))
	{
		tb_DisplayError("tb_DrawPictureToBitmap:  Illegal destination windows bounds.");
		return;
	}

#endif

	if ((Picture>5000) && (Picture!=10000))
		PHandle = tb_GetPicture(Picture,false);
	else
		PHandle = GetPicture(Picture);

	if (PHandle != nil)
	{
		HLock((Handle)PHandle);
		r->right = r->left + ((**(PHandle)).picFrame.right - (**(PHandle)).picFrame.left);
		r->bottom = r->top + ((**(PHandle)).picFrame.bottom - (**(PHandle)).picFrame.top);
		//s=*r;
		OffsetRect(r, h, v);

		if (dstBitmap>4999)
		{
#if (Debug==true)

			if (win[dstBitmap-5000]==NULL)
			{
				HUnlock((Handle)PHandle);

				if ((Picture>5000) && (Picture!=10000))
					KillPicture(PHandle);
				else
					ReleaseResource((Handle)PHandle);

				return;
			}

#endif
			//GetGWorld(&tbsavedPort, &tbsavedDevice);
			//SetGWorld(gw[scratchBMP], nil);
			//DrawPicture(PHandle,&s);
			//SetGWorld(tbsavedPort, tbsavedDevice);
			///CopyBits( GetPortBitMapForCopyBits( gw[scratchBMP] ), GetPortBitMapForCopyBits(GetWindowPort( win[dstBitmap-5000] )), &s, r, srcCopy, NULL );
			GetGWorld(&tbsavedPort, &tbsavedDevice);

			SetPortWindowPort(win[dstBitmap-5000]);

			DrawPicture(PHandle, r);

			SetGWorld(tbsavedPort, tbsavedDevice);

			thePort = GetWindowPort(win[dstBitmap-5000]);

			if (QDIsPortBuffered(thePort))
			{
				RgnHandle theRgn;
				theRgn = NewRgn();
				SetRectRgn(theRgn, r->left, r->top, r->right, r->bottom);
				QDFlushPortBuffer(thePort, theRgn);
				DisposeRgn(theRgn);
			}
		}
		else
		{
#if (Debug==true)

			if (gw[dstBitmap]==NULL)
			{
				HUnlock((Handle)PHandle);

				if ((Picture>5000) && (Picture!=10000))
					KillPicture(PHandle);
				else
					ReleaseResource((Handle)PHandle);

				return;
			}

#endif
			GetGWorld(&tbsavedPort, &tbsavedDevice);

			SetGWorld(gw[dstBitmap], nil);

			DrawPicture(PHandle,r);

			SetGWorld(tbsavedPort, tbsavedDevice);
		}

		HUnlock((Handle)PHandle);

		if ((Picture>5000) && (Picture!=10000))
			KillPicture(PHandle);
		else
			ReleaseResource((Handle)PHandle);
	}
	else
	{
		//tb_DisplayError("tb_DrawPictureToBitmap:  Null Picture.");
		return;
	}

	return;

}

// ----------------------------------------------------------------- //
void tb_DisposeBitmap(TBitmap Bitmap)
// ----------------------------------------------------------------- //

{
	int				i;

	if (Bitmap>4999) return;

	if (gwFree[Bitmap]==true) return;

	if (gw[Bitmap]!=nil)
	{
		for (i=0; i<MaxWindows; i++)
			if (winFree[i]==false) if (win[i]!=nil)
				{
					SetPortWindowPort(win[i]);
					break;
				}

		UnlockPixels(GetGWorldPixMap(gw[Bitmap]));

		DisposeGWorld(gw[Bitmap]);
		gw[Bitmap]=nil;
		gwFree[Bitmap]=true;
	}

}

// ----------------------------------------------------------------- //
void tb_DisposeOfWindow(TWindow theWindow)
// ----------------------------------------------------------------- //

{

	if (theWindow<5000) return;

	if (winFree[theWindow-5000]==true) return;

	if (win[theWindow-5000]!=nil)
	{
		DisposeWindow(win[theWindow-5000]);
		win[theWindow-5000]=nil;
		winFree[theWindow-5000]=true;
	}

}

// ----------------------------------------------------------------- //
int tb_CreateBitmap(TRect *r, int Picture, int Depth)
// ----------------------------------------------------------------- //

{

	int					i;
	int					found=-1;
	TRect				t;

	for (i=0; i<MaxGWorlds; i++)
		if (gwFree[i]==true)
		{
			found=i;
			break;
		}

	if (found==-1) return (GWorldError);

	if (Picture>0)
	{
		if ((Picture>5000) && (Picture!=10000))
			PHandle = tb_GetPicture(Picture,false);
		else
			PHandle = GetPicture(Picture);

		SetRect(r, 0, 0, 0, 0);

		if (PHandle != nil)
		{
			HLock((Handle)PHandle);
			r->right = r->left + ((**(PHandle)).picFrame.right - (**(PHandle)).picFrame.left);
			r->bottom =r->top + ((**(PHandle)).picFrame.bottom - (**(PHandle)).picFrame.top);
			Error = NewGWorld(&gw[found], Depth, r, nil, nil, 0);
			gwRect[found]=*r;

			if (Error != noErr)
			{
#if (Debug==true)
				tb_DisplayError("tb_CreateBitmap:  Unable to create GWorld.");
#endif
				fi_WriteToErrorLog("Unable to create GWorld");
				return (GWorldError);
			}

			MoveHHi((Handle)gw[found]);

			GetGWorld(&tbsavedPort, &tbsavedDevice);
			LockPixels( GetGWorldPixMap( gw[found] ) );
			SetGWorld(gw[found], nil);
			DrawPicture(PHandle, r);
			HUnlock((Handle)PHandle);

			if ((Picture>5000) && (Picture!=10000))
				KillPicture(PHandle);
			else
				ReleaseResource((Handle)PHandle);

			RGBForeColor(&myBlack);

			RGBBackColor(&myWhite);

			gwForeColor[found].red=0;

			gwForeColor[found].green=0;

			gwForeColor[found].blue=0;

			gwBackColor[found].red=65535;

			gwBackColor[found].green=65535;

			gwBackColor[found].blue=65535;

			gwPixMapHdl[found]		=	GetGWorldPixMap(gw[found]);

			gwBaseAddress[found]	=	GetPixBaseAddr(gwPixMapHdl[found]);

			gwRowBytes[found]			=	(*gwPixMapHdl[found])->rowBytes & 0x3FFF;

			SetGWorld(tbsavedPort, tbsavedDevice);
		}
		else
		{
			SysBeep(0);
#if (Debug==true)
			tb_DisplayError("tb_CreateBitmap:  Nil picture.");
#endif
			fi_WriteToErrorLog("Nil Picture");
		}
	}
	else
	{
		t=*r;
		OffsetRect(&t, -t.left, -t.top);
		Error = NewGWorld(&gw[found], Depth, &t, NULL, NULL, 0);
		gwRect[found]=t;
		MoveHHi((Handle)gw[found]);
		GetGWorld(&tbsavedPort, &tbsavedDevice);
		LockPixels( GetGWorldPixMap( gw[found] ) );
		SetGWorld(gw[found], nil);
		//RGBForeColor(&myWhite);
		//PaintRect(&t);
		RGBForeColor(&myBlack);
		RGBBackColor(&myWhite);
		gwForeColor[found].red=0;
		gwForeColor[found].green=0;
		gwForeColor[found].blue=0;
		gwBackColor[found].red=65535;
		gwBackColor[found].green=65535;
		gwBackColor[found].blue=65535;
		gwPixMapHdl[found]		=	GetGWorldPixMap(gw[found]);
		gwBaseAddress[found]	=	GetPixBaseAddr(gwPixMapHdl[found]);
		gwRowBytes[found]			=	(*gwPixMapHdl[found])->rowBytes & 0x3FFF;
		SetGWorld(tbsavedPort, tbsavedDevice);
	}

	gwFree[found]=false;

	return (found);

}

// ----------------------------------------------------------------- //
TBoolean tb_PascalStringsEqual(unsigned char str1[],unsigned char str2[])
// ----------------------------------------------------------------- //

{

	int		len1=0;
	int		len2=0;
	int		i;

	len1=str1[0];
	len2=str2[0];

	if (len1!=len2) return (false);

	for (i = 1; i <= len1; i++)
		if (str1[i]!=str2[i]) return (false);

	return (true);

}

// ----------------------------------------------------------------- //
void tb_PascaltoCStr(Str255 thePString,char StrC[kStringLength])
// ----------------------------------------------------------------- //

{

	int				len;

	len=thePString[0];
	BlockMove(&thePString[1],&StrC[0],len);
	StrC[len]='\0';

}

// ----------------------------------------------------------------- //
void tb_CopyStringAt(char *str1,char *str2, int pos)
// ----------------------------------------------------------------- //

{
	int		i;

	strcpy(str2,"");

	if (strlen(str1)<pos)
		return;

	i=pos;

	do
	{
		str2[i-pos]=str1[i];
		i++;
	}
	while ((str1[i]!='\0') && (i<strlen(str1)));

	str2[i-pos]='\0';

}

// ----------------------------------------------------------------- //
void tb_DeleteStringPadding(char *str1)
// ----------------------------------------------------------------- //

{
	int		    i;
	char      str2[kStringLength];
	int       s;
	int       e;

	if (strcmp(str1,"")==0)
		return;

	s=0;

	e=strlen(str1);

	i=0;

	while (str1[s]==' ')
		s++;

	while (str1[e-1]==' ')
		e--;

	if ((s==0) && (e==strlen(str1)))
		return;

	strcpy(str2,"");

	if (s>=e)
		return;

	for (i=s;i<e;i++)
	{
		str2[i-s]=str1[i];
		str2[i-s+1]='\0';
	}

	strcpy(str1,str2);

}

// ----------------------------------------------------------------- //
void tb_CtoPStr255(char *str,unsigned char str2[])
// ----------------------------------------------------------------- //

{

	int		len=0;
	int		i;

	for (i = 0; i < 255; i++)
		if (str[i]=='\0')
		{
			len=i;
			break;
		}

	str2[0] = len;

	for (i = 1; i <= len; i++)
		str2[i] = str[i-1];

}

// ----------------------------------------------------------------- //
void tb_CtoPStr31(char *str,unsigned char str2[])
// ----------------------------------------------------------------- //

{

	int		len=0;
	int		i;

	for (i = 0; i < 31; i++)
		if (str[i]=='\0')
		{
			len=i;
			break;
		}

	str2[0] = len;

	for (i = 1; i <= len; i++)
		str2[i] = str[i-1];

}

// ----------------------------------------------------------------- //
int tb_GetWindowMatch(WindowPtr wp) // Ignore this function.  It is Mac specific.
// ----------------------------------------------------------------- //

{
	int			i;

	for (i=0;i<MaxWindows;i++)
		if (wp==win[i])	return (i+5000);

	return (-1);

}

// ----------------------------------------------------------------- //
char* tb_Unscramble(char text[kStringLength])
// ----------------------------------------------------------------- //
{
	int len=strlen(text);
	int cpt;
	char real[kStringLength];
	
	int k[10]={5,3,3,4,1,4,3,1,0,3};
	
	strcpy(real, "");
	
	for (cpt=0; cpt<len; cpt++)
	{
		if ((int)text[cpt]<91)
			real[cpt]=text[cpt]+k[cpt%10];
		else
			real[cpt]=text[cpt]-k[cpt%10];
	}
	
	real[len]='\0';
		
	return (real);
}

//Return date or time
// ----------------------------------------------------------------- //
char* tb_getTimeAndDate(char getWhat[kStringLength])
// ----------------------------------------------------------------- //

{
	long									t;
	DateTimeRec						Date;
	unsigned long					Secs;
	char									dateAndTime[kStringLength]; //[128];
	char									temp[kStringLength]; //[50];



	t = ReadDateTime(&Secs);
	strcpy(dateAndTime,"[");
	SecondsToDate(Secs, &Date);

	if(strcmp(getWhat,"date&time")==0 || strcmp(getWhat,"date")==0)
	{
		tb_IntegerToString(Date.day,temp);

		strcat(dateAndTime,temp);

		strcat(dateAndTime,"/");

		tb_IntegerToString(Date.month,temp);

		strcat(dateAndTime,temp);

		strcat(dateAndTime,"/");

		tb_IntegerToString(Date.year,temp);

		strcat(dateAndTime,temp);

		
	}
	if(strcmp(getWhat,"date&time")==0)
		strcat(dateAndTime," at ");
	

	if(strcmp(getWhat,"date&time")==0 || strcmp(getWhat,"time")==0)
	{
		tb_IntegerToString(Date.hour,temp);

		if (Date.hour<10)
			strcat(dateAndTime,"0");

		strcat(dateAndTime,temp);

		strcat(dateAndTime,":");

		tb_IntegerToString(Date.minute,temp);

		if (Date.minute<10)
			strcat(dateAndTime,"0");

		strcat(dateAndTime,temp);

		strcat(dateAndTime,":");

		tb_IntegerToString(Date.second,temp);

		if (Date.second<10)
			strcat(dateAndTime,"0");

		strcat(dateAndTime,temp);

		strcat(dateAndTime,"]");
	}
	return(dateAndTime);

};

// MaxScreenDepth returns the maximum bit depth that the monitor is capable of
// (i.e. 8 for 256 colors).
pascal short MaxScreenDepth( GDHandle theGDevice )
{
	short		x;
	short		hasThisDepth=1;

	for ( x=2; x<=32; x*=2 )
		if ( HasDepth( theGDevice, x, gdDevType, 1 ) )
			hasThisDepth = x;
		else
			break;

	return hasThisDepth;
}

// Returns true if the monitor supports the given depth
pascal Boolean SupportsDepth( GDHandle theGDevice, short aDepth)
{
	GDHandle		savedDevice;
	Boolean			val;

	savedDevice = GetGDevice();
	SetGDevice( theGDevice );

	val = HasDepth( theGDevice, aDepth, 1 << gdDevType, GetScreenMode(theGDevice) );

	SetGDevice( savedDevice );

	return val;
}

// GetScreenDepth returns the current bit depth of this monitor.
pascal short GetScreenDepth( GDHandle theGDevice )
{
	return ((**((**(theGDevice)).gdPMap)).pixelSize);
}

// SetScreenDepth changes the bit depth of the screen (the number of colors that it can
// display at once).
pascal void SetScreenDepth( GDHandle theGDevice, short newDepth )
{
	GDHandle		savedDevice;

	savedDevice = GetGDevice();
	SetGDevice( theGDevice );

	if ( newDepth != GetScreenDepth(theGDevice) )
		if ( HasDepth( theGDevice, newDepth, 1 << gdDevType, GetScreenMode(theGDevice) ) )
			SetDepth( theGDevice, newDepth, 1 << gdDevType, GetScreenMode(theGDevice) );

	SetGDevice( savedDevice );
}

// Returns true if the monitor supports the given mode
pascal Boolean SupportsMode( GDHandle theGDevice, short aMode)
{
	GDHandle		savedDevice;
	Boolean			val;

	savedDevice = GetGDevice();
	SetGDevice( theGDevice );

	val =  HasDepth( theGDevice, GetScreenDepth(theGDevice), 1 << gdDevType, aMode );

	SetGDevice( savedDevice );

	return val;
}

// GetScreenMode returns one if the monitor is displaying colors, zero if it's displaying in grays.
pascal short GetScreenMode( GDHandle theGDevice )
{
	if ( TestDeviceAttribute( theGDevice, gdDevType ) )
		return 1;	//colors
	else
		return 0;	//grays
}

// SetScreenMode sets this monitor to color if 1 is sent, or grays if zero is sent.
pascal void SetScreenMode( GDHandle theGDevice, short newMode )
{
	GDHandle		savedDevice;

	savedDevice = GetGDevice();
	SetGDevice( theGDevice );

	if ( newMode != GetScreenMode(theGDevice) )
		if ( HasDepth( theGDevice, GetScreenDepth(theGDevice), 1 << gdDevType, newMode ) )
			SetDepth( theGDevice, GetScreenDepth(theGDevice), 1 << gdDevType, newMode );

	SetGDevice( savedDevice );
}

// SetScreenModeDepth sets the monitor to the color mode and depth specified.
pascal void SetScreenModeDepth( GDHandle theGDevice, short newDepth, short newMode )
{
	GDHandle		savedDevice;

	savedDevice = GetGDevice();
	SetGDevice( theGDevice );

	if ( (newDepth != GetScreenDepth(theGDevice)) || (newMode != GetScreenMode(theGDevice)) )
		if ( HasDepth( theGDevice, newDepth, 1 << gdDevType, newMode ) )
			SetDepth( theGDevice, newDepth, 1 << gdDevType, newMode );

	SetGDevice( savedDevice );
}

