#ifndef _GRAPHICS
#define _GRAPHICS

#include "Keys.h"
#include "Remap.h"
#include "Globals.h"
#include "MacTextEditor.h"

#define 						kElevated								true
#define 						kNotElevated						false

#define 						kMainBMP								1
#define 						kHavenBMP								2
#define 						kTownBMP								3
#define 						kIceBMP								  4
#define 						kDungeonBMP							5
#define 						kCaveBMP								6

#define 						kMapNull								0
#define 						kMapStructure						1
#define 						kMapRoad								1
#define 						kFlagStructure					9

#define 						kMaxLightSources				100

extern TWindow									tbWindow;										// main window

extern TBoolean									standingInBank;
extern TBoolean									standingOnActionSpot;

extern int									idCreatureID;
extern int									idTitle;
extern int									idGender;
extern char									idPlayerName[kStringLength];
extern char									idGuildName[kStringLength];
extern char                 				idPetName[kStringLength];
extern char									idNPCName[kStringLength];
extern int									idClass;
extern int									idClassLevel;
extern int									idRightHand;
extern int									idLeftHand;
extern int									idLegs;
extern int									idTorso;
extern int									idFeet;
extern int									idArms;
extern int									idRightFinger;
extern int									idLeftFinger;
extern int									idHead;
extern int									idNeck;
extern int									idHands;
extern int									idWaist;
extern int									idKarma;
extern char									idCustomTitle[kStringLength];

extern int									localEffectsID				[kMaxLocalFX];
extern UInt8								localEffectsType			[kMaxLocalFX];
extern UInt8								localEffectsLocation	[kMaxLocalFX];
extern TBoolean							localEffectsServiced	[kMaxLocalFX];
extern int									currentCategory;
extern int									currentSkillDisplay;
extern TRect								destRect[kMaxScreenElements];							// screen rectangles of play area
extern TRect								destScreenRect[kMaxScreenElements];							// screen rectangles of play area

extern long									playerDeaths;
extern long									creatureDeaths;
extern long									cycles;
extern TBoolean							nightVision;
extern TBoolean							inDungeon;

//--------------------------------------------------------------

void gr_MakeRect								(TRect *r,int left, int top, int width, int height);
void gr_InitializeRectangles		(void);
void gr_RenderPlayArea					(int xOff, int yOff,TBoolean drawOnScreen);
void gr_RenderInventory					(TBoolean drawOnScreen);
void gr_CreateBitmaps						(void);
void gr_Animate									(void);
void gr_AddText									(char	newLine[kStringLength], TBoolean systemMessage, TBoolean log);
void gr_RenderPlayerDisplay			(TBoolean drawOnScreen);
int  gr_DistanceFromPlayer			(TPoint pt); // client
void gr_RenderCharacterDisplay	(TBoolean drawOnScreen);
void gr_BuildHuman							(int i, TBoolean onRaft, TBoolean maskOnly);
void gr_MarkArmedInventoryItems	(void); // client
void gr_CreateElement						(int id,int left, int top, int right, int bottom, int grid, int theBase, TBoolean isElevated, int domain, int color);
void gr_RenderMapView						(TBoolean drawOnScreen);
//void gr_DrawButtons							(TBoolean drawOnScreen);
void gr_RenderPackContents			(TBoolean drawOnScreen);
void gr_RenderBankContents			(TBoolean drawOnScreen);
void gr_GetCreatureRect					(int i, TRect	*r);
void gr_RenderMakeList					(TBoolean drawOnScreen, int skill, int category);
void gr_AddLocalFX							(UInt8 position, UInt16 id, UInt8 type, UInt8 dest); // client
void gr_KillLocalFX							(void); // client
void gr_RenderMakeCategory			(TBoolean drawOnScreen, int skill);
void gr_SwapBitmaps							(void);
TBoolean gr_SnowAt							(int row, int col);
void gr_RenderIDDisplay					(TBoolean drawOnScreen);
void gr_DrawServerMain					(void); // server only
void gr_DrawServerNameAt				(int i); // server only
void gr_ShowServerStats					(void); // server only
void gr_DrawButtonPicture				(int bmp, int id, int x, int y, int button, int state, TBoolean drawOnScreen);
int  gr_ActiveButtonOf          (int i);
void gr_RenderActionDisplay			(TBoolean drawOnScreen);
TBoolean gr_ClickedOnCreatureRegion (TPoint pt, int i);
void gr_RenderChatDisplay       (TBoolean drawOnScreen, int buttonDown);
void gr_AddToChatLog            (char	newLine[kStringLength], int messageType);
void gr_RenderPlayArea2         (TBoolean drawOnScreen);
TBoolean gr_CheckMouseOnUpButton    (TPoint pt);
TBoolean gr_CheckMouseOnDownButton  (TPoint pt);
TBoolean gr_DragThumb               (TPoint pt);
TBoolean gr_CheckMouseOnPageUpButton(TPoint pt);
TBoolean gr_CheckMouseOnPageDownButton(TPoint pt);
void gr_ShowDebugLocation         (int d); // server only
void gr_RenderMapWindow           (TBoolean drawOnScreen);
void gr_RenderPlayArea3           (void);
void gr_RenderMapWindow           (TBoolean drawOnScreen);
void gr_RenderMapWindow3(void);
void gr_HideAllWindows            (void);
void gr_OpenFloatingWindow        (int theWindow);
void gr_CloseFloatingWindow(int theWindow);
void gr_CompressWindowStack(void);
void gr_RemoveFromWindowStack(int theWindow);
void gr_AddToWindowStack(int theWindow);
int gr_TopOfWindowStack(void);
void gr_RenderBankWindow(void);
void gr_OpenFloatingWindowID(int theWindow);
void gr_SaveWindowPrefs(void);
TBoolean gr_PointerOverFloatingWindow(void);
void gr_TempHideAllWindows(void);
void gr_RenderTextLine(void);

void gr_RenderInventoryWindow           (void);
void gr_RenderPackContentsWindow        (void);
void gr_RenderShopContentsWindow        (void);
void gr_RenderSellListWindow            (void);
void gr_RenderIDDisplayWindow           (void);
void gr_RenderMakeListWindow            (int skill, int category);
void gr_RenderSkillsWindow              (void);
void gr_RenderStatsWindow               (void);
void gr_RenderChatDisplayWindow         (int buttonDown);
void gr_RenderCharacterDisplayWindow    (void);
void gr_RenderChessWindow				(void);
void gr_RenderCalendarWindow			(void);
void gr_RenderPartyWindow               (void);
void gr_RenderQuestWindow               (void);
void gr_RenderPetsWindow                (void);
void gr_RenderAboutWindow               (void);
void gr_Resume                          (void);
void gr_DrawAllWindows									(void);
void gr_HiliteWindow                    (int theWindow);
void gr_HiliteToolboxWindow             (int theWindow);
void gr_RenderWindow                    (int theWindow);
void gr_CloseFloatingWindowID           (int theWindow);
void gr_CreateDistanceSpell				(int cc, int ct, int theSpell);

//--------------------------------------------------------------

#endif