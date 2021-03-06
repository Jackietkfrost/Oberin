#include "Toolbox.h"
#include "Common.h"
#include "Globals.h"
#include "Graphics.h"
#include "Creatures.h"
#include "Network.h"
#include "Players.h"
#include "FileIO.h"
#include "Resource.h"
#include "Skills.h"
#include "Items.h"
#include "Npc.h"
#include "Shops.h"
#include "Splash.h"
#include "GameText.h"

extern TBoolean						gotFirstDrawMessage;
extern TBoolean						thisIsFirstRefresh;
extern int								frame;
extern char								playerText[kStringLength];
extern UInt16							playerRow;
extern UInt16							playerCol;
extern UInt16							previousPlayerRow;
extern UInt16							previousPlayerCol;
extern int								targetMode;
extern int								currentInfoDisplay;
extern Boolean						Done;
extern int								displayAreaButtonCurrent[10];
extern int					      currentDialog;
extern int					      tbLogOnWindow;

extern WindowPtr          logonWindow;
extern WindowPtr          statsWindow;
extern WindowPtr          characterWindow;
extern WindowPtr          mapWindow;
extern int                tbMapWindow;
extern int                tbCharacterWindow;
extern int                tbStatsWindow;

char											spName[kStringLength];
char											spPassword[kStringLength];
char											spIP[kStringLength];
char											partyFilename[kStringLength];

int												typingOn;
unsigned long							nextSwitch; // beta

extern char               playerNames[kMaxPlayers][kStringLength];
extern UInt32             playerIDs[kMaxPlayers];
extern TBoolean           windowVisible[kMaxWindows];
extern TBoolean           attemptingReconnect;

#ifndef _SERVERONLY

// ----------------------------------------------------------------- //
void sp_InitSplash(void)
// ----------------------------------------------------------------- //

{
	int             i;

	nextSwitch=TickCount()+(60*10);
	tb_SetCursor(0);
	typingOn=1;
	strcpy(spName,"");
	strcpy(spPassword,"");
	strcpy(spIP,"");

	strcpy(partyFilename,"");

	for (i=0;i<kMaxPlayers;i++)
	{
		strcpy(playerNames[i],"");
		playerIDs[i]=0;
	}

}

// ----------------------------------------------------------------- //
void sp_DrawSplashText(TBoolean onScreen)
// ----------------------------------------------------------------- //

{
	TRect							r;
	TRect							s;
	char							xspName[kStringLength];
	char							xspPassword[kStringLength];
	char							xspIP[kStringLength];
	int								i;

	if (typingOn==0)
		return;

	strcpy(xspName,spName);

	strcpy(xspPassword,spPassword);

	strcpy(xspIP,spIP);

	for ( i = 0; i < strlen(xspPassword); i++)
		xspPassword[i]=42;

	if (typingOn==1)
		strcat(xspName,"_");
	else if (typingOn==2)
		strcat(xspPassword,"_");
	else
		strcat(xspIP,"_");

	//tb_DrawPictureToBitmap(osBMP, 203, 193, 264, &r);
	tb_DrawPictureToBitmap(osBMP, 203, 48, 210, &r);

	tb_SetFont(osBMP, kFontIDGeneva);

	tb_SetFontSize(osBMP, 18);

	tb_SetFontStyle(osBMP, tb_Bold);

	tb_SetForeColor(osBMP,65535, 65535, 65535);

	//b_DrawString(osBMP, 334-144, 287-37, xspName);
	//tb_DrawString(osBMP, 334-144, 328-37, xspPassword);

	tb_DrawString(osBMP, 191, 234, xspName);

	tb_DrawString(osBMP, 191, 275, xspPassword);

	tb_DrawString(osBMP, 191, 316, xspIP);

	if (onScreen)
	{
		//tb_SetRect(&s,193,264,606,335);
		tb_SetRect(&s,48,210,461,322);
		//r=s;
		//tb_OffsetRect(&r,horizontalOffset,verticalOffset);
		tb_CopyBitmap(osBMP,tbLogOnWindow,&s,&s,false);
	}

//	tb_SetForeColor(osBMP,0, 0, 0);
//	tb_SetFontSize(osBMP, 10);
	//tb_SetFontStyle(osBMP, tb_Plain);
//	tb_DrawString(osBMP, 9, 20, kBuildString);
//	tb_SetRect(&s,0,0,100,20);
//	tb_CopyBitmap(osBMP,tbLogOnWindow,&s,&s,false);

}

// ----------------------------------------------------------------- //
void sp_DrawSplash(void)
// ----------------------------------------------------------------- //

{
	TRect							r;
	TRect							s;
	//TRect							b;

	tb_DrawPictureToBitmap(osBMP, 243, 0, 0, &r);
	sp_DrawSplashText(false);
	tb_SetRect(&s,0,0,512,512);
	r=s;
	tb_CopyBitmap(osBMP,tbLogOnWindow,&s,&r,false);
	tb_SetForeColor(tbLogOnWindow, 0, 0, 0);

}

// ----------------------------------------------------------------- //
void sp_AutoLogon (void)	// beta
// ----------------------------------------------------------------- //

{
	TRect								r;
	//char								dummy[kStringLength];


	tb_DrawPictureToBitmap(tbLogOnWindow, 199, 461+horizontalOffset, 371+verticalOffset, &r);
	onLine								=	true;
	gotFirstDrawMessage		=	false;
	thisIsFirstRefresh		=	true;
	frame									=	0;
	strcpy(playerText,"");
	playerRow							=	0;
	playerCol							=	0;
	previousPlayerRow			=	0;
	previousPlayerCol			=	0;
	currentInfoDisplay		=	kCharacterDisplay;
	targetMode						=	kNoTarget;
	pl_InitPlayers();
	in_ClearLocalInventory();
	nw_InitNetworkingVariables();
	UpdateEvent(tbLogOnWindow);
	tb_DrawPictureToBitmap(tbLogOnWindow, 198, horizontalOffset+130, verticalOffset+300, &r);

	tb_CtoPStr31("MyName",playerName);
	strcpy(myPassword, "password");

	cr_CreatureControls();
	pl_InitPlayers();
	it_InitializeItems();
	in_InitializeContainers();
	sh_InitializeShops();

	if (nw_DoJoin()!=0)
	{
		nw_ShutdownNetworking();
		onLine=false;
		sp_InitSplash();
		UpdateEvent(0);
	}
	else
	{
		tx_AddText(kWelcomeMsg);
		tb_FlushMouseEvents();
		//sh_ClearShopContents();
	}

	nextSwitch=TickCount()+(60*5);

}

// ----------------------------------------------------------------- //
void sp_AutoLogoff (void)	// beta
// ----------------------------------------------------------------- //

{
	//TRect								r;
	//char								dummy[kStringLength];


	nw_SendAction(kLogOff, kHighPriority, kSendNow,true);
	nw_ShutdownNetworking();
	onLine=false;
	sp_InitSplash();
	UpdateEvent(0);
	nextSwitch=TickCount()+(60*tb_Rnd(0,45));//30);

}

// ----------------------------------------------------------------- //
void sp_SplashKeyDownEvent (char key)
// ----------------------------------------------------------------- //

{
	TRect								r;
	char								dummy[kStringLength];


	if ((int)key==8)
	{
		if (typingOn==1)
		{
			if (strlen(spName)>0)
			{
				spName[strlen(spName)-1]='\0';
				sp_DrawSplashText(true);
			}
		}
		else if (typingOn==2)
		{
			if (strlen(spPassword)>0)
			{
				spPassword[strlen(spPassword)-1]='\0';
				sp_DrawSplashText(true);
			}
		}
		else if (typingOn==3)
		{
			if (strlen(spIP)>0)
			{
				spIP[strlen(spIP)-1]='\0';
				sp_DrawSplashText(true);
			}
		}

		return;
	}

	if ((int)key==9)
	{
		if (typingOn==1)
			typingOn=2;
		else if (typingOn==2)
			typingOn=3;
		else
			typingOn=1;

		sp_DrawSplashText(true);

		return;
	}

	if (((int)key==13) || ((int)key==3))
	{
		tb_DrawPictureToBitmap(tbLogOnWindow, 199, 316, 352, &r);
		onLine	=	true;
		displayAreaButtonCurrent[0]=-1;
		displayAreaButtonCurrent[1]=-1;
		displayAreaButtonCurrent[2]=-1;
		displayAreaButtonCurrent[3]=-1;
		displayAreaButtonCurrent[4]=-1;
		displayAreaButtonCurrent[5]=-1;
		displayAreaButtonCurrent[6]=-1;
		displayAreaButtonCurrent[7]=-1;
		displayAreaButtonCurrent[8]=-1;
		displayAreaButtonCurrent[9]=-1;
		displayAreaButtonCurrent[10]=-1;
		displayAreaButtonState[0]=kDimButton;
		displayAreaButtonState[1]=kDimButton;
		displayAreaButtonState[2]=kDimButton;
		displayAreaButtonState[3]=kDimButton;
		displayAreaButtonState[4]=kDimButton;
		displayAreaButtonState[5]=kDimButton;
		displayAreaButtonState[6]=kDimButton;
		displayAreaButtonState[7]=kDimButton;
		displayAreaButtonState[8]=kDimButton;
		displayAreaButtonState[9]=kDimButton;
		displayAreaButtonState[10]=kDimButton;
		gotFirstDrawMessage		=	false;
		thisIsFirstRefresh		=	true;
		frame									=	0;
		strcpy(playerText,"");
		playerRow							=	0;
		playerCol							=	0;
		previousPlayerRow			=	0;
		previousPlayerCol			=	0;
		currentInfoDisplay		=	kCharacterDisplay;
		targetMode						=	kNoTarget;
		currentDialog         = 0;
		gr_InitializeRectangles();

		// -- also iniitialize values in mouse down

		pl_InitPlayers();
		in_ClearLocalInventory();
		//nw_InitNetworkingVariables();
		//UpdateEvent(tbLogOnWindow);
		//tb_DrawPictureToBitmap(tbLogOnWindow, 198, 130, 300, &r);
		tb_DrawPictureToBitmap(tbLogOnWindow, 198, 0, 0, &r);

		//tb_CtoPStr31("David",playerName);
		//strcpy(myPassword, "mac");
		tb_CtoPStr31(spName,playerName);
		strcpy(partyFilename, spName);
		strcat(partyFilename," party list");
		strcpy(myPassword, spPassword);
		//np_InitNPCs();
		//cr_InitCreatures();
		it_InitializeItems();
		cr_CreatureControls();
		pl_InitPlayers();
		//it_InitializeItems();
		in_InitializeContainers();
		//rs_InitResources();
		sh_InitializeShops();
		nw_InitNetworkingVariables();
		attemptingReconnect=false;

		if (nw_DoJoin()!=0)
		{
			nw_ShutdownNetworking();
			onLine=false;
			sp_InitSplash();
			UpdateEvent(tbLogOnWindow);
		}
		else
		{
			HideWindow(logonWindow);

			//    gr_HideAllWindows();
			//     windowVisible[kStatsWindow]=true;
			//     windowVisible[kCharacterWindow]=true;
			//     windowVisible[kMapWindow]=true;
			ShowWindow(Window);
			//    gr_OpenFloatingWindow(tbMapWindow);
			//    gr_OpenFloatingWindow(tbCharacterWindow);
			//    gr_OpenFloatingWindow(tbStatsWindow);
			//ShowWindow(statsWindow);
			//ShowWindow(characterWindow);
			//ShowWindow(mapWindow);
			onLine=true;
			tx_AddText(kWelcomeMsg);
			gr_AddText("----------------------",false,true);
			tb_FlushMouseEvents();
			cm_ReadParty();
		}

		return;
	}

	strcpy(dummy,"");

	dummy[0]=key;
	dummy[1]='\0';

	if (typingOn==1)
	{
		if (strlen(spName)<18)
			strcat(spName,dummy);
		else
			return;
	}
	else if (typingOn==2)
	{
		if (strlen(spPassword)<18)
			strcat(spPassword,dummy);
		else
			return;
	}
	else if (typingOn==3)
	{
		if (strlen(spIP)<15)
			strcat(spIP,dummy);
		else
			return;
	}

	sp_DrawSplashText(true);

}

// ----------------------------------------------------------------- //
void sp_SplashMouseDownEvent (int /*theWindow*/, TPoint pt)
// ----------------------------------------------------------------- //

{
	TRect									r;
	TRect									connectRect;
	TRect									quitRect;
	TRect									nameRect;
	TRect									passwordRect;
	TRect									ipRect;
	TPoint								mpt;
	//TRect                 c;


	tb_SetRect(&nameRect,48,211,461,240);
	tb_SetRect(&passwordRect,48,252,461,281);
	tb_SetRect(&ipRect,48,292,461,322);

	tb_SetRect(&connectRect,316,352,451,394);
	tb_SetRect(&quitRect,59,352,194,394);


	if (tb_PointInRect(pt, &nameRect))
	{
		typingOn=1;
		sp_DrawSplashText(true);
	}

	if (tb_PointInRect(pt, &passwordRect))
	{
		typingOn=2;
		sp_DrawSplashText(true);
	}

	if (tb_PointInRect(pt, &ipRect))
	{
		typingOn=3;
		sp_DrawSplashText(true);
	}

	if (tb_PointInRect(pt, &quitRect))
	{
		tb_DrawPictureToBitmap(tbLogOnWindow, 200, quitRect.left, quitRect.top, &r);

		do
		{
			tb_SetPort(tbLogOnWindow);
			tb_GetMousePosition(&mpt);

			if (tb_PointInRect(mpt, &quitRect))
				tb_DrawPictureToBitmap(tbLogOnWindow, 200, quitRect.left, quitRect.top, &r);
			else
				tb_DrawPictureToBitmap(tbLogOnWindow, 202, quitRect.left, quitRect.top, &r);
		}
		while (tb_IsButtonDown());

		tb_DrawPictureToBitmap(tbLogOnWindow, 202, quitRect.left, quitRect.top, &r);

		if (tb_PointInRect(mpt, &quitRect))
			Done=true;
	}

	if (tb_PointInRect(pt, &connectRect))
	{
		tb_DrawPictureToBitmap(tbLogOnWindow, 199, connectRect.left, connectRect.top, &r);

		do
		{
			tb_SetPort(tbLogOnWindow);
			tb_GetMousePosition(&mpt);

			if (tb_PointInRect(mpt, &connectRect))
				tb_DrawPictureToBitmap(tbLogOnWindow, 199, connectRect.left, connectRect.top, &r);
			else
				tb_DrawPictureToBitmap(tbLogOnWindow, 201, connectRect.left, connectRect.top, &r);
		}
		while (tb_IsButtonDown());

		tb_DrawPictureToBitmap(tbLogOnWindow, 201, connectRect.left, connectRect.top, &r);

		if (tb_PointInRect(mpt, &connectRect))
		{
			onLine=true;
			displayAreaButtonCurrent[0]=-1;
			displayAreaButtonCurrent[1]=-1;
			displayAreaButtonCurrent[2]=-1;
			displayAreaButtonCurrent[3]=-1;
			displayAreaButtonCurrent[4]=-1;
			displayAreaButtonCurrent[5]=-1;
			displayAreaButtonCurrent[6]=-1;
			displayAreaButtonCurrent[7]=-1;
			displayAreaButtonCurrent[8]=-1;
			displayAreaButtonCurrent[9]=-1;
			displayAreaButtonCurrent[10]=-1;
			displayAreaButtonState[0]=kDimButton;
			displayAreaButtonState[1]=kDimButton;
			displayAreaButtonState[2]=kDimButton;
			displayAreaButtonState[3]=kDimButton;
			displayAreaButtonState[4]=kDimButton;
			displayAreaButtonState[5]=kDimButton;
			displayAreaButtonState[6]=kDimButton;
			displayAreaButtonState[7]=kDimButton;
			displayAreaButtonState[8]=kDimButton;
			displayAreaButtonState[9]=kDimButton;
			displayAreaButtonState[10]=kDimButton;
			gotFirstDrawMessage		=	false;
			thisIsFirstRefresh		=	true;
			frame									=	0;
			strcpy(playerText,"");
			playerRow							=	0;
			playerCol							=	0;
			previousPlayerRow			=	0;
			previousPlayerCol			=	0;
			currentInfoDisplay		=	kCharacterDisplay;
			targetMode						=	kNoTarget;
			currentDialog         = 0;
			gr_InitializeRectangles();

			// -- also iniitialize values in key down

			pl_InitPlayers();
			in_ClearLocalInventory();

			nw_InitNetworkingVariables();

			//UpdateEvent(tbLogOnWindow);
			//tb_SetForeColor(tbLogOnWindow, 0, 0, 0);
			//tb_SetRect(&c,0,0,512,512);
			//tb_PaintRect(tbLogOnWindow,&c);
			tb_DrawPictureToBitmap(tbLogOnWindow, 198, 0, 0, &r);

			tb_CtoPStr31(spName,playerName);
			strcpy(partyFilename, spName);
			strcat(partyFilename," party list");
			strcpy(myPassword, spPassword);

			//np_InitNPCs();
			//cr_InitCreatures();
			it_InitializeItems();
			cr_CreatureControls();
			pl_InitPlayers();
			//it_InitializeItems();
			in_InitializeContainers();
			//rs_InitResources();
			sh_InitializeShops();
			nw_InitNetworkingVariables();

			attemptingReconnect=false;

			if (nw_DoJoin()!=0)
			{
				nw_ShutdownNetworking();
				onLine=false;
				sp_InitSplash();
				UpdateEvent(tbLogOnWindow);
			}
			else
			{
				HideWindow(logonWindow);
				//gr_HideAllWindows();
				//windowVisible[kStatsWindow]=true;
				//windowVisible[kCharacterWindow]=true;
				//windowVisible[kMapWindow]=true;
				ShowWindow(Window);
				//gr_OpenFloatingWindow(tbMapWindow);
				//gr_OpenFloatingWindow(tbCharacterWindow);
				//gr_OpenFloatingWindow(tbStatsWindow);
				tx_AddText(kWelcomeMsg);
				gr_AddText("----------------------",false,true);
				tb_FlushMouseEvents();
				cm_ReadParty();
			}
		}

	}


}

// ----------------------------------------------------------------- //
void sp_SplashNullEvent(void)
// ----------------------------------------------------------------- //

{

	tb_SetCursor(0);

}

#endif