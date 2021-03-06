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
#include "FX.h"
#include "Action.h"
#include "GameText.h"


TWindow							tbWindow;										// main window
TBoolean						onLine;
#ifdef	_SERVER
UInt16							map[kMapWidth][kMapHeight];
#endif
//UInt16							map2[kMapWidth][kMapHeight];
UInt16							mapToScreenRow[kMaxScreenElements];
UInt16							mapToScreenCol[kMaxScreenElements];

TRect								clickBaseRect						[kMaxScreenElements];
TRect								clickObjectRect					[kMaxScreenElements];
TRect								clickCreatureRect				[kMaxScreenElements];
//RgnHandle						clickCreatureRgn				[kMaxScreenElements];
TRect								clickTerrainRect				[kMaxScreenElements];
int									clickTerrainID					[kMaxScreenElements];

TBoolean						makeCategory						[kMaxInventorySize];

TRect								inventoryDropRect[kMaxInventorySize];
TRect								pickUpRect[kMaxInventorySize];
TRect								inventoryGiveRect[kMaxInventorySize];
TRect								inventoryUseRect[kMaxInventorySize];
TRect								skillDirectionRect[kMaxSkills];
TRect								skillRect[kMaxSkills];

TRect								characterTabRect;
TRect								inventoryTabRect;
TRect								skillsTabRect;

TRect								displayAreaButton[11];
int									displayAreaButtonState[11];
int									displayAreaButtonCurrent[11];

int									currentInfoDisplay;

UInt16							playerRow;
UInt16							playerCol;

UInt16							previousPlayerRow;
UInt16							previousPlayerCol;

UInt16							playerID;

UInt16							state[kMaxScreenElements];
TBoolean						poisoned[kMaxScreenElements];
TBoolean						cursed[kMaxScreenElements];
TBoolean						isPet[kMaxScreenElements];
TBoolean						regenerating[kMaxScreenElements];
TBoolean						afk[kMaxScreenElements];
TBoolean						gmChar[kMaxScreenElements];
TBoolean						questChar[kMaxScreenElements];
TBoolean						stealthChar[kMaxScreenElements];
TBoolean            			playerOnline[kMaxScreenElements];
TBoolean						morphed[kMaxScreenElements];

int									frame;

TRegion							directionRgn[8];

char								playerText[kStringLength];    //[kCommandStringMaxLength];

TRect								chatUpRect;
TRect								chatDownRect;
TRect								chatThumbRect;
int                 startChatLine;
TRect								chatPageUpRect;
TRect								chatPageDownRect;

TBoolean            chimeOn;
int									targetMode;
TBoolean						warOn=false;
int									monsterTime; // beta
int									refreshTime; // beta
int									decayTime; // beta
int									resourceTime; // beta
int									rotateShopTime; // beta
int									serverFXTime; // beta

int									moveCount=0;//beta

char								message[kStringLength]; //[128];
char								itemName[kStringLength]; //[48];

int									playerAction=0;	// used for dropping multiple items
int									targetSlot;	// used for dropping/giving multiple items
int									targetCount;	// used for dropping/giving multiple items
int                 insertionPoint;

int									previousTile=256;
TBoolean						hudOn;
TBoolean						namesOn;
TBoolean						musicOn;
TBoolean						pathFind;
unsigned long       nextPathTime;
unsigned long       nextHouseKeeping;
int                 useSlot;
TBoolean            keyPadControl;
TBoolean			backgroundnotification;
TBoolean			colorOwnName;

int					currentDragLine;
int					currentDragDest;
WindowRef			dragInitWindow;

char                f1Str[kStringLength]; //[kCommandStringMaxLength];
char                f2Str[kStringLength]; //[kCommandStringMaxLength];
char                f3Str[kStringLength]; //[kCommandStringMaxLength];
char                f4Str[kStringLength]; //[kCommandStringMaxLength];
char                f5Str[kStringLength]; //[kCommandStringMaxLength];
char                f6Str[kStringLength]; //[kCommandStringMaxLength];
char                f7Str[kStringLength]; //[kCommandStringMaxLength];
char                f8Str[kStringLength]; //[kCommandStringMaxLength];
char                f9Str[kStringLength]; //[kCommandStringMaxLength];
char                f10Str[kStringLength]; //[kCommandStringMaxLength];
char                f11Str[kStringLength]; //[kCommandStringMaxLength];
char                f12Str[kStringLength]; //[kCommandStringMaxLength];
char                f13Str[kStringLength]; //[kCommandStringMaxLength];
char                f14Str[kStringLength]; //[kCommandStringMaxLength];
char                f15Str[kStringLength]; //[kCommandStringMaxLength];

#ifdef _SERVER
#ifndef _SERVERONLY
int									clearMap=0;
#endif
#endif

#ifndef _SERVERONLY
char				localMacros[kMaxMacros][2][kStringLength];
#endif

extern unsigned long					nextStep;
extern unsigned long					nextRender;
extern Boolean 								gHost;
extern UInt32									lastRefreshTick;

extern char							textLog[1000];
extern unsigned int					logTime;

extern EventRecord 					*gMouseDownRef;
extern DragTrackingHandlerUPP		gDragTrackingHandlerUPP;
extern DragReceiveHandlerUPP		gDragReceiveHandlerUPP;
extern PicHandle					PHandle;

extern TBoolean						logText;
extern TBoolean           skipOldMessages;
extern TBoolean			  displayFeedback;

extern int                gSoundVolume;
extern int                gMusicVolume;

extern int					horizontalOffset;
extern int					verticalOffset;
extern TRect				playScreenRect;
extern TRect				playOSScreenRect;
extern TRect				playRect;
extern int					currentDialog;
extern TRect				currentDialogButton1;
extern TRect				currentDialogButton2;
extern TBoolean     darkTextBacks;
extern TBitmap			textLineBMP;
extern TRect        textLineScreenRect;

extern int					xOffset;
extern int					yOffset;
extern playerLocalSkillsType				localSkills				[kMaxSkills];
extern itemType			itemList						[kMaxItems];
extern int									flip; // beta
extern TBoolean														canStep;
extern TBoolean							applicationSuspended;
extern FSSpec				AppFSSpec;
extern TRect        chessScreenRect[96];
extern int          chessSelectedPiece;
extern int					actionValue;
extern TBoolean			canSwimIn[kMaxIcons];						// specifies whether object is water
extern int          magicLoreTarget;
extern Movie				theMusic;
extern TBoolean			gotFirstRefresh;
extern char         localContainerName[kStringLength]; //[15];
extern void startMusic (void);
extern inventoryType	localSortedInventory		[kMaxInventorySize];
extern int					localInventoryLookup		[kMaxInventorySize];
extern TRect        currentMapWindowsRect;
extern TRect		inventoryWindowRect;
extern TRect		bankWindowRect;
extern int          gxoffset;
extern int          gyoffset;

extern int          tbMapWindow;
extern int          tbInventoryWindow;
extern int          tbPackContentsWindow;
extern int          tbShopContentsWindow;
extern int          tbSellListWindow;
extern int          tbIDWindow;
extern int          tbMakeWindow;
extern int          tbSkillsWindow;
extern int          tbStatsWindow;
extern int          tbChatWindow;
extern int          tbCharacterWindow;
extern int          tbChessWindow;
extern int          tbLogOnWindow;
extern int          tbBankWindow;
extern int          tbPartyWindow;
extern int          tbQuestWindow;
extern int          tbPetsWindow;
extern int          tbAboutWindow;
extern int			tbCalendarWindow;

extern UInt32       cursorBlinkTime;
extern TBoolean			drawHealthBars;

extern WindowPtr    makeWindow;
extern WindowPtr	inventoryWindow;
extern WindowPtr	bankWindow;

extern TPoint       mainWindowPosition;
extern TPoint       windowPosition[kMaxWindows];
extern TBoolean     windowVisible[kMaxWindows];
extern TBoolean     windowActive[kMaxWindows];
extern inventoryType	bankInventory				[kMaxInventorySize];
extern char					localParty[16][kStringLength]; //[15];
extern TBoolean     standingOnContainer;

extern int                 localPetStatus          [kMaxPets];
extern int                 localPetType            [kMaxPets];
extern char                localPetName            [kMaxPets][kStringLength];
extern int                 localPetLoyalty         [kMaxPets];
extern char									partyFilename[kStringLength];
extern int				        creatureID[kMaxScreenElements];

extern WindowPtr		mapWindow;
extern void GetKeyEvents(void);

extern UInt8				localRightFinger;
extern UInt8				localLeftFinger;
extern UInt8				localClass;
extern UInt8				localSpeed;

extern int          		maxSkillLevel[7][kMaxSkills];


// ----------------------------------------------------------------- //
void cm_ClearParty(void)
// ----------------------------------------------------------------- //

{
	int     i;

	for (i=0;i<16;i++)
		strcpy(localParty[i],"");

}

// ----------------------------------------------------------------- //
void cm_ReadParty(void)
// ----------------------------------------------------------------- //

{
	int         	i;

	if (strcmp(partyFilename,"")==0)
		return;

	if (tb_OpenPartyFileForInput(partyFilename)==0)
	{

		for (i=0;i<16;i++)
			if (tb_ReadFromFile(&localParty[i][0],sizeof(localParty[i]))!=0)								goto fileError;

		tb_CloseFile();

		return;
	}

fileError:

	tb_CloseFile();
	cm_ClearParty();
	//cm_WriteParty();

}

// ----------------------------------------------------------------- //
void cm_WriteParty(void)
// ----------------------------------------------------------------- //

{
	int       i;


	if (strcmp(partyFilename,"")==0)
		return;

	if (tb_OpenPartyFileForOutput(partyFilename)==0)
	{
		for (i=0;i<16;i++)
			tb_WriteToFile(&localParty[i][0],     sizeof(localParty[i]));

		tb_CloseFile();
	}

}

// ----------------------------------------------------------------- //
void cm_ClearPrefs(void)
// ----------------------------------------------------------------- //

{
	int     i;

	fi_WriteToErrorLog("Clearing Prefs");

	tb_SoundOn			      =	true;
	musicOn		    	      =	true;
	hudOn	        	      =	false;
	pathFind      	      = false; //Dave 11-23-2012 true
	namesOn       	      = true;
	drawHealthBars	      = true;
	currentSkillDisplay   = 0;
	currentCategory       = 0;
	chimeOn               = true;
	logText								= false;
	displayFeedback			= true;
	keyPadControl         	= true;  //Dave 11-23-2012 false
	backgroundnotification	= true;
	colorOwnName				= false;
	darkTextBacks         	= false;
	gSoundVolume          	= 7;
	gMusicVolume          	= 7;
	skipOldMessages       	= true;
	tb_PlaySound(9999);

	strcpy(f1Str,"");
	strcpy(f2Str,"");
	strcpy(f3Str,"");
	strcpy(f4Str,"");
	strcpy(f5Str,"");
	strcpy(f6Str,"");
	strcpy(f7Str,"");
	strcpy(f8Str,"");
	strcpy(f9Str,"");
	strcpy(f10Str,"");
	strcpy(f11Str,"");
	strcpy(f12Str,"");
	strcpy(f13Str,"");
	strcpy(f14Str,"");
	strcpy(f15Str,"");

	for (i=0;i<16;i++)
		strcpy(localParty[i],"");

	for (i=0;i<kMaxWindows;i++)
	{
		windowPosition[i].h=532-6;
		windowPosition[i].v=38-22+6;
		windowVisible[i]=false;
		windowActive[i]=false;
	}

	mainWindowPosition.h=8-6;

	mainWindowPosition.v=44-22;

	windowPosition[kStatsWindow].h      = 532-6;
	windowPosition[kStatsWindow].v      = 38-22+6;
	windowVisible[kStatsWindow]         = true;
	windowActive[kStatsWindow]         = true;

	windowPosition[kCharacterWindow].h  = 532-6;
	windowPosition[kCharacterWindow].v  = 128-22+6;
	windowVisible[kCharacterWindow]     = true;

	windowPosition[kMapWindow].h        = 532-6;
	windowPosition[kMapWindow].v        = 458-22+6;
	windowVisible[kMapWindow]           = true;

}

// ----------------------------------------------------------------- //
void cm_ReadPrefs(void)
// ----------------------------------------------------------------- //

{
	int         	i;
	char					dummy[16][kStringLength]; //[15];

	if (tb_OpenPrefsForInput()==0)
	{
		if (tb_ReadFromFile(&pathFind, 		          sizeof(pathFind))!=0)			      goto fileError;

		if (tb_ReadFromFile(&currentSkillDisplay, 	sizeof(currentSkillDisplay))!=0)			  goto fileError;

		if (tb_ReadFromFile(&currentCategory, 		  sizeof(currentCategory))!=0)			      goto fileError;

		if (tb_ReadFromFile(&f1Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f2Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f3Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f4Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f5Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f6Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f7Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f8Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f9Str[0], 		      sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f10Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f11Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f12Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f13Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f14Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&f15Str[0], 		    sizeof(f1Str))!=0)			        goto fileError;

		if (tb_ReadFromFile(&namesOn, 		      sizeof(namesOn))!=0)            goto fileError;

		if (tb_ReadFromFile(&drawHealthBars, 		sizeof(drawHealthBars))!=0)     goto fileError;

		if (tb_ReadFromFile(&mainWindowPosition,sizeof(mainWindowPosition))!=0) goto fileError;

		if (tb_ReadFromFile(&chimeOn, 		      sizeof(chimeOn))!=0)			      goto fileError;

		for (i=0;i<kMaxWindows;i++)
			if (tb_ReadFromFile(&windowPosition[i],sizeof(windowPosition[i]))!=0) goto fileError;

		for (i=0;i<kMaxWindows;i++)
			if (tb_ReadFromFile(&windowVisible[i],sizeof(windowVisible[i]))!=0)   goto fileError;

		for (i=0;i<16;i++)
			if (tb_ReadFromFile(&dummy[i][0],sizeof(dummy[i]))!=0)								goto fileError;

		if (tb_ReadFromFile(&logText, 		       sizeof(logText))!=0)			      { logText				=	true; }

		if (tb_ReadFromFile(&keyPadControl, 		 sizeof(keyPadControl))!=0)			{ keyPadControl	=	false; }

		if (tb_ReadFromFile(&darkTextBacks, 		 sizeof(darkTextBacks))!=0)			{ darkTextBacks	=	true; }

		if (tb_ReadFromFile(&gSoundVolume, 		   sizeof(gSoundVolume))!=0)			{ gSoundVolume	=	7; }

		if (tb_ReadFromFile(&gMusicVolume, 		   sizeof(gMusicVolume))!=0)			{ gMusicVolume	=	7; }

		if (tb_ReadFromFile(&skipOldMessages, 	 sizeof(skipOldMessages))!=0)		{ skipOldMessages	=	true; }

		if (tb_ReadFromFile(&backgroundnotification, 	 sizeof(backgroundnotification))!=0)		{ backgroundnotification	=	true; }

		if (tb_ReadFromFile(&colorOwnName, 	 sizeof(colorOwnName))!=0)		{ colorOwnName	=	true; }
		
		if (tb_ReadFromFile(&displayFeedback, 	 sizeof(displayFeedback))!=0)		{ displayFeedback	=	true; }


		tb_CloseFile();

//			for (i=0;i<kMaxWindows;i++)
//			  windowPosition[i].v=windowPosition[i].v+2; // gs window bug 11/19

		windowVisible[kChessWindow]         = false;
		windowVisible[kShopContentsWindow]  = false;
		windowVisible[kSellListWindow]      = false;
		windowVisible[kMakeWindow]          = false;
		windowVisible[kPackContentsWindow]  = false;
		windowVisible[kQuestWindow]         = false;
		windowVisible[kIDWindow]            = false;
		windowVisible[kBankWindow]          = false;
		windowVisible[kAboutWindow]         = false;

		tb_PlaySound(9999);
		return;
	}

fileError:

	tb_CloseFile();
	cm_ClearPrefs();
	cm_WritePrefs();

}

// ----------------------------------------------------------------- //
void cm_WritePrefs(void)
// ----------------------------------------------------------------- //

{
	int       i;

	if (tb_OpenPrefsForOutput()==0)
	{
		tb_WriteToFile(&pathFind, 					    sizeof(pathFind));
		tb_WriteToFile(&currentSkillDisplay, 		sizeof(currentSkillDisplay));
		tb_WriteToFile(&currentCategory, 				sizeof(currentCategory));
		tb_WriteToFile(&f1Str[0], 					    sizeof(f1Str));
		tb_WriteToFile(&f2Str[0], 					    sizeof(f2Str));
		tb_WriteToFile(&f3Str[0], 					    sizeof(f3Str));
		tb_WriteToFile(&f4Str[0], 					    sizeof(f4Str));
		tb_WriteToFile(&f5Str[0], 					    sizeof(f5Str));
		tb_WriteToFile(&f6Str[0], 					    sizeof(f6Str));
		tb_WriteToFile(&f7Str[0], 					    sizeof(f7Str));
		tb_WriteToFile(&f8Str[0], 					    sizeof(f8Str));
		tb_WriteToFile(&f9Str[0], 					    sizeof(f9Str));
		tb_WriteToFile(&f10Str[0], 					    sizeof(f10Str));
		tb_WriteToFile(&f11Str[0], 					    sizeof(f11Str));
		tb_WriteToFile(&f12Str[0], 					    sizeof(f12Str));
		tb_WriteToFile(&f13Str[0], 					    sizeof(f13Str));
		tb_WriteToFile(&f14Str[0], 					    sizeof(f14Str));
		tb_WriteToFile(&f15Str[0], 					    sizeof(f15Str));
		tb_WriteToFile(&namesOn, 					      sizeof(namesOn));
		tb_WriteToFile(&drawHealthBars, 				sizeof(drawHealthBars));
		tb_WriteToFile(&mainWindowPosition, 	  sizeof(mainWindowPosition));
		tb_WriteToFile(&chimeOn, 	              sizeof(chimeOn));

		for (i=0;i<kMaxWindows;i++)
			tb_WriteToFile(&windowPosition[i],    sizeof(windowPosition[i]));

		for (i=0;i<kMaxWindows;i++)
			tb_WriteToFile(&windowVisible[i],     sizeof(windowVisible[i]));

		for (i=0;i<16;i++)
			tb_WriteToFile(&localParty[i][0],     sizeof(localParty[i]));

		tb_WriteToFile(&logText, 					    	sizeof(logText));

		tb_WriteToFile(&keyPadControl, 					sizeof(keyPadControl));

		tb_WriteToFile(&darkTextBacks, 					sizeof(darkTextBacks));

		tb_WriteToFile(&gSoundVolume, 					sizeof(gSoundVolume));

		tb_WriteToFile(&gMusicVolume, 					sizeof(gMusicVolume));

		tb_WriteToFile(&skipOldMessages, 				sizeof(skipOldMessages));

		tb_WriteToFile(&backgroundnotification, 				sizeof(backgroundnotification));

		tb_WriteToFile(&colorOwnName, 				sizeof(colorOwnName));
		
		tb_WriteToFile(&displayFeedback, 				sizeof(displayFeedback));

		tb_CloseFile();
	}

	//cm_WriteParty();

}

// ----------------------------------------------------------------- //
void ClearPlayerAction(void)
// ----------------------------------------------------------------- //

{

	if (playerAction==kDropping)
		tx_AddText(kDropCancelledMsg);

	if (playerAction==kGiving)
		tx_AddText(kGiveCancelledMsg);

	if (playerAction==kTaking)
		tx_AddText(kTakeCancelledMsg);

	if (playerAction==kBuying)
		tx_AddText(kBuyCancelledMsg);

	if (playerAction==kSelling)
		tx_AddText(kSellCancelledMsg);

	if (playerAction==kDepositing)
		tx_AddText(kDepositCancelledMsg);

	if (playerAction==kWithdrawing)
		tx_AddText(kWithdrawalCancelledMsg);

	if (playerAction==kLocate)
		tx_AddText(kLocateCancelledMsg);

	playerAction		=	0;

	targetSlot			=	0;

	targetCount			=	0;

	targetMode			=	kNoTarget;  // 0.1.3

	strcpy(playerText,"");  // 0.1.3

	gr_RenderTextLine();

}

/*
// ----------------------------------------------------------------- //
TBoolean PointerOverTarget (TPoint pt)
// ----------------------------------------------------------------- //

{
	int								i;
	int								tm=targetMode;


	if (targetMode==kNoTarget)
		return (false);
 // else
 //   return (true);

	if (targetMode==kAttackTarget)
		{
			for (i=251;i>=0;i--)
				if (i!=90)
					{

						//if (clickCreatureRect[i].bottom!=0)
						//	{
						//		tb_SetPort(tbWindow);
						//		PaintRgn(clickCreatureRgn[i]);
						//	}

						//if ((targetMode==kAttackTarget) || (tm==kGiveTarget) || (tm==kHealing) || (tm==kAnatomy) || (tm==kEvaluatingIntelligence))
						//	{
								//if (tb_PointInRect(pt, &clickCreatureRect[i]))
								  if (gr_ClickedOnCreatureRegion(pt, i))

									//if (PtInRgn(pt, clickCreatureRgn[i]))
										return (true);

						//	}

						//else if (targetMode==kLumberjacking)
						//	{
						//		if ((clickTerrainID[i]>=359) && (clickTerrainID[i]<=368)) // not a tree -------
						//			if (tb_PointInRect(pt, &clickTerrainRect[i]))
						//				if (gr_DistanceFromPlayer(pt)<=100)
						//					return (true);
						//	}

						//else if (targetMode==kMining)
						//	{
						//		if ((clickTerrainID[i]>=536) && (clickTerrainID[i]<=544)) // not a rock -------
						//			if (tb_PointInRect(pt, &clickTerrainRect[i]))
						//				if (gr_DistanceFromPlayer(pt)<=100)
						//					return (true);
						//	}

					}
			return (false);
		}
	else
		return (true);

	return (false);

}
*/
// ----------------------------------------------------------------- //
void CheckKeys(TBoolean /*mouseEvent*/, int keyCode)
// ----------------------------------------------------------------- //

{	
	TPoint						pt;
	int								dir;
	TBoolean					keyMove=false;
	int								minDist;
	int								tempDist;
	int								foundIndex;
	int								centerX;
	int								centerY;
	int               i;

	if (applicationSuspended)
		return;

#ifdef _SERVERONLY
	tb_SetCursor(0);

	return;

#endif

#ifdef _SERVER
#ifndef _SERVERONLY
	if (clearMap!=0)
	{
		map[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=clearMap;
		//clientMap[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=clearMap;
		clientMap[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=clearMap;
	}

#endif
#endif

	//-------------------------------------------------------------------------

	if (keyPadControl)
	{
		if (keyCode!=0)
		{
			if (keyCode==KeyPad8Key)								{	dir=1;	keyMove=true; }
			else if (keyCode==KeyPad9Key)							{	dir=2;	keyMove=true; }
			else if (keyCode==KeyPad6Key)							{	dir=3;	keyMove=true; }
			else if (keyCode==KeyPad3Key)							{	dir=4;	keyMove=true; }
			else if (keyCode==KeyPad2Key)							{	dir=5;	keyMove=true; }
			else if (keyCode==KeyPad1Key)							{	dir=6;	keyMove=true; }
			else if (keyCode==KeyPad4Key)							{	dir=7;	keyMove=true; }
			else if (keyCode==KeyPad7Key)							{	dir=8;	keyMove=true; }
		}
		else
		{
			if (tb_IsKeyDown(KeyPad8Key))					{	dir=1;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad9Key))				{	dir=2;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad6Key))				{	dir=3;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad3Key))				{	dir=4;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad2Key))				{	dir=5;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad1Key))				{	dir=6;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad4Key))				{	dir=7;	keyMove=true; }
			else if (tb_IsKeyDown(KeyPad7Key))				{	dir=8;	keyMove=true; }
		}

		if (keyMove)
		{
			if (tb_IsKeyDown(ControlKey))
			{
				if (dir==1)			  { nw_SendAction(kFace5,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==2)	{ nw_SendAction(kFace4,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }

				else if (dir==3)	{ nw_SendAction(kFace7,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }

				else if (dir==4)	{ nw_SendAction(kFace2,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==5)	{ nw_SendAction(kFace8,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==6)	{ nw_SendAction(kFace3,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==7)	{ nw_SendAction(kFace6,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==8)	{ nw_SendAction(kFace1,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
			}
			else
			{
				if (dir==8) 	 { nw_SendAction(1,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==4) { nw_SendAction(2,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==6) { nw_SendAction(3,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==2) { nw_SendAction(4,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==1) { nw_SendAction(5,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==3) { nw_SendAction(6,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==5) { nw_SendAction(8,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				else if (dir==7) { nw_SendAction(7,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }

				return;
			}
		}
	}

	//-------------------------------------------------------------------------

	if (targetMode!=kInventoryTarget)
		if (gr_PointerOverFloatingWindow())
		{
			tb_SetCursor(0);
			return;
		}

	tb_GetLocalMousePosition(tbWindow, &pt);

	//--------------------------------------------------------------------------

	if (tb_IsKeyDown(ControlKey))
	{
		if (tb_PointInRegion(pt, directionRgn[0]))			{ tb_SetCursor(130);}
		else if (tb_PointInRegion(pt, directionRgn[1]))	{ tb_SetCursor(137);}
		else if (tb_PointInRegion(pt, directionRgn[2]))	{ tb_SetCursor(133);}
		else if (tb_PointInRegion(pt, directionRgn[3]))	{ tb_SetCursor(135);}
		else if (tb_PointInRegion(pt, directionRgn[4]))	{ tb_SetCursor(131);}
		else if (tb_PointInRegion(pt, directionRgn[5]))	{ tb_SetCursor(134);}
		else if (tb_PointInRegion(pt, directionRgn[6]))	{ tb_SetCursor(132);}
		else if (tb_PointInRegion(pt, directionRgn[7]))	{ tb_SetCursor(136);}

		if (tb_IsButtonDown())
		{
			if (tb_PointInRegion(pt, directionRgn[0]))			{ nw_SendAction(kFace5,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }//
			else if (tb_PointInRegion(pt, directionRgn[1]))	{ nw_SendAction(kFace4,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
			else if (tb_PointInRegion(pt, directionRgn[2]))	{ nw_SendAction(kFace7,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
			else if (tb_PointInRegion(pt, directionRgn[3]))	{ nw_SendAction(kFace2,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }//
			else if (tb_PointInRegion(pt, directionRgn[4]))	{ nw_SendAction(kFace8,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }  //
			else if (tb_PointInRegion(pt, directionRgn[5]))	{ nw_SendAction(kFace3,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }//
			else if (tb_PointInRegion(pt, directionRgn[6]))	{ nw_SendAction(kFace6,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }//
			else if (tb_PointInRegion(pt, directionRgn[7]))	{ nw_SendAction(kFace1,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
		}

		return;
	}

	//--------------------------------------------------------------------------

	if (targetMode!=kNoTarget)
		tb_SetCursor(138);
	else if (tb_PointInRect(pt, &playScreenRect))
	{
		if ((tb_IsKeyDown(OptionKey)) && (playerDead==false))
			tb_SetCursor(139);
		else if (pathFind)
			tb_SetCursor(140);
	}
	else
		tb_SetCursor(0);

	if (pathFind)
		if (TickCount()>nextPathTime)
			if (tb_IsButtonDown())
				if (tb_LocalPointInContent(tbWindow,pt))
					if (targetMode==kNoTarget)
						if (!tb_IsKeyDown(OptionKey))
						{
							if (tb_PointInRect(pt, &playScreenRect))
							{
								minDist=1000;
								foundIndex=-1;

								for (i=0;i<kMaxScreenElements;i++)
									if (tb_PointInRect(pt, &destScreenRect[i]))
									{
										centerX=destScreenRect[i].left+((destScreenRect[i].right-destScreenRect[i].left)/2);
										centerY=destScreenRect[i].top+((destScreenRect[i].bottom-destScreenRect[i].top)/2);
										tempDist=fabs(centerX-pt.h)+fabs(centerY-pt.v);

										if (tempDist<minDist)
										{
											minDist=tempDist;
											foundIndex=i;
										}
									}

								if (foundIndex!=-1)
								{
									nw_SendPathMessage(mapToScreenRow[foundIndex]+playerRow, mapToScreenCol[foundIndex]+playerCol);
									nextPathTime=TickCount()+60;// version 0.1.2
									return;
								}
							}
						}

	//-------------------------------------
	if (targetMode==kNoTarget)
		if (pathFind==false)
			if (!tb_IsKeyDown(OptionKey))
			{
				if (tb_PointInRegion(pt, directionRgn[0]))			{ tb_SetCursor(130);}
				else if (tb_PointInRegion(pt, directionRgn[1]))	{ tb_SetCursor(137);}
				else if (tb_PointInRegion(pt, directionRgn[2]))	{ tb_SetCursor(133);}
				else if (tb_PointInRegion(pt, directionRgn[3]))	{ tb_SetCursor(135);}
				else if (tb_PointInRegion(pt, directionRgn[4]))	{ tb_SetCursor(131);}
				else if (tb_PointInRegion(pt, directionRgn[5]))	{ tb_SetCursor(134);}
				else if (tb_PointInRegion(pt, directionRgn[6]))	{ tb_SetCursor(132);}
				else if (tb_PointInRegion(pt, directionRgn[7]))	{ tb_SetCursor(136);}

				if (tb_IsButtonDown())
				{
					if (tb_PointInRegion(pt, directionRgn[0]))			{ nw_SendAction(5,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[1]))	{ nw_SendAction(4,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[2]))	{ nw_SendAction(6,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[3]))	{ nw_SendAction(2,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[4]))	{ nw_SendAction(8,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[5]))	{ nw_SendAction(3,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[6]))	{ nw_SendAction(7,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
					else if (tb_PointInRegion(pt, directionRgn[7]))	{ nw_SendAction(1,kNormalPriority,kCanWait,true);  tb_FlushMouseEvents(); }
				}
			}

}

// ----------------------------------------------------------------- //
void cm_HouseKeeping(void) // mod 8/12
// ----------------------------------------------------------------- //

{
	if (TickCount()<nextHouseKeeping)
		return;

	fi_SaveAllContainers();

	nextHouseKeeping=TickCount()+5*60*60;

}

// ----------------------------------------------------------------- //
void NullEvent(void)
// ----------------------------------------------------------------- //

{

	GetKeyEvents();

#ifndef _SERVERONLY
	//----------------------------------	not online

	if (onLine==false)
	{
		sp_SplashNullEvent();
		return;
	}
	else
		nw_IsGameOkay();

	//----------------------------------
#endif

#ifdef _SERVER
	if (gNetGame==NULL)
		nw_RestartServer();

	//if (TickCount()>nextHouseKeeping) // mod 8/12
	cm_HouseKeeping();

	in_CheckContainerDecay();

	rs_RefreshResources();

	sh_RotateShopInventory();

	cr_CheckCreaturePopulation();

#endif

#ifndef _SERVERONLY
	if (nextStep!=0)
		if (TickCount()>nextStep)
		{
			canStep=true;
			xOffset=0;
			yOffset=0;
			nextStep=0;
			nextRender=0;
			if (flip==0) flip=1; else flip=0;
		}

	if (TickCount()>nextRender)	// test comment out 11/3
	{
		gxoffset=xOffset;
		gyoffset=yOffset;
		GetKeyEvents();
		gr_RenderPlayArea(xOffset, yOffset,true);
		GetKeyEvents();
	}

	if (TickCount()>cursorBlinkTime)	// test comment out 11/3
		gr_RenderTextLine();

#endif

#ifdef _SERVER
	nw_SendRefreshToAllPlayers();

	cr_MoveCreatures();

	fx_ServiceFX();

	rs_GetCurrentCalendar();

#endif

}

#ifdef _SERVER
// ----------------------------------------------------------------- //
TBoolean isGrass(int row, int col)
// ----------------------------------------------------------------- //

{

	if ((map[row][col]>=256) && (map[row][col]<=263))
		return (true);
	else
		return (false);

}

#endif

// ----------------------------------------------------------------- //
void BeginEvent(void)
// ----------------------------------------------------------------- //

{
	int						i;
	TPoint 				RgnArray[tb_MaxRegionsPoints];
	//long					size;
	int						row;
	int						col;
	OSErr					e;
	short					refNum;
	//short					vRefNum;
	//long					dirID;
	long					s;
	//short					tempRefNum;
	//FSSpec				tempFSSpec;
	FSSpec				worldFSSpec;
	//int           count;
	//int						r;
	//int						c;
	//int						o;
	//TBoolean			good;
	//int						h;
	//int						w;
	//int						r1;
	//int						c1;

	fx_Initialize();

#ifndef _SERVERONLY

	RgnArray[0].h = 146+0-18;				RgnArray[0].v = 69+0-69;
	RgnArray[1].h = 280+0-18;				RgnArray[1].v = 270+0-69;
	RgnArray[2].h = 401+0-18;				RgnArray[2].v = 69+0-69;
	RgnArray[3].h = -1;							RgnArray[3].v = -1;
	directionRgn[0] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 401+0-18;				RgnArray[0].v = 69+0-69;
	RgnArray[1].h = 280+0-18;				RgnArray[1].v = 270+0-69;
	RgnArray[2].h = 529+0-18;				RgnArray[2].v = 196+0-69;
	RgnArray[3].h = 529+0-18;				RgnArray[3].v = 69+0-69;
	RgnArray[4].h = -1;							RgnArray[4].v = -1;
	directionRgn[1] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 529+0-18;				RgnArray[0].v = 196+0-69;
	RgnArray[1].h = 280+0-18;				RgnArray[1].v = 270+0-69;
	RgnArray[2].h = 529+0-18;				RgnArray[2].v = 453+0-69;
	RgnArray[3].h = -1;							RgnArray[3].v = -1;
	directionRgn[2] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 529+0-18;				RgnArray[0].v = 453+0-69;
	RgnArray[1].h = 529+0-18;				RgnArray[1].v = 580+0-69;
	RgnArray[2].h = 401+0-18;				RgnArray[2].v = 580+0-69;
	RgnArray[3].h = 280+0-18;				RgnArray[3].v = 270+0-69;
	RgnArray[4].h = -1;							RgnArray[4].v = -1;
	directionRgn[3] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 401+0-18;				RgnArray[0].v = 580+0-69;
	RgnArray[1].h = 146+0-18;				RgnArray[1].v = 580+0-69;
	RgnArray[2].h = 280+0-18;				RgnArray[2].v = 270+0-69;
	RgnArray[3].h = -1;							RgnArray[3].v = -1;
	directionRgn[4] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 280+0-18;				RgnArray[0].v = 270+0-69;
	RgnArray[1].h = 146+0-18;				RgnArray[1].v = 580+0-69;
	RgnArray[2].h = 18+0-18;				RgnArray[2].v = 580+0-69;
	RgnArray[3].h = 18+0-18;				RgnArray[3].v = 453+0-69;
	RgnArray[4].h = -1;							RgnArray[4].v = -1;
	directionRgn[5] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 280+0-18;				RgnArray[0].v = 270+0-69;
	RgnArray[1].h = 18+0-18;				RgnArray[1].v = 453+0-69;
	RgnArray[2].h = 18+0-18;				RgnArray[2].v = 196+0-69;
	RgnArray[3].h = -1;							RgnArray[3].v = -1;
	directionRgn[6] = tb_CreateRegion(RgnArray);

	RgnArray[0].h = 18+0-18;				RgnArray[0].v = 196+0-69;
	RgnArray[1].h = 18+0-18;				RgnArray[1].v = 69+0-69;
	RgnArray[2].h = 146+0-18;				RgnArray[2].v = 69+0-69;
	RgnArray[3].h = 280+0-18;				RgnArray[3].v = 270+0-69;
	RgnArray[4].h = -1;							RgnArray[4].v = -1;
	directionRgn[7] = tb_CreateRegion(RgnArray);
#endif

#ifdef _SERVER
	nextHouseKeeping=TickCount()+5*60*60;//+216000;
	FSMakeFSSpec(AppFSSpec.vRefNum, AppFSSpec.parID, "\pOberin World", &worldFSSpec);
	e=FSpOpenDF (&worldFSSpec, fsCurPerm, &refNum);
	e=SetFPos(refNum, fsFromStart,0);
	s=(kMapHeight*2)*500;

	for (row=0;row<kMapHeight;row=row+500)
	{
		e	=	FSRead(refNum, &s, &map[row][0]);
	}

	e	=	FSClose(refNum);

//  count=0;

	/*
	   for (row=1;row<(kMapHeight-1);row++)
	  	for (col=1;col<(kMapWidth-1);col++)
	  		{
	  		    if (map[row][col]==1048)
	  			    {
	  			      tb_Idle();;
	  			    }
	  		}
	*/

	/*
	   for (row=1;row<(kMapHeight-1);row++)
	  	for (col=1;col<(kMapWidth-1);col++)
	  		{
	  		    if ((map[row][col]>=1040) && (map[row][col]<=1047))
	  			    {
	  			      map[row][col]=tb_Rnd(257,263);
	  			    }
	  		}
	*/

	/*
		for (count=0;count<100;count++)
			for (row=448;row<577;row++)
				for (col=69;col<209;col++)
					{

						r=tb_Rnd(451,574);
						c=tb_Rnd(72,206);

						w=tb_Rnd(4,13);
						h=tb_Rnd(4,13);

						good=true;
						for (r1=r-1;r1<r+w+1;r1++)
							for (c1=c-1;c1<c+h+1;c1++)
								if (map[r1][c1]!=608)
									good=false;

						if (good)
							{
								for (r1=r;r1<r+w;r1++)
									for (c1=c;c1<c+h;c1++)
										map[r1][c1]=1011;

								for (r1=r;r1<r+w;r1++)
									map[r1][c+h]=907;

								for (c1=c;c1<c+h;c1++)
									map[r+w][c1]=905;

								map[r+w][c+h]=906;
							}

					}
	
*/
 	//Randomize underground rocks   Dave 11-25-2012 commented out
	for (row=0;row<kMapWidth;row++)
		for (col=0;col<kMapHeight;col++)
		{
			if (map[row][col]>535 && map[row][col]<545)
				map[row][col]=tb_Rnd(536,544);
			else if (map[row][col]>=992 && map[row][col]<=994)
				map[row][col]=tb_Rnd(992,994);
		}


	/*
		count=0;
		while (count<150)
			{
				r=tb_Rnd(784,1043);
				c=tb_Rnd(2944,3255);
				if ((map[r][c]>256) && (map[r][c]<264))
					{
						map[r][c]=tb_Rnd(1040,1047);
						count++;
					}
			}
	*/

	/*
		count=0;
		while (count<200)
			{
				r=tb_Rnd(451,574);
				c=tb_Rnd(72,206);
				if (map[r][c]==608)
					{
						map[r][c]=tb_Rnd(895,899);
						count++;
					}
			}
	*/

	/*
	   for (row=1;row<(kMapHeight-1);row++)
	  	for (col=1;col<(kMapWidth-1);col++)
	  		{
		      if (gr_SnowAt(row,col)==true)
	  		    if ((map[row][col]>=257) && (map[row][col]<=263))
	  			    {
	  			      map[row][col]=256;
	  			    }
	  		}
	*/

	/*
	  for (row=1;row<(kMapHeight-1);row++)
	    for (col=1;col<(kMapWidth-1);col++)
	    	{

	    		if (map[row][col]==263)
	    		  map[row][col]=tb_Rnd(256,262);
	    		//if (((map[row][col]>=359) && (map[row][col]<=373)) && (tb_Rnd(1,3)==1))
	    		//  {
	    		//    map[row][col]=tb_Rnd(256,263);
	    		//  }

	    	}
	*/

	/*
	  for (row=1;row<(kMapHeight-1);row++)
	  	for (col=1;col<(kMapWidth-1);col++)
	  		{

	  			if (((map[row][col]>=359) && (map[row][col]<=368)) && (map[row][col]!=364))
	  			  {
	  			    map[row][col]=tb_Rnd(359,373);
	  			    if (map[row][col]==364)
	  			      map[row][col]=tb_Rnd(359,373);
	  			    if (map[row][col]==364)
	  			      map[row][col]=tb_Rnd(359,373);
	  			  }

	  		}
	*/


//   for (row=340;row<=440;row++)
//	    for (col=60;col<=197;col++)
//          map[row][col]=990;

//  for (row=331;row<=339;row++)
// 	for (col=59;col<=197;col++)
//     map[row][col]=535;
	/*
	  count=0;
	   for (row=1;row<(kMapHeight-1);row++)
	  	for (col=1;col<(kMapWidth-1);col++)
	  		{
		      if (gr_SnowAt(row,col)==false)
	  		    if ((map[row][col]>=359) && (map[row][col]<=368))
	  			    {
	  			      count++;
	  			    }
	  		}

	tb_Idle();
	*/

	/*
	  count=0;
	  while (count<100000)
	    {
	      row=tb_Rnd(1,kMapHeight);
	      col=tb_Rnd(1,kMapWidth);
	  		if ((map[row][col]>=359) && (map[row][col]<=368))
		      if (!((row>2075) && (row<2310) && (col>1749) && (col<2001)))
		        if (gr_SnowAt(row,col)==false)
	  	        {
	    			    map[row][col]=tb_Rnd(256,262);
	    			    count++;
	  	        }
	    }
	*/

//  count=0;
//  for (row=1;row<(kMapHeight-1);row++)
//  	for (col=1;col<(kMapWidth-1);col++)
//  		{

//  			if (map[row][col]==263)
//  			  {
//	        if (!((row>2075) && (row<2310) && (col>1749) && (col<2001)))
//  			    count++;
//  			    map[row][col]=tb_Rnd(256,262);
//  			  }

//  		}

#endif

#ifndef _SERVERONLY


	FSMakeFSSpec(AppFSSpec.vRefNum, AppFSSpec.parID, "\pOberin World", &worldFSSpec);

	e=FSpOpenDF (&worldFSSpec, fsCurPerm, &refNum);

	e=SetFPos(refNum, fsFromStart,0);

	s=(kMapHeight*2)*500;

	for (row=0;row<kMapHeight;row=row+500)
	{
		e	=	FSRead(refNum, &s, &clientMap[row][0]);
	}

	e	=	FSClose(refNum);



// gs map change //	fi_OpenMapFile();

// gs map change //	for (row=0;row<400;row++)
// gs map change //		for (col=0;col<400;col++)
// gs map change //			clientMap[row][col]=0;

// gs map change //	rowOffset=65000;
// gs map change //	colOffset=65000;
/**/
	for (row=0;row<kMapWidth;row++)
		for (col=0;col<kMapHeight;col++)
		{
			if (clientMap[row][col]>535 && clientMap[row][col]<545)
				clientMap[row][col]=tb_Rnd(536,544);
			else if (clientMap[row][col]>=992 && clientMap[row][col]<=994)
				clientMap[row][col]=tb_Rnd(992,994);
		}

#endif

/**/
// rock randomization Dave 11-25-2012 commented out
#ifdef _SERVER

	for (row=0;row<kMapWidth;row++)
		for (col=0;col<kMapHeight;col++)
		{
			if (map[row][col]>535 && map[row][col]<545)
				map[row][col]=tb_Rnd(536,544);
			else if (map[row][col]>=992 && map[row][col]<=994)
				map[row][col]=tb_Rnd(992,994);
		}

#endif

	/*
		i=0;
		while (i<20000)
			{
				row=tb_Rnd(1,kMapHeight);
				col=tb_Rnd(1,kMapWidth);
				if (!gr_SnowAt(row, col))
					if ((map[row][col]>=359) && (map[row][col]<=368))
						{
							map[row][col]=tb_Rnd(256,263);
							i++;
						}
			}
	*/
	/*
		i=0;
		while (i<100000)
			{
				row=tb_Rnd(1,kMapHeight);
				col=tb_Rnd(1,kMapWidth);
				if (!gr_SnowAt(row, col))
					if ((map[row][col]>=256) && (map[row][col]<=263))
						{
							map[row][col]=tb_Rnd(918,927);
							i++;
						}
			}
	*/

//	i=0;
//	while (i<20000)
//		{
//			row=tb_Rnd(1,kMapHeight);
//			col=tb_Rnd(1,kMapWidth);
//			if (!gr_SnowAt(row, col))
//				if ((map[row][col]>=256) && (map[row][col]<=263))
//					{
//						map[row][col]=tb_Rnd(915,917);
//						i++;
//					}
//		}

	// diversify roads
//	for (row=1;row<(kMapHeight-1);row++)
//		for (col=1;col<(kMapWidth-1);col++)
//			if (map[row][col]==401)
//			{
//				i=tb_Rnd(908,914);
//				if (i==908)	i=401;
//				map[row][col]=i;
//			}

//	tb_OpenNamedInputFile("world");
//	for (row=0;row<kMapHeight;row++)
//		for (col=0;col<kMapWidth;col++)
//			{
//				tb_ReadFromFile(&map[row][col], 2);
//				if (tb_IsKeyDown(SpaceBar))
//					tb_Idle();
//			}
//	tb_CloseFile();

	//tb_OpenNamedInputFile("world");
	//size=sizeof(map);
	//tb_ReadFromFile(&map[0], size);
	//tb_CloseFile();
	/*
		// smooth
		for (row=1;row<(kMapHeight-1);row++)
			for (col=1;col<(kMapWidth-1);col++)
				{
					map[row][col]=map2[row][col];

					if ((map2[row][col]==401) && (map2[row][col+1]!=401) && (map2[row][col-1]==401) && isGrass(row-1,col))
						map[row][col]=407;//tb_Rnd(407,410);

					if ((map2[row][col]==401) && (map2[row][col-1]!=401) && (map2[row][col+1]==401) && isGrass(row+1,col))
						map[row][col]=411;//tb_Rnd(407,410);

					if ((map2[row][col]==401) && isGrass(row+1,col) && isGrass(row,col+1))
						map[row][col]=415;//tb_Rnd(407,410);

					if ((map2[row][col]==401) && isGrass(row-1,col) && isGrass(row,col-1))
						map[row][col]=423;//tb_Rnd(407,410);

				}

		// smooth
		for (row=1;row<(kMapHeight-1);row++)
			for (col=1;col<(kMapWidth-1);col++)
				{

					if ((map[row][col]==401) && isGrass(row,col+1))
						map[row][col]=403;//tb_Rnd(403,406);

					if ((map[row][col]==401) && isGrass(row,col-1))
						map[row][col]=431;//tb_Rnd(403,406);

					if ((map[row][col]==401) && isGrass(row-1,col))
						map[row][col]=427;//tb_Rnd(403,406);

					if ((map[row][col]==401) && isGrass(row+1,col))
						map[row][col]=435;//tb_Rnd(403,406);

				}
	*/
//	for (x=0;x<kMapWidth;x++)
//		for (y=0;y<kMapWidth;y++)
//			map[x][y]=tb_Rnd(256,262);

//	for (i=0;i<50000;i++)
//		map[tb_Rnd(0,(kMapWidth-1))][tb_Rnd(0,(kMapHeight-1))]=tb_Rnd(264,266);

//	for (i=0;i<1000000;i++)
//		map[tb_Rnd(0,(kMapWidth-1))][tb_Rnd(0,(kMapHeight-1))]=tb_Rnd(359,368);

	frame=0;

	strcpy(playerText,"");

	insertionPoint=0;

	playerRow=65000;

	playerCol=65000;
	
	localSpeed=0;
	
	strcpy(textLog, "");
	
	logTime=0;

	previousPlayerRow=0;

	previousPlayerCol=0;

	nextPathTime=0;

	currentInfoDisplay=kCharacterDisplay;

	targetMode=kNoTarget;

	magicLoreTarget=-1;

	for (i=0;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&inventoryDropRect[i],	0, 0, 0, 0);
		tb_SetRect(&pickUpRect[i],				0, 0, 0, 0);
		tb_SetRect(&inventoryGiveRect[i],	0, 0, 0, 0);
		tb_SetRect(&inventoryUseRect[i],	0, 0, 0, 0);
	}

	tb_SetRect(&characterTabRect,	548+horizontalOffset, 19+verticalOffset, 636+horizontalOffset, 42+verticalOffset);

	tb_SetRect(&inventoryTabRect,	636+horizontalOffset, 19+verticalOffset, 726+horizontalOffset, 42+verticalOffset);
	tb_SetRect(&skillsTabRect,		726+horizontalOffset, 19+verticalOffset, 783+horizontalOffset, 42+verticalOffset);

	for (i=0;i<kMaxScreenElements;i++)
	{
		tb_SetRect(&clickBaseRect[i],0,0,0,0);
		tb_SetRect(&clickObjectRect[i],0,0,0,0);
		tb_SetRect(&clickCreatureRect[i],0,0,0,0);
		tb_SetRect(&clickTerrainRect[i],0,0,0,0);
		//clickCreatureRgn[i]=NewRgn();
		clickTerrainID[i]=0;
	}

//#ifndef _SERVERONLY
	gr_InitializeRectangles();

	sk_InitializeSkills();

//#endif

#ifdef _SERVER
	np_InitNPCs();

	qu_InitializeQuests();

	in_InitializeContainers();

	it_InitializeItems();

	cr_InitCreatures();

	pl_InitPlayers();

	//it_InitializeItems();
	rs_InitResources();

	//sh_InitializeShops();
	ac_InitializeActions();

	fi_GetBankSettings();

	fi_GetTempleSettings();

	fi_GetPortalSettings();

	fi_GetStableSettings();

	fi_GetShopSettings();

	fi_GetNPCSettings();

	fi_GetLadderSettings();

	fi_GetGMSettings();

	gh_InitGuildHalls();

#endif

#ifndef _SERVERONLY
	gr_CreateBitmaps();

	//fi_LoadMacros();

#endif

	nw_InitNetworking();

#ifdef _SERVER
	nw_DoHost();

	onLine=true;

	tx_AddText(kWelcomeMsg);

#endif

	tb_FlushMouseEvents();

}

// ----------------------------------------------------------------- //
void cm_ReloadSettings(void)
// ----------------------------------------------------------------- //

{

	fi_GetBankSettings();
	fi_LoadBanFile();
	fi_GetTempleSettings();
	fi_GetPortalSettings();
	fi_GetStableSettings();
	fi_GetShopSettings();
	np_ClearAllNPCs();
	cr_ReloadCreatures();
	fi_GetNPCSettings();
	fi_GetLadderSettings();
	fi_GetGMSettings();
	fi_GetWeaponSettings();
	fi_GetItemSettings();
	sh_AdjustShopPrices();

}

// ----------------------------------------------------------------- //
void EndEvent(void)
// ----------------------------------------------------------------- //

{

#ifndef _SERVERONLY
	cm_WritePrefs();
#endif

#ifdef _SERVER
#ifndef _SERVERONLY
	fi_Logout(1);
#endif
#endif
	nw_ShutdownNetworking();
	

}

// ----------------------------------------------------------------- //
void UpdateEvent(int theWindow)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY

	TRect			b;
	TRect			r;
	TRect			s;

	if (theWindow==tbMapWindow)
	{
		gr_RenderMapWindow(true);
		return;
	}

	if (theWindow==tbInventoryWindow)
	{
		gr_RenderInventoryWindow();
		return;
	}

	if (theWindow==tbCalendarWindow)
	{
		gr_RenderCalendarWindow();
		return;
	}

	if (theWindow==tbPackContentsWindow)
	{
		gr_RenderPackContentsWindow();
		return;
	}

	if (theWindow==tbQuestWindow)
	{
		gr_RenderQuestWindow();
		return;
	}

	if (theWindow==tbShopContentsWindow)
	{
		gr_RenderShopContentsWindow();
		return;
	}

	if (theWindow==tbSellListWindow)
	{
		gr_RenderSellListWindow();
		return;
	}

	if (theWindow==tbIDWindow)
	{
		gr_RenderIDDisplayWindow();
		return;
	}

	if (theWindow==tbMakeWindow)
	{
		gr_RenderMakeListWindow(currentSkillDisplay,currentCategory);
		return;
	}

	if (theWindow==tbSkillsWindow)
	{
		gr_RenderSkillsWindow();
		return;
	}

	if (theWindow==tbStatsWindow)
	{
		gr_RenderStatsWindow();
		return;
	}

	if (theWindow==tbChatWindow)
	{
		gr_RenderChatDisplayWindow(0);
		return;
	}

	if (theWindow==tbCharacterWindow)
	{
		gr_RenderCharacterDisplayWindow();
		return;
	}

	if (theWindow==tbChessWindow)
	{
		gr_RenderChessWindow();
		return;
	}

	if (theWindow==tbLogOnWindow)
	{
		sp_DrawSplash();
		return;
	}

	if (theWindow==tbBankWindow)
	{
		gr_RenderBankWindow();
		return;
	}

	if (theWindow==tbPartyWindow)
	{
		gr_RenderPartyWindow();
		return;
	}

	if (theWindow==tbPetsWindow)
	{
		gr_RenderPetsWindow();
		return;
	}

	if (theWindow==tbAboutWindow)
	{
		gr_RenderAboutWindow();
		return;
	}

#endif

#ifdef _SERVERONLY
	gr_DrawServerMain();

	return;

#endif

#ifndef _SERVERONLY

	if (onLine==false)
	{
		sp_DrawSplash();
		return;
	}

	if (theWindow==tbWindow)
	{
		gr_RenderPlayArea(xOffset, yOffset,false);
		gr_RenderTextLine();
		return;
	}

	return;

	tb_DrawPictureToBitmap(osBMP, 130, 0, 0, &r);
	gr_RenderPlayerDisplay(false);
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
	//gr_DrawButtons(false);
	gr_RenderMapView(false);
	//gr_RenderPlayArea2(false);
	gxoffset=xOffset;
	gyoffset=yOffset;
	//fl_UpdateFloater(kMainFloater);
	//gr_RenderPlayArea3();
	gr_RenderPlayArea(xOffset, yOffset,false);

	tb_SetRect(&s,0,0,800,600);
	r=s;
	tb_OffsetRect(&r,horizontalOffset,verticalOffset);
	tb_CopyBitmap(osBMP,tbWindow,&s,&r,false);

	tb_SetForeColor(tbWindow, 0, 0, 0);

	tb_SetRect(&b,0,0,screenBounds.right,r.top-1);
	tb_PaintRect(tbWindow, &b);

	tb_SetRect(&b,0,r.bottom+1,screenBounds.right,screenBounds.bottom);
	tb_PaintRect(tbWindow, &b);

	tb_SetRect(&b,0,0,r.left-1,screenBounds.bottom);
	tb_PaintRect(tbWindow, &b);

	tb_SetRect(&b,r.right+1,0,screenBounds.right,screenBounds.bottom);
	tb_PaintRect(tbWindow, &b);

#endif

}

// ----------------------------------------------------------------- //
void MyMenuEvent(int /*theMenu*/, int /*theMenuItem*/)
// ----------------------------------------------------------------- //

{

	// Not used

}

// ----------------------------------------------------------------- //
void KeyDownEvent (char key, TBoolean repeatKey, UInt16 keyCode)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY

	//char					dummy[kStringLength]; //[5];
	TBoolean      doFunctionKey=false;
	char					temp[kStringLength]; //[5];
	int           i;

	//---------------------------------------------

	if (onLine==false)
	{
		sp_SplashKeyDownEvent(key);
		return;
	}

	//---------------------------------------------

	if (currentDialog!=0)
		return;

	if (key=='\34')
	{
		insertionPoint--;

		if (insertionPoint<0) insertionPoint=0;

		return;
	}

	if (key=='\35')
	{
		insertionPoint++;

		if (insertionPoint>strlen(playerText)) insertionPoint=strlen(playerText);

		return;
	}

//	#ifndef _DEBUGGING
	if (keyPadControl)
	{
		if ((keyCode>82) && (keyCode<90))
			return;

		if ((keyCode>90) && (keyCode<93))
			return;
	}

//	#endif

	if (keyCode==Function1Key)
		if (!repeatKey)
		{
			strcpy(playerText,f1Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function2Key)
		if (!repeatKey)
		{
			strcpy(playerText,f2Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function3Key)
		if (!repeatKey)
		{
			strcpy(playerText,f3Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function4Key)
		if (!repeatKey)
		{
			strcpy(playerText,f4Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function5Key)
		if (!repeatKey)
		{
			strcpy(playerText,f5Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function6Key)
		if (!repeatKey)
		{
			strcpy(playerText,f6Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function7Key)
		if (!repeatKey)
		{
			strcpy(playerText,f7Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function8Key)
		if (!repeatKey)
		{
			strcpy(playerText,f8Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function9Key)
		if (!repeatKey)
		{
			strcpy(playerText,f9Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function10Key)
		if (!repeatKey)
		{
			strcpy(playerText,f10Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function11Key)
		if (!repeatKey)
		{
			strcpy(playerText,f11Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function12Key)
		if (!repeatKey)
		{
			strcpy(playerText,f12Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function13Key)
		if (!repeatKey)
		{
			strcpy(playerText,f13Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function14Key)
		if (!repeatKey)
		{
			strcpy(playerText,f14Str);
			doFunctionKey=true;
		}
		else
			return;

	if (keyCode==Function15Key)
		if (!repeatKey)
		{
			strcpy(playerText,f15Str);
			doFunctionKey=true;
		}
		else
			return;

	if (doFunctionKey)
	{
		if ((playerText[0]=='+') || (playerText[0]=='.') || (playerText[0]=='/') || (playerAction!=0))
		{
			ParseInputString();
			return;
		}

		//if (!playerDead)  // 0.1.4
		nw_SendTextToHost(playerText);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	//---------------------------------------------

	if ((int)keyCode==71)
	{
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((int)key==27)
	{
		targetMode=kNoTarget;
		ClearPlayerAction();
		return;
	}

	if ((int)key==8)
	{
		if (strlen(playerText)>0)
		{
			if (insertionPoint==strlen(playerText))
			{
				playerText[strlen(playerText)-1]='\0';
				insertionPoint--;

				if (insertionPoint<0) insertionPoint=0;
			}
			else
			{
				insertionPoint--;

				if (insertionPoint<0) insertionPoint=0;

				//if (insertionPoint>0)
				for (i=insertionPoint;i<strlen(playerText);i++)
					playerText[i]=playerText[i+1];

				//insertionPoint--;
				//if (insertionPoint<0) insertionPoint=0;
			}

			// playerText[strlen(playerText)-1]='\0';
			//insertionPoint--;
			//if (insertionPoint<0) insertionPoint=0;
		}

		gr_RenderTextLine();

		return;
	}

	if (((int)key==13) || ((int)key==3))
	{
#ifdef _SERVER

		if ((playerText[0]=='+') || (playerText[0]=='.') || (playerText[0]=='/') || (playerAction!=0))
		{
			ParseInputString();
			return;
		}

#endif
#ifndef _SERVER
		if ((playerText[0]=='/') || (playerAction!=0))
		{
			ParseInputString();
			return;
		}

#endif
		//if (!playerDead)  0.1.3
		if (strcmp(playerText,"")!=0)
			nw_SendTextToHost(playerText);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

//	strcpy(dummy,"");
//	dummy[0]=key;
//	dummy[1]='\0';
//	if (strlen(playerText)<(kCommandStringMaxLength-1)) // 0.1.4
//	  {
//		  strcat(playerText,dummy);
//		  gr_RenderTextLine();
//		}
//	else
//		return;


	if (strlen(playerText)>=(kStringLength-1))
	{
		tb_Beep(0);
		return;
	}

	for (i=0;i<insertionPoint;i++)
		temp[i]=playerText[i];

	temp[insertionPoint]=key;

	temp[insertionPoint+1]='\0';

	for (i=insertionPoint;i<strlen(playerText);i++)
	{
		temp[i+1]=playerText[i];
		temp[i+2]='\0';
	}

	insertionPoint++;

	strcpy(playerText,temp);
	gr_RenderTextLine();

	//strcpy(playerText,"");



	//if ((key=='i') || (key=='I'))
	//	nw_SendAction(kRefreshInventory, kHighPriority, kSendNow);

	//if ((key=='g') || (key=='G'))
	//	nw_SendAction(kPickUp, kHighPriority, kSendNow);
#endif

}

// ----------------------------------------------------------------- //
void checkHitCreature (TPoint pt, int theSkill, char failMessage[kStringLength]) //[100])
// ----------------------------------------------------------------- //

{
	int								i;
	int								slot;
	int               hits;

	if (!tb_PointInRect(pt, &playScreenRect))
		return;

	hits=0;

	for (i=(kMaxScreenElements-1);i>=0;i--)
		if (creatureID[i]!=0)
			if (clickCreatureRect[i].right!=0)	// 11/3
				if (tb_PointInRect(pt, &clickCreatureRect[i]))
					hits++;

	if (hits>0)
		for (i=(kMaxScreenElements-1);i>=0;i--)
			if (creatureID[i]!=0)
				if (clickCreatureRect[i].right!=0) // 11/3
					if (tb_PointInRect(pt, &clickCreatureRect[i]))
						if ((gr_ClickedOnCreatureRegion(pt, i)) || (hits==1))
						{
							if (theSkill==kGiveTarget)
							{
								if (i!=90)
								{
									if (targetSlot==0)
										nw_SendGiveGold(i, targetCount);
									else
									{
										//nw_SendItemTransferMessage(i, targetSlot, targetCount);
										nw_SendItemTransferMessage(creatureID[i], targetSlot, targetCount);
									}
								}

								playerAction=0;

								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
								strcpy(playerText,"");
								return;
							}

							//------- healing ... dec bandage ----------
							else if (theSkill==kHealing)
							{
								slot=in_PlayerHas(kBandage);

								if (slot>0)
								{
									//nw_SendTargetMessage(i,theSkill);
									nw_SendTargetMessage(creatureID[i],theSkill);
									nw_SendDeleteFromInventorySlot(slot);

									if (localInventory[slot].itemCount>0)
										localInventory[slot].itemCount--;

									if (localInventory[slot].itemCount==0)
										localInventory[slot].itemID=0;

									gr_RenderPlayerDisplay(true);
								}
								else
								{
									targetMode=kNoTarget;
									targetSlot=0;
									targetCount=0;
								}
							}

							//------- res spell  ----------
							else if (theSkill==kResurrectTarget)
							{
								if (i!=90)
									nw_SendTargetMessage(creatureID[i],kResurrectTarget);

								//nw_SendTargetMessage(i,kResurrectTarget);
								targetMode=kNoTarget;

								targetSlot=0;

								targetCount=0;
							}

							//------- lightning bolt spell  ----------
							else if (theSkill==kLightningBoltTarget)
							{
								if (i!=90)
									nw_SendTargetMessage(creatureID[i],kLightningBoltTarget);

								//nw_SendTargetMessage(i,kLightningBoltTarget);
								targetMode=kNoTarget;

								targetSlot=0;

								targetCount=0;
							}

							//------- lesser l spell  ----------
							else if (theSkill==kLesserHealTarget)
							{
								//nw_SendTargetMessage(i,kLesserHealTarget);
								nw_SendTargetMessage(creatureID[i],kLesserHealTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- greater heal spell  ----------
							else if (theSkill==kGreaterHealTarget)
							{
								//nw_SendTargetMessage(i,kGreaterHealTarget);
								nw_SendTargetMessage(creatureID[i],kGreaterHealTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- remove curse spell  ----------
							else if (theSkill==kRemoveCurseTarget)
							{
								nw_SendTargetMessage(creatureID[i],kRemoveCurseTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- full heal spell  ----------
							else if (theSkill==kFullHealTarget)
							{
								//nw_SendTargetMessage(i,kFullHealTarget);
								nw_SendTargetMessage(creatureID[i],kFullHealTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- poison spell  ----------
							else if (theSkill==kPoisonTarget)
							{
								//nw_SendTargetMessage(i,kPoisonTarget);
								nw_SendTargetMessage(creatureID[i],kPoisonTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- reflective armor spell  ----------
							else if (theSkill==kReflectiveArmorTarget)
							{
								//nw_SendTargetMessage(i,kReflectiveArmorTarget);
								nw_SendTargetMessage(creatureID[i],kReflectiveArmorTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- drain mana spell  ----------
							else if (theSkill==kDrainManaTarget)
							{
								//nw_SendTargetMessage(i,kDrainManaTarget);
								nw_SendTargetMessage(creatureID[i],kDrainManaTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- disorientation spell  ----------
							else if (theSkill==kDisorientationTarget)
							{
								//nw_SendTargetMessage(i,kDisorientationTarget);
								nw_SendTargetMessage(creatureID[i],kDisorientationTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- magic reflection spell  ----------
							else if (theSkill==kMagicReflectionTarget)
							{
								//nw_SendTargetMessage(i,kMagicReflectionTarget);
								nw_SendTargetMessage(creatureID[i],kMagicReflectionTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- paralyze spell  ----------
							else if (theSkill==kParalyzeTarget)
							{
								//nw_SendTargetMessage(i,kParalyzeTarget);
								nw_SendTargetMessage(creatureID[i],kParalyzeTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- clumsy spell  ----------
							else if (theSkill==kClumsySpellTarget)
							{
								//nw_SendTargetMessage(i,kClumsySpellTarget);
								nw_SendTargetMessage(creatureID[i],kClumsySpellTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- agility spell  ----------
							else if (theSkill==kAgilitySpellTarget)
							{
								//nw_SendTargetMessage(i,kAgilitySpellTarget);
								nw_SendTargetMessage(creatureID[i],kAgilitySpellTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- weaken spell  ----------
							else if (theSkill==kWeakenSpellTarget)
							{
								//nw_SendTargetMessage(i,kWeakenSpellTarget);
								nw_SendTargetMessage(creatureID[i],kWeakenSpellTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- strength spell  ----------
							else if (theSkill==kStrengthSpellTarget)
							{
								//nw_SendTargetMessage(i,kStrengthSpellTarget);
								nw_SendTargetMessage(creatureID[i],kStrengthSpellTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- simple mind spell  ----------
							else if (theSkill==kSimpleMindSpellTarget)
							{
								//nw_SendTargetMessage(i,kSimpleMindSpellTarget);
								nw_SendTargetMessage(creatureID[i],kSimpleMindSpellTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- elevate mind spell  ----------
							else if (theSkill==kElevateMindTarget)
							{
								//nw_SendTargetMessage(i,kElevateMindTarget);
								nw_SendTargetMessage(creatureID[i],kElevateMindTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireball spell  ----------
							else if (theSkill==kFireballTarget)
							{
								nw_SendTargetMessage(creatureID[i],kFireballTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireblast spell  ----------
							else if (theSkill==kFireblastTarget)
							{
								nw_SendTargetMessage(creatureID[i],kFireblastTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireblast2 spell  ----------
							else if (theSkill==kMagicMissileTarget)
							{
								nw_SendTargetMessage(creatureID[i],kMagicMissileTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireblast3 spell  ----------
							else if (theSkill==kFireblast3Target)
							{
								nw_SendTargetMessage(creatureID[i],kFireblast3Target);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireblast4 spell  ----------
							else if (theSkill==kFireblast4Target)
							{
								nw_SendTargetMessage(creatureID[i],kFireblast4Target);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- fireblast5 spell  ----------
							else if (theSkill==kFireblast5Target)
							{
								nw_SendTargetMessage(creatureID[i],kFireblast5Target);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- lesser cure spell  ----------
							else if (theSkill==kLesserDetoxifyTarget)
							{
								//nw_SendTargetMessage(i,kLesserDetoxifyTarget);
								nw_SendTargetMessage(creatureID[i],kLesserDetoxifyTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- greater cure spell  ----------
							else if (theSkill==kGreaterDetoxifyTarget)
							{
								//nw_SendTargetMessage(i,kGreaterDetoxifyTarget);
								nw_SendTargetMessage(creatureID[i],kGreaterDetoxifyTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- full cure spell  ----------
							else if (theSkill==kFullDetoxifyTarget)
							{
								//nw_SendTargetMessage(i,kFullDetoxifyTarget);
								nw_SendTargetMessage(creatureID[i],kFullDetoxifyTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- explosion spell  ----------
							else if (theSkill==kExplosionTarget)
							{
								//nw_SendTargetMessage(i,kExplosionTarget);
								nw_SendTargetMessage(creatureID[i],kExplosionTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- invisibility spell  ----------
							else if (theSkill==kInvisibilityTarget)
							{
								//nw_SendTargetMessage(i,kInvisibilityTarget);
								nw_SendTargetMessage(creatureID[i],kInvisibilityTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- opportunity spell  ----------
							else if (theSkill==kOpportunityTarget)
							{
								//nw_SendTargetMessage(i,kInvisibilityTarget);
								nw_SendTargetMessage(creatureID[i],kOpportunityTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- night vision spell  ----------
							else if (theSkill==kNightVisionTarget)
							{
								//nw_SendTargetMessage(i,kNightVisionTarget);
								nw_SendTargetMessage(creatureID[i],kNightVisionTarget);
								targetMode=kNoTarget;
								targetSlot=0;
								targetCount=0;
							}

							//------- attacking  ----------
							else if (theSkill==kAttackTarget)
							{
								//if (i!=90)
								//nw_SendTargetMessage(i,kAttackTarget);
								nw_SendTargetMessage(creatureID[i],kAttackTarget);
							}
							else	//-------------------------------------
								nw_SendTargetMessage(creatureID[i],theSkill);

							//nw_SendTargetMessage(i,theSkill);
							//-------------------------------------------

							return;
						}

	if (theSkill==kAttackTarget)
		return;

	gr_AddText(failMessage,false,true);

	tb_SetCursor(0);

	while (tb_IsButtonDown())
	{
		NullEvent();
		nw_GetMessages();
	}

	playerAction	=	0;

	targetMode    = kNoTarget;
	targetSlot    = 0;
	targetCount   = 0;

	return;

}

// ----------------------------------------------------------------- //
TBoolean DoButton (TPoint pt, TRect r, int active, int pressed, TBoolean reversed, TBoolean sticky, int theButton)
// ----------------------------------------------------------------- //

{
	TPoint						mpt;
	TRect							d;

	mpt=pt;

	if (reversed)
	{
		if (tb_PointInRect(pt, &r))
		{
			tb_SetCursor(0);
			targetMode=kNoTarget;
			ClearPlayerAction();

			tb_DrawPictureToBitmap(tbWindow, active, r.left, r.top, &d);

			while (tb_IsButtonDown())
			{
				tb_GetMousePosition(&mpt);

				if (tb_PointInRect(mpt, &r))
					tb_DrawPictureToBitmap(tbWindow, active, r.left, r.top, &d);
				else
					tb_DrawPictureToBitmap(tbWindow, pressed, r.left, r.top, &d);

				NullEvent();

				nw_GetMessages();
			}

			if (tb_PointInRect(mpt, &r))
			{
				if (!sticky)
				{
					tb_WaitTick(5);
					tb_DrawPictureToBitmap(tbWindow, pressed, r.left, r.top, &d);
				}
				else if (theButton>0)
				{
					displayAreaButtonState[theButton]		=kActiveButton;
					displayAreaButtonCurrent[theButton]	=kActiveButton;
				}

				tb_FlushMouseEvents();

				return (true);
			}

			tb_DrawPictureToBitmap(tbWindow, pressed, r.left, r.top, &d);
		}
	}
	else
	{
		if (tb_PointInRect(pt, &r))
		{
			tb_SetCursor(0);
			targetMode=kNoTarget;
			ClearPlayerAction();

			tb_DrawPictureToBitmap(tbWindow, pressed, r.left, r.top, &d);

			while (tb_IsButtonDown())
			{
				tb_GetMousePosition(&mpt);

				if (tb_PointInRect(mpt, &r))
					tb_DrawPictureToBitmap(tbWindow, pressed, r.left, r.top, &d);
				else
					tb_DrawPictureToBitmap(tbWindow, active, r.left, r.top, &d);

				NullEvent();

				nw_GetMessages();
			}

			if (tb_PointInRect(mpt, &r))
			{
				if (!sticky)
				{
					tb_WaitTick(5);
					tb_DrawPictureToBitmap(tbWindow, active, r.left, r.top, &d);
				}
				else if (theButton>0)
				{
					displayAreaButtonState[theButton]		=kPressedButton;
					displayAreaButtonCurrent[theButton]	=kPressedButton;
				}

				tb_FlushMouseEvents();

				return (true);
			}

			tb_DrawPictureToBitmap(tbWindow, active, r.left, r.top, &d);
		}
	}

	tb_FlushMouseEvents();

	return (false);

}

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void chooseMakeCategory (int category)
// ----------------------------------------------------------------- //

{

	if (currentSkillDisplay	== kTailoringLeatherworking)
	{
		//currentInfoDisplay=kMakeDisplay;
		gr_OpenFloatingWindow(tbMakeWindow);
		//ShowWindow(makeWindow);
		//SelectWindow(makeWindow);
		//UpdateEvent(tbMakeWindow);
		windowVisible[kMakeWindow]=true;

		if (category==1)	gr_RenderMakeListWindow(currentSkillDisplay, kShirtType);

		if (category==2)	gr_RenderMakeListWindow(currentSkillDisplay, kPantsType);

		if (category==3)	gr_RenderMakeListWindow(currentSkillDisplay, kRobeType);

		if (category==4)	gr_RenderMakeListWindow(currentSkillDisplay, kLeatherArmorType);

		if (category==5)	gr_RenderMakeListWindow(currentSkillDisplay, kWizardHatType);
		//Dave
		if (category==6)	gr_RenderMakeListWindow(currentSkillDisplay, kJesterHatType);
		
		if (category==7)	gr_RenderMakeListWindow(currentSkillDisplay, kMagicRobeType);
		
		if (category==8)	gr_RenderMakeListWindow(currentSkillDisplay, kMiscTailorType);

		//if (category==5)	gr_RenderMakeList(true, currentSkillDisplay, kBootsType);
		//if (category==6)	gr_RenderMakeList(true, currentSkillDisplay, kWaistType);
	}
	else if (currentSkillDisplay == kEnchanting)
	{
		gr_OpenFloatingWindow(tbMakeWindow);
		windowVisible[kMakeWindow]=true;

		if (category==1)	gr_RenderMakeListWindow(currentSkillDisplay, kEtheriteType);

		if (category==2)	gr_RenderMakeListWindow(currentSkillDisplay, kInfusedCrystalType);

		if (category==3)	gr_RenderMakeListWindow(currentSkillDisplay, kLesserRingType);

		if (category==4)	gr_RenderMakeListWindow(currentSkillDisplay, kCharmType);

		if (category==5)	gr_RenderMakeListWindow(currentSkillDisplay, kMiscType);
		//Dave
		if (category==6)	gr_RenderMakeListWindow(currentSkillDisplay, kOrbType);
	}
	else if (currentSkillDisplay == kBlacksmithing)
	{
		gr_OpenFloatingWindow(tbMakeWindow);
		windowVisible[kMakeWindow]=true;

		if (category==1)	gr_RenderMakeListWindow(currentSkillDisplay, kArmorType);

		if (category==2)	gr_RenderMakeListWindow(currentSkillDisplay, kAnyWeaponType);

		if (category==3)	gr_RenderMakeListWindow(currentSkillDisplay, kMagicWeaponType);

		if (category==4)	gr_RenderMakeListWindow(currentSkillDisplay, kMiscType);
		//Dave
		if (category==5)	gr_RenderMakeListWindow(currentSkillDisplay, kToolType);
		//if (category==5)	gr_RenderMakeListWindow(currentSkillDisplay, kLumberjackToolType, kMiningToolType, kSewingToolType, kTinkerToolType, kWoodToolType, kBlacksmithToolType);
		/*if (category==5)	gr_RenderMakeListWindow(currentSkillDisplay, kLumberjackToolType);
			gr_RenderMakeListWindow(currentSkillDisplay, kMiningToolType);	
			gr_RenderMakeListWindow(currentSkillDisplay, kSewingToolType);	
			gr_RenderMakeListWindow(currentSkillDisplay, kTinkerToolType);	
			gr_RenderMakeListWindow(currentSkillDisplay, kWoodToolType);	
			gr_RenderMakeListWindow(currentSkillDisplay, kBlacksmithToolType);	
	*/
	}
	

}

#endif

// ----------------------------------------------------------------- //
void cm_MoveInsertionPoint (int x)
// ----------------------------------------------------------------- //

{
	int             i;
	char            theText1[kStringLength];
	char            theText2[kStringLength];

	tb_SetFont(textLineBMP, kFontIDGeneva);
	tb_SetFontSize(textLineBMP, 10);
	tb_SetFontStyle(textLineBMP, tb_Bold);

	if (strcmp(playerText,"")==0)
	{
		insertionPoint=0;
		return;
	}

	x=x+2;

	theText1[0]=playerText[0];
	theText1[1]='\0';

	if (x<tb_StringWidth(textLineBMP,theText1))
	{
		insertionPoint=0;
		return;
	}

	if (x>tb_StringWidth(textLineBMP,playerText))
	{
		insertionPoint=strlen(playerText);
		return;
	}

	strcpy(theText1,"");

	strcpy(theText2,"");

	for (i=0;i<strlen(playerText);i++)
	{
		theText1[i]=playerText[i];
		theText1[i+1]='\0';

		theText2[i]=playerText[i];
		theText2[i+1]=playerText[i+1];
		theText2[i+2]='\0';

		if ((tb_StringWidth(textLineBMP,theText1)<x) && (tb_StringWidth(textLineBMP,theText2)>x))
		{
			insertionPoint=i+1;
			return;
		}

	}

}

// ----------------------------------------------------------------- //
void MouseDownEvent (int theWindow, TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY

	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	//int								canAdd;
	//UInt32						canAddGold;
	//UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	int								minDist;
	int								tempDist;
	int								foundIndex;
	int								centerX;
	int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;

	//---------------------------------------------------- not online

	if (onLine==false)
	{
		sp_SplashMouseDownEvent(theWindow, pt);
		return;
	}


//	if (currentInfoDisplay==kChatDisplay)
//	  {
//      if (gr_CheckMouseOnUpButton(pt))              tb_Idle();
//      else if (gr_CheckMouseOnDownButton(pt))       tb_Idle();
//      else if (gr_DragThumb(pt))                    tb_Idle();
//      else if (gr_CheckMouseOnPageUpButton(pt))     tb_Idle();
//      else if (gr_CheckMouseOnPageDownButton(pt))   tb_Idle();
//	  }
	//----------------------------------------------------

	if (tb_PointInRect(pt, &textLineScreenRect))
	{
		//tb_Beep(0);
		cm_MoveInsertionPoint(pt.h);
		return;
	}

	if ((tb_PointInRect(pt, &playScreenRect)) && (pathFind==false))
		CheckKeys(true,0);

	//--------------------------------

	//--------------------------------------------------------------------------



	//--------------------------------------------------------------------------

	//if (currentInfoDisplay==kSkillsDisplay)
	//	for (i=0;i<kMaxSkills;i++)	// check for skill direction buttons
	//		if (tb_PointInRect(pt, &skillDirectionRect[i]))
	//			{
	//				nw_SendSkillDirectionChange(i);
	//				localSkills[i].direction++;
	//				if (localSkills[i].direction==3) localSkills[i].direction=0;
	//				tb_DrawPictureToBitmap(tbWindow, 137+localSkills[i].direction, horizontalOffset+767, verticalOffset+70+(i*13)-10, &skillDirectionRect[i]);
	//				return;
	//			}


	//--------------------------------------------------------------------------

	if (targetMode==kNoTarget)
		if ((pathFind) && (tb_PointInRect(pt, &playScreenRect)))
			if (!tb_IsKeyDown(OptionKey))
			{
				minDist=1000;
				foundIndex=-1;

				for (i=0;i<kMaxScreenElements;i++)
					if (tb_PointInRect(pt, &destScreenRect[i]))
					{
						centerX=destScreenRect[i].left+((destScreenRect[i].right-destScreenRect[i].left)/2);
						centerY=destScreenRect[i].top+((destScreenRect[i].bottom-destScreenRect[i].top)/2);
						tempDist=fabs(centerX-pt.h)+fabs(centerY-pt.v);

						if (tempDist<minDist)
						{
							minDist=tempDist;
							foundIndex=i;
						}
					}

				if (foundIndex!=-1)
				{
					nw_SendPathMessage(mapToScreenRow[foundIndex]+playerRow, mapToScreenCol[foundIndex]+playerCol);
					nextPathTime=TickCount()+60;// version 0.1.2
					return;
				}
			}

	//--------------------------------------------------------------------------
	if (targetMode==kNoTarget)
		if (tb_IsKeyDown(OptionKey))
		{
			checkHitCreature(pt, kAttackTarget, "");
			return;
		}

//	if (!tb_IsKeyDown(OptionKey))
	{

		//if (targetMode==kCreatureTarget)	// target creature	-------------------------------------------------- // beta .. take out
		//	{
		//		if (!tb_PointInRect(pt, &playScreenRect))
		//			return;
		//		for (i=251;i>=0;i--)
		//			if (tb_PointInRect(pt, &clickCreatureRect[i]))
		//				{
		//					nw_SendTargetMessage(i,0);
		//					return;
		//				}
		//	}
		//
		//else

		if (targetMode==kJailTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kJailTarget, "");
			return;
		}
		else if (targetMode==kMuteTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kMuteTarget, "");
			return;
		}
		else if (targetMode==kNPCITarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kNPCITarget, "");
			return;
		}
		else if (targetMode==kSkillsTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kSkillsTarget, "");
			return;
		}
		else if (targetMode==kMsayTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kMsayTarget, "");
			return;
		}
		else if (targetMode==kMakeEvilTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kMakeEvilTarget, "");
			return;
		}
		else if (targetMode==kMakeGoodTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kMakeGoodTarget, "");
			return;
		}
		else if (targetMode==kKillTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kKillTarget, "");
			return;
		}
		else if (targetMode==kUnmuteTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kUnmuteTarget, "");
			return;
		}
		else if (targetMode==kPitTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kPitTarget, "");
			return;
		}
		else if (targetMode==kKickTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kKickTarget, "");
			return;
		}
		else if (targetMode==kBanTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kBanTarget, "");
			return;
		}
		else if (targetMode==kBanishTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kBanishTarget, "");
			return;
		}
		else if (targetMode==kAKATarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kAKATarget, "");
			return;
		}
		else if (targetMode==kIPTarget)	// ip	--------------------------------------------------
		{
			checkHitCreature(pt, kIPTarget, "");
			return;
		}
		else if (targetMode==kNoGainTarget)	// id	--------------------------------------------------  // mod 8/4
		{
			checkHitCreature(pt, kNoGainTarget, "");
			return;
		}
		else if (targetMode==kGainTarget)	// id	--------------------------------------------------  // mod 8/4
		{
			checkHitCreature(pt, kGainTarget, "");
			return;
		}
		else if (targetMode==kResTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kResTarget, "");
			return;
		}
		else if (targetMode==kDressupTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kDressupTarget, "");
			return;
		}
		else if (targetMode==kResetTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kResetTarget, "");
			return;
		}
		else if (targetMode==kHomeTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kHomeTarget, "");
			return;
		}
		else if (targetMode==kActivateTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kActivateTarget, "");
			return;
		}
		else if (targetMode==kDumpInvTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kDumpInvTarget, "");
			return;
		}
		else if (targetMode==kDumpBankTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kDumpBankTarget, "");
			return;
		}
		else if (targetMode==kClassTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kClassTarget, "");
			return;
		}
		else if (targetMode==kTameTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kTameTarget, "You must target a creature.");// to tame.");
			return;
		}
		else if (targetMode==kIDTarget)	// id	--------------------------------------------------
		{
			checkHitCreature(pt, kIDTarget, "You must target a creature.");// to identify.");
			return;
		}
		else if (targetMode==kLesserHealTarget)	// lesser heal spell	--------------------------------------------------
		{
			checkHitCreature(pt, kLesserHealTarget, "You must target a creature.");// when casting a lesser heal spell.");
			return;
		}
		else if (targetMode==kNightVisionTarget)	// night vision spell	--------------------------------------------------
		{
			checkHitCreature(pt, kNightVisionTarget, "You must target a creature.");// when casting a night vision spell.");
			return;
		}
		else if (targetMode==kInvisibilityTarget)	// invisibility spell	--------------------------------------------------
		{
			checkHitCreature(pt, kInvisibilityTarget, "You must target a creature.");// when casting an invisibility spell.");
			return;
		}
		else if (targetMode==kOpportunityTarget)	// opportunity spell	--------------------------------------------------
		{
			checkHitCreature(pt, kOpportunityTarget, "You must target a creature.");// when casting an invisibility spell.");
			return;
		}
		else if (targetMode==kExplosionTarget)	// greater cure spell	--------------------------------------------------
		{
			checkHitCreature(pt, kExplosionTarget, "You must target a creature.");// when casting an explosion spell.");
			return;
		}
		else if (targetMode==kGreaterDetoxifyTarget)	// greater cure spell	--------------------------------------------------
		{
			checkHitCreature(pt, kGreaterDetoxifyTarget, "You must target a creature.");// when casting a greater detoxify spell.");
			return;
		}
		else if (targetMode==kFullDetoxifyTarget)	// greater cure spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFullDetoxifyTarget, "You must target a creature.");// when casting a full detoxify spell.");
			return;
		}
		else if (targetMode==kClumsySpellTarget)	// clumsy spell	--------------------------------------------------
		{
			checkHitCreature(pt, kClumsySpellTarget, "You must target a creature.");// when casting a clumsy spell.");
			return;
		}
		else if (targetMode==kAgilitySpellTarget)	// agility spell	--------------------------------------------------
		{
			checkHitCreature(pt, kAgilitySpellTarget, "You must target a creature.");// when casting an agility spell.");
			return;
		}
		else if (targetMode==kWeakenSpellTarget)	// weaken spell	--------------------------------------------------
		{
			checkHitCreature(pt, kWeakenSpellTarget, "You must target a creature.");// when casting a weaken spell.");
			return;
		}
		else if (targetMode==kStrengthSpellTarget)	// strength spell	--------------------------------------------------
		{
			checkHitCreature(pt, kStrengthSpellTarget, "You must target a creature.");// when casting a strength spell.");
			return;
		}
		else if (targetMode==kSimpleMindSpellTarget)	// simple mind spell	--------------------------------------------------
		{
			checkHitCreature(pt, kSimpleMindSpellTarget, "You must target a creature.");// when casting a simple mind spell.");
			return;
		}
		else if (targetMode==kElevateMindTarget)	// elevate mind spell	--------------------------------------------------
		{
			checkHitCreature(pt, kElevateMindTarget, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kFireballTarget)	// fireball spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFireballTarget, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kFireblastTarget)	// fireblast spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFireblastTarget, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kMagicMissileTarget)	// fireblast spell	--------------------------------------------------
		{
			checkHitCreature(pt, kMagicMissileTarget, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kFireblast3Target)	// fireblast spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFireblast3Target, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kFireblast4Target)	// fireblast spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFireblast4Target, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kFireblast5Target)	// fireblast spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFireblast5Target, "You must target a creature.");// when casting an elevate mind spell.");
			return;
		}
		else if (targetMode==kLesserDetoxifyTarget)	// lesser cure spell	--------------------------------------------------
		{
			checkHitCreature(pt, kLesserDetoxifyTarget, "You must target a creature.");// when casting a lesser detoxify spell.");
			return;
		}
		else if (targetMode==kDrainManaTarget)	// paralyze spell	--------------------------------------------------
		{
			checkHitCreature(pt, kDrainManaTarget, "You must target a creature.");// when casting a drain mana spell.");
			return;
		}
		else if (targetMode==kDisorientationTarget)	// disorientation spell	--------------------------------------------------
		{
			checkHitCreature(pt, kDisorientationTarget, "You must target a creature.");// when casting a disorientation spell.");
			return;
		}
		else if (targetMode==kMagicReflectionTarget)	// magic reflection spell	--------------------------------------------------
		{
			checkHitCreature(pt, kMagicReflectionTarget, "You must target a creature.");// when casting a magic reflection spell.");
			return;
		}
		else if (targetMode==kParalyzeTarget)	// paralyze spell	--------------------------------------------------
		{
			checkHitCreature(pt, kParalyzeTarget, "You must target a creature.");// when casting a paralyze spell.");
			return;
		}
		else if (targetMode==kPoisonTarget)	// poison spell	--------------------------------------------------
		{
			checkHitCreature(pt, kPoisonTarget, "You must target a creature.");// when casting a poison spell.");
			return;
		}
		else if (targetMode==kReflectiveArmorTarget)	// reflective armor spell	--------------------------------------------------
		{
			checkHitCreature(pt, kReflectiveArmorTarget, "You must target a creature.");// when casting a reflective armor spell.");
			return;
		}
		else if (targetMode==kGreaterHealTarget)	// greater heal spell	--------------------------------------------------
		{
			checkHitCreature(pt, kGreaterHealTarget, "You must target a creature.");// when casting a greater heal spell.");
			return;
		}
		else if (targetMode==kRemoveCurseTarget)	// remove curse spell	--------------------------------------------------
		{
			checkHitCreature(pt, kRemoveCurseTarget, "You must target a creature.");// when casting a remove curse spell.");
			return;
		}
		else if (targetMode==kFullHealTarget)	// full heal spell	--------------------------------------------------
		{
			checkHitCreature(pt, kFullHealTarget, "You must target a creature.");// when casting a full heal spell.");
			return;
		}
		else if (targetMode==kResurrectTarget)	// res spell	--------------------------------------------------
		{
			checkHitCreature(pt, kResurrectTarget, "You must target a ghost.");// when casting a resurrect spell.");
			return;
		}
		else if (targetMode==kLightningBoltTarget)	// lightning bolt spell	--------------------------------------------------
		{
			checkHitCreature(pt, kLightningBoltTarget, "You must target a creature.");// when casting a lightning bolt spell.");
			return;
		}
		else if (targetMode==kGiveTarget)	// giving an item to a creature	--------------------------------------------------
		{
			checkHitCreature(pt, kGiveTarget, "You must target a creature.");// when giving an item.");
			return;
		}
		else if (targetMode==kHealing)	// healing target creature	--------------------------------------------------
		{
			checkHitCreature(pt, kHealing, "You must target a creature.");// when using the healing skill.");
			return;
		}

		/*else if (targetMode==kStealth)	// thievery target creature	--------------------------------------------------
			{
				//checkHitCreature(pt, kStealth, "You must target a creature when using the thievery skill.");
				return;
			}	*/
		else if (targetMode==kAnatomy)	// anatomy target creature	--------------------------------------------------
		{
			checkHitCreature(pt, kAnatomy, "You must target a creature.");// when using the anatomy skill.");
			return;
		}
		
	//	else if (targetMode==kGMCurse)	// id	--------------------------------------------------
		//{
		//	checkHitCreature(pt, kGMCurse, "You curse someone?");
		//	return;
		//}
		//else if (targetMode==kAnimalLore)	// anatomy target creature	--------------------------------------------------
		//	{
		//		checkHitCreature(pt, kAnimalLore, "You must target a creature when using the animal lore skill.");
		//		return;
		//	}
		//else if (targetMode==kEvaluatingIntelligence)	// anatomy target creature	--------------------------------------------------
		//	{
		//		checkHitCreature(pt, kEvaluatingIntelligence, "You must target a creature when using the evaluating intelligence skill.");
		//		return;
		//	}

		else if (targetMode==kLumberjacking)	// target tree	------------------------------------------------------
		{
			if (!tb_PointInRect(pt, &playScreenRect))
				return;

			for (i=251;i>=0;i--)
				if (tb_PointInRect(pt, &clickTerrainRect[i]))
				{
					if ((clickTerrainID[i]<359) || (clickTerrainID[i]>378)) // not a tree -------
					{
						tx_AddText(kMustTargetTreeMsg);
						tb_SetCursor(0);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}
					else if (gr_DistanceFromPlayer(pt)>80)	// too far from tree ... pixel distance from tree ------
					{
						tx_AddText(kTooFarFromTreeMsg);
						tb_SetCursor(0);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}
					else	// ok ... chop tree ------
					{
						tx_AddText(kStartChoppingTreeMsg);
						nw_SendTargetResourceMessage(mapToScreenRow[i]+playerRow, mapToScreenCol[i]+playerCol,kLumberjacking);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}

					return;
				}

			tx_AddText(kMustTargetTreeMsg); // clicked in play area, but not on object

			tb_SetCursor(0);

			while (tb_IsButtonDown())
			{
				NullEvent();
				nw_GetMessages();
			}

			targetMode=kNoTarget;
		}

		else if (targetMode==kMining)	// target rock	------------------------------------------------------
		{
			if (!tb_PointInRect(pt, &playScreenRect))
				return;

			for (i=251;i>=0;i--)
				if (tb_PointInRect(pt, &clickTerrainRect[i]))
				{
					if (clickTerrainID[i]<386 || (clickTerrainID[i]<536 && clickTerrainID[i]>394) || (clickTerrainID[i]>544)) // not a rock -------
					{
						tx_AddText(kMustTargetRockMsg);
						tb_SetCursor(0);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}
					else if (gr_DistanceFromPlayer(pt)>80)	// too far from rock ... pixel distance from rock ------
					{
						tx_AddText(kTooFarFromRockMsg);
						tb_SetCursor(0);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}
					else	// ok ... mine for ore ------
					{
						tx_AddText(kStartMiningOreMsg);
						nw_SendTargetResourceMessage(mapToScreenRow[i]+playerRow, mapToScreenCol[i]+playerCol,kMining);

						while (tb_IsButtonDown())
						{
							NullEvent();
							nw_GetMessages();
						}

						targetMode=kNoTarget;
					}

					return;
				}

			tx_AddText(kMustTargetRockMsg);

			tb_SetCursor(0);

			while (tb_IsButtonDown())
			{
				NullEvent();
				nw_GetMessages();
			}

			targetMode=kNoTarget;
		}
		else if ((targetMode==kFireFieldTarget) || (targetMode==kTeleportTarget))// Teleport & Fire Field Spell	------------------------------------------------------
		{
			if (!tb_PointInRect(pt, &playScreenRect))
				return;

			//for (i=251;i>=0;i--)
			minDist=1000;

			foundIndex=-1;

			for (i=0;i<kMaxScreenElements;i++)
				if (tb_PointInRect(pt, &destScreenRect[i]))
				{
					centerX=destScreenRect[i].left+((destScreenRect[i].right-destScreenRect[i].left)/2);
					centerY=destScreenRect[i].top+((destScreenRect[i].bottom-destScreenRect[i].top)/2);
					tempDist=fabs(centerX-pt.h)+fabs(centerY-pt.v);

					if (tempDist<minDist)
					{
						minDist=tempDist;
						foundIndex=i;
					}
				}

			if (foundIndex!=-1)
			{
				nw_SendTargetResourceMessage(mapToScreenRow[foundIndex]+playerRow, mapToScreenCol[foundIndex]+playerCol,targetMode);

				while (tb_IsButtonDown())
				{
					NullEvent();
					nw_GetMessages();
				}

				//targetMode=kAttackTarget;
				return;
			}
		}
		else	//------------------------------------------------------------------------------------------------
		{

		}
	}

#endif
}

// ----------------------------------------------------------------- //
TBoolean DoFloaterButton (TBitmap bmp, TPoint pt, TRect r, int active, int pressed, TBoolean reversed, TBoolean sticky, int theButton, TBoolean use)
// ----------------------------------------------------------------- //

{
	TPoint						mpt;
	TRect						d;
	TBoolean					drawTick=true;

	mpt=pt;

	if (bmp==tbInventoryWindow)
		if (!in_ItemIsUsable(localInventory[currentDragLine].itemID))
			if (use)
				drawTick=false;
			
	if (bmp==tbBankWindow)
		if (!in_ItemIsUsable(bankInventory[currentDragLine].itemID))
			if (use)
				drawTick=false;
			
			

	if (reversed)
	{
		if (tb_PointInRect(pt, &r))
		{
			if (drawTick)
			{
				tb_SetCursor(0);
				targetMode=kNoTarget;
				ClearPlayerAction();
				tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);
			}

			while (tb_IsButtonDown())
			{
				tb_SetPort(bmp);
				tb_GetMousePosition(&mpt);

				if (drawTick)
				{
					if (tb_PointInRect(mpt, &r))
						tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);
					else
						tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);
				}


				if (bmp==tbInventoryWindow)
					if (WaitMouseMoved(mpt) && tb_IsKeyDown(ShiftKey))
					{
						if (drawTick)
							tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);

						if (currentDragLine>0)
							DoStartDrag(r, bmp);

						return (false);
					}
				//Bank Sort
				if (bmp==tbBankWindow)
					if (WaitMouseMoved(mpt) && tb_IsKeyDown(ShiftKey))
					{
						if (drawTick)
							tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);

						if (currentDragLine>0)
							DoStartDrag(r, bmp);

						return (false);
					}

				if (drawTick)
				{
					NullEvent();
					nw_GetMessages();
				}
			}

			if (tb_PointInRect(mpt, &r) && drawTick)
			{
				if (!sticky)
				{
					tb_WaitTick(10);
					tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);
				}
				else if (theButton>0)
				{
					displayAreaButtonState[theButton]=kActiveButton;
					displayAreaButtonCurrent[theButton]=kActiveButton;
				}

				tb_FlushMouseEvents();

				return (true);
			}

			if (drawTick)
				tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);
		}
	}
	else
	{
		if (tb_PointInRect(pt, &r))
		{
			if (drawTick)
			{
				tb_SetCursor(0);
				targetMode=kNoTarget;
				ClearPlayerAction();
				tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);
			}

			while (tb_IsButtonDown())
			{
				tb_SetPort(bmp);
				tb_GetMousePosition(&mpt);

				if (drawTick)
				{
					if (tb_PointInRect(mpt, &r))
						tb_DrawPictureToBitmap(bmp, pressed, r.left, r.top, &d);
					else
						tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);
				}

				if (bmp==tbInventoryWindow)
					if (WaitMouseMoved(mpt) && tb_IsKeyDown(ShiftKey))
					{
						if (drawTick)
							tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);

						if (currentDragLine>0)
							DoStartDrag(r, bmp);

						return (false);
					}
				//Bank Sort
				if (bmp==tbBankWindow)
					if (WaitMouseMoved(mpt) && tb_IsKeyDown(ShiftKey))
					{
						if (drawTick)
							tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);

						if (currentDragLine>0)
							DoStartDrag(r, bmp);

						return (false);
					}

				if (drawTick)
				{
					NullEvent();
					nw_GetMessages();
				}
			}

			if (tb_PointInRect(mpt, &r) && drawTick)
			{
				if (!sticky)
				{
					tb_WaitTick(10);
					tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);
				}
				else if (theButton>0)
				{
					displayAreaButtonState[theButton]=kPressedButton;
					displayAreaButtonCurrent[theButton]=kPressedButton;
				}

				tb_FlushMouseEvents();

				return (true);
			}

			if (drawTick)
				tb_DrawPictureToBitmap(bmp, active, r.left, r.top, &d);
		}
	}

	if (drawTick)
		tb_FlushMouseEvents();

	return (false);

}

// ----------------------------------------------------------------- //
void DoStartDrag (TRect r, TBitmap bmp)
// ----------------------------------------------------------------- //
{
	OSErr			theErr;
	DragReference	dragRef;
	RgnHandle		theRgn;
	//TRect			bmpRect;
	Rect			originalHiliteRect, zeroedHiliteRect;
	Rect			*PicRec;
	Point			offsetPoint;
	CGrafPtr		savedPortPtr;
	GDHandle		saveDeviceHdl;
	GWorldPtr		dragGWorldPtr = NULL;
	PixMapHandle	dragPixMapHdl;//, windPixMapHdl;
	RgnHandle		dragRgnHdl, tempRgnHdl;
	RgnHandle		maskRgnHdl;
	char			theName[kStringLength];
	//int				theSize;
	Str255			ps;
	RGBColor		DragClr;

	if (theErr=NewDrag(&dragRef))
		return;

	//fi_WriteToErrorLog("init drag..");


	//fi_WriteToErrorLog("Getting name...");

	if (bmp==tbInventoryWindow)
		in_GetItemName(localInventory[currentDragLine].itemID, theName, localInventory[currentDragLine].itemCount);
	else if (bmp==tbBankWindow)
		in_GetItemName(bankInventory[currentDragLine].itemID, theName, bankInventory[currentDragLine].itemCount);
	else
		strcpy(theName, "");

	tb_CtoPStr255(theName, ps);

	/*strcat(theName,";");

	theSize=0;
	while(theName[theSize]!=';')
		theSize++;

	AddDragItemFlavor(dragRef,1,'TEXT',theName, theSize, flavorSenderOnly);*/

	//fi_WriteToErrorLog("Setting Rgn..");

	theRgn=NewRgn();

	//tb_GetWindowRect(bmp, &bmpRect);

	SetRectRgn(theRgn, r.left, r.top, r.right, r.bottom);


	maskRgnHdl =  dragRgnHdl = tempRgnHdl = NULL;

	GetRegionBounds(theRgn,&originalHiliteRect);

	zeroedHiliteRect = originalHiliteRect;

	OffsetRect(&zeroedHiliteRect,-originalHiliteRect.left,-originalHiliteRect.top);

	GetGWorld(&savedPortPtr,&saveDeviceHdl);

	//fi_WriteToErrorLog("drag image..");

	theErr = NewGWorld(&dragGWorldPtr,0,&zeroedHiliteRect,NULL,NULL,0);

	if (dragGWorldPtr != NULL && theErr == noErr)
	{
		//fi_WriteToErrorLog("setGworld..");
		SetGWorld(dragGWorldPtr,NULL);
		//fi_WriteToErrorLog("set.");
		DragClr.red=58339;
		DragClr.green=58339;
		DragClr.blue=58339;
		RGBBackColor(&DragClr);
		EraseRect(&zeroedHiliteRect);
		//fi_WriteToErrorLog("Getting PixMap..");
		dragPixMapHdl = GetGWorldPixMap(dragGWorldPtr);
		//fi_WriteToErrorLog("got. Drawing Tick...");

		if (bmp==tbInventoryWindow)
		{
			MoveTo(0,10);

			PHandle=NULL;

			if (in_ItemIsUsable(localInventory[currentDragLine].itemID))
			{
				if (in_ItemEquiped(currentDragLine))
					PHandle=GetPicture(gr_ActiveButtonOf(currentDragLine)+1);
				else
					PHandle=GetPicture(gr_ActiveButtonOf(currentDragLine));
			}

			if (PHandle!=NULL)
			{
				SetRect(PicRec,0,0,((**(PHandle)).picFrame.right - (**(PHandle)).picFrame.left),((**(PHandle)).picFrame.bottom - (**(PHandle)).picFrame.top));
				DrawPicture(PHandle, PicRec);
				//fi_WriteToErrorLog("drawn.");
				ReleaseResource((Handle)PHandle);
			}
		}

		//Sort Bank
		if (bmp==tbBankWindow)
		{
			MoveTo(0,10);

			PHandle=NULL;

			if (PHandle!=NULL)
			{
				SetRect(PicRec,0,0,((**(PHandle)).picFrame.right - (**(PHandle)).picFrame.left),((**(PHandle)).picFrame.bottom - (**(PHandle)).picFrame.top));
				DrawPicture(PHandle, PicRec);
				//fi_WriteToErrorLog("drawn.");
				ReleaseResource((Handle)PHandle);
			}
		}
		//fi_WriteToErrorLog("Name..");
		MoveTo(14,10);

		TextFont(kFontIDGeneva);

		TextFace(0);
		
		TextSize(10);

		DrawString(ps);

		//fi_WriteToErrorLog("All done. Setting old GWorld back...");
		SetGWorld(savedPortPtr,saveDeviceHdl);

		//fi_WriteToErrorLog("set.");

		maskRgnHdl = NewRgn();

		if (maskRgnHdl != NULL)
		{
			CopyRgn(theRgn,maskRgnHdl);
			OffsetRgn(maskRgnHdl,-originalHiliteRect.left,-originalHiliteRect.top);

			SetPt(&offsetPoint,originalHiliteRect.left,originalHiliteRect.top);
			LocalToGlobal(&offsetPoint);
//fi_WriteToErrorLog("Setting drag image...");
			SetDragImage(dragRef,dragPixMapHdl,maskRgnHdl,offsetPoint,kDragStandardTranslucency);
//fi_WriteToErrorLog("set.");
		}
	}

	//SetRectRgn(theRgn, r.left, r.bottom, r.left+150, r.bottom+1);
	//SetPt(&offsetPoint, 0, 0);
	//LocalToGlobal(&offsetPoint);
	//OffsetRgn(theRgn, offsetPoint.h, offsetPoint.v);

	//fi_WriteToErrorLog("Tracking drag..");

	theErr=TrackDrag(dragRef, gMouseDownRef, theRgn);

	/*if (theErr!=noErr && theErr!=userCanceledErr)
		return;*/
	//fi_WriteToErrorLog("disposing");

	if (dragRef)			DisposeDrag(dragRef);

	if (theRgn)				DisposeRgn(theRgn);

	if (dragGWorldPtr)		DisposeGWorld(dragGWorldPtr);

	if (dragRgnHdl) 			DisposeRgn(dragRgnHdl);

	if (tempRgnHdl) 			DisposeRgn(tempRgnHdl);

	targetMode=kNoTarget;

	//fi_WriteToErrorLog("disposed");


}

// ----------------------------------------------------------------- //
OSErr dragTrackingHandler(DragTrackingMessage trackingMessage,WindowRef windowRef, void */*handlerRefCon*/,DragRef /*dragRef*/)
// ----------------------------------------------------------------- //
{
	//RgnHandle	windowRgn;
	Point		mousePt;
	int			theLine;
	//char debug[kStringLength];

	/*windowRgn=NewRgn();
	if (windowRef==inventoryWindow)
		RectRgn(windowRgn, &inventoryWindowRect);
	else if (windowRef==bankWindow)
		RectRgn(windowRgn, &bankWindowRect);*/

	switch (trackingMessage)
	{
		case kDragTrackingEnterHandler:

			if (dragInitWindow==NULL)
				dragInitWindow=windowRef;

			break;

			/*case kDragTrackingEnterWindow:
				ShowDragHilite(dragRef, windowRgn, true);
			break;*/

		case kDragTrackingInWindow:
			SetPortWindowPort(windowRef);

			tb_GetMousePosition(&mousePt);

			//GlobalToLocal(&mousePt);
			theLine=mousePt.v;

			theLine-=42;

			theLine/=13;

			//tb_IntegerToString(theLine, debug);
			//gr_AddText(debug, false, true);
			currentDragDest=theLine;

			//ShowDragHilite(dragRef, windowRgn, true);
			break;

			/*case kDragTrackingLeaveWindow:
				//HideDragHilite(dragRef);
			break;*/
	}
}

// ----------------------------------------------------------------- //
OSErr  dragReceiveHandler(WindowRef windowRef,void */*handlerRefCon*/,DragRef /*dragRef*/)
// ----------------------------------------------------------------- //
{
	int 				i;
	int 				start, stop2, stop;
	int					futureEquip[kMaxInventorySize];
	int					cpt;
	inventoryType		moved;
	//char				debug[kStringLength];

	if (currentDragDest<1 || currentDragDest==currentDragLine)
		return;

	if (windowRef==inventoryWindow)
	{
		if (dragInitWindow!=inventoryWindow)
			return;

		start=currentDragLine;

		stop=currentDragDest;

		stop2=cpt=0;

		for (i=1; i<kMaxInventorySize; i++)
		{
			if (localInventory[i].itemID!=0)
				cpt++;

			if (cpt==stop && stop2==0)
				stop2=i;
		}

		stop=stop2;

		//tb_IntegerToString(start, debug);
		//gr_AddText(debug, false, true);

		//tb_IntegerToString(stop, debug);
		//gr_AddText(debug, false, true);

		if (stop<=0 || start<=0 || stop>=kMaxInventorySize || start>=kMaxInventorySize || stop==start)
			return;

		moved=localInventory[start];

		futureEquip[stop]=in_GetEquipSlot(start);

		if (start>stop)
		{
			for (i=start; i>stop; i--)
			{
				futureEquip[i]=in_GetEquipSlot(i-1);
				localInventory[i].itemID=localInventory[i-1].itemID;
				localInventory[i].itemCount=localInventory[i-1].itemCount;
			}
		}
		else
		{
			for (i=start; i<stop; i++)
			{
				futureEquip[i]=in_GetEquipSlot(i+1);
				localInventory[i].itemID=localInventory[i+1].itemID;
				localInventory[i].itemCount=localInventory[i+1].itemCount;
			}
		}

		localInventory[stop].itemID=moved.itemID;

		localInventory[stop].itemCount=moved.itemCount;

		if (start>stop)
		{
			for (i=start; i>=stop; i--)
				in_EquipLocal(futureEquip[i], i);
		}
		else
		{
			for (i=start; i<=stop; i++)
				in_EquipLocal(futureEquip[i], i);
		}


		gr_RenderInventoryWindow();
	}
	else if (windowRef==bankWindow)
	{
		if (dragInitWindow!=bankWindow)
			return;

		start=currentDragLine;

		stop=currentDragDest;

		stop2=cpt=0;

		for (i=1; i<kMaxInventorySize; i++)
		{
			if (bankInventory[i].itemID!=0)
				cpt++;

			if (cpt==stop && stop2==0)
				stop2=i;
		}

		stop=stop2;

		//tb_IntegerToString(start, debug);
		//gr_AddText(debug, false, true);

		//tb_IntegerToString(stop, debug);
		//gr_AddText(debug, false, true);

		if (stop<=0 || start<=0 || stop>=kMaxInventorySize || start>=kMaxInventorySize || stop==start)
			return;

		moved=bankInventory[start];


		if (start>stop)
		{
			for (i=start; i>stop; i--)
			{
				bankInventory[i].itemID=bankInventory[i-1].itemID;
				bankInventory[i].itemCount=bankInventory[i-1].itemCount;
			}
		}
		else
		{
			for (i=start; i<stop; i++)
			{
				bankInventory[i].itemID=bankInventory[i+1].itemID;
				bankInventory[i].itemCount=bankInventory[i+1].itemCount;
			}
		}

		bankInventory[stop].itemID=moved.itemID;

		bankInventory[stop].itemCount=moved.itemCount;



		gr_RenderBankWindow();
	}

	dragInitWindow=NULL;

	currentDragLine=0;
	currentDragDest=0;
	nw_SendSortMessageToServer(windowRef, start, stop);
}

// ----------------------------------------------------------------- //
void MouseDownEventPackContentsWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	int								canAdd;
	UInt32						canAddGold;
	UInt32						goldTotal;
	int								gold1;
	int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	//-----------------------------------------------

	if (strcmp(localContainerName,"")==0)
		line=0;
	else
		line=1;

	if (((pickUpInventory[0].itemID!=0) || (pickUpInventory[0].itemCount!=0)) && (pickUpInventory[0].itemID<60000))
	{
		tb_SetRect(&pickUpRect[0],553-547,70+(line*13)-10-18,553-547+12,70+(line*13)-10+12-18);
		pickUpRect[0].right=784-547;

		if (DoFloaterButton (tbPackContentsWindow, pt, pickUpRect[0], 132, 175, false, false, -1, false))
		{
			if (tb_IsKeyDown(OptionKey))
			{
				tx_AddText(kHowMuchGoldToTakeMsg);
				playerAction=kTaking;
				targetSlot=0;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				goldTotal=in_GetGoldTotal(pickUpInventory[0].itemID, pickUpInventory[0].itemCount);
				canAddGold=in_CanAddGoldtoInventory_c(goldTotal);

				if (canAddGold>0)
				{
					goldTotal=goldTotal-canAddGold;
					in_SetGoldTotal(goldTotal, &gold1, &gold2);
					pickUpInventory[0].itemID			=	gold1;
					pickUpInventory[0].itemCount	=	gold2;
					nw_SendTakeFromList(0, canAddGold);
					gr_RenderPlayerDisplay(true);
				}
				else
					tx_AddText(kCantCarryMoreGoldMsg);
			}
		}

		line++;
	}

	for (i=1;i<kMaxInventorySize;i++)	// check for pick up buttons
	{

		if ((pickUpInventory[i].itemID!=0) && (pickUpInventory[i].itemID<60000))
		{
			tb_SetRect(&pickUpRect[i],553-547,70+(line*13)-10-18,553-547+12,70+(line*13)-10+12-18);
			pickUpRect[i].right=784-547;
			line++;
		}
		else
			tb_SetRect(&pickUpRect[i],0,0,0,0);

		if (DoFloaterButton (tbPackContentsWindow, pt, pickUpRect[i], 132, 175, false, false, -1, false))
		{
			if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(pickUpInventory[i].itemID)) && (pickUpInventory[i].itemCount>1))
			{
				tx_AddText(kHowManyToTakeMsg);
				playerAction=kTaking;
				targetSlot=i;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				canAdd=in_CanAddtoInventory_c(pickUpInventory[i].itemID, pickUpInventory[i].itemCount);

				if (canAdd>0)
				{
					nw_SendTakeFromList(i, canAdd);

					if (in_CanGroup(pickUpInventory[i].itemID))
					{
						pickUpInventory[i].itemCount	=	pickUpInventory[i].itemCount - canAdd;

						if (pickUpInventory[i].itemCount==0)
							pickUpInventory[i].itemID		=	0;
					}
					else
					{
						pickUpInventory[i].itemID			=	0;
						pickUpInventory[i].itemCount	=	0;
					}

					gr_RenderPlayerDisplay(true); //?
				}
				else
					tx_AddText(kPackIsFullMsg);
			}

			return;
		}

	}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventShopContentsWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	int								canAdd;
	//UInt32						canAddGold;
	UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	line=1;

	for (i=0;i<kMaxInventorySize;i++)	// check for buy buttons
	{

		if (shopInventory[i].itemID!=0)
		{
			tb_SetRect(&pickUpRect[i],553-547,75+(line*13)-10-18,553+12-547,75+(line*13)-10+12-18);
			pickUpRect[i].right=784;
			line++;
		}
		else
			tb_SetRect(&pickUpRect[i],0,0,0,0);

		if (DoFloaterButton(tbShopContentsWindow, pt, pickUpRect[i], 132, 175, false, false, -1, false))
		{
			if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(shopInventory[i].itemID)))
			{
				gr_AddText("How many do you want to buy?",false,true);
				playerAction=kBuying;
				targetSlot=i;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				goldTotal=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);

				if (shopInventory[i].cost>goldTotal)
				{
					gr_AddText("You don't have enough gold.",false,true);
				}
				else
				{
					canAdd=in_CanAddtoInventory_c(shopInventory[i].itemID, 1);

					if (canAdd>0)
					{
						nw_SendBuyFromList(i, canAdd);
					}
					else
						gr_AddText("Your pack is full.",false,true);
				}
			}

			return;
		}

	}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventSellListWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	//int								canAdd;
	//UInt32						canAddGold;
	//UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	line=1;

	for (i=1;i<kMaxInventorySize;i++)	// check for sell buttons
	{

		tb_SetRect(&pickUpRect[i],0,0,0,0);

		if (localInventory[i].itemID!=0)
			if (sh_InSellList(localInventory[i].itemID))
			{
				tb_SetRect(&pickUpRect[i],553-547,75+(line*13)-10-18,553+12-547,75+(line*13)-10+12-18);
				pickUpRect[i].right=784-547;
				line++;
			}

		if (DoFloaterButton (tbSellListWindow, pt, pickUpRect[i], 135, 176, false, false, -1, false))
		{
			if ((tb_IsKeyDown(AltKey)) && (in_CanGroup(localInventory[i].itemID)))
			{
				if (nw_CanSend())
				{
					if (in_CanGroup(localInventory[i].itemID))
						nw_SendItemTransferMessage(254, i, kMaxInventoryItemCount);
					else
						nw_SendItemTransferMessage(254, i, localInventory[i].itemCount);
				}
			}
			else if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(localInventory[i].itemID)))
			{
				gr_AddText("How many do you want to sell?",false,true);
				playerAction=kSelling;
				targetSlot=i;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				if (in_CanGroup(localInventory[i].itemID))
					nw_SendItemTransferMessage(254, i, 1);
				else
					nw_SendItemTransferMessage(254, i, localInventory[i].itemCount);
			}

			return;
		}

	}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventInventoryWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	//int								canAdd;
	//UInt32						canAddGold;
	//UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	TBoolean          sticky;
	TBoolean          targetInventory=false;

	//-----------------------------------------------

	if (targetMode==kInventoryTarget)
		targetInventory=true;

	tb_SetRect(&inventoryDropRect[0],553-547,70+(0*13)-10-18,553+12-547,70+(0*13)-10+12-18);

	tb_SetRect(&inventoryGiveRect[0],567-547,70+(0*13)-10-18,567+12-547,70+(0*13)-10+12-18);

	if (DoFloaterButton (tbInventoryWindow, pt, inventoryDropRect[0], 135, 176, false, false, -1, false))
	{
		if (tb_IsKeyDown(AltKey))
		{
			nw_SendGiveGold(255,kMaxGold);
		}
		else if (tb_IsKeyDown(OptionKey))
		{
			if (standingInBank)
			{
				gr_AddText("How much gold do you want to deposit?",false,true);
				playerAction=kDepositing;
				strcpy(playerText,"");
			}
			else
			{
				gr_AddText("How much gold do you want to drop?",false,true);
				playerAction=kDropping;
				strcpy(playerText,"");
			}

			targetSlot=0;

			targetCount=0;
		}
		else if (nw_CanSend())
		{
			nw_SendGiveGold(255, 1);
		}

		return;
	}

	if (DoFloaterButton (tbInventoryWindow, pt, inventoryGiveRect[0], 146, 177, false, false, -1, false))
	{
		if (tb_IsKeyDown(AltKey))
		{
			gr_AddText("Target the recepient.",false,false);
			targetMode=kGiveTarget;
			targetSlot=0;
			targetCount=kMaxGold;
		}
		else if (tb_IsKeyDown(OptionKey))
		{
			gr_AddText("How much gold do you want to give?",false,true);
			playerAction=kGiving;
			targetSlot=0;
			targetCount=0;
			strcpy(playerText,"");
		}
		else if (nw_CanSend())
		{
			gr_AddText("Target the recepient.",false,false);
			targetMode=kGiveTarget;
			targetSlot=0;
			targetCount=1;
			strcpy(playerText,"");
		}

		return;
	}

	line=0;

	for (i=1;i<kMaxInventorySize;i++)	// check for inventory drop buttons
	{
		currentDragLine=i;

		if (localSortedInventory[i].itemID!=0)
		{
			line++;
			tb_SetRect(&inventoryDropRect[i],553-547,70+(line*13)-10-18,553+12-547,70+(line*13)-10+12-18);
			tb_SetRect(&inventoryGiveRect[i],567-547,70+(line*13)-10-18,567+12-547,70+(line*13)-10+12-18);
			//if (in_ItemIsUsable(localSortedInventory[i].itemID))
			tb_SetRect(&inventoryUseRect[i],581-547,70+(line*13)-10-18,581+12-547,70+(line*13)-10+12-18);
			//else
			//tb_SetRect(&inventoryUseRect[i],0,0,0,0);
			inventoryUseRect[i].right=784-547;
		}
		else
		{
			tb_SetRect(&inventoryDropRect[i],0,0,0,0);
			tb_SetRect(&inventoryGiveRect[i],0,0,0,0);
			tb_SetRect(&inventoryUseRect[i],0,0,0,0);
		}

		if (DoFloaterButton (tbInventoryWindow, pt, inventoryGiveRect[i], 146, 177, false, false, -1, false))
		{
			if ((tb_IsKeyDown(AltKey)) && (in_CanGroup(localSortedInventory[i].itemID)))
			{
				gr_AddText("Target the recepient.",false,false);
				targetMode=kGiveTarget;
				targetSlot=localInventoryLookup[i];

				if (in_CanGroup(localSortedInventory[i].itemID))
					targetCount=kMaxInventoryItemCount;
				else
					targetCount=localSortedInventory[i].itemCount;
			}
			else if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(localSortedInventory[i].itemID)) && (localSortedInventory[i].itemCount>1))
			{
				gr_AddText("How many do you want to give?",false,true);
				playerAction=kGiving;
				targetSlot=localInventoryLookup[i];
				strcpy(playerText,"");
			}
			else
				if (nw_CanSend())
				{
					gr_AddText("Target the recepient.",false,false);
					targetMode=kGiveTarget;
					targetSlot=localInventoryLookup[i];

					if (in_CanGroup(localSortedInventory[i].itemID))
						targetCount=1;
					else
						targetCount=localSortedInventory[i].itemCount;
				}

			return;
		}

		if (DoFloaterButton (tbInventoryWindow, pt, inventoryDropRect[i], 135, 176, false, false, -1, false) && !playerDead)
		{
			if ((tb_IsKeyDown(AltKey)) && (in_CanGroup(localSortedInventory[i].itemID)))
			{
				if (nw_CanSend())
				{
					if (in_CanGroup(localSortedInventory[i].itemID))
						nw_SendItemTransferMessage(255, localInventoryLookup[i], kMaxInventoryItemCount);
					else
						nw_SendItemTransferMessage(255, localInventoryLookup[i], localSortedInventory[i].itemCount);
				}
			}
			else if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(localSortedInventory[i].itemID)) && (localSortedInventory[i].itemCount>1))
			{
				if (standingInBank)
				{
					gr_AddText("How many do you want to deposit?",false,true);
					playerAction=kDepositing;
					strcpy(playerText,"");
				}
				else
				{
					gr_AddText("How many do you want to drop?",false,true);
					playerAction=kDropping;
					strcpy(playerText,"");
				}

				targetSlot=localInventoryLookup[i];
			}
			else if (nw_CanSend())
			{
				if (in_CanGroup(localSortedInventory[i].itemID))
					nw_SendItemTransferMessage(255, localInventoryLookup[i], 1);
				else
					nw_SendItemTransferMessage(255, localInventoryLookup[i], localSortedInventory[i].itemCount);

				playerAction=0;

				targetSlot=0;

				strcpy(playerText,"");
			}

			return;
		}

		//if (targetMode==kMagicLore) // target inventory item
		//  if (DoFloaterButton (tbInventoryWindow, pt, inventoryUseRect[i], gr_ActiveButtonOf(localInventoryLookup[i]), gr_ActiveButtonOf(localInventoryLookup[i])+1, in_ItemEquiped(localInventoryLookup[i]), false, -1))
		//    {
		//     if (itemList[localSortedInventory[i].itemID].magical==false)
		//       {
		//          gr_AddText("It appears to be an ordinary item.",false,true);
		//				if (warOn)
		//					{
		//						targetMode=kAttackTarget;
		//						targetSlot=0;
		//						targetCount=0;
		//					}
		//				else
		//					{
		//						targetMode=kNoTarget;
		//						targetSlot=0;
		//						targetCount=0;
		//					}
		//        }
		//      else
		//        {
		//          magicLoreTarget=localSortedInventory[i].itemID;
		//		      nw_SendTargetMessage(localInventoryLookup[i],kMagicLore);
		//		    }
		//		  return;
		//    }

		if (itemList[localInventory[localInventoryLookup[i]].itemID].useLocation==kCantArm)
			sticky=false;
		else
			sticky=true;

		if (targetInventory)
		{
			if (DoFloaterButton (tbInventoryWindow, pt, inventoryUseRect[i], gr_ActiveButtonOf(localInventoryLookup[i]), gr_ActiveButtonOf(localInventoryLookup[i])+1, in_ItemEquiped(localInventoryLookup[i]), false, -1, true))
			{
				it_TryToRepairItem(localInventoryLookup[i]);
				ClearPlayerAction();
			}
		}

		else if (DoFloaterButton (tbInventoryWindow, pt, inventoryUseRect[i], gr_ActiveButtonOf(localInventoryLookup[i]), gr_ActiveButtonOf(localInventoryLookup[i])+1, in_ItemEquiped(localInventoryLookup[i]), sticky, -1, true))
		{
			if (in_ItemEquiped(localInventoryLookup[i]) && !playerDead)
			{
				in_UnEquipItem(localInventoryLookup[i]);
				return;
			}
			else
			{
				useSlot=localInventoryLookup[i];
				in_UseItem(localInventoryLookup[i]);
				return;
			}
		}

	}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventSkillsWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i, cpt;
	TRect							r;
	TPoint						mpt;
	int								line=0;
	//int								canAdd;
	//UInt32						canAddGold;
	//UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	//-----------------------------------------------
	//for (i=0;i<kMaxSkills;i++)	// check for skill direction buttons
	//	if (tb_PointInRect(pt, &skillDirectionRect[i]))
	//		{
	//			nw_SendSkillDirectionChange(i);
	//			localSkills[i].direction++;
	//			if (localSkills[i].direction==3) localSkills[i].direction=0;
	//			tb_DrawPictureToBitmap(tbSkillsWindow, 137+localSkills[i].direction, 767-547, 70+(i*13)-10-18, &skillDirectionRect[i]);
	//			return;
	//		}

	cpt=0;

	for (i=0;i<kMaxSkills;i++)	// check for skill buttons
		if (localClass>6 || maxSkillLevel[localClass][i]>0 || questChar[90] || (localRightFinger!=0 && itemList[itemList[localInventory[localRightFinger].itemID].spell].makeSkill==i) || (localLeftFinger!=0 && itemList[itemList[localInventory[localLeftFinger].itemID].spell].makeSkill==i))
		{
			if (tb_PointInRect(pt, &skillRect[i]))
			{
				tb_GetLocalMousePosition(tbSkillsWindow,&mpt);
				tb_DrawPictureToBitmap(tbSkillsWindow, 141, 553-547, 70+(cpt*13)-10-18, &r);

				while (tb_IsButtonDown())
				{
					NullEvent();
					tb_GetLocalMousePosition(tbSkillsWindow,&mpt);

					if (tb_PointInRect(mpt, &skillRect[i]))
						tb_DrawPictureToBitmap(tbSkillsWindow, 141, 553-547, 70+(cpt*13)-10-18, &r);
					else
						tb_DrawPictureToBitmap(tbSkillsWindow, 140, 553-547, 70+(cpt*13)-10-18, &r);

					nw_GetMessages();
				}

				tb_DrawPictureToBitmap(tbSkillsWindow, 140, 553-547, 70+(cpt*13)-10-18, &r);

				if (tb_PointInRect(mpt, &skillRect[i]))
					sk_UseSkill(i);

				return;
			}

			cpt++;
		}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventMakeWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	TRect							r;
	//TPoint						mpt;
	int								line=0;
	//int								canAdd;
	//UInt32						canAddGold;
	//UInt32						goldTotal;
	//int								gold1;
	//int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	if (currentSkillDisplay	== kTailoringLeatherworking || currentSkillDisplay == kEnchanting || currentSkillDisplay==kBlacksmithing)
		if (currentCategory == 0)
		{
			line=0;

			for (i=1;i<kMaxInventorySize;i++)	// check for make buttons
			{
				if (makeCategory[i])
				{
					tb_SetRect(&r,553-547,70+(line*13)-10-18,553+12-547,70+(line*13)-10+12-18);
					r.right=784-547;
					line++;
				}
				else
					tb_SetRect(&r,0,0,0,0);

				if (DoFloaterButton(tbMakeWindow, pt, r, 140, 141, false, false, -1, false))
				{
					chooseMakeCategory(i);
					return;
				}

			}

		}

	//-----------------------------------------------

	line=0;

	for (i=1;i<kMaxInventorySize;i++)	// check for make buttons
	{

		if (makeList[i].itemID!=0)
		{
			if (it_HaveIngredients(makeList[i].itemID) || (localRightFinger!=0 && itemList[localInventory[localRightFinger].itemID].spell==makeList[i].itemID) || (localLeftFinger!=0 && itemList[localInventory[localLeftFinger].itemID].spell==makeList[i].itemID))
			{
				tb_SetRect(&r,553-547,70+(line*13)-10-18,553+12-547,70+(line*13)-10+12-18);
				r.right=784-547;
			}
			else
				tb_SetRect(&r,0,0,0,0);

			line++;
		}
		else
			tb_SetRect(&r,0,0,0,0);

		if (DoFloaterButton(tbMakeWindow, pt, r, 140, 141, false, false, -1, false))
		{
			if (nw_CanSend())
			{
				it_TryToMakeItem(makeList[i].itemID);
			}

			return;
		}

	}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventChessWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;

	for (i=0;i<96;i++)
		if (tb_PointInRect(pt, &chessScreenRect[i]))
		{
			if (chessSelectedPiece==i)
			{
				chessSelectedPiece=-1;
				gr_RenderChessWindow();
				return;
			}
			else if (chessSelectedPiece==-1)
			{
				chessSelectedPiece=i;
				gr_RenderChessWindow();
				return;
			}
			else
			{
				nw_SendChessMove(chessSelectedPiece,i);
				chessSelectedPiece=-1;
				gr_RenderChessWindow();
				return;
			}
		}

#endif
}

// ----------------------------------------------------------------- //
void MouseDownEventBankWindow (TPoint pt)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;
	//TRect							r;
	//TPoint						mpt;
	int								line=0;
	int								canAdd;
	UInt32						canAddGold;
	UInt32						goldTotal;
	int								gold1;
	int								gold2;
	//int								minDist;
	//int								tempDist;
	//int								foundIndex;
	//int								centerX;
	//int								centerY;
	//TBoolean          targeting;
	//TBoolean          sticky;


	line=0;

	if (((bankInventory[0].itemID!=0) || (bankInventory[0].itemCount!=0)) && (bankInventory[0].itemID<60000))
	{
		tb_SetRect(&pickUpRect[0],553-547,70+(line*13)-10-18,553+12-547,70+(line*13)-10+12-18);
		pickUpRect[0].right=784-547;

		if (DoFloaterButton(tbBankWindow, pt, pickUpRect[0], 132, 175, false, false, -1, false))
		{
			if (tb_IsKeyDown(OptionKey))
			{
				gr_AddText("How much gold do you want to withdraw?",false,true);
				playerAction=kWithdrawing;
				targetSlot=0;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				goldTotal=in_GetGoldTotal(bankInventory[0].itemID, bankInventory[0].itemCount);
				canAddGold=in_CanAddGoldtoInventory_c(goldTotal);

				if (canAddGold>0)
				{
					if (canAddGold>999999)
						canAddGold=999999;

					goldTotal=goldTotal-canAddGold;

					in_SetGoldTotal(goldTotal, &gold1, &gold2);

					bankInventory[0].itemID			=	gold1;

					bankInventory[0].itemCount	=	gold2;

					nw_SendTakeFromBank(0, canAddGold);

					gr_RenderPlayerDisplay(true);
				}
				else
					gr_AddText("You can't carry any more gold.",false,true);
			}
		}

		line++;
	}

	for (i=1;i<kMaxInventorySize;i++)	// check for pick up buttons
	{
		currentDragLine=i;

		if ((bankInventory[i].itemID!=0) && (bankInventory[i].itemID<60000))
		{
			tb_SetRect(&pickUpRect[i],553-547,70+(line*13)-10-18,553+12-547,70+(line*13)-10+12-18);
			pickUpRect[i].right=784-547;
			line++;
		}
		else
			tb_SetRect(&pickUpRect[i],0,0,0,0);

		if (DoFloaterButton(tbBankWindow, pt, pickUpRect[i], 132, 175, false, false, -1, false))
		{
			if ((tb_IsKeyDown(OptionKey)) && (in_CanGroup(bankInventory[i].itemID)) && (bankInventory[i].itemCount>1))
			{
				gr_AddText("How many do you want to withdraw?",false,true);
				playerAction=kWithdrawing;
				targetSlot=i;
				targetCount=0;
				strcpy(playerText,"");
			}
			else if (nw_CanSend())
			{
				canAdd=in_CanAddtoInventory_c(bankInventory[i].itemID, bankInventory[i].itemCount);

				if (canAdd>0)
				{
					nw_SendTakeFromBank(i, canAdd);

					if (in_CanGroup(bankInventory[i].itemID))
					{
						bankInventory[i].itemCount	=	bankInventory[i].itemCount - canAdd;

						if (bankInventory[i].itemCount==0)
							bankInventory[i].itemID		=	0;
					}
					else
					{
						bankInventory[i].itemID			=	0;
						bankInventory[i].itemCount	=	0;
					}

					gr_RenderPlayerDisplay(true);
				}
				else
					gr_AddText("Your pack is full.",false,true);
			}

			return;
		}

	}

#endif
}

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void CheckMacro (void)
// ----------------------------------------------------------------- //
{
	int							cpt;
	TBoolean					again=false;
	char						firstWord[kStringLength];
	TBoolean					petCommand=false;
	TBoolean					found=false;
	char						macroText[kStringLength];

	return;
	
	for (cpt=0; cpt<strlen(playerText); cpt++)
	{
		if (playerText[cpt]==' ')
			found=true;
		
		if (playerText[cpt]=='\0')
			break;
		
		macroText[cpt]=playerText[cpt];
		macroText[cpt+1]='\0';
		
		if(!found)
		{
			firstWord[cpt]=playerText[cpt];
			firstWord[cpt+1]='\0';
		}
	}
	
	if (pl_IsPetCommand(firstWord))
	{
		petCommand=true;
		strcpy(macroText, firstWord);
	}	
	
	for (cpt=0; cpt<kMaxMacros; cpt++)
	{
		if (strcmp(localMacros[cpt][0], macroText)==0)
		{
			if (localMacros[cpt][1][0]=='/') //Macro generated command 
			{
				if (petCommand) //pet commands can't generate other commands
					return;
				
				strcpy(playerText, localMacros[cpt][1]);
				again=true;
			}
			else //Macro generated text
			{
				nw_SendTextToHost(localMacros[cpt][1]);
				again=false;
			}	
		}
	}


	if (again)
		CheckMacro(); //Recursive
}


// ----------------------------------------------------------------- //
void ParseInputString (void)
// ----------------------------------------------------------------- //

{
	long						count;
	UInt32						gold;
	int							gold1;
	int							gold2;
	char						countStr[kStringLength]; //[16];
	long						mapElement;
	int							i;
	int               			j;
	//OSErr						e;
	//long						s;
	//FSSpec						worldFSSpec;
	int							row;
	int               			col;
	char						systemMessage[kStringLength]; //[128];
	long              			num1;
	long              			num2;
	char						numStr1[kStringLength]; //[16];
	char						numStr2[kStringLength]; //[16];
	TBoolean          			found;

	if (currentDialog!=0)
		return;

	if (strcmp(playerText,"")==0)
		return;

	for (i=0;i<kStringLength;i++)
	{
		numStr1[i]='\0';
		numStr2[i]='\0';
	}

	strcpy(numStr1,playerText);

	for (i=0;i<kStringLength;i++)
		playerText[i]='\0';

	strcpy(playerText,numStr1);

	strcpy(countStr,"");

	strcpy(systemMessage,"");

	strcpy(numStr1,"");

	strcpy(numStr2,"");

	//if (playerText[0]=='/')
		//CheckMacro();

#ifdef _SERVER
	/*
		if (strcmp(playerText,"/h1")==0)
	    {
	      tb_Beep();
	      for (row=2102;row<=2107;row++)
	        for (col=1977;col<=1981;col++)
	          {
	      			map[mapToScreenRow[90]+playerRow+row-2102][mapToScreenCol[90]+playerCol+col-1977]=map[row][col]; // server
	      			clientMap[mapToScreenRow[90]+playerRow+row-2102][mapToScreenCol[90]+playerCol+col-1977]=map[row][col];
	          }
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
	    }

		if (strcmp(playerText,"/h2")==0)
	    {
	      tb_Beep();
	      for (row=2090;row<=2095;row++)
	        for (col=1975;col<=1979;col++)
	          {
	      			map[mapToScreenRow[90]+playerRow+row-2090][mapToScreenCol[90]+playerCol+col-1975]=map[row][col]; // server
	      			clientMap[mapToScreenRow[90]+playerRow+row-2090][mapToScreenCol[90]+playerCol+col-1975]=map[row][col];
	          }
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
	    }

		if (strcmp(playerText,"/h3")==0)
	    {
	      tb_Beep();
	      for (row=2096;row<=2101;row++)
	        for (col=1966;col<=1970;col++)
	          {
	      			map[mapToScreenRow[90]+playerRow+row-2096][mapToScreenCol[90]+playerCol+col-1966]=map[row][col]; // server
	      			clientMap[mapToScreenRow[90]+playerRow+row-2096][mapToScreenCol[90]+playerCol+col-1966]=map[row][col];
	          }
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
	    }
	
	if (strcmp(playerText,".")==0)
	{
		map[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=previousTile; // server
		clientMap[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=previousTile;
	}

	if (playerText[0]=='.')	// beta ---- map editor
	{
		i=0;

		while (playerText[i]!='\0')
		{
			playerText[i]=playerText[i+1];
			i++;
		}

		playerText[i]='\0';

		tb_StringToNum(playerText,&mapElement);

		if ((mapElement>1) && (mapElement<kMaxIcons))
		{
			map[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=mapElement;
			clientMap[mapToScreenRow[90]+playerRow][mapToScreenCol[90]+playerCol]=mapElement;
			previousTile=mapElement;
		}

		strcpy(playerText,"");

		gr_RenderTextLine();
		return;
	}

	if (playerText[0]=='+')	// beta ---- map editor
	{
		i=0;

		while (playerText[i]!='\0')
		{
			playerText[i]=playerText[i+1];
			i++;
		}

		playerText[i]='\0';

		tb_StringToNum(playerText,&mapElement);

		if ((mapElement>1) && (mapElement<kMaxIcons))
		{
			clearMap=mapElement;
		}
		else
			clearMap=0;

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}*/

#endif

	//---------------------------------------------------------------------------------------------------

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='1'))
	{
		if (localPetType[0]==0)
		{
			tx_AddText(kPetNotFoundMsg);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		tb_CopyStringAt(playerText,systemMessage, 6);

		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")!=0)
			if (strlen(systemMessage)<14)
			{
				for (i=0;i<strlen(systemMessage);i++)
					if (systemMessage[i]==' ')
						systemMessage[i]='-';

				strcpy(localPetName[0],systemMessage);

				nw_SendPetName(0, systemMessage);

				tx_AddText(kPetNamedMsg);

				gr_RenderPetsWindow();
			}
			else
				tx_AddText(kNameToLongMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='2'))
	{
		if (localPetType[1]==0)
		{
			tx_AddText(kPetNotFoundMsg);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		tb_CopyStringAt(playerText,systemMessage, 6);

		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")!=0)
			if (strlen(systemMessage)<14)
			{
				for (i=0;i<strlen(systemMessage);i++)
					if (systemMessage[i]==' ')
						systemMessage[i]='-';

				strcpy(localPetName[1],systemMessage);

				nw_SendPetName(1, systemMessage);

				tx_AddText(kPetNamedMsg);

				gr_RenderPetsWindow();
			}
			else
				tx_AddText(kNameToLongMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='3'))
	{
		if (localPetType[2]==0)
		{
			tx_AddText(kPetNotFoundMsg);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		tb_CopyStringAt(playerText,systemMessage, 6);

		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")!=0)
			if (strlen(systemMessage)<14)
			{
				for (i=0;i<strlen(systemMessage);i++)
					if (systemMessage[i]==' ')
						systemMessage[i]='-';

				strcpy(localPetName[2],systemMessage);

				nw_SendPetName(2, systemMessage);

				tx_AddText(kPetNamedMsg);

				gr_RenderPetsWindow();
			}
			else
				tx_AddText(kNameToLongMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='4'))
	{
		if (localPetType[3]==0)
		{
			tx_AddText(kPetNotFoundMsg);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		tb_CopyStringAt(playerText,systemMessage, 6);

		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")!=0)
			if (strlen(systemMessage)<14)
			{
				for (i=0;i<strlen(systemMessage);i++)
					if (systemMessage[i]==' ')
						systemMessage[i]='-';

				strcpy(localPetName[3],systemMessage);

				nw_SendPetName(3, systemMessage);

				tx_AddText(kPetNamedMsg);

				gr_RenderPetsWindow();
			}
			else
				tx_AddText(kNameToLongMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='5'))
	{
		if (localPetType[4]==0)
		{
			tx_AddText(kPetNotFoundMsg);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		tb_CopyStringAt(playerText,systemMessage, 6);

		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")!=0)
			if (strlen(systemMessage)<14)
			{
				for (i=0;i<strlen(systemMessage);i++)
					if (systemMessage[i]==' ')
						systemMessage[i]='-';

				strcpy(localPetName[4],systemMessage);

				nw_SendPetName(4, systemMessage);

				tx_AddText(kPetNamedMsg);

				gr_RenderPetsWindow();
			}
			else
				tx_AddText(kNameToLongMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	//---------------------------------------------------------------------------------------------------
	if ((playerText[0]=='/') && (playerText[1]=='a') && (playerText[2]=='d') && (playerText[3]=='d'))
	{
		tb_CopyStringAt(playerText,systemMessage, 5);
		tb_DeleteStringPadding(systemMessage);

		if (strcmp(systemMessage,"")==0)
		{
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		strcpy(numStr1,systemMessage);

		tb_MakeLowerCase(numStr1);

		for (i=0;i<16;i++)
		{
			strcpy(numStr2,localParty[i]);
			tb_MakeLowerCase(numStr2);

			if (strcmp(numStr1,numStr2)==0)
			{
				tx_AddText(kAlreadyInPartyMsg);
				strcpy(playerText,"");
				gr_RenderTextLine();
				return;
			}
		}

		if (strlen(systemMessage)<15)
		{
			found=false;

			for (i=0;i<16;i++)
				if (strcmp(localParty[i],"")==0)
				{
					found=true;
					nw_SendAddToParty(i, systemMessage);
					strcpy(localParty[i],systemMessage);
					gr_RenderPartyWindow();
					cm_WriteParty();
					break;
				}

			if (found==false)
				tx_AddText(kPartyFullMsg);
		}
		else
			tx_AddText(kInvalidNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	//---------------------------------------------------------------------------------------------------
	if ((playerText[0]=='/') && (playerText[1]=='r') && (playerText[2]=='e') && (playerText[3]=='m') && (playerText[4]=='o') && (playerText[5]=='v') && (playerText[6]=='e'))
	{
		tb_CopyStringAt(playerText,systemMessage, 8);
		tb_DeleteStringPadding(systemMessage);

		if (strlen(systemMessage)<15)
		{
			for (i=0;i<strlen(systemMessage);i++)
				if (((int)systemMessage[i]>=65) && ((int)systemMessage[i]<=90))
					systemMessage[i]=(char)((int)systemMessage[i]+32);

			found=false;

			for (i=0;i<16;i++)
			{
				strcpy(numStr1,localParty[i]);

				for (j=0;j<strlen(numStr1);j++)
					if (((int)numStr1[j]>=65) && ((int)numStr1[j]<=90))
						numStr1[j]=(char)((int)numStr1[j]+32);

				if (strcmp(numStr1,systemMessage)==0)
				{
					found=true;
					strcpy(localParty[i],"");
					nw_SendRemoveFromParty(i);
					gr_RenderPartyWindow();
					cm_WriteParty();
					break;
				}
			}

			if (found==false)
				tx_AddText(kNotFoundInPartyMsg);
		}
		else
			tx_AddText(kInvalidNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	//---------------------------------------------------------------------------------------------------

	if (strcmp(playerText,"/inv")==0)
	{
		targetMode=kDumpInvTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/showbank")==0)
	{
		targetMode=kDumpBankTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/activate")==0)
	{
		targetMode=kActivateTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/aka")==0)
	{
		targetMode=kAKATarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/ip")==0)
	{
		targetMode=kIPTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/skilloff")==0)  // mod 8/4
	{
		targetMode=kNoGainTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/skillon")==0)  // mod 8/4
	{
		targetMode=kGainTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/kill")==0)
	{
		targetMode=kKillTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/res")==0)
	{
		targetMode=kResTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/reset")==0)
	{
		targetMode=kResetTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/banish")==0)
	{
		targetMode=kBanishTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/kick")==0)
	{
		targetMode=kKickTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/ban")==0)
	{
		targetMode=kBanTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/jail")==0)
	{
		targetMode=kJailTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText, "/npcinfo")==0)
	{
		targetMode=kNPCITarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/mute")==0)
	{
		targetMode=kMuteTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/skills")==0)
	{
		targetMode=kSkillsTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/unmute")==0)
	{
		targetMode=kUnmuteTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/class")==0)
	{
		targetMode=kClassTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/makeevil")==0)
	{
		targetMode=kMakeEvilTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/makegood")==0)
	{
		targetMode=kMakeGoodTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/pit")==0)
	{
		targetMode=kPitTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/home")==0)
	{
		targetMode=kHomeTarget;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/slow")==0)
	{
		nw_SendAction(kSlow, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/fast")==0)
	{
		nw_SendAction(kFast, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	//if (strcmp(playerText,"/ip")==0)
	//	{
	//		nw_SendAction(kGetIP, kHighPriority, kSendNow,false);
	//		strcpy(playerText,"");
	//    gr_RenderTextLine();
	//    return;
	//	}

	if (strcmp(playerText,"/init")==0)
	{
		nw_SendAction(kGMInit, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/info")==0)
	{
		nw_SendAction(kGMInfo, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	/*
		if (strcmp(playerText,"/gmhome")==0)
			{
				nw_SendAction(kGMHome, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}
	*/
	if (strcmp(playerText,"/ccon")==0)
	{
		nw_SendAction(kCCOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/ccoff")==0)
	{
		nw_SendAction(kCCOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/news")==0)
	{
		nw_SendAction(kNews, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/msgon")==0)
	{
		nw_SendAction(kMsgOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}


	if (strcmp(playerText,"/msgoff")==0)
	{
		nw_SendAction(kMsgOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/showmsg")==0)
	{
		nw_SendAction(kMsgShow, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/shutdown")==0)
	{
		nw_SendAction(kShutdown, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/fanfare")==0)
	{
		nw_SendAction(kFanfare, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/reload")==0)
	{
		nw_SendAction(kReload, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/reload macros")==0)
	{
		fi_LoadMacros();
		gr_AddText("Macros reloaded.", false, true);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	

	if (strcmp(playerText,"/boom")==0)
	{
		nw_SendAction(kBoom, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/beep")==0)
	{
		nw_SendAction(kBeep, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/thunder")==0)
	{
		nw_SendAction(kThunder, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/horn")==0)
	{
		nw_SendAction(kHorn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/flight")==0)
	{
		nw_SendAction(kDragon, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/flute")==0)
	{
		nw_SendAction(kFlute, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/scream")==0)
	{
		nw_SendAction(kScream, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	//Dave
	if (strcmp(playerText,"/laugh")==0)
	{
		nw_SendAction(kLichLaugh, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/splash")==0)
	{
		nw_SendAction(kSplash, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/shark")==0)
	{
		nw_SendAction(kJaws, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	//Dave^
	if (strcmp(playerText,"/mdeath")==0)
	{
		nw_SendAction(kGasp, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/fdeath")==0)
	{
		nw_SendAction(kFemaleGasp, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	/*
	if (strcmp(playerText,"/afk")==0)
	{
		nw_SendAction(kAFK, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/gkarma")==0)
	{
			if (strcmp(systemMessage,"")==0)
		{
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		strcpy(numStr1,systemMessage);

		tb_MakeLowerCase(numStr1);
		
		
	}
	
	if ((playerText[0]=='/') && (playerText[1]=='r') && (playerText[2]=='o') && (playerText[3]=='l') && (playerText[4]=='l'))
	{
		int dieNum;
		int dieKind;
		tb_CopyStringAt(playerText,countStr, 7);
		tb_StringToNum(countStr,&count);
		
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	
	if (strcmp(playerText,"/curse")==0)

	{
		targetMode=kGMCurse;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	*/
	if ((playerText[0]=='/') && (playerText[1]=='d') && (playerText[2]=='e') && (playerText[3]=='v'))
	{
		nw_SendAction(kDev, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	//Dave^
	if (strcmp(playerText,"/good")==0)
	{
		nw_SendAction(kGoodChar, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/evil")==0)
	{
		nw_SendAction(kEvilChar, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/meet next")==0)
	{
		nw_SendAction(kMeetNext, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/meet last")==0)
	{
		nw_SendAction(kMeetLast, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='g') && (playerText[2]=='u') && (playerText[3]=='i') && (playerText[4]=='l') && (playerText[5]=='d')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 7);

		if (strlen(systemMessage)<48)
			nw_SendGuildToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='s') && (playerText[2]=='i') && (playerText[3]=='g') && (playerText[4]=='n')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<kCommandStringMaxLength)
			nw_SendSignToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='s') && (playerText[3]=='a') && (playerText[4]=='y'))
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<kCommandStringMaxLength)
			nw_SendMsayToHost(systemMessage);

		targetMode=kMsayTarget;

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='n') && (playerText[2]=='o') && (playerText[3]=='t') && (playerText[4]=='e'))
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<kCommandStringMaxLength)
			nw_SendNoteToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='h') && (playerText[2]=='a') && (playerText[3]=='l') && (playerText[4]=='l')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<kCommandStringMaxLength)
			nw_SendHallToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='t') && (playerText[2]=='n') && (playerText[3]=='a') && (playerText[4]=='m') && (playerText[5]=='e')) // mod 8/25
	{
		//if (pl_AuthorizedTo(player[i].creatureIndex,kIsApprenticeGM)==false)//Dave add 11-05-2010
		
		tb_CopyStringAt(playerText,systemMessage, 7);
		if (strlen(systemMessage)<48)
			nw_SendNameChangeToHost(systemMessage, 0);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p')  && (playerText[2]=='n') && (playerText[3]=='a') && (playerText[4]=='m') && (playerText[5]=='e')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 7);

		if (strlen(systemMessage)<48)
			nw_SendNameChangeToHost(systemMessage, 1);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='t')  && (playerText[2]=='i') && (playerText[3]=='t') && (playerText[4]=='l') && (playerText[5]=='e')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 7);

		if (strlen(systemMessage)<48)
			nw_SendCustomTitleToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='e') && (playerText[2]=='x') && (playerText[3]=='p')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 5);

		if (strlen(systemMessage)<48)
			nw_SendExpToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='r') && (playerText[2]=='e') && (playerText[3]=='x') && (playerText[4]=='p')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<48)
			nw_SendRExpToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	//Whisper

	if ((playerText[0]=='/') && (playerText[1]=='w') && (playerText[2]=='h') && (playerText[3]=='i') && (playerText[4]=='s') && (playerText[5]=='p') && (playerText[6]=='e') && (playerText[7]=='r'))
	{
		tb_CopyStringAt(playerText,systemMessage, 9);
		//if (strlen(systemMessage)<48)
		nw_SendWhisperToHost(systemMessage);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

//	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='s') && (playerText[3]=='g') && (playerText[4]=='1')) // mod 8/25
//	  {
//	    tb_CopyStringAt(playerText,systemMessage, 6);
//	    if (strlen(systemMessage)<kStringLength)
//	      nw_SendMsgToHost(1,systemMessage);
//			strcpy(playerText,"");
//      gr_RenderTextLine();
//      return;
//	  }

//	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='s') && (playerText[3]=='g') && (playerText[4]=='2')) // mod 8/25
//	  {
//	    tb_CopyStringAt(playerText,systemMessage, 6);
//	    if (strlen(systemMessage)<kStringLength)
//	      nw_SendMsgToHost(2,systemMessage);
//			strcpy(playerText,"");
//      gr_RenderTextLine();
//      return;
//	  }

//	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='s') && (playerText[3]=='g') && (playerText[4]=='3')) // mod 8/25
//	  {
//	    tb_CopyStringAt(playerText,systemMessage, 6);
//	    if (strlen(systemMessage)<kStringLength)
//	      nw_SendMsgToHost(3,systemMessage);
//			strcpy(playerText,"");
//      gr_RenderTextLine();
//      return;
//	  }

	if ((playerText[0]=='/') && (playerText[1]=='o') && (playerText[2]=='w') && (playerText[3]=='n') && (playerText[4]=='e') && (playerText[5]=='r')) // mod 8/25
	{
		tb_CopyStringAt(playerText,systemMessage, 7);

		if (strlen(systemMessage)<48)
			nw_SendGiftToHost(systemMessage);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='e') && (playerText[3]=='e') && (playerText[4]=='t'))
	{
		tb_CopyStringAt(playerText,systemMessage, 6);

		if (strlen(systemMessage)<15)
			nw_SendMeetToHost(systemMessage,false);
		else
			tx_AddText(kInvalidPlayerNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='s') && (playerText[2]=='u') && (playerText[3]=='m') && (playerText[4]=='m') && (playerText[5]=='o') && (playerText[6]=='n'))
	{
		tb_CopyStringAt(playerText,systemMessage, 8);

		if (strlen(systemMessage)<15)
			nw_SendSummonToHost(systemMessage,1);
		else
			tx_AddText(kInvalidPlayerNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='r') && (playerText[2]=='e') && (playerText[3]=='t') && (playerText[4]=='u') && (playerText[5]=='r') && (playerText[6]=='n'))
	{
		tb_CopyStringAt(playerText,systemMessage, 8);

		if (strlen(systemMessage)<15)
			nw_SendSummonToHost(systemMessage,2);
		else
			tx_AddText(kInvalidPlayerNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if (playerText[0]=='/' && playerText[1]=='a' && playerText[2]=='n' && playerText[3]=='n') // /ann
	{
		tb_CopyStringAt(playerText, systemMessage, 5);
		nw_SendAnnounceToHost(systemMessage);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='t') && (playerText[2]=='m') && (playerText[3]=='e') && (playerText[4]=='e') && (playerText[5]=='t'))  // tmeet
	{
		tb_CopyStringAt(playerText,systemMessage, 7);

		if (strlen(systemMessage)<15)
			nw_SendMeetToHost(systemMessage,true);
		else
			tx_AddText(kInvalidPlayerNameMsg);

		strcpy(playerText,"");

		gr_RenderTextLine();

		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='b') && (playerText[2]=='u') && (playerText[3]=='g'))
	{
		tb_CopyStringAt(playerText,systemMessage, 4);
		nw_SendBugTextToHost(systemMessage);
		strcpy(playerText,"");
		tx_AddText(kCommentLoggedMsg);
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='a') && (playerText[2]=='l') && (playerText[3]=='l') && (playerText[4]==' '))
	{
		tb_CopyStringAt(playerText,systemMessage, 5);
		nw_SendBroadcastTextToHost(systemMessage);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/sharing on")==0)
	{
		nw_SendAction(kSharingOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		tx_AddText(kSharingOnMsg);
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/sharing off")==0)
	{
		nw_SendAction(kSharingOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		tx_AddText(kSharingOffMsg);
		gr_RenderTextLine();
		return;
	}

	//if (strcmp(playerText,"/return")==0)
//		{
//			nw_SendAction(kReturn, kHighPriority, kSendNow,false);
//			strcpy(playerText,"");
//      gr_RenderTextLine();
//		}

	if (strcmp(playerText,"/users")==0)
	{
		nw_SendAction(kUsers, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/status")==0)
	{
		nw_SendAction(kStatus, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/pos")==0)
	{
		nw_SendAction(kPosition, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/stealth on")==0)
	{
		nw_SendAction(kStealthOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/stealth off")==0)
	{
		nw_SendAction(kStealthOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/notrack on")==0)
	{
		nw_SendAction(kNoTrackOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/notrack off")==0)
	{
		nw_SendAction(kNoTrackOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/autolock on")==0)
	{
		nw_SendAction(kAutoLockOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/autolock off")==0)
	{
		nw_SendAction(kAutoLockOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/godmode on")==0)
	{
		nw_SendAction(kGodModeOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/godmode off")==0)
	{
		nw_SendAction(kGodModeOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if (strcmp(playerText,"/nodeath on")==0)
	{
		nw_SendAction(kNoDeathOn, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/nodeath off")==0)
	{
		nw_SendAction(kNoDeathOff, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	//-------------------------------------

	//-------------------------------------

	if (strcmp(playerText,"/fx")==0)
	{
		nw_SendAction(kGetFX, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/clear")==0)
	{
		nw_SendAction(kKillFX, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	/*
		if (strcmp(playerText,"/prison")==0)
			{
				nw_SendAction(kGotoPrison, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/inprison")==0)
			{
				nw_SendAction(kGotoInPrison, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/desert")==0)
			{
				nw_SendAction(kGotoDesert, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/ice")==0)
			{
				nw_SendAction(kGotoIceIsland, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/dungeon")==0)
			{
				nw_SendAction(kGotoDungeon, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/bank")==0)
			{
				nw_SendAction(kGotoBank, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/cemetery")==0)
			{
				nw_SendAction(kGotoCemetary, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/lizard")==0)
			{
				nw_SendAction(kGotoLizard, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/castle")==0)
			{
				nw_SendAction(kGotoCastle, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/sewer")==0)
			{
				nw_SendAction(kGotoSewer, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/carth")==0)
			{
				nw_SendAction(kGotoCarth, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/trince")==0)
			{
				nw_SendAction(kGotoTrince, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}

		if (strcmp(playerText,"/mince")==0)
			{
				nw_SendAction(kGotoMince, kHighPriority, kSendNow,false);
				strcpy(playerText,"");
	      gr_RenderTextLine();
	      return;
			}
	*/
	//-------------------------------------

	if ((playerText[0]=='/') && (playerText[1]=='s') && (playerText[2]=='p') && (playerText[3]=='a') && (playerText[4]=='w') && (playerText[5]=='n'))
	{
		tb_CopyStringAt(playerText,countStr, 7);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(0, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='g') && (playerText[2]=='o'))
	{
		tb_CopyStringAt(playerText,countStr, 4);
		numStr1[0]=countStr[0];
		numStr1[1]=countStr[1];
		numStr1[2]=countStr[2];
		numStr1[3]=countStr[3];
		numStr1[4]='\0';
		numStr2[0]=countStr[5];
		numStr2[1]=countStr[6];
		numStr2[2]=countStr[7];
		numStr2[3]=countStr[8];
		numStr2[4]='\0';
		tb_StringToNum(numStr1,&num1);
		tb_StringToNum(numStr2,&num2);
		nw_SendGMCommand(1, num1, num2);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='c') && (playerText[2]=='r') && (playerText[3]=='e') && (playerText[4]=='a') && (playerText[5]=='t') && (playerText[6]=='u') && (playerText[7]=='r') && (playerText[8]=='e'))
	{
		tb_CopyStringAt(playerText,countStr, 10);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(2, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='m') && (playerText[2]=='o') && (playerText[3]=='r') && (playerText[4]=='p') && (playerText[5]=='h'))
	{
		tb_CopyStringAt(playerText,countStr, 7);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(3, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='d') && (playerText[2]=='r') && (playerText[3]=='o') && (playerText[4]=='p'))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(4, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='d') && (playerText[2]=='e') && (playerText[3]=='c') && (playerText[4]=='a') && (playerText[5]=='y'))
	{
		tb_CopyStringAt(playerText,countStr, 7);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(5, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='l') && (playerText[2]=='o') && (playerText[3]=='c') && (playerText[4]=='k') && (playerText[5]==' '))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(6, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='t') && (playerText[2]=='r') && (playerText[3]=='a') && (playerText[4]=='p'))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(7, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='p') && (playerText[2]=='r') && (playerText[3]=='o') && (playerText[4]=='p'))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(8, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='t') && (playerText[2]=='r') && (playerText[3]=='a') && (playerText[4]=='c') && (playerText[5]=='k') && (playerText[6]=='s'))
	{
		tb_CopyStringAt(playerText,countStr, 8);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(9, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='n') && (playerText[2]=='p') && (playerText[3]=='c'))
	{
		tb_CopyStringAt(playerText,countStr, 5);
		numStr1[0]=countStr[0];
		i = 0;
		for(j=0;j<=strlen(countStr);j++)
		{
			if(countStr[j]==',')
				break;
			
			numStr1[i]=countStr[j];
			i++;	
		}
		j++;
		numStr1[i]='\0';
		i = 0;
		
		for(j=j;j<=strlen(countStr);j++)
		{
			numStr2[i]=countStr[j];
			i++;	
		}

		numStr2[i]='\0';
		tb_StringToNum(numStr1,&num1);
		tb_StringToNum(numStr2,&num2);
		nw_SendGMCommand(10, num1, num2);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='s') && (playerText[2]=='p') && (playerText[3]=='e') && (playerText[4]=='e') && (playerText[5]=='d'))
	{
		tb_CopyStringAt(playerText,countStr, 7);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(15, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='h') && (playerText[2]=='g') && (playerText[3]=='u')&& (playerText[4]=='i')&& (playerText[5]=='l')&& (playerText[6]=='d'))
	{
		tb_CopyStringAt(playerText,countStr, 8);
		nw_SendHallGuildToServer(countStr);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='h') && (playerText[2]=='e') && (playerText[3]=='x')&& (playerText[4]=='t')&& (playerText[5]=='r')&& (playerText[6]=='a'))
	{
		tb_CopyStringAt(playerText,countStr, 8);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(12, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if ((playerText[0]=='/') && (playerText[1]=='g') && (playerText[2]=='m') && (playerText[3]=='s') && (playerText[4]=='p') && (playerText[5]=='a') && (playerText[6]=='w') && (playerText[7]=='n'))
	{
		tb_CopyStringAt(playerText,countStr, 9);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(14, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}
	
	if ((playerText[0]=='/') && (playerText[1]=='d') && (playerText[2]=='n') && (playerText[3]=='p') && (playerText[4]=='c'))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(16, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		targetMode=kDressupTarget;
		return;
	}
	
	if ((playerText[0]=='/') && (playerText[1]=='d') && (playerText[2]=='r') && (playerText[3]=='e') && (playerText[4]=='d'))
	{
		tb_CopyStringAt(playerText,countStr, 6);
		tb_StringToNum(countStr,&count);
		nw_SendGMCommand(17, count, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

#ifdef _SERVER
	if (strcmp(playerText,"/id")==0)
	{
		tb_NumToString(map[mapToScreenRow[90]+playerRow][mapToScreenRow[90]+playerCol],countStr);
		gr_AddText(countStr,false,true);
		strcpy(playerText,"");
		gr_RenderTextLine();
	}

#endif

//#ifdef _DEBUGGING
//	if (strcmp(playerText,"/pos")==0)
//		{
//			numtostring((mapToScreenRow[90]+playerRow),countStr);
//			gr_AddText(countStr,false,true);
//			numtostring((mapToScreenRow[90]+playerCol),countStr);
//			gr_AddText(countStr,false,true);
//			strcpy(playerText,"");
//		}
//#endif

#ifdef _SERVER
	if (strcmp(playerText,"/opos")==0)
	{
		tb_NumToString((playerRow),countStr);
		gr_AddText(countStr,false,true);
		tb_NumToString((playerCol),countStr);
		gr_AddText(countStr,false,true);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

#endif

	if (strcmp(playerText,"/hchest")==0)
	{
		nw_SendGMCommand(11, 0, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/hclear")==0)
	{
		nw_SendGMCommand(13, 0, 0);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

#ifdef _SERVER
	if (strcmp(playerText,"/save")==0)
	{
		for (row=1;row<(kMapHeight-1);row++)
			for (col=1;col<(kMapWidth-1);col++)
			{

				if ((map[row][col]==535) && canSwimIn[map[row][col+1]])
					map[row][col+1]=275;

				if ((map[row][col]==535) && canSwimIn[map[row+1][col]])
					map[row+1][col]=275;

				if ((map[row][col]==535) && canSwimIn[map[row][col-1]])
					map[row][col-1]=275;

				if ((map[row][col]==535) && canSwimIn[map[row-1][col]])
					map[row-1][col]=275;

			}

		//fi_SaveGuildHalls();
		tb_OpenOutputFile("Oberin World 2", false);

		tb_WriteToFile(&map[0],							sizeof(map));

		tb_CloseFile();

		strcpy(playerText,"");

		gr_RenderTextLine();

		/*FSMakeFSSpec(AppFSSpec.vRefNum, AppFSSpec.parID, "\pworld 2", &worldFSSpec);
		e=FSpOp}F (&worldFSSpec, fsCurPerm, &refNum);
		e=SetFPos(refNum, fsFromStart,0);
		s=(kMapHeight*2)*700;
		for (row=0;row<kMapHeight;row=row+700)
			{
				e = FSWrite(refNum, &s, &map[row][0]);
			}
		e	=	FSClose(refNum);
		strcpy(playerText,"");*/
	}

#endif

//	if (strcmp(playerText,"/i")==0)
//		{
//			nw_SendAction(kRefreshInventory, kHighPriority, kSendNow,true);
//			strcpy(playerText,"");
//			ClearPlayerAction();
//			return;
//		}

	//----------------------------- assign function keys

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='5'))
	{
		tb_CopyStringAt(playerText,f15Str, 4);
		tb_DeleteStringPadding(f15Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='4'))
	{
		tb_CopyStringAt(playerText,f14Str, 4);
		tb_DeleteStringPadding(f14Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='3'))
	{
		tb_CopyStringAt(playerText,f13Str, 4);
		tb_DeleteStringPadding(f13Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='2'))
	{
		tb_CopyStringAt(playerText,f12Str, 4);
		tb_DeleteStringPadding(f12Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='1'))
	{
		tb_CopyStringAt(playerText,f11Str, 4);
		tb_DeleteStringPadding(f11Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1') && (playerText[3]=='0'))
	{
		tb_CopyStringAt(playerText,f10Str, 4);
		tb_DeleteStringPadding(f10Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='9'))
	{
		tb_CopyStringAt(playerText,f9Str, 3);
		tb_DeleteStringPadding(f9Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='8'))
	{
		tb_CopyStringAt(playerText,f8Str, 3);
		tb_DeleteStringPadding(f8Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='7'))
	{
		tb_CopyStringAt(playerText,f7Str, 3);
		tb_DeleteStringPadding(f7Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='6'))
	{
		tb_CopyStringAt(playerText,f6Str, 3);
		tb_DeleteStringPadding(f6Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='5'))
	{
		tb_CopyStringAt(playerText,f5Str, 3);
		tb_DeleteStringPadding(f5Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='4'))
	{
		tb_CopyStringAt(playerText,f4Str, 3);
		tb_DeleteStringPadding(f4Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='3'))
	{
		tb_CopyStringAt(playerText,f3Str, 3);
		tb_DeleteStringPadding(f3Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='2'))
	{
		tb_CopyStringAt(playerText,f2Str, 3);
		tb_DeleteStringPadding(f2Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if ((playerText[0]=='/') && ((playerText[1]=='f') || (playerText[1]=='F')) && (playerText[2]=='1'))
	{
		tb_CopyStringAt(playerText,f1Str, 3);
		tb_DeleteStringPadding(f1Str);
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}


	//-------------- player command line --------------

	if (playerText[0]=='/') // convert command line to lower case
	{
		tb_DeleteStringPadding(playerText);

		for (i=0;i<strlen(playerText);i++)
			if (((int)playerText[i]>=65) && ((int)playerText[i]<=90)) // make item name lower case
				playerText[i]=(char)((int)playerText[i]+32);
	}

	if (strcmp(playerText,"/help")==0)
	{
		nw_SendAction(kHelp, kHighPriority, kSendNow,false);
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	//----------------------------- skills

	if (!playerDead)
	{

		if (strcmp(playerText,"/alchemy/cleric")==0)
		{
			sk_UseSkill(kAlchemyCleric);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/alchemy/druid")==0)
		{
			sk_UseSkill(kAlchemyDruid);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}
		
		if (strcmp(playerText,"/enchanting")==0)
		{
			sk_UseSkill(kEnchanting);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/poisoning")==0)
		{
			sk_UseSkill(kPoisoning);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/anatomy")==0)
		{
			sk_UseSkill(kAnatomy);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/blacksmithing")==0)
		{
			sk_UseSkill(kBlacksmithing);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/cooking")==0)
		{
			sk_UseSkill(kCooking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/detecting hidden")==0)
		{
			sk_UseSkill(kDetectingHidden);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/fishing")==0)
		{
			sk_UseSkill(kFishing);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/healing")==0)
		{
			sk_UseSkill(kHealing);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/hiding")==0)
		{
			sk_UseSkill(kHiding);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/lockpicking")==0)
		{
			sk_UseSkill(kLockpicking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/lumberjacking")==0)
		{
			sk_UseSkill(kLumberjacking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/magery/cleric")==0)
		{
			sk_UseSkill(kMageryCleric);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/magery/druid")==0)
		{
			sk_UseSkill(kMageryDruid);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/magery/wizard")==0)
		{
			sk_UseSkill(kMageryWizard);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/meditation")==0)
		{
			sk_UseSkill(kMeditation);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/special/fighter")==0)
		{
			sk_UseSkill(kSpecialFighter);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/special/ranger")==0)
		{
			sk_UseSkill(kSpecialRanger);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/special/rogue")==0)
		{
			sk_UseSkill(kSpecialRogue);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/mining")==0)
		{
			sk_UseSkill(kMining);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/removing traps")==0)
		{
			sk_UseSkill(kRemoveTrap);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/tailoring")==0)
		{
			sk_UseSkill(kTailoringLeatherworking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}
		
		if (strcmp(playerText,"/enchanting")==0)
		{
			sk_UseSkill(kEnchanting);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/tinkering")==0)
		{
			sk_UseSkill(kTinkering);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/woodcrafting")==0)
		{
			sk_UseSkill(kWoodworking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/stealth")==0)
		{
			sk_UseSkill(kStealth);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/taming")==0)
		{
			sk_UseSkill(kTaming);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if (strcmp(playerText,"/tracking")==0)
		{
			sk_UseSkill(kTracking);
			strcpy(playerText,"");
			gr_RenderTextLine();
			return;
		}

		if ((strcmp(playerText,"/get")==0) && (playerDead==false))
		{
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();

			if (standingOnContainer)
			{
				nw_SendAction(kPickUp, kNormalPriority, kCanWait,true);
				in_ClearPickUpPack();

				if (currentInfoDisplay==kPackContentsDisplay)
				{
					currentInfoDisplay=kCharacterDisplay;
					gr_RenderPlayerDisplay(true);
				}
			}

			return;
		}

		if (strcmp(playerText,"/insult")==0)
		{
			nw_SendAction(kInsult, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		}

	}

	//----------------------------- path find on/off

	if (strcmp(playerText,"/map")==0)
	{
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if (strcmp(playerText,"/path on")==0)
	{
		pathFind=true;
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if (strcmp(playerText,"/path off")==0)
	{
		pathFind=false;
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if (strcmp(playerText,"/path toggle")==0)
	{
		if (pathFind)
		{
			pathFind=false;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}
		else
		{
			pathFind=true;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}

		return;
	}

	//----------------------------- hud display on/off
	/*
	if (strcmp(playerText,"/hud on")==0)
		{
			hudOn=true;
			strcpy(playerText,"");
	  cm_WritePrefs();
			return;
		}
		
	if (strcmp(playerText,"/hud off")==0)
		{
			hudOn=false;
			strcpy(playerText,"");
	  cm_WritePrefs();
			return;
		}

	if (strcmp(playerText,"/hud toggle")==0)
		{
		  if (hudOn)
		    {
			    hudOn=false;
	      cm_WritePrefs();
			    strcpy(playerText,"");
			  }
			else
			  {
			    hudOn=true;
	      cm_WritePrefs();
			    strcpy(playerText,"");
			  }
			return;
		}
	*/
	//----------------------------- chat display

	//----------------------------- names display on/off

	if (strcmp(playerText,"/names on")==0)
	{
		namesOn=true;
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if (strcmp(playerText,"/names off")==0)
	{
		namesOn=false;
		strcpy(playerText,"");
		gr_RenderTextLine();
		cm_WritePrefs();
		return;
	}

	if (strcmp(playerText,"/names toggle")==0)
	{
		if (namesOn)
		{
			namesOn=false;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}
		else
		{
			namesOn=true;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}

		return;
	}
	
	
	//Dave dice rolls
	
	if (strcmp(playerText,"/1d6")==0)
	{
		nw_SendAction(k1d6, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	
	if (strcmp(playerText,"/1d20")==0)
	{
		nw_SendAction(k1d20, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	if (strcmp(playerText,"/1d100")==0)

	{
		nw_SendAction(k1d100, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	if (strcmp(playerText,"/int")==0)

	{
		nw_SendAction(kWisdomRoll, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	if (strcmp(playerText,"/str")==0)

	{
		nw_SendAction(kStrengthRoll, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	
	if (strcmp(playerText,"/dex")==0)

	{
		nw_SendAction(kDexRoll, kNormalPriority, kCanWait,true);
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
			return;
		
	}
	
	
	//----------------------------- chime effects on/off

	if (strcmp(playerText,"/chime on")==0)
	{
		chimeOn=true;
		cm_WritePrefs();
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/chime off")==0)
	{
		chimeOn=false;
		cm_WritePrefs();
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (strcmp(playerText,"/chime toggle")==0)
	{
		if (chimeOn)
		{
			chimeOn=false;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}
		else
		{
			chimeOn=true;
			cm_WritePrefs();
			strcpy(playerText,"");
			gr_RenderTextLine();
		}

		return;
	}




	//----------------------------- music effects on/off
	/*
		if (strcmp(playerText,"/music on")==0)
			{
				musicOn=true;
	      startMusic();
	      cm_WritePrefs();
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
			}

		if (strcmp(playerText,"/music off")==0)
			{
				musicOn=false;
		    if (theMusic!=NULL)
			    StopMovie(theMusic);
	      cm_WritePrefs();
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
			}

		if (strcmp(playerText,"/music toggle")==0)
			{
			  if (musicOn)
			    {
				    musicOn=false;
		        if (theMusic!=NULL)
			        StopMovie(theMusic);
	          cm_WritePrefs();
				    strcpy(playerText,"");
	          gr_RenderTextLine();
				  }
				else
				  {
				    musicOn=true;
	          startMusic();
	          cm_WritePrefs();
				    strcpy(playerText,"");
	          gr_RenderTextLine();
				  }
				return;
			}
	*/
	//----------------------------- sound effects on/off
	/*
		if (strcmp(playerText,"/sound on")==0)
			{
				tb_SoundOn=true;
	      cm_WritePrefs();
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
			}

		if (strcmp(playerText,"/sound off")==0)
			{
				tb_SoundOn=false;
	      cm_WritePrefs();
				strcpy(playerText,"");
	      gr_RenderTextLine();
				return;
			}

		if (strcmp(playerText,"/sound toggle")==0)
			{
			  if (tb_SoundOn)
			    {
				    tb_SoundOn=false;
	          cm_WritePrefs();
				    strcpy(playerText,"");
	          gr_RenderTextLine();
				  }
				else
				  {
				    tb_SoundOn=true;
	          cm_WritePrefs();
				    strcpy(playerText,"");
	          gr_RenderTextLine();
				  }
				return;
			}
	*/
	//-----------------------------

	//if (strcmp(playerText,"/r")==0)
	//	{
	//		gr_AddText("Target the item in your inventory that you want to repair...");
	//		targetMode=kInventoryTarget;
	//		strcpy(playerText,"");
	//		ClearPlayerAction();
	//		return;
	//	}

	if (playerAction==kGiving)	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		tx_AddText(kTargetRecepientMsg);

		targetMode=kGiveTarget;
		targetCount=count;
		strcpy(playerText,"");
		gr_RenderTextLine();
		return;
	}

	if (playerAction==kLocate)	//--------------------------------------------------------------------
	{
		if ((strcmp(playerText,"")==0) || (strlen(playerText)>14))
		{
			ClearPlayerAction();
			return;
		}

		nw_SendLocateToHost(playerText);

		strcpy(playerText,"");
		gr_RenderTextLine();
		playerAction=0;
		ClearPlayerAction();
		return;
	}

	if ((playerAction==kDropping) || (playerAction==kDepositing))	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		if (targetSlot==0)
		{
			gold=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);

			if (count>gold)
				count=gold;

			nw_SendGiveGold(255, count);

			strcpy(playerText,"");

			gr_RenderTextLine();

			playerAction=0;

			ClearPlayerAction();
		}
		else
		{
			if (in_CanGroup(localInventory[targetSlot].itemID))
			{
				if (count>kMaxInventoryItemCount)
					count=kMaxInventoryItemCount;

				if (count>localInventory[targetSlot].itemCount)
					count=localInventory[targetSlot].itemCount;
			}

			nw_SendItemTransferMessage(255, targetSlot, count);

			playerAction=0;
			targetSlot=0;
			strcpy(playerText,"");
			gr_RenderTextLine();
			ClearPlayerAction();
		}

		return;
	}

	if (playerAction==kTaking)	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		if (targetSlot==0)
		{
			gold=in_GetGoldTotal(pickUpInventory[0].itemID, pickUpInventory[0].itemCount);

			if (count>gold) count=gold;

			count=in_CanAddGoldtoInventory_c(count);

			if (count>0)
			{
				gold=gold-count;
				in_SetGoldTotal(gold, &gold1, &gold2);
				pickUpInventory[0].itemID			=	gold1;
				pickUpInventory[0].itemCount	=	gold2;
				nw_SendTakeFromList(0, count);
				gr_RenderPlayerDisplay(true);
			}
			else
				tx_AddText(kCantCarryMoreGoldMsg);

			strcpy(playerText,"");

			gr_RenderTextLine();

			playerAction=0;

			ClearPlayerAction();
		}
		else
		{
			if (count>kMaxInventoryItemCount)
				count=kMaxInventoryItemCount;

			if (count>pickUpInventory[targetSlot].itemCount)
				count=pickUpInventory[targetSlot].itemCount;

			count=in_CanAddtoInventory_c(pickUpInventory[targetSlot].itemID, count);

			if (count>0)
			{
				nw_SendTakeFromList(targetSlot, count);

				if (in_CanGroup(pickUpInventory[targetSlot].itemID))
				{
					pickUpInventory[targetSlot].itemCount	=	pickUpInventory[targetSlot].itemCount - count;

					if (pickUpInventory[targetSlot].itemCount==0)
						pickUpInventory[targetSlot].itemID	=	0;
				}
				else
				{
					pickUpInventory[targetSlot].itemID		=	0;
					pickUpInventory[targetSlot].itemCount	=	0;
				}

				gr_RenderPlayerDisplay(true);
			}
			else
				tx_AddText(kPackIsFullMsg);

			playerAction=0;

			targetSlot=0;

			strcpy(playerText,"");

			gr_RenderTextLine();

			ClearPlayerAction();
		}

		return;
	}

	if (playerAction==kWithdrawing)	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		if (targetSlot==0)
		{
			gold=in_GetGoldTotal(bankInventory[0].itemID, bankInventory[0].itemCount);

			if (count>gold) count=gold;

			count=in_CanAddGoldtoInventory_c(count);

			if (count>0)
			{
				if (count>999999)
					count=999999;

				gold=gold-count;

				in_SetGoldTotal(gold, &gold1, &gold2);

				bankInventory[0].itemID			=	gold1;

				bankInventory[0].itemCount	=	gold2;

				nw_SendTakeFromBank(0, count);

				gr_RenderPlayerDisplay(true);
			}
			else
				tx_AddText(kCantCarryMoreGoldMsg);

			strcpy(playerText,"");

			gr_RenderTextLine();

			playerAction=0;

			ClearPlayerAction();
		}
		else
		{
			if (count>kMaxInventoryItemCount)
				count=kMaxInventoryItemCount;

			if (count>bankInventory[targetSlot].itemCount)
				count=bankInventory[targetSlot].itemCount;

			count=in_CanAddtoInventory_c(bankInventory[targetSlot].itemID, count);

			if (count>0)
			{
				nw_SendTakeFromBank(targetSlot, count);

				if (in_CanGroup(bankInventory[targetSlot].itemID))
				{
					bankInventory[targetSlot].itemCount	=	bankInventory[targetSlot].itemCount - count;

					if (bankInventory[targetSlot].itemCount==0)
						bankInventory[targetSlot].itemID	=	0;
				}
				else
				{
					bankInventory[targetSlot].itemID		=	0;
					bankInventory[targetSlot].itemCount	=	0;
				}

				gr_RenderPlayerDisplay(true);
			}
			else
				tx_AddText(kPackIsFullMsg);

			playerAction=0;

			targetSlot=0;

			strcpy(playerText,"");

			gr_RenderTextLine();

			ClearPlayerAction();
		}

		return;
	}

	if (playerAction==kBuying)	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		if (count>kMaxInventoryItemCount)
			count=kMaxInventoryItemCount;

		if (count>0)
		{
			gold=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);

			if (shopInventory[targetSlot].cost>gold)
			{
				tx_AddText(kNotEnoughGoldMsg);
			}
			else
			{
				count=in_CanAddtoInventory_c(shopInventory[targetSlot].itemID, count);

				if (count>0)
				{
					while ((shopInventory[targetSlot].cost*count)>gold)
					{
						count--;

						if (count==0) break;
					}

					if (count>0)
					{
						nw_SendBuyFromList(targetSlot, count);
					}
				}
				else
					tx_AddText(kPackIsFullMsg);
			}
		}

		playerAction=0;

		targetSlot=0;
		strcpy(playerText,"");
		gr_RenderTextLine();
		ClearPlayerAction();
		return;

	}

	if (playerAction==kSelling)	//--------------------------------------------------------------------
	{
		count=0;
		tb_StringToNum(playerText,&count);

		if (count<1)
		{
			ClearPlayerAction();
			return;
		}

		if (in_CanGroup(localInventory[targetSlot].itemID))
		{
			if (count>in_QuantityOf_c(localInventory[targetSlot].itemID))
				count=in_QuantityOf_c(localInventory[targetSlot].itemID);
		}

		nw_SendItemTransferMessage(254, targetSlot, count);

		playerAction=0;
		targetSlot=0;
		strcpy(playerText,"");
		gr_RenderTextLine();
		ClearPlayerAction();
		return;
	}



	nw_SendTextToHost(playerText);

	strcpy(playerText,"");
	gr_RenderTextLine();
	ClearPlayerAction();

}


#endif