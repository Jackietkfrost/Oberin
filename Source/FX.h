#ifndef _FX
#define _FX

////////////////////////////////////////////////////////////////////
// See FX.c for comments
////////////////////////////////////////////////////////////////////

#include "toolbox.h"
#include "Globals.h"

#define _MAXEFFECTS	250
#define	_MAXPARTICLES 50

#define	kfxRandom	-1
#define	kfxRed		0
#define	kfxGreen	1
#define	kfxBlue		2

#define	kfxStill	0
#define	kfxJitter	1
#define	kfxRise		2
#define	kfxFall		3

#define	kfxStarID	5000
#define	kfxStarFrames	4
#define	kfxFlare0ID	4000		
#define	kfxFlare1ID	4001		
#define	kfxFlare2ID	4002		
#define	kfxFlare3ID	4003		
#define	kfxFlare4ID	4004

#define	kfxFireballID	4700
#define kfxMMID			4800
#define kfxMassHealID	4140
#define	kfxExplosionID	4130
#define kfxBuffID		5000
#define kfxResID		4100
#define kfxDomeID		4900

#define kfxFireblastID	4200
#define kfxFireblast2ID 4110
#define kfxFireblast3ID 4300
#define kfxFireblast4ID 4400
#define kfxFireblast5ID 4500

#define kfxSpatAxeID 	4340
#define kfxSummonID 	4240
#define kfxFBFieldID 	4540
#define kfxSpatSwordID 	4640

#define	kfxBrackRoot	4999

#define	kfxLightSource		0
#define	kfxLightRing		1
#define	kfxLightEllipse		2
#define kfxLightEllipseRing	3

#define	kfxFireCone			0
#define kfxFireWall			1

#define kfxWhiteFire		-1
#define kfxRedFire			0
#define kfxGreenFire		1
#define kfxBlueFire			2

#define	kfxBlendNone		0
#define	kfxBlendAll			1
#define	kfxBlendVert		2
#define	kfxBlendHorz		3
#define	kfxBlendDiagL		4
#define	kfxBlendDiagR		5

#define kfxFlatBottom		0
#define kfxRoundBottom		1
#define kfxCornerBottom		2
#define kfxSlantLBottom		3
#define kfxSlantRBottom		4

typedef struct _TFXSparklesData
{
	int			x;
	int			y;
	int			r;	
	TBoolean	Active;	
	int			color;
	int			num;
	int			size;
	int			xi[100];
	int			yi[100];
	int			c[100];
	int			d[100];
	TBoolean	start;
	int			dx, dy;
	int			motion;
} TFXSparklesData;

typedef	struct _TFXLightFlash
{
	int			x;
	int			y;	
	TBoolean	Active;
	int			color;
	int			radius;
	int			w;
	int			h;
	TBoolean	start;
	int			wsize;
	int			hsize;
	int			dw;
	int			dh;
	int			type;
	int			step;
	int			level;
} TFXLightFlash;

typedef	struct _TFXParticle
{	
	float	x;
	float	y;
	float	vi;	
	float	angle;
	int		life;
	int		r;
	int		g;
	int		b;
	int		time;
	float			gravity;
	TBoolean	Active;
	int		clr;
} TFXParticle;

typedef struct _TFXParticleExplosion
{
	TFXParticle		p[_MAXPARTICLES];
	int				force;
	int				x;
	int				y;
	TBoolean		Active;		
} TFXParticleExplosion;

typedef struct	_TFXParticleHose
{
	TFXParticle		p[_MAXPARTICLES];
	int				force;
	int				x;
	int				y;
	TBoolean		Active;	
	float			angle;
	int				life;
	float			gravity;
	int				da;
	int				clr;
} TFXParticleHose;

typedef struct _TFXFireParticle
{
	int	life;
	int age;
	unsigned int ri;
	unsigned int gi;
	unsigned int bi;
	unsigned int rn;
	unsigned int gn;
	unsigned int bn;
	int	x;
	int y;
} TFXFireParticle, *pTFXFireParticle;

typedef struct _TFXFire
{
	int	w;
	int	h;
	pTFXFireParticle	p;
	TBoolean	Active;
	TRect	r;
	int	shape;
	int clr;
	int	blend;
	TBoolean invert;
	TBitmap	buf;
	int bottomShape;
} TFXFire;

//------------------------------------------------------------

extern	UInt8									fxMap				[kMapWidth][kMapHeight];
extern	UInt8									serverFX		[kMaxFX];
extern	UInt8									dayNight;
extern	UInt8									serverFX						[kMaxFX];
extern	unsigned long					serverFXKillTime		[kMaxFX];
extern	UInt16								serverFXRow					[kMaxFX];
extern	UInt16								serverFXCol					[kMaxFX];

//------------------------------------------------------------

int 		fx_AddServerFX(int type, int life, int row, int col, int misc, int misc2); // server
void 		fx_CreatureServerFX(int cc, int ct, int type, int life, int misc); // server
void 		fx_ServiceFX(void); // server
TBoolean 	fx_MovingFX(int fx);

void		fx_Initialize(void);

int			fx_CreateSparkles(int x, int y, int r, int color, int num, int size, int motion);
void		fx_DisposeSparkles(int index);
//TBoolean	fx_DrawSparkles(TBitmap bitmap, int index);
TBoolean	fx_DrawSparkles(TBitmap bitmap, int index, int nx, int ny);

int			fx_CreateFlash(int x, int y, int w, int h, int color, int type);
void		fx_DisposeFlash(int index);
TBoolean	fx_DrawFlash(TBitmap bitmap, int index, int nx, int ny);

void		fx_LensFlare(TBitmap bitmap, int x, int y, float angle, int length, unsigned int r, unsigned int g, unsigned int b, int alpha);

TBoolean	fx_UpdateParticleState(TFXParticle* p, int dtime);

void		fx_DrawFireball(int bitmap, int tot, int fr, int x, int y);
void		fx_MagicMissile(int bitmap, int tot, int fr, int x, int y);
void		fx_MassHeal(int bitmap, int tot, int fr, int x, int y);
void		fx_Buff(int bitmap, int buff, int tot, int fr, int x, int y);
void		fx_Res(int bitmap, int tot, int fr, int x, int y);
void		fx_Explosion(int bitmap, int tot, int fr, int x, int y);
void		fx_Dome(int bitmap, int type, int tot, int fr, int x, int y);

void		fx_Fireblast(int bitmap, int tot, int fr, int x, int y);
void		fx_Fireblast2(int bitmap, int tot, int fr, int x, int y);
void		fx_Fireblast3(int bitmap, int tot, int fr, int x, int y);
void		fx_Fireblast4(int bitmap, int tot, int fr, int x, int y);
void		fx_Fireblast5(int bitmap, int tot, int fr, int x, int y);

//TODO merge
void		fx_CreatureSummon(int bitmap, int tot, int fr, int x, int y);
void		fx_SpatSword(int bitmap, int tot, int fr, int x, int y);							
void		fx_SpatAxe(int bitmap, int tot, int fr, int x, int y);
void		fx_FBField(int bitmap, int tot, int fr, int x, int y);
void		fx_BrackenRoot(int bitmap, int tot, int fr, int x, int y);

int			fx_CreateParticleExplosion(int x, int y, int force, int life, float gravity, float angle, int clr);
void		fx_DisposeParticleExplosion(int index);
TBoolean	fx_DrawParticleExplosion(TBitmap bitmap, int index);

int			fx_CreateParticleHose(int x, int y, int force, int life, float gravity, float angle, int da, int clr);
void		fx_DisposeParticleHose(int index);
//TBoolean	fx_DrawParticleHose(TBitmap bitmap, int index);
TBoolean	fx_DrawParticleHose(TBitmap bitmap, int index, int nx, int ny);

int		fx_CreateFire(TRect r, int shape, int clr, int blend, TBoolean invert, int bottomShape, TBoolean prime);
void	fx_DisposeFire(int index);
void	fx_DrawFire(TBitmap bitmap, int index, int nx, int ny);
void	fx_GrowFire(int index, int height);
void	fx_PrimeFlames(int index);

void	fx_DrawFire2(TBitmap bitmap, int index);
void	fx_DrawFire3(TBitmap bitmap, int index);
void	fx_DrawFire4(TBitmap bitmap, int index);
void	fx_DrawFire5(TBitmap bitmap, int index);

#endif
