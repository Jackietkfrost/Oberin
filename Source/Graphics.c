#include "Toolbox.h"
#include "Graphics.h"
#include "Globals.h"
#include "Common.h"
#include "Inventory.h"
#include "Players.h"
#include "Skills.h"
#include "Creatures.h"
#include "Items.h"
#include "Shops.h"
#include "FX.h"
#include "FileIO.h"
#include "Action.h"
#include "Resource.h"
// #include "Floater.h"
#include "MacWindows.h"
#include "GameText.h"
#include "Async Sound.h"
#include "Npc.h"

#define             kMaxChatLogLines              500
#define             kMaxChatLogLineLength         75
#define             kNullBase         						0

TBoolean						gotFirstDrawMessage;
TBoolean						thisIsFirstRefresh;

char                chatLog[kMaxChatLogLines][kStringLength]; //[kMaxChatLogLineLength];
char                chatLogType[kMaxChatLogLines];

int									sparkle[kMaxScreenElements];

int                 stepFrame=4;
int                 stepDirection;
unsigned long       nextRenderStep1;
unsigned long       nextRenderStep2;
unsigned long       nextRenderStep3;
unsigned long       nextRenderStep4;

int									localEffectsID					[kMaxLocalFX];
UInt8								localEffectsType				[kMaxLocalFX];
UInt8								localEffectsLocation		[kMaxLocalFX];
UInt8								localEffectsDestination	[kMaxLocalFX];
TBoolean							localEffectsServiced		[kMaxLocalFX];
int									localEffectsIndex				[kMaxLocalFX];
int									localEffectsFrame		[kMaxLocalFX];

int									prevDoorCount;
short					      nameFont;
short               chatFont;
TBoolean            darkTextBacks;

int                 localPetStatus          [kMaxPets];
int                 localPetType            [kMaxPets];
char                localPetName            [kMaxPets][kStringLength];
int                 localPetLoyalty         [kMaxPets];

TBoolean						inDungeon;
int									currentDialog;
TRect								currentDialogButton1;
TRect								currentDialogButton2;

inventoryType				localSortedInventory		[kMaxInventorySize];
int									localInventoryLookup		[kMaxInventorySize];

TBoolean						wasStandingInShop;//				=	standingInShop;
TBoolean						wasStandingInBank;//				=	standingInBank;
TBoolean						wasStandingOnActionSpot;//	=	standingOnActionSpot;

#ifndef _SERVERONLY
TBitmap							osBMP;											// offscreen bitmap
TBitmap							playBMP;
TBitmap							smallGridBMP;								// small game icons
TBitmap							smallGridMaskBMP;						// small game icons mask
TBitmap							weaponGridBMP;
TBitmap							weaponGridMaskBMP;
TBitmap							womanGridBMP;
TBitmap							womanGridMaskBMP;
TBitmap							manGridBMP;
TBitmap							manGridMaskBMP;
TBitmap							armorGridBMP;
TBitmap							armorGridMaskBMP;
TBitmap							headGridBMP;
TBitmap							headGridMaskBMP;
TBitmap							humanBuilderBMP;
TBitmap							humanBuilderMaskBMP;
TBitmap							humanBuilderWorkSpaceBMP;
TBitmap							humanAfkBMP;
TBitmap							humanAfkMaskBMP;
TBitmap							havenGridBMP;
TBitmap							havenGridMaskBMP;
TBitmap							dungeonGridBMP;
TBitmap							dungeonGridMaskBMP;
TBitmap							caveGridBMP;
TBitmap							caveGridMaskBMP;
TBitmap							iceGridBMP;
TBitmap							iceGridMaskBMP;
TBitmap							creatureGridBMP[6];
TBitmap							creatureGridMaskBMP[6];
TBitmap							mapViewBMP;
TBitmap							mapViewMaskBMP;
TBitmap							townGridBMP;
TBitmap							townGridMaskBMP;
TBitmap							containerBMP;
TBitmap							containerMaskBMP;
TBitmap							greendragonBMP;
TBitmap							greendragonMaskBMP;
TBitmap							statBMP;
TBitmap							scrollBMP;
TBitmap							chessBMP;
TBitmap							chessMaskBMP;
TBitmap							chessSelectBMP;
TBitmap							regionBMP;
TBitmap							creatureColorizerBMP;
TBitmap							mapCenterBMP;
TBitmap							mapCenterMaskBMP;
TBitmap							mapTrackingBMP[12];
TBitmap							mapTrackingMaskBMP[3];
TBitmap							chatBMP;
TBitmap							statsBMP;
TBitmap							CalendarBMP;
TBitmap							targetBMP;
TBitmap							targetMaskBMP;
TBitmap							getBMP;
TBitmap							getMaskBMP;
TBitmap							textLineBMP;
TBitmap							tracksBMP;
TBitmap							tracksMaskBMP;

TBitmap							bufferBMP;
TBitmap							questTextBMP;

TBitmap							mapWindowBMP;
//TRect               mapWindowRect;
TRect               currentMapWindowsRect;
TRect               textLineRect;
TRect               textLineScreenRect;

//TBitmap							mainWindowBMP;
//TRect               mainWindowRect;
//TRect               currentMainWindowsRect;
TRect               statsRect;
TRect               targetRect;
TRect               getRect;
TRect               questTextRect;

#endif

int									localTarget;

TRect               mapCenterRect;
TRect               mapCenterDestRect;
int                 chessSelectedPiece;
TRect               chessRect[96];
TRect               chessScreenRect[96];
TRect               chessPiece[13];
TRect               chatRect;

TRect								regionBMPRect;
TRect								statRect;
TRect								scrollRect;

TBoolean						nightVision;

char								localParty[16][kStringLength]; //[15];
int									idCreatureID;
int									idTitle;
int									idGender;
char								idPlayerName[kStringLength]; //[15];
char								idGuildName[kStringLength]; //[15];
char								idNPCName[kStringLength]; //[15];
char                idPetName[kStringLength];
int									idClass;//
int									idClassLevel;//
int									idRightHand;//
int									idLeftHand;//
int									idLegs;//
int									idTorso;//
int									idFeet;//
int									idArms;//
int									idRightFinger;//
int									idLeftFinger;//
int									idHead;//
int									idNeck;
int									idHands;//
int									idWaist;//
int									idKarma;//
char								idCustomTitle[kStringLength];

int                 gxoffset;
int                 gyoffset;

int									currentMainBitmap;
int									currentDungeonBitmap;

int									currentSkillDisplay;
int									currentCategory;
TRect								mapViewRect;
int									flip=0;
TRect								humanBuilderRect;
TRect								osRect;
TRect								destRect[kMaxScreenElements];							// screen rectangles of play area
TRect								destCreatureRect[kMaxScreenElements];							// screen rectangles of play area
TRect								destScreenRect[kMaxScreenElements];							// screen rectangles of play area
TRect								srcRect[kMaxIcons];							// source rectangles of objects
TRect								creatureRect[5][8][3];
TBoolean						elevated[kMaxIcons];							// specifies whether object is elevated above ground. ex: tree
TBoolean						obstacle[kMaxIcons];							// specifies whether object will block spells and archery
TBoolean						canWalkOn[kMaxIcons];						// specifies whether object can be walked on. ex: grass
TBoolean						canFlyOver[kMaxIcons];						// specifies whether object can be flown over. ex: grass/water
TBoolean						canSwimIn[kMaxIcons];						// specifies whether object is water
TBoolean						showRaft[kMaxIcons];						// specifies whether object is water
UInt16							base[kMaxIcons];									// specifies base graphic of elevated object.  ex: ground under tree
UInt8								mapColor[kMaxIcons];
UInt8								sourceGrid[kMaxIcons];						// specifies which graphic grid to use
TBoolean						doorway[kMaxIcons];
TBoolean						sign[kMaxIcons];
TRect								animationRect[100];
int									animationFrame6;
int									animationFrame2;
unsigned long				animationFrame2Time;
TBoolean            updateMapView=false;

char								chatText[kMaxChatSlots][kStringLength]; //[64];
//unsigned long				chatDecay[kMaxScreenElements];
unsigned long				chatDecay[kMaxChatSlots];
//UInt16							chatReverseLookup[kMaxChatSlots];

unsigned long				flyTime;
int									fliesPtr;
TRect								fliesRect[10];

TRect								bloodTrail[kBloodEnd+1];
TRect								baseHuman[8][3];
TRect								humanMaleGhost[8][2];
TRect								humanFemaleGhost[8][2];
TRect								humanShirt[8];
TRect								humanPants[8][3];
TRect								humanRobe[8][3];
TRect								humanWhiteRobe[8][3];
TRect								humanGMRobe[8][3];
TRect								humanRoyalRobe[8][3];
TRect								humanStaff[8][2];
TRect								humanSickle[8][2];
TRect								humanSword[8][2];
TRect								humanFireSword[8][2];
TRect								humanTrident[8][2];
TRect								humanSmallAxe[8][2];
TRect								humanPickAxe[8][2];
TRect								humanFishingPole[8][2];
TRect								humanBattleAxe[8][2];
TRect								humanClub[8][2];
TRect								humanMace[8][2];
TRect								humanSpect[8][2]; //Dave 11-15-2012
TRect								humanFlail[8][2];
TRect								humanFlail[8][2];
TRect								humanGlaive[8][2];
TRect								humanSpear[8][2];
TRect								humanBow[8][2];
TRect								humanShortSword[8][2];
TRect								humanBreastPlate[8];
TRect								humanBlackBreastPlate[8];
TRect								humanBreastChain[8];
TRect								humanBrownBreastLeather[8];
TRect								humanBreastLeather[8];
TRect								humanLegsPlate[8][3];
TRect								humanBlackLegsPlate[8][3];
TRect								humanMedalShield[8];
TRect								humanBoots[8][3];
TRect								humanArms[8][3];
TRect								humanOpenPlateHelmet[8];
TRect								humanSlottedPlateHelmet[8];
TRect								humanClosedPlateHelmet[8];
TRect								humanVikingHelmet[8];
TRect								humanChainMailHelmet[8];
TRect								humanWoodenShield[8];
TRect								humanLegsChain[8][3];
TRect								humanLegsLeather[8][3];
TRect								humanWizardHat[8];
TRect								humanJesterHat[8];
TRect								humanCrown[8];
TRect								humanMaleHair[8][2];
TRect								humanFemaleHair[8][2];

TRect								humanBlackOpenPlateHelmet[8];
TRect								humanBlackSlottedPlateHelmet[8];
TRect								humanBlackClosedPlateHelmet[8];
TRect								humanBlackVikingHelmet[8];

TRect								humanAFKTag;

TRect								humanGrayArms[8][3];
TRect								humanBlackArms[8][3];
TRect								humanBrownArms[8][3];

TRect								lizard					  [8][3];
TRect								skeleton					[8][3];
TRect								skeletonarcher					[8][3];
TRect								slug							[8][3];
TRect								beholder					[8][3];
TRect								hellHound					[8][3];
TRect								wolf							[8][3];
TRect								rat							  [8][3];
TRect								dragon						[8][3];
TRect								sheep							[8][3];
TRect								scorpion					[8][3];
TRect								harpie						[8][2];
TRect								boneMage					[8][3];
TRect								bat							  [8][3];
TRect								spider						[8][3];
TRect								snail							[8][3];
TRect								ant								[8][3];
TRect								troll							[8][3];
TRect								bear						[8][3];
TRect								giant							[8][3];
TRect								centaur						[8][3];
TRect								serpent						[8][3];
TRect								zombie						[8][3];
TRect								queenant						[8][3];
TRect								snake						[8][3];
TRect								youngdragon					[8][3];
TRect								cube						[8][3];
TRect								blinkhound					[8][3];
TRect								dwarf1						[8][3];
TRect								tusker						[8][3];
TRect								pixie						[8][2];
TRect								ancientdragon				[8][3];
TRect								lich						[8][3];
TRect								lion						[8][3];
TRect								golem						[8][3];
TRect								shark						[8][3];
TRect								wrath						[8][3];


TRect							containerRect[kMaxContainerTypes];
TRect							tracksRect[kMaxTracksTypes];
TBoolean						containerIsCorpse[kMaxContainerTypes];
TBoolean						fliesOnCorpse[kMaxContainerTypes];
TBoolean            containerIsTreasure[kMaxContainerTypes];
TBoolean            gmCanSpawn[kMaxContainerTypes];
TBoolean            gmProp[kMaxContainerTypes];
int 								containerColor[kMaxContainerTypes];

TRect								inventoryRect;
TRect								inventoryScreenRect;

TRect								playRect;
TRect								playScreenRect;
TRect								playOSScreenRect;
TRect								healthBar[kMaxScreenElements];
TRect								secondaryBar[kMaxScreenElements];
TRect								afkTag[kMaxScreenElements];

UInt8								creatureDirection	[kMaxScreenElements];
UInt8								creatureFacing		[kMaxScreenElements];
UInt8								creatureHealth		[kMaxScreenElements];
UInt8								creatureMp			[kMaxScreenElements];
UInt8								creatureAttacking	[kMaxScreenElements];
int								  creatureID	      [kMaxScreenElements];

char								theText[8][kStringLength]; //[128];
TBoolean						theTextActive[8];
unsigned int				theTextDecay[8];
TBoolean						theTextSystem[8];

TBoolean						drawHealthBars;

int									xOffset;
int									yOffset;

int									prevxOff=0;
int									prevyOff=0;
int                 gFirstLine;
int                 firstLine;
int                 lastLine;
int                 currentLine;
int                 currentQuestSlot;

UInt32              cursorBlinkTime;
TBoolean            cursorBlink;

int                 friend1;
int                 friend2;
int                 friend3;
int                 friend4;
TBoolean            friendOnline[16];
TBoolean            friendSharing[16];

short								maxAscent;
short								maxDescent;

TBoolean						standingOnContainer;
TBoolean						standingInShop;
TBoolean						standingInBank;
TBoolean						standingOnActionSpot;
int									actionValue;
int									windowStack[kMaxWindows];

extern UInt8				chessBoard[kMaxChessBoards][96];
extern UInt16				map[kMapWidth][kMapHeight];
extern UInt16				mapToScreenRow[kMaxScreenElements];
extern UInt16				mapToScreenCol[kMaxScreenElements];
extern UInt16				playerRow;
extern UInt16				playerCol;
extern UInt16				previousPlayerRow;
extern UInt16				previousPlayerCol;
extern UInt16				state[kMaxScreenElements];
extern TBoolean			poisoned[kMaxScreenElements];
extern TBoolean			cursed[kMaxScreenElements];
extern TBoolean			isPet[kMaxScreenElements];
extern TBoolean			gmChar[kMaxScreenElements];
extern TBoolean			questChar[kMaxScreenElements];
extern TBoolean     playerOnline[kMaxScreenElements];
extern TBoolean			stealthChar[kMaxScreenElements];
extern TBoolean			afk[kMaxScreenElements];
extern TBoolean				morphed[kMaxScreenElements];
extern TBoolean				regenerating[kMaxScreenElements];
extern UInt16				containerState[kMaxScreenElements];
extern UInt16				tracksState[kMaxScreenElements];
extern UInt16				bloodStainState[kMaxScreenElements];
extern UInt8				human[kMaxScreenElements][19];
extern TBoolean			hudOn;
extern TBoolean			namesOn;
extern inventoryType	bankInventory				[kMaxInventorySize];
extern TBoolean			logText;
extern TBoolean			displayFeedback;

extern Boolean 			gHost;
extern char					buildString[128];

extern int					horizontalOffset;
extern int					verticalOffset;
extern char					playerText[kStringLength]; //[kCommandStringMaxLength];
extern TRect				clickBaseRect[kMaxScreenElements];
extern TRect				clickObjectRect[kMaxScreenElements];
extern TRect				clickCreatureRect[kMaxScreenElements];
//extern RgnHandle		clickCreatureRgn[kMaxScreenElements];
extern TRect				clickTerrainRect[kMaxScreenElements];
extern int					clickTerrainID[kMaxScreenElements];
extern unsigned long	nextRender;
extern long					lastJoin;
extern int					maxDataSize; // beta variable
extern TBoolean			gotFirstRefresh;
extern int					currentInfoDisplay;
extern TRect				inventoryDropRect[kMaxInventorySize];
extern TRect				pickUpRect[kMaxInventorySize];
extern TRect				inventoryGiveRect[kMaxInventorySize];
extern TRect				inventoryUseRect[kMaxInventorySize];
extern TRect				skillDirectionRect[kMaxSkills];
extern TRect				skillRect[kMaxSkills];
extern int					targetMode;
extern UInt8				localStrength;
extern UInt8				localIntelligence;
extern UInt8				localDexterity;
extern UInt8				localArmorRating;
extern UInt8				localHitPoints;
extern UInt8				localMagicPoints;
extern UInt8				localStamina;
extern UInt16				localWeight;
extern UInt8				localClass;
extern UInt8				localLevel;
extern UInt16				localHunger;

extern playerLocalSkillsType				localSkills[kMaxSkills];
extern UInt8				localRightHand;
extern UInt8				localLeftHand;
extern UInt8				localLegs;
extern UInt8				localTorso;
extern UInt8				localFeet;
extern UInt8				localArms;
extern UInt8				localRightFinger;
extern UInt8				localLeftFinger;
extern UInt8				localHead;
extern UInt8				localNeck;
extern UInt8				localHands;
extern UInt8				localWaist;
extern TRegion			directionRgn[8];

extern UInt8				localChess[96];

extern UInt8				localTracking[kMaxTracked][3];
extern unsigned long		currentlyTracking;
extern UInt8				monstersTracked;
extern int					trackingRow;
extern int					trackingCol;

extern itemType			itemList						[kMaxItems];

extern TBoolean				colorOwnName;

extern int					serverFXTime; // beta
extern int					rotateShopTime; // beta
extern int					monsterTime; // beta
extern int					refreshTime; // beta
extern int					decayTime; // beta
extern int					resourceTime; // beta
extern TBoolean			animateStep;
extern creatureType					creature					[kMaxCreatures];
extern creatureControls			creatureInfo			[kMaxCreatureTypes];

extern TBoolean			inventorySlotArmed				[kMaxInventorySize];

extern TRect				displayAreaButton[11];
extern int					displayAreaButtonState[11];
extern int					displayAreaButtonCurrent[11];

extern int					inventoryCheckSumFailures; // beta
extern int					dataReSendError; // beta

extern int					numberOfPlayersOnline;
extern long					numberOfLogons;
extern long					numberOfLogoffs;
extern long					gracefulLogoffs;
extern long					failedLogons;
extern long					serverFull;
extern int          currentNumberOfContainers;
extern int          currentNumberOfTracks;
extern int          ginsengSpawn;
extern int          lotusFlowerSpawn;
extern int          mandrakeRootSpawn;
extern int          garlicSpawn;
extern int          volcanicAshSpawn;
extern int			mushroomSpawn; //Dave
//extern int				eggSpawn; //Dave
extern long					currentCreatures;
extern int					playerAction;
extern char         localContainerName[kStringLength]; //[15];
extern char         playerNames[kMaxPlayers][kStringLength]; //[15];
extern UInt32       playerIDs[kMaxPlayers];

extern TRect				chatUpRect;
extern TRect				chatDownRect;
extern TRect				chatThumbRect;
extern int          startChatLine;
extern TRect				chatPageUpRect;
extern TRect				chatPageDownRect;
extern int          gxoffset;
extern int          gyoffset;

extern int          tbMapWindow;
extern TRect        mapWindowRect;

extern int          tbInventoryWindow;
extern TRect        inventoryWindowRect;

extern int          tbCalendarWindow;
extern TRect        CalendarWindowRect;

extern int          tbPackContentsWindow;
extern TRect        packContentsWindowRect;
extern WindowPtr		packContentsWindow;

extern int          tbShopContentsWindow;
extern TRect        shopContentsWindowRect;
extern WindowPtr		shopContentsWindow;

extern int          tbSellListWindow;
extern TRect        sellListWindowRect;
extern WindowPtr		sellListWindow;

extern WindowPtr		idWindow;
extern TRect        idWindowRect;
extern int          tbIDWindow;

extern WindowPtr		makeWindow;
extern TRect        makeWindowRect;
extern int          tbMakeWindow;

extern WindowPtr		skillsWindow;
extern TRect        skillsWindowRect;
extern int          tbSkillsWindow;

extern WindowPtr		statsWindow;
extern TRect        statsWindowRect;
extern int          tbStatsWindow;

extern WindowPtr		chatWindow;
extern TRect        chatWindowRect;
extern int          tbChatWindow;

extern WindowPtr		characterWindow;
extern TRect        characterWindowRect;
extern int          tbCharacterWindow;

extern WindowPtr		chessWindow;
extern TRect        chessWindowRect;
extern int          tbChessWindow;

extern WindowPtr		mapWindow;
extern TRect        mapWindowRect;
extern int          tbMapWindow;

extern WindowPtr		inventoryWindow;
extern TRect        inventoryWindowRect;
extern int          tbInventoryWindow;

extern WindowPtr		CalendarWindow;
extern TRect        calednarWindowRect;
extern int          tbCalendarWindow;

extern WindowPtr		logonWindow;
extern int          tbLogOnWindow;

extern WindowPtr		bankWindow;
extern TRect        bankWindowRect;
extern int          tbBankWindow;

extern WindowPtr		partyWindow;
extern TRect        partyWindowRect;
extern int          tbPartyWindow;

extern WindowPtr		questWindow;
extern TRect        questWindowRect;
extern int          tbQuestWindow;

extern WindowPtr		petsWindow;
extern TRect        petsWindowRect;
extern int          tbPetsWindow;

extern WindowPtr		aboutWindow;
extern TRect        aboutWindowRect;
extern int          tbAboutWindow;

extern UInt16			currentYear;
extern UInt8			currentCycle;
extern UInt8			currentHour;
extern UInt8			currentMinute;
extern UInt8			currentCalDay;
extern UInt8			cycleDays[kMooncycles];
//extern DialogPtr		chatDialog;


extern int          questGold;
extern int          questExperience;
extern int          questItem;
extern int          questCount;
extern char         questDescription[1024];

extern TBoolean     windowVisible[kMaxWindows];
extern TBoolean     windowActive[kMaxWindows];
extern TPoint       mainWindowPosition;
extern TPoint       windowPosition[kMaxWindows];

extern int          maxSkillLevel[7][kMaxSkills];

//extern UInt8        chatLookUpTable   [kMaxCreatures];
extern char					chatString        [kMaxChatSlots][kStringLength];	// saved
extern UInt16				chatCreatureID    [kMaxChatSlots];	// saved
//extern UInt16				chatReverseLookup	[kMaxChatSlots];

//extern TXNObject   	textObject;               // text object
extern int          insertionPoint;
extern int			fxFrames[kMaxFX];

extern int			localSpat;

#ifdef _SERVERONLY
TRect								nameRect[501];
int									maxMessagesSent;
int									maxMessagesReceived;
int									maxDecayTime;
int									maxResourceTime;
int									maxMonsterTime;
int									maxRotateShopTime;
int									maxRefreshTime;
int									maxServerFXTime;
int									maxBPSSent;
int									maxBPSReceived;
long								minCycles;
unsigned long						totalBytesSent;
int									serverRestarts;
unsigned long						totalBytesReceived;
long								playerDeaths;
long								creatureDeaths;
long								cycles;
#endif

#ifdef _SERVER
extern UInt8						fxMap					[kMapWidth][kMapHeight];
extern UInt8						serverFX				[kMaxFX];
extern unsigned long				serverFXKillTime		[kMaxFX];
extern UInt16						serverFXRow				[kMaxFX];
extern int							serverFXMisc			[kMaxFX];
extern int							serverFXMisc2			[kMaxFX];
extern UInt16						serverFXCol				[kMaxFX];
#endif

// ----------------------------------------------------------------- //
void gr_DrawAllWindows(void)
// ----------------------------------------------------------------- //

{

	gr_RenderMapWindow(true);
	gr_RenderInventoryWindow();
	gr_RenderCalendarWindow();
	gr_RenderPackContentsWindow();
	gr_RenderShopContentsWindow();
	gr_RenderSellListWindow();
	gr_RenderIDDisplayWindow();
	gr_RenderMakeListWindow(currentSkillDisplay,currentCategory);
	gr_RenderSkillsWindow();
	gr_RenderStatsWindow();
	gr_RenderChatDisplayWindow(0);
	gr_RenderCharacterDisplayWindow();
	gr_RenderChessWindow();
	gr_RenderBankWindow();
	gr_RenderPartyWindow();
	gr_RenderQuestWindow();
	gr_RenderPetsWindow();
	gr_RenderAboutWindow();

}

// ----------------------------------------------------------------- //
void gr_HiliteToolboxWindow(int theWindow)
// ----------------------------------------------------------------- //

{

	if (theWindow==tbMapWindow)               { gr_HiliteWindow(kMapWindow);             }

	if (theWindow==tbInventoryWindow)         { gr_HiliteWindow(kInventoryWindow);       }

	if (theWindow==tbCalendarWindow)         { gr_HiliteWindow(kCalendarWindow);       }

	if (theWindow==tbPackContentsWindow)      { gr_HiliteWindow(kPackContentsWindow);    }

	if (theWindow==tbShopContentsWindow)      { gr_HiliteWindow(kShopContentsWindow);    }

	if (theWindow==tbSellListWindow)          { gr_HiliteWindow(kSellListWindow);        }

	if (theWindow==tbIDWindow)                { gr_HiliteWindow(kIDWindow);              }

	if (theWindow==tbMakeWindow)              { gr_HiliteWindow(kMakeWindow);            }

	if (theWindow==tbSkillsWindow)            { gr_HiliteWindow(kSkillsWindow);          }

	if (theWindow==tbStatsWindow)             { gr_HiliteWindow(kStatsWindow);           }

	if (theWindow==tbChatWindow)              { gr_HiliteWindow(kChatWindow);            }

	if (theWindow==tbCharacterWindow)         { gr_HiliteWindow(kCharacterWindow);       }

	if (theWindow==tbChessWindow)             { gr_HiliteWindow(kChessWindow);           }

	if (theWindow==tbBankWindow)             	{ gr_HiliteWindow(kBankWindow);            }

	if (theWindow==tbPartyWindow)             { gr_HiliteWindow(kPartyWindow);           }

	if (theWindow==tbQuestWindow)             { gr_HiliteWindow(kQuestWindow);           }

	if (theWindow==tbPetsWindow)              { gr_HiliteWindow(kPetsWindow);            }

	if (theWindow==tbAboutWindow)             { gr_HiliteWindow(kAboutWindow);           }

}

// ----------------------------------------------------------------- //
void gr_HiliteWindow(int theWindow)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY

	if (windowActive[theWindow])
		return;

	if ((windowActive[kMapWindow]) && (windowVisible[kMapWindow]))                    { windowActive[kMapWindow]=false;           gr_RenderMapWindow(true); }

	if ((windowActive[kInventoryWindow]) && (windowVisible[kInventoryWindow]))        { windowActive[kInventoryWindow]=false;     gr_RenderInventoryWindow();}

	if ((windowActive[kCalendarWindow]) && (windowVisible[kCalendarWindow]))        	{ windowActive[kCalendarWindow]=false;     gr_RenderCalendarWindow();}

	if ((windowActive[kPackContentsWindow]) && (windowVisible[kPackContentsWindow]))  { windowActive[kPackContentsWindow]=false;  gr_RenderPackContentsWindow();}

	if ((windowActive[kShopContentsWindow]) && (windowVisible[kShopContentsWindow]))  { windowActive[kShopContentsWindow]=false;  gr_RenderShopContentsWindow();}

	if ((windowActive[kSellListWindow]) && (windowVisible[kSellListWindow]))          { windowActive[kSellListWindow]=false;      gr_RenderSellListWindow();}

	if ((windowActive[kIDWindow]) && (windowVisible[kIDWindow]))                      { windowActive[kIDWindow]=false;            gr_RenderIDDisplayWindow();}

	if ((windowActive[kMakeWindow]) && (windowVisible[kMakeWindow]))                  { windowActive[kMakeWindow]=false;          gr_RenderMakeListWindow(currentSkillDisplay,currentCategory); }

	if ((windowActive[kSkillsWindow]) && (windowVisible[kSkillsWindow]))              { windowActive[kSkillsWindow]=false;        gr_RenderSkillsWindow();}

	if ((windowActive[kStatsWindow]) && (windowVisible[kStatsWindow]))                { windowActive[kStatsWindow]=false;         gr_RenderStatsWindow();}

	if ((windowActive[kChatWindow]) && (windowVisible[kChatWindow]))                  { windowActive[kChatWindow]=false;          gr_RenderChatDisplayWindow(0);}

	if ((windowActive[kCharacterWindow]) && (windowVisible[kCharacterWindow]))        { windowActive[kCharacterWindow]=false;     gr_RenderCharacterDisplayWindow();}

	if ((windowActive[kChessWindow]) && (windowVisible[kChessWindow]))                { windowActive[kChessWindow]=false;         gr_RenderChessWindow();}

	if ((windowActive[kBankWindow]) && (windowVisible[kBankWindow]))                  { windowActive[kBankWindow]=false;          gr_RenderBankWindow();}

	if ((windowActive[kPartyWindow]) && (windowVisible[kPartyWindow]))                { windowActive[kPartyWindow]=false;         gr_RenderPartyWindow();}

	if ((windowActive[kQuestWindow]) && (windowVisible[kQuestWindow]))                { windowActive[kQuestWindow]=false;         gr_RenderQuestWindow();}

	if ((windowActive[kPetsWindow]) && (windowVisible[kPetsWindow]))                  { windowActive[kPetsWindow]=false;          gr_RenderPetsWindow();}

	if ((windowActive[kAboutWindow]) && (windowVisible[kAboutWindow]))                { windowActive[kAboutWindow]=false;         gr_RenderAboutWindow();}

	windowActive[theWindow]=true;

	gr_AddToWindowStack(theWindow);

	if (theWindow==kMapWindow)               { gr_RenderMapWindow(true); }

	if (theWindow==kInventoryWindow)         { gr_RenderInventoryWindow();}

	if (theWindow==kCalendarWindow)         { gr_RenderCalendarWindow();}

	if (theWindow==kPackContentsWindow)      { gr_RenderPackContentsWindow();}

	if (theWindow==kShopContentsWindow)      { gr_RenderShopContentsWindow();}

	if (theWindow==kSellListWindow)          { gr_RenderSellListWindow();}

	if (theWindow==kIDWindow)                { gr_RenderIDDisplayWindow();}

	if (theWindow==kMakeWindow)              { gr_RenderMakeListWindow(currentSkillDisplay,currentCategory); }

	if (theWindow==kSkillsWindow)            { gr_RenderSkillsWindow();}

	if (theWindow==kStatsWindow)             { gr_RenderStatsWindow();}

	if (theWindow==kChatWindow)              { gr_RenderChatDisplayWindow(0);}

	if (theWindow==kCharacterWindow)         { gr_RenderCharacterDisplayWindow();}

	if (theWindow==kChessWindow)             { gr_RenderChessWindow();}

	if (theWindow==kBankWindow)              { gr_RenderBankWindow();}

	if (theWindow==kPartyWindow)             { gr_RenderPartyWindow();}

	if (theWindow==kQuestWindow)             { gr_RenderQuestWindow();}

	if (theWindow==kPetsWindow)              { gr_RenderPetsWindow();}

	if (theWindow==kAboutWindow)             { gr_RenderAboutWindow();}

	//gr_AddToWindowStack(theWindow);

	gr_SaveWindowPrefs();

#endif
}

// ----------------------------------------------------------------- //
void gr_RenderWindow(int theWindow)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY

	if (theWindow==kMapWindow)               { gr_RenderMapWindow(true); }

	if (theWindow==kInventoryWindow)         { gr_RenderInventoryWindow();}

	if (theWindow==kCalendarWindow)         { gr_RenderCalendarWindow();}

	if (theWindow==kPackContentsWindow)      { gr_RenderPackContentsWindow();}

	if (theWindow==kShopContentsWindow)      { gr_RenderShopContentsWindow();}

	if (theWindow==kSellListWindow)          { gr_RenderSellListWindow();}

	if (theWindow==kIDWindow)                { gr_RenderIDDisplayWindow();}

	if (theWindow==kMakeWindow)              { gr_RenderMakeListWindow(currentSkillDisplay,currentCategory); }

	if (theWindow==kSkillsWindow)            { gr_RenderSkillsWindow();}

	if (theWindow==kStatsWindow)             { gr_RenderStatsWindow();}

	if (theWindow==kChatWindow)              { gr_RenderChatDisplayWindow(0);}

	if (theWindow==kCharacterWindow)         { gr_RenderCharacterDisplayWindow();}

	if (theWindow==kChessWindow)             { gr_RenderChessWindow();}

	if (theWindow==kBankWindow)              { gr_RenderBankWindow();}

	if (theWindow==kPartyWindow)             { gr_RenderPartyWindow();}

	if (theWindow==kQuestWindow)             { gr_RenderQuestWindow();}

	if (theWindow==kPetsWindow)              { gr_RenderPetsWindow();}

	if (theWindow==kAboutWindow)             { gr_RenderAboutWindow();}

#endif
}

// ----------------------------------------------------------------- //
TBoolean gr_PointerOverFloatingWindow(void)
// ----------------------------------------------------------------- //

{
	TRect				r;
	TPoint      mpt;

	tb_GetMousePosition(&mpt);
	LocalToGlobal(&mpt);

	if (windowVisible[kMapWindow])          	{ tb_GetWindowRect(tbMapWindow, &r);          if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kInventoryWindow])      { tb_GetWindowRect(tbInventoryWindow, &r);    if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kCalendarWindow])      { tb_GetWindowRect(tbCalendarWindow, &r);    if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kPackContentsWindow])   { tb_GetWindowRect(tbPackContentsWindow, &r); if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kShopContentsWindow])   { tb_GetWindowRect(tbShopContentsWindow, &r); if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kSellListWindow])       { tb_GetWindowRect(tbSellListWindow, &r);     if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kIDWindow])             { tb_GetWindowRect(tbIDWindow, &r);         	if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kMakeWindow])           { tb_GetWindowRect(tbMakeWindow, &r);         if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kSkillsWindow])         { tb_GetWindowRect(tbSkillsWindow, &r);       if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kStatsWindow])          { tb_GetWindowRect(tbStatsWindow, &r);        if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kChatWindow])           { tb_GetWindowRect(tbChatWindow, &r);         if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kCharacterWindow])      { tb_GetWindowRect(tbCharacterWindow, &r);    if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kChessWindow])          { tb_GetWindowRect(tbChessWindow, &r);        if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kBankWindow])           { tb_GetWindowRect(tbBankWindow, &r);         if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kPartyWindow])          { tb_GetWindowRect(tbPartyWindow, &r);        if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kQuestWindow])          { tb_GetWindowRect(tbQuestWindow, &r);        if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kPetsWindow])           { tb_GetWindowRect(tbPetsWindow, &r);         if (tb_PointInRect(mpt, &r)) return (true);   }

	if (windowVisible[kAboutWindow])          { tb_GetWindowRect(tbAboutWindow, &r);        if (tb_PointInRect(mpt, &r)) return (true);   }

	return (false);

}

// ----------------------------------------------------------------- //
int gr_TopOfWindowStack(void)
// ----------------------------------------------------------------- //

{
	int								i;

	for (i=(kMaxWindows-1);i>=0;i--)
		if (windowStack[i]!=0)
			return (windowStack[i]);

	return (0);

}

// ----------------------------------------------------------------- //
void gr_CompressWindowStack(void)
// ----------------------------------------------------------------- //

{
	int								i;
	int								j;
	int								windowStackBackup[kMaxWindows];

	for (i=0;i<kMaxWindows;i++)
		windowStackBackup[i]=0;

	j=0;

	for (i=0;i<kMaxWindows;i++)
		if (windowStack[i]!=0)
		{
			windowStackBackup[j]=windowStack[i];
			j++;
		}

	for (i=0;i<kMaxWindows;i++)
		windowStack[i]=windowStackBackup[i];

}

// ----------------------------------------------------------------- //
void gr_RemoveFromWindowStack(int theWindow)
// ----------------------------------------------------------------- //

{
	int								i;

	for (i=0;i<kMaxWindows;i++)
		if (windowStack[i]==theWindow)
		{
			windowStack[i]=0;
			break;
		}

	gr_CompressWindowStack();

}

// ----------------------------------------------------------------- //
void gr_AddToWindowStack(int theWindow)
// ----------------------------------------------------------------- //

{
	int								i;

	for (i=0;i<kMaxWindows;i++)
		if (windowStack[i]==theWindow)
		{
			windowStack[i]=0;
			break;
		}

	for (i=(kMaxWindows-1);i>=0;i--)
		if (windowStack[i]==0)
		{
			windowStack[i]=theWindow;
			break;
		}

	gr_CompressWindowStack();

}

// ----------------------------------------------------------------- //
void gr_CloseFloatingWindowID(int theWindow)
// ----------------------------------------------------------------- //

{

	if (theWindow==kMapWindow)               { gr_CloseFloatingWindow(tbMapWindow);           }

	if (theWindow==kInventoryWindow)         { gr_CloseFloatingWindow(tbInventoryWindow);     }

	if (theWindow==kCalendarWindow)         { gr_CloseFloatingWindow(tbCalendarWindow);     }

	if (theWindow==kPackContentsWindow)      { gr_CloseFloatingWindow(tbPackContentsWindow);  }

	if (theWindow==kShopContentsWindow)      { gr_CloseFloatingWindow(tbShopContentsWindow);  }

	if (theWindow==kSellListWindow)          { gr_CloseFloatingWindow(tbSellListWindow);      }

	if (theWindow==kIDWindow)                { gr_CloseFloatingWindow(tbIDWindow);            }

	if (theWindow==kMakeWindow)              { gr_CloseFloatingWindow(tbMakeWindow);          }

	if (theWindow==kSkillsWindow)            { gr_CloseFloatingWindow(tbSkillsWindow);        }

	if (theWindow==kStatsWindow)             { gr_CloseFloatingWindow(tbStatsWindow);         }

	if (theWindow==kChatWindow)              { gr_CloseFloatingWindow(tbChatWindow);          }

	if (theWindow==kCharacterWindow)         { gr_CloseFloatingWindow(tbCharacterWindow);     }

	if (theWindow==kChessWindow)             { gr_CloseFloatingWindow(tbChessWindow);         }

	if (theWindow==kBankWindow)              { gr_CloseFloatingWindow(tbBankWindow);          }

	if (theWindow==kPartyWindow)             { gr_CloseFloatingWindow(tbPartyWindow);         }

	if (theWindow==kQuestWindow)             { gr_CloseFloatingWindow(tbQuestWindow);         }

	if (theWindow==kPetsWindow)              { gr_CloseFloatingWindow(tbPetsWindow);          }

	if (theWindow==kAboutWindow)             { gr_CloseFloatingWindow(tbAboutWindow);         }

}

// ----------------------------------------------------------------- //
void gr_CloseFloatingWindow(int theWindow)
// ----------------------------------------------------------------- //

{
	int					top;

	if (theWindow>0)
	{
		if (theWindow==tbMapWindow)               { gr_RemoveFromWindowStack(kMapWindow);           HideWindow(mapWindow);            windowActive[kMapWindow]=false;           windowVisible[kMapWindow]=false; }

		if (theWindow==tbInventoryWindow)         { gr_RemoveFromWindowStack(kInventoryWindow);     HideWindow(inventoryWindow);      windowActive[kInventoryWindow]=false;     windowVisible[kInventoryWindow]=false; }

		if (theWindow==tbCalendarWindow)         { gr_RemoveFromWindowStack(kCalendarWindow);     HideWindow(CalendarWindow);      windowActive[kCalendarWindow]=false;     windowVisible[kCalendarWindow]=false; }

		if (theWindow==tbPackContentsWindow)      { gr_RemoveFromWindowStack(kPackContentsWindow);  HideWindow(packContentsWindow);   windowActive[kPackContentsWindow]=false;  windowVisible[kPackContentsWindow]=false; in_ClearPickUpPack(); }

		if (theWindow==tbShopContentsWindow)      { gr_RemoveFromWindowStack(kShopContentsWindow);  HideWindow(shopContentsWindow);   windowActive[kShopContentsWindow]=false;  windowVisible[kShopContentsWindow]=false; sh_ClearShopContents();  }

		if (theWindow==tbSellListWindow)          { gr_RemoveFromWindowStack(kSellListWindow);      HideWindow(sellListWindow);       windowActive[kSellListWindow]=false;      windowVisible[kSellListWindow]=false;     sh_ClearSellList(); }

		if (theWindow==tbIDWindow)                { gr_RemoveFromWindowStack(kIDWindow);            HideWindow(idWindow);             windowActive[kIDWindow]=false;            windowVisible[kIDWindow]=false; }

		if (theWindow==tbMakeWindow)              { gr_RemoveFromWindowStack(kMakeWindow);          HideWindow(makeWindow);           windowActive[kMakeWindow]=false;          windowVisible[kMakeWindow]=false; }

		if (theWindow==tbSkillsWindow)            { gr_RemoveFromWindowStack(kSkillsWindow);        HideWindow(skillsWindow);         windowActive[kSkillsWindow]=false;        windowVisible[kSkillsWindow]=false; }

		if (theWindow==tbStatsWindow)             { gr_RemoveFromWindowStack(kStatsWindow);         HideWindow(statsWindow);          windowActive[kStatsWindow]=false;         windowVisible[kStatsWindow]=false; }

		if (theWindow==tbChatWindow)              { gr_RemoveFromWindowStack(kChatWindow);          HideWindow(chatWindow);           windowActive[kChatWindow]=false;          windowVisible[kChatWindow]=false; }

		if (theWindow==tbCharacterWindow)         { gr_RemoveFromWindowStack(kCharacterWindow);     HideWindow(characterWindow);      windowActive[kCharacterWindow]=false;     windowVisible[kCharacterWindow]=false; }

		if (theWindow==tbChessWindow)             { gr_RemoveFromWindowStack(kChessWindow);         HideWindow(chessWindow);          windowActive[kChessWindow]=false;         windowVisible[kChessWindow]=false; }

		if (theWindow==tbBankWindow)             	{ gr_RemoveFromWindowStack(kBankWindow);          HideWindow(bankWindow);           windowActive[kBankWindow]=false;          windowVisible[kBankWindow]=false;         in_ClearBank(); }

		if (theWindow==tbPartyWindow)             { gr_RemoveFromWindowStack(kPartyWindow);         HideWindow(partyWindow);          windowActive[kPartyWindow]=false;         windowVisible[kPartyWindow]=false; }

		if (theWindow==tbQuestWindow)             { gr_RemoveFromWindowStack(kQuestWindow);         HideWindow(questWindow);          windowActive[kQuestWindow]=false;         windowVisible[kQuestWindow]=false; currentQuest=0; }

		if (theWindow==tbPetsWindow)              { gr_RemoveFromWindowStack(kPetsWindow);          HideWindow(petsWindow);           windowActive[kPetsWindow]=false;          windowVisible[kPetsWindow]=false; }

		if (theWindow==tbAboutWindow)             { gr_RemoveFromWindowStack(kAboutWindow);         HideWindow(aboutWindow);          windowActive[kAboutWindow]=false;         windowVisible[kAboutWindow]=false; }

	}

	top=gr_TopOfWindowStack();

	if (top!=0)
	{
		if (top==kMapWindow)               { SelectWindow(mapWindow);          gr_HiliteWindow(kMapWindow); }

		if (top==kInventoryWindow)         { SelectWindow(inventoryWindow);    gr_HiliteWindow(kInventoryWindow); }

		if (top==kCalendarWindow)         { SelectWindow(CalendarWindow);    gr_HiliteWindow(kCalendarWindow); }

		if (top==kPackContentsWindow)      { SelectWindow(packContentsWindow); gr_HiliteWindow(kPackContentsWindow); }

		if (top==kShopContentsWindow)      { SelectWindow(shopContentsWindow); gr_HiliteWindow(kShopContentsWindow); }

		if (top==kSellListWindow)          { SelectWindow(sellListWindow);     gr_HiliteWindow(kSellListWindow); }

		if (top==kIDWindow)                { SelectWindow(idWindow);           gr_HiliteWindow(kIDWindow); }

		if (top==kMakeWindow)              { SelectWindow(makeWindow);         gr_HiliteWindow(kMakeWindow); }

		if (top==kSkillsWindow)            { SelectWindow(skillsWindow);       gr_HiliteWindow(kSkillsWindow); }

		if (top==kStatsWindow)             { SelectWindow(statsWindow);        gr_HiliteWindow(kStatsWindow); }

		if (top==kChatWindow)              { SelectWindow(chatWindow);         gr_HiliteWindow(kChatWindow); }

		if (top==kCharacterWindow)         { SelectWindow(characterWindow);    gr_HiliteWindow(kCharacterWindow); }

		if (top==kChessWindow)             { SelectWindow(chessWindow);        gr_HiliteWindow(kChessWindow); }

		if (top==kBankWindow)              { SelectWindow(bankWindow);         gr_HiliteWindow(kBankWindow); }

		if (top==kPartyWindow)             { SelectWindow(partyWindow);        gr_HiliteWindow(kPartyWindow); }

		if (top==kQuestWindow)             { SelectWindow(questWindow);        gr_HiliteWindow(kQuestWindow); }

		if (top==kPetsWindow)              { SelectWindow(petsWindow);         gr_HiliteWindow(kPetsWindow); }

		if (top==kAboutWindow)             { SelectWindow(aboutWindow);        gr_HiliteWindow(kAboutWindow); }

		UpdateEvent(top);
	}

	gr_SaveWindowPrefs();

}

// ----------------------------------------------------------------- //
void gr_SaveWindowPrefs(void)
// ----------------------------------------------------------------- //

{
	TRect				r;

	tb_GetWindowRect(tbWindow, &r);
	mainWindowPosition.h=r.left;
	mainWindowPosition.v=r.top;

	if (windowVisible[kMapWindow])          	{ tb_GetWindowRect(tbMapWindow, &r);         windowPosition[kMapWindow].h=r.left;  windowPosition[kMapWindow].v=r.top;   }

	if (windowVisible[kInventoryWindow])      { tb_GetWindowRect(tbInventoryWindow, &r);   windowPosition[kInventoryWindow].h=r.left;  windowPosition[kInventoryWindow].v=r.top;   }

	if (windowVisible[kCalendarWindow])      { tb_GetWindowRect(tbCalendarWindow, &r);   windowPosition[kCalendarWindow].h=r.left;  windowPosition[kCalendarWindow].v=r.top;   }

	if (windowVisible[kPackContentsWindow])   { tb_GetWindowRect(tbPackContentsWindow, &r);windowPosition[kPackContentsWindow].h=r.left;  windowPosition[kPackContentsWindow].v=r.top;   }

	if (windowVisible[kShopContentsWindow])   { tb_GetWindowRect(tbShopContentsWindow, &r);windowPosition[kShopContentsWindow].h=r.left;  windowPosition[kShopContentsWindow].v=r.top;   }

	if (windowVisible[kSellListWindow])       { tb_GetWindowRect(tbSellListWindow, &r);    windowPosition[kSellListWindow].h=r.left;  windowPosition[kSellListWindow].v=r.top;   }

	if (windowVisible[kIDWindow])             { tb_GetWindowRect(tbIDWindow, &r);         	windowPosition[kIDWindow].h=r.left;  windowPosition[kIDWindow].v=r.top;   }

	if (windowVisible[kMakeWindow])           { tb_GetWindowRect(tbMakeWindow, &r);         windowPosition[kMakeWindow].h=r.left;  windowPosition[kMakeWindow].v=r.top;   }

	if (windowVisible[kSkillsWindow])         { tb_GetWindowRect(tbSkillsWindow, &r);       windowPosition[kSkillsWindow].h=r.left;  windowPosition[kSkillsWindow].v=r.top;   }

	if (windowVisible[kStatsWindow])          { tb_GetWindowRect(tbStatsWindow, &r);        windowPosition[kStatsWindow].h=r.left;  windowPosition[kStatsWindow].v=r.top;   }

	if (windowVisible[kChatWindow])           { tb_GetWindowRect(tbChatWindow, &r);         windowPosition[kChatWindow].h=r.left;  windowPosition[kChatWindow].v=r.top;   }

	if (windowVisible[kCharacterWindow])      { tb_GetWindowRect(tbCharacterWindow, &r);    windowPosition[kCharacterWindow].h=r.left;  windowPosition[kCharacterWindow].v=r.top;   }

	if (windowVisible[kChessWindow])          { tb_GetWindowRect(tbChessWindow, &r);        windowPosition[kChessWindow].h=r.left;  windowPosition[kChessWindow].v=r.top;   }

	if (windowVisible[kBankWindow])           { tb_GetWindowRect(tbBankWindow, &r);         windowPosition[kBankWindow].h=r.left;  windowPosition[kBankWindow].v=r.top;   }

	if (windowVisible[kPartyWindow])          { tb_GetWindowRect(tbPartyWindow, &r);        windowPosition[kPartyWindow].h=r.left;  windowPosition[kPartyWindow].v=r.top;   }

	if (windowVisible[kQuestWindow])          { tb_GetWindowRect(tbQuestWindow, &r);        windowPosition[kQuestWindow].h=r.left;  windowPosition[kQuestWindow].v=r.top;   }

	if (windowVisible[kPetsWindow])           { tb_GetWindowRect(tbPetsWindow, &r);         windowPosition[kPetsWindow].h=r.left;  windowPosition[kPetsWindow].v=r.top;   }

	if (windowVisible[kAboutWindow])          { tb_GetWindowRect(tbAboutWindow, &r);        windowPosition[kAboutWindow].h=r.left;  windowPosition[kAboutWindow].v=r.top;   }

	cm_WritePrefs();

}

// ----------------------------------------------------------------- //
void gr_Resume(void)
// ----------------------------------------------------------------- //

{
	int               top;  // mod 8/1
	//RgnHandle 				updateRgn;


	if (onLine)
	{
		ShowWindow(Window);
		SelectWindow(Window);
		HiliteWindow(Window,true);

		if (windowVisible[kMapWindow])          	{ ShowWindow(mapWindow);             }

		if (windowVisible[kInventoryWindow])      { ShowWindow(inventoryWindow);       }

		if (windowVisible[kCalendarWindow])      { ShowWindow(CalendarWindow);       }

		if (windowVisible[kPackContentsWindow])   { ShowWindow(packContentsWindow);    }

		if (windowVisible[kShopContentsWindow])   { ShowWindow(shopContentsWindow);    }

		if (windowVisible[kSellListWindow])       { ShowWindow(sellListWindow);        }

		if (windowVisible[kIDWindow])             { ShowWindow(idWindow);              }

		if (windowVisible[kMakeWindow])           { ShowWindow(makeWindow);            }

		if (windowVisible[kSkillsWindow])         { ShowWindow(skillsWindow);          }

		if (windowVisible[kStatsWindow])          { ShowWindow(statsWindow);           }

		if (windowVisible[kChatWindow])           { ShowWindow(chatWindow);            }

		if (windowVisible[kCharacterWindow])      { ShowWindow(characterWindow);       }

		if (windowVisible[kChessWindow])          { ShowWindow(chessWindow);           }

		if (windowVisible[kBankWindow])           { ShowWindow(bankWindow);            }

		if (windowVisible[kPartyWindow])          { ShowWindow(partyWindow);           }

		if (windowVisible[kQuestWindow])          { ShowWindow(questWindow);           }

		if (windowVisible[kPetsWindow])           { ShowWindow(petsWindow);            }

		if (windowVisible[kAboutWindow])          { ShowWindow(aboutWindow);            }

		top=gr_TopOfWindowStack();

		if (top>0)
		{
			gr_OpenFloatingWindowID(top);
			// gr_OpenFloatingWindow(top);
			//   						UpdateEvent(top);
			//if (top==tbMapWindow)               { CollapseWindow(mapWindow,true);  CollapseWindow(mapWindow,false); }
			//if (top==tbMapWindow)
			//{
			//GetWindowRegion(mapWindow,kWindowStructureRgn,updateRgn);
			//InvalWindowRgn(mapWindow,updateRgn);
			//}

			/*
			if (top==tbMapWindow)               { GetWindowRegion(mapWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(mapWindow,updateRgn); }
			if (top==tbInventoryWindow)         { GetWindowRegion(inventoryWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(inventoryWindow,updateRgn); }
			if (top==tbPackContentsWindow)      { GetWindowRegion(packContentsWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(packContentsWindow,updateRgn); }
			if (top==tbShopContentsWindow)      { GetWindowRegion(shopContentsWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(shopContentsWindow,updateRgn); }
			if (top==tbSellListWindow)          { GetWindowRegion(sellListWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(sellListWindow,updateRgn); }
			if (top==tbIDWindow)                { GetWindowRegion(idWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(idWindow,updateRgn); }
			if (top==tbMakeWindow)              { GetWindowRegion(makeWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(makeWindow,updateRgn); }
			if (top==tbSkillsWindow)            { GetWindowRegion(skillsWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(skillsWindow,updateRgn); }
			if (top==tbStatsWindow)             { GetWindowRegion(statsWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(statsWindow,updateRgn); }
			if (top==tbChatWindow)              { GetWindowRegion(chatWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(chatWindow,updateRgn); }
			if (top==tbCharacterWindow)         { GetWindowRegion(characterWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(characterWindow,updateRgn); }
			if (top==tbChessWindow)             { GetWindowRegion(chessWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(chessWindow,updateRgn); }
			if (top==tbBankWindow)             	{ GetWindowRegion(bankWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(bankWindow,updateRgn); }
			if (top==tbPartyWindow)             { GetWindowRegion(partyWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(partyWindow,updateRgn); }
			if (top==tbQuestWindow)             { GetWindowRegion(questWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(questWindow,updateRgn); }
			if (top==tbPetsWindow)              { GetWindowRegion(petsWindow,kWindowStructureRgn,updateRgn);  InvalWindowRgn(petsWindow,updateRgn); }
			*/
		}

	}
	else
	{
		ShowWindow(logonWindow);
		SelectWindow(logonWindow);
		HiliteWindow(logonWindow,true);
	}


}

// ----------------------------------------------------------------- //
void gr_OpenFloatingWindowID(int theWindow)
// ----------------------------------------------------------------- //

{

	if (theWindow==kMapWindow)               { ShowWindow(mapWindow);            SelectWindow(mapWindow);          windowActive[kMapWindow]=false;           gr_HiliteWindow(kMapWindow);           windowVisible[kMapWindow]=true; }

	if (theWindow==kInventoryWindow)         { ShowWindow(inventoryWindow);      SelectWindow(inventoryWindow);    windowActive[kInventoryWindow]=false;     gr_HiliteWindow(kInventoryWindow);     windowVisible[kInventoryWindow]=true; }

	if (theWindow==kCalendarWindow)         { ShowWindow(CalendarWindow);      SelectWindow(CalendarWindow);    windowActive[kCalendarWindow]=false;     gr_HiliteWindow(kCalendarWindow);     windowVisible[kCalendarWindow]=true; }

	if (theWindow==kPackContentsWindow)      { ShowWindow(packContentsWindow);   SelectWindow(packContentsWindow); windowActive[kPackContentsWindow]=false;  gr_HiliteWindow(kPackContentsWindow);  windowVisible[kPackContentsWindow]=true; }

	if (theWindow==kShopContentsWindow)      { ShowWindow(shopContentsWindow);   SelectWindow(shopContentsWindow); windowActive[kShopContentsWindow]=false;  gr_HiliteWindow(kShopContentsWindow);  windowVisible[kShopContentsWindow]=true; }

	if (theWindow==kSellListWindow)          { ShowWindow(sellListWindow);       SelectWindow(sellListWindow);     windowActive[kSellListWindow]=false;      gr_HiliteWindow(kSellListWindow);      windowVisible[kSellListWindow]=true; }

	if (theWindow==kIDWindow)                { ShowWindow(idWindow);             SelectWindow(idWindow);           windowActive[kIDWindow]=false;            gr_HiliteWindow(kIDWindow);            windowVisible[kIDWindow]=true; }

	if (theWindow==kMakeWindow)              { ShowWindow(makeWindow);           SelectWindow(makeWindow);         windowActive[kMakeWindow]=false;          gr_HiliteWindow(kMakeWindow);          windowVisible[kMakeWindow]=true; }

	if (theWindow==kSkillsWindow)            { ShowWindow(skillsWindow);         SelectWindow(skillsWindow);       windowActive[kSkillsWindow]=false;        gr_HiliteWindow(kSkillsWindow);        windowVisible[kSkillsWindow]=true; }

	if (theWindow==kStatsWindow)             { ShowWindow(statsWindow);          SelectWindow(statsWindow);        windowActive[kStatsWindow]=false;         gr_HiliteWindow(kStatsWindow);         windowVisible[kStatsWindow]=true; }

	if (theWindow==kChatWindow)              { ShowWindow(chatWindow);           SelectWindow(chatWindow);         windowActive[kChatWindow]=false;          gr_HiliteWindow(kChatWindow);          windowVisible[kChatWindow]=true; }

	if (theWindow==kCharacterWindow)         { ShowWindow(characterWindow);      SelectWindow(characterWindow);    windowActive[kCharacterWindow]=false;     gr_HiliteWindow(kCharacterWindow);     windowVisible[kCharacterWindow]=true; }

	if (theWindow==kChessWindow)             { ShowWindow(chessWindow);          SelectWindow(chessWindow);        windowActive[kChessWindow]=false;         gr_HiliteWindow(kChessWindow);         windowVisible[kChessWindow]=true; }

	if (theWindow==kBankWindow)              { ShowWindow(bankWindow);           SelectWindow(bankWindow);         windowActive[kBankWindow]=false;          gr_HiliteWindow(kBankWindow);          windowVisible[kBankWindow]=true; }

	if (theWindow==kPartyWindow)             { ShowWindow(partyWindow);          SelectWindow(partyWindow);        windowActive[kPartyWindow]=false;         gr_HiliteWindow(kPartyWindow);         windowVisible[kPartyWindow]=true; }

	if (theWindow==kQuestWindow)             { ShowWindow(questWindow);          SelectWindow(questWindow);        windowActive[kQuestWindow]=false;         gr_HiliteWindow(kQuestWindow);         windowVisible[kQuestWindow]=true; }

	if (theWindow==kPetsWindow)              { ShowWindow(petsWindow);           SelectWindow(petsWindow);         windowActive[kPetsWindow]=false;          gr_HiliteWindow(kPetsWindow);          windowVisible[kPetsWindow]=true; }

	if (theWindow==kAboutWindow)             { ShowWindow(aboutWindow);          SelectWindow(aboutWindow);        windowActive[kAboutWindow]=false;         gr_HiliteWindow(kAboutWindow);         windowVisible[kAboutWindow]=true; }

// if (theWindow==kChatWindow)              { ShowWindow(GetDialogWindow(chatDialog));   SelectWindow(GetDialogWindow(chatDialog));    }

	gr_AddToWindowStack(theWindow);

	gr_SaveWindowPrefs();

	/*
	  if (theWindow==kMapWindow)               { gr_AddToWindowStack(tbMapWindow);          ShowWindow(mapWindow);            SelectWindow(mapWindow);          gr_HiliteWindow(kMapWindow);            windowVisible[kMapWindow]=true;           gr_RenderMapWindow(true); }
	  if (theWindow==kInventoryWindow)         { gr_AddToWindowStack(tbInventoryWindow);    ShowWindow(inventoryWindow);      SelectWindow(inventoryWindow);    gr_HiliteWindow(kInventoryWindow);      windowVisible[kInventoryWindow]=true;     gr_RenderInventoryWindow();}
	  if (theWindow==kPackContentsWindow)      { gr_AddToWindowStack(tbPackContentsWindow); ShowWindow(packContentsWindow);   SelectWindow(packContentsWindow); gr_HiliteWindow(kPackContentsWindow);   windowVisible[kPackContentsWindow]=true;  gr_RenderPackContentsWindow();}
	  if (theWindow==kShopContentsWindow)      { gr_AddToWindowStack(tbShopContentsWindow); ShowWindow(shopContentsWindow);   SelectWindow(shopContentsWindow); gr_HiliteWindow(kShopContentsWindow);   windowVisible[kShopContentsWindow]=true;  gr_RenderShopContentsWindow();}
	  if (theWindow==kSellListWindow)          { gr_AddToWindowStack(tbSellListWindow);     ShowWindow(sellListWindow);       SelectWindow(sellListWindow);     gr_HiliteWindow(kSellListWindow);       windowVisible[kSellListWindow]=true;      gr_RenderSellListWindow();}
	  if (theWindow==kIDWindow)                { gr_AddToWindowStack(tbIDWindow);           ShowWindow(idWindow);             SelectWindow(idWindow);           gr_HiliteWindow(kIDWindow);             windowVisible[kIDWindow]=true;            gr_RenderIDDisplayWindow();}
	  if (theWindow==kMakeWindow)              { gr_AddToWindowStack(tbMakeWindow);         ShowWindow(makeWindow);           SelectWindow(makeWindow);         gr_HiliteWindow(kMakeWindow);           windowVisible[kMakeWindow]=true;          gr_RenderMakeListWindow(currentSkillDisplay,currentCategory); }
	  if (theWindow==kSkillsWindow)            { gr_AddToWindowStack(tbSkillsWindow);       ShowWindow(skillsWindow);         SelectWindow(skillsWindow);       gr_HiliteWindow(kSkillsWindow);         windowVisible[kSkillsWindow]=true;        gr_RenderSkillsWindow();}
	  if (theWindow==kStatsWindow)             { gr_AddToWindowStack(tbStatsWindow);        ShowWindow(statsWindow);          SelectWindow(statsWindow);        gr_HiliteWindow(kStatsWindow);          windowVisible[kStatsWindow]=true;         gr_RenderStatsWindow();}
	  if (theWindow==kChatWindow)              { gr_AddToWindowStack(tbChatWindow);         ShowWindow(chatWindow);           SelectWindow(chatWindow);         gr_HiliteWindow(kChatWindow);           windowVisible[kChatWindow]=true;          gr_RenderChatDisplayWindow(0);}
	  if (theWindow==kCharacterWindow)         { gr_AddToWindowStack(tbCharacterWindow);    ShowWindow(characterWindow);      SelectWindow(characterWindow);    gr_HiliteWindow(kCharacterWindow);      windowVisible[kCharacterWindow]=true;     gr_RenderCharacterDisplayWindow();}
	  if (theWindow==kChessWindow)             { gr_AddToWindowStack(tbChessWindow);        ShowWindow(chessWindow);          SelectWindow(chessWindow);        gr_HiliteWindow(kChessWindow);          windowVisible[kChessWindow]=true;         gr_RenderChessWindow();}
	  if (theWindow==kBankWindow)              { gr_AddToWindowStack(tbBankWindow);         ShowWindow(bankWindow);           SelectWindow(bankWindow);         gr_HiliteWindow(kBankWindow);           windowVisible[kBankWindow]=true;          gr_RenderBankWindow();}
	  if (theWindow==kPartyWindow)             { gr_AddToWindowStack(tbPartyWindow);        ShowWindow(partyWindow);          SelectWindow(partyWindow);        gr_HiliteWindow(kPartyWindow);          windowVisible[kPartyWindow]=true;         gr_RenderPartyWindow();}
	  if (theWindow==kQuestWindow)             { gr_AddToWindowStack(tbQuestWindow);        ShowWindow(questWindow);          SelectWindow(questWindow);        gr_HiliteWindow(kQuestWindow);          windowVisible[kQuestWindow]=true;         gr_RenderQuestWindow();}
	  if (theWindow==kPetsWindow)              { gr_AddToWindowStack(tbPetsWindow);         ShowWindow(petsWindow);           SelectWindow(petsWindow);         gr_HiliteWindow(kPetsWindow);           windowVisible[kPetsWindow]=true;          gr_RenderPetsWindow();}

		//gr_AddToWindowStack(theWindow);

		gr_SaveWindowPrefs();
	*/
}

// ----------------------------------------------------------------- //
void gr_OpenFloatingWindow(int theWindow)
// ----------------------------------------------------------------- //

{

	if (theWindow==tbMapWindow)               { gr_AddToWindowStack(kMapWindow);          ShowWindow(mapWindow);            SelectWindow(mapWindow);          windowActive[kMapWindow]=false;           gr_HiliteWindow(kMapWindow); }

	if (theWindow==tbInventoryWindow)         { gr_AddToWindowStack(kInventoryWindow);    ShowWindow(inventoryWindow);      SelectWindow(inventoryWindow);    windowActive[kInventoryWindow]=false;     gr_HiliteWindow(kInventoryWindow); }

	if (theWindow==tbCalendarWindow)         { gr_AddToWindowStack(kCalendarWindow);    ShowWindow(CalendarWindow);      SelectWindow(CalendarWindow);    windowActive[kCalendarWindow]=false;     gr_HiliteWindow(kCalendarWindow); }

	if (theWindow==tbPackContentsWindow)      { gr_AddToWindowStack(kPackContentsWindow); ShowWindow(packContentsWindow);   SelectWindow(packContentsWindow); windowActive[kPackContentsWindow]=false;  gr_HiliteWindow(kPackContentsWindow); }

	if (theWindow==tbShopContentsWindow)      { gr_AddToWindowStack(kShopContentsWindow); ShowWindow(shopContentsWindow);   SelectWindow(shopContentsWindow); windowActive[kShopContentsWindow]=false;  gr_HiliteWindow(kShopContentsWindow); }

	if (theWindow==tbSellListWindow)          { gr_AddToWindowStack(kSellListWindow);     ShowWindow(sellListWindow);       SelectWindow(sellListWindow);     windowActive[kSellListWindow]=false;      gr_HiliteWindow(kSellListWindow); }

	if (theWindow==tbIDWindow)                { gr_AddToWindowStack(kIDWindow);           ShowWindow(idWindow);             SelectWindow(idWindow);           windowActive[kIDWindow]=false;            gr_HiliteWindow(kIDWindow); }

	if (theWindow==tbMakeWindow)              { gr_AddToWindowStack(kMakeWindow);         ShowWindow(makeWindow);           SelectWindow(makeWindow);         windowActive[kMakeWindow]=false;          gr_HiliteWindow(kMakeWindow); }

	if (theWindow==tbSkillsWindow)            { gr_AddToWindowStack(kSkillsWindow);       ShowWindow(skillsWindow);         SelectWindow(skillsWindow);       windowActive[kSkillsWindow]=false;        gr_HiliteWindow(kSkillsWindow); }

	if (theWindow==tbStatsWindow)             { gr_AddToWindowStack(kStatsWindow);        ShowWindow(statsWindow);          SelectWindow(statsWindow);        windowActive[kStatsWindow]=false;         gr_HiliteWindow(kStatsWindow); }

	if (theWindow==tbChatWindow)              { gr_AddToWindowStack(kChatWindow);         ShowWindow(chatWindow);           SelectWindow(chatWindow);         windowActive[kChatWindow]=false;          gr_HiliteWindow(kChatWindow); }

	if (theWindow==tbCharacterWindow)         { gr_AddToWindowStack(kCharacterWindow);    ShowWindow(characterWindow);      SelectWindow(characterWindow);    windowActive[kCharacterWindow]=false;     gr_HiliteWindow(kCharacterWindow); }

	if (theWindow==tbChessWindow)             { gr_AddToWindowStack(kChessWindow);        ShowWindow(chessWindow);          SelectWindow(chessWindow);        windowActive[kChessWindow]=false;         gr_HiliteWindow(kChessWindow); }

	if (theWindow==tbBankWindow)             	{ gr_AddToWindowStack(kBankWindow);         ShowWindow(bankWindow);          	SelectWindow(bankWindow);        	windowActive[kBankWindow]=false;          gr_HiliteWindow(kBankWindow); }

	if (theWindow==tbPartyWindow)             { gr_AddToWindowStack(kPartyWindow);        ShowWindow(partyWindow);          SelectWindow(partyWindow);        windowActive[kPartyWindow]=false;         gr_HiliteWindow(kPartyWindow); }

	if (theWindow==tbQuestWindow)             { gr_AddToWindowStack(kQuestWindow);        ShowWindow(questWindow);          SelectWindow(questWindow);        windowActive[kQuestWindow]=false;         gr_HiliteWindow(kQuestWindow); }

	if (theWindow==tbPetsWindow)              { gr_AddToWindowStack(kPetsWindow);         ShowWindow(petsWindow);           SelectWindow(petsWindow);         windowActive[kPetsWindow]=false;          gr_HiliteWindow(kPetsWindow); }

	if (theWindow==tbAboutWindow)             { gr_AddToWindowStack(kAboutWindow);        ShowWindow(aboutWindow);          SelectWindow(aboutWindow);        windowActive[kAboutWindow]=false;         gr_HiliteWindow(kAboutWindow); }

// if (theWindow==tbChatWindow)              { ShowWindow(GetDialogWindow(chatDialog));   SelectWindow(GetDialogWindow(chatDialog)); }

	gr_SaveWindowPrefs();
}

// ----------------------------------------------------------------- //
void gr_TempHideAllWindows(void)
// ----------------------------------------------------------------- //

{

	HideWindow(Window);
	HideWindow(logonWindow);
	HideWindow(mapWindow);
	HideWindow(inventoryWindow);
	HideWindow(CalendarWindow);
	HideWindow(packContentsWindow);
	HideWindow(shopContentsWindow);
	HideWindow(sellListWindow);
	HideWindow(idWindow);
	HideWindow(makeWindow);
	HideWindow(skillsWindow);
	HideWindow(statsWindow);
	HideWindow(chatWindow);
	HideWindow(characterWindow);
	HideWindow(chessWindow);
	HideWindow(bankWindow);
	HideWindow(partyWindow);
	HideWindow(questWindow);
	HideWindow(petsWindow);
	//HideWindow(aboutWindow);

	if (windowVisible[kAboutWindow])
		gr_HiliteWindow(kAboutWindow);

}

// ----------------------------------------------------------------- //
void gr_HideAllWindows(void)
// ----------------------------------------------------------------- //

{
	int             i;

	HideWindow(Window);
	HideWindow(logonWindow);
	HideWindow(mapWindow);
	HideWindow(inventoryWindow);
	HideWindow(CalendarWindow);
	HideWindow(packContentsWindow);
	HideWindow(shopContentsWindow);
	HideWindow(sellListWindow);
	HideWindow(idWindow);
	HideWindow(makeWindow);
	HideWindow(skillsWindow);
	HideWindow(statsWindow);
	HideWindow(chatWindow);
	HideWindow(characterWindow);
	HideWindow(chessWindow);
	HideWindow(bankWindow);
	HideWindow(partyWindow);
	HideWindow(questWindow);
	HideWindow(petsWindow);
	HideWindow(aboutWindow);

	for (i=0;i<kMaxWindows;i++)
	{
		windowVisible[i]=false;
		windowStack[i]=0;
	}

	gr_SaveWindowPrefs();
}


#ifdef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_DrawServerNameAt(int i) // server only
// ----------------------------------------------------------------- //

{
	// merge 7/28 entire function
	TRect									r;
	char									theText[kStringLength]; //[48];

	if ((i<1) || (i>100))
		return;

	tb_SetForeColor(tbWindow, 65535, 65535, 65535);

	tb_PaintRect(tbWindow,&nameRect[i]);

	if (player[i].id==0)
		return;

	if (player[i].online)
	{
		if (player[i].category!=0 && player[i].category!=8)
			tb_SetForeColor(tbWindow, 0, 65535, 0);
		else if (player[i].dead)
			tb_SetForeColor(tbWindow, 16384, 16384, 16384);
		else if (creature[player[i].creatureIndex].attackTarget!=0)
			tb_SetForeColor(tbWindow, 65535, 0, 65535);
		else
			tb_SetForeColor(tbWindow, 0, 0, 65535);
	}
	else
		tb_SetForeColor(tbWindow, 65535, 0, 0);

	tb_SetFont(tbWindow, kFontIDGeneva);

	tb_SetFontSize(tbWindow, 9);

	tb_SetFontStyle(tbWindow, tb_Plain);

	tb_SetClip(tbWindow, &nameRect[i]);

	if ((player[i].category==1) || (player[i].category==2) || (player[i].category==6) || (player[i].category==7)) // merge 7/27
		strcpy(theText, "(GM) ");
	else if ((player[i].category==4) || (player[i].category==5)) // merge 7/27
		strcpy(theText, "(Q) ");
	else
		strcpy(theText,"");

	if ((player[i].category==4) || (player[i].category==5)) // merge 7/27
		strcat(theText,player[i].userName);
	else
		strcat(theText,player[i].playerName);

	tb_DrawString(tbWindow, nameRect[i].left+1, nameRect[i].bottom-2, theText);

//	tb_DrawString(tbWindow, nameRect[i].left+1, nameRect[i].bottom-2, "Test Nameypj");

	r=screenBounds;

	tb_SetClip(tbWindow, &r);

}

#endif

// ----------------------------------------------------------------- //
void gr_ShowDebugLocation(int /*d*/) // server only
// ----------------------------------------------------------------- //

{
	//TRect       r;

	return;

	//tb_SetFont(tbWindow, kFontIDGeneva);
	//tb_SetFontSize(tbWindow, 9);
	//tb_SetFontStyle(tbWindow, tb_Plain);
	//tb_SetForeColor(tbWindow, 65535, 65535, 65535);
	//tb_SetRect(&r,980,30,1010,45);
	//tb_PaintRect(tbWindow,&r);
	//tb_SetForeColor(tbWindow, 0, 0, 0);
	//tb_FrameRect(tbWindow,&r);
	//tb_DrawNumber(tbWindow, 985, 41, d, tb_Left);

}

#ifdef _SERVERONLY

// ----------------------------------------------------------------- //
void gr_ShowServerStats(void) // server only
// ----------------------------------------------------------------- //

{
// merge 7/28 entire function
	TRect									r;
	//char									theText[kStringLength]; //[48];
	int										needed;
	int										i;
	int										vo=-20;

//return;

	tb_SetForeColor(tbWindow, 65535, 65535, 65535);
	tb_SetRect(&r,90,49,165,124);
	tb_PaintRect(tbWindow,&r);

	tb_SetFont(tbWindow, kFontIDGeneva);
	tb_SetFontSize(tbWindow, 9);
	tb_SetFontStyle(tbWindow, tb_Plain);

	if (numberOfPlayersOnline>maxPlayersOnline)
		maxPlayersOnline=numberOfPlayersOnline;

	if (bytesSent>maxBPSSent)
		maxBPSSent=bytesSent;

	if (bytesReceived>maxBPSReceived)
		maxBPSReceived=bytesReceived;

	tb_SetForeColor(tbWindow, 0, 0, 0);

	tb_DrawNumber(tbWindow, 158, 61+(12*0), numberOfPlayersOnline, tb_Right);

	tb_DrawNumber(tbWindow, 158, 61+(12*1), maxPlayersOnline, tb_Right);

	tb_DrawNumber(tbWindow, 158, 61+(12*2), bytesSent, tb_Right);

	tb_DrawNumber(tbWindow, 158, 61+(12*3), maxBPSSent, tb_Right);

	tb_DrawNumber(tbWindow, 158, 61+(12*4), bytesReceived, tb_Right);

	tb_DrawNumber(tbWindow, 158, 61+(12*5), maxBPSReceived, tb_Right);

	return;

	tb_SetForeColor(tbWindow, 65535, 65535, 65535);

	tb_SetRect(&r,4,63+vo,161,110+vo);

	tb_PaintRect(tbWindow,&r);

	tb_SetRect(&r,260-2,22+vo,347-4,110+vo);

	tb_PaintRect(tbWindow,&r);

	tb_SetRect(&r,436-4,22+vo,645-9,110+vo);

	tb_PaintRect(tbWindow,&r);

	tb_SetRect(&r,736-6,22+vo,855-8,110+vo);

	tb_PaintRect(tbWindow,&r);

	tb_SetRect(&r,937-8,22+vo,1010,110+vo);

	tb_PaintRect(tbWindow,&r);

	tb_SetForeColor(tbWindow, 0, 0, 0);

	tb_SetFont(tbWindow, kFontIDGeneva);

	tb_SetFontSize(tbWindow, 48);

	tb_SetFontStyle(tbWindow, tb_Bold);

	tb_DrawNumber(tbWindow, 82, 106+vo, numberOfPlayersOnline, tb_Center);

	tb_SetFontSize(tbWindow, 9);

	tb_SetFontStyle(tbWindow, tb_Plain);

	if (numberOfPlayersOnline>maxPlayersOnline)
		maxPlayersOnline=numberOfPlayersOnline;

	if (messagesSent>maxMessagesSent)
		maxMessagesSent=messagesSent;

	if (messagesReceived>maxMessagesReceived)
		maxMessagesReceived=messagesReceived;

	if (decayTime>maxDecayTime)
		maxDecayTime=decayTime;

	if (resourceTime>maxResourceTime)
		maxResourceTime=resourceTime;

	if (monsterTime>maxMonsterTime)
		maxMonsterTime=monsterTime;

	if (rotateShopTime>maxRotateShopTime)
		maxRotateShopTime=rotateShopTime;

	if (refreshTime>maxRefreshTime)
		maxRefreshTime=refreshTime;

	if (serverFXTime>maxServerFXTime)
		maxServerFXTime=serverFXTime;

	if (bytesSent>maxBPSSent)
		maxBPSSent=bytesSent;

	if (bytesReceived>maxBPSReceived)
		maxBPSReceived=bytesReceived;

	if (cycles<minCycles)
		minCycles=cycles;

	totalBytesSent=totalBytesSent+bytesSent;

	totalBytesReceived=totalBytesReceived+bytesReceived;


	tb_DrawNumber(tbWindow, 265-2, 40+vo, maxPlayersOnline, tb_Left);

	tb_DrawNumber(tbWindow, 265-2, 53+vo, failedLogons, tb_Left);	// avg users online

	tb_DrawNumber(tbWindow, 265-2, 66+vo, numberOfLogons, tb_Left);

	tb_DrawNumber(tbWindow, 265-2, 79+vo, numberOfLogoffs, tb_Left);

	tb_DrawNumber(tbWindow, 265-2, 92+vo, gracefulLogoffs, tb_Left);

	tb_DrawNumber(tbWindow, 265-2, 105+vo, serverFull, tb_Left);


	tb_DrawNumber(tbWindow, 440-4, 40+vo, (long)serverRestarts, tb_Left);  // merge 6/29/01

//	tb_DrawNumber(tbWindow, 440, 40, (long)totalBytesSent, tb_Left);  // merge 6/29/01

//	tb_DrawNumber(tbWindow, 440, 53, (long)totalBytesReceived, tb_Left);  // merge 6/29/01

	tb_DrawNumber(tbWindow, 440-4, 66+vo, maxMessageSize, tb_Left);

	tb_DrawNumber(tbWindow, 440-4, 79+vo, bytesSent, tb_Left);

	tb_DrawNumber(tbWindow, 540-4, 79+vo, maxBPSSent, tb_Left);

	tb_DrawNumber(tbWindow, 440-4, 92+vo, bytesReceived, tb_Left);

	tb_DrawNumber(tbWindow, 540-4, 92+vo, maxBPSReceived, tb_Left);

	tb_DrawNumber(tbWindow, 440-4, 105+vo, exceededNT, tb_Left);



	tb_DrawNumber(tbWindow, 740, 40+vo, tb_Round(((float)currentNumberOfContainers/(float)kMaxContainers)*100), tb_Left);

	//tb_DrawNumber(tbWindow, 740, 40, decayTime, tb_Left);
	//tb_DrawNumber(tbWindow, 740+60, 40, maxDecayTime, tb_Left);


	tb_DrawNumber(tbWindow, 740-6, 53+vo, ginsengSpawn + lotusFlowerSpawn + mandrakeRootSpawn + garlicSpawn + volcanicAshSpawn + mushroomSpawn, tb_Left); //Dave +mushroomSpawn

	//tb_DrawNumber(tbWindow, 740, 53, resourceTime, tb_Left);
	//tb_DrawNumber(tbWindow, 740+60, 53, maxResourceTime, tb_Left);

	needed=0;

	for (i=0;i<kMaxCreatureTypes;i++)
		if (creatureInfo[i].population<creatureInfo[i].desiredPopulation)
			needed=needed+(creatureInfo[i].desiredPopulation-creatureInfo[i].population);

	tb_DrawNumber(tbWindow, 740-6, 66+vo, needed, tb_Left);

	//tb_DrawNumber(tbWindow, 740, 66, monsterTime, tb_Left);
	//tb_DrawNumber(tbWindow, 740+60, 66, maxMonsterTime, tb_Left);

	tb_DrawNumber(tbWindow, 740-6, 79+vo, tb_Round(((float)currentCreatures/(float)(kMaxCreatures-5000))*100), tb_Left);

	//tb_DrawNumber(tbWindow, 740, 79, rotateShopTime, tb_Left);
	//tb_DrawNumber(tbWindow, 740+60, 79, maxRotateShopTime, tb_Left);

	tb_DrawNumber(tbWindow, 740-6, 92+vo, messagesReceived, tb_Left);

	tb_DrawNumber(tbWindow, 740+60-6, 92+vo, maxMessagesReceived, tb_Left);

	tb_DrawNumber(tbWindow, 740-6, 105+vo, messagesSent, tb_Left);

	tb_DrawNumber(tbWindow, 740+60-6, 105+vo, maxMessagesSent, tb_Left);


	//tb_DrawNumber(tbWindow, 941, 40, refreshTime, tb_Left);
	//tb_DrawNumber(tbWindow, 941+60, 40, maxRefreshTime, tb_Left);

	//tb_DrawNumber(tbWindow, 941, 53, serverFXTime, tb_Left);
	//tb_DrawNumber(tbWindow, 941+60, 53, maxServerFXTime, tb_Left);

	tb_DrawNumber(tbWindow, 941-8, 66+vo, skillGain, tb_Left);

	tb_DrawNumber(tbWindow, 941-8, 79+vo, playerDeaths, tb_Left);

	tb_DrawNumber(tbWindow, 941-8, 92+vo, creatureDeaths, tb_Left);

	tb_DrawNumber(tbWindow, 941-8, 105+vo, cycles, tb_Left);

	tb_DrawNumber(tbWindow, 941+60-8, 105+vo, minCycles, tb_Left);

}

// ----------------------------------------------------------------- //
void gr_DrawServerMain(void) // server only
// ----------------------------------------------------------------- //

{
	TRect				r;
	int					i;

	tb_DrawPictureToBitmap(tbWindow, 10000, 0, 0, &r);

	tb_SetRect(&nameRect[1],167,1,289,13);
	tb_SetRect(&nameRect[41],291,1,413,13);
	tb_SetRect(&nameRect[81],415,1,537,13);
	tb_SetRect(&nameRect[121],539,1,661,13);
	tb_SetRect(&nameRect[161],663,1,785,13);

	for (i=2;i<=40;i++)
	{
		nameRect[i]=nameRect[i-1];
		tb_OffsetRect(&nameRect[i],0,14);

		nameRect[i+40]=nameRect[i-1+40];
		tb_OffsetRect(&nameRect[i+40],0,14);

		nameRect[i+80]=nameRect[i-1+80];
		tb_OffsetRect(&nameRect[i+80],0,14);

		nameRect[i+120]=nameRect[i-1+120];
		tb_OffsetRect(&nameRect[i+120],0,14);

		nameRect[i+160]=nameRect[i-1+160];
		tb_OffsetRect(&nameRect[i+160],0,14);
	}

	/*
		tb_SetRect(&nameRect[1],1,93,201,117);
		tb_SetRect(&nameRect[21],203,93,403,117);
		tb_SetRect(&nameRect[41],405,93,605,117);
		tb_SetRect(&nameRect[61],607,93,807,117);
		tb_SetRect(&nameRect[81],809,93,1009,117);

		for (i=2;i<=20;i++)
			{
				nameRect[i]=nameRect[i-1];
				tb_OffsetRect(&nameRect[i],0,26);

				nameRect[i+20]=nameRect[i-1+20];
				tb_OffsetRect(&nameRect[i+20],0,26);

				nameRect[i+40]=nameRect[i-1+40];
				tb_OffsetRect(&nameRect[i+40],0,26);

				nameRect[i+60]=nameRect[i-1+60];
				tb_OffsetRect(&nameRect[i+60],0,26);

				nameRect[i+80]=nameRect[i-1+80];
				tb_OffsetRect(&nameRect[i+80],0,26);

			}
	*/

	/*
	tb_SetRect(&nameRect[1],3,96+21,103+1,107+21+1);
	tb_SetRect(&nameRect[51],105,96+21,205+1,107+21+1);
	tb_SetRect(&nameRect[101],207,96+21,307+1,107+21+1);
	tb_SetRect(&nameRect[151],309,96+21,409+1,107+21+1);
	tb_SetRect(&nameRect[201],411,96+21,511+1,107+21+1);
	tb_SetRect(&nameRect[251],513,96+21,613+1,107+21+1);
	tb_SetRect(&nameRect[301],615,96+21,715+1,107+21+1);
	tb_SetRect(&nameRect[351],717,96+21,817+1,107+21+1);
	tb_SetRect(&nameRect[401],819,96+21,919+1,107+21+1);
	tb_SetRect(&nameRect[451],921,96+21,1021+1,107+21+1);

	for (i=2;i<=50;i++)
		{
			nameRect[i]=nameRect[i-1];
			tb_OffsetRect(&nameRect[i],0,13);

			nameRect[i+50]=nameRect[i-1+50];
			tb_OffsetRect(&nameRect[i+50],0,13);

			nameRect[i+100]=nameRect[i-1+100];
			tb_OffsetRect(&nameRect[i+100],0,13);

			nameRect[i+150]=nameRect[i-1+150];
			tb_OffsetRect(&nameRect[i+150],0,13);

			nameRect[i+200]=nameRect[i-1+200];
			tb_OffsetRect(&nameRect[i+200],0,13);

			nameRect[i+250]=nameRect[i-1+250];
			tb_OffsetRect(&nameRect[i+250],0,13);

			nameRect[i+300]=nameRect[i-1+300];
			tb_OffsetRect(&nameRect[i+300],0,13);

			nameRect[i+350]=nameRect[i-1+350];
			tb_OffsetRect(&nameRect[i+350],0,13);

			nameRect[i+400]=nameRect[i-1+400];
			tb_OffsetRect(&nameRect[i+400],0,13);

			nameRect[i+450]=nameRect[i-1+450];
			tb_OffsetRect(&nameRect[i+450],0,13);
		}
	*/

	for (i=1;i<kMaxPlayers;i++)
		gr_DrawServerNameAt(i);

}

#endif

// ----------------------------------------------------------------- //
void gr_AddLocalFX(UInt8 position, UInt16 id, UInt8 type, UInt8 dest) // client
// ----------------------------------------------------------------- //

{
	int										i;
	TRect                 r;


	if ((dest<0) || (dest>=kMaxScreenElements))			// beta debug
		return;

	//fi_WriteToErrorLog("Out of Range - gr_AddLocalFX");

	if (type==0)
		return;

	for (i=0;i<kMaxLocalFX;i++)
		if ((id==localEffectsID[i]) && (type==localEffectsType[i]))
		{
			localEffectsLocation		[i]=position;
			localEffectsDestination		[i]=dest;
			localEffectsServiced		[i]=true;
			return;
		}

	for (i=0;i<kMaxLocalFX;i++)
		if (id==localEffectsID[i])
		{

			if (localEffectsType[i]==kSparkle)
				fx_DisposeSparkles(localEffectsIndex[i]);
			//Dave 12-15-2012
			if (localEffectsType[i]==kSparkle2)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kSparkle3)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kSparkle4)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kHealFountain)
				fx_DisposeParticleHose(localEffectsIndex[i]);
			if (localEffectsType[i]==kSteamFX)
				fx_DisposeFire(localEffectsIndex[i]);
			
			if (localEffectsType[i]==kWaterFountain)
				fx_DisposeParticleHose(localEffectsIndex[i]);
			

			if (localEffectsType[i]==kTempleFire)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}

			if (localEffectsType[i]==kSmallFire)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}

			if (localEffectsType[i]==kFireField)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}


			if (localEffectsType[i]==kLightExplosion)
				fx_DisposeFlash(localEffectsIndex[i]);

			if (localEffectsType[i]==kLightEllipse)
				fx_DisposeFlash(localEffectsIndex[i]);
			//Dave
			//if (localEffectsType[i]==kExplosion2)
			//	fx_CreateParticleExplosion(localEffectsIndex[i]);

			localEffectsID					[i]=-1;

			localEffectsType				[i]=0;

			localEffectsLocation		[i]=255;

			localEffectsDestination	[i]=255;

			localEffectsServiced		[i]=false;

			localEffectsIndex				[i]=-1;

			localEffectsFrame				[i]=0;
		}

	for (i=0;i<kMaxLocalFX;i++)
		if (localEffectsID[i]==-1)
		{
			localEffectsID					[i]=id;
			localEffectsType				[i]=type;
			localEffectsLocation		[i]=position;
			localEffectsServiced		[i]=true;
			localEffectsDestination	[i]=dest;
			localEffectsFrame			[i]=0;

			if (type==kLightEllipse)
				localEffectsIndex			[i]=fx_CreateFlash(destRect[dest].left+32, destRect[dest].top, 150, 75, kfxRed, kfxLightEllipseRing);

			//tb_RenderLightRingEllipse(playBMP, sx, sy+14, localEffectsMisc[j], (localEffectsMisc[j]/2), 50, 0, 50);

			if (type==kLightExplosion)
				localEffectsIndex			[i]=fx_CreateFlash(destRect[dest].left+32, destRect[dest].top, 100, 100, kfxRed, kfxLightSource);

			if (type==kSparkle)
				localEffectsIndex			[i]=fx_CreateSparkles(destRect[dest].left+32, destRect[dest].top, 36, kfxRed, 60, 2, kfxFall);
			//Dave 12-15-2012
			if (type==kSparkle2)
				localEffectsIndex			[i]=fx_CreateSparkles(destRect[dest].left+32, destRect[dest].top, 20, kfxGreen, 40, 3, kfxJitter);
			if (type==kSparkle3)
				localEffectsIndex			[i]=fx_CreateSparkles(destRect[dest].left+32, destRect[dest].top, 30, kfxRed, 60, 1, kfxJitter);
			if (type==kSparkle4)
				localEffectsIndex			[i]=fx_CreateSparkles(destRect[dest].left+32, destRect[dest].top, 36, kfxBlue, 60, 3, kfxRise);
			if (type==kHealFountain)
				localEffectsIndex			[i]=fx_CreateParticleHose(destRect[dest].left+29, destRect[dest].top+0, 200, 1000, 300, 90, 45, kfxRandom);
			if (type==kSteamFX)
				localEffectsIndex			[i]=fx_CreateFire(destRect[dest],kfxFireCone,kfxRandom,kfxBlendAll,true,kfxRoundBottom, true);
			

			if (type==kWaterFountain)
				localEffectsIndex			[i]=fx_CreateParticleHose(destRect[dest].left+29, destRect[dest].top+24, 100, 1200, 100, 90, 20, kfxBlue);

			if (type==kSmallFire)
			{
				localEffectsIndex			[i]=fx_CreateFire(destRect[dest],kfxFireWall,kfxRedFire,kfxBlendVert,false,kfxRoundBottom, true);
			}

			if (type==kTempleFire)
			{
				r=destRect[dest];
				//tb_InsetRect(&r,10,10);
				r.right=r.right-30;
				r.bottom=r.bottom-30;
				localEffectsIndex			[i]=fx_CreateFire(r,kfxFireWall,kfxBlueFire,kfxBlendVert,false,kfxRoundBottom, true);
			}

			if (type==kFireField)
			{
				localEffectsIndex			[i]=fx_CreateFire(destRect[dest],kfxFireWall,kfxRedFire,kfxBlendVert,false,kfxRoundBottom, false);
			}
			//Dave
			//if (type==kExplosion2)
			//	localEffectsIndex			[i]=fx_CreateParticleExplosion(destRect[dest].left+29, destRect[dest].top+24, 100, 20, 10.0, 90.0);
			
			return;
		}

}

// ----------------------------------------------------------------- //
void gr_KillLocalFX(void) // client
// ----------------------------------------------------------------- //

{
	int										i;


	for (i=0;i<kMaxLocalFX;i++)
		if (localEffectsServiced[i]==false)
		{

			if (localEffectsType[i]==kSparkle)
				fx_DisposeSparkles(localEffectsIndex[i]);
			//Dave 12-15-2012
			if (localEffectsType[i]==kSparkle2)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kSparkle3)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kSparkle4)
				fx_DisposeSparkles(localEffectsIndex[i]);
			if (localEffectsType[i]==kHealFountain)
				fx_DisposeParticleHose(localEffectsIndex[i]);
			if (localEffectsType[i]==kSteamFX)
				fx_DisposeFire(localEffectsIndex[i]);
			
			if (localEffectsType[i]==kWaterFountain)
				fx_DisposeParticleHose(localEffectsIndex[i]);
			

			if (localEffectsType[i]==kSmallFire)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}

			if (localEffectsType[i]==kTempleFire)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}

			if (localEffectsType[i]==kFireField)
			{
				fx_DisposeFire(localEffectsIndex[i]);
			}

			localEffectsID					[i]=-1;

			localEffectsType				[i]=0;
			localEffectsLocation		[i]=255;
			localEffectsDestination	[i]=255;
			localEffectsServiced		[i]=false;
			localEffectsIndex				[i]=-1;
			localEffectsFrame				[i]=0;
		}

}

// ----------------------------------------------------------------- //
void gr_MarkArmedInventoryItems(void) // client
// ----------------------------------------------------------------- //

{
	int						i;
	TRect					r;

	if (currentInfoDisplay!=kInventoryDisplay)
		return;

	for (i=1;i<kMaxInventorySize;i++)
		if (localSortedInventory[i].itemID!=0)
			if (in_ItemIsUsable(localSortedInventory[i].itemID))
				if (inventoryUseRect[i].left!=0)
				{
					if (inventorySlotArmed[localInventoryLookup[i]])
					{
						if (in_ItemEquiped(localInventoryLookup[i])==false)
						{
							tb_DrawPictureToBitmap(tbWindow, gr_ActiveButtonOf(localInventoryLookup[i]), inventoryUseRect[i].left, inventoryUseRect[i].top, &r);
							inventorySlotArmed[i]=false;
						}
					}
					else
					{
						if (in_ItemEquiped(localInventoryLookup[i])==true)
						{
							tb_DrawPictureToBitmap(tbWindow, gr_ActiveButtonOf(localInventoryLookup[i])+1, inventoryUseRect[i].left, inventoryUseRect[i].top, &r);
							inventorySlotArmed[i]=true;
						}
					}
				}

}

// ----------------------------------------------------------------- //
int gr_DistanceFromPlayer(TPoint pt) // client
// ----------------------------------------------------------------- //

{
	int							x1=(263);
	int							y1=(202);
	float						d;

	d=sqrt(((pt.h-x1)*(pt.h-x1))+((pt.v-y1)*(pt.v-y1)));

	return ((int)d);

}

// ----------------------------------------------------------------- //
void gr_MakeRect(TRect *r,int left, int top, int width, int height)
// ----------------------------------------------------------------- //

{
	int				right=left+width;
	int				bottom=top+height;
	TRect			t;

	tb_SetRect(&t,left,top, right, bottom);
	*r=t;

}

// ----------------------------------------------------------------- //
void gr_Animate(void)
// ----------------------------------------------------------------- //

{
	unsigned long						currentTime;

	currentTime=TickCount();


	if (currentTime>animationFrame2Time)
	{
		animationFrame2++;

		if (animationFrame2>1)
			animationFrame2=0;

		animationFrame2Time=currentTime+1;
	}

	animationFrame6++;

	if (animationFrame6>=6) animationFrame6=0;

	srcRect[kAirElemental]=animationRect[animationFrame6]; // air elemental

	srcRect[kWaterElemental]=animationRect[animationFrame6]; // water elemental

	srcRect[kPoisonElemental]=animationRect[animationFrame6]; // poison elemental

	srcRect[kFireElemental]=animationRect[animationFrame6]; // fire elemental
	//Dave
	srcRect[kEarthElemental]=animationRect[animationFrame6]; // earth elemental

	if (currentTime>flyTime)
	{
		fliesPtr++;

		if (fliesPtr>=10)
			fliesPtr=0;

		flyTime=currentTime+3;
	}
}

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
//void gr_AddToChatLog(char	newLine[128], int messageType)
void gr_AddToChatLog(char	newLine[kStringLength], int messageType)
// ----------------------------------------------------------------- //

{
	int								i;
	int               j;
	int               k;
	char              line[10][kStringLength]; //[kMaxChatLogLineLength];
	TBoolean          spaceFound;

	if (strcmp(newLine,"")==0)
		return;

	tb_SetFont(osBMP, kFontIDGeneva);

	tb_SetFontSize(osBMP, 10);

	tb_SetFontStyle(osBMP, tb_Plain);

	if (logText)
		fi_WriteToTextLog(newLine);

	for (i=0;i<10;i++)
		strcpy(line[i],"");

	i=0;

	j=0;

	k=0;

	spaceFound=false;

	while (newLine[i]!='\0')
	{
		line[j][k]=newLine[i];
		line[j][k+1]='\0';

		if (newLine[i]==' ')
			spaceFound=true;

		if ((tb_StringWidth(osBMP,line[j])>210) || (k>kMaxChatLogLineLength-3))
		{
			if (!spaceFound)
			{
				k--;
				i--;
				i--;
			}
			else
			{
				spaceFound=false;

				while (line[j][k]!=' ')
				{
					k--;
					i--;

					if (i==0)
					{
						i=kMaxChatLogLineLength-4;
						k=kMaxChatLogLineLength-3;
						break;
					}
				}
			}

			line[j][k]='\0';

			//i++;
			j++;
			k=-1;
		}

		i++;

		k++;
	}

	line[j][k]='\0';

	gFirstLine=0;

	for (i=(j+1);i<kMaxChatLogLines;i++)
	{
		strcpy(chatLog[i-j-1],chatLog[i]);
		chatLogType[i-j-1]=chatLogType[i];

		if (gFirstLine==0)
			if (strcmp(chatLog[i-j-1],"")!=0)
				gFirstLine=i-j-1;
	}

	k=0;

	while (j>=0)
	{
		strcpy(chatLog[kMaxChatLogLines-j-1],line[k]);
		chatLogType[kMaxChatLogLines-j-1]=(UInt8)messageType;
		j--;
		k++;
	}

	//if (currentInfoDisplay==kChatDisplay)
	gr_RenderChatDisplayWindow(0);

}

#endif

// ----------------------------------------------------------------- //
void gr_AddText(char	newLine[kStringLength], TBoolean systemMessage, TBoolean log)
// ----------------------------------------------------------------- //

{
#ifndef _SERVERONLY
	int								i;

	//04/09/07 - changed from 9 lines to 7

	if (displayFeedback || systemMessage)
	{
		for (i=0;i<7;i++)
		{
			strcpy(theText[i],theText[i+1]);
			theTextActive[i]	=	theTextActive[i+1];
			theTextDecay[i]		=	theTextDecay[i+1];
			theTextSystem[i]	=	theTextSystem[i+1];
		}

		strcpy(theText[7],newLine);

		theTextActive[7]	=	true;
		theTextDecay[7]		=	TickCount()+10*60;
		theTextSystem[7]	=	systemMessage;
	}
	

	if (log)
	{
		if (systemMessage)
			gr_AddToChatLog(newLine, kSystemMessage);
		else
			gr_AddToChatLog(newLine, kPlayerMessage);
	}

#endif
}

// ----------------------------------------------------------------- //
void gr_CreateElement(int id,int left, int top, int right, int bottom, int grid, int theBase, TBoolean isElevated, int domain, int color)
// ----------------------------------------------------------------- //

{

	if ((id<0) || (id>=kMaxIcons))			// beta debug
		fi_WriteToErrorLog("Out of Range - gr_CreateElement");

	tb_SetRect(&srcRect[id],	left,		top,		right,		bottom);

	mapColor[id]	= (UInt8)color;

	sourceGrid[id]			=	(UInt8)grid;

	elevated[id]	=	isElevated;

	base[id]			=	theBase;

	canWalkOn[id]	=	false;

	canSwimIn[id]	= false;

	canFlyOver[id]= false;

	if (domain==kLand)
	{
		canWalkOn[id]	=	true;
	}

	if (domain==kWater)
	{
		canSwimIn[id]	= true;
	}

	if (domain==kAir)
	{
		canWalkOn[id]	=	true;
		canSwimIn[id]	= true;
		canFlyOver[id]= false;
	}

}

// ----------------------------------------------------------------- //
void gr_InitializeRectangles(void)
// ----------------------------------------------------------------- //

{
	int						row;
	int						col;
	int						x;
	int						y;
	int						index;
	int						i;
	int           c;
	TRect					r;
	TRect					s;
	TRect					u;
	TRect					startRect;
	//char          ss[kStringLength]; //[32];

#ifdef _SERVERONLY
	maxMessagesSent=0;
	maxMessagesReceived=0;
	maxDecayTime=0;
	maxResourceTime=0;
	maxMonsterTime=0;
	maxRotateShopTime=0;
	maxRefreshTime=0;
	maxServerFXTime=0;
	serverRestarts=0;
	totalBytesSent=0;
	totalBytesReceived=0;
	maxBPSSent=0;
	maxBPSReceived=0;
	playerDeaths=0;
	creatureDeaths=0;
	cycles=0;
	minCycles=10000;
#endif

	cursorBlinkTime=TickCount();
	cursorBlink=true;

	gotFirstDrawMessage		=	false;
	thisIsFirstRefresh		=	true;
	prevDoorCount					= -1;
	nightVision						= false;
	chessSelectedPiece    = -1;
	currentDialog					= 0;
	localTarget						= 255;

	friend1               = 0;
	friend2               = 0;
	friend3               = 0;
	friend4               = 0;

	for (i=0;i<kMaxPets;i++)
	{
		localPetStatus[i]   = 0;
		localPetType[i]     = 0;
		localPetLoyalty[i]  = 0;
		strcpy(localPetName[i],"");
	}

	for (i=0;i<16;i++)
	{
		friendOnline[i]=false;
		friendSharing[i]=false;
	}

	for (i=0;i<kMaxWindows;i++)
		windowStack[i]=0;

	for (i=0;i<kMaxChatLogLines;i++)
	{
		strcpy(chatLog[i],"");
		chatLogType[i]=0;
	}

//	for (i=0;i<kMaxChatLogLines;i++)
//	  {
//	    tb_IntegerToString(i,ss);
//	    strcat(ss," Line Number");
//	    strcpy(chatLog[i],ss);
//	    chatLogType[i]=0;
//	  }

	for (i=0;i<kMaxLocalFX;i++)
	{
		localEffectsID					[i]=-1;
		localEffectsType				[i]=0;
		localEffectsLocation		[i]=255;
		localEffectsDestination	[i]=255;
		localEffectsServiced		[i]=false;
		localEffectsIndex				[i]=-1;
	}

	for (i=0;i<kMaxScreenElements;i++)
	{
		sparkle[i]=-1;
	}

	for (i=0;i<kMaxChatSlots;i++)
	{
		chatDecay[i]=0;
		//chatReverseLookup[i]=0;
	}

	for (i=0;i<kMaxScreenElements;i++)
	{
		strcpy(chatText[i],"");
		//chatDecay[i]=0;
	}

	for (i=0;i<7;i++)
	{
		strcpy(theText[i],"");
		theTextActive[i]	=	false;
		theTextDecay[i]		=	0;
		theTextSystem[i]	=	false;
	}

	tb_SetRect(&chatUpRect,0,0,0,0);

	tb_SetRect(&chatDownRect,0,0,0,0);
	tb_SetRect(&chatThumbRect,0,0,0,0);
	startChatLine = kMaxChatLogLines-40;
	gFirstLine    = 0;
	firstLine     = 0;
	lastLine      = 0;
	currentLine   = 0;

	tb_SetRect(&chessRect[0],550,73,578,101);
	tb_SetRect(&chessRect[0],579,73,607,101);
	tb_SetRect(&chessRect[0],608,73,636,101);

	for (c=0;c<8;c++)
	{
		tb_SetRect(&chessRect[c],550+(29*c),73,578+(29*c),101);
		tb_SetRect(&chessRect[c+8],550+(29*c),102,578+(29*c),130);
		tb_SetRect(&chessRect[c+16],550+(29*c),131,578+(29*c),159);
		tb_SetRect(&chessRect[c+24],550+(29*c),160,578+(29*c),188);
		tb_SetRect(&chessRect[c+32],550+(29*c),189,578+(29*c),217);
		tb_SetRect(&chessRect[c+40],550+(29*c),218,578+(29*c),246);
		tb_SetRect(&chessRect[c+48],550+(29*c),247,578+(29*c),275);
		tb_SetRect(&chessRect[c+56],550+(29*c),276,578+(29*c),304);

		tb_SetRect(&chessRect[c+64],550+(29*c),324,578+(29*c),352);
		tb_SetRect(&chessRect[c+72],550+(29*c),353,578+(29*c),381);
		tb_SetRect(&chessRect[c+80],550+(29*c),382,578+(29*c),410);
		tb_SetRect(&chessRect[c+88],550+(29*c),411,578+(29*c),439);
	}

	for (i=0;i<96;i++)
	{
		tb_OffsetRect(&chessRect[i],-547,-70);
		chessScreenRect[i]=chessRect[i];
	}

	tb_SetRect(&chessPiece[kBlackKing],1,1,29,29);

	tb_SetRect(&chessPiece[kBlackQueen],31,1,59,29);
	tb_SetRect(&chessPiece[kBlackRook],61,1,89,29);
	tb_SetRect(&chessPiece[kBlackBishop],91,1,119,29);
	tb_SetRect(&chessPiece[kBlackKnight],121,1,149,29);
	tb_SetRect(&chessPiece[kBlackPawn],151,1,179,29);
	tb_SetRect(&chessPiece[kWhiteKing],1,31,29,59);
	tb_SetRect(&chessPiece[kWhiteQueen],31,31,59,59);
	tb_SetRect(&chessPiece[kWhiteRook],61,31,89,59);
	tb_SetRect(&chessPiece[kWhiteBishop],91,31,119,59);
	tb_SetRect(&chessPiece[kWhiteKnight],121,31,149,59);
	tb_SetRect(&chessPiece[kWhitePawn],151,31,179,59);

	flyTime=TickCount();
	fliesPtr=0;
	tb_SetRect(&fliesRect[0],1,287,53,321);
	tb_SetRect(&fliesRect[1],54,287,106,321);
	tb_SetRect(&fliesRect[2],107,287,159,321);
	tb_SetRect(&fliesRect[3],160,287,212,321);
	tb_SetRect(&fliesRect[4],213,287,265,321);
	tb_SetRect(&fliesRect[5],266,287,318,321);
	tb_SetRect(&fliesRect[6],319,287,371,321);
	tb_SetRect(&fliesRect[7],372,287,424,321);
	tb_SetRect(&fliesRect[8],425,287,477,321);
	tb_SetRect(&fliesRect[9],478,287,530,321);

	tb_SetRect(&inventoryScreenRect,548+horizontalOffset,382+verticalOffset,760+horizontalOffset,580+verticalOffset);
	tb_SetRect(&inventoryRect,0,0,212,198);

	//tb_SetRect(&playScreenRect,0,32,512,512+32);
	//tb_SetRect(&playScreenRect,18+horizontalOffset,69+verticalOffset,512+18+horizontalOffset,512+69+verticalOffset);
	tb_SetRect(&playScreenRect,0,0,512,512);

	tb_SetRect(&playOSScreenRect,18,69,512+18,512+69);

	tb_SetRect(&baseHuman[0][0],1,1,64,74);
	tb_SetRect(&baseHuman[0][1],65,1,128,74);
	tb_SetRect(&baseHuman[0][2],449,1,512,74);

	tb_SetRect(&baseHuman[1][0],1,75,64,148);
	tb_SetRect(&baseHuman[1][1],65,75,128,148);
	tb_SetRect(&baseHuman[1][2],449,75,512,148);

	tb_SetRect(&baseHuman[2][0],1,149,64,222);
	tb_SetRect(&baseHuman[2][1],65,149,128,222);
	tb_SetRect(&baseHuman[2][2],449,149,512,222);

	tb_SetRect(&baseHuman[3][0],1,223,64,296);
	tb_SetRect(&baseHuman[3][1],65,223,128,296);
	tb_SetRect(&baseHuman[3][2],449,223,512,296);

	tb_SetRect(&baseHuman[4][0],1,297,64,370);
	tb_SetRect(&baseHuman[4][1],65,297,128,370);
	tb_SetRect(&baseHuman[4][2],449,297,512,370);

	tb_SetRect(&baseHuman[5][0],1,371,64,444);
	tb_SetRect(&baseHuman[5][1],65,371,128,444);
	tb_SetRect(&baseHuman[5][2],449,371,512,444);

	tb_SetRect(&baseHuman[6][0],1,445,64,518);
	tb_SetRect(&baseHuman[6][1],65,445,128,518);
	tb_SetRect(&baseHuman[6][2],449,445,512,518);

	tb_SetRect(&baseHuman[7][0],1,519,64,592);
	tb_SetRect(&baseHuman[7][1],65,519,128,592);
	tb_SetRect(&baseHuman[7][2],449,519,512,592);

	tb_SetRect(&humanShirt[0],129,1,192,74);
	tb_SetRect(&humanShirt[1],129,75,192,148);
	tb_SetRect(&humanShirt[2],129,149,192,222);
	tb_SetRect(&humanShirt[3],129,223,192,296);
	tb_SetRect(&humanShirt[4],129,297,192,370);
	tb_SetRect(&humanShirt[5],129,371,192,444);
	tb_SetRect(&humanShirt[6],129,445,192,518);
	tb_SetRect(&humanShirt[7],129,519,192,592);

	tb_SetRect(&humanPants[0][0],193,1,256,74);
	tb_SetRect(&humanPants[0][1],257,1,320,74);
	tb_SetRect(&humanPants[0][2],193,1,256,74);

	tb_SetRect(&humanPants[1][0],193,75,256,148);
	tb_SetRect(&humanPants[1][1],257,75,320,148);
	tb_SetRect(&humanPants[1][2],193,75,256,148);

	tb_SetRect(&humanPants[2][0],193,149,256,222);
	tb_SetRect(&humanPants[2][1],257,149,320,222);
	tb_SetRect(&humanPants[2][2],193,149,256,222);

	tb_SetRect(&humanPants[3][0],193,223,256,296);
	tb_SetRect(&humanPants[3][1],257,223,320,296);
	tb_SetRect(&humanPants[3][2],193,223,256,296);

	tb_SetRect(&humanPants[4][0],193,297,256,370);
	tb_SetRect(&humanPants[4][1],257,297,320,370);
	tb_SetRect(&humanPants[4][2],193,297,256,370);

	tb_SetRect(&humanPants[5][0],193,371,256,444);
	tb_SetRect(&humanPants[5][1],257,371,320,444);
	tb_SetRect(&humanPants[5][2],193,371,256,444);

	tb_SetRect(&humanPants[6][0],193,445,256,518);
	tb_SetRect(&humanPants[6][1],257,445,320,518);
	tb_SetRect(&humanPants[6][2],193,445,256,518);

	tb_SetRect(&humanPants[7][0],193,519,256,592);
	tb_SetRect(&humanPants[7][1],257,519,320,592);
	tb_SetRect(&humanPants[7][2],193,519,256,592);

	tb_SetRect(&humanRobe[0][0],321,1,384,74);
	tb_SetRect(&humanRobe[0][1],385,1,448,74);
	tb_SetRect(&humanRobe[0][2],513,1,576,74);

	tb_SetRect(&humanRobe[1][0],321,75,384,148);
	tb_SetRect(&humanRobe[1][1],385,75,448,148);
	tb_SetRect(&humanRobe[1][2],513,75,576,148);

	tb_SetRect(&humanRobe[2][0],321,149,384,222);
	tb_SetRect(&humanRobe[2][1],385,149,448,222);
	tb_SetRect(&humanRobe[2][2],513,149,576,222);

	tb_SetRect(&humanRobe[3][0],321,223,384,296);
	tb_SetRect(&humanRobe[3][1],385,223,448,296);
	tb_SetRect(&humanRobe[3][2],513,223,576,296);

	tb_SetRect(&humanRobe[4][0],321,297,384,370);
	tb_SetRect(&humanRobe[4][1],385,297,448,370);
	tb_SetRect(&humanRobe[4][2],513,297,576,370);

	tb_SetRect(&humanRobe[5][0],321,371,384,444);
	tb_SetRect(&humanRobe[5][1],385,371,448,444);
	tb_SetRect(&humanRobe[5][2],513,371,576,444);

	tb_SetRect(&humanRobe[6][0],321,445,384,518);
	tb_SetRect(&humanRobe[6][1],385,445,448,518);
	tb_SetRect(&humanRobe[6][2],513,445,576,518);

	tb_SetRect(&humanRobe[7][0],321,519,384,592);
	tb_SetRect(&humanRobe[7][1],385,519,448,592);
	tb_SetRect(&humanRobe[7][2],513,519,576,592);
	
	//human white robe
	tb_SetRect(&humanWhiteRobe[0][0],1154,1,1216,73);
	tb_SetRect(&humanWhiteRobe[0][1],1218,1,1280,73);
	tb_SetRect(&humanWhiteRobe[0][2],1282,1,1344,73);

	tb_SetRect(&humanWhiteRobe[1][0],1154,75,1216,147);
	tb_SetRect(&humanWhiteRobe[1][1],1218,75,1280,147);
	tb_SetRect(&humanWhiteRobe[1][2],1282,75,1344,147);

	tb_SetRect(&humanWhiteRobe[2][0],1154,149,1216,221);
	tb_SetRect(&humanWhiteRobe[2][1],1218,149,1280,221);
	tb_SetRect(&humanWhiteRobe[2][2],1282,149,1344,221);

	tb_SetRect(&humanWhiteRobe[3][0],1154,223,1216,295);
	tb_SetRect(&humanWhiteRobe[3][1],1218,223,1280,295);
	tb_SetRect(&humanWhiteRobe[3][2],1282,223,1344,295);

	tb_SetRect(&humanWhiteRobe[4][0],1154,297,1216,369);
	tb_SetRect(&humanWhiteRobe[4][1],1218,297,1280,369);
	tb_SetRect(&humanWhiteRobe[4][2],1282,297,1344,369);

	tb_SetRect(&humanWhiteRobe[5][0],1154,371,1216,443);
	tb_SetRect(&humanWhiteRobe[5][1],1218,371,1280,443);
	tb_SetRect(&humanWhiteRobe[5][2],1282,371,1344,443);

	tb_SetRect(&humanWhiteRobe[6][0],1154,445,1216,517);
	tb_SetRect(&humanWhiteRobe[6][1],1218,445,1280,517);
	tb_SetRect(&humanWhiteRobe[6][2],1282,445,1344,517);

	tb_SetRect(&humanWhiteRobe[7][0],1154,519,1216,591);
	tb_SetRect(&humanWhiteRobe[7][1],1218,519,1280,591);
	tb_SetRect(&humanWhiteRobe[7][2],1282,519,1344,591);

	// human GM robe
	tb_SetRect(&humanGMRobe[0][0],577,1,640,74);
	tb_SetRect(&humanGMRobe[0][1],641,1,704,74);
	tb_SetRect(&humanGMRobe[0][2],705,1,768,74);

	tb_SetRect(&humanGMRobe[1][0],577,75,640,148);
	tb_SetRect(&humanGMRobe[1][1],641,75,704,148);
	tb_SetRect(&humanGMRobe[1][2],705,75,768,148);

	tb_SetRect(&humanGMRobe[2][0],577,149,640,222);
	tb_SetRect(&humanGMRobe[2][1],641,149,704,222);
	tb_SetRect(&humanGMRobe[2][2],705,149,768,222);

	tb_SetRect(&humanGMRobe[3][0],577,223,640,296);
	tb_SetRect(&humanGMRobe[3][1],641,223,704,296);
	tb_SetRect(&humanGMRobe[3][2],705,223,768,296);

	tb_SetRect(&humanGMRobe[4][0],577,297,640,370);
	tb_SetRect(&humanGMRobe[4][1],641,297,704,370);
	tb_SetRect(&humanGMRobe[4][2],705,297,768,370);

	tb_SetRect(&humanGMRobe[5][0],577,371,640,444);
	tb_SetRect(&humanGMRobe[5][1],641,371,704,444);
	tb_SetRect(&humanGMRobe[5][2],705,371,768,444);

	tb_SetRect(&humanGMRobe[6][0],577,445,640,518);
	tb_SetRect(&humanGMRobe[6][1],641,445,704,518);
	tb_SetRect(&humanGMRobe[6][2],705,445,768,518);

	tb_SetRect(&humanGMRobe[7][0],577,519,640,592);
	tb_SetRect(&humanGMRobe[7][1],641,519,704,592);
	tb_SetRect(&humanGMRobe[7][2],705,519,768,592);

	// AFK tag
	tb_SetRect(&humanAFKTag, 1601,1,1622,22);

	// human royal robe
	tb_SetRect(&humanRoyalRobe[0][0],769,1,832,74);
	tb_SetRect(&humanRoyalRobe[0][1],833,1,896,74);
	tb_SetRect(&humanRoyalRobe[0][2],897,1,960,74);

	tb_SetRect(&humanRoyalRobe[1][0],769,75,832,148);
	tb_SetRect(&humanRoyalRobe[1][1],833,75,896,148);
	tb_SetRect(&humanRoyalRobe[1][2],897,75,960,148);

	tb_SetRect(&humanRoyalRobe[2][0],769,149,832,222);
	tb_SetRect(&humanRoyalRobe[2][1],833,149,896,222);
	tb_SetRect(&humanRoyalRobe[2][2],897,149,960,222);

	tb_SetRect(&humanRoyalRobe[3][0],769,223,832,296);
	tb_SetRect(&humanRoyalRobe[3][1],833,223,896,296);
	tb_SetRect(&humanRoyalRobe[3][2],897,223,960,296);

	tb_SetRect(&humanRoyalRobe[4][0],769,297,832,370);
	tb_SetRect(&humanRoyalRobe[4][1],833,297,896,370);
	tb_SetRect(&humanRoyalRobe[4][2],897,297,960,370);

	tb_SetRect(&humanRoyalRobe[5][0],769,371,832,444);
	tb_SetRect(&humanRoyalRobe[5][1],833,371,896,444);
	tb_SetRect(&humanRoyalRobe[5][2],897,371,960,444);

	tb_SetRect(&humanRoyalRobe[6][0],769,445,832,518);
	tb_SetRect(&humanRoyalRobe[6][1],833,445,896,518);
	tb_SetRect(&humanRoyalRobe[6][2],897,445,960,518);

	tb_SetRect(&humanRoyalRobe[7][0],769,519,832,592);
	tb_SetRect(&humanRoyalRobe[7][1],833,519,896,592);
	tb_SetRect(&humanRoyalRobe[7][2],897,519,960,592);

	// --------------- staff frames
	tb_SetRect(&humanStaff[0][0],577,1,640,74);
	tb_SetRect(&humanStaff[0][1],641,1,704,74);

	tb_SetRect(&humanStaff[1][0],577,75,640,148);
	tb_SetRect(&humanStaff[1][1],641,75,704,148);

	tb_SetRect(&humanStaff[2][0],577,149,640,222);
	tb_SetRect(&humanStaff[2][1],641,149,704,222);

	tb_SetRect(&humanStaff[3][0],577,223,640,296);
	tb_SetRect(&humanStaff[3][1],641,223,704,296);

	tb_SetRect(&humanStaff[4][0],577,297,640,370);
	tb_SetRect(&humanStaff[4][1],641,297,704,370);

	tb_SetRect(&humanStaff[5][0],577,371,640,444);
	tb_SetRect(&humanStaff[5][1],641,371,704,444);

	tb_SetRect(&humanStaff[6][0],577,445,640,518);
	tb_SetRect(&humanStaff[6][1],641,445,704,518);

	tb_SetRect(&humanStaff[7][0],577,519,640,592);
	tb_SetRect(&humanStaff[7][1],641,519,704,592);

	// --------------- sickle frames
	tb_SetRect(&humanSickle[0][0],705,1,768,74);
	tb_SetRect(&humanSickle[0][1],769,1,832,74);

	tb_SetRect(&humanSickle[1][0],705,75,768,148);
	tb_SetRect(&humanSickle[1][1],769,75,832,148);

	tb_SetRect(&humanSickle[2][0],705,149,768,222);
	tb_SetRect(&humanSickle[2][1],769,149,832,222);

	tb_SetRect(&humanSickle[3][0],705,223,768,296);
	tb_SetRect(&humanSickle[3][1],769,223,832,296);

	tb_SetRect(&humanSickle[4][0],705,297,768,370);
	tb_SetRect(&humanSickle[4][1],769,297,832,370);

	tb_SetRect(&humanSickle[5][0],705,371,768,444);
	tb_SetRect(&humanSickle[5][1],769,371,832,444);

	tb_SetRect(&humanSickle[6][0],705,445,768,518);
	tb_SetRect(&humanSickle[6][1],769,445,832,518);

	tb_SetRect(&humanSickle[7][0],705,519,768,592);
	tb_SetRect(&humanSickle[7][1],769,519,832,592);

	// --------------- sword frames
	tb_SetRect(&humanSword[0][0],833,1,896,74);
	tb_SetRect(&humanSword[0][1],897,1,960,74);

	tb_SetRect(&humanSword[1][0],833,75,896,148);
	tb_SetRect(&humanSword[1][1],897,75,960,148);

	tb_SetRect(&humanSword[2][0],833,149,896,222);
	tb_SetRect(&humanSword[2][1],897,149,960,222);

	tb_SetRect(&humanSword[3][0],833,223,896,296);
	tb_SetRect(&humanSword[3][1],897,223,960,296);

	tb_SetRect(&humanSword[4][0],833,297,896,370);
	tb_SetRect(&humanSword[4][1],897,297,960,370);

	tb_SetRect(&humanSword[5][0],833,371,896,444);
	tb_SetRect(&humanSword[5][1],897,371,960,444);

	tb_SetRect(&humanSword[6][0],833,445,896,518);
	tb_SetRect(&humanSword[6][1],897,445,960,518);

	tb_SetRect(&humanSword[7][0],833,519,896,592);
	tb_SetRect(&humanSword[7][1],897,519,960,592);

	// --------------- fire sword frames
	tb_SetRect(&humanFireSword[0][0],1,1,64,74);
	tb_SetRect(&humanFireSword[0][1],65,1,128,74);

	tb_SetRect(&humanFireSword[1][0],1,75,64,148);
	tb_SetRect(&humanFireSword[1][1],65,75,128,148);

	tb_SetRect(&humanFireSword[2][0],1,149,64,222);
	tb_SetRect(&humanFireSword[2][1],65,149,128,222);

	tb_SetRect(&humanFireSword[3][0],1,223,64,296);
	tb_SetRect(&humanFireSword[3][1],65,223,128,296);

	tb_SetRect(&humanFireSword[4][0],1,297,64,370);
	tb_SetRect(&humanFireSword[4][1],65,297,128,370);

	tb_SetRect(&humanFireSword[5][0],1,371,64,444);
	tb_SetRect(&humanFireSword[5][1],65,371,128,444);

	tb_SetRect(&humanFireSword[6][0],1,445,64,518);
	tb_SetRect(&humanFireSword[6][1],65,445,128,518);

	tb_SetRect(&humanFireSword[7][0],1,519,64,592);
	tb_SetRect(&humanFireSword[7][1],65,519,128,592);

	// --------------- trident frames
	tb_SetRect(&humanTrident[0][0],129,1,192,74);
	tb_SetRect(&humanTrident[0][1],193,1,256,74);

	tb_SetRect(&humanTrident[1][0],129,75,192,148);
	tb_SetRect(&humanTrident[1][1],193,75,256,148);

	tb_SetRect(&humanTrident[2][0],129,149,192,222);
	tb_SetRect(&humanTrident[2][1],193,149,256,222);

	tb_SetRect(&humanTrident[3][0],129,223,192,296);
	tb_SetRect(&humanTrident[3][1],193,223,256,296);

	tb_SetRect(&humanTrident[4][0],129,297,192,370);
	tb_SetRect(&humanTrident[4][1],193,297,256,370);

	tb_SetRect(&humanTrident[5][0],129,371,192,444);
	tb_SetRect(&humanTrident[5][1],193,371,256,444);

	tb_SetRect(&humanTrident[6][0],129,445,192,518);
	tb_SetRect(&humanTrident[6][1],193,445,256,518);

	tb_SetRect(&humanTrident[7][0],129,519,192,592);
	tb_SetRect(&humanTrident[7][1],193,519,256,592);

	// --------------- small axe frames
	tb_SetRect(&humanSmallAxe[0][0],961,1,1024,74);
	tb_SetRect(&humanSmallAxe[0][1],1025,1,1088,74);

	tb_SetRect(&humanSmallAxe[1][0],961,75,1024,148);
	tb_SetRect(&humanSmallAxe[1][1],1025,75,1088,148);

	tb_SetRect(&humanSmallAxe[2][0],961,149,1024,222);
	tb_SetRect(&humanSmallAxe[2][1],1025,149,1088,222);

	tb_SetRect(&humanSmallAxe[3][0],961,223,1024,296);
	tb_SetRect(&humanSmallAxe[3][1],1025,223,1088,296);

	tb_SetRect(&humanSmallAxe[4][0],961,297,1024,370);
	tb_SetRect(&humanSmallAxe[4][1],1025,297,1088,370);

	tb_SetRect(&humanSmallAxe[5][0],961,371,1024,444);
	tb_SetRect(&humanSmallAxe[5][1],1025,371,1088,444);

	tb_SetRect(&humanSmallAxe[6][0],961,445,1024,518);
	tb_SetRect(&humanSmallAxe[6][1],1025,445,1088,518);

	tb_SetRect(&humanSmallAxe[7][0],961,519,1024,592);
	tb_SetRect(&humanSmallAxe[7][1],1025,519,1088,592);

	// --------------- pick axe frames
	tb_SetRect(&humanPickAxe[0][0],1089,1,1152,74);
	tb_SetRect(&humanPickAxe[0][1],1153,1,1216,74);

	tb_SetRect(&humanPickAxe[1][0],1089,75,1152,148);
	tb_SetRect(&humanPickAxe[1][1],1153,75,1216,148);

	tb_SetRect(&humanPickAxe[2][0],1089,149,1152,222);
	tb_SetRect(&humanPickAxe[2][1],1153,149,1216,222);

	tb_SetRect(&humanPickAxe[3][0],1089,223,1152,296);
	tb_SetRect(&humanPickAxe[3][1],1153,223,1216,296);

	tb_SetRect(&humanPickAxe[4][0],1089,297,1152,370);
	tb_SetRect(&humanPickAxe[4][1],1153,297,1216,370);

	tb_SetRect(&humanPickAxe[5][0],1089,371,1152,444);
	tb_SetRect(&humanPickAxe[5][1],1153,371,1216,444);

	tb_SetRect(&humanPickAxe[6][0],1089,445,1152,518);
	tb_SetRect(&humanPickAxe[6][1],1153,445,1216,518);

	tb_SetRect(&humanPickAxe[7][0],1089,519,1152,592);
	tb_SetRect(&humanPickAxe[7][1],1153,519,1216,592);

	// --------------- fishing pole frames
	tb_SetRect(&humanFishingPole[0][0],1217,1,1280,74);
	tb_SetRect(&humanFishingPole[0][1],1281,1,1344,74);

	tb_SetRect(&humanFishingPole[1][0],1217,75,1280,148);
	tb_SetRect(&humanFishingPole[1][1],1281,75,1344,148);

	tb_SetRect(&humanFishingPole[2][0],1217,149,1280,222);
	tb_SetRect(&humanFishingPole[2][1],1281,149,1344,222);

	tb_SetRect(&humanFishingPole[3][0],1217,223,1280,296);
	tb_SetRect(&humanFishingPole[3][1],1281,223,1344,296);

	tb_SetRect(&humanFishingPole[4][0],1217,297,1280,370);
	tb_SetRect(&humanFishingPole[4][1],1281,297,1344,370);

	tb_SetRect(&humanFishingPole[5][0],1217,371,1280,444);
	tb_SetRect(&humanFishingPole[5][1],1281,371,1344,444);

	tb_SetRect(&humanFishingPole[6][0],1217,445,1280,518);
	tb_SetRect(&humanFishingPole[6][1],1281,445,1344,518);

	tb_SetRect(&humanFishingPole[7][0],1217,519,1280,592);
	tb_SetRect(&humanFishingPole[7][1],1281,519,1344,592);

	// --------------- battle axe frames
	tb_SetRect(&humanBattleAxe[0][0],1345,1,1408,74);
	tb_SetRect(&humanBattleAxe[0][1],1409,1,1472,74);

	tb_SetRect(&humanBattleAxe[1][0],1345,75,1408,148);
	tb_SetRect(&humanBattleAxe[1][1],1409,75,1472,148);

	tb_SetRect(&humanBattleAxe[2][0],1345,149,1408,222);
	tb_SetRect(&humanBattleAxe[2][1],1409,149,1472,222);

	tb_SetRect(&humanBattleAxe[3][0],1345,223,1408,296);
	tb_SetRect(&humanBattleAxe[3][1],1409,223,1472,296);

	tb_SetRect(&humanBattleAxe[4][0],1345,297,1408,370);
	tb_SetRect(&humanBattleAxe[4][1],1409,297,1472,370);

	tb_SetRect(&humanBattleAxe[5][0],1345,371,1408,444);
	tb_SetRect(&humanBattleAxe[5][1],1409,371,1472,444);

	tb_SetRect(&humanBattleAxe[6][0],1345,445,1408,518);
	tb_SetRect(&humanBattleAxe[6][1],1409,445,1472,518);

	tb_SetRect(&humanBattleAxe[7][0],1345,519,1408,592);
	tb_SetRect(&humanBattleAxe[7][1],1409,519,1472,592);

	// --------------- club frames
	tb_SetRect(&humanClub[0][0],1345,1,1408,74);
	tb_SetRect(&humanClub[0][1],1409,1,1472,74);

	tb_SetRect(&humanClub[1][0],1345,75,1408,148);
	tb_SetRect(&humanClub[1][1],1409,75,1472,148);

	tb_SetRect(&humanClub[2][0],1345,149,1408,222);
	tb_SetRect(&humanClub[2][1],1409,149,1472,222);

	tb_SetRect(&humanClub[3][0],1345,223,1408,296);
	tb_SetRect(&humanClub[3][1],1409,223,1472,296);

	tb_SetRect(&humanClub[4][0],1345,297,1408,370);
	tb_SetRect(&humanClub[4][1],1409,297,1472,370);

	tb_SetRect(&humanClub[5][0],1345,371,1408,444);
	tb_SetRect(&humanClub[5][1],1409,371,1472,444);

	tb_SetRect(&humanClub[6][0],1345,445,1408,518);
	tb_SetRect(&humanClub[6][1],1409,445,1472,518);

	tb_SetRect(&humanClub[7][0],1345,519,1408,592);
	tb_SetRect(&humanClub[7][1],1409,519,1472,592);

	// --------------- club frames
	tb_SetRect(&humanClub[0][0],1473,1,1536,74);
	tb_SetRect(&humanClub[0][1],1537,1,1600,74);

	tb_SetRect(&humanClub[1][0],1473,75,1536,148);
	tb_SetRect(&humanClub[1][1],1537,75,1600,148);

	tb_SetRect(&humanClub[2][0],1473,149,1536,222);
	tb_SetRect(&humanClub[2][1],1537,149,1600,222);

	tb_SetRect(&humanClub[3][0],1473,223,1536,296);
	tb_SetRect(&humanClub[3][1],1537,223,1600,296);

	tb_SetRect(&humanClub[4][0],1473,297,1536,370);
	tb_SetRect(&humanClub[4][1],1537,297,1600,370);

	tb_SetRect(&humanClub[5][0],1473,371,1536,444);
	tb_SetRect(&humanClub[5][1],1537,371,1600,444);

	tb_SetRect(&humanClub[6][0],1473,445,1536,518);
	tb_SetRect(&humanClub[6][1],1537,445,1600,518);

	tb_SetRect(&humanClub[7][0],1473,519,1536,592);
	tb_SetRect(&humanClub[7][1],1537,519,1600,592);

	// --------------- mace frames
	tb_SetRect(&humanMace[0][0],1601,1,1664,74);
	tb_SetRect(&humanMace[0][1],1665,1,1728,74);

	tb_SetRect(&humanMace[1][0],1601,75,1664,148);
	tb_SetRect(&humanMace[1][1],1665,75,1728,148);

	tb_SetRect(&humanMace[2][0],1601,149,1664,222);
	tb_SetRect(&humanMace[2][1],1665,149,1728,222);

	tb_SetRect(&humanMace[3][0],1601,223,1664,296);
	tb_SetRect(&humanMace[3][1],1665,223,1728,296);

	tb_SetRect(&humanMace[4][0],1601,297,1664,370);
	tb_SetRect(&humanMace[4][1],1665,297,1728,370);

	tb_SetRect(&humanMace[5][0],1601,371,1664,444);
	tb_SetRect(&humanMace[5][1],1665,371,1728,444);

	tb_SetRect(&humanMace[6][0],1601,445,1664,518);
	tb_SetRect(&humanMace[6][1],1665,445,1728,518);

	tb_SetRect(&humanMace[7][0],1601,519,1664,592);
	tb_SetRect(&humanMace[7][1],1665,519,1728,592);

	// --------------- flail frames
	tb_SetRect(&humanFlail[0][0],1729,1,1792,74);
	tb_SetRect(&humanFlail[0][1],1793,1,1856,74);

	tb_SetRect(&humanFlail[1][0],1729,75,1792,148);
	tb_SetRect(&humanFlail[1][1],1793,75,1856,148);

	tb_SetRect(&humanFlail[2][0],1729,149,1792,222);
	tb_SetRect(&humanFlail[2][1],1793,149,1856,222);

	tb_SetRect(&humanFlail[3][0],1729,223,1792,296);
	tb_SetRect(&humanFlail[3][1],1793,223,1856,296);

	tb_SetRect(&humanFlail[4][0],1729,297,1792,370);
	tb_SetRect(&humanFlail[4][1],1793,297,1856,370);

	tb_SetRect(&humanFlail[5][0],1729,371,1792,444);
	tb_SetRect(&humanFlail[5][1],1793,371,1856,444);

	tb_SetRect(&humanFlail[6][0],1729,445,1792,518);
	tb_SetRect(&humanFlail[6][1],1793,445,1856,518);

	tb_SetRect(&humanFlail[7][0],1729,519,1792,592);
	tb_SetRect(&humanFlail[7][1],1793,519,1856,592);

	// --------------- glaive frames
	tb_SetRect(&humanGlaive[0][0],1857,1,1920,74);
	tb_SetRect(&humanGlaive[0][1],1921,1,1984,74);

	tb_SetRect(&humanGlaive[1][0],1857,75,1920,148);
	tb_SetRect(&humanGlaive[1][1],1921,75,1984,148);

	tb_SetRect(&humanGlaive[2][0],1857,149,1920,222);
	tb_SetRect(&humanGlaive[2][1],1921,149,1984,222);

	tb_SetRect(&humanGlaive[3][0],1857,223,1920,296);
	tb_SetRect(&humanGlaive[3][1],1921,223,1984,296);

	tb_SetRect(&humanGlaive[4][0],1857,297,1920,370);
	tb_SetRect(&humanGlaive[4][1],1921,297,1984,370);

	tb_SetRect(&humanGlaive[5][0],1857,371,1920,444);
	tb_SetRect(&humanGlaive[5][1],1921,371,1984,444);

	tb_SetRect(&humanGlaive[6][0],1857,445,1920,518);
	tb_SetRect(&humanGlaive[6][1],1921,445,1984,518);

	tb_SetRect(&humanGlaive[7][0],1857,519,1920,592);
	tb_SetRect(&humanGlaive[7][1],1921,519,1984,592);

	// --------------- spear frames
	tb_SetRect(&humanSpear[0][0],1985,1,2048,74);
	tb_SetRect(&humanSpear[0][1],2049,1,2112,74);

	tb_SetRect(&humanSpear[1][0],1985,75,2048,148);
	tb_SetRect(&humanSpear[1][1],2049,75,2112,148);

	tb_SetRect(&humanSpear[2][0],1985,149,2048,222);
	tb_SetRect(&humanSpear[2][1],2049,149,2112,222);

	tb_SetRect(&humanSpear[3][0],1985,223,2048,296);
	tb_SetRect(&humanSpear[3][1],2049,223,2112,296);

	tb_SetRect(&humanSpear[4][0],1985,297,2048,370);
	tb_SetRect(&humanSpear[4][1],2049,297,2112,370);

	tb_SetRect(&humanSpear[5][0],1985,371,2048,444);
	tb_SetRect(&humanSpear[5][1],2049,371,2112,444);

	tb_SetRect(&humanSpear[6][0],1985,445,2048,518);
	tb_SetRect(&humanSpear[6][1],2049,445,2112,518);

	tb_SetRect(&humanSpear[7][0],1985,519,2048,592);
	tb_SetRect(&humanSpear[7][1],2049,519,2112,592);

	// --------------- bow frames
	tb_SetRect(&humanBow[0][0],2113,1,2176,74);
	tb_SetRect(&humanBow[0][1],2113,1,2176,74);

	tb_SetRect(&humanBow[1][0],2113,75,2176,148);
	tb_SetRect(&humanBow[1][1],2113,75,2176,148);

	tb_SetRect(&humanBow[2][0],2113,149,2176,222);
	tb_SetRect(&humanBow[2][1],2113,149,2176,222);

	tb_SetRect(&humanBow[3][0],2113,223,2176,296);
	tb_SetRect(&humanBow[3][1],2113,223,2176,296);

	tb_SetRect(&humanBow[4][0],2113,297,2176,370);
	tb_SetRect(&humanBow[4][1],2113,297,2176,370);

	tb_SetRect(&humanBow[5][0],2113,371,2176,444);
	tb_SetRect(&humanBow[5][1],2113,371,2176,444);

	tb_SetRect(&humanBow[6][0],2113,445,2176,518);
	tb_SetRect(&humanBow[6][1],2113,445,2176,518);

	tb_SetRect(&humanBow[7][0],2113,519,2176,592);
	tb_SetRect(&humanBow[7][1],2113,519,2176,592);

	// --------------- short sword frames
	tb_SetRect(&humanShortSword[0][0],2177,1,2240,74);
	tb_SetRect(&humanShortSword[0][1],2241,1,2304,74);

	tb_SetRect(&humanShortSword[1][0],2177,75,2240,148);
	tb_SetRect(&humanShortSword[1][1],2241,75,2304,148);

	tb_SetRect(&humanShortSword[2][0],2177,149,2240,222);
	tb_SetRect(&humanShortSword[2][1],2241,149,2304,222);

	tb_SetRect(&humanShortSword[3][0],2177,223,2240,296);
	tb_SetRect(&humanShortSword[3][1],2241,223,2304,296);

	tb_SetRect(&humanShortSword[4][0],2177,297,2240,370);
	tb_SetRect(&humanShortSword[4][1],2241,297,2304,370);

	tb_SetRect(&humanShortSword[5][0],2177,371,2240,444);
	tb_SetRect(&humanShortSword[5][1],2241,371,2304,444);

	tb_SetRect(&humanShortSword[6][0],2177,445,2240,518);
	tb_SetRect(&humanShortSword[6][1],2241,445,2304,518);

	tb_SetRect(&humanShortSword[7][0],2177,519,2240,592);
	tb_SetRect(&humanShortSword[7][1],2241,519,2304,592);
	
	
	// --------------- scepter frames   //Dave add 11-15-2012
	tb_SetRect(&humanSpect[0][0],257,1,321,74);
	tb_SetRect(&humanSpect[0][1],322,1,383,74);

	tb_SetRect(&humanSpect[1][0],257,75,321,148);
	tb_SetRect(&humanSpect[1][1],322,75,383,148);

	tb_SetRect(&humanSpect[2][0],257,149,321,222);
	tb_SetRect(&humanSpect[2][1],322,149,383,222);

	tb_SetRect(&humanSpect[3][0],257,223,321,296);
	tb_SetRect(&humanSpect[3][1],322,223,383,296);

	tb_SetRect(&humanSpect[4][0],257,297,321,370);
	tb_SetRect(&humanSpect[4][1],322,297,383,370);

	tb_SetRect(&humanSpect[5][0],257,371,321,444);
	tb_SetRect(&humanSpect[5][1],322,371,383,444);

	tb_SetRect(&humanSpect[6][0],257,445,321,518);
	tb_SetRect(&humanSpect[6][1],322,445,383,518);

	tb_SetRect(&humanSpect[7][0],257,519,321,592);
	tb_SetRect(&humanSpect[7][1],322,519,383,592);

	// --------------- slotted plate helmet frames
	tb_SetRect(&humanSlottedPlateHelmet[0],129,1,192,74);
	tb_SetRect(&humanSlottedPlateHelmet[1],129,75,192,148);
	tb_SetRect(&humanSlottedPlateHelmet[2],129,149,192,222);
	tb_SetRect(&humanSlottedPlateHelmet[3],129,223,192,296);
	tb_SetRect(&humanSlottedPlateHelmet[4],129,297,192,370);
	tb_SetRect(&humanSlottedPlateHelmet[5],129,371,192,444);
	tb_SetRect(&humanSlottedPlateHelmet[6],129,445,192,518);
	tb_SetRect(&humanSlottedPlateHelmet[7],129,519,192,592);

	// --------------- closed plate helmet frames
	tb_SetRect(&humanClosedPlateHelmet[0],65,1,128,74);
	tb_SetRect(&humanClosedPlateHelmet[1],65,75,128,148);
	tb_SetRect(&humanClosedPlateHelmet[2],65,149,128,222);
	tb_SetRect(&humanClosedPlateHelmet[3],65,223,128,296);
	tb_SetRect(&humanClosedPlateHelmet[4],65,297,128,370);
	tb_SetRect(&humanClosedPlateHelmet[5],65,371,128,444);
	tb_SetRect(&humanClosedPlateHelmet[6],65,445,128,518);
	tb_SetRect(&humanClosedPlateHelmet[7],65,519,128,592);

	// --------------- viking helmet frames
	tb_SetRect(&humanVikingHelmet[0],193,1,256,74);
	tb_SetRect(&humanVikingHelmet[1],193,75,256,148);
	tb_SetRect(&humanVikingHelmet[2],193,149,256,222);
	tb_SetRect(&humanVikingHelmet[3],193,223,256,296);
	tb_SetRect(&humanVikingHelmet[4],193,297,256,370);
	tb_SetRect(&humanVikingHelmet[5],193,371,256,444);
	tb_SetRect(&humanVikingHelmet[6],193,445,256,518);
	tb_SetRect(&humanVikingHelmet[7],193,519,256,592);

	// --------------- chain mail helmet frames
	tb_SetRect(&humanChainMailHelmet[0],1,1,64,74);
	tb_SetRect(&humanChainMailHelmet[1],1,75,64,148);
	tb_SetRect(&humanChainMailHelmet[2],1,149,64,222);
	tb_SetRect(&humanChainMailHelmet[3],1,223,64,296);
	tb_SetRect(&humanChainMailHelmet[4],1,297,64,370);
	tb_SetRect(&humanChainMailHelmet[5],1,371,64,444);
	tb_SetRect(&humanChainMailHelmet[6],1,445,64,518);
	tb_SetRect(&humanChainMailHelmet[7],1,519,64,592);

	// --------------- open plate helmet frames
	tb_SetRect(&humanOpenPlateHelmet[0],1,1,64,74);
	tb_SetRect(&humanOpenPlateHelmet[1],1,75,64,148);
	tb_SetRect(&humanOpenPlateHelmet[2],1,149,64,222);
	tb_SetRect(&humanOpenPlateHelmet[3],1,223,64,296);
	tb_SetRect(&humanOpenPlateHelmet[4],1,297,64,370);
	tb_SetRect(&humanOpenPlateHelmet[5],1,371,64,444);
	tb_SetRect(&humanOpenPlateHelmet[6],1,445,64,518);
	tb_SetRect(&humanOpenPlateHelmet[7],1,519,64,592);



	// --------------- slotted plate helmet frames
	tb_SetRect(&humanBlackSlottedPlateHelmet[0],385,1,448,74);
	tb_SetRect(&humanBlackSlottedPlateHelmet[1],385,75,448,148);
	tb_SetRect(&humanBlackSlottedPlateHelmet[2],385,149,448,222);
	tb_SetRect(&humanBlackSlottedPlateHelmet[3],385,223,448,296);
	tb_SetRect(&humanBlackSlottedPlateHelmet[4],385,297,448,370);
	tb_SetRect(&humanBlackSlottedPlateHelmet[5],385,371,448,444);
	tb_SetRect(&humanBlackSlottedPlateHelmet[6],385,445,448,518);
	tb_SetRect(&humanBlackSlottedPlateHelmet[7],385,519,448,592);

	// --------------- closed plate helmet frames
	tb_SetRect(&humanBlackClosedPlateHelmet[0],321,1,384,74);
	tb_SetRect(&humanBlackClosedPlateHelmet[1],321,75,384,148);
	tb_SetRect(&humanBlackClosedPlateHelmet[2],321,149,384,222);
	tb_SetRect(&humanBlackClosedPlateHelmet[3],321,223,384,296);
	tb_SetRect(&humanBlackClosedPlateHelmet[4],321,297,384,370);
	tb_SetRect(&humanBlackClosedPlateHelmet[5],321,371,384,444);
	tb_SetRect(&humanBlackClosedPlateHelmet[6],321,445,384,518);
	tb_SetRect(&humanBlackClosedPlateHelmet[7],321,519,384,592);

	// --------------- viking helmet frames
	tb_SetRect(&humanBlackVikingHelmet[0],449,1,512,74);
	tb_SetRect(&humanBlackVikingHelmet[1],449,75,512,148);
	tb_SetRect(&humanBlackVikingHelmet[2],449,149,512,222);
	tb_SetRect(&humanBlackVikingHelmet[3],449,223,512,296);
	tb_SetRect(&humanBlackVikingHelmet[4],449,297,512,370);
	tb_SetRect(&humanBlackVikingHelmet[5],449,371,512,444);
	tb_SetRect(&humanBlackVikingHelmet[6],449,445,512,518);
	tb_SetRect(&humanBlackVikingHelmet[7],449,519,512,592);

	// --------------- open plate helmet frames
	tb_SetRect(&humanBlackOpenPlateHelmet[0],257,1,320,74);
	tb_SetRect(&humanBlackOpenPlateHelmet[1],257,75,320,148);
	tb_SetRect(&humanBlackOpenPlateHelmet[2],257,149,320,222);
	tb_SetRect(&humanBlackOpenPlateHelmet[3],257,223,320,296);
	tb_SetRect(&humanBlackOpenPlateHelmet[4],257,297,320,370);
	tb_SetRect(&humanBlackOpenPlateHelmet[5],257,371,320,444);
	tb_SetRect(&humanBlackOpenPlateHelmet[6],257,445,320,518);
	tb_SetRect(&humanBlackOpenPlateHelmet[7],257,519,320,592);

	// --------------- wizard hat frames
	tb_SetRect(&humanWizardHat[0],513,1,576,74);
	tb_SetRect(&humanWizardHat[1],513,75,576,148);
	tb_SetRect(&humanWizardHat[2],513,149,576,222);
	tb_SetRect(&humanWizardHat[3],513,223,576,296);
	tb_SetRect(&humanWizardHat[4],513,297,576,370);
	tb_SetRect(&humanWizardHat[5],513,371,576,444);
	tb_SetRect(&humanWizardHat[6],513,445,576,518);
	tb_SetRect(&humanWizardHat[7],513,519,576,592);

	// --------------- jester hat frames
	tb_SetRect(&humanJesterHat[0],577,1,640,74);
	tb_SetRect(&humanJesterHat[1],577,75,640,148);
	tb_SetRect(&humanJesterHat[2],577,149,640,222);
	tb_SetRect(&humanJesterHat[3],577,223,640,296);
	tb_SetRect(&humanJesterHat[4],577,297,640,370);
	tb_SetRect(&humanJesterHat[5],577,371,640,444);
	tb_SetRect(&humanJesterHat[6],577,445,640,518);
	tb_SetRect(&humanJesterHat[7],577,519,640,592);

	// --------------- crown frames
	tb_SetRect(&humanCrown[0],641,1,704,74);
	tb_SetRect(&humanCrown[1],641,75,704,148);
	tb_SetRect(&humanCrown[2],641,149,704,222);
	tb_SetRect(&humanCrown[3],641,223,704,296);
	tb_SetRect(&humanCrown[4],641,297,704,370);
	tb_SetRect(&humanCrown[5],641,371,704,444);
	tb_SetRect(&humanCrown[6],641,445,704,518);
	tb_SetRect(&humanCrown[7],641,519,704,592);

	// --------------- Male hair frames
	tb_SetRect(&humanMaleHair[0][1],800,1,863,74);
	tb_SetRect(&humanMaleHair[1][1],800,75,863,148);
	tb_SetRect(&humanMaleHair[2][1],800,149,863,222);
	tb_SetRect(&humanMaleHair[3][1],800,223,863,296);
	tb_SetRect(&humanMaleHair[4][1],800,297,863,370);
	tb_SetRect(&humanMaleHair[5][1],800,371,863,444);
	tb_SetRect(&humanMaleHair[6][1],800,445,863,518);
	tb_SetRect(&humanMaleHair[7][1],800,519,863,592);


	// --------------- Female hair frames
	tb_SetRect(&humanFemaleHair[0][1],864,1,927,74);
	tb_SetRect(&humanFemaleHair[1][1],864,75,927,148);
	tb_SetRect(&humanFemaleHair[2][1],864,149,927,222);
	tb_SetRect(&humanFemaleHair[3][1],864,223,927,296);
	tb_SetRect(&humanFemaleHair[4][1],864,297,927,370);
	tb_SetRect(&humanFemaleHair[5][1],864,371,927,444);
	tb_SetRect(&humanFemaleHair[6][1],864,445,927,518);
	tb_SetRect(&humanFemaleHair[7][1],864,519,927,592);

	// --------------- breast plate frames
	tb_SetRect(&humanBreastPlate[0],1,1,64,74);
	tb_SetRect(&humanBreastPlate[1],1,75,64,148);
	tb_SetRect(&humanBreastPlate[2],1,149,64,222);
	tb_SetRect(&humanBreastPlate[3],1,223,64,296);
	tb_SetRect(&humanBreastPlate[4],1,297,64,370);
	tb_SetRect(&humanBreastPlate[5],1,371,64,444);
	tb_SetRect(&humanBreastPlate[6],1,445,64,518);
	tb_SetRect(&humanBreastPlate[7],1,519,64,592);

	// --------------- black breast plate frames
	tb_SetRect(&humanBlackBreastPlate[0],65,1,128,74);
	tb_SetRect(&humanBlackBreastPlate[1],65,75,128,148);
	tb_SetRect(&humanBlackBreastPlate[2],65,149,128,222);
	tb_SetRect(&humanBlackBreastPlate[3],65,223,128,296);
	tb_SetRect(&humanBlackBreastPlate[4],65,297,128,370);
	tb_SetRect(&humanBlackBreastPlate[5],65,371,128,444);
	tb_SetRect(&humanBlackBreastPlate[6],65,445,128,518);
	tb_SetRect(&humanBlackBreastPlate[7],65,519,128,592);

	// --------------- breast leather frames
	tb_SetRect(&humanBrownBreastLeather[0],193,1,256,74);
	tb_SetRect(&humanBrownBreastLeather[1],193,75,256,148);
	tb_SetRect(&humanBrownBreastLeather[2],193,149,256,222);
	tb_SetRect(&humanBrownBreastLeather[3],193,223,256,296);
	tb_SetRect(&humanBrownBreastLeather[4],193,297,256,370);
	tb_SetRect(&humanBrownBreastLeather[5],193,371,256,444);
	tb_SetRect(&humanBrownBreastLeather[6],193,445,256,518);
	tb_SetRect(&humanBrownBreastLeather[7],193,519,256,592);

	// --------------- breast leather frames
	tb_SetRect(&humanBreastLeather[0],129,1,192,74);
	tb_SetRect(&humanBreastLeather[1],129,75,192,148);
	tb_SetRect(&humanBreastLeather[2],129,149,192,222);
	tb_SetRect(&humanBreastLeather[3],129,223,192,296);
	tb_SetRect(&humanBreastLeather[4],129,297,192,370);
	tb_SetRect(&humanBreastLeather[5],129,371,192,444);
	tb_SetRect(&humanBreastLeather[6],129,445,192,518);
	tb_SetRect(&humanBreastLeather[7],129,519,192,592);

	// --------------- breast chain frames
	tb_SetRect(&humanBreastChain[0],257,1,320,74);
	tb_SetRect(&humanBreastChain[1],257,75,320,148);
	tb_SetRect(&humanBreastChain[2],257,149,320,222);
	tb_SetRect(&humanBreastChain[3],257,223,320,296);
	tb_SetRect(&humanBreastChain[4],257,297,320,370);
	tb_SetRect(&humanBreastChain[5],257,371,320,444);
	tb_SetRect(&humanBreastChain[6],257,445,320,518);
	tb_SetRect(&humanBreastChain[7],257,519,320,592);

	// --------------- plate legs frames
	tb_SetRect(&humanLegsPlate[0][0],321,1,384,74);
	tb_SetRect(&humanLegsPlate[0][1],385,1,448,74);
	tb_SetRect(&humanLegsPlate[0][2],321,1,384,74);

	tb_SetRect(&humanLegsPlate[1][0],321,75,384,148);
	tb_SetRect(&humanLegsPlate[1][1],385,75,448,148);
	tb_SetRect(&humanLegsPlate[1][2],321,75,384,148);

	tb_SetRect(&humanLegsPlate[2][0],321,149,384,222);
	tb_SetRect(&humanLegsPlate[2][1],385,149,448,222);
	tb_SetRect(&humanLegsPlate[2][2],321,149,384,222);

	tb_SetRect(&humanLegsPlate[3][0],321,223,384,296);
	tb_SetRect(&humanLegsPlate[3][1],385,223,448,296);
	tb_SetRect(&humanLegsPlate[3][2],321,223,384,296);

	tb_SetRect(&humanLegsPlate[4][0],321,297,384,370);
	tb_SetRect(&humanLegsPlate[4][1],385,297,448,370);
	tb_SetRect(&humanLegsPlate[4][2],321,297,384,370);

	tb_SetRect(&humanLegsPlate[5][0],321,371,384,444);
	tb_SetRect(&humanLegsPlate[5][1],385,371,448,444);
	tb_SetRect(&humanLegsPlate[5][2],321,371,384,444);

	tb_SetRect(&humanLegsPlate[6][0],321,445,384,518);
	tb_SetRect(&humanLegsPlate[6][1],385,445,448,518);
	tb_SetRect(&humanLegsPlate[6][2],321,445,384,518);

	tb_SetRect(&humanLegsPlate[7][0],321,519,384,592);
	tb_SetRect(&humanLegsPlate[7][1],385,519,448,592);
	tb_SetRect(&humanLegsPlate[7][2],321,519,384,592);

	// ---------------plate black legs frames
	tb_SetRect(&humanBlackLegsPlate[0][0],449,1,512,74);
	tb_SetRect(&humanBlackLegsPlate[0][1],513,1,576,74);
	tb_SetRect(&humanBlackLegsPlate[0][2],449,1,512,74);

	tb_SetRect(&humanBlackLegsPlate[1][0],449,75,512,148);
	tb_SetRect(&humanBlackLegsPlate[1][1],513,75,576,148);
	tb_SetRect(&humanBlackLegsPlate[1][2],449,75,512,148);

	tb_SetRect(&humanBlackLegsPlate[2][0],449,149,512,222);
	tb_SetRect(&humanBlackLegsPlate[2][1],513,149,576,222);
	tb_SetRect(&humanBlackLegsPlate[2][2],449,149,512,222);

	tb_SetRect(&humanBlackLegsPlate[3][0],449,223,512,296);
	tb_SetRect(&humanBlackLegsPlate[3][1],513,223,576,296);
	tb_SetRect(&humanBlackLegsPlate[3][2],449,223,512,296);

	tb_SetRect(&humanBlackLegsPlate[4][0],449,297,512,370);
	tb_SetRect(&humanBlackLegsPlate[4][1],513,297,576,370);
	tb_SetRect(&humanBlackLegsPlate[4][2],449,297,512,370);

	tb_SetRect(&humanBlackLegsPlate[5][0],449,371,512,444);
	tb_SetRect(&humanBlackLegsPlate[5][1],513,371,576,444);
	tb_SetRect(&humanBlackLegsPlate[5][2],449,371,512,444);

	tb_SetRect(&humanBlackLegsPlate[6][0],449,445,512,518);
	tb_SetRect(&humanBlackLegsPlate[6][1],513,445,576,518);
	tb_SetRect(&humanBlackLegsPlate[6][2],449,445,512,518);

	tb_SetRect(&humanBlackLegsPlate[7][0],449,519,512,592);
	tb_SetRect(&humanBlackLegsPlate[7][1],513,519,576,592);
	tb_SetRect(&humanBlackLegsPlate[7][2],449,519,512,592);

	// --------------- chain legs frames
	tb_SetRect(&humanLegsChain[0][0],961,1,1024,74);
	tb_SetRect(&humanLegsChain[0][1],1025,1,1088,74);
	tb_SetRect(&humanLegsChain[0][2],961,1,1024,74);

	tb_SetRect(&humanLegsChain[1][0],961,75,1024,148);
	tb_SetRect(&humanLegsChain[1][1],1025,75,1088,148);
	tb_SetRect(&humanLegsChain[1][2],961,75,1024,148);

	tb_SetRect(&humanLegsChain[2][0],961,149,1024,222);
	tb_SetRect(&humanLegsChain[2][1],1025,149,1088,222);
	tb_SetRect(&humanLegsChain[2][2],961,149,1024,222);

	tb_SetRect(&humanLegsChain[3][0],961,223,1024,296);
	tb_SetRect(&humanLegsChain[3][1],1025,223,1088,296);
	tb_SetRect(&humanLegsChain[3][2],961,223,1024,296);

	tb_SetRect(&humanLegsChain[4][0],961,297,1024,370);
	tb_SetRect(&humanLegsChain[4][1],1025,297,1088,370);
	tb_SetRect(&humanLegsChain[4][2],961,297,1024,370);

	tb_SetRect(&humanLegsChain[5][0],961,371,1024,444);
	tb_SetRect(&humanLegsChain[5][1],1025,371,1088,444);
	tb_SetRect(&humanLegsChain[5][2],961,371,1024,444);

	tb_SetRect(&humanLegsChain[6][0],961,445,1024,518);
	tb_SetRect(&humanLegsChain[6][1],1025,445,1088,518);
	tb_SetRect(&humanLegsChain[6][2],961,445,1024,518);

	tb_SetRect(&humanLegsChain[7][0],961,519,1024,592);
	tb_SetRect(&humanLegsChain[7][1],1025,519,1088,592);
	tb_SetRect(&humanLegsChain[7][2],961,519,1024,592);

	// --------------- leather legs frames
	tb_SetRect(&humanLegsLeather[0][0],1089,1,1152,74);
	tb_SetRect(&humanLegsLeather[0][1],1153,1,1216,74);
	tb_SetRect(&humanLegsLeather[0][2],1089,1,1152,74);

	tb_SetRect(&humanLegsLeather[1][0],1089,75,1152,148);
	tb_SetRect(&humanLegsLeather[1][1],1153,75,1216,148);
	tb_SetRect(&humanLegsLeather[1][2],1089,75,1152,148);

	tb_SetRect(&humanLegsLeather[2][0],1089,149,1152,222);
	tb_SetRect(&humanLegsLeather[2][1],1153,149,1216,222);
	tb_SetRect(&humanLegsLeather[2][2],1089,149,1152,222);

	tb_SetRect(&humanLegsLeather[3][0],1089,223,1152,296);
	tb_SetRect(&humanLegsLeather[3][1],1153,223,1216,296);
	tb_SetRect(&humanLegsLeather[3][2],1089,223,1152,296);

	tb_SetRect(&humanLegsLeather[4][0],1089,297,1152,370);
	tb_SetRect(&humanLegsLeather[4][1],1153,297,1216,370);
	tb_SetRect(&humanLegsLeather[4][2],1089,297,1152,370);

	tb_SetRect(&humanLegsLeather[5][0],1089,371,1152,444);
	tb_SetRect(&humanLegsLeather[5][1],1153,371,1216,444);
	tb_SetRect(&humanLegsLeather[5][2],1089,371,1152,444);

	tb_SetRect(&humanLegsLeather[6][0],1089,445,1152,518);
	tb_SetRect(&humanLegsLeather[6][1],1153,445,1216,518);
	tb_SetRect(&humanLegsLeather[6][2],1089,445,1152,518);

	tb_SetRect(&humanLegsLeather[7][0],1089,519,1152,592);
	tb_SetRect(&humanLegsLeather[7][1],1153,519,1216,592);
	tb_SetRect(&humanLegsLeather[7][2],1089,519,1152,592);

	// --------------- medal shield frames
	tb_SetRect(&humanMedalShield[0],577,1,640,74);
	tb_SetRect(&humanMedalShield[1],577,75,640,148);
	tb_SetRect(&humanMedalShield[2],577,149,640,222);
	tb_SetRect(&humanMedalShield[3],577,223,640,296);
	tb_SetRect(&humanMedalShield[4],577,297,640,370);
	tb_SetRect(&humanMedalShield[5],577,371,640,444);
	tb_SetRect(&humanMedalShield[6],577,445,640,518);
	tb_SetRect(&humanMedalShield[7],577,519,640,592);

	// --------------- wooden shield frames
	tb_SetRect(&humanWoodenShield[0],897,1,960,74);
	tb_SetRect(&humanWoodenShield[1],897,75,960,148);
	tb_SetRect(&humanWoodenShield[2],897,149,960,222);
	tb_SetRect(&humanWoodenShield[3],897,223,960,296);
	tb_SetRect(&humanWoodenShield[4],897,297,960,370);
	tb_SetRect(&humanWoodenShield[5],897,371,960,444);
	tb_SetRect(&humanWoodenShield[6],897,445,960,518);
	tb_SetRect(&humanWoodenShield[7],897,519,960,592);

	// ---------------plate black legs frames
	tb_SetRect(&humanBoots[0][0],641,1,704,74);
	tb_SetRect(&humanBoots[0][1],705,1,768,74);
	tb_SetRect(&humanBoots[0][2],641,1,704,74);

	tb_SetRect(&humanBoots[1][0],641,75,704,148);
	tb_SetRect(&humanBoots[1][1],705,75,768,148);
	tb_SetRect(&humanBoots[1][2],641,75,704,148);

	tb_SetRect(&humanBoots[2][0],641,149,704,222);
	tb_SetRect(&humanBoots[2][1],705,149,768,222);
	tb_SetRect(&humanBoots[2][2],641,149,704,222);

	tb_SetRect(&humanBoots[3][0],641,223,704,296);
	tb_SetRect(&humanBoots[3][1],705,223,768,296);
	tb_SetRect(&humanBoots[3][2],641,223,704,296);

	tb_SetRect(&humanBoots[4][0],641,297,704,370);
	tb_SetRect(&humanBoots[4][1],705,297,768,370);
	tb_SetRect(&humanBoots[4][2],641,297,704,370);

	tb_SetRect(&humanBoots[5][0],641,371,704,444);
	tb_SetRect(&humanBoots[5][1],705,371,768,444);
	tb_SetRect(&humanBoots[5][2],641,371,704,444);

	tb_SetRect(&humanBoots[6][0],641,445,704,518);
	tb_SetRect(&humanBoots[6][1],705,445,768,518);
	tb_SetRect(&humanBoots[6][2],641,445,704,518);

	tb_SetRect(&humanBoots[7][0],641,519,704,592);
	tb_SetRect(&humanBoots[7][1],705,519,768,592);
	tb_SetRect(&humanBoots[7][2],641,519,704,592);

	// ---------------bare human arm frames
	tb_SetRect(&humanArms[0][0],769,1,832,74);
	tb_SetRect(&humanArms[0][2],833,1,896,74);
	tb_SetRect(&humanArms[0][1],769,1,832,74);

	tb_SetRect(&humanArms[1][0],769,75,832,148);
	tb_SetRect(&humanArms[1][2],833,75,896,148);
	tb_SetRect(&humanArms[1][1],769,75,832,148);

	tb_SetRect(&humanArms[2][0],769,149,832,222);
	tb_SetRect(&humanArms[2][2],833,149,896,222);
	tb_SetRect(&humanArms[2][1],769,149,832,222);

	tb_SetRect(&humanArms[3][0],769,223,832,296);
	tb_SetRect(&humanArms[3][2],833,223,896,296);
	tb_SetRect(&humanArms[3][1],769,223,832,296);

	tb_SetRect(&humanArms[4][0],769,297,832,370);
	tb_SetRect(&humanArms[4][2],833,297,896,370);
	tb_SetRect(&humanArms[4][1],769,297,832,370);

	tb_SetRect(&humanArms[5][0],769,371,832,444);
	tb_SetRect(&humanArms[5][2],833,371,896,444);
	tb_SetRect(&humanArms[5][1],769,371,832,444);

	tb_SetRect(&humanArms[6][0],769,445,832,518);
	tb_SetRect(&humanArms[6][2],833,445,896,518);
	tb_SetRect(&humanArms[6][1],769,445,832,518);

	tb_SetRect(&humanArms[7][0],769,519,832,592);
	tb_SetRect(&humanArms[7][2],833,519,896,592);
	tb_SetRect(&humanArms[7][1],769,519,832,592);

	// ---------------human gray arm frames
	tb_SetRect(&humanGrayArms[0][0],1217,1,1280,74);
	tb_SetRect(&humanGrayArms[0][2],1281,1,1344,74);
	tb_SetRect(&humanGrayArms[0][1],1217,1,1280,74);

	tb_SetRect(&humanGrayArms[1][0],1217,75,1280,148);
	tb_SetRect(&humanGrayArms[1][2],1281,75,1344,148);
	tb_SetRect(&humanGrayArms[1][1],1217,75,1280,148);

	tb_SetRect(&humanGrayArms[2][0],1217,149,1280,222);
	tb_SetRect(&humanGrayArms[2][2],1281,149,1344,222);
	tb_SetRect(&humanGrayArms[2][1],1217,149,1280,222);

	tb_SetRect(&humanGrayArms[3][0],1217,223,1280,296);
	tb_SetRect(&humanGrayArms[3][2],1281,223,1344,296);
	tb_SetRect(&humanGrayArms[3][1],1217,223,1280,296);

	tb_SetRect(&humanGrayArms[4][0],1217,297,1280,370);
	tb_SetRect(&humanGrayArms[4][2],1281,297,1344,370);
	tb_SetRect(&humanGrayArms[4][1],1217,297,1280,370);

	tb_SetRect(&humanGrayArms[5][0],1217,371,1280,444);
	tb_SetRect(&humanGrayArms[5][2],1281,371,1344,444);
	tb_SetRect(&humanGrayArms[5][1],1217,371,1280,444);

	tb_SetRect(&humanGrayArms[6][0],1217,445,1280,518);
	tb_SetRect(&humanGrayArms[6][2],1281,445,1344,518);
	tb_SetRect(&humanGrayArms[6][1],1217,445,1280,518);

	tb_SetRect(&humanGrayArms[7][0],1217,519,1280,592);
	tb_SetRect(&humanGrayArms[7][2],1281,519,1344,592);
	tb_SetRect(&humanGrayArms[7][1],1217,519,1280,592);

	// ---------------human brown arm frames
	tb_SetRect(&humanBrownArms[0][0],1345,1,1408,74);
	tb_SetRect(&humanBrownArms[0][2],1409,1,1472,74);
	tb_SetRect(&humanBrownArms[0][1],1345,1,1408,74);

	tb_SetRect(&humanBrownArms[1][0],1345,75,1408,148);
	tb_SetRect(&humanBrownArms[1][2],1409,75,1472,148);
	tb_SetRect(&humanBrownArms[1][1],1345,75,1408,148);

	tb_SetRect(&humanBrownArms[2][0],1345,149,1408,222);
	tb_SetRect(&humanBrownArms[2][2],1409,149,1472,222);
	tb_SetRect(&humanBrownArms[2][1],1345,149,1408,222);

	tb_SetRect(&humanBrownArms[3][0],1345,223,1408,296);
	tb_SetRect(&humanBrownArms[3][2],1409,223,1472,296);
	tb_SetRect(&humanBrownArms[3][1],1345,223,1408,296);

	tb_SetRect(&humanBrownArms[4][0],1345,297,1408,370);
	tb_SetRect(&humanBrownArms[4][2],1409,297,1472,370);
	tb_SetRect(&humanBrownArms[4][1],1345,297,1408,370);

	tb_SetRect(&humanBrownArms[5][0],1345,371,1408,444);
	tb_SetRect(&humanBrownArms[5][2],1409,371,1472,444);
	tb_SetRect(&humanBrownArms[5][1],1345,371,1408,444);

	tb_SetRect(&humanBrownArms[6][0],1345,445,1408,518);
	tb_SetRect(&humanBrownArms[6][2],1409,445,1472,518);
	tb_SetRect(&humanBrownArms[6][1],1345,445,1408,518);

	tb_SetRect(&humanBrownArms[7][0],1345,519,1408,592);
	tb_SetRect(&humanBrownArms[7][2],1409,519,1472,592);
	tb_SetRect(&humanBrownArms[7][1],1345,519,1408,592);

	// ---------------human black arm frames
	tb_SetRect(&humanBlackArms[0][0],1473,1,1536,74);
	tb_SetRect(&humanBlackArms[0][2],1537,1,1600,74);
	tb_SetRect(&humanBlackArms[0][1],1473,1,1536,74);

	tb_SetRect(&humanBlackArms[1][0],1473,75,1536,148);
	tb_SetRect(&humanBlackArms[1][2],1537,75,1600,148);
	tb_SetRect(&humanBlackArms[1][1],1473,75,1536,148);

	tb_SetRect(&humanBlackArms[2][0],1473,149,1536,222);
	tb_SetRect(&humanBlackArms[2][2],1537,149,1600,222);
	tb_SetRect(&humanBlackArms[2][1],1473,149,1536,222);

	tb_SetRect(&humanBlackArms[3][0],1473,223,1536,296);
	tb_SetRect(&humanBlackArms[3][2],1537,223,1600,296);
	tb_SetRect(&humanBlackArms[3][1],1473,223,1536,296);

	tb_SetRect(&humanBlackArms[4][0],1473,297,1536,370);
	tb_SetRect(&humanBlackArms[4][2],1537,297,1600,370);
	tb_SetRect(&humanBlackArms[4][1],1473,297,1536,370);

	tb_SetRect(&humanBlackArms[5][0],1473,371,1536,444);
	tb_SetRect(&humanBlackArms[5][2],1537,371,1600,444);
	tb_SetRect(&humanBlackArms[5][1],1473,371,1536,444);

	tb_SetRect(&humanBlackArms[6][0],1473,445,1536,518);
	tb_SetRect(&humanBlackArms[6][2],1537,445,1600,518);
	tb_SetRect(&humanBlackArms[6][1],1473,445,1536,518);

	tb_SetRect(&humanBlackArms[7][0],1473,519,1536,592);
	tb_SetRect(&humanBlackArms[7][2],1537,519,1600,592);
	tb_SetRect(&humanBlackArms[7][1],1473,519,1536,592);

	//creatureRect[5][8][3];
	tb_SetRect(&creatureRect[1][0][0],   1,   87+31, 	58,    143+31); // sea turtle
	tb_SetRect(&creatureRect[1][0][1],  59,   87+31, 	116,   143+31); // sea turtle
	tb_SetRect(&creatureRect[1][0][2],  117,  87+31, 	174,   143+31); // sea turtle

	tb_SetRect(&creatureRect[1][1][0],   1,   144+31, 	58,    200+31); // sea turtle
	tb_SetRect(&creatureRect[1][1][1],  59,   144+31, 	116,   200+31); // sea turtle
	tb_SetRect(&creatureRect[1][1][2],  117,  144+31, 	174,   200+31); // sea turtle

	tb_SetRect(&creatureRect[1][2][0],   1,   201+31, 	58,    257+31); // sea turtle
	tb_SetRect(&creatureRect[1][2][1],  59,   201+31, 	116,   257+31); // sea turtle
	tb_SetRect(&creatureRect[1][2][2],  117,  201+31, 	174,   257+31); // sea turtle

	tb_SetRect(&creatureRect[1][3][0],   1,   258+31, 	58,    314+31); // sea turtle
	tb_SetRect(&creatureRect[1][3][1],  59,   258+31, 	116,   314+31); // sea turtle
	tb_SetRect(&creatureRect[1][3][2],  117,  258+31, 	174,   314+31); // sea turtle

	tb_SetRect(&creatureRect[1][4][0],   1,   315+31, 	58,    371+31); // sea turtle
	tb_SetRect(&creatureRect[1][4][1],  59,   315+31, 	116,   371+31); // sea turtle
	tb_SetRect(&creatureRect[1][4][2],  117,  315+31, 	174,   371+31); // sea turtle

	tb_SetRect(&creatureRect[1][5][0],   1,   372+31, 	58,    428+31); // sea turtle
	tb_SetRect(&creatureRect[1][5][1],  59,   372+31, 	116,   428+31); // sea turtle
	tb_SetRect(&creatureRect[1][5][2],  117,  372+31, 	174,   428+31); // sea turtle

	tb_SetRect(&creatureRect[1][6][0],   1,   429+31, 	58,    485+31); // sea turtle
	tb_SetRect(&creatureRect[1][6][1],  59,   429+31, 	116,   485+31); // sea turtle
	tb_SetRect(&creatureRect[1][6][2],  117,  429+31, 	174,   485+31); // sea turtle

	tb_SetRect(&creatureRect[1][7][0],   1,   486+31, 	58,    542+31); // sea turtle
	tb_SetRect(&creatureRect[1][7][1],  59,   486+31, 	116,   542+31); // sea turtle
	tb_SetRect(&creatureRect[1][7][2],  117,  486+31, 	174,   542+31); // sea turtle

	// ---------------lizard man frames
	tb_SetRect(&lizard[0][0],1510,1,1643,113);
	tb_SetRect(&lizard[0][1],1644,1,1777,113);
	tb_SetRect(&lizard[0][2],1778,1,1911,113);

	tb_SetRect(&lizard[1][0],1510,114,1643,226);
	tb_SetRect(&lizard[1][1],1644,114,1777,226);
	tb_SetRect(&lizard[1][2],1778,114,1911,226);

	tb_SetRect(&lizard[2][0],1510,227,1643,339);
	tb_SetRect(&lizard[2][1],1644,227,1777,339);
	tb_SetRect(&lizard[2][2],1778,227,1911,339);

	tb_SetRect(&lizard[3][0],1510,340,1643,452);
	tb_SetRect(&lizard[3][1],1644,340,1777,452);
	tb_SetRect(&lizard[3][2],1778,340,1911,452);

	tb_SetRect(&lizard[4][0],1510,453,1643,565);
	tb_SetRect(&lizard[4][1],1644,453,1777,565);
	tb_SetRect(&lizard[4][2],1778,453,1911,565);

	tb_SetRect(&lizard[5][0],1510,566,1643,678);
	tb_SetRect(&lizard[5][1],1644,566,1777,678);
	tb_SetRect(&lizard[5][2],1778,566,1911,678);

	tb_SetRect(&lizard[6][0],1510,679,1643,791);
	tb_SetRect(&lizard[6][1],1644,679,1777,791);
	tb_SetRect(&lizard[6][2],1778,679,1911,791);

	tb_SetRect(&lizard[7][0],1510,792,1643,904);
	tb_SetRect(&lizard[7][1],1644,792,1777,904);
	tb_SetRect(&lizard[7][2],1778,792,1911,904);

	// ---------------skeleton frames
	tb_SetRect(&skeleton[0][0],175,118,238,214);
	tb_SetRect(&skeleton[0][1],239,118,302,214);
	tb_SetRect(&skeleton[0][2],303,118,366,214);

	tb_SetRect(&skeleton[1][0],175,215,238,311);
	tb_SetRect(&skeleton[1][1],239,215,302,311);
	tb_SetRect(&skeleton[1][2],303,215,366,311);

	tb_SetRect(&skeleton[2][0],175,312,238,408);
	tb_SetRect(&skeleton[2][1],239,312,302,408);
	tb_SetRect(&skeleton[2][2],303,312,366,408);

	tb_SetRect(&skeleton[3][0],175,409,238,505);
	tb_SetRect(&skeleton[3][1],239,409,302,505);
	tb_SetRect(&skeleton[3][2],303,409,366,505);

	tb_SetRect(&skeleton[4][0],175,506,238,602);
	tb_SetRect(&skeleton[4][1],239,506,302,602);
	tb_SetRect(&skeleton[4][2],303,506,366,602);

	tb_SetRect(&skeleton[5][0],175,603,238,699);
	tb_SetRect(&skeleton[5][1],239,603,302,699);
	tb_SetRect(&skeleton[5][2],303,603,366,699);

	tb_SetRect(&skeleton[6][0],175,700,238,796);
	tb_SetRect(&skeleton[6][1],239,700,302,796);
	tb_SetRect(&skeleton[6][2],303,700,366,796);

	tb_SetRect(&skeleton[7][0],175,797,238,893);
	tb_SetRect(&skeleton[7][1],239,797,302,893);
	tb_SetRect(&skeleton[7][2],303,797,366,893);
	
	
	// ---------------skeleton archer frames
	tb_SetRect(&skeletonarcher[0][0],335,0,397,95);
	tb_SetRect(&skeletonarcher[0][1],399,0,461,95);
	tb_SetRect(&skeletonarcher[0][2],463,0,525,95);

	tb_SetRect(&skeletonarcher[1][0],335,97,397,192);
	tb_SetRect(&skeletonarcher[1][1],399,97,461,192);
	tb_SetRect(&skeletonarcher[1][2],463,97,525,192);

	tb_SetRect(&skeletonarcher[2][0],335,194,397,289);
	tb_SetRect(&skeletonarcher[2][1],399,194,461,289);
	tb_SetRect(&skeletonarcher[2][2],463,194,525,289);

	tb_SetRect(&skeletonarcher[3][0],335,291,397,386);
	tb_SetRect(&skeletonarcher[3][1],399,291,461,386);
	tb_SetRect(&skeletonarcher[3][2],463,291,525,386);

	tb_SetRect(&skeletonarcher[4][0],335,388,397,483);
	tb_SetRect(&skeletonarcher[4][1],399,388,461,483);
	tb_SetRect(&skeletonarcher[4][2],463,388,525,483);

	tb_SetRect(&skeletonarcher[5][0],335,485,397,580);
	tb_SetRect(&skeletonarcher[5][1],399,485,461,580);
	tb_SetRect(&skeletonarcher[5][2],463,485,525,580);

	tb_SetRect(&skeletonarcher[6][0],335,582,397,677);
	tb_SetRect(&skeletonarcher[6][1],399,582,461,677);
	tb_SetRect(&skeletonarcher[6][2],463,582,525,677);

	tb_SetRect(&skeletonarcher[7][0],335,679,397,774);
	tb_SetRect(&skeletonarcher[7][1],399,679,461,774);
	tb_SetRect(&skeletonarcher[7][2],463,679,525,774);


	// ---------------bone mage frames
	tb_SetRect(&boneMage[0][0],1318,118,1381,214);
	tb_SetRect(&boneMage[0][1],1382,118,1445,214);
	tb_SetRect(&boneMage[0][2],1446,118,1509,214);

	tb_SetRect(&boneMage[1][0],1318,215,1381,311);
	tb_SetRect(&boneMage[1][1],1382,215,1445,311);
	tb_SetRect(&boneMage[1][2],1446,215,1509,311);

	tb_SetRect(&boneMage[2][0],1318,312,1381,408);
	tb_SetRect(&boneMage[2][1],1382,312,1445,408);
	tb_SetRect(&boneMage[2][2],1446,312,1509,408);

	tb_SetRect(&boneMage[3][0],1318,409,1381,505);
	tb_SetRect(&boneMage[3][1],1382,409,1445,505);
	tb_SetRect(&boneMage[3][2],1446,409,1509,505);

	tb_SetRect(&boneMage[4][0],1318,506,1381,602);
	tb_SetRect(&boneMage[4][1],1382,506,1445,602);
	tb_SetRect(&boneMage[4][2],1446,506,1509,602);

	tb_SetRect(&boneMage[5][0],1318,603,1381,699);
	tb_SetRect(&boneMage[5][1],1382,603,1445,699);
	tb_SetRect(&boneMage[5][2],1446,603,1509,699);

	tb_SetRect(&boneMage[6][0],1318,700,1381,796);
	tb_SetRect(&boneMage[6][1],1382,700,1445,796);
	tb_SetRect(&boneMage[6][2],1446,700,1509,796);

	tb_SetRect(&boneMage[7][0],1318,797,1381,893);
	tb_SetRect(&boneMage[7][1],1382,797,1445,893);
	tb_SetRect(&boneMage[7][2],1446,797,1509,893);

	// ---------------harpie frames
	tb_SetRect(&harpie[0][0],129,894,220,1007);
	tb_SetRect(&harpie[0][1],221,894,312,1007);

	tb_SetRect(&harpie[1][0],607,924,698,1037);
	tb_SetRect(&harpie[1][1],699,924,790,1037);

	tb_SetRect(&harpie[2][0],889,553,980,666);
	tb_SetRect(&harpie[2][1],981,553,1072,666);

	tb_SetRect(&harpie[3][0],889,667,980,780);
	tb_SetRect(&harpie[3][1],981,667,1072,780);

	tb_SetRect(&harpie[4][0],889,781,980,894);
	tb_SetRect(&harpie[4][1],981,781,1072,894);

	tb_SetRect(&harpie[5][0],889,895,980,1008);
	tb_SetRect(&harpie[5][1],981,895,1072,1008);

	tb_SetRect(&harpie[6][0],1073,617,1164,730);
	tb_SetRect(&harpie[6][1],1165,617,1256,730);

	tb_SetRect(&harpie[7][0],1073,731,1164,844);
	tb_SetRect(&harpie[7][1],1165,731,1256,844);

	// ---------------scorpion frames
	tb_SetRect(&scorpion[0][0],1096,1,1169,77);
	tb_SetRect(&scorpion[0][1],1170,1,1243,77);
	tb_SetRect(&scorpion[0][2],1244,1,1317,77);

	tb_SetRect(&scorpion[1][0],1096,78,1169,154);
	tb_SetRect(&scorpion[1][1],1170,78,1243,154);
	tb_SetRect(&scorpion[1][2],1244,78,1317,154);

	tb_SetRect(&scorpion[2][0],1096,155,1169,231);
	tb_SetRect(&scorpion[2][1],1170,155,1243,231);
	tb_SetRect(&scorpion[2][2],1244,155,1317,231);

	tb_SetRect(&scorpion[3][0],1096,232,1169,308);
	tb_SetRect(&scorpion[3][1],1170,232,1243,308);
	tb_SetRect(&scorpion[3][2],1244,232,1317,308);

	tb_SetRect(&scorpion[4][0],1096,309,1169,385);
	tb_SetRect(&scorpion[4][1],1170,309,1243,385);
	tb_SetRect(&scorpion[4][2],1244,309,1317,385);

	tb_SetRect(&scorpion[5][0],1096,386,1169,462);
	tb_SetRect(&scorpion[5][1],1170,386,1243,462);
	tb_SetRect(&scorpion[5][2],1244,386,1317,462);

	tb_SetRect(&scorpion[6][0],1096,463,1169,539);
	tb_SetRect(&scorpion[6][1],1170,463,1243,539);
	tb_SetRect(&scorpion[6][2],1244,463,1317,539);

	tb_SetRect(&scorpion[7][0],1096,540,1169,616);
	tb_SetRect(&scorpion[7][1],1170,540,1243,616);
	tb_SetRect(&scorpion[7][2],1244,540,1317,616);

	// ---------------ant frames
	tb_SetRect(&ant[0][1],886,1,962,61);
	tb_SetRect(&ant[0][0],963,1,1039,61);
	tb_SetRect(&ant[0][2],1040,1,1116,61);

	tb_SetRect(&ant[1][1],886,62,962,122);
	tb_SetRect(&ant[1][0],963,62,1039,122);
	tb_SetRect(&ant[1][2],1040,62,1116,122);

	tb_SetRect(&ant[2][1],886,123,962,183);
	tb_SetRect(&ant[2][0],963,123,1039,183);
	tb_SetRect(&ant[2][2],1040,123,1116,183);

	tb_SetRect(&ant[3][1],886,184,962,244);
	tb_SetRect(&ant[3][0],963,184,1039,244);
	tb_SetRect(&ant[3][2],1040,184,1116,244);

	tb_SetRect(&ant[4][1],886,245,962,305);
	tb_SetRect(&ant[4][0],963,245,1039,305);
	tb_SetRect(&ant[4][2],1040,245,1116,305);

	tb_SetRect(&ant[5][1],886,306,962,366);
	tb_SetRect(&ant[5][0],963,306,1039,366);
	tb_SetRect(&ant[5][2],1040,306,1116,366);

	tb_SetRect(&ant[6][1],886,367,962,427);
	tb_SetRect(&ant[6][0],963,367,1039,427);
	tb_SetRect(&ant[6][2],1040,367,1116,427);

	tb_SetRect(&ant[7][1],886,428,962,498);
	tb_SetRect(&ant[7][0],963,428,1039,498);
	tb_SetRect(&ant[7][2],1040,428,1116,498);

	// ---------------queen ant frames
	tb_SetRect(&queenant[0][1],361,19,460,119);
	tb_SetRect(&queenant[0][0],462,19,562,119);
	tb_SetRect(&queenant[0][2],564,19,662,119);

	tb_SetRect(&queenant[1][1],361,120,460,220);
	tb_SetRect(&queenant[1][0],462,120,562,220);
	tb_SetRect(&queenant[1][2],564,120,662,220);

	tb_SetRect(&queenant[2][1],361,221,460,321);
	tb_SetRect(&queenant[2][0],462,221,562,321);
	tb_SetRect(&queenant[2][2],564,221,662,321);

	tb_SetRect(&queenant[3][1],361,322,460,422);
	tb_SetRect(&queenant[3][0],462,322,562,422);
	tb_SetRect(&queenant[3][2],564,322,662,422);

	tb_SetRect(&queenant[4][1],361,423,460,523);
	tb_SetRect(&queenant[4][0],462,423,562,523);
	tb_SetRect(&queenant[4][2],564,423,662,523);

	tb_SetRect(&queenant[5][1],361,524,460,624);
	tb_SetRect(&queenant[5][0],462,524,562,624);
	tb_SetRect(&queenant[5][2],564,524,662,624);

	tb_SetRect(&queenant[6][1],361,625,460,725);
	tb_SetRect(&queenant[6][0],462,625,562,725);
	tb_SetRect(&queenant[6][2],564,625,662,725);

	tb_SetRect(&queenant[7][1],361,726,460,826);
	tb_SetRect(&queenant[7][0],462,726,562,826);
	tb_SetRect(&queenant[7][2],564,726,662,826);

	// ---------------blink hound frames
	tb_SetRect(&blinkhound[0][0],765,19,837,90);
	tb_SetRect(&blinkhound[0][1],839,19,911,90);
	tb_SetRect(&blinkhound[0][2],913,19,985,90);

	tb_SetRect(&blinkhound[1][0],765,92,837,163);
	tb_SetRect(&blinkhound[1][1],839,92,911,163);
	tb_SetRect(&blinkhound[1][2],913,92,985,163);

	tb_SetRect(&blinkhound[2][0],765,165,837,236);
	tb_SetRect(&blinkhound[2][1],839,165,911,236);
	tb_SetRect(&blinkhound[2][2],913,165,985,236);

	tb_SetRect(&blinkhound[3][0],765,238,837,309);
	tb_SetRect(&blinkhound[3][1],839,238,911,309);
	tb_SetRect(&blinkhound[3][2],913,238,985,309);

	tb_SetRect(&blinkhound[4][0],765,311,837,382);
	tb_SetRect(&blinkhound[4][1],839,311,911,382);
	tb_SetRect(&blinkhound[4][2],913,311,985,382);

	tb_SetRect(&blinkhound[5][0],765,384,837,455);
	tb_SetRect(&blinkhound[5][1],839,384,911,455);
	tb_SetRect(&blinkhound[5][2],913,384,985,455);

	tb_SetRect(&blinkhound[6][0],765,457,837,528);
	tb_SetRect(&blinkhound[6][1],839,457,911,528);
	tb_SetRect(&blinkhound[6][2],913,457,985,528);

	tb_SetRect(&blinkhound[7][0],765,530,837,601);
	tb_SetRect(&blinkhound[7][1],839,530,911,601);
	tb_SetRect(&blinkhound[7][2],913,530,985,601);
	
	// ---------------dwarves frames

	tb_SetRect(&dwarf1[0][0],1,961,100,1060); //SW
	tb_SetRect(&dwarf1[0][1],102,961,201,1060);
	tb_SetRect(&dwarf1[0][2],203,961,302,1060);

	tb_SetRect(&dwarf1[1][0],1,1062,100,1161);//S
	tb_SetRect(&dwarf1[1][1],102,1062,201,1161);
	tb_SetRect(&dwarf1[1][2],203,1062,302,1161);

	tb_SetRect(&dwarf1[2][0],1,1163,100,1262);//SE
	tb_SetRect(&dwarf1[2][1],102,1163,201,1262);
	tb_SetRect(&dwarf1[2][2],203,1163,302,1262);

	tb_SetRect(&dwarf1[3][0],1,1264,100,1363);//E
	tb_SetRect(&dwarf1[3][1],102,1264,201,1363);
	tb_SetRect(&dwarf1[3][2],203,1264,302,1363);

	tb_SetRect(&dwarf1[4][0],1,1365,100,1464);//NE
	tb_SetRect(&dwarf1[4][1],102,1365,201,1464);
	tb_SetRect(&dwarf1[4][2],203,1365,302,1464);

	tb_SetRect(&dwarf1[5][0],1,1466,100,1565);//N
	tb_SetRect(&dwarf1[5][1],102,1466,201,1565);
	tb_SetRect(&dwarf1[5][2],203,1466,302,1565);

	tb_SetRect(&dwarf1[6][0],1,1567,100,1666);//NW
	tb_SetRect(&dwarf1[6][1],102,1567,201,1666);
	tb_SetRect(&dwarf1[6][2],203,1567,302,1666);

	tb_SetRect(&dwarf1[7][0],1,1668,100,1767);//W
	tb_SetRect(&dwarf1[7][1],102,1668,201,1767);
	tb_SetRect(&dwarf1[7][2],203,1668,302,1767);

	// ---------------cube frames
	tb_SetRect(&cube[0][0],987,502,1086,600);
	tb_SetRect(&cube[0][1],1088,502,1186,600);
	tb_SetRect(&cube[0][2],1188,502,1286,600);

	tb_SetRect(&cube[1][0],987,402,1086,500);
	tb_SetRect(&cube[1][1],1088,402,1186,500);
	tb_SetRect(&cube[1][2],1188,402,1286,500);

	tb_SetRect(&cube[2][0],987,302,1086,400);
	tb_SetRect(&cube[2][1],1088,302,1186,400);
	tb_SetRect(&cube[2][2],1188,302,1286,400);

	tb_SetRect(&cube[3][0],987,202,1086,300);
	tb_SetRect(&cube[3][1],1088,202,1186,300);
	tb_SetRect(&cube[3][2],1188,202,1286,300);

	tb_SetRect(&cube[4][0],987,102,1086,200);
	tb_SetRect(&cube[4][1],1088,102,1186,200);
	tb_SetRect(&cube[4][2],1188,102,1286,200);

	tb_SetRect(&cube[5][0],987,1,1086,100);
	tb_SetRect(&cube[5][1],1088,1,1186,100);
	tb_SetRect(&cube[5][2],1188,1,1286,100);

	tb_SetRect(&cube[6][0],987,702,1086,800);
	tb_SetRect(&cube[6][1],1088,702,1186,800);
	tb_SetRect(&cube[6][2],1188,702,1286,800);

	tb_SetRect(&cube[7][0],987,602,1086,700);
	tb_SetRect(&cube[7][1],1088,602,1186,700);
	tb_SetRect(&cube[7][2],1188,602,1286,700);

	// ---------------young dragon frames
	tb_SetRect(&youngdragon[0][0],1664,627,1788,750);
	tb_SetRect(&youngdragon[0][1],1790,627,1913,750);
	tb_SetRect(&youngdragon[0][2],1915,627,2038,750);

	tb_SetRect(&youngdragon[1][0],1664,502,1788,625);
	tb_SetRect(&youngdragon[1][1],1790,502,1913,625);
	tb_SetRect(&youngdragon[1][2],1915,502,2038,625);

	tb_SetRect(&youngdragon[2][0],1664,377,1788,500);
	tb_SetRect(&youngdragon[2][1],1790,377,1913,500);
	tb_SetRect(&youngdragon[2][2],1915,377,2038,500);

	tb_SetRect(&youngdragon[3][0],1664,252,1788,375);
	tb_SetRect(&youngdragon[3][1],1790,252,1913,375);
	tb_SetRect(&youngdragon[3][2],1915,252,2038,375);

	tb_SetRect(&youngdragon[4][0],1664,127,1788,250);
	tb_SetRect(&youngdragon[4][1],1790,127,1913,250);
	tb_SetRect(&youngdragon[4][2],1915,127,2038,250);

	tb_SetRect(&youngdragon[5][0],1664,1,1788,125);
	tb_SetRect(&youngdragon[5][1],1790,1,1913,125);
	tb_SetRect(&youngdragon[5][2],1915,1,2038,125);

	tb_SetRect(&youngdragon[6][0],1664,877,1788,1000);
	tb_SetRect(&youngdragon[6][1],1790,877,1913,1000);
	tb_SetRect(&youngdragon[6][2],1915,877,2038,1000);

	tb_SetRect(&youngdragon[7][0],1664,752,1788,875);
	tb_SetRect(&youngdragon[7][1],1790,752,1913,875);
	tb_SetRect(&youngdragon[7][2],1915,752,2038,875);

	// ---------------giant snake frames
	tb_SetRect(&snake[0][1],2,1,119,120);
	tb_SetRect(&snake[0][0],121,1,238,120);
	tb_SetRect(&snake[0][2],241,1,359,120);

	tb_SetRect(&snake[1][1],2,121,119,240);
	tb_SetRect(&snake[1][0],121,121,238,240);
	tb_SetRect(&snake[1][2],241,121,359,240);

	tb_SetRect(&snake[2][1],2,241,119,360);
	tb_SetRect(&snake[2][0],121,241,238,360);
	tb_SetRect(&snake[2][2],241,241,359,360);

	tb_SetRect(&snake[3][1],2,361,119,480);
	tb_SetRect(&snake[3][0],121,361,238,480);
	tb_SetRect(&snake[3][2],241,361,359,480);

	tb_SetRect(&snake[4][1],2,481,119,600);
	tb_SetRect(&snake[4][0],121,481,238,600);
	tb_SetRect(&snake[4][2],241,481,359,600);

	tb_SetRect(&snake[5][1],2,601,119,720);
	tb_SetRect(&snake[5][0],121,601,238,720);
	tb_SetRect(&snake[5][2],241,601,359,720);

	tb_SetRect(&snake[6][1],2,721,119,840);
	tb_SetRect(&snake[6][0],121,721,238,840);
	tb_SetRect(&snake[6][2],241,721,359,840);

	tb_SetRect(&snake[7][1],2,841,119,960);
	tb_SetRect(&snake[7][0],121,841,238,960);
	tb_SetRect(&snake[7][2],241,841,359,960);

	// ---------------snail frames
	tb_SetRect(&snail[0][0],610,1,701,71);
	tb_SetRect(&snail[0][1],702,1,793,71);
	tb_SetRect(&snail[0][2],794,1,885,71);

	tb_SetRect(&snail[1][0],610,72,701,142);
	tb_SetRect(&snail[1][1],702,72,793,142);
	tb_SetRect(&snail[1][2],794,72,885,142);

	tb_SetRect(&snail[2][0],610,143,701,213);
	tb_SetRect(&snail[2][1],702,143,793,213);
	tb_SetRect(&snail[2][2],794,143,885,213);

	tb_SetRect(&snail[3][0],610,214,701,284);
	tb_SetRect(&snail[3][1],702,214,793,284);
	tb_SetRect(&snail[3][2],794,214,885,284);

	tb_SetRect(&snail[4][0],610,285,701,355);
	tb_SetRect(&snail[4][1],702,285,793,355);
	tb_SetRect(&snail[4][2],794,285,885,355);

	tb_SetRect(&snail[5][0],610,356,701,426);
	tb_SetRect(&snail[5][1],702,356,793,426);
	tb_SetRect(&snail[5][2],794,356,885,426);

	tb_SetRect(&snail[6][0],610,427,701,497);
	tb_SetRect(&snail[6][1],702,427,793,497);
	tb_SetRect(&snail[6][2],794,427,885,497);

	tb_SetRect(&snail[7][0],610,498,701,568);
	tb_SetRect(&snail[7][1],702,498,793,568);
	tb_SetRect(&snail[7][2],794,498,885,568);

	// ---------------bat frames
	tb_SetRect(&bat[0][0],1,1,77,94);
	tb_SetRect(&bat[0][1],78,1,154,94);
	tb_SetRect(&bat[0][2],155,1,231,94);

	tb_SetRect(&bat[1][0],1,95,77,188);
	tb_SetRect(&bat[1][1],78,95,154,188);
	tb_SetRect(&bat[1][2],155,95,231,188);

	tb_SetRect(&bat[2][0],1,189,77,282);
	tb_SetRect(&bat[2][1],78,189,154,282);
	tb_SetRect(&bat[2][2],155,189,231,282);

	tb_SetRect(&bat[3][0],1,283,77,376);
	tb_SetRect(&bat[3][1],78,283,154,376);
	tb_SetRect(&bat[3][2],155,283,231,376);

	tb_SetRect(&bat[4][0],1,377,77,470);
	tb_SetRect(&bat[4][1],78,377,154,470);
	tb_SetRect(&bat[4][2],155,377,231,470);

	tb_SetRect(&bat[5][0],1,471,77,564);
	tb_SetRect(&bat[5][1],78,471,154,564);
	tb_SetRect(&bat[5][2],155,471,231,564);

	tb_SetRect(&bat[6][0],1,565,77,658);
	tb_SetRect(&bat[6][1],78,565,154,658);
	tb_SetRect(&bat[6][2],155,565,231,658);

	tb_SetRect(&bat[7][0],1,659,77,752);
	tb_SetRect(&bat[7][1],78,659,154,752);
	tb_SetRect(&bat[7][2],155,659,231,752);

	// ---------------slug frames
	tb_SetRect(&slug[0][0],367,118,486,237);
	tb_SetRect(&slug[0][2],487,118,606,237);
	tb_SetRect(&slug[0][1],367,118,486,237);

	tb_SetRect(&slug[1][0],367,238,486,357);
	tb_SetRect(&slug[1][2],487,238,606,357);
	tb_SetRect(&slug[1][1],367,238,486,357);

	tb_SetRect(&slug[2][0],367,358,486,477);
	tb_SetRect(&slug[2][2],487,358,606,477);
	tb_SetRect(&slug[2][1],367,358,486,477);

	tb_SetRect(&slug[3][0],367,478,486,597);
	tb_SetRect(&slug[3][2],487,478,606,597);
	tb_SetRect(&slug[3][1],367,478,486,597);

	tb_SetRect(&slug[4][0],367,598,486,717);
	tb_SetRect(&slug[4][2],487,598,606,717);
	tb_SetRect(&slug[4][1],367,598,486,717);

	tb_SetRect(&slug[5][0],367,718,486,837);
	tb_SetRect(&slug[5][2],487,718,606,837);
	tb_SetRect(&slug[5][1],367,718,486,837);

	tb_SetRect(&slug[6][0],367,838,486,957);
	tb_SetRect(&slug[6][2],487,838,606,957);
	tb_SetRect(&slug[6][1],367,838,486,957);

	tb_SetRect(&slug[7][0],367,958,486,1077);
	tb_SetRect(&slug[7][2],487,958,606,1077);
	tb_SetRect(&slug[7][1],367,958,486,1077);

	// ---------------beholder frames
	tb_SetRect(&beholder[0][0],379,1,455,84);
	tb_SetRect(&beholder[0][1],379,1,455,84);
	tb_SetRect(&beholder[0][2],379,1,455,84);

	tb_SetRect(&beholder[1][0],456,1,532,84);
	tb_SetRect(&beholder[1][1],456,1,532,84);
	tb_SetRect(&beholder[1][2],456,1,532,84);

	tb_SetRect(&beholder[2][0],129,1008,205,1091);
	tb_SetRect(&beholder[2][1],129,1008,205,1091);
	tb_SetRect(&beholder[2][2],129,1008,205,1091);

	tb_SetRect(&beholder[3][0],206,1008,282,1091);
	tb_SetRect(&beholder[3][1],206,1008,282,1091);
	tb_SetRect(&beholder[3][2],206,1008,282,1091);

	tb_SetRect(&beholder[4][0],283,1008,359,1091);
	tb_SetRect(&beholder[4][1],283,1008,359,1091);
	tb_SetRect(&beholder[4][2],283,1008,359,1091);

	tb_SetRect(&beholder[5][0],1318,1,1394,84);
	tb_SetRect(&beholder[5][1],1318,1,1394,84);
	tb_SetRect(&beholder[5][2],1318,1,1394,84);

	tb_SetRect(&beholder[6][0],1395,1,1471,84);
	tb_SetRect(&beholder[6][1],1395,1,1471,84);
	tb_SetRect(&beholder[6][2],1395,1,1471,84);

	tb_SetRect(&beholder[7][0],791,924,867,1007);
	tb_SetRect(&beholder[7][1],791,924,867,1007);
	tb_SetRect(&beholder[7][2],791,924,867,1007);

	// ---------------sea serpent frames
	tb_SetRect(&serpent[0][0],727,1,843,97);
	tb_SetRect(&serpent[0][1],727,1,843,97);
	tb_SetRect(&serpent[0][2],844,1,960,97);

	tb_SetRect(&serpent[1][0],727,98,843,194);
	tb_SetRect(&serpent[1][1],727,98,843,194);
	tb_SetRect(&serpent[1][2],844,98,960,194);

	tb_SetRect(&serpent[2][0],727,195,843,291);
	tb_SetRect(&serpent[2][1],727,195,843,291);
	tb_SetRect(&serpent[2][2],844,195,960,291);

	tb_SetRect(&serpent[3][0],727,292,843,388);
	tb_SetRect(&serpent[3][1],727,292,843,388);
	tb_SetRect(&serpent[3][2],844,292,960,388);

	tb_SetRect(&serpent[4][0],727,389,843,485);
	tb_SetRect(&serpent[4][1],727,389,843,485);
	tb_SetRect(&serpent[4][2],844,389,960,485);

	tb_SetRect(&serpent[5][0],727,486,843,582);
	tb_SetRect(&serpent[5][1],727,486,843,582);
	tb_SetRect(&serpent[5][2],844,486,960,582);

	tb_SetRect(&serpent[6][0],727,583,843,679);
	tb_SetRect(&serpent[6][1],727,583,843,679);
	tb_SetRect(&serpent[6][2],844,583,960,679);

	tb_SetRect(&serpent[7][0],727,680,843,776);
	tb_SetRect(&serpent[7][1],727,680,843,776);
	tb_SetRect(&serpent[7][2],844,680,960,776);

	// ---------------hell hound frames
	tb_SetRect(&hellHound[0][0],607,1,700,93);
	tb_SetRect(&hellHound[0][1],701,1,794,93);
	tb_SetRect(&hellHound[0][2],795,1,888,93);

	tb_SetRect(&hellHound[1][0],607,94,700,186);
	tb_SetRect(&hellHound[1][1],701,94,794,186);
	tb_SetRect(&hellHound[1][2],795,94,888,186);

	tb_SetRect(&hellHound[2][0],607,187,700,279);
	tb_SetRect(&hellHound[2][1],701,187,794,279);
	tb_SetRect(&hellHound[2][2],795,187,888,279);

	tb_SetRect(&hellHound[3][0],607,280,700,372);
	tb_SetRect(&hellHound[3][1],701,280,794,372);
	tb_SetRect(&hellHound[3][2],795,280,888,372);

	tb_SetRect(&hellHound[4][0],607,373,700,465);
	tb_SetRect(&hellHound[4][1],701,373,794,465);
	tb_SetRect(&hellHound[4][2],795,373,888,465);

	tb_SetRect(&hellHound[5][0],607,466,700,558);
	tb_SetRect(&hellHound[5][1],701,466,794,558);
	tb_SetRect(&hellHound[5][2],795,466,888,558);

	tb_SetRect(&hellHound[6][0],607,559,700,651);
	tb_SetRect(&hellHound[6][1],701,559,794,651);
	tb_SetRect(&hellHound[6][2],795,559,888,651);

	tb_SetRect(&hellHound[7][0],607,652,700,744);
	tb_SetRect(&hellHound[7][1],701,652,794,744);
	tb_SetRect(&hellHound[7][2],795,652,888,744);

	// ---------------rat frames
	tb_SetRect(&rat[0][0],1,1094,69,1162);
	tb_SetRect(&rat[0][1],70,1094,138,1162);
	tb_SetRect(&rat[0][2],139,1094,207,1162);

	tb_SetRect(&rat[1][0],208,1094,276,1162);
	tb_SetRect(&rat[1][1],277,1094,345,1162);
	tb_SetRect(&rat[1][2],346,1094,414,1162);

	tb_SetRect(&rat[2][0],791,1009,859,1077);
	tb_SetRect(&rat[2][1],860,1009,928,1077);
	tb_SetRect(&rat[2][2],929,1009,997,1077);

	tb_SetRect(&rat[3][0],1073,845,1141,913);
	tb_SetRect(&rat[3][1],1142,845,1210,913);
	tb_SetRect(&rat[3][2],1211,845,1279,913);

	tb_SetRect(&rat[4][0],1073,914,1141,982);
	tb_SetRect(&rat[4][1],1142,914,1210,982);
	tb_SetRect(&rat[4][2],1211,914,1279,982);

	tb_SetRect(&rat[5][0],1073,983,1141,1051);
	tb_SetRect(&rat[5][1],1142,983,1210,1051);
	tb_SetRect(&rat[5][2],1211,983,1279,1051);

	tb_SetRect(&rat[6][0],1280,894,1348,962);
	tb_SetRect(&rat[6][1],1349,894,1417,962);
	tb_SetRect(&rat[6][2],1418,894,1486,962);

	tb_SetRect(&rat[7][0],1280,963,1348,1031);
	tb_SetRect(&rat[7][1],1349,963,1417,1031);
	tb_SetRect(&rat[7][2],1418,963,1486,1031);

	// ---------------wolf frames
	tb_SetRect(&wolf[0][0],889,1,957,69);
	tb_SetRect(&wolf[0][1],958,1,1026,69);
	tb_SetRect(&wolf[0][2],1027,1,1095,69);

	tb_SetRect(&wolf[1][0],889,70,957,138);
	tb_SetRect(&wolf[1][1],958,70,1026,138);
	tb_SetRect(&wolf[1][2],1027,70,1095,138);

	tb_SetRect(&wolf[2][0],889,139,957,207);
	tb_SetRect(&wolf[2][1],958,139,1026,207);
	tb_SetRect(&wolf[2][2],1027,139,1095,207);

	tb_SetRect(&wolf[3][0],889,208,957,276);
	tb_SetRect(&wolf[3][1],958,208,1026,276);
	tb_SetRect(&wolf[3][2],1027,208,1095,276);

	tb_SetRect(&wolf[4][0],889,277,957,345);
	tb_SetRect(&wolf[4][1],958,277,1026,345);
	tb_SetRect(&wolf[4][2],1027,277,1095,345);

	tb_SetRect(&wolf[5][0],889,346,957,414);
	tb_SetRect(&wolf[5][1],958,346,1026,414);
	tb_SetRect(&wolf[5][2],1027,346,1095,414);

	tb_SetRect(&wolf[6][0],889,415,957,483);
	tb_SetRect(&wolf[6][1],958,415,1026,483);
	tb_SetRect(&wolf[6][2],1027,415,1095,483);

	tb_SetRect(&wolf[7][0],889,484,957,552);
	tb_SetRect(&wolf[7][1],958,484,1026,552);
	tb_SetRect(&wolf[7][2],1027,484,1095,552);

	// ---------------sheep frames
	tb_SetRect(&sheep[0][0],1,574,64,638);
	tb_SetRect(&sheep[0][1],65,574,128,638);
	tb_SetRect(&sheep[0][2],1,574,64,638);

	tb_SetRect(&sheep[1][0],1,639,64,703);
	tb_SetRect(&sheep[1][1],65,639,128,703);
	tb_SetRect(&sheep[1][2],1,639,64,703);

	tb_SetRect(&sheep[2][0],1,704,64,768);
	tb_SetRect(&sheep[2][1],65,704,128,768);
	tb_SetRect(&sheep[2][2],1,704,64,768);

	tb_SetRect(&sheep[3][0],1,769,64,833);
	tb_SetRect(&sheep[3][1],65,769,128,833);
	tb_SetRect(&sheep[3][2],1,769,64,833);

	tb_SetRect(&sheep[4][0],1,834,64,898);
	tb_SetRect(&sheep[4][1],65,834,128,898);
	tb_SetRect(&sheep[4][2],1,834,64,898);

	tb_SetRect(&sheep[5][0],1,899,64,963);
	tb_SetRect(&sheep[5][1],65,899,128,963);
	tb_SetRect(&sheep[5][2],1,899,64,963);

	tb_SetRect(&sheep[6][0],1,964,64,1028);
	tb_SetRect(&sheep[6][1],65,964,128,1028);
	tb_SetRect(&sheep[6][2],1,964,64,1028);

	tb_SetRect(&sheep[7][0],1,1029,64,1093);
	tb_SetRect(&sheep[7][1],65,1029,128,1093);
	tb_SetRect(&sheep[7][2],1,1029,64,1093);

	// ---------------giant frames
	tb_SetRect(&giant[0][0],1,1,146,110);
	tb_SetRect(&giant[0][1],147,1,292,110);
	tb_SetRect(&giant[0][2],293,1,438,110);

	tb_SetRect(&giant[1][0],1,111,146,220);
	tb_SetRect(&giant[1][1],147,111,292,220);
	tb_SetRect(&giant[1][2],293,111,438,220);

	tb_SetRect(&giant[2][0],1,221,146,330);
	tb_SetRect(&giant[2][1],147,221,292,330);
	tb_SetRect(&giant[2][2],293,221,438,330);

	tb_SetRect(&giant[3][0],1,331,146,440);
	tb_SetRect(&giant[3][1],147,331,292,440);
	tb_SetRect(&giant[3][2],293,331,438,440);

	tb_SetRect(&giant[4][0],1,441,146,550);
	tb_SetRect(&giant[4][1],147,441,292,550);
	tb_SetRect(&giant[4][2],293,441,438,550);

	tb_SetRect(&giant[5][0],1,551,146,660);
	tb_SetRect(&giant[5][1],147,551,292,660);
	tb_SetRect(&giant[5][2],293,551,438,660);

	tb_SetRect(&giant[6][0],1,661,146,770);
	tb_SetRect(&giant[6][1],147,661,292,770);
	tb_SetRect(&giant[6][2],293,661,438,770);

	tb_SetRect(&giant[7][0],1,771,146,880);
	tb_SetRect(&giant[7][1],147,771,292,880);
	tb_SetRect(&giant[7][2],293,771,438,880);

	// ---------------centaur frames
	tb_SetRect(&centaur[0][0],439,1,534,108);
	tb_SetRect(&centaur[0][1],535,1,630,108);
	tb_SetRect(&centaur[0][2],631,1,726,108);

	tb_SetRect(&centaur[1][0],439,109,534,216);
	tb_SetRect(&centaur[1][1],535,109,630,216);
	tb_SetRect(&centaur[1][2],631,109,726,216);

	tb_SetRect(&centaur[2][0],439,217,534,324);
	tb_SetRect(&centaur[2][1],535,217,630,324);
	tb_SetRect(&centaur[2][2],631,217,726,324);

	tb_SetRect(&centaur[3][0],439,325,534,432);
	tb_SetRect(&centaur[3][1],535,325,630,432);
	tb_SetRect(&centaur[3][2],631,325,726,432);

	tb_SetRect(&centaur[4][0],439,433,534,540);
	tb_SetRect(&centaur[4][1],535,433,630,540);
	tb_SetRect(&centaur[4][2],631,433,726,540);

	tb_SetRect(&centaur[5][0],439,541,534,648);
	tb_SetRect(&centaur[5][1],535,541,630,648);
	tb_SetRect(&centaur[5][2],631,541,726,648);

	tb_SetRect(&centaur[6][0],439,649,534,756);
	tb_SetRect(&centaur[6][1],535,649,630,756);
	tb_SetRect(&centaur[6][2],631,649,726,756);

	tb_SetRect(&centaur[7][0],439,757,534,864);
	tb_SetRect(&centaur[7][1],535,757,630,864);
	tb_SetRect(&centaur[7][2],631,757,726,864);

	// ---------------troll frames
	tb_SetRect(&troll[0][0],1117,1,1231,81);
	tb_SetRect(&troll[0][1],1232,1,1346,81);
	tb_SetRect(&troll[0][2],1347,1,1461,81);

	tb_SetRect(&troll[1][0],1117,82,1231,162);
	tb_SetRect(&troll[1][1],1232,82,1346,162);
	tb_SetRect(&troll[1][2],1347,82,1461,162);

	tb_SetRect(&troll[2][0],1117,163,1231,243);
	tb_SetRect(&troll[2][1],1232,163,1346,243);
	tb_SetRect(&troll[2][2],1347,163,1461,243);

	tb_SetRect(&troll[3][0],1117,244,1231,324);
	tb_SetRect(&troll[3][1],1232,244,1346,324);
	tb_SetRect(&troll[3][2],1347,244,1461,324);

	tb_SetRect(&troll[4][0],1117,325,1231,405);
	tb_SetRect(&troll[4][1],1232,325,1346,405);
	tb_SetRect(&troll[4][2],1347,325,1461,405);

	tb_SetRect(&troll[5][0],1117,406,1231,486);
	tb_SetRect(&troll[5][1],1232,406,1346,486);
	tb_SetRect(&troll[5][2],1347,406,1461,486);

	tb_SetRect(&troll[6][0],1117,487,1231,567);
	tb_SetRect(&troll[6][1],1232,487,1346,567);
	tb_SetRect(&troll[6][2],1347,487,1461,567);

	tb_SetRect(&troll[7][0],1117,568,1231,648);
	tb_SetRect(&troll[7][1],1232,568,1346,648);
	tb_SetRect(&troll[7][2],1347,568,1461,648);

	// ---------------zombie frames
	tb_SetRect(&zombie[0][0],961,1,1053,91);
	tb_SetRect(&zombie[0][1],1054,1,1146,91);
	tb_SetRect(&zombie[0][2],1147,1,1239,91);

	tb_SetRect(&zombie[1][0],961,92,1053,182);
	tb_SetRect(&zombie[1][1],1054,92,1146,182);
	tb_SetRect(&zombie[1][2],1147,92,1239,182);

	tb_SetRect(&zombie[2][0],961,183,1053,273);
	tb_SetRect(&zombie[2][1],1054,183,1146,273);
	tb_SetRect(&zombie[2][2],1147,183,1239,273);

	tb_SetRect(&zombie[3][0],961,274,1053,364);
	tb_SetRect(&zombie[3][1],1054,274,1146,364);
	tb_SetRect(&zombie[3][2],1147,274,1239,364);

	tb_SetRect(&zombie[4][0],961,365,1053,455);
	tb_SetRect(&zombie[4][1],1054,365,1146,455);
	tb_SetRect(&zombie[4][2],1147,365,1239,455);

	tb_SetRect(&zombie[5][0],961,456,1053,546);
	tb_SetRect(&zombie[5][1],1054,456,1146,546);
	tb_SetRect(&zombie[5][2],1147,456,1239,546);

	tb_SetRect(&zombie[6][0],961,547,1053,637);
	tb_SetRect(&zombie[6][1],1054,547,1146,637);
	tb_SetRect(&zombie[6][2],1147,547,1239,637);

	tb_SetRect(&zombie[7][0],961,638,1053,720);
	tb_SetRect(&zombie[7][1],1054,638,1146,720);
	tb_SetRect(&zombie[7][2],1147,638,1239,720);

	// ---------------spider frames
	tb_SetRect(&spider[0][0],232,1,357,101);
	tb_SetRect(&spider[0][1],358,1,483,101);
	tb_SetRect(&spider[0][2],484,1,609,101);

	tb_SetRect(&spider[1][0],232,102,357,202);
	tb_SetRect(&spider[1][1],358,102,483,202);
	tb_SetRect(&spider[1][2],484,102,609,202);

	tb_SetRect(&spider[2][0],232,203,357,303);
	tb_SetRect(&spider[2][1],358,203,483,303);
	tb_SetRect(&spider[2][2],484,203,609,303);

	tb_SetRect(&spider[3][0],232,304,357,404);
	tb_SetRect(&spider[3][1],358,304,483,404);
	tb_SetRect(&spider[3][2],484,304,609,404);

	tb_SetRect(&spider[4][0],232,405,357,505);
	tb_SetRect(&spider[4][1],358,405,483,505);
	tb_SetRect(&spider[4][2],484,405,609,505);

	tb_SetRect(&spider[5][0],232,506,357,606);
	tb_SetRect(&spider[5][1],358,506,483,606);
	tb_SetRect(&spider[5][2],484,506,609,606);

	tb_SetRect(&spider[6][0],232,607,357,707);
	tb_SetRect(&spider[6][1],358,607,483,707);
	tb_SetRect(&spider[6][2],484,607,609,707);

	tb_SetRect(&spider[7][0],232,708,357,808);
	tb_SetRect(&spider[7][1],358,708,483,808);
	tb_SetRect(&spider[7][2],484,708,609,808);

	// ---------------dragon frames
	tb_SetRect(&dragon[0][0],1,1,201,191);
	tb_SetRect(&dragon[0][1],202,1,402,191);
	tb_SetRect(&dragon[0][2],403,1,603,191);

	tb_SetRect(&dragon[1][0],1,192,201,384);
	tb_SetRect(&dragon[1][1],202,192,402,384);
	tb_SetRect(&dragon[1][2],403,192,603,384);

	tb_SetRect(&dragon[2][0],1,385,201,577);
	tb_SetRect(&dragon[2][1],202,385,402,577);
	tb_SetRect(&dragon[2][2],403,385,603,577);

	tb_SetRect(&dragon[3][0],1,578,201,770);
	tb_SetRect(&dragon[3][1],202,578,402,770);
	tb_SetRect(&dragon[3][2],403,578,603,770);

	tb_SetRect(&dragon[4][0],1,771,201,962);
	tb_SetRect(&dragon[4][1],202,771,402,962);
	tb_SetRect(&dragon[4][2],403,771,603,962);

	tb_SetRect(&dragon[5][0],1,963,201,1155);
	tb_SetRect(&dragon[5][1],202,963,402,1155);
	tb_SetRect(&dragon[5][2],403,963,603,1155);

	tb_SetRect(&dragon[6][0],1,1156,201,1347);
	tb_SetRect(&dragon[6][1],202,1156,402,1347);
	tb_SetRect(&dragon[6][2],403,1156,603,1347);

	tb_SetRect(&dragon[7][0],1,1348,201,1540);
	tb_SetRect(&dragon[7][1],202,1348,402,1540);
	tb_SetRect(&dragon[7][2],403,1348,603,1540);

	// ---------------tusker frames
	tb_SetRect(&tusker[0][0],1112,1001,1276,1165);  //SW
	tb_SetRect(&tusker[0][1],1278,1001,1442,1165);
	tb_SetRect(&tusker[0][2],1444,1001,1608,1165);

	tb_SetRect(&tusker[1][0],1610,1333,1774,1497); 	//S
	tb_SetRect(&tusker[1][1],1776,1334,1940,1497);
	tb_SetRect(&tusker[1][2],1288,333,1452,497);

	tb_SetRect(&tusker[2][0],1112,1333,1276,1498);	//SE
	tb_SetRect(&tusker[2][1],1278,1333,1442,1497);
	tb_SetRect(&tusker[2][2],1444,1333,1608,1497);

	tb_SetRect(&tusker[3][0],1610,1167,1774,1331);	//E
	tb_SetRect(&tusker[3][1],1776,1167,1940,1331);
	tb_SetRect(&tusker[3][2],1288,167,1452,331);

	tb_SetRect(&tusker[4][0],1610,1499,1774,1663);	//NE
	tb_SetRect(&tusker[4][1],1776,1499,1940,1663);
	tb_SetRect(&tusker[4][2],1288,499,1452,663);

	tb_SetRect(&tusker[5][0],1610,1001,1774,1165);	//N
	tb_SetRect(&tusker[5][1],1776,1001,1940,1165);
	tb_SetRect(&tusker[5][2],1288,1,1452,165);

	tb_SetRect(&tusker[6][0],1112,1499,1276,1663);	//NW
	tb_SetRect(&tusker[6][1],1278,1499,1442,1663);
	tb_SetRect(&tusker[6][2],1444,1499,1608,1663);

	tb_SetRect(&tusker[7][0],1112,1167,1276,1332);	//W
	tb_SetRect(&tusker[7][1],1278,1167,1442,1332);
	tb_SetRect(&tusker[7][2],1444,1167,1608,1331);

	// ---------------pixie frames
	tb_SetRect(&pixie[0][0],1464,175,1491,231);  //SW
	tb_SetRect(&pixie[0][1],1493,175,1520,231);

	tb_SetRect(&pixie[1][0],1464,233,1491,289); //S
	tb_SetRect(&pixie[1][1],1493,233,1520,289);

	tb_SetRect(&pixie[2][0],1464,291,1491,347);  //SE
	tb_SetRect(&pixie[2][1],1493,291,1520,347);

	tb_SetRect(&pixie[3][0],1464,349,1491,405);  //E
	tb_SetRect(&pixie[3][1],1493,349,1520,405);

	tb_SetRect(&pixie[4][0],1464,407,1491,463);  //NE
	tb_SetRect(&pixie[4][1],1493,407,1520,463);

	tb_SetRect(&pixie[5][0],1464,1,1491,57);  //N
	tb_SetRect(&pixie[5][1],1493,1,1520,57);

	tb_SetRect(&pixie[6][0],1464,59,1491,115);  //NW
	tb_SetRect(&pixie[6][1],1493,59,1520,115);

	tb_SetRect(&pixie[7][0],1464,117,1491,173);  //W
	tb_SetRect(&pixie[7][1],1493,117,1520,173);
	
	
	
	
	// ---------------ancient dragon frames
	tb_SetRect(&ancientdragon[0][0],1,801,225,959);  //SW
	tb_SetRect(&ancientdragon[0][1],227,801,451,959);
	tb_SetRect(&ancientdragon[0][2],453,801,677,959);

	tb_SetRect(&ancientdragon[1][0],1,161,225,319); 	//S
	tb_SetRect(&ancientdragon[1][1],227,161,451,319);
	tb_SetRect(&ancientdragon[1][2],453,161,677,319);

	tb_SetRect(&ancientdragon[2][0],1,491,225,639);	//SE
	tb_SetRect(&ancientdragon[2][1],227,491,451,639);
	tb_SetRect(&ancientdragon[2][2],453,491,677,639);

	tb_SetRect(&ancientdragon[3][0],1,641,225,799);	//E
	tb_SetRect(&ancientdragon[3][1],227,641,451,799);
	tb_SetRect(&ancientdragon[3][2],453,641,677,799);

	tb_SetRect(&ancientdragon[4][0],1,1,225,159);	//NE
	tb_SetRect(&ancientdragon[4][1],227,1,451,159);
	tb_SetRect(&ancientdragon[4][2],453,1,677,159);

	tb_SetRect(&ancientdragon[5][0],1,321,225,489);	//N
	tb_SetRect(&ancientdragon[5][1],227,321,451,489);
	tb_SetRect(&ancientdragon[5][2],453,321,677,489);

	tb_SetRect(&ancientdragon[6][0],1,961,225,1119);	//NW
	tb_SetRect(&ancientdragon[6][1],227,961,451,1119);
	tb_SetRect(&ancientdragon[6][2],453,961,677,1119);

	tb_SetRect(&ancientdragon[7][0],1,1122,225,1279);	//W
	tb_SetRect(&ancientdragon[7][1],227,1122,451,1279);
	tb_SetRect(&ancientdragon[7][2],453,1122,677,1279);
	
	// ---------------lich frames
	tb_SetRect(&lich[0][1],757,897,832,996);  //SW
	tb_SetRect(&lich[0][0],680,897,755,996);
	tb_SetRect(&lich[0][2],834,897,909,996);

	tb_SetRect(&lich[1][1],757,998,832,1097); 	//S
	tb_SetRect(&lich[1][0],680,998,755,1097);
	tb_SetRect(&lich[1][2],834,998,909,1097);

	tb_SetRect(&lich[2][1],757,1099,832,1198);	//SE
	tb_SetRect(&lich[2][0],680,1099,755,1198);
	tb_SetRect(&lich[2][2],834,1099,909,1198);

	tb_SetRect(&lich[3][1],757,1200,832,1299);	//E
	tb_SetRect(&lich[3][0],680,1200,755,1299);
	tb_SetRect(&lich[3][2],834,1200,909,1299);

	tb_SetRect(&lich[4][1],757,1301,832,1400);	//NE
	tb_SetRect(&lich[4][0],680,1301,755,1400);
	tb_SetRect(&lich[4][2],834,1301,909,1400);

	tb_SetRect(&lich[5][1],757,594,832,693);	//N
	tb_SetRect(&lich[5][0],680,594,755,693);
	tb_SetRect(&lich[5][2],834,594,909,693);

	tb_SetRect(&lich[6][1],757,695,832,794);	//NW
	tb_SetRect(&lich[6][0],680,695,755,794);
	tb_SetRect(&lich[6][2],834,695,909,794);

	tb_SetRect(&lich[7][1],757,796,832,895);	//W
	tb_SetRect(&lich[7][0],680,796,755,895);
	tb_SetRect(&lich[7][2],834,796,909,895);

	// ---------------male human ghost	 
	tb_SetRect(&humanMaleGhost[0][0],680,1,743,74);  //SW
	tb_SetRect(&humanMaleGhost[0][1],744,1,807,74);

	tb_SetRect(&humanMaleGhost[1][0],680,75,743,148); 	//S
	tb_SetRect(&humanMaleGhost[1][1],744,75,807,148);

	tb_SetRect(&humanMaleGhost[2][0],680,149,743,222);	//SE
	tb_SetRect(&humanMaleGhost[2][1],744,149,807,222);

	tb_SetRect(&humanMaleGhost[3][0],680,223,743,296);	//E
	tb_SetRect(&humanMaleGhost[3][1],744,223,807,296);

	tb_SetRect(&humanMaleGhost[4][0],680,297,743,370);	//NE
	tb_SetRect(&humanMaleGhost[4][1],744,297,807,370);

	tb_SetRect(&humanMaleGhost[5][0],680,371,743,444);	//N
	tb_SetRect(&humanMaleGhost[5][1],744,371,807,444);

	tb_SetRect(&humanMaleGhost[6][0],680,445,743,518);	//NW
	tb_SetRect(&humanMaleGhost[6][1],744,445,807,518);

	tb_SetRect(&humanMaleGhost[7][0],680,519,743,592);	//W
	tb_SetRect(&humanMaleGhost[7][1],744,519,807,592);
	
	// ---------------female human ghost	 	 
	tb_SetRect(&humanFemaleGhost[0][0],808,1,871,74);  //SW
	tb_SetRect(&humanFemaleGhost[0][1],872,1,935,74);

	tb_SetRect(&humanFemaleGhost[1][0],808,75,871,148); 	//S
	tb_SetRect(&humanFemaleGhost[1][1],872,75,935,148);

	tb_SetRect(&humanFemaleGhost[2][0],808,149,871,222);	//SE
	tb_SetRect(&humanFemaleGhost[2][1],872,149,935,222);

	tb_SetRect(&humanFemaleGhost[3][0],808,223,871,296);	//E
	tb_SetRect(&humanFemaleGhost[3][1],872,223,935,296);

	tb_SetRect(&humanFemaleGhost[4][0],808,297,871,370);	//NE
	tb_SetRect(&humanFemaleGhost[4][1],872,297,935,370);

	tb_SetRect(&humanFemaleGhost[5][0],808,371,871,444);	//N
	tb_SetRect(&humanFemaleGhost[5][1],872,371,935,444);

	tb_SetRect(&humanFemaleGhost[6][0],808,445,871,518);	//NW
	tb_SetRect(&humanFemaleGhost[6][1],872,445,935,518);

	tb_SetRect(&humanFemaleGhost[7][0],808,519,871,592);	//W
	tb_SetRect(&humanFemaleGhost[7][1],872,519,935,592);
	
	// ---------------lion frames
	tb_SetRect(&lion[6][0], 1,1403,118,1520); //NW
	tb_SetRect(&lion[6][1], 1,1522,118,1639);
	tb_SetRect(&lion[6][2], 1,1641,118,1758);
	
	tb_SetRect(&lion[7][0], 120,1403,237,1520); //W
	tb_SetRect(&lion[7][1], 120,1522,237,1639);
	tb_SetRect(&lion[7][2], 120,1641,237,1758);
	
	tb_SetRect(&lion[4][0], 239,1403,356,1520); //NE
	tb_SetRect(&lion[4][1], 239,1522,356,1639);
	tb_SetRect(&lion[4][2], 239,1641,356,1758);
	
	tb_SetRect(&lion[2][0], 358,1403,475,1520); //SE
	tb_SetRect(&lion[2][1], 358,1522,475,1639);
	tb_SetRect(&lion[2][2], 358,1641,475,1758);
	
	tb_SetRect(&lion[5][0], 477,1403,594,1520); //N
	tb_SetRect(&lion[5][1], 477,1522,594,1639);
	tb_SetRect(&lion[5][2], 477,1641,594,1758);
	
	tb_SetRect(&lion[0][0], 596,1403,713,1520); //SW
	tb_SetRect(&lion[0][1], 596,1522,713,1639);
	tb_SetRect(&lion[0][2], 596,1641,713,1758);
	
	tb_SetRect(&lion[3][0], 715,1403,832,1520); //E
	tb_SetRect(&lion[3][1], 715,1522,832,1639);
	tb_SetRect(&lion[3][2], 715,1641,832,1758);
	
	tb_SetRect(&lion[1][0], 834,1403,951,1520); //S
	tb_SetRect(&lion[1][1], 834,1522,951,1639);
	tb_SetRect(&lion[1][2], 834,1641,951,1758);
	
	// ---------------golem frames
	tb_SetRect(&golem[6][0], 942,901,1089,1049); //NW
	tb_SetRect(&golem[6][1], 1091,901,1238,1049);
	tb_SetRect(&golem[6][2], 1240,901,1387,1049);
	
	tb_SetRect(&golem[7][0], 942,451,1089,599); //W
	tb_SetRect(&golem[7][1], 1091,451,1238,599);
	tb_SetRect(&golem[7][2], 1240,451,1387,599);
	
	tb_SetRect(&golem[4][0], 942,601,1089,749); //NE
	tb_SetRect(&golem[4][1], 1091,601,1238,749);
	tb_SetRect(&golem[4][2], 1240,601,1387,749);
	
	tb_SetRect(&golem[2][0], 942,1051,1089,1199); //SE
	tb_SetRect(&golem[2][1], 1091,1051,1238,1199);
	tb_SetRect(&golem[2][2], 1240,1051,1387,1199);
	
	tb_SetRect(&golem[5][0], 942,751,1089,899); //N
	tb_SetRect(&golem[5][1], 1091,751,1238,899);
	tb_SetRect(&golem[5][2], 1240,751,1387,899);
	
	tb_SetRect(&golem[0][0], 942,1,1089,149); //SW
	tb_SetRect(&golem[0][1], 1091,1,1238,149);
	tb_SetRect(&golem[0][2], 1240,1,1387,149);
	
	tb_SetRect(&golem[3][0], 942,301,1089,449); //E
	tb_SetRect(&golem[3][1], 1091,301,1238,449);
	tb_SetRect(&golem[3][2], 1240,301,1387,449);
	
	tb_SetRect(&golem[1][0], 942,151,1089,299); //S
	tb_SetRect(&golem[1][1], 1091,151,1238,299);
	tb_SetRect(&golem[1][2], 1240,151,1387,299);
	
	// ---------------bear frames
	tb_SetRect(&bear[6][0], 119,832,236,949); //NW
	tb_SetRect(&bear[6][1], 119,951,236,1068);
	tb_SetRect(&bear[6][2], 119,1070,236,1187);
	
	tb_SetRect(&bear[7][0], 238,832,355,949); //W
	tb_SetRect(&bear[7][1], 238,951,355,1068);
	tb_SetRect(&bear[7][2], 238,1070,355,1187);
	
	tb_SetRect(&bear[4][0], 357,832,474,949); //NE
	tb_SetRect(&bear[4][1], 357,951,474,1068);
	tb_SetRect(&bear[4][2], 357,1070,474,1187);
	
	tb_SetRect(&bear[2][0], 833,832,950,949); //SE
	tb_SetRect(&bear[2][1], 833,951,950,1068);
	tb_SetRect(&bear[2][2], 833,1070,950,1187);
	
	tb_SetRect(&bear[5][0], 714,832,831,949); //N
	tb_SetRect(&bear[5][1], 714,951,831,1068);
	tb_SetRect(&bear[5][2], 714,1070,831,1187);
	
	tb_SetRect(&bear[0][0], 595,832,712,949); //SW
	tb_SetRect(&bear[0][1], 595,951,712,1068);
	tb_SetRect(&bear[0][2], 595,1070,712,1187);
	
	tb_SetRect(&bear[3][0], 476,832,593,949); //E
	tb_SetRect(&bear[3][1], 476,951,593,1068);
	tb_SetRect(&bear[3][2], 476,1070,593,1187);
	
	tb_SetRect(&bear[1][0], 0,832,117,949); //S
	tb_SetRect(&bear[1][1], 0,951,117,1068);
	tb_SetRect(&bear[1][2], 0,1070,117,1187);

// ---------------shark frames
	tb_SetRect(&shark[6][0], 595,1189,712,1306); //NW
	tb_SetRect(&shark[6][1], 595,1189,712,1306);
	tb_SetRect(&shark[6][2], 595,1308,712,1425);
	
	tb_SetRect(&shark[7][0], 119,1189,236,1306); //W
	tb_SetRect(&shark[7][1], 119,1189,236,1306);
	tb_SetRect(&shark[7][2], 119,1308,236,1425);
	
	tb_SetRect(&shark[4][0], 833,1189,950,1306); //NE
	tb_SetRect(&shark[4][1], 833,1189,950,1306);
	tb_SetRect(&shark[4][2], 833,1308,950,1425);
	
	tb_SetRect(&shark[2][0], 0,1189,117,1306); //SE
	tb_SetRect(&shark[2][1], 0,1189,117,1306);
	tb_SetRect(&shark[2][2], 0,1308,117,1425);
	
	tb_SetRect(&shark[5][0], 714,1189,831,1306); //N
	tb_SetRect(&shark[5][1], 714,1189,831,1306);
	tb_SetRect(&shark[5][2], 714,1308,831,1425);
	
	tb_SetRect(&shark[0][0], 357,1189,474,1306); //SW
	tb_SetRect(&shark[0][1], 357,1189,474,1306);
	tb_SetRect(&shark[0][2], 357,1308,474,1425);
	
	tb_SetRect(&shark[3][0], 238,1189,355,1306); //E
	tb_SetRect(&shark[3][1], 238,1189,355,1306);
	tb_SetRect(&shark[3][2], 238,1308,355,1425);
	
	tb_SetRect(&shark[1][0], 476,1189,593,1306); //S
	tb_SetRect(&shark[1][1], 476,1189,593,1306);
	tb_SetRect(&shark[1][2], 476,1308,593,1425);
	
	
		// ---------------wrath frames
	tb_SetRect(&wrath[0][1],112,479,221,596);  //SW
	tb_SetRect(&wrath[0][0],0,479,109,596);
	tb_SetRect(&wrath[0][2],224,479,333,596);

	tb_SetRect(&wrath[1][1],112,599,221,716); 	//S
	tb_SetRect(&wrath[1][0],0,599,109,716);
	tb_SetRect(&wrath[1][2],224,599,333,716);

	tb_SetRect(&wrath[2][1],112,719,221,835); 	//SE
	tb_SetRect(&wrath[2][0],0,719,109,835);
	tb_SetRect(&wrath[2][2],224,719,333,835);

	tb_SetRect(&wrath[3][1],112,838,221,955); 	//E
	tb_SetRect(&wrath[3][0],0,838,109,955);
	tb_SetRect(&wrath[3][2],224,838,333,955);

	tb_SetRect(&wrath[4][1],112,0,221,117); 	//NE
	tb_SetRect(&wrath[4][0],0,0,109,117);
	tb_SetRect(&wrath[4][2],224,0,333,117);

	tb_SetRect(&wrath[5][1],112,120,221,237); 	//N
	tb_SetRect(&wrath[5][0],0,120,109,237);
	tb_SetRect(&wrath[5][2],224,120,333,237);
	
	tb_SetRect(&wrath[6][1],112,240,221,356); 	//NW
	tb_SetRect(&wrath[6][0],0,240,109,356);
	tb_SetRect(&wrath[6][2],224,240,333,356);

	tb_SetRect(&wrath[7][1],112,359,221,476); 	//W
	tb_SetRect(&wrath[7][0],0,359,109,476);
	tb_SetRect(&wrath[7][2],224,359,333,476);
	
	
	animationFrame2=0;
	animationFrame2Time=TickCount();
	animationFrame6=0;
	tb_SetRect(&animationRect[0],1,1,63,117); // air elemental
	tb_SetRect(&animationRect[1],64,1,126,117); // air elemental
	tb_SetRect(&animationRect[2],127,1,189,117); // air elemental
	tb_SetRect(&animationRect[3],190,1,252,117); // air elemental
	tb_SetRect(&animationRect[4],253,1,315,117); // air elemental
	tb_SetRect(&animationRect[5],316,1,378,117); // air elemental

	tb_SetRect(&animationRect[6],1073,894,1135,1010); // water elemental
	tb_SetRect(&animationRect[7],1136,894,1198,1010); // water elemental
	tb_SetRect(&animationRect[8],1199,894,1261,1010); // water elemental
	tb_SetRect(&animationRect[9],1262,894,1324,1010); // water elemental
	tb_SetRect(&animationRect[10],1325,894,1387,1010); // water elemental
	tb_SetRect(&animationRect[11],1388,894,1450,1010); // water elemental

	tb_SetRect(&bloodTrail[1],13,492,76,556); // blood
	tb_SetRect(&bloodTrail[2],77,492,140,556); // blood
	tb_SetRect(&bloodTrail[3],141,492,204,556); // blood

	tb_SetRect(&bloodTrail[4],1,557,64,621); // blood
	tb_SetRect(&bloodTrail[5],65,557,128,621); // blood
	tb_SetRect(&bloodTrail[6],129,557,192,621); // blood

	tb_SetRect(&bloodTrail[7],269,474,332,538); // blood
	tb_SetRect(&bloodTrail[8],333,474,396,538); // blood
	tb_SetRect(&bloodTrail[9],397,474,460,538); // blood

	tb_SetRect(&bloodTrail[10],579,487,642,551); // blood
	tb_SetRect(&bloodTrail[11],643,487,706,551); // blood
	tb_SetRect(&bloodTrail[12],707,487,770,551); // blood

	for (x=0;x<kMaxIcons;x++)
	{
		sourceGrid[x]	=	1;
		elevated[x]		=	true;		// specify exceptions below
		canWalkOn[x]	=	false;	// specify exceptions below
		canFlyOver[x]	= false;	// specify exceptions below
		canSwimIn[x]	= false;	// specify exceptions below
		showRaft[x]	= false;	// specify exceptions below

		base[x]				=	0;
		mapColor[x]		= 0;
		doorway[x]		= false;
		sign[x]				= false;
	}

	doorway[629]=true;

	doorway[633]=true;
	doorway[693]=true;
	doorway[694]=true;
	doorway[713]=true;
	doorway[782]=true;
	doorway[810]=true;
	doorway[823]=true;
	doorway[1085]=true;
	doorway[1088]=true;

	tb_SetRect(&srcRect[kHuman],1,1,64,104); // human
	tb_SetRect(&srcRect[kNPCHuman],1,1,64,104); // human
	tb_SetRect(&srcRect[kNPCHumanFemale],1,1,64,104);//humanwfemalebody

	tb_SetRect(&srcRect[kGazer],379,1,442,65); // gazer
	tb_SetRect(&srcRect[kAirElemental],1,1,63,117); // air elemental
	tb_SetRect(&srcRect[kReaper],607,745,834,923); // reaper
	tb_SetRect(&srcRect[kBrigandShelter1],607,1038,742,1134); // brigand shelter
	tb_SetRect(&srcRect[kBrigandShelter2],998,1052,1133,1148); // brigand shelter
	tb_SetRect(&srcRect[kBrigandShelter3],1134,1052,1269,1148); // brigand shelter
//	tb_SetRect(&srcRect[kSorcererTower],610,569,985,894); // sorcerer tower

	tb_SetRect(&srcRect[256],	1,	1,		64,		65); // grass
	tb_SetRect(&srcRect[257],	1,	66,		64,		130); // grass
	tb_SetRect(&srcRect[258],	1,	131,	64,		195); // grass
	tb_SetRect(&srcRect[259],	1,	196,	64,		260); // grass
	tb_SetRect(&srcRect[260],	1,	261,	64,		325); // grass
	tb_SetRect(&srcRect[261],	65,	1,	128,		65); // grass
	tb_SetRect(&srcRect[262],	65,	66,	128,		130); // grass
	tb_SetRect(&srcRect[263],	129,	66,	192,		130); // grass

	tb_SetRect(&srcRect[264],	1,	326,	64,		390); // mud
	tb_SetRect(&srcRect[265],	65,	313,	128,		377); // mud
	tb_SetRect(&srcRect[266],	65,	378,	128,		442); // mud

	tb_SetRect(&srcRect[275],	129,	347,	192,		411); // water

	tb_SetRect(&srcRect[359],193,1,262,181); // tree
	tb_SetRect(&srcRect[360],263,1,356,171); // tree
	tb_SetRect(&srcRect[361],357,1,428,179); // tree
	tb_SetRect(&srcRect[362],429,1,516,142); // tree
	tb_SetRect(&srcRect[363],517,1,644,153); // tree
	tb_SetRect(&srcRect[364],65,131,162,312); // tree
	tb_SetRect(&srcRect[365],163,182,314,346); // tree
	tb_SetRect(&srcRect[366],315,180,420,293); // tree
	tb_SetRect(&srcRect[367],429,143,498,328); // tree
	tb_SetRect(&srcRect[368],499,154,598,304); // tree

	tb_SetRect(&srcRect[369],1336,1,1399,172); // new tree
	tb_SetRect(&srcRect[370],1281,321,1434,492); // new tree
	tb_SetRect(&srcRect[371],1281,493,1434,664); // new tree
	tb_SetRect(&srcRect[372],1281,665,1430,836); // new tree
	tb_SetRect(&srcRect[373],1281,837,1380,1008); // new tree

	gr_CreateElement(374,	65,	131, 162, 312,kIceBMP, 398, kElevated, 	kNullDomain, kMapNull);	 // snow tree

	gr_CreateElement(375,	193,	1, 272, 152,kIceBMP, 385, kElevated, 	kNullDomain, kMapNull);	 // palm tree // gs add to lumberjacking
	gr_CreateElement(376,	273,	1, 340, 161,kIceBMP, 385, kElevated, 	kNullDomain, kMapNull);	 // palm tree // gs add to lumberjacking
	gr_CreateElement(377,	341,	1, 431, 167,kIceBMP, 385, kElevated, 	kNullDomain, kMapNull);	 // palm tree // gs add to lumberjacking
	gr_CreateElement(378,	432,	1, 527, 155,kIceBMP, 385, kElevated, 	kNullDomain, kMapNull);	 // palm tree // gs add to lumberjacking

	gr_CreateElement(385,	163,	282,	226,		346, kIceBMP, 	 0, 	kNotElevated, 	kLand, kMapNull); // palm tree base

	gr_CreateElement(386,	645,	1,	708,		152, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 1 // gs add ice rock to mining, etc.
	gr_CreateElement(387,	709,	1,	759,		117, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 2
	gr_CreateElement(388,	760,	1,	821,		148, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 3
	gr_CreateElement(389,	822,	1,	881,		101, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 4
	gr_CreateElement(390,	882,	1,	951,		172, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 5
	gr_CreateElement(391,	599,	154,	682,		342, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 6
	gr_CreateElement(392,	683,	153,	766,		277, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 7
	gr_CreateElement(393,	767,	149,	856,		309, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 8
	gr_CreateElement(394,	857,	173,	928,		338, kIceBMP, 	 399, 	kElevated, 	kNullDomain, kMapNull); // ice rock 9

	gr_CreateElement(395,	683, 278,	746, 346, kIceBMP, 	 399, 	kElevated, 	kLand, kMapNull);		// ice latter down

	gr_CreateElement(396,	1,	66, 64, 130,kIceBMP, 0, kNotElevated, 	kLand, kMapNull);	 // snow structure shadow
	gr_CreateElement(397,	1,	131, 64, 195,kIceBMP, 0, kNotElevated, 	kLand, kMapNull);	 // snow structure shadow

	gr_CreateElement(398,	129,	66, 192, 130,kIceBMP, 0, kNotElevated, 	kLand, kMapNull);	 // snow tree base
	gr_CreateElement(399,	1,	1, 64, 65,kIceBMP, 0, kNotElevated, 	kLand, kMapNull);	 // snow

	tb_SetRect(&srcRect[400],	321,	347,	384,		411); // ice
	tb_SetRect(&srcRect[401],	257,	347,	320,		411); // road
	tb_SetRect(&srcRect[402],	193,	347,	256,		411); // sand


	tb_SetRect(&srcRect[403],	385,	347,	448,		411); // road/grass border
	tb_SetRect(&srcRect[404],	385,	412,	448,		476); // road/grass border
	tb_SetRect(&srcRect[405],	385,	477,	448,		541); // road/grass border
	tb_SetRect(&srcRect[406],	385,	542,	448,		606); // road/grass border

	tb_SetRect(&srcRect[407],	449,	347,	512,		411); // road/grass border
	tb_SetRect(&srcRect[408],	449,	412,	512,		476); // road/grass border
	tb_SetRect(&srcRect[409],	449,	477,	512,		541); // road/grass border
	tb_SetRect(&srcRect[410],	449,	542,	512,		606); // road/grass border

	tb_SetRect(&srcRect[411],	513,	347,	576,		411); // road/grass border
	tb_SetRect(&srcRect[412],	513,	412,	576,		476); // road/grass border
	tb_SetRect(&srcRect[413],	513,	477,	576,		541); // road/grass border
	tb_SetRect(&srcRect[414],	513,	542,	576,		606); // road/grass border

	tb_SetRect(&srcRect[415],	577,	347,	640,		411); // road/grass border
	tb_SetRect(&srcRect[416],	577,	412,	640,		476); // road/grass border
	tb_SetRect(&srcRect[417],	577,	477,	640,		541); // road/grass border
	tb_SetRect(&srcRect[418],	577,	542,	640,		606); // road/grass border

	//tb_SetRect(&srcRect[419],	641,	347,	704,		411); // road/grass border
	//tb_SetRect(&srcRect[420],	641,	412,	704,		476); // road/grass border
	//tb_SetRect(&srcRect[421],	641,	477,	704,		541); // road/grass border
	//tb_SetRect(&srcRect[422],	641,	542,	704,		606); // road/grass border

	tb_SetRect(&srcRect[423],	641,	347,	704,		411); // road/grass border
	tb_SetRect(&srcRect[424],	641,	412,	704,		476); // road/grass border
	tb_SetRect(&srcRect[425],	641,	477,	704,		541); // road/grass border
	tb_SetRect(&srcRect[426],	641,	542,	704,		606); // road/grass border

	tb_SetRect(&srcRect[427],	705,	347,	768,		411); // road/grass border
	tb_SetRect(&srcRect[428],	705,	412,	768,		476); // road/grass border
	tb_SetRect(&srcRect[429],	705,	477,	768,		541); // road/grass border
	tb_SetRect(&srcRect[430],	705,	542,	768,		606); // road/grass border

	tb_SetRect(&srcRect[431],	769,	347,	832,		411); // road/grass border
	tb_SetRect(&srcRect[432],	769,	412,	832,		476); // road/grass border
	tb_SetRect(&srcRect[433],	769,	477,	832,		541); // road/grass border
	tb_SetRect(&srcRect[434],	769,	542,	832,		606); // road/grass border

	tb_SetRect(&srcRect[435],	833,	347,	896,		411); // road/grass border
	tb_SetRect(&srcRect[436],	833,	412,	896,		476); // road/grass border
	tb_SetRect(&srcRect[437],	833,	477,	896,		541); // road/grass border
	tb_SetRect(&srcRect[438],	833,	542,	896,		606); // road/grass border

//-----

	tb_SetRect(&srcRect[439],	385-384,	347+260,	448-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[440],	385-384,	412+260,	448-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[441],	385-384,	477+260,	448-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[442],	385-384,	542+260,	448-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[443],	449-384,	347+260,	512-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[444],	449-384,	412+260,	512-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[445],	449-384,	477+260,	512-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[446],	449-384,	542+260,	512-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[447],	513-384,	347+260,	576-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[448],	513-384,	412+260,	576-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[449],	513-384,	477+260,	576-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[450],	513-384,	542+260,	576-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[451],	577-384,	347+260,	640-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[452],	577-384,	412+260,	640-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[453],	577-384,	477+260,	640-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[454],	577-384,	542+260,	640-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[455],	641-384,	347+260,	704-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[456],	641-384,	412+260,	704-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[457],	641-384,	477+260,	704-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[458],	641-384,	542+260,	704-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[459],	705-384,	347+260,	768-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[460],	705-384,	412+260,	768-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[461],	705-384,	477+260,	768-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[462],	705-384,	542+260,	768-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[463],	769-384,	347+260,	832-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[464],	769-384,	412+260,	832-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[465],	769-384,	477+260,	832-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[466],	769-384,	542+260,	832-384,		606+260); // water/grass border

	tb_SetRect(&srcRect[467],	833-384,	347+260,	896-384,		411+260); // water/grass border
	tb_SetRect(&srcRect[468],	833-384,	412+260,	896-384,		476+260); // water/grass border
	tb_SetRect(&srcRect[469],	833-384,	477+260,	896-384,		541+260); // water/grass border
	tb_SetRect(&srcRect[470],	833-384,	542+260,	896-384,		606+260); // water/grass border

//-----

	tb_SetRect(&srcRect[471],	385+128,	347+260,	448+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[472],	385+128,	412+260,	448+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[473],	385+128,	477+260,	448+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[474],	385+128,	542+260,	448+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[475],	449+128,	347+260,	512+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[476],	449+128,	412+260,	512+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[477],	449+128,	477+260,	512+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[478],	449+128,	542+260,	512+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[479],	513+128,	347+260,	576+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[480],	513+128,	412+260,	576+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[481],	513+128,	477+260,	576+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[482],	513+128,	542+260,	576+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[483],	577+128,	347+260,	640+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[484],	577+128,	412+260,	640+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[485],	577+128,	477+260,	640+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[486],	577+128,	542+260,	640+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[487],	641+128,	347+260,	704+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[488],	641+128,	412+260,	704+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[489],	641+128,	477+260,	704+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[490],	641+128,	542+260,	704+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[491],	705+128,	347+260,	768+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[492],	705+128,	412+260,	768+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[493],	705+128,	477+260,	768+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[494],	705+128,	542+260,	768+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[495],	769+128,	347+260,	832+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[496],	769+128,	412+260,	832+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[497],	769+128,	477+260,	832+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[498],	769+128,	542+260,	832+128,		606+260); // sand/grass border

	tb_SetRect(&srcRect[499],	833+128,	347+260,	896+128,		411+260); // sand/grass border
	tb_SetRect(&srcRect[500],	833+128,	412+260,	896+128,		476+260); // sand/grass border
	tb_SetRect(&srcRect[501],	833+128,	477+260,	896+128,		541+260); // sand/grass border
	tb_SetRect(&srcRect[502],	833+128,	542+260,	896+128,		606+260); // sand/grass border

//-----

	gr_CreateElement(503,	385-384,	347+260,	448-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(504,	385-384,	412+260,	448-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(505,	385-384,	477+260,	448-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(506,	385-384,	542+260,	448-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(507,	449-384,	347+260,	512-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(508,	449-384,	412+260,	512-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(509,	449-384,	477+260,	512-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(510,	449-384,	542+260,	512-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(511,	513-384,	347+260,	576-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(512,	513-384,	412+260,	576-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(513,	513-384,	477+260,	576-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(514,	513-384,	542+260,	576-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(515,	577-384,	347+260,	640-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(516,	577-384,	412+260,	640-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(517,	577-384,	477+260,	640-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(518,	577-384,	542+260,	640-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(519,	641-384,	347+260,	704-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(520,	641-384,	412+260,	704-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(521,	641-384,	477+260,	704-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(522,	641-384,	542+260,	704-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(523,	705-384,	347+260,	768-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(524,	705-384,	412+260,	768-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(525,	705-384,	477+260,	768-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(526,	705-384,	542+260,	768-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(527,	769-384,	347+260,	832-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(528,	769-384,	412+260,	832-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(529,	769-384,	477+260,	832-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(530,	769-384,	542+260,	832-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	gr_CreateElement(531,	833-384,	347+260,	896-384,		411+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(532,	833-384,	412+260,	896-384,		476+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(533,	833-384,	477+260,	896-384,		541+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water
	gr_CreateElement(534,	833-384,	542+260,	896-384,		606+260,kIceBMP,0, 		kNotElevated, 	kWater, kMapNull);	 // snow/water

	//for (i=503;i<535;i++) // gs 9/25
	//	sourceGrid[i]			=	kIceBMP;

//	***** tb_SetRect(&srcRect[535],	129,	1,	192,		65); // satchel
	tb_SetRect(&srcRect[535],	321,	347,	384,		411); // ether

	tb_SetRect(&srcRect[536],	645,	1,	708,		152); // rock 1
	tb_SetRect(&srcRect[537],	709,	1,	759,		117); // rock 2
	tb_SetRect(&srcRect[538],	760,	1,	821,		148); // rock 3
	tb_SetRect(&srcRect[539],	822,	1,	881,		101); // rock 4
	tb_SetRect(&srcRect[540],	882,	1,	951,		172); // rock 5
	tb_SetRect(&srcRect[541],	599,	154,	682,		342); // rock 6
	tb_SetRect(&srcRect[542],	683,	153,	766,		277); // rock 7
	tb_SetRect(&srcRect[543],	767,	149,	856,		309); // rock 8
	tb_SetRect(&srcRect[544],	857,	173,	928,		338); // rock 9

	tb_SetRect(&srcRect[545],	513,	867,	576,		985); // log cabin
	tb_SetRect(&srcRect[546],	577,	867,	640,		985); // log cabin
	tb_SetRect(&srcRect[547],	641,	867,	704,		985); // log cabin
	tb_SetRect(&srcRect[548],	705,	867,	768,		1008); // log cabin
	tb_SetRect(&srcRect[549],	769,	867,	832,		1008); // log cabin
	tb_SetRect(&srcRect[550],	513,	986,	576,		1129); // log cabin
	tb_SetRect(&srcRect[551],	577,	986,	640,		1120); // log cabin
	tb_SetRect(&srcRect[552],	641,	986,	704,		1129); // log cabin

	tb_SetRect(&srcRect[553],	1025,	802,	1088,		866); // brick floor

	gr_CreateElement(554,			  1, 391, 	64, 	 466, 			kMainBMP, 	  567, 	kElevated, 			kNullDomain, kMapNull);							// grave
	gr_CreateElement(555,			  1, 467, 	64, 	 537, 			kMainBMP, 	  567, 	kElevated, 			kNullDomain, kMapNull);							// grave
	gr_CreateElement(556,			 65, 443,  128, 	 512, 			kMainBMP, 	  567, 	kElevated, 			kNullDomain, kMapNull);							// grave
	gr_CreateElement(557,			 65, 513,  128, 	 587, 			kMainBMP, 	  567, 	kElevated, 			kNullDomain, kMapNull);							// grave

	gr_CreateElement(558,			897, 357,  960, 	 472, 			kMainBMP, 	  257, 	kElevated, 			kNullDomain, kMapNull);							// fence
	gr_CreateElement(559,			961, 357,  1024, 	 472, 			kMainBMP, 	  258, 	kElevated, 			kNullDomain, kMapNull);							// fence

	gr_CreateElement(560,			952,   1,  1015, 	 116, 			kMainBMP, 	  259, 	kElevated, 			kNullDomain, kMapNull);							// fence corner
	gr_CreateElement(561,		 1016,   1,  1079, 	 116, 			kMainBMP, 	  259, 	kElevated, 			kNullDomain, kMapNull);							// fence corner
	gr_CreateElement(562,			952, 117,  1015, 	 232, 			kMainBMP, 	  259, 	kElevated, 			kNullDomain, kMapNull);							// fence corner
	gr_CreateElement(563,		 1016, 117,  1079, 	 232, 			kMainBMP, 	  259, 	kElevated, 			kNullDomain, kMapNull);							// fence corner

	gr_CreateElement(564,		 929, 233,   992, 	 348, 			kMainBMP, 	    0, 	kElevated, 			kNullDomain, kMapNull);							// fence gate
	gr_CreateElement(565,		 993, 233,  1056, 	 348, 			kMainBMP, 	    0, 	kElevated, 			kNullDomain, kMapNull);							// fence gate
	gr_CreateElement(566,		1057, 233,  1174, 	 368, 			kMainBMP, 	  263, 	kElevated, 			kNullDomain, kMapNull);							// dead tree

	gr_CreateElement(567,		1080, 157,  1143, 	 232, 			kMainBMP, 	   0, 	kNotElevated, 	kNullDomain, kMapNull);							// grave base

	tb_SetRect(&srcRect[568],	897,	488,	960,		606); // log cabin

	gr_CreateElement(569,			  1, 		1, 		  64, 		212, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// arch
	gr_CreateElement(570,			 65, 		1, 		 128, 		244, 			kHavenBMP, 	575, 	kElevated, 			kLand, kMapNull);							// arch
	gr_CreateElement(571,			129, 		1, 		 192, 		266, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// arch
	gr_CreateElement(572,			  1, 		213, 		64, 		478, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// arch
	gr_CreateElement(573,			 65, 	  245, 	 128, 		488, 			kHavenBMP, 	575, 	kElevated, 			kLand, kMapNull);							// arch
	gr_CreateElement(574,			129, 	  267, 	 192, 		478, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// arch
	gr_CreateElement(575,			193, 		  1, 	 256, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// gray brick floor
	gr_CreateElement(576,			193, 	   66, 	 256, 		130, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// red brick floor
	gr_CreateElement(577,			193, 	  131, 	 256, 		312, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// venus
	gr_CreateElement(578,			257, 	  1, 	 	 320, 		 90, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);				// shrub
	gr_CreateElement(579,			257, 	  91, 	 320, 		155, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(580,			257, 	 156, 	 320, 		220, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(581,			257, 	 221, 	 320, 		285, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(582,			193, 	 313, 	 256, 		377, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(583,			193, 	 378, 	 256, 		442, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps

	gr_CreateElement(584,			257, 	 286, 	 320, 		350, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(585,			257, 	 351, 	 320, 		415, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(586,			257, 	 416, 	 320, 		480, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps

	gr_CreateElement(587,			321, 	   1, 	 384, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(588,			321, 	  66, 	 384, 		130, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps

	gr_CreateElement(589,			321, 	 131, 	 384, 		290, 			kHavenBMP, 	718, 	kElevated, 			kNullDomain, kMapNull);							// column
	gr_CreateElement(590,			321, 	 291, 	 384, 		355, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps
	gr_CreateElement(591,			321, 	 356, 	 384, 		420, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// steps

	gr_CreateElement(592,			385, 	   1, 	 448, 		167, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);							// brick wall
	gr_CreateElement(593,			385, 	 168, 	 448, 		334, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);							// brick wall
	gr_CreateElement(594,			449, 	   1, 	 512, 		167, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);							// brick wall

	gr_CreateElement(595,			385, 	 335, 	 448, 		490, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);							// bookcase
	gr_CreateElement(596,			449, 	 168, 	 512, 		323, 			kHavenBMP, 	575, 	kElevated, 			kNullDomain, kMapNull);							// bookcase

	gr_CreateElement(597,			513, 	   1, 	 576, 		191, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall
	gr_CreateElement(598,			577, 	   1, 	 640, 		191, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall
	gr_CreateElement(599,			641, 	   1, 	 704, 		191, 			kHavenBMP, 	967, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall

	gr_CreateElement(600,			513, 	 192, 	 576, 		434, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall
	gr_CreateElement(601,			577, 	 192, 	 640, 		434, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall
	gr_CreateElement(602,			641, 	 192, 	 704, 		434, 			kHavenBMP, 	967, 	kElevated, 			kNullDomain, kMapNull);							// Castle wall

	gr_CreateElement(603,			449, 	 324, 	 512, 		388, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(604,			449, 	 389, 	 512, 		453, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// Moat cliff

	gr_CreateElement(605,			513, 	 435, 	 576, 		499, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(606,			577, 	 435, 	 640, 		499, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// Moat cliff

	gr_CreateElement(607,			641, 	 435, 	 704, 		499, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(608,			705, 	   1, 	 768, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Brick
	gr_CreateElement(609,			705, 	 131, 	 768, 		373, 			kHavenBMP, 	612, 	kElevated, 			kLand, kMapNull);										// Castle Front

	gr_CreateElement(610,			705, 	 131, 	 768, 		373, 			kHavenBMP, 	608, 	kElevated, 			kLand, kMapNull);										// Castle Front
	gr_CreateElement(611,			705, 	 374, 	 768, 		533, 			kHavenBMP, 	614, 	kElevated, 			kNullDomain, kMapNull);							// Castle Column

	gr_CreateElement(612,			769, 	   1, 	 832, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle shadow brick
	gr_CreateElement(613,			769, 	  66, 	 832, 		130, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle shadow brick
	gr_CreateElement(614,			769, 	 131, 	 832, 		195, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle shadow brick
	gr_CreateElement(615,			769, 	 196, 	 832, 		260, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle shadow brick
	gr_CreateElement(616,			769, 	 261, 	 832, 		325, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle light brick

	gr_CreateElement(617,			769, 	 326, 	 832, 		390, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Drawbridge
	gr_CreateElement(618,			833, 	   1, 	 896, 		191, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// torch
	gr_CreateElement(619,			833, 	 192, 	 896, 		382, 			kHavenBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// torch
	gr_CreateElement(620,			705, 	  66, 	 768, 		130, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Drawbridge shadow
	gr_CreateElement(621,			769, 	 391, 	 832, 		455, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Drawbridge shadow
	gr_CreateElement(622,			833, 	 383, 	 896, 		447, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Drawbridge Brick

	gr_CreateElement(623,			769, 	 456, 	 832, 		520, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(624,			833, 	 448, 	 896, 		512, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(625,			321, 	 421, 	 384, 		485, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff
	gr_CreateElement(626,			193, 	 443, 	 256, 		507, 			kHavenBMP, 	0, 		kNotElevated, 	kNullDomain, kMapNull);							// Moat cliff

	gr_CreateElement(627,			449, 	 454, 	 512, 		518, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// shadow base
	gr_CreateElement(628,			257, 	 481, 	 320, 		545, 			kHavenBMP, 	627, 	kElevated, 			kNullDomain, kMapNull);							// castle table

	gr_CreateElement(629,			  1, 	 479, 	  64, 		669, 			kHavenBMP, 	608, 	kElevated, 			kLand, kMapNull);										// castle 1 door part 1
	gr_CreateElement(630,			 65, 	 489, 	 128, 		679, 			kHavenBMP, 	608, 	kElevated, 			kNullDomain, kMapNull);							// castle 1 door part 2
	gr_CreateElement(631,			 129,  479, 	 192, 		669, 			kHavenBMP, 	632, 	kElevated, 			kNullDomain, kMapNull);							// castle open door 1
	gr_CreateElement(632,			 193,  508, 	 256, 		572, 			kHavenBMP, 	608, 	kNotElevated, 	kLand, kMapNull);										// castle door shadow

	gr_CreateElement(633,			 513,  500, 	 576, 		690, 			kHavenBMP, 	608, 	kElevated, 			kLand, kMapNull);										// castle 2 door part 1
	gr_CreateElement(634,			 577,  500, 	 640, 		690, 			kHavenBMP, 	608, 	kElevated, 			kNullDomain, kMapNull);							// castle 2 door part 2
	gr_CreateElement(635,			 641,  500, 	 704, 		690, 			kHavenBMP, 	632, 	kElevated, 			kNullDomain, kMapNull);							// castle open door 2

	gr_CreateElement(636,			 321,  486, 	 384, 		550, 			kHavenBMP, 	613, 	kElevated, 			kNullDomain, kMapNull);							// castle table }
	gr_CreateElement(637,			 385,  491, 	 448, 		555, 			kHavenBMP, 	627, 	kElevated, 			kNullDomain, kMapNull);							// castle table/castle

	gr_CreateElement(638,			 449,  519, 	 512, 		583, 			kHavenBMP, 	0, 	kNotElevated, 			kNullDomain, kMapNull);						// chair shadow

	gr_CreateElement(639,			 193,  573, 	 256, 		653, 			kHavenBMP, 	638, 	kElevated, 			kNullDomain, kMapNull);							// castle chair
	gr_CreateElement(640,			 257,  546, 	 320, 		626, 			kHavenBMP, 	638, 	kElevated, 			kNullDomain, kMapNull);							// castle chair
	gr_CreateElement(641,			 321,  551, 	 384, 		631, 			kHavenBMP, 	638, 	kElevated, 			kNullDomain, kMapNull);							// castle chair
	gr_CreateElement(642,			 385,  556, 	 448, 		636, 			kHavenBMP, 	638, 	kElevated, 			kNullDomain, kMapNull);							// castle chair

	gr_CreateElement(643,			 449,  584, 	 512, 		648, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// * Barrel

	gr_CreateElement(644,			 705,  534, 	 768, 		695, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle stove
	gr_CreateElement(645,			 833,  513, 	 896, 		716, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle stove

	gr_CreateElement(646,			 257,  627, 	 320, 		691, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle stool
	gr_CreateElement(647,			 769,  521, 	 832, 		711, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle pans
	gr_CreateElement(648,			 897,    1, 	 960, 		191, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle pans

	gr_CreateElement(649,			 897,  192, 	 960, 		382, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table
	gr_CreateElement(650,			 897,  383, 	 960, 		447, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table
	gr_CreateElement(651,			 897,  448, 	 960, 		512, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table
	gr_CreateElement(652,			 897,  513, 	 960, 		577, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table
	gr_CreateElement(653,			 897,  578, 	 960, 		642, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table
	gr_CreateElement(654,			 897,  643, 	 960, 		707, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle table

	gr_CreateElement(655,			 961,    1, 	1024, 		124, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle chest

	gr_CreateElement(656,			 961,  125, 	1024, 		202, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle hay
	gr_CreateElement(657,			 321,  632, 	 384, 		696, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle firewood
	gr_CreateElement(658,			 961,  203, 	1024, 		445, 			kHavenBMP, 	659, 	kElevated, 			kLand, kMapNull);							// castle gate
	gr_CreateElement(659,			 961,  446, 	1024, 		510, 			kHavenBMP, 	  0, 	kNotElevated, 	kLand, kMapNull);										// castle gate base


	gr_CreateElement(660,			 961,  511, 	1024, 		575, 			kHavenBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapNull);							// moat cliff
	gr_CreateElement(661,			 961,  576, 	1024, 		640, 			kHavenBMP, 	  0, 	kNotElevated, 	kLand, kMapNull);										// moat cliff

	gr_CreateElement(662,			 961,  641, 	1024, 		705, 			kHavenBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapNull);							// moat cliff shadow
	gr_CreateElement(663,			 385,  637, 	 448, 		701, 			kHavenBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapNull);							// water shadow

	gr_CreateElement(664,			 1025,  1, 	  1088, 		 74, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// guard bed
	gr_CreateElement(665,			 1025,  75, 	1088, 		139, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// guard bed/chest

	gr_CreateElement(666,			 1025,  140, 	1088, 		222, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// fountain

	gr_CreateElement(667,			 1025,  223, 	1088, 		413, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle shields
	gr_CreateElement(668,			 1025,  414, 	1088, 		604, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// castle spears
	gr_CreateElement(669,			 1025,  605, 	1088, 		669, 			kHavenBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);							// water barrel

	gr_CreateElement(670,			 193,  654, 	256, 		745, 			kHavenBMP, 	  627, 	kElevated, 			kNullDomain, kMapNull);							// bed
	gr_CreateElement(671,			 129,  670, 	192, 		772, 			kHavenBMP, 	    0, 	kElevated, 			kNullDomain, kMapNull);							// dresser
	gr_CreateElement(672,			 449,  649, 	512, 		713, 			kHavenBMP, 	    0, 	kNotElevated, 	kNullDomain, kMapNull);							// chest

	gr_CreateElement(673,			   1,  670, 	 64, 		734, 			kHavenBMP, 	    0, 	kNotElevated, 	kNullDomain, kMapNull);							// anvil

	gr_CreateElement(674,			  65,  680, 	128, 		791, 			kHavenBMP, 	    0, 	kElevated, 			kNullDomain, kMapNull);							// throne

	gr_CreateElement(675,			  513,  867, 	576, 		1000, 			kMainBMP, 	  697, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(676,			  577,  867, 	640, 		1000, 			kMainBMP, 	  705, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(677,			  641,  867, 	704, 		1000, 			kMainBMP, 	  695, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(678,			  705,  867, 	768, 		1000, 			kMainBMP, 	  699, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(679,			  769,  867, 	832, 		1000, 			kMainBMP, 	  706, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(680,			  833,  867, 	896, 		1000, 			kMainBMP, 	  700, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(681,			  897,  867, 	960, 		1000, 			kMainBMP, 	  698, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(682,			  961,  867, 1024, 		1000, 			kMainBMP, 	  698, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(683,			 1025,  867, 1088, 		1000, 			kMainBMP, 	  697, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin fireplace

	gr_CreateElement(684,			  513, 1001, 	576, 		1134, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(685,			  577, 1001, 	640, 		1134, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(686,			  641, 1001, 	704, 		1134, 			kMainBMP, 	  697, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(687,			  705, 1001, 	768, 		1134, 			kMainBMP, 	  695, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(688,			  769, 1001, 	832, 		1134, 			kMainBMP, 	  707, 	kElevated, 			kLand, kMapStructure);										// log cabin door
	gr_CreateElement(689,			  833, 1001, 	896, 		1134, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(690,			  897, 1001, 	960, 		1134, 			kMainBMP, 	  708, 	kElevated, 			kLand, kMapStructure);										// log cabin door
	gr_CreateElement(691,			  961, 1001, 1024, 		1134, 			kMainBMP, 	  698, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(692,			 1025, 1001, 1088, 		1134, 			kMainBMP, 	  695, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin fireplace

	gr_CreateElement(693,			  897,  473,  960, 		 606, 			kMainBMP, 	  707, 	kElevated, 			kLand, kMapStructure);										// log cabin closed door
	gr_CreateElement(694,			  961,  473, 1024, 		 606, 			kMainBMP, 	  708, 	kElevated, 			kLand, kMapStructure);										// log cabin closed door

	gr_CreateElement(695,			  129,  412,  192, 		 476, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(696,			  193,  412,  256, 		 476, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(697,			  257,  412,  320, 		 476, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(698,			  321,  412,  384, 		 476, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(699,			  129,  477,  192, 		 541, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(700,			  193,  477,  256, 		 541, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base

	gr_CreateElement(701,			  641, 1001, 	704, 		1134, 			kMainBMP, 	  698, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin window
	gr_CreateElement(702,			  705, 1001, 	768, 		1134, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin window
	gr_CreateElement(703,			  513,  867, 	576, 		1000, 			kMainBMP, 	  698, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(704,			  641,  867, 	704, 		1000, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin

	gr_CreateElement(705,			  257,  477,  320, 		 541, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base
	gr_CreateElement(706,			  321,  477,  384, 		 541, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin brick base

	gr_CreateElement(707,			  129,  542,  192, 		 606, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin door brick base
	gr_CreateElement(708,			  193,  542,  256, 		 606, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin door brick base

	gr_CreateElement(709,		   1025, 570,  1088, 		 703, 			kMainBMP, 	  695, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin intersection

	gr_CreateElement(710,			  257,  542,  320, 		 606, 			kMainBMP, 	    0, 	kNotElevated, 	kLand, kMapStructure);										// log cabin door brick base
	gr_CreateElement(711,			  769, 1001, 	832, 		1134, 			kMainBMP, 	  710, 	kElevated, 			kLand, kMapStructure);										// log cabin door
	gr_CreateElement(712,			  833, 1001, 	896, 		1134, 			kMainBMP, 	  710, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin door
	gr_CreateElement(713,			  897,  473,  960, 		 606, 			kMainBMP, 	  710, 	kElevated, 			kLand, kMapStructure);										// log cabin closed door

	gr_CreateElement(714,			  1025, 436, 	1088, 	 569, 			kMainBMP, 	  706, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin intersection

	gr_CreateElement(715,			  257, 692, 	320, 	 756, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// brick shadow
	gr_CreateElement(716,			  321, 697, 	384, 	 761, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// brick shadow
	gr_CreateElement(717,			  385, 702, 	448, 	 766, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// brick shadow
	gr_CreateElement(718,			  449, 714, 	512, 	 778, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// brick shadow

	gr_CreateElement(719,			  1080, 74, 	1143, 	 156, 			kMainBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);					// pier
	gr_CreateElement(720,			  1089, 369, 	1152, 	 451, 			kMainBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);					// pier
	gr_CreateElement(721,			  1089, 452, 	1152, 	 534, 			kMainBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);					// pier
	gr_CreateElement(722,			  1089, 535, 	1152, 	 623, 			kMainBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);					// pier

	gr_CreateElement(723,			  1089, 706, 	1152, 	 788, 			kMainBMP, 	0, 	kNotElevated, 			kNullDomain, kMapStructure);					// pier barrel base
	gr_CreateElement(724,			  1089, 624, 	1152, 	 705, 			kMainBMP, 723, 	kElevated, 					kNullDomain, kMapStructure);					// pier barrel

	gr_CreateElement(725,			  577, 691, 	640, 	 755, 			kHavenBMP, 0, 	kNotElevated, 					kNullDomain, kMapStructure);					// coffin
	gr_CreateElement(726,			  641, 691, 	704, 	 755, 			kHavenBMP, 0, 	kNotElevated, 					kNullDomain, kMapStructure);					// coffin

	gr_CreateElement(727,			  833,  867, 	896, 		1000, 			kMainBMP, 	  728, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin inner corner

	gr_CreateElement(728,			  321,  542, 	384, 		606, 			kMainBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin inner brick base
	gr_CreateElement(729,			  1,  538, 	64, 		602, 			kMainBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin inner brick base
	gr_CreateElement(730,			  577,  867, 	640, 		1000, 			kMainBMP, 	  729, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin inner corner
	gr_CreateElement(731,		   1025, 570,  1088, 		 703, 			kMainBMP, 	  696, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin intersection

	gr_CreateElement(732,		   1089, 789,  1152, 		 853, 			kMainBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapStructure);							// log cabin intersection base
	gr_CreateElement(733,		   1089, 854,  1152, 		 987, 			kMainBMP, 	  732, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin intersection

	gr_CreateElement(734,		   1089, 988,  1152, 		1069, 			kMainBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin bed

	gr_CreateElement(735,		   65, 867,  128, 		1000, 			kMainBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin dresser

	gr_CreateElement(736,		  129, 867,  192, 		1000, 			kMainBMP, 	  710, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin torch

	gr_CreateElement(737,		  1089, 1070,  1152, 		1134, 			kMainBMP, 	745, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin table

	gr_CreateElement(738,		  193, 867,  256, 		931, 					kMainBMP, 	739, 	kElevated, 			kNullDomain, kMapStructure);							// blacksmith fire
	gr_CreateElement(739,		  257, 867,  320, 		931, 					kMainBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);							// blacksmith fire base

	gr_CreateElement(740,		  193, 932,  256, 		996, 					kMainBMP, 	741, 	kElevated, 			kNullDomain, kMapStructure);							// blacksmith anvil
	gr_CreateElement(741,		  257, 932,  320, 		996, 					kMainBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);							// blacksmith anvil base

	gr_CreateElement(742,		  193, 997,  256, 	 1061, 					kMainBMP, 	743, 	kElevated, 			kNullDomain, kMapStructure);							// blacksmith wood
	gr_CreateElement(743,		  257, 997,  320, 	 1061, 					kMainBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);							// blacksmith wood base

	gr_CreateElement(744,		  193, 1062,  256, 		1126, 				kMainBMP, 	745, 	kElevated, 			kNullDomain, kMapStructure);							// blacksmith table
	gr_CreateElement(745,		  257, 1062,  320, 		1126, 				kMainBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);							// blacksmith table base

	gr_CreateElement(746,		  1025, 802,  1088, 	866, 				  kMainBMP, 	  0, 	kNotElevated, 	kLand, kMapStructure);											// blacksmith brick

	gr_CreateElement(747,		  705, 	   1, 	 768, 		 65, 				  kHavenBMP, 	  0, 	kNotElevated, 	kLand, kMapStructure);											// bank brick
	gr_CreateElement(748,			769, 	   1, 	 832, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// bank shadow brick
	gr_CreateElement(749,			769, 	  66, 	 832, 		130, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// bank shadow brick
	gr_CreateElement(750,			 961,  203, 	1024, 		445, 			kHavenBMP, 	659, 	kElevated, 			kLand, kMapNull);										// bank gate

	gr_CreateElement(751,			 1089,  1, 	1152, 		65, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(752,			 1089,  66, 	1152, 		130, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(753,			 1089,  131, 	1152, 		195, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(754,			 1089,  196, 	1152, 		260, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(755,			 1089,  261, 	1152, 		325, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(756,			 1089,  326, 	1152, 		390, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(757,			 1089,  391, 	1152, 		455, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps
	gr_CreateElement(758,			 1089,  456, 	1152, 		520, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// bank steps

	gr_CreateElement(759,			 449,  867, 	512, 		964, 			kMainBMP, 	773, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(760,			 321,  867, 	384, 		964, 			kMainBMP, 	773, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(761,			 385,  867, 	448, 		964, 			kMainBMP, 	773, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(762,			 449,  965, 	512, 		1062, 			kMainBMP, 	793, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(763,			 385,  965, 	448, 		1062, 			kMainBMP, 	256, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(764,			 129,  1001, 	192, 		1134, 			kMainBMP, 	256, 	kElevated, 			kNullDomain, kMapStructure);										// Porch rail
	gr_CreateElement(765,			 1,  966, 	64, 		1030, 			kMainBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// cabin step
	gr_CreateElement(766,			  513, 1001, 	576, 		1134, 			kMainBMP, 	  710, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(767,			  577, 1001, 	640, 		1134, 			kMainBMP, 	  710, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin
	gr_CreateElement(768,			  705, 1001, 	768, 		1134, 			kMainBMP, 	  710, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin window
	gr_CreateElement(769,			  321, 965, 	384, 		1062, 			kMainBMP, 	  771, 	kElevated, 			kNullDomain, kMapStructure);							// porch
	gr_CreateElement(770,			  1, 1031, 	64, 		1128, 			kMainBMP, 	  772, 	kElevated, 			kNullDomain, kMapStructure);							// porch

	gr_CreateElement(771,			 65, 1001, 	128, 		1065, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// brick corner
	gr_CreateElement(772,			 65, 1066, 	128, 		1130, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// brick edge
	gr_CreateElement(773,			 321, 1063, 	384, 		1127, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// brick edge

	gr_CreateElement(774,			 385, 1063, 	448, 		1127, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// brick edge
	gr_CreateElement(775,			  641,  867, 	704, 		1000, 			kMainBMP, 	  774, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin

	gr_CreateElement(776,			  449,  1063, 	512, 		1127, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// brick edge base
	gr_CreateElement(777,			  897,  867, 	960, 		1000, 			kMainBMP, 	  776, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin

	gr_CreateElement(778,			  1,  1, 	64, 		65, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// bench base
	gr_CreateElement(779,			  65,  1, 	128, 		65, 			kTownBMP, 	  778, 	kElevated, 			kNullDomain, kMapStructure);							// bench

	gr_CreateElement(780,			  1080,  9, 	1143, 		73, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// log cabin door base
	gr_CreateElement(781,			  897, 1001, 	960, 		1134, 			kMainBMP, 	  780, 	kElevated, 			kLand, kMapStructure);										// log cabin door
	gr_CreateElement(782,			  961,  473, 1024, 		 606, 			kMainBMP, 	  780, 	kElevated, 			kLand, kMapStructure);										// log cabin closed door
	gr_CreateElement(783,			  961, 1001, 1024, 		1134, 			kMainBMP, 	  780, 	kElevated, 			kNullDomain, kMapStructure);							// log cabin door edge

	gr_CreateElement(784,			  1,  66, 	64, 		199, 			kTownBMP, 	  553, 	kElevated, 			kNullDomain, kMapStructure);							// cabin stove

	gr_CreateElement(785,			  129,  1, 	192, 		65, 			kTownBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapStructure);							// cabin barrel base
	gr_CreateElement(786,			  193,  1, 	256, 		65, 			kTownBMP, 	  785, 	kElevated, 			kNullDomain, kMapStructure);							// cabin barrels

	gr_CreateElement(787,			  257,  1, 	320, 		65, 			kTownBMP, 	  745, 	kElevated, 			kNullDomain, kMapStructure);							// cabin table
	gr_CreateElement(788,			  321,  1, 	384, 		65, 			kTownBMP, 	  745, 	kElevated, 			kNullDomain, kMapStructure);							// cabin table
	gr_CreateElement(789,			  385,  1, 	448, 		65, 			kTownBMP, 	  745, 	kElevated, 			kNullDomain, kMapStructure);							// cabin table

	gr_CreateElement(790,			  129, 66, 	192, 	 130, 			kTownBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapStructure);							// cabin stool base
	gr_CreateElement(791,			  65,  66, 	128, 	 130, 			kTownBMP, 	  790, 	kElevated, 			kNullDomain, kMapStructure);							// cabin stool

	gr_CreateElement(792,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// eatery floor
	gr_CreateElement(793,			  1025,	369,	1088,		433, 			kMainBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapStructure);							// porch base

	gr_CreateElement(794,			  193,  66, 	256, 	 170, 			kTownBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// loom
	gr_CreateElement(795,			  257,  66, 	320, 	 170, 			kTownBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// loom
	gr_CreateElement(796,			  321,  66, 	384, 	 170, 			kTownBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// loom
	gr_CreateElement(797,			  385,  66, 	448, 	 170, 			kTownBMP, 	  0, 	kElevated, 			kNullDomain, kMapStructure);							// loom
	gr_CreateElement(798,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// tailor floor

	gr_CreateElement(799,			  65,  131, 	128, 	 195, 			kTownBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapStructure);							// clothes rack base
	gr_CreateElement(800,			  193,  171, 	256, 	 277, 			kTownBMP, 	799, 	kElevated, 					kNullDomain, kMapStructure);							// clothes rack
	gr_CreateElement(801,			  257,  171, 	320, 	 277, 			kTownBMP, 	799, 	kElevated, 					kNullDomain, kMapStructure);							// clothes rack
	gr_CreateElement(830,			  321,  171, 	384, 	 277, 			kTownBMP, 	799, 	kElevated, 					kNullDomain, kMapStructure);							// clothes rack

	gr_CreateElement(802,			  1,  200, 	64, 	 333, 				kTownBMP, 	698, 	kElevated, 					kNullDomain, kMapStructure);							// brick house
	gr_CreateElement(803,			  1,  200, 	64, 	 333, 				kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);							// brick house

	gr_CreateElement(804,			  65,  196, 	128, 	 329, 			kTownBMP, 	696, 	kElevated, 					kNullDomain, kMapStructure);							// brick house
	gr_CreateElement(805,			  65,  196, 	128, 	 329, 			kTownBMP, 	695, 	kElevated, 					kNullDomain, kMapStructure);							// brick house

	gr_CreateElement(806,			  1,  334, 	64, 	 467, 				kTownBMP, 	700, 	kElevated, 					kNullDomain, kMapStructure);							// brick house
	gr_CreateElement(807,			  65,  330, 	128, 	 463, 			kTownBMP, 	706, 	kElevated, 					kNullDomain, kMapStructure);							// brick house
	gr_CreateElement(808,			  129,  131, 	192, 	 264, 			kTownBMP, 	705, 	kElevated, 					kNullDomain, kMapStructure);							// brick house
	gr_CreateElement(809,			  129,  265, 	192, 	 398, 			kTownBMP, 	699, 	kElevated, 					kNullDomain, kMapStructure);							// brick house

	gr_CreateElement(810,			  321,  278, 	384, 	 411, 			kTownBMP, 	707, 	kElevated, 					kLand, kMapStructure);							// brick door closed
	gr_CreateElement(811,			  193,  278, 	256, 	 411, 			kTownBMP, 	707, 	kElevated, 					kLand, kMapStructure);							// brick door open
	gr_CreateElement(812,			  257,  278, 	320, 	 411, 			kTownBMP, 	696, 	kElevated, 					kNullDomain, kMapStructure);							// brick door

	gr_CreateElement(813,			  385,  171, 	448, 	 304, 			kTownBMP, 	698, 	kElevated, 					kNullDomain, kMapStructure);							// brick window
	gr_CreateElement(814,			  385,  171, 	448, 	 304, 			kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);							// brick window

	gr_CreateElement(815,			  449,  171, 	512, 	 304, 			kTownBMP, 	698, 	kElevated, 					kNullDomain, kMapStructure);							// brick shutter
	gr_CreateElement(816,			  513,  171, 	576, 	 304, 			kTownBMP, 	698, 	kElevated, 					kNullDomain, kMapStructure);							// brick shutter

	gr_CreateElement(817,			  385,  305, 	448, 	 438, 			kTownBMP, 	696, 	kElevated, 					kNullDomain, kMapStructure);							// brick window
	gr_CreateElement(818,			  385,  305, 	448, 	 438, 			kTownBMP, 	695, 	kElevated, 					kNullDomain, kMapStructure);							// brick window

	gr_CreateElement(819,			  449,  305, 	512, 	 438, 			kTownBMP, 	696, 	kElevated, 					kNullDomain, kMapStructure);							// brick shutter
	gr_CreateElement(820,			  513,  305, 	576, 	 438, 			kTownBMP, 	696, 	kElevated, 					kNullDomain, kMapStructure);							// brick shutter

	gr_CreateElement(821,			  1144,  9, 	1207, 	 73, 			kMainBMP, 	0, 	kNotElevated, 					kNullDomain, kMapStructure);							// column shadow
	gr_CreateElement(822,			  1025,  704, 	1088, 	 801, 	kMainBMP, 	821, 	kElevated, 					kNullDomain, kMapStructure);							// column

	gr_CreateElement(823,			  321,  412, 	384, 	 545, 			kTownBMP, 	708, 	kElevated, 					kLand, kMapStructure);							// brick door closed
	gr_CreateElement(824,			  193,  412, 	256, 	 545, 			kTownBMP, 	708, 	kElevated, 					kLand, kMapStructure);							// brick door open
	gr_CreateElement(825,			  257,  412, 	320, 	 545, 			kTownBMP, 	698, 	kElevated, 					kNullDomain, kMapStructure);							// brick door

	gr_CreateElement(826,			  129,  399, 	192, 	 463, 			kTownBMP, 	0, 	kNotElevated, 					kNullDomain, kMapStructure);							// sign base
	gr_CreateElement(827,			  449,  1, 	512, 	 99, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure); sign[827]=true;							// food sign
	gr_CreateElement(828,			  513,  1, 	576, 	 99, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure); sign[828]=true;						// tailor sign
	gr_CreateElement(829,			  1,  468, 	64, 	 566, 				kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure); sign[829]=true;						// weapons sign

	gr_CreateElement(830,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// provisioner floor

	gr_CreateElement(831,			  449,	100,	512,		164, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// shelf shadow
	gr_CreateElement(832,			  65,	 464,	 128,		571, 			  kTownBMP, 	831, 	kElevated, 					kNullDomain, kMapStructure);							// shop shelf
	gr_CreateElement(833,			  129,	 464,	 192,		571, 			  kTownBMP, 	831, 	kElevated, 					kNullDomain, kMapStructure);							// shop shelf
	gr_CreateElement(834,			  385,	 439,	 448,		546, 			  kTownBMP, 	831, 	kElevated, 					kNullDomain, kMapStructure);							// shop shelf

	gr_CreateElement(835,			  513,	100,	576,		164, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// counter shadow
	gr_CreateElement(836,			  577,	 1,	 640,		80, 			  kTownBMP, 	835, 	kElevated, 					kNullDomain, kMapStructure);							// counter

	gr_CreateElement(837,			  577,	81,	640,		145, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// counter shadow
	gr_CreateElement(838,			  577,	146,	 640,		225, 			  kTownBMP, 	837, 	kElevated, 					kNullDomain, kMapStructure);							// counter

	gr_CreateElement(839,			  577,  226, 	640, 	 359, 				kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);							// brick house wall shelf

	gr_CreateElement(840,			  577,	360,	 640,		439, 			  kTownBMP, 	835, 	kElevated, 					kNullDomain, kMapStructure);							// counter

	gr_CreateElement(841,			  449,	439,	512,		503, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// barrel shadow
	gr_CreateElement(842,			  513,	439,	 576,		503, 			  kTownBMP, 	841, 	kElevated, 					kNullDomain, kMapStructure);							// barrel

	gr_CreateElement(843,			  449,	504,	512,		568, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// inn sign shadow
	gr_CreateElement(844,			  577,	440,	 640,		538, 			  kTownBMP, 	843, 	kElevated, 					kNullDomain, kMapStructure);	sign[844]=true;						// in sign

	gr_CreateElement(845,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// tinker floor
	gr_CreateElement(846,			  1,  567, 	64, 	 665, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[846]=true;						// tinker sign

	gr_CreateElement(847,			  129,  572, 	192, 	 654, 					kTownBMP, 	0, 	kNotElevated, 					kLand, kMapStructure);							// bridge
	gr_CreateElement(848,			  65,  572, 	128, 	 665, 					kTownBMP, 	0, 	kNotElevated, 					kLand, kMapStructure);							// bridge
	gr_CreateElement(849,			  193,  546, 	256, 	 628, 					kTownBMP, 	0, 	kNotElevated, 					kLand, kMapStructure);							// bridge
	gr_CreateElement(850,			  257,  546, 	320, 	 628, 					kTownBMP, 	0, 	kNotElevated, 					kLand, kMapStructure);							// bridge

	gr_CreateElement(851,			  321,	 546,	 384,		653, 			  kTownBMP, 	831, 	kElevated, 					kNullDomain, kMapStructure);							// magic shop shelf

	gr_CreateElement(852,			  385,	 547,	 448,		680, 			  kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);							// brick house torch
	gr_CreateElement(853,			  449,	 569,	 512,		702, 			  kTownBMP, 	695, 	kElevated, 					kNullDomain, kMapStructure);							// brick house torch

	gr_CreateElement(854,			  1144,	 74,	 1207,		207, 			  kMainBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);							// cabin torch

	gr_CreateElement(855,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// magic floor

	gr_CreateElement(856,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// herbalist floor
	gr_CreateElement(857,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// weapons floor
	gr_CreateElement(858,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// alchemist floor
	gr_CreateElement(859,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// bakery floor
	gr_CreateElement(860,			  513,  504, 	576, 	 637, 			kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);				// bakery window
	gr_CreateElement(861,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// jewler floor

	gr_CreateElement(862,			1089, 	 521, 	 1152, 		 585, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// grass shadow
	gr_CreateElement(863,			1089, 	 586, 	 1152, 		 650, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// grass shadow
	gr_CreateElement(864,			1089, 	 651, 	 1152, 		 715, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// grass shadow
	gr_CreateElement(865,			 705,  696, 	768, 		760, 			kHavenBMP, 	0, 	kNotElevated, 			kLand, kMapStructure);										// guard tower steps
	gr_CreateElement(866,			193, 	 629, 	 256, 		 693, 			kTownBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// archery target shadow
	gr_CreateElement(867,			  257,  629, 	320, 	 718, 			kTownBMP, 	866, 	kElevated, 					kNullDomain, kMapStructure);				// archery target
	gr_CreateElement(868,			  577,  539, 	640, 	 646, 			kTownBMP, 	831, 	kElevated, 					kNullDomain, kMapStructure);				// archery shelf
	gr_CreateElement(869,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// archery floor

	gr_CreateElement(870,			  641,  1, 	704, 	 99, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[870]=true;						// magic sign
	gr_CreateElement(871,			  641, 100, 	704, 	 198, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[871]=true;						// alchemist sign
	gr_CreateElement(872,			  641, 199, 	704, 	 297, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[872]=true;						// blacksmith sign
	gr_CreateElement(873,			  641, 298, 	704, 	 396, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[873]=true;						// archery sign
	gr_CreateElement(874,			  641, 397, 	704, 	 495, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[874]=true;						// armor sign
	gr_CreateElement(875,			  641, 496, 	704, 	 594, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[875]=true;						// provisioner sign
	gr_CreateElement(876,			  641, 595, 	704, 	 693, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[876]=true;						// jeweler sign
	gr_CreateElement(877,			  513, 638, 	576, 	 736, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[877]=true;						// herbalist sign
	gr_CreateElement(878,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// armor floor

	gr_CreateElement(879,			  705,  1, 	768, 	 134, 				kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);		// wall armor
	gr_CreateElement(880,			  705,  135, 	768, 	 268, 				kTownBMP, 	697, 	kElevated, 					kNullDomain, kMapStructure);		// wall armor

	gr_CreateElement(881,			257, 	 1, 	 320, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web
	gr_CreateElement(882,			321, 	 1, 	 384, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web
	gr_CreateElement(883,			385, 	 1, 	 448, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web

	gr_CreateElement(884,			129, 	 192, 	 192, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web
	gr_CreateElement(885,			193, 	 192, 	 256, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web
	gr_CreateElement(886,			257, 	 192, 	 320, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon web

	gr_CreateElement(887,			1, 	 1, 	 64, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood
	gr_CreateElement(888,			65, 	 1, 	 128, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood
	gr_CreateElement(889,			129, 	 1, 	 192, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood
	gr_CreateElement(890,			193, 	 1, 	 256, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood

	gr_CreateElement(891,			1, 	 192, 	 64, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood
	gr_CreateElement(892,			65, 	 192, 	 128, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon blood

	gr_CreateElement(893,			1, 	 383, 	 64, 		466, 			kDungeonBMP, 	608, 	kElevated, 			kNullDomain, kMapNull);							// dungeon skulls

	gr_CreateElement(894,			  449,	1,	512,		65, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon skulls
	gr_CreateElement(895,			  449,	66,	512,		130, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon skull
	gr_CreateElement(896,			  449,	131,	512,		195, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon skull/spine
	gr_CreateElement(897,			  449,	196,	512,		260, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon rib cage
	gr_CreateElement(898,			  321,	192,	384,		256, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon big bone
	gr_CreateElement(899,			  385,	192,	448,		256, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon floor blood

	gr_CreateElement(900,			  321,	257,	384,		430, 			kDungeonBMP, 	  608, 	kElevated, 			kLand, kMapNull);							// dungeon latter up
	gr_CreateElement(901,			  65,	383,	128,		451, 			kDungeonBMP, 	  608, 	kElevated, 			kLand, kMapNull);							// dungeon latter down

	gr_CreateElement(902,			  129,	383,	192,		447, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon dark floor
	gr_CreateElement(903,			  193,	383,	256,		447, 			kDungeonBMP, 	  0, 	kNotElevated, 			kWater, kMapNull);							// dungeon blue water
	gr_CreateElement(904,			  257,	383,	320,		447, 			kDungeonBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapNull);							// dungeon green water

	gr_CreateElement(905,			  385,	257,	448,		321, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border
	gr_CreateElement(906,			  449,	261,	512,		325, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border
	gr_CreateElement(907,			  385,	322,	448,		386, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border

	gr_CreateElement(908,			  683, 278, 	746, 	 346, 					kMainBMP, 	0, 	kNotElevated, 					kLand, kMapStructure);		// grass dungeon entrance

	gr_CreateElement(909,		 1153, 369,  1216, 	 433, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road
	gr_CreateElement(910,		 1153, 434,  1216, 	 498, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road
	gr_CreateElement(911,		 1153, 499,  1216, 	 563, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road
	gr_CreateElement(912,		 1153, 564,  1216, 	 628, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road
	gr_CreateElement(913,		 1153, 629,  1216, 	 693, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road
	gr_CreateElement(914,		 1153, 694,  1216, 	 758, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapRoad);							// road

	gr_CreateElement(915,		 1153, 759,  1216, 	 823, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// fallen log
	gr_CreateElement(916,		 1153, 824,  1216, 	 888, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// fallen log
	gr_CreateElement(917,		 1153, 889,  1216, 	 953, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// fallen log

	gr_CreateElement(918,		 1208, 1,  1271, 	 65, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// mushrooms 1
	gr_CreateElement(919,		 1208, 66,  1271, 	 130, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 1
	gr_CreateElement(920,		 1208, 131,  1271, 	 195, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 2
	gr_CreateElement(921,		 1175, 208,  1238, 	 272, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// mushrooms 2
	gr_CreateElement(922,		 1175, 273,  1238, 	 337, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// mushrooms 3
	gr_CreateElement(923,		 1217, 338,  1280, 	 402, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 3
	gr_CreateElement(924,		 1217, 403,  1280, 	 467, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 4
	gr_CreateElement(925,		 1217, 468,  1280, 	 532, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 5
	gr_CreateElement(926,		 1153, 954,  1216, 	 1018, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 6
	gr_CreateElement(927,		 1153, 1019,  1216, 	 1083, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// shrub 7

	gr_CreateElement(928,			705,  338, 	768, 		402, 			kTownBMP, 	  745, 	kElevated, 			kNullDomain, kMapStructure);			// pub chess table

	gr_CreateElement(929,			1025,  802, 	1088, 		866, 			1, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// active pub brick

	gr_CreateElement(930,			  1,  666, 	64, 	 730, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple brick

	gr_CreateElement(931,			705,  403, 	768, 	 562, 			kTownBMP, 	932, 	kElevated, 			kNullDomain, kMapStructure);				// temple column
	gr_CreateElement(932,			129,  655, 	192, 	 719, 			kTownBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);				// temple column shadow

	gr_CreateElement(933,			705,  563, 	768, 	 627, 			kTownBMP, 	934, 	kElevated, 			kNullDomain, kMapStructure);				// temple fire container
	gr_CreateElement(934,			 65,  666, 	128, 	 730, 			kTownBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);				// temple fire container shadow

	gr_CreateElement(935,			705,  628, 	768, 	 692, 			kTownBMP, 	936, 	kElevated, 			kNullDomain, kMapStructure);				// temple altar
	gr_CreateElement(936,			577,  647, 	640, 	 711, 			kTownBMP, 	  0, 	kNotElevated, 	kNullDomain, kMapStructure);				// temple altar shadow

	gr_CreateElement(937,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// pub floor

	gr_CreateElement(938,			705,  338, 	768, 		402, 			kTownBMP, 	  940, 	kElevated, 			kNullDomain, kMapStructure);			// library chess table
	gr_CreateElement(939,			193, 		  1, 	 256, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// active library floor
	gr_CreateElement(940,			769, 		712, 	 832, 		 776, 		kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);							// active library floor

	gr_CreateElement(941,			769,  1, 	832, 	 65, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple step
	gr_CreateElement(942,			769,  66, 	832, 	 130, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple step
	gr_CreateElement(943,			769,  131, 	832, 	 195, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple step
	gr_CreateElement(944,			769,  196, 	832, 	 260, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple step
	gr_CreateElement(945,			769,  261, 	832, 	 325, 			kTownBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);					// temple step
	gr_CreateElement(946,			833, 	 717, 	 896, 		 781, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// grass shadow
	gr_CreateElement(947,			769, 326, 	832, 	 424, 					kTownBMP, 	826, 	kElevated, 					kNullDomain, kMapStructure);	sign[947]=true;	// pub sign

	gr_CreateElement(948,			833, 233,  896, 	 348, 			kTownBMP, 	  257, 	kElevated, 			kNullDomain, kMapStructure);							// white fence
	gr_CreateElement(949,			833, 581,  896, 	 696, 			kTownBMP, 	  258, 	kElevated, 			kNullDomain, kMapStructure);							// white fence

	gr_CreateElement(950,			833,   1,  896, 	 116, 			kTownBMP, 	  259, 	kElevated, 			kNullDomain, kMapStructure);							// white fence corner
	gr_CreateElement(951,		  833, 117,  896, 	 232, 			kTownBMP, 	  259, 	kElevated, 			kNullDomain, kMapStructure);							// white fence corner
	gr_CreateElement(952,			833, 349,  896, 	 464, 			kTownBMP, 	  259, 	kElevated, 			kNullDomain, kMapStructure);							// white fence corner
	gr_CreateElement(953,		  833, 465,  896, 	 580, 			kTownBMP, 	  259, 	kElevated, 			kNullDomain, kMapStructure);							// white fence corner
	gr_CreateElement(954,		  769, 425,  832, 	 540, 			kTownBMP, 	  259, 	kElevated, 			kNullDomain, kMapStructure);							// white fence gate

	gr_CreateElement(955,			  321,  278, 	384, 	 411, 			kTownBMP, 	707, 	kElevated, 					kNullDomain, kMapStructure);							// brick door closed/locked

	gr_CreateElement(956,		  769, 541,  832, 	 618, 			kTownBMP, 	  256, 	kElevated, 			kNullDomain, kMapNull);							// stable hay

	gr_CreateElement(957,		  769, 619,  832, 	 683, 			kTownBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapNull);							// trough shadow
	gr_CreateElement(958,		  385, 681,  448, 	 745, 			kTownBMP, 	 957, 	kElevated, 			kNullDomain, kMapNull);							// trough

	gr_CreateElement(959,		  321, 654,  384, 	 718, 			kTownBMP, 	  0, 	kNotElevated, 			kNullDomain, kMapNull);							// horse tie shadow
	gr_CreateElement(960,		  769, 684,  832, 	 748, 			kTownBMP, 	 959, 	kElevated, 			kNullDomain, kMapNull);							// horse tie

	gr_CreateElement(961,			 1217,  533,  1280, 		 597, 			kMainBMP, 	  0, 	kNotElevated, 	kWater, kMapNull);						// raft

	gr_CreateElement(962,			385, 387, 	 448, 		577, 			kDungeonBMP, 	608, 	kElevated, 			kNullDomain, kMapNull);							// dungeon sewer pipe
	gr_CreateElement(963,			449, 326, 	 512, 		516, 			kDungeonBMP, 	608, 	kElevated, 			kNullDomain, kMapNull);							// dungeon sewer pipe

	gr_CreateElement(964,			  193,	448,	256,		512, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border
	gr_CreateElement(965,			  257,	448,	320,		512, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border
	gr_CreateElement(966,			  321,	431,	384,		495, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon water border

	gr_CreateElement(967,			897, 	   708, 	 960, 		 772, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Dark Brick

	gr_CreateElement(968,			  1217, 598, 	1280, 		713, 			kMainBMP, 	  256, 	kElevated, 			kNullDomain, kMapStructure);							// stick guy
	gr_CreateElement(969,			  1217, 714, 	1280, 		829, 			kMainBMP, 	  256, 	kElevated, 			kNullDomain, kMapStructure);							// stick guy
	gr_CreateElement(970,			  1217, 830,  1280, 		945, 			kMainBMP, 	  256, 	kElevated, 			kNullDomain, kMapStructure);							// stick guy

	gr_CreateElement(971,			705,  563, 	768, 	 627, 			kTownBMP, 	940, 	kElevated, 			kNullDomain, kMapStructure);				// temple fire container
	gr_CreateElement(972,			961,  706, 	1024, 	 770, 			kHavenBMP, 256, 	kElevated, 			kNullDomain, kMapStructure);				// lizard fire

	gr_CreateElement(973,			  1217, 946,  1280, 		1010, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// guild entrance shadow

	gr_CreateElement(974,			1025,  670, 	1088, 		734, 			kHavenBMP, 	  638, 	kElevated, 			kNullDomain, kMapStructure);			// mariranger chess table
	gr_CreateElement(975,			1153,  1, 	1216, 		191, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// guild window
	gr_CreateElement(976,			1153,  192, 	1216, 		382, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// guild window
	gr_CreateElement(977,			1153,  383, 	1216, 		573, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// guild window
	gr_CreateElement(978,			1153,  574, 	1216, 		764, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// guild window
	gr_CreateElement(979,			705, 	   1, 	 768, 		 65, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Guild Active Brick
	gr_CreateElement(980,			1217,  1, 	1280, 		243, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// marirangers brick
	gr_CreateElement(981,			1217,  244, 	1280, 		486, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// marirangers brick
	gr_CreateElement(982,			  1025,	802,	1088,		866, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapStructure);							// carth provisioner floor
	gr_CreateElement(983,			961,  706, 	1024, 	 770, 			kHavenBMP, 256, 	kElevated, 			kNullDomain, kMapStructure);				// helena fire
	gr_CreateElement(984,			 1217,  487, 	1280, 		729, 			kHavenBMP, 	659, 	kElevated, 			kNullDomain, kMapNull);							// prison gate
	gr_CreateElement(985,			 1281,  1, 	1344, 		107, 			kHavenBMP, 	986, 	kElevated, 			kNullDomain, kMapNull);							// guilotine
	gr_CreateElement(986,			 1281,  108, 	1344, 		172, 			kHavenBMP, 	0, 	kNotElevated, 			kNullDomain, kMapNull);							// guilotine base

	gr_CreateElement(987,			  193,	513,	256,		577, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// ice dungeon water border
	gr_CreateElement(988,			  257,	578,	320,		642, 			kHavenBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// ice dungeon water border

	gr_CreateElement(989,			1272,  1, 	1335, 	 142, 			kMainBMP, 263, 	kElevated, 			kNullDomain, kMapStructure);				// teleporter

	gr_CreateElement(990,			 321,  1, 	384, 		65, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave floor
	gr_CreateElement(991,			 1,  1, 	64, 		202, 			  kCaveBMP, 	990, 	kElevated, 			kNullDomain, kMapStructure);							// cave rock
	gr_CreateElement(992,			 65,  1, 	128, 		181, 			  kCaveBMP, 	990, 	kElevated, 			kNullDomain, kMapStructure);							// cave rock
	gr_CreateElement(993,			 129,  1, 	192, 		181, 			kCaveBMP, 	990, 	kElevated, 			kNullDomain, kMapStructure);							// cave rock
	gr_CreateElement(994,			 193,  1, 	256, 		181, 			kCaveBMP, 	990, 	kElevated, 			kNullDomain, kMapStructure);							// cave rock
	gr_CreateElement(995,			 257,  1, 	320, 		181, 			kCaveBMP, 	990, 	kElevated, 			kNullDomain, kMapStructure);							// cave rock

	gr_CreateElement(996,			 129,  203, 	192, 		306, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(997,			 321,  66, 	384, 		199, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(998,			 385,  1, 	448, 		134, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(999,			 449,  1, 	512, 		89, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(1000,			 513,  1, 	576, 		89, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(1001,			 577,  1, 	640, 		108, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite
	gr_CreateElement(1002,			 641,  1, 	704, 		89, 			kCaveBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// cave stalagtite

	gr_CreateElement(1003,			 1,  203, 	64, 		267, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave small stalagtite
	gr_CreateElement(1004,			 65,  203, 	128, 		267, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave small stalagtite
	gr_CreateElement(1005,			 1,  268, 	64, 		332, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave small stalagtite
	gr_CreateElement(1006,			 65,  268, 	128, 		332, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave small stalagtite
	gr_CreateElement(1007,			 1,  333, 	64, 		397, 			kCaveBMP, 	0, 	kNotElevated, 			kLand, kMapNull);							// cave small stalagtite

	gr_CreateElement(1008,			 129,	347,	192,		411, 			kCaveBMP, 	0, 	kNotElevated, 			kNullDomain, kMapNull);					// cave builder temp

	gr_CreateElement(1009,			 1,  398, 	64, 		599, 			kCaveBMP, 	990, 	kElevated, 			kLand, kMapNull);						// cave exit
	gr_CreateElement(1010,			 1239,	196,	1322,		320, 		kMainBMP, 	256, 	kElevated, 			kLand, kMapNull);					  // cave entrance


	gr_CreateElement(1011,			  193,	513,	256,		577, 			kDungeonBMP, 	  0, 	kElevated, 			kNullDomain, kMapNull);				// dungeon lava
	gr_CreateElement(1012,			  193,	578,	256,		642, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon lava corner
	gr_CreateElement(1013,			  193,	643,	256,		707, 			kDungeonBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// dungeon lava corner

	gr_CreateElement(1014,			1272,  1, 	1335, 	 142, 			kMainBMP, 614, 	kElevated, 			kNullDomain, kMapStructure);				// dungeon teleporter
	gr_CreateElement(1015,			257,  513, 	320, 	 577, 			kDungeonBMP, 614, 	kElevated, 			kNullDomain, kMapStructure);				// dungeon fire
	gr_CreateElement(1016,			1089,  716, 	1152, 	 780, 			kHavenBMP, 614, 	kElevated, 			kNullDomain, kMapStructure);				// stars

	gr_CreateElement(1017,			257, 	 578, 	 320, 		737, 			kDungeonBMP, 	614, 	kElevated, 			kNullDomain, kMapNull);							// Castle Broken Column
	gr_CreateElement(1018,			321, 	   496, 	 384, 		 560, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Cracked Floor
	gr_CreateElement(1019,			321, 	   561, 	 384, 		 625, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Cracked Floor
	gr_CreateElement(1020,			1, 	   670, 	 64, 		 734, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Cracked Floor
	gr_CreateElement(1021,			65, 	   680, 	 128, 		 744, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Castle Ribs Floor
	gr_CreateElement(1022,			449, 	 517, 	 512, 		759, 			kDungeonBMP, 	659, 	kElevated, 			kLand, kMapNull);										// Castle Broken Gate
	gr_CreateElement(1023,			129, 	 708, 	 197, 		771, 			kDungeonBMP, 	614, 	kElevated, 			kNullDomain, kMapNull);										// Castle Broken Column Top
	gr_CreateElement(1024,			321, 	 626, 	 384, 		696, 			kDungeonBMP, 	256, 	kElevated, 			kNullDomain, kMapNull);										// Castle Broken Wall Top
	gr_CreateElement(1025,			897, 	 1, 	 960, 		151, 			kDungeonBMP, 	256, 	kElevated, 			kNullDomain, kMapNull);										// Castle Broken Wall

	gr_CreateElement(1026,			897, 	 152, 	 960, 		394, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle damaged wall
	gr_CreateElement(1027,			897, 	 395, 	 960, 		637, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// Castle damaged wall

	gr_CreateElement(1028,			1345, 	193, 	 1408, 		 256, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1029,			1345, 	257, 	 1408, 		 321, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1030,			1345, 	322, 	 1408, 		 386, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1031,			1345, 	387, 	 1408, 		 451, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1032,			1345, 	452, 	 1408, 		 516, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1033,			1345, 	517, 	 1408, 		 581, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1034,			1345, 	582, 	 1408, 		 646, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick
	gr_CreateElement(1035,			1345, 	647, 	 1408, 		 711, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// Pit Brick

	gr_CreateElement(1036,			1281,  173, 	1344, 		363, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// pit wall skeleton
	gr_CreateElement(1037,			1281,  364, 	1344, 		554, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// pit wall skeleton
	gr_CreateElement(1038,			1281,  555, 	1344, 		745, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// pit wall skeleton
	gr_CreateElement(1039,			1345,  1, 	1408, 		191, 			kHavenBMP, 	  kNullBase, 	kElevated, 			kNullDomain, kMapStructure);			// pit wall skeleton

	gr_CreateElement(1040,		 1323, 173,  1386, 	 237, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1041,		 1323, 238,  1386, 	 302, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1042,		 1217, 1011,  1280, 	 1075, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1043,		 1281, 1011,  1344, 	 1075, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1044,		 1345, 1011,  1408, 	 1075, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1045,		 1217, 1076,  1280, 	 1140, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1046,		 1281, 1076,  1344, 	 1140, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones
	gr_CreateElement(1047,		 1345, 1076,  1408, 	 1140, 			kMainBMP, 	  0, 	kNotElevated, 			kLand, kMapNull);							// pvp bones

	gr_CreateElement(1048,		 1153, 1084,  1216, 	 1148, 			kMainBMP, 	  256, 	kElevated, 			kLand, kMapNull);		sign[1048]=true;					// house sign

	tb_SetRect(&srcRect[1049],	385,	347,	448,		411); // ocean/beach border
	tb_SetRect(&srcRect[1050],	385,	412,	448,		476); // ocean/beach border
	tb_SetRect(&srcRect[1051],	385,	477,	448,		541); // ocean/beach border
	tb_SetRect(&srcRect[1052],	385,	542,	448,		606); // ocean/beach border

	tb_SetRect(&srcRect[1053],	449,	347,	512,		411); // ocean/beach border
	tb_SetRect(&srcRect[1054],	449,	412,	512,		476); // ocean/beach border
	tb_SetRect(&srcRect[1055],	449,	477,	512,		541); // ocean/beach border
	tb_SetRect(&srcRect[1056],	449,	542,	512,		606); // ocean/beach border

	tb_SetRect(&srcRect[1057],	513,	347,	576,		411); // ocean/beach border
	tb_SetRect(&srcRect[1058],	513,	412,	576,		476); // ocean/beach border
	tb_SetRect(&srcRect[1059],	513,	477,	576,		541); // ocean/beach border
	tb_SetRect(&srcRect[1060],	513,	542,	576,		606); // ocean/beach border

	tb_SetRect(&srcRect[1061],	577,	347,	640,		411); // ocean/beach border
	tb_SetRect(&srcRect[1062],	577,	412,	640,		476); // ocean/beach border
	tb_SetRect(&srcRect[1063],	577,	477,	640,		541); // ocean/beach border
	tb_SetRect(&srcRect[1064],	577,	542,	640,		606); // ocean/beach border

	tb_SetRect(&srcRect[1065],	641,	347,	704,		411); // ocean/beach border
	tb_SetRect(&srcRect[1066],	641,	412,	704,		476); // ocean/beach border
	tb_SetRect(&srcRect[1067],	641,	477,	704,		541); // ocean/beach border
	tb_SetRect(&srcRect[1068],	641,	542,	704,		606); // ocean/beach border

	tb_SetRect(&srcRect[1069],	705,	347,	768,		411); // ocean/beach border
	tb_SetRect(&srcRect[1070],	705,	412,	768,		476); // ocean/beach border
	tb_SetRect(&srcRect[1071],	705,	477,	768,		541); // ocean/beach border
	tb_SetRect(&srcRect[1072],	705,	542,	768,		606); // ocean/beach border

	tb_SetRect(&srcRect[1073],	769,	347,	832,		411); // ocean/beach border
	tb_SetRect(&srcRect[1074],	769,	412,	832,		476); // ocean/beach border
	tb_SetRect(&srcRect[1075],	769,	477,	832,		541); // ocean/beach border
	tb_SetRect(&srcRect[1076],	769,	542,	832,		606); // ocean/beach border

	tb_SetRect(&srcRect[1077],	833,	347,	896,		411); // ocean/beach border
	tb_SetRect(&srcRect[1078],	833,	412,	896,		476); // ocean/beach border
	tb_SetRect(&srcRect[1079],	833,	477,	896,		541); // ocean/beach border
	tb_SetRect(&srcRect[1080],	833,	542,	896,		606); // ocean/beach border

	gr_CreateElement(1081,			833, 	   1, 	 896, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);	// dungeon torch
	gr_CreateElement(1082,			833, 	 192, 	 896, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);	// dungeon torch
	gr_CreateElement(1083,			1025, 	   1, 	 1088, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);	// ruins torch
	gr_CreateElement(1084,			1025, 	 192, 	 1088, 		382, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);	// ruins torch

	gr_CreateElement(1085,			  1, 	 479, 	  64, 		669, 			kDungeonBMP, 	1097, 	kElevated, 			kLand, kMapNull);										// dungeon 1 door part 1
	gr_CreateElement(1086,			 65, 	 489, 	 128, 		679, 			kDungeonBMP, 	1097, 	kElevated, 			kNullDomain, kMapNull);							// dungeon 1 door part 2
	gr_CreateElement(1087,			 129,  479, 	 192, 		669, 			kDungeonBMP, 	632, 	kElevated, 			kNullDomain, kMapNull);							// dungeon open door 1

	gr_CreateElement(1088,			 513,  500, 	 576, 		690, 			kDungeonBMP, 	1097, 	kElevated, 			kLand, kMapNull);										// dungeon 2 door part 1
	gr_CreateElement(1089,			 577,  500, 	 640, 		690, 			kDungeonBMP, 	1097, 	kElevated, 			kNullDomain, kMapNull);							// dungeon 2 door part 2
	gr_CreateElement(1090,			 641,  500, 	 704, 		690, 			kDungeonBMP, 	632, 	kElevated, 			kNullDomain, kMapNull);							// dungeon open door 2

	gr_CreateElement(1091,			513, 	   1, 	 576, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon wall
	gr_CreateElement(1092,			577, 	   1, 	 640, 		191, 			kDungeonBMP, 	kNullBase, 	kElevated, 			kNullDomain, kMapNull);							// dungeon wall
	gr_CreateElement(1093,			641, 	   1, 	 704, 		191, 			kDungeonBMP, 	967, 	kElevated, 			kNullDomain, kMapNull);							// dungeon wall

	gr_CreateElement(1094,			385, 	  578, 	 448, 		739, 			kDungeonBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// dungeon wall
	gr_CreateElement(1095,			1025, 	383, 	 1088, 		586, 			kDungeonBMP, 	0, 	kElevated, 			kNullDomain, kMapNull);							// dungeon wall

	gr_CreateElement(1096,			705, 	 374, 	 768, 		533, 			kDungeonBMP, 	1097, 	kElevated, 			kNullDomain, kMapNull);							// dungeon Column

	gr_CreateElement(1097,			705, 	   1, 	 768, 		 65, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// dungeon Brick
	gr_CreateElement(1098,			769, 	 196, 	 832, 		260, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// dungeon shadow brick
	gr_CreateElement(1099,			769, 	   1, 	 832, 		 65, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// dungeon shadow brick
	gr_CreateElement(1100,			769, 	  66, 	 832, 		130, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// dungeon shadow brick

	gr_CreateElement(1101,			321, 	 697, 	 384, 		 761, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// ruins Brick
	gr_CreateElement(1102,			833, 	 717, 	 896, 		781, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// ruins shadow brick
	gr_CreateElement(1103,			897, 	 638, 	 960, 		 702, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// ruins shadow brick
	gr_CreateElement(1104,			897, 	 703, 	 960, 		767, 			kDungeonBMP, 	0, 		kNotElevated, 	kLand, kMapNull);										// ruins shadow brick

	gr_CreateElement(1105,			1025, 	 587, 	 1088, 		777, 			kDungeonBMP, 	1097, 	kElevated, 			kNullDomain, kMapStructure);							// dungeon wall

	gr_CreateElement(1106,			1153, 	   1, 	 1216, 		191, 			kDungeonBMP, 	1108, 	kElevated, 			kLand, kMapNull);							// dungeon wall
	gr_CreateElement(1107,			1153, 	   257, 	 1216, 		447, 			kDungeonBMP, 	1108, 	kElevated, 			kNullDomain, kMapStructure);							// dungeon wall
	gr_CreateElement(1108,			1153, 	   192, 	 1216, 		256, 			kDungeonBMP, 	0, 	kNotElevated, 			kNullDomain, kMapStructure);							// dungeon wall
	//Dave changed flag tile to tower
	//gr_CreateElement(1109,			1153, 	   448, 	 1216, 		536, 			kDungeonBMP, 	0, 	kElevated, 			kNullDomain, kFlagStructure);							// dungeon wall
	gr_CreateElement(1109,			1217, 	   1, 	 1371, 		401, 			kDungeonBMP, 	608, 	kElevated, 			kLand, kMapNull);							// tower ladder up
	tb_SetRect(&srcRect[1110],	385-320,	347-14,	448-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1111],	385-320,	412-14,	448-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1112],	385-320,	477-14,	448-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1113],	385-320,	542-14,	448-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1114],	449-320,	347-14,	512-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1115],	449-320,	412-14,	512-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1116],	449-320,	477-14,	512-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1117],	449-320,	542-14,	512-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1118],	513-320,	347-14,	576-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1119],	513-320,	412-14,	576-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1120],	513-320,	477-14,	576-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1121],	513-320,	542-14,	576-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1122],	577-320,	347-14,	640-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1123],	577-320,	412-14,	640-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1124],	577-320,	477-14,	640-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1125],	577-320,	542-14,	640-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1126],	641-320,	347-14,	704-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1127],	641-320,	412-14,	704-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1128],	641-320,	477-14,	704-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1129],	641-320,	542-14,	704-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1130],	705-320,	347-14,	768-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1131],	705-320,	412-14,	768-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1132],	705-320,	477-14,	768-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1133],	705-320,	542-14,	768-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1134],	769-320,	347-14,	832-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1135],	769-320,	412-14,	832-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1136],	769-320,	477-14,	832-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1137],	769-320,	542-14,	832-320,		606-14); // lava/cave border

	tb_SetRect(&srcRect[1138],	833-320,	347-14,	896-320,		411-14); // lava/cave border
	tb_SetRect(&srcRect[1139],	833-320,	412-14,	896-320,		476-14); // lava/cave border
	tb_SetRect(&srcRect[1140],	833-320,	477-14,	896-320,		541-14); // lava/cave border
	tb_SetRect(&srcRect[1141],	833-320,	542-14,	896-320,		606-14); // lava/cave border

	gr_CreateElement(1142,			641, 	   90, 	 704, 		332, 			kCaveBMP, 	608, 	kElevated, 			kNullDomain, kMapStructure);							// fort wall
	gr_CreateElement(1143,			577, 	   109, 	 640, 		299, 			kCaveBMP, 	608, 	kElevated, 			kNullDomain, kMapStructure);							// fort wall
	gr_CreateElement(1144,			513, 	   90, 	 576, 		332, 			kCaveBMP, 	608, 	kElevated, 			kNullDomain, kMapStructure);				// fort wall
	// Castle wall
	gr_CreateElement(1145,			193, 	   203, 	 256, 		267, 			kCaveBMP, 	0, 	kNotElevated, 			kNullDomain, kMapStructure);							// cave lava
	//---------------------------------------------.90

	tb_SetRect(&srcRect[1146],	385-320,	347-14+260,	448-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1147],	385-320,	412-14+260,	448-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1148],	385-320,	477-14+260,	448-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1149],	385-320,	542-14+260,	448-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1150],	449-320,	347-14+260,	512-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1151],	449-320,	412-14+260,	512-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1152],	449-320,	477-14+260,	512-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1153],	449-320,	542-14+260,	512-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1154],	513-320,	347-14+260,	576-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1155],	513-320,	412-14+260,	576-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1156],	513-320,	477-14+260,	576-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1157],	513-320,	542-14+260,	576-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1158],	577-320,	347-14+260,	640-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1159],	577-320,	412-14+260,	640-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1160],	577-320,	477-14+260,	640-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1161],	577-320,	542-14+260,	640-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1162],	641-320,	347-14+260,	704-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1163],	641-320,	412-14+260,	704-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1164],	641-320,	477-14+260,	704-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1165],	641-320,	542-14+260,	704-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1166],	705-320,	347-14+260,	768-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1167],	705-320,	412-14+260,	768-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1168],	705-320,	477-14+260,	768-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1169],	705-320,	542-14+260,	768-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1170],	769-320,	347-14+260,	832-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1171],	769-320,	412-14+260,	832-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1172],	769-320,	477-14+260,	832-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1173],	769-320,	542-14+260,	832-320,		606-14+260); // lava/grass border

	tb_SetRect(&srcRect[1174],	833-320,	347-14+260,	896-320,		411-14+260); // lava/grass border
	tb_SetRect(&srcRect[1175],	833-320,	412-14+260,	896-320,		476-14+260); // lava/grass border
	tb_SetRect(&srcRect[1176],	833-320,	477-14+260,	896-320,		541-14+260); // lava/grass border
	tb_SetRect(&srcRect[1177],	833-320,	542-14+260,	896-320,		606-14+260); // lava/grass border

	gr_CreateElement(1178,		 897, 1,  960, 	 99, 			kTownBMP, 	  843, 	kElevated, 			kLand, kMapNull);	sign[1178]=true;						// house sign
	gr_CreateElement(1179,		 897, 100,  960, 	 198, 			kTownBMP, 	  826, 	kElevated, 			kLand, kMapNull);		sign[1179]=true;					// house sign

	gr_CreateElement(1180, 		1409,	589,	1471,	791, kHavenBMP, 0, kElevated, kNullDomain, kMapNull);  //forge facing south
	gr_CreateElement(1181, 		1409,	427,	1471,	587, kHavenBMP, 0, kElevated, kNullDomain, kMapNull);  //forge facing south

	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	gr_CreateElement(1182,			1409, 	   1, 	 1471, 		 64, 			kHavenBMP, 	0, 		kNotElevated, 	kLand, kMapNull); // cliff west face
	
	for (x=1110;x<=1141;x++)
		sourceGrid[x]			=	(UInt8)kCaveBMP;

	for (x=1146;x<=1177;x++)
		sourceGrid[x]			=	(UInt8)kCaveBMP;

	//---------------------------------------------.90

	for (x=0;x<kMaxIcons;x++)
		obstacle[x]=elevated[x];

	obstacle[554]=false;

	obstacle[555]=false;

	obstacle[556]=false;

	obstacle[557]=false;

	obstacle[558]=false;

	obstacle[559]=false;

	obstacle[560]=false;

	obstacle[561]=false;

	obstacle[562]=false;

	obstacle[563]=false;

	obstacle[578]=false;

	obstacle[628]=false;

	obstacle[636]=false;

	obstacle[637]=false;

	obstacle[639]=false;

	obstacle[640]=false;

	obstacle[641]=false;

	obstacle[642]=false;

	obstacle[643]=false;

	obstacle[646]=false;

	obstacle[649]=false;

	obstacle[650]=false;

	obstacle[651]=false;

	obstacle[652]=false;

	obstacle[653]=false;

	obstacle[654]=false;

	obstacle[655]=false;

	obstacle[656]=false;

	obstacle[657]=false;

	obstacle[664]=false;

	obstacle[665]=false;

	obstacle[669]=false;

	obstacle[670]=false;

	obstacle[671]=false;

	obstacle[672]=false;

	obstacle[673]=false;

	obstacle[724]=false;

	obstacle[725]=false;

	obstacle[726]=false;

	obstacle[734]=false;

	// obstacle[735]=false;

	obstacle[737]=false;

	obstacle[738]=false;

	obstacle[740]=false;

	obstacle[742]=false;

	obstacle[744]=false;

	obstacle[759]=false;

	obstacle[760]=false;

	obstacle[761]=false;

	obstacle[762]=false;

	obstacle[763]=false;

	obstacle[769]=false;  // ?

	obstacle[770]=false;  // ?

	obstacle[726]=false;

	obstacle[779]=false;

	obstacle[786]=false;

	obstacle[787]=false;

	obstacle[788]=false;

	obstacle[789]=false;

	obstacle[791]=false;

	obstacle[836]=false;

	obstacle[838]=false;

	obstacle[840]=false;

	obstacle[842]=false;

	obstacle[928]=false;

	//obstacle[933]=false;
	obstacle[935]=false;

	obstacle[938]=false;

	obstacle[956]=false;

	obstacle[958]=false;

	obstacle[960]=false;

	obstacle[961]=false;  // ?

	//---------------------------------------------

	elevated[256]		=false;	// grass

	elevated[257]		=false;	// grass

	elevated[258]		=false;	// grass

	elevated[259]		=false;	// grass

	elevated[260]		=false;	// grass

	elevated[261]		=false;	// grass

	elevated[262]		=false;	// grass

	elevated[263]		=false;	// grass

	elevated[264]		=false;	// mud

	elevated[265]		=false;	// mud

	elevated[266]		=false;	// mud

	elevated[275]		=false;	// water

	for (i=400;i<536;i++)
		elevated[i]		=false;

	for (i=1049;i<1081;i++) // beach
	{
		elevated[i]			= false;
		canWalkOn[i]		= true;
		sourceGrid[i]		=	kIceBMP;
	}

	for (i=0;i<kMaxIcons;i++) // signs
		if (sign[i])
			canWalkOn[i]		= true;

	elevated[553]		=false;	// brick floor

	//---------

	canSwimIn[275]	= true;	// water

	for (x=439;x<=470;x++)
		canSwimIn[x]	= true;

	for (x=503;x<=534;x++)
		canSwimIn[x]	= true;

	canWalkOn[256]	=true;	// grass

	canWalkOn[257]	=true;	// grass

	canWalkOn[258]	=true;	// grass

	canWalkOn[259]	=true;	// grass

	canWalkOn[260]	=true;	// grass

	canWalkOn[261]	=true;	// grass

	canWalkOn[262]	=true;	// grass

	canWalkOn[263]	=true;	// grass

	canWalkOn[264]	=true;	// mud

	canWalkOn[265]	=true;	// mud

	canWalkOn[266]	=true;	// mud

	canWalkOn[553]	=true;	// brick

	for (x=400;x<439;x++)
		canWalkOn[x]	= true;

	for (x=471;x<535;x++)
		canWalkOn[x]	= true;

	//----------------------------------------

	for (x=0;x<kMaxIcons;x++)
		canFlyOver[x]	= canWalkOn[x];

	canFlyOver[275]	= true;		// water

	for (x=439;x<471;x++)			// water
		canFlyOver[x]	= true;

	canFlyOver[903]	= true;		// water

	canFlyOver[904]	= true;		// water

	//----------------------------------------

	base[359]				= 263;

	base[360]				= 263;

	base[361]				= 263;

	base[362]				= 263;

	base[363]				= 263;

	base[364]				= 263;

	base[365]				= 263;

	base[366]				= 263;

	base[367]				= 263;

	base[368]				= 263;

	base[369]				= 263;

	base[370]				= 263;

	base[371]				= 263;

	base[372]				= 263;

	base[373]				= 263;


	base[536]				= 256;	// rock

	base[537]				= 256;	// rock

	base[538]				= 256;	// rock

	base[539]				= 256;	// rock

	base[540]				= 256;	// rock

	base[541]				= 256;	// rock

	base[542]				= 256;	// rock

	base[543]				= 256;	// rock

	base[544]				= 256;	// rock

	base[386]				= 399;	// rock

	base[387]				= 399;	// rock

	base[388]				= 399;	// rock

	base[389]				= 399;	// rock

	base[390]				= 399;	// rock

	base[391]				= 399;	// rock

	base[392]				= 399;	// rock

	base[393]				= 399;	// rock

	base[394]				= 399;	// rock

	/*
		base[536]				= kNullBase;	// rock
		base[537]				= kNullBase;	// rock
		base[538]				= kNullBase;	// rock
		base[539]				= kNullBase;	// rock
		base[540]				= kNullBase;	// rock
		base[541]				= kNullBase;	// rock
		base[542]				= kNullBase;	// rock
		base[543]				= kNullBase;	// rock
		base[544]				= kNullBase;	// rock
	*/

	base[545]				= 553;	// log cabin

	base[546]				= 553;	// log cabin

	base[547]				= 553;	// log cabin

	base[548]				= 553;	// log cabin

	base[549]				= 553;	// log cabin

	base[550]				= 553;	// log cabin

	base[551]				= 553;	// log cabin

	base[552]				= 553;	// log cabin

	//---------------------------------------------

	for (x=0;x<kMaxContainerTypes;x++)
	{
		tb_SetRect(&containerRect[x],0,0,0,0);
		containerColor[x]     = kNoCreatureColor;
		containerIsCorpse[x]  = false;
		fliesOnCorpse[x]      = false;
		containerIsTreasure[x]= false;
		gmCanSpawn[x]         = false;
		gmProp[x]         		= false;
	}

	tb_SetRect(&containerRect[1],	65,	1,	128,		65);          gmCanSpawn[1]=true;// satchel

	tb_SetRect(&containerRect[2],	1,	1,	64,			65);	 				gmCanSpawn[2]=true;       containerIsCorpse[2]=true; // skeleton corpse
	tb_SetRect(&containerRect[3],	1,	66,	120,		185);	 				gmCanSpawn[3]=true;       containerIsCorpse[3]=true; 		fliesOnCorpse[3]=true;		containerColor[3]=kForestDragonColor; // slug corpse
	tb_SetRect(&containerRect[4],	1,	186,	94,		273);	 				gmCanSpawn[4]=true;       containerIsCorpse[4]=true; 		fliesOnCorpse[4]=true;		 // hell hound corpse
	tb_SetRect(&containerRect[5],	129,	1,	197,		69);				gmCanSpawn[5]=true;       containerIsCorpse[5]=true; 		fliesOnCorpse[5]=true;		 // grey wolf corpse

	tb_SetRect(&containerRect[6],	198,	1,	261,		65); // garlic
	tb_SetRect(&containerRect[7],	262,	1,	325,		65); // lotus
	tb_SetRect(&containerRect[8],	326,	1,	389,		65); // fire
	tb_SetRect(&containerRect[9],	390,	1,	453,		65); // mandrake
	tb_SetRect(&containerRect[10],	198,	66,	261,		130); // ginseng
	tb_SetRect(&containerRect[11],	262,	66,	325,		130); // volcanic ash
	tb_SetRect(&containerRect[12],	630,	131,	693,		195); // quest scroll

	tb_SetRect(&containerRect[13],	121,	131,	334,		286);	 	gmCanSpawn[13]=true;      containerIsCorpse[13]=true; // reaper corpse
	tb_SetRect(&containerRect[14],	335,	131,	398,		195);	 	gmCanSpawn[14]=true;      containerIsCorpse[14]=true; 		fliesOnCorpse[14]=true;		 // human corpse
	tb_SetRect(&containerRect[15],	454,	1,	634,		115);	 		gmCanSpawn[15]=true;      containerIsCorpse[15]=true; 		fliesOnCorpse[15]=true;		 containerColor[15]=kRedDragonColor; // red dragon corpse
	tb_SetRect(&containerRect[16],	390,	66,	447,		122);	 		gmCanSpawn[16]=true;      containerIsCorpse[16]=true; 		fliesOnCorpse[16]=true;		 // turtle corpse
	tb_SetRect(&containerRect[17],	335,	196,	398,		260);	 	gmCanSpawn[17]=true;      containerIsCorpse[17]=true; 		fliesOnCorpse[17]=true;		 // sheep corpse
	tb_SetRect(&containerRect[18],	399,	123,	496,		187);	 	gmCanSpawn[18]=true;      containerIsCorpse[18]=true; 		fliesOnCorpse[18]=true;		 // scorpion corpse
	tb_SetRect(&containerRect[19],	399,	188,	462,		252);	 	gmCanSpawn[19]=true;      containerIsCorpse[19]=true; 		fliesOnCorpse[19]=true;		 // harpie corpse
	tb_SetRect(&containerRect[20],	454,	1,	634,		115);	 		gmCanSpawn[20]=true;      containerIsCorpse[20]=true; 		fliesOnCorpse[20]=true;		 containerColor[20]=kIceDragonColor; // ice dragon corpse
	tb_SetRect(&containerRect[21],	497,	116,	560,		180);	 	gmCanSpawn[21]=true;      containerIsCorpse[21]=true; // bone mage corpse
	tb_SetRect(&containerRect[22],	463,	188,	526,		252);	 	gmCanSpawn[22]=true;      containerIsCorpse[22]=true; 		fliesOnCorpse[22]=true;		 // gazer corpse
	tb_SetRect(&containerRect[23],	561,	116,	629,		184);		gmCanSpawn[23]=true;      containerIsCorpse[23]=true; 		fliesOnCorpse[23]=true;		 // brown wolf corpse
	tb_SetRect(&containerRect[24],	527,	185,	595,		253);		gmCanSpawn[24]=true;      containerIsCorpse[24]=true; 		fliesOnCorpse[24]=true;		 // black wolf corpse
	tb_SetRect(&containerRect[25],	531,	254,	594,		318);		gmCanSpawn[25]=true;      containerIsCorpse[25]=true; 		fliesOnCorpse[25]=true;		 // rat corpse
	tb_SetRect(&containerRect[26],	635,	1,	698,		65);		  gmCanSpawn[26]=true;      containerIsCorpse[26]=true; 		fliesOnCorpse[26]=true;		 // lizard man corpse
	tb_SetRect(&containerRect[27],	635,	66,	698,		130);     containerIsTreasure[27]=true;// dungeon chest
	tb_SetRect(&containerRect[28],	635,	66,	698,		130);     containerIsTreasure[28]=true;// sewer chest
	tb_SetRect(&containerRect[29],	635,	66,	698,		130);     containerIsTreasure[29]=true;// lizard chest
	tb_SetRect(&containerRect[30],	635,	66,	698,		130);     // permanent chest
	tb_SetRect(&containerRect[31],	454,	1,	634,		115);	 		gmCanSpawn[31]=true;      containerIsCorpse[31]=true; 		fliesOnCorpse[31]=true;		 containerColor[31]=kForestDragonColor; // ice dragon corpse
	tb_SetRect(&containerRect[32],	596,	196,	659,	260);     gmCanSpawn[32]=true;      containerIsTreasure[32]=true;// ice dungeon chest
	tb_SetRect(&containerRect[33],	595,	261,	658,	325);			gmCanSpawn[33]=true;      containerIsCorpse[33]=true; 		fliesOnCorpse[33]=true;		 // bat corpse
	tb_SetRect(&containerRect[34],	660,	196,	765,	291);	 		gmCanSpawn[34]=true;      containerIsCorpse[34]=true; 		fliesOnCorpse[34]=true;		  // giant spider
	tb_SetRect(&containerRect[35],	660,	196,	765,	291);	 		gmCanSpawn[35]=true;      containerIsCorpse[35]=true; 		fliesOnCorpse[35]=true;		 containerColor[35]=kIceDragonColor; // frost spider
	tb_SetRect(&containerRect[36],	660,	196,	765,	291);	 		gmCanSpawn[36]=true;      containerIsCorpse[36]=true; 		fliesOnCorpse[36]=true;		 containerColor[36]=kRedDragonColor; // lava spider
	tb_SetRect(&containerRect[37],	699,	1,	762,	65);				gmCanSpawn[37]=true;      containerIsCorpse[37]=true; 		fliesOnCorpse[37]=true;		 // snail corpse
	tb_SetRect(&containerRect[38],	659,	292,	741,	356);			gmCanSpawn[38]=true;      containerIsCorpse[38]=true; 		fliesOnCorpse[38]=true;		 // ant corpse
	tb_SetRect(&containerRect[39],	699,	66,	762,	130);			  gmCanSpawn[39]=true;      containerIsCorpse[39]=true; 		fliesOnCorpse[39]=true;		 // troll corpse
	tb_SetRect(&containerRect[40],	1,	322,	94,	410);			    gmCanSpawn[40]=true;      containerIsCorpse[40]=true; 		fliesOnCorpse[40]=true;		 // giant corpse
	tb_SetRect(&containerRect[41],	95,	322,	171,	386);			  gmCanSpawn[41]=true;      containerIsCorpse[41]=true; 		fliesOnCorpse[41]=true;		 // centaur corpse
	tb_SetRect(&containerRect[42],	172,	322,	247,	386);			gmCanSpawn[42]=true;      containerIsCorpse[42]=true; 		fliesOnCorpse[42]=true;		 // serpent corpse
	tb_SetRect(&containerRect[43],	248,	322,	324,	386);			gmCanSpawn[43]=true;      containerIsCorpse[43]=true; 		fliesOnCorpse[43]=true;		 // zombie corpse
	tb_SetRect(&containerRect[44],	1,	66,	120,		185);	 			gmCanSpawn[44]=true;      containerIsCorpse[44]=true; 		fliesOnCorpse[44]=true;		containerColor[44]=kRedDragonColor; // slug corpse
	tb_SetRect(&containerRect[45],	325,	322,	445,			408);	gmCanSpawn[45]=true;      containerIsCorpse[45]=true; // brigand shelter 1 corpse
	tb_SetRect(&containerRect[46],	454,	1,	634,		115);	 		gmCanSpawn[46]=true;      containerIsCorpse[46]=true; 		fliesOnCorpse[46]=true;		 containerColor[46]=kBlackDragonColor; // black dragon corpse

	tb_SetRect(&containerRect[47],	446,	322,	509,		386); // reaper seed

	tb_SetRect(&containerRect[48],	635,	66,	698,		130);     gmCanSpawn[48]=true;     // dungeon chest
	tb_SetRect(&containerRect[49],	596,	196,	659,	260);     gmCanSpawn[49]=true;     // ice dungeon chest

	tb_SetRect(&containerRect[50],	510,	322,	573,	386);     gmProp[50]=true;     // fire prop
	tb_SetRect(&containerRect[51],	574,	326,	637,	390);     gmProp[51]=true;     // camp kettle prop

	tb_SetRect(&containerRect[52],	766,	1,	829,	116);     	gmProp[52]=true;     // guy on stick prop
	tb_SetRect(&containerRect[53],	766,	117,	829,	232);     gmProp[53]=true;     // guy on stick prop
	tb_SetRect(&containerRect[54],	766,	233,	829,	348);     gmProp[54]=true;     // guy on stick prop

	tb_SetRect(&containerRect[55],	635,	66,	698,		130);     containerIsTreasure[55]=true;// fire dungeon chest

	tb_SetRect(&containerRect[56],	638,	357,	701,		421);     gmCanSpawn[56]=true;     // gift
	tb_SetRect(&containerRect[57],	702,	357,	765,		421);     gmCanSpawn[57]=true;     // gift
	tb_SetRect(&containerRect[58],	766,	357,	829,		421);     gmCanSpawn[58]=true;     // gift

	tb_SetRect(&containerRect[59],	397,	409,	460,		473);     gmProp[59]=true;          // pumpkin
	tb_SetRect(&containerRect[60],	95,	387,	149,		491);       gmProp[60]=true;          // christmas tree
	tb_SetRect(&containerRect[61],	150,	387,	204,		491);     gmProp[61]=true;          // christmas tree

	tb_SetRect(&containerRect[62],	1,	411,	64,		475);  		gmCanSpawn[62]=true;   	containerColor[62]=kRedDragonColor;        // red egg
	tb_SetRect(&containerRect[63],	205,387,	267,	450);     gmCanSpawn[63]=true;   		  // purple egg
	tb_SetRect(&containerRect[64],	1,	411,	64,		475);     gmCanSpawn[64]=true;      	containerColor[64]=kForestDragonColor; // green egg
	tb_SetRect(&containerRect[65],	1,	411,	64,		475);     gmCanSpawn[65]=true;       	containerColor[65]=kWaterElementalColor; // blue egg

	tb_SetRect(&containerRect[66],	461,	391,	578,		526);     gmProp[66]=true;          // dead tree

	tb_SetRect(&containerRect[67],	205,	452,	268,		516);    gmCanSpawn[67]=true;         // blue flag
	tb_SetRect(&containerRect[68],	579,	422,	642,		486);    gmCanSpawn[68]=true;     // red flag
	tb_SetRect(&containerRect[69],	643,	422,	706,		486);    gmCanSpawn[69]=true;     // green flag
	tb_SetRect(&containerRect[70],	707,	422,	770,		486);    gmCanSpawn[70]=true;     // yellow flag

	tb_SetRect(&containerRect[71],	660,	196,	765,	291);	 		gmCanSpawn[71]=true;      containerIsCorpse[71]=true; 		fliesOnCorpse[71]=true;		 containerColor[71]=kSandSpiderColor; // lava spider

	tb_SetRect(&containerRect[72],	771,	422, 	829,	485);	gmCanSpawn[72]=true; //bottle

	tb_SetRect(&containerRect[73],	1,	66,	120,		185);	 			gmCanSpawn[73]=true;      containerIsCorpse[73]=true; 		fliesOnCorpse[73]=true;		containerColor[73]=kBlackDragonColor; // slug corpse
	tb_SetRect(&containerRect[74],	454,	1,	634,		115);	 			gmCanSpawn[74]=true;      containerIsCorpse[74]=true; 		fliesOnCorpse[74]=true;		containerColor[74]=kPinkDragonColor; // abyss dragon corpse

	tb_SetRect(&containerRect[75],	205,	517,	267,		580);  // Dragon tooth
	tb_SetRect(&containerRect[76],	269,	539,	331,		602);  // Piece of coal
	tb_SetRect(&containerRect[77],	333,	539,	395,		602);  // Amber
	tb_SetRect(&containerRect[78],	397,	539,	459,		602); // Eel
	tb_SetRect(&containerRect[79],	461,	527,	523,		590); // Log
	tb_SetRect(&containerRect[80],	525,	552,	587,		615);  // PG
	tb_SetRect(&containerRect[81],	771,	487,	829,		550);  gmCanSpawn[81]=true; // Empty bottle
	tb_SetRect(&containerRect[82],	830,	1,		960,		104);  gmProp[82]=true; // tent
	tb_SetRect(&containerRect[83],	717,	552,	779,		615);  gmCanSpawn[83]=true; //blue bottle
	tb_SetRect(&containerRect[84],	589,	552,	651,		615);  gmCanSpawn[84]=true; //red bottle
	tb_SetRect(&containerRect[85],	653,	552,	715,		615);  gmCanSpawn[85]=true; //yellow bottle
	tb_SetRect(&containerRect[86],	121,	66,		196,		129);  gmProp[86]=true; //tombstone

	tb_SetRect(&containerRect[87],	830,	357,	892,		420); //red crystal
	tb_SetRect(&containerRect[88],	894,	357,	956,		420); //green crystal
	tb_SetRect(&containerRect[89],	830,	422,	892,		485); //orange crystal
	tb_SetRect(&containerRect[90],	894,	422,	956,		485); //yellow crystal
	tb_SetRect(&containerRect[91],	830,	487,	892,		550); //black crystal
	tb_SetRect(&containerRect[92],	894,	487,	956,		550); //purple crystal
	tb_SetRect(&containerRect[93],	894,	552,	956,		621); //blue crystal
	tb_SetRect(&containerRect[94],	830,	552,	892,		621); //fire crystal

	tb_SetRect(&containerRect[95],	830,	162,	892,		225);  gmCanSpawn[81]=true; // Poison bottle

	tb_SetRect(&containerRect[96],	958,	407,	1058,		507);  gmCanSpawn[96]=true;      containerIsCorpse[96]=true; 		fliesOnCorpse[96]=true;	 // queen ant corpse
	tb_SetRect(&containerRect[97],	958,	508,	1077,		621);  gmCanSpawn[97]=true;      containerIsCorpse[97]=true; 		fliesOnCorpse[97]=true;	 // giant snake corpse

	tb_SetRect(&containerRect[98],	958,	277,	1020,		340);  gmCanSpawn[98]=true; //Candy

	tb_SetRect(&containerRect[99],	958,	212,	1020,		274);  gmCanSpawn[99]=true; //Cur 1
	tb_SetRect(&containerRect[100],	1022,	213,	1084,		274);  gmCanSpawn[100]=true; //Cur 2
	tb_SetRect(&containerRect[101],	1023,	147,	1083,		210);  gmCanSpawn[101]=true; //Cur 3
	tb_SetRect(&containerRect[102],	1022,	277,	1083,		338);  gmCanSpawn[102]=true; //Curio big
	tb_SetRect(&containerRect[103],	958,	147,	1020,		210);  gmCanSpawn[103]=true; //Curio small
	tb_SetRect(&containerRect[104],	1086,	1,		1209,		124);  gmCanSpawn[104]=true;      containerIsCorpse[104]=true; 		fliesOnCorpse[104]=true;	containerColor[104]=kRedDragonColor; // red draco corpse

	tb_SetRect(&containerRect[105],	959,	343,	1020,		405);  gmCanSpawn[105]=true; // Body
	tb_SetRect(&containerRect[106],	1086,	342,	1144,		405);  // Golden Box

	tb_SetRect(&containerRect[107],	1150,	226,	1212,		289);  // Pure blue crystal
	tb_SetRect(&containerRect[108],	1150,	421,	1212,		484);  // Pure red crystal
	tb_SetRect(&containerRect[109],	1150,	486,	1212,		549);  // Pure yellow crystal
	tb_SetRect(&containerRect[110],	1150,	291,	1212,		354);  // Pure orange crystal
	tb_SetRect(&containerRect[111],	1214,	226,	1276,		289);  // Pure green crystal
	tb_SetRect(&containerRect[112],	1150,	356,	1212,		419);  // Pure purple crystal
	tb_SetRect(&containerRect[113],	1150,	551,	1212,		614);  // Pure fire crystal
	tb_SetRect(&containerRect[114],	1214,	291,	1276,		354);  // Pure ice crystal
	tb_SetRect(&containerRect[115],	1214,	356,	1276,		419);  // Pure black crystal

	tb_SetRect(&containerRect[116],	1086,	199,	1148,		262);  gmCanSpawn[116]=true;  // brigand's head
	tb_SetRect(&containerRect[117],	1086,	264,	1148,		327);  gmCanSpawn[117]=true;  // sorcerer's head
	tb_SetRect(&containerRect[118],	1086,	126,	1158,		197);  gmCanSpawn[118]=true;      containerIsCorpse[118]=true; 		fliesOnCorpse[118]=true; // blink hound corpse
	tb_SetRect(&containerRect[119],	1160,	126,	1259,		224);  gmCanSpawn[119]=true;      containerIsCorpse[119]=true; 		fliesOnCorpse[119]=true; // gooey corpse
	tb_SetRect(&containerRect[120],	1086,	1,		1209,		124);  gmCanSpawn[120]=true;      containerIsCorpse[120]=true; 		fliesOnCorpse[120]=true;	containerColor[120]=kIceDragonColor; // ice draco corpse
	tb_SetRect(&containerRect[121],	1086,	1,		1209,		124);  gmCanSpawn[121]=true;      containerIsCorpse[121]=true; 		fliesOnCorpse[121]=true;	containerColor[121]=kForestDragonColor; // forest draco corpse
	tb_SetRect(&containerRect[122],	1086,	1,		1209,		124);  gmCanSpawn[122]=true;      containerIsCorpse[122]=true; 		fliesOnCorpse[122]=true;	containerColor[122]=kBlackDragonColor; // black draco corpse
	tb_SetRect(&containerRect[123],	1214,	425,	1276,		614);  gmProp[123]=true; //Andris walls
	tb_SetRect(&containerRect[124],	694,	131,	756,		194);  gmCanSpawn[124]=true; 	  containerIsCorpse[124]=true; 		fliesOnCorpse[124]=true;// Female corpse
	tb_SetRect(&containerRect[125],	1261,	1,		1425,		165);  gmCanSpawn[125]=true; 	  containerIsCorpse[125]=true; 		fliesOnCorpse[125]=true;// tusker corpse
	tb_SetRect(&containerRect[126], 1342,	226,	1404,		289);  gmProp[126]=true; //Cradle
	tb_SetRect(&containerRect[127], 1278,	226, 	1340, 		289);  gmCanSpawn[127]=true; //Green Dragon Egg
	tb_SetRect(&containerRect[128], 1278,	291,	1340,		354);  gmCanSpawn[128]=true; //Golden Dragon Egg
	tb_SetRect(&containerRect[129], 1278,	356,	1340,		419);  gmCanSpawn[129]=true; //Black Dragon Egg
	tb_SetRect(&containerRect[130], 1278,	421,	1340,		484);  gmCanSpawn[130]=true; //Red Dragon Egg
	tb_SetRect(&containerRect[131], 1278,	486,	1340,		549);  gmCanSpawn[131]=true; //Blue Dragon Egg
	tb_SetRect(&containerRect[132], 1278,	551,	1340,		614);  gmCanSpawn[132]=true; //Purple Dragon Egg
	tb_SetRect(&containerRect[133],	1342,	487,	1516,		614);  gmCanSpawn[133]=true; 	  containerIsCorpse[133]=true; 		fliesOnCorpse[133]=true;	containerColor[133]=kBlackDragonColor;// ancient black dragon corpse
	tb_SetRect(&containerRect[134],	1342,	487,	1516,		614);  gmCanSpawn[134]=true; 	  containerIsCorpse[134]=true; 		fliesOnCorpse[134]=true;	containerColor[134]=kForestDragonColor;// ancient forest dragon corpse
	tb_SetRect(&containerRect[135],	1342,	487,	1516,		614);  gmCanSpawn[135]=true; 	  containerIsCorpse[135]=true; 		fliesOnCorpse[135]=true;	containerColor[135]=kIceDragonColor;// ancient ice dragon corpse
	tb_SetRect(&containerRect[136],	1342,	487,	1516,		614);  gmCanSpawn[136]=true; 	  containerIsCorpse[136]=true; 		fliesOnCorpse[136]=true;	containerColor[136]=kRedDragonColor;// ancient black dragon corpse
	tb_SetRect(&containerRect[137],	1406,	226,	1481,		325);  gmCanSpawn[137]=true;      containerIsCorpse[137]=true; // lich corpse
	tb_SetRect(&containerRect[138],	1427,	2,		1543,		117);  gmCanSpawn[138]=true; 	  containerIsCorpse[138]=true; 		fliesOnCorpse[138]=true; //lion corpse
	tb_SetRect(&containerRect[139],	1342,	356,	1404,		419);  gmCanSpawn[139]=true; //rusty necklace
	tb_SetRect(&containerRect[140], 1406,	356,	1468,		419);  gmCanSpawn[140]=true; //nevia flower
	tb_SetRect(&containerRect[141],	1470,	356,	1531,		419);  gmCanSpawn[141]=true; //pure crystal
	tb_SetRect(&containerRect[142], 1545,	1,		1717,		126);  gmCanSpawn[142]=true; 		containerIsCorpse[142]=true; //golem corpse
	tb_SetRect(&containerRect[143], 1261,	167,	1317,		224);  gmCanSpawn[143]=true; //Red rose
	tb_SetRect(&containerRect[144], 1319,	167,	1375,		224);  gmCanSpawn[144]=true; //yellow rose
	tb_SetRect(&containerRect[145], 1377,	167,	1433,		224);  gmCanSpawn[145]=true; //blue rose
	tb_SetRect(&containerRect[146], 1435,	167,	1491,		224);  gmCanSpawn[146]=true; //black rose
	tb_SetRect(&containerRect[147], 894,	162,	956,		225);  gmCanSpawn[147]=true; //mushroom
	tb_SetRect(&containerRect[148], 1493,	161,	1555,		224);  gmCanSpawn[148]=true;  //raft
	tb_SetRect(&containerRect[149], 830,	227,	892,		290);  gmCanSpawn[149]=true; //book1
	tb_SetRect(&containerRect[150], 894,	227,	956,		290);  gmCanSpawn[150]=true;  //book2
	tb_SetRect(&containerRect[151], 830,	292,	892,		355);  gmCanSpawn[151]=true;  //book3
	tb_SetRect(&containerRect[152], 894,	292,	956,		355);  gmCanSpawn[152]=true; //book4
	tb_SetRect(&containerRect[153], 1557,	128,	1629,		200);  gmCanSpawn[153]=true; //liquor bottle
	tb_SetRect(&containerRect[154], 1631,	128,	1703,		200);  gmCanSpawn[154]=true; //mug
	tb_SetRect(&containerRect[155], 1557,	202,	1629,		274);  gmCanSpawn[155]=true; //lobster
	tb_SetRect(&containerRect[156], 326,	66,		388,		129);  gmCanSpawn[156]=true; //silver leaf
	tb_SetRect(&containerRect[157], 1483,	226,	1555,		275);  gmCanSpawn[157]=true;  //jink plushie
	tb_SetRect(&containerRect[158], 1483,	277,	1555,		325);  gmCanSpawn[158]=true;  //voodoo doll
	tb_SetRect(&containerRect[159], 1597,	350,	1669,		422);  gmCanSpawn[159]=true; //amber bottle
	tb_SetRect(&containerRect[160], 1671,	350,	1743,		422);  gmCanSpawn[160]=true; //clear bottle
	tb_SetRect(&containerRect[161], 1631,	276,	1703,		348);  gmCanSpawn[161]=true; containerColor[161]=kFireElementalColor;//pumpkin pie
	tb_SetRect(&containerRect[162], 1533,	379,	1595,		620);  gmProp[162]=true;//fort wall
	tb_SetRect(&containerRect[163], 1059,	407,	1144,		506);  gmProp[163]=true;//hedge
	tb_SetRect(&containerRect[164], 1557,	276,	1629,		348);  gmCanSpawn[164]=true; containerColor[164]=kPinkDragonColor;//wizard token
	tb_SetRect(&containerRect[165], 1557,	276,	1629,		348);  gmCanSpawn[165]=true; containerColor[165]=kForestDragonColor;//druid token
	tb_SetRect(&containerRect[166], 1557,	276,	1629,		348);  gmCanSpawn[166]=true; containerColor[166]=kSandSpiderColor;//cleric token
	tb_SetRect(&containerRect[167], 1557,	276,	1629,		348);  gmCanSpawn[167]=true; containerColor[167]=kWaterElementalColor;//fighter token
	tb_SetRect(&containerRect[168], 1557,	276,	1629,		348);  gmCanSpawn[168]=true; containerColor[168]=kFireElementalColor;//ranger token
	tb_SetRect(&containerRect[169], 1557,	276,	1629,		348);  gmCanSpawn[169]=true; containerColor[169]=kBrownWolfColor;//rogue token
	tb_SetRect(&containerRect[170], 1342,	291,	1404,		354);  gmCanSpawn[170]=true; containerColor[170]=kPinkDragonColor;//trout
	tb_SetRect(&containerRect[171], 1342,	291,	1404,		354);  gmCanSpawn[171]=true; //tuna
	tb_SetRect(&containerRect[172], 1342,	291,	1404,		354);  gmCanSpawn[172]=true; containerColor[172]=kPoisonElementalColor;//sea bass
	tb_SetRect(&containerRect[173], 1631,	202,	1703,		274);  gmCanSpawn[173]=true; containerColor[173]=kRedDragonColor;//red ring
	tb_SetRect(&containerRect[174], 1631,	202,	1703,		274);  gmCanSpawn[174]=true; containerColor[174]=kBlue;//blue ring
	tb_SetRect(&containerRect[175], 1631,	202,	1703,		274);  gmCanSpawn[175]=true; containerColor[175]=kPoisonElementalColor;//green ring
	tb_SetRect(&containerRect[176], 1631,	202,	1703,		274);  gmCanSpawn[176]=true; containerColor[176]=kSandSpiderColor;//yellow ring
	tb_SetRect(&containerRect[177], 1631,	202,	1703,		274);  gmCanSpawn[177]=true; containerColor[177]=kBrownWolfColor;//brown ring
	tb_SetRect(&containerRect[178], 1631,	202,	1703,		274);  gmCanSpawn[178]=true; containerColor[178]=kFireElementalColor;//orange ring
	tb_SetRect(&containerRect[179], 1631,	202,	1703,		274);  gmCanSpawn[179]=true; containerColor[179]=kPinkDragonColor;//pink ring
	tb_SetRect(&containerRect[180], 1631,	276,	1703,		348);  gmCanSpawn[180]=true; containerColor[180]=kSandSpiderColor;//banana pie
	tb_SetRect(&containerRect[181], 1631,	276,	1703,		348);  gmCanSpawn[181]=true; containerColor[181]=kRedDragonColor;//Apple pie
	tb_SetRect(&containerRect[182], 1631,	276,	1703,		348);  gmCanSpawn[182]=true; containerColor[182]=kPinkDragonColor;//lobster pie
	tb_SetRect(&containerRect[183], 1545,	1,		1717,		126);  gmCanSpawn[183]=true; 	containerColor[183]=kIceDragonColor;	containerIsCorpse[183]=true; //ice golem corpse
	tb_SetRect(&containerRect[184], 1545,	1,		1717,		126);  gmCanSpawn[184]=true; 	containerColor[184]=kRedDragonColor;    containerIsCorpse[184]=true; //magma  golem corpse
	tb_SetRect(&containerRect[185], 1597,	424,	1669,		496);  gmCanSpawn[185]=true; //skull
	tb_SetRect(&containerRect[186], 1631,	276,	1703,		348);  gmCanSpawn[186]=true; containerColor[186]=kBrownWolfColor;//eel pie
	tb_SetRect(&containerRect[187], 1671,	424,	1743,		496);  gmCanSpawn[187]=true;//crate
	tb_SetRect(&containerRect[188], 1671,	498,	1743,		570);  gmCanSpawn[188]=true; //OSU
	tb_SetRect(&containerRect[189], 1,		622,	83,			745);  gmCanSpawn[189]=true; gmProp[189]=true;//Rock
	tb_SetRect(&containerRect[190], 830,	617,	892,		680);  gmCanSpawn[190]=true;// ice crystal
	tb_SetRect(&containerRect[191], 1745,	61,		1899,		469);  gmProp[191]=true;// test tower
	tb_SetRect(&containerRect[192], 1597,	498,	1669,		570);  gmCanSpawn[192]=true; containerIsCorpse[192]=true; fliesOnCorpse[192]=true;// pixie body
	tb_SetRect(&containerRect[193], 1545,	1,		1717,		126);  gmCanSpawn[193]=true; 	containerColor[193]=kSandSpiderColor;    containerIsCorpse[193]=true; //sand  golem corpse
	tb_SetRect(&containerRect[194],	454,	1,		634,		115);  gmCanSpawn[194]=true;    containerColor[194]=kSandSpiderColor;	 containerIsCorpse[194]=true; 	fliesOnCorpse[194]=true;	 // sand dragon corpse
	tb_SetRect(&containerRect[195],	1086,	1,		1209,		124);  gmCanSpawn[195]=true;      containerIsCorpse[195]=true; 		fliesOnCorpse[195]=true;	containerColor[195]=kSandSpiderColor; // sand draco corpse
	tb_SetRect(&containerRect[196], 1,	1,		3,		3);  	gmProp[196]=true;// blank prop
	tb_SetRect(&containerRect[197],	172,	322,	247,	386);			gmCanSpawn[197]=true;      containerIsCorpse[197]=true; 	containerColor[197]=kBlackDragonColor;		fliesOnCorpse[197]=true;		 // serpent corpse
	tb_SetRect(&containerRect[198],	1597,	572,	1669,	644); gmCanSpawn[198]=true; // candle
	tb_SetRect(&containerRect[199],	1671,	572,	1743,	644); gmCanSpawn[199]=true; // birthday cake
	tb_SetRect(&containerRect[200],	 1,		1,		3,		3);   gmCanSpawn[200]=true;  containerIsCorpse[200]=true; fliesOnCorpse[200]=true;//flies drop
	tb_SetRect(&containerRect[201],	1,	411,	64,		475);      gmCanSpawn[201]=true;   	containerColor[201]=kBlackDragonColor; // black egg
	tb_SetRect(&containerRect[202],	1745,	471,	1846,570);      gmCanSpawn[202]=true;   	fliesOnCorpse[202]=true; containerIsCorpse[202]=true; containerColor[202]=kBlackDragonColor; // black bear corpse
	tb_SetRect(&containerRect[203],	1745,	471,	1846,570);      gmCanSpawn[203]=true;   	fliesOnCorpse[203]=true; containerIsCorpse[203]=true; containerColor[203]=kBrownWolfColor; // brown bear corpse
	tb_SetRect(&containerRect[204],	1745,	572,	1817,644);      gmCanSpawn[204]=true;   	 // charnath statue
	tb_SetRect(&containerRect[205], 1819,	572,	1891,		644);  gmCanSpawn[205]=true;// key white
	tb_SetRect(&containerRect[206], 1819,	572,	1891,		644);  gmCanSpawn[206]=true; containerColor[206]=kGreen;// key small
	tb_SetRect(&containerRect[207], 1819,	572,	1891,		644);  gmCanSpawn[207]=true; containerColor[207]=kPurple;// key rusty
//	tb_SetRect(&containerRect[208], 1819,	572,	1891,		644);  gmCanSpawn[208]=true; containerColor[208]=kPurple;// bronze yellow
	tb_SetRect(&containerRect[209], 1819,	572,	1891,		644);  gmCanSpawn[209]=true; containerColor[209]=kRed;// key red
	tb_SetRect(&containerRect[210], 85,	622,	191,		718);  gmCanSpawn[210]=true; fliesOnCorpse[210]=true; containerIsCorpse[210]=true;// shark corpse
	tb_SetRect(&containerRect[211],	830,	357,	892,		420); gmCanSpawn[211]=true;//Infused red crystal
	tb_SetRect(&containerRect[212],	894,	357,	956,		420); gmCanSpawn[212]=true;//Infused green crystal
	tb_SetRect(&containerRect[213],	830,	422,	892,		485); gmCanSpawn[213]=true;//Infused orange crystal
	tb_SetRect(&containerRect[214],	894,	422,	956,		485); gmCanSpawn[214]=true; //Infused yellow crystal
	tb_SetRect(&containerRect[215],	830,	487,	892,		550); gmCanSpawn[215]=true;//Infused black crystal
	tb_SetRect(&containerRect[216],	894,	487,	956,		550); gmCanSpawn[216]=true;//Infused purple crystal
	tb_SetRect(&containerRect[217],	894,	552,	956,		621); gmCanSpawn[217]=true;//Infused blue crystal
	tb_SetRect(&containerRect[218],	830,	552,	892,		621); gmCanSpawn[218]=true;//Infused fire crystal
	tb_SetRect(&containerRect[219],	1150,	226,	1212,		289); gmCanSpawn[219]=true; // Infused Pure blue crystal
	tb_SetRect(&containerRect[220],	1150,	421,	1212,		484); gmCanSpawn[220]=true; // Infused Pure red crystal
	tb_SetRect(&containerRect[221],	1150,	486,	1212,		549); gmCanSpawn[221]=true; // Infused Pure yellow crystal
	tb_SetRect(&containerRect[222],	1150,	291,	1212,		354); gmCanSpawn[222]=true; // Infused Pure orange crystal
	tb_SetRect(&containerRect[223],	1214,	226,	1276,		289); gmCanSpawn[223]=true; // Infused Pure green crystal
	tb_SetRect(&containerRect[224],	1150,	356,	1212,		419); gmCanSpawn[224]=true; // Infused Pure purple crystal
	//tb_SetRect(&containerRect[225],	1150,	551,	1212,		614); gmCanSpawn[225]=true; // Infused Pure fire crystal
	//tb_SetRect(&containerRect[226],	1214,	291,	1276,		354); gmCanSpawn[226]=true; // Infused Pure ice crystal
	tb_SetRect(&containerRect[227],	1214,	356,	1276,		419); gmCanSpawn[227]=true; // Infused Pure black crystal
	tb_SetRect(&containerRect[228],	964,	1,	1057,	100); gmCanSpawn[228]=true; fliesOnCorpse[228]=true; containerIsCorpse[228]=true;// Dwarf corpse
	tb_SetRect(&containerRect[229],	497,	116,	560,	180);	 	gmCanSpawn[229]=true;      containerIsCorpse[229]=true; containerColor[229]=kRedDragonColor;// hell mage corpse
	tb_SetRect(&containerRect[230], 193,	582,	249,	639);  gmCanSpawn[230]=true; //Purple rose
	tb_SetRect(&containerRect[231], 193,	641,	255,	721);  gmCanSpawn[231]=true; gmProp[231]=true;//barrel
	// kMaxContainerTypes=50

	//---------------------------------------------

	for (x=0;x<kMaxTracksTypes;x++)
		tb_SetRect(&tracksRect[x],0,0,0,0);

	tb_SetRect(&tracksRect[1],	1,1,64,64);  //human tracks E-W

	tb_SetRect(&tracksRect[2],	1,196,64,259); //human tracks N-S

	tb_SetRect(&tracksRect[3],	66,1,128,64); //dragon tracks E-W

	tb_SetRect(&tracksRect[4],	66,196,129,259); //dragon tracks N-S

	tb_SetRect(&tracksRect[5],	1,66,64,129); //note

	tb_SetRect(&tracksRect[6],	1,131,64,194); //arrow west

	tb_SetRect(&tracksRect[7],	261,196,330,259); //arrow NW

	tb_SetRect(&tracksRect[8],	332,196,395,259); //arrow north

	tb_SetRect(&tracksRect[9],	332,131,394,194); //arrow NE

	tb_SetRect(&tracksRect[10],	261,131,330,194); //arrow east

	tb_SetRect(&tracksRect[11],	196,131,259,194); //arrow SE

	tb_SetRect(&tracksRect[12],	131,131,194,194); //arrow south

	tb_SetRect(&tracksRect[13],	66,131,129,194); //arrow SW

	tb_SetRect(&tracksRect[14],	196,196,259,259); //Map X

	tb_SetRect(&tracksRect[15],	66,261,129,324); //animal tracks east/west

	tb_SetRect(&tracksRect[16],	131,261,194,324); //animal tracks north/south

	tb_SetRect(&tracksRect[17],	1,261,64,324); //blood tracks

	tb_SetRect(&tracksRect[18], 332,261,395,324); //sign
	
	tb_SetRect(&tracksRect[19], 261,326,330,389); //dragon tracks south
	
	tb_SetRect(&tracksRect[20],196,326,259,389); //dragon west
	
	tb_SetRect(&tracksRect[21],1,326,64,389); //runes
	tb_SetRect(&tracksRect[22],66,326,129,389);
	tb_SetRect(&tracksRect[23],131,326,194,389);
	tb_SetRect(&tracksRect[24],1,391,64,454);
	tb_SetRect(&tracksRect[25],66,391,129,454);
	tb_SetRect(&tracksRect[26],131,391,194,454);
	tb_SetRect(&tracksRect[27],397,0,656,259); //Ground patch //Dave
	tb_SetRect(&tracksRect[28],332,391,395,454); //web test//Dave
	tb_SetRect(&tracksRect[29],398,416,492,511); // leaves //Dave
	tb_SetRect(&tracksRect[30],398,513,492,608); // leaves 2//Dave
	tb_SetRect(&tracksRect[31],494,416,589,511); // leaves 3//Dave
	tb_SetRect(&tracksRect[32],494,513,589,608); // leaves 4//Dave
	tb_SetRect(&tracksRect[33],397,610,492,705); //whirlpool//Dave
	
	
	//tracks above 50, appear over the player
	
	tb_SetRect(&tracksRect[51], 131,196,194,259); //bracken root
	
	tb_SetRect(&tracksRect[52],397,261,510,413); //web test//Dave
	
	tb_SetRect(&tracksRect[53],658,0,1079,398); //cage//Dave
	
	tb_SetRect(&tracksRect[54],591,401,1012,796); //smoke//Dave
	
	
	
	
	//tb_SetRect(&tracksRect[55],2,2,4,4); //invisible track//Dave

	//---------------------------------------------

	for (x=0;x<kMaxScreenElements;x++)
	{
		gr_MakeRect(&destRect[x], 0, 0, 0, 0);
		creatureDirection[x]=4;
		creatureFacing[x]=0;
	}

	gr_MakeRect(&startRect,410,14,63,64);

	tb_OffsetRect(&startRect,-58,-463);
	tb_SetRect(&s,0,0,544,544);
	tb_SetRect(&s,0,0,560,560);

	//tb_SetRect(&s,0-64,0-64,560+32,560+200+150);
	tb_SetRect(&s,0-8,0-8,560+8,560+175);

	r=startRect;
	index=0;
	row=22;
	col=25;

	for (x=0;x<50;x++)	//24
	{
		for (y=0;y<50;y++)	//26
		{
			if (x>=24)
				SetRect(&s,0-8,0-8,560+8,560+200);

			if (tb_IntersectRect(&u,&r,&s))
			{
				if (index>=kMaxScreenElements)
					{ fi_WriteToErrorLog("Index out of range - InitializeRectangles"); tb_Halt(); }

				destRect[index]=r;

				//destScreenRect[index]=destRect[index];
				//tb_OffsetRect(&destScreenRect[index],horizontalOffset-24,verticalOffset-48);
				//destScreenRect[index].top=destScreenRect[index].bottom-64;
				mapToScreenRow[index]=row;
				mapToScreenCol[index]=col;
				index++;

				if (index==kMaxScreenElements)
					tb_Idle();
			}

			tb_OffsetRect(&r,32,32);

			row--;
		}

		tb_OffsetRect(&startRect,-32,32);

		r=startRect;
		row=22;
		col--;
	}

}

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderPlayerDisplay(TBoolean /*drawOnScreen*/)
// ----------------------------------------------------------------- //

{
	//TRect										r;


	if (gotFirstDrawMessage==false)
	{
		gotFirstDrawMessage=true;
		//tb_DrawPictureToBitmap(tbWindow, 130, horizontalOffset, verticalOffset, &r);
	}


	gr_RenderInventoryWindow();

	gr_RenderCalendarWindow();
	//gr_RenderPackContentsWindow();//
	gr_RenderShopContentsWindow();
	gr_RenderSellListWindow();
	gr_RenderMakeListWindow(currentSkillDisplay, currentCategory);
	gr_RenderSkillsWindow();
	gr_RenderStatsWindow();
	//gr_RenderBankWindow();//

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderStatsWindow(void)
// ----------------------------------------------------------------- //

{
	//TRect								o;
	//TRect								s;
	int									i, tmpclass;
	char								characterClass[kStringLength];
	char								hungerStr[kStringLength];
	char								itemStr[kStringLength];

	if (windowVisible[kStatsWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	tb_CopyBitmap(statsBMP, bufferBMP, &statsRect, &statsRect, false);

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 9);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	localWeight=0;

	for (i=1;i<kMaxInventorySize;i++)
		if (localInventory[i].itemID!=0)
			if (in_CanGroup(localInventory[i].itemID))
				localWeight=localWeight+itemList[localInventory[i].itemID].weight+(itemList[localInventory[i].itemID].weight*localInventory[i].itemCount)/10;
			else
				localWeight=localWeight+itemList[localInventory[i].itemID].weight;

	tmpclass=localClass;

	if (localClass>10)
		localClass-=10;

	if (localClass==1)
		strcpy(characterClass,"Cleric");
	else if (localClass==2)
		strcpy(characterClass,"Druid");
	else if (localClass==3)
		strcpy(characterClass,"Fighter");
	else if (localClass==4)
		strcpy(characterClass,"Ranger");
	else if (localClass==5)
		strcpy(characterClass,"Rogue");
	else if (localClass==6)
		strcpy(characterClass,"Wizard");
	else if (localClass==7)
		strcpy(characterClass,"N/A");
	else if (localClass==8)
		strcpy(characterClass,"Counselor");
	else if (localClass==9)
		strcpy(characterClass,"Game Master");
	else if (localClass==10)
		strcpy(characterClass,"Developer");
	else
		strcpy(characterClass,"N/A");

	localClass=tmpclass;

	tb_NumToString(localLevel,itemStr);

	strcat(characterClass," level ");

	strcat(characterClass,itemStr);

	tb_DrawString(bufferBMP, 650-547, 13+(0*16), characterClass );

	if (localHunger==0)
		strcpy(hungerStr,"Starving");
	else if (localHunger<1750)
		strcpy(hungerStr,"Very Hungry");
	else if (localHunger<3500)
		strcpy(hungerStr,"A bit Hungry");
	else if (localHunger<5250)
		strcpy(hungerStr,"Full");
	else if (localHunger<=7000)
		strcpy(hungerStr,"Very Full");

	if (playerDead)
	{
		tb_DrawString(bufferBMP, 662-7-547, 14+(1*20), "-" );
		tb_DrawString(bufferBMP, 662-7-547, 16+(2*20), "-" );
		tb_DrawString(bufferBMP, 662-7-547, 18+(3*20), "-" );
		tb_DrawString(bufferBMP, 662-7-547, 20+(4*20), "-" );
		tb_DrawString(bufferBMP, 779-7-547, 14+(1*20), "-" );
		tb_DrawString(bufferBMP, 779-7-547, 16+(2*20), "-" );
		tb_DrawString(bufferBMP, 779-7-547, 18+(3*20), "-" );
		tb_DrawString(bufferBMP, 724-7-547, 20+(4*20), "-" );
	}
	else
	{
		tb_DrawNumber(bufferBMP, 662-547, 14+(1*20), localStrength, 			tb_Right);
		tb_DrawNumber(bufferBMP, 662-547, 16+(2*20), localIntelligence, 	tb_Right);
		tb_DrawNumber(bufferBMP, 662-547, 18+(3*20), localDexterity, 		tb_Right);
		tb_DrawNumber(bufferBMP, 662-547, 20+(4*20), localArmorRating, 	tb_Right);
		tb_DrawNumber(bufferBMP, 779-547, 14+(1*20), localHitPoints, 		tb_Right);
		tb_DrawNumber(bufferBMP, 779-547, 16+(2*20), localMagicPoints, 	tb_Right);
		tb_DrawNumber(bufferBMP, 779-547, 18+(3*20), localWeight, 				tb_Right);
		tb_DrawString(bufferBMP, 724-547, 20+(4*20), hungerStr);
	}

	if (windowActive[kStatsWindow])
		tb_CopyBitmap(bufferBMP, tbStatsWindow, &statsRect, &statsRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbStatsWindow, &statsRect, &statsRect, false);

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderSellListWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	int						line;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	char					costStr[kStringLength]; //[25];
	UInt32				gold;	// version 0.1.1

	if (windowVisible[kSellListWindow]==false)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 190, 0, 0, &d);	// sell title

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	// version 0.1.1 --------------------------------------------------------------
	tb_DrawString(bufferBMP, 553-547, 70+(0*lineSpace)-18, "Gold Pieces:");
	gold=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);
	tb_IntegerToString(gold,costStr);
	strcat(costStr," gp");
	tb_DrawString(bufferBMP, 777-(tb_StringWidth(bufferBMP,costStr))-547, 70+(0*lineSpace)-18, costStr);
	// ----------------------------------------------------------------------------

	line=1;

	for (i=1;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&pickUpRect[i],	0, 0, 0, 0);

		if (localInventory[i].itemID!=0)
			if (sh_InSellList(localInventory[i].itemID))
			{

				if (in_CanGroup(localInventory[i].itemID))
					in_GetItemName(localInventory[i].itemID, itemName, 0);
				else
					in_GetItemName(localInventory[i].itemID, itemName, localInventory[i].itemCount);

				tb_DrawPictureToBitmap(bufferBMP, 135, 553-547, 75+(line*lineSpace)-10-18, &pickUpRect[i]);

				tb_OffsetRect(&pickUpRect[i],horizontalOffset,verticalOffset);

				tb_DrawString(bufferBMP, 570-547, 75+(line*lineSpace)-18, itemName);

				tb_IntegerToString(sh_SellPriceOf(localInventory[i].itemID),costStr);

				strcat(costStr," gp");

				tb_DrawString(bufferBMP, 777-(tb_StringWidth(bufferBMP,costStr))-547, 75+(line*lineSpace)-18, costStr);

				line++;

			}
	}

	if (windowActive[kSellListWindow])
		tb_CopyBitmap(bufferBMP, tbSellListWindow, &r, &sellListWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbSellListWindow, &r, &sellListWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderShopContentsWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	int						line;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	char					costStr[kStringLength]; //[25];
	UInt32				gold;	// version 0.1.1

	if (windowVisible[kShopContentsWindow]==false)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 189, 0, 0, &d);		// buy title

	d=r;

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	tb_DrawString(bufferBMP, 553-547, 70+(0*lineSpace)-18, "Gold Pieces:");
	gold=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);
	tb_IntegerToString(gold,costStr);
	strcat(costStr," gp");
	tb_DrawString(bufferBMP, 777-(tb_StringWidth(bufferBMP,costStr))-547, 70+(0*lineSpace)-18, costStr);

	line=1;

	for (i=0;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&pickUpRect[i],	0, 0, 0, 0);

		if (shopInventory[i].itemID!=0)
		{

			if (in_CanGroup(shopInventory[i].itemID))
				in_GetItemName(shopInventory[i].itemID, itemName, 0);
			else
				in_GetItemName(shopInventory[i].itemID, itemName, 127);

			tb_DrawPictureToBitmap(bufferBMP, 132, 553-547, 75+(line*lineSpace)-10-18, &pickUpRect[i]);

			tb_DrawString(bufferBMP, 570-547, 75+(line*lineSpace)-18, itemName);

			tb_IntegerToString(shopInventory[i].cost,costStr);

			strcat(costStr," gp");

			tb_DrawString(bufferBMP, 777-(tb_StringWidth(bufferBMP,costStr))-547, 75+(line*lineSpace)-18, costStr);

			line++;

		}
	}

	if (windowActive[kShopContentsWindow])
		tb_CopyBitmap(bufferBMP, tbShopContentsWindow, &r, &shopContentsWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbShopContentsWindow, &r, &shopContentsWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderIDDisplayWindow(void)
// ----------------------------------------------------------------- //

{
	//int						i;
	int						line;
	int						lineSpace	= 13;
	//char 					itemName[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	char					itemStr[kStringLength]; //[64];
	char					title[kStringLength]; //[64];
	char					guild[kStringLength]; //[64];
	char          characterClass[kStringLength];
	//Dave
	//int count;

	if (windowVisible[kIDWindow]==false)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 226, 0, 0, &d);	// pick up title

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 42768, 42768, 42768);
		//tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	line=70-18;

	if (idCreatureID!=kHuman)
	{
		tb_SetFontStyle(bufferBMP, tb_Bold);
		tb_DrawString(bufferBMP, 553-547, line, "Creature Type:");
		tb_SetFontStyle(bufferBMP, tb_Plain);
		line=line+16;

		tx_GetCreatureName(idCreatureID, itemStr);
		tb_DrawString(bufferBMP, 558-547, line, itemStr);

		line=line+16;

		tb_NumToString(idClassLevel,itemStr);
		strcpy(characterClass,"Level ");
		strcat(characterClass,itemStr);
		tb_DrawString(bufferBMP, 558-547, line, characterClass);

		//if (creatureInfo[idCreatureID].tamingLevel<101)

		if (idTitle!=3)
		{
			line=line+22;
			tb_SetFontStyle(bufferBMP, tb_Bold);
			tb_DrawString(bufferBMP, 553-547, line, "Status:");
			tb_SetFontStyle(bufferBMP, tb_Plain);
			line=line+16;

			if (idTitle==0)
				tb_DrawString(bufferBMP, 558-547, line, "Wild");
			else
			{
				tb_DrawString(bufferBMP, 558-547, line, "Tame");
				line=line+16;
				strcpy(guild,"Name:  ");
				strcat(guild,idPetName);
				tb_DrawString(bufferBMP, 558-547, line, guild);
			}
		}

		if (strcmp(idNPCName,"")!=0)
		{
			line=line+16;
			strcpy(title,"Name:  ");
			strcat(title,idNPCName);
			tb_SetFontStyle(bufferBMP, tb_Plain);
			tb_DrawString(bufferBMP, 558-547, line, title);
		}

	}
	else
	{
		tb_SetFontStyle(bufferBMP, tb_Bold);
		tb_DrawString(bufferBMP, 553-547, line, "Creature Type:");
		tb_SetFontStyle(bufferBMP, tb_Plain);
		line=line+16;

		if (idTitle==98)
		{
			if (idGender==0)
				tb_DrawString(bufferBMP, 558-547, line, "Ghost");
			else
				tb_DrawString(bufferBMP, 558-547, line, "Ghost");
		}
		else
		{
			if (idGender==0)
				tb_DrawString(bufferBMP, 558-547, line, "Human Male");
			else
				tb_DrawString(bufferBMP, 558-547, line, "Human Female");
		}

		line=line+22;

		tb_SetFontStyle(bufferBMP, tb_Bold);
		tb_DrawString(bufferBMP, 553-547, line, "Identification:");
		tb_SetFontStyle(bufferBMP, tb_Plain);
		line=line+16;

		if (idGender==0)
		{
			//Dave 12-03-2012 adding negative title EXP for scoundrels
			if (idTitle==10)					strcpy(title,"Name:  The Vagrant ");

			if (idTitle==11)					strcpy(title,"Name:  The Annoying ");

			if (idTitle==12)					strcpy(title,"Name:  The Wicked ");

			if (idTitle==13)					strcpy(title,"Name:  The Horrible ");

			if (idTitle==14)					strcpy(title,"Name:  The Notorious ");

			if (idTitle==15)					strcpy(title,"Name:  Master Scoundrel ");
			
			
			if (idTitle==0)					strcpy(title,"Name:  The Peasant "); // 0

			if (idTitle==1)					strcpy(title,"Name:  Citizen ");  // 1

			if (idTitle==2)					strcpy(title,"Name:  The Respectable ");

			if (idTitle==3)					strcpy(title,"Name:  The Honorable ");

			if (idTitle==4)					strcpy(title,"Name:  The Illustrious ");

			if (idTitle==5)					strcpy(title,"Name:  The Distinguished "); // 5

			if (idTitle==6)					strcpy(title,"Name:  The Noble ");

			if (idTitle==7)					strcpy(title,"Name:  Master ");

			if (idTitle==8)					strcpy(title,"Name:  Sir ");

			if (idTitle==9)					strcpy(title,"Name:  Lord ");   // 9

			if (idTitle==90)				strcpy(title,"Name:  Developer ");

			if (idTitle==91)				strcpy(title,"Name:  Administrator ");

			if (idTitle==92)				strcpy(title,"Name:  Magistrate ");

			if (idTitle==93)				strcpy(title,"Name:  Chief Justice ");

			if (idTitle==94)				strcpy(title,"Name:  Guardian ");

			if (idTitle==95)				strcpy(title,"Name:  Seer ");

			if (idTitle==96)				strcpy(title,"Name:  Game Master ");

			if (idTitle==97)				strcpy(title,"Name:  Counselor ");

			if (idTitle==98)				strcpy(title,"Name:  The Ghost of ");

			if (idTitle==99)				strcpy(title,"Name:  The Scoundrel ");
		}
		else //Female
		{
			if (idTitle==10)					strcpy(title,"Name:  The Wench ");

			if (idTitle==11)					strcpy(title,"Name:  The Annoying ");

			if (idTitle==12)					strcpy(title,"Name:  The Wicked ");

			if (idTitle==13)					strcpy(title,"Name:  The Horrible ");

			if (idTitle==14)					strcpy(title,"Name:  The Notorious ");

			if (idTitle==15)					strcpy(title,"Name:  Mistress Picaroon ");
					
		
			if (idTitle==0)					strcpy(title,"Name:  The Peasant ");

			if (idTitle==1)					strcpy(title,"Name:  Citizen ");

			if (idTitle==2)					strcpy(title,"Name:  The Respectable ");

			if (idTitle==3)					strcpy(title,"Name:  The Honorable ");

			if (idTitle==4)					strcpy(title,"Name:  The Illustrious ");

			if (idTitle==5)					strcpy(title,"Name:  The Distinguished ");

			if (idTitle==6)					strcpy(title,"Name:  The Noble ");

			if (idTitle==7)					strcpy(title,"Name:  Mistress "); //mistress

			if (idTitle==8)					strcpy(title,"Name:  Dame "); // Madam

			if (idTitle==9)					strcpy(title,"Name:  Lady ");

			if (idTitle==90)				strcpy(title,"Name:  Developer ");

			if (idTitle==91)				strcpy(title,"Name:  Administrator ");

			if (idTitle==92)				strcpy(title,"Name:  Magistrate ");

			if (idTitle==93)				strcpy(title,"Name:  Chief Justice ");

			if (idTitle==94)				strcpy(title,"Name:  Guardian ");

			if (idTitle==95)				strcpy(title,"Name:  Seer ");

			if (idTitle==96)				strcpy(title,"Name:  Game Master ");

			if (idTitle==97)				strcpy(title,"Name:  Counselor ");

			if (idTitle==98)				strcpy(title,"Name:  The Ghost of ");

			if (idTitle==99)				strcpy(title,"Name:  The Scoundrel ");
		}

		if (strcmp(idCustomTitle, "None")!=0 && strcmp(idCustomTitle, "none")!=0)
		{
			strcpy(title, "Name:  ");
			strcat(title, idCustomTitle);
			strcat(title, " ");
		}

		strcat(title,idPlayerName);

		tb_DrawString(bufferBMP, 558-547, line, title);
		line=line+16;

		if (idTitle!=98)
		{
			tb_SetFontStyle(bufferBMP, tb_Plain);

			/*	if (idClass==1)
			    strcpy(characterClass,"Class:  Cleric");
				else if (idClass==2)
			    strcpy(characterClass,"Class:  Druid");
				else if (idClass==3)
			    strcpy(characterClass,"Class:  Fighter");
				else if (idClass==4)
			    strcpy(characterClass,"Class:  Ranger");
				else if (idClass==5)
			    strcpy(characterClass,"Class:  Rogue");
				else if (idClass==6)
			    strcpy(characterClass,"Class:  Wizard");
				else if (idClass==7)
			    strcpy(characterClass,"Class:  N/A");
				else if (idClass==8)
			    strcpy(characterClass,"Class:  Counselor");
				else if (idClass==9)
			    strcpy(characterClass,"Class:  Game Master");
				else if (idClass==10)
			    strcpy(characterClass,"Class:  Developer");
				else
			    strcpy(characterClass,"Class:  N/A");
			  
			  //if (idClass<7)
			  //	{
			  		tb_NumToString(idClassLevel,itemStr);
			  	strcat(characterClass," Level ");
			  	strcat(characterClass,itemStr);
			//	}
			  
			  tb_DrawString(bufferBMP, 558-547, line, characterClass);

				line=line+16;*/  //02/14/2005 - Aslan's idea

			strcpy(guild,"Guild:  ");
			strcat(guild,idGuildName);
			tb_DrawString(bufferBMP, 558-547, line, guild);
			//Dave removed breaker
				/*
			line=line+16;
			if (idKarma==0)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Unbelievably Poor");
			if (idKarma==1)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Extremely Poor");
			if (idKarma==2)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Very Poor");
			if (idKarma==3)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Poor");
			if (idKarma==4)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Average");
			if (idKarma==5)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Good");
			if (idKarma==6)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Very Good");
			if (idKarma==7)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Extremely Good");
			if (idKarma==8)	tb_DrawString(bufferBMP, 558-547, line, "Karma: Unbelievably Good");
				
			*/
			
			
			line=line+22;

			tb_SetFontStyle(bufferBMP, tb_Bold);

			tb_DrawString(bufferBMP, 553-547, line, "Currently Armed:");

			tb_SetFontStyle(bufferBMP, tb_Plain);
			line=line+16;

			if (idRightHand!=0)
			{
				in_GetItemName(idRightHand, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idLeftHand!=0)
			{
				in_GetItemName(idLeftHand, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idHead!=0)
			{
				in_GetItemName(idHead, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idNeck!=0)
			{
				in_GetItemName(idNeck, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idTorso!=0)
			{
				in_GetItemName(idTorso, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idWaist!=0)
			{
				in_GetItemName(idWaist, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idLegs!=0)
			{
				in_GetItemName(idLegs, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idFeet!=0)
			{
				in_GetItemName(idFeet, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idArms!=0)
			{
				in_GetItemName(idArms, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idHands!=0)
			{
				in_GetItemName(idHands, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idRightFinger!=0)
			{
				in_GetItemName(idRightFinger, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

			if (idLeftFinger!=0)
			{
				in_GetItemName(idLeftFinger, itemStr, -1);
				tb_DrawString(bufferBMP, 558-547, line, itemStr);
				line=line+13;
			}

		}

		//Guild emblems

		if (strcmp(idGuildName,"Serpent's Pledge")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3000, 0, 315, &d);

		if (strcmp(idGuildName,"Mirith Vanguard")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3001, 0, 315, &d);

		if (strcmp(idGuildName,"Dawnstriders")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3002, 0, 315, &d);

		if (strcmp(idGuildName,"Oberin Vanguard")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3003, 0, 315, &d);

		if (strcmp(idGuildName,"The Guardians")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3004, 0, 315, &d);

		if (strcmp(idGuildName,"The Roman Empire")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3005, 0, 315, &d);

		if (strcmp(idGuildName,"Lerilin Vanguard")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3007, 0, 315, &d);

		if (strcmp(idGuildName,"Andris Vanguard")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3008, 0, 315, &d);

		if (strcmp(idGuildName,"GYPSY")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3009, 0, 315, &d);

		if (strcmp(idGuildName,"Three Paths")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3010, 0, 315, &d);

		if (strcmp(idGuildName,"Sect of the Crystal")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3011, 0, 315, &d);

		if (strcmp(idGuildName,"Rhovanion Rebels")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3012, 0, 315, &d);

		if (strcmp(idGuildName,"The Looting Jesters")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3013, 0, 315, &d);

		if (strcmp(idGuildName,"Whe-T'ang")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3014, 0, 315, &d);

		if (strcmp(idGuildName,"Dragon Riders")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3015, 0, 315, &d);

		if (strcmp(idGuildName,"Samourai Freiheit")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3016, 0, 315, &d);

		if (strcmp(idGuildName,"The Gestalts")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3017, 0, 315, &d);

		if (strcmp(idGuildName,"Children of the Sun")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3018, 0, 315, &d);

		if (strcmp(idGuildName,"Ordo Illuminatis")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3019, 0, 315, &d);

		if (strcmp(idGuildName,"Black Roses")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3020, 0, 315, &d);
		
		if (strcmp(idGuildName,"Diluculo Guardians")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3021, 0, 315, &d);
		
		if (strcmp(idGuildName,"Abashiri")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3022, 0, 315, &d);
		
		if (strcmp(idGuildName,"Circle of Friends")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3023, 0, 315, &d);
		
		if (strcmp(idGuildName,"S.R.C.N.")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3024, 0, 315, &d);
		
		if (strcmp(idGuildName,"Red Battalion")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3025, 0, 315, &d);
		
		if (strcmp(idGuildName,"Equitas Concilium")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3026, 0, 315, &d);
		
		if (strcmp(idGuildName,"Tipsy Socialists")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3027, 0, 315, &d);
		
		if (strcmp(idGuildName,"Guardians")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3028, 0, 315, &d);
		
		if (strcmp(idGuildName,"Shadow Clan")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3029, 0, 315, &d);
		
		if (strcmp(idGuildName,"Cavalier Knights")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3030, 0, 315, &d);
		
		if (strcmp(idGuildName,"Marali Vanguard")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3031, 0, 315, &d);
		
		if (strcmp(idGuildName,"Wardens of Faith")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3033, 0, 315, &d);
		
		if (strcmp(idGuildName,"The Peace Givers")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3034, 0, 315, &d);
		
		if (strcmp(idGuildName,"Phoenix Dawn")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3035, 0, 315, &d);
		
		if (strcmp(idGuildName,"Suzugane")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3036, 0, 315, &d);
		
		if (strcmp(idGuildName,"Crystal Raiders")==0)
			tb_DrawPictureToBitmap(bufferBMP,  3038, 0, 315, &d);
		
		if (strcmp(idGuildName,"Citrinitas Aegis")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3039, 0, 315, &d);
		
		if (strcmp(idGuildName,"The Pillars")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3040, 0, 315, &d);
	
		if (strcmp(idGuildName,"The Lost Souls")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3041, 0, 315, &d);
		
		if (strcmp(idGuildName,"Gold of the Gods")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3042, 0, 315, &d);
		
		if (strcmp(idGuildName,"The Black Hand")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3043, 0, 315, &d);
			
		if (strcmp(idGuildName,"Black Hand")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3044, 0, 315, &d);
		
		if (strcmp(idGuildName,"USC")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3045, 0, 315, &d);
		
		if (strcmp(idGuildName,"Buckeye")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3046, 0, 315, &d);
		
		if (strcmp(idGuildName,"Dragon's Gate")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3047, 0, 315, &d);
		
		if (strcmp(idGuildName,"Raven Shield")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3048, 0, 315, &d);
		
		if (strcmp(idGuildName,"The Arctic Legion")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3049, 0, 315, &d);
		
		if (strcmp(idGuildName,"Ebony Empire")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3050, 0, 315, &d);
		
		if (strcmp(idGuildName,"Scamp")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3051, 0, 315, &d);
		
		if (strcmp(idGuildName,"Ordo Chao")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3052, 0, 315, &d);
		
		if (strcmp(idGuildName,"The Hidden Vale")==0)
			tb_DrawPictureToBitmap(bufferBMP, 3053, 0, 315, &d);
		
	}

	if (windowActive[kIDWindow])
		tb_CopyBitmap(bufferBMP, tbIDWindow, &r, &idWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbIDWindow, &r, &idWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderSkillsWindow(void)
// ----------------------------------------------------------------- //

{
	int						skill;
	int						lineSpace	= 13;
	TRect					r;
	TRect					d;
	char 					skillName[kStringLength]; //[32];
	int						total=0;
	int						theCheck;
	int cpt;

	if (windowVisible[kSkillsWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	/*if (playerDead)
		theCheck=131;
	else
		theCheck=140;*/

	tb_DrawPictureToBitmap(bufferBMP, 238, 0, 0, &d);		// skills title

	if (playerDead)
	{
		theCheck=131;
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		theCheck=140;
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont			(bufferBMP, kFontIDGeneva);

	tb_SetFontSize	(bufferBMP, 10);
	tb_SetFontStyle	(bufferBMP, tb_Plain);

	cpt=0;

	for (skill=0;skill<kMaxSkills;skill++)
		if (localClass>6 || maxSkillLevel[localClass][skill]>0 || questChar[90] || (localRightFinger!=0 && itemList[itemList[localInventory[localRightFinger].itemID].spell].makeSkill==skill) || (localLeftFinger!=0 && itemList[itemList[localInventory[localLeftFinger].itemID].spell].makeSkill==skill))
		{
			if (sk_IsUseableSkill(skill))
			{
				tb_DrawPictureToBitmap(bufferBMP, theCheck, 553-547, 70+(cpt*lineSpace)-10-18, &skillRect[skill]);
				skillRect[skill].right=764-547-65;
			}
			else
				tb_SetRect(&skillRect[skill],0,0,0,0);

			tx_GetSkillName(skill, skillName);

			tb_DrawString(bufferBMP, 567-547, 70+(cpt*lineSpace)-18, skillName);

			tb_DrawNumber(bufferBMP, 763-547-70, 70+(cpt*lineSpace)-18, localSkills[skill].level, tb_Right);

			cpt++;
		}
	tb_SetRect(&r,0,0,152,42+(13.2*cpt));

	if (windowActive[kSkillsWindow])
		tb_CopyBitmap(bufferBMP, tbSkillsWindow, &r, &skillsWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbSkillsWindow, &r, &skillsWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderCalendarWindow(void)
// ----------------------------------------------------------------- //

{
	TRect					r;
	TRect					d;
	char 					theSentence[kStringLength];
	char					toStr[kStringLength];
	int						total=0;
	int 					cpt, cpt2;

	if (windowVisible[kCalendarWindow]==false)
		return;


	tb_CopyBitmap(CalendarBMP, bufferBMP, &CalendarWindowRect, &CalendarWindowRect, false);


	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont		(bufferBMP, kFontIDGeneva);

	tb_SetFontSize	(bufferBMP, 10);
	tb_SetFontStyle	(bufferBMP, tb_Plain);

	if (currentYear==0)
		strcpy(theSentence, "Initializing Calendar...");
	else
	{
		tb_NumToString(currentYear+240, toStr);
		strcpy(theSentence, toStr);
	}

	tb_DrawString(bufferBMP, 40, 39, theSentence);

	switch (currentCycle)
	{
		case	kWakingMoon	:
			strcpy(theSentence, "Waking Moon");
			break;

		case	kGrowingMoon	:
			strcpy(theSentence, "Growing Moon");
			break;

		case	kBleedingMoon	:
			strcpy(theSentence, "Bleeding Moon");
			break;


		case	kRedMoon	:
			strcpy(theSentence, "Red Moon");
			break;

		case	kHarvestMoon	:
			strcpy(theSentence, "Harvest Moon");
			break;

		case	kDancingMoon	:
			strcpy(theSentence, "Dancing Moon");
			break;

		case	kLuckyMoon	:
			strcpy(theSentence, "Lucky Moon");
			break;

		case	kBlueMoon	:
			strcpy(theSentence, "Blue Moon");
			break;

		case	kFightingMoon	:
			strcpy(theSentence, "Fighting Moon");
			break;

		case	kSleepingMoon	:
			strcpy(theSentence, "Sleeping Moon");
			break;
	}

	tb_DrawString(bufferBMP, 40, 57, theSentence);

	tb_NumToString(currentCalDay, toStr);
	strcpy(theSentence, toStr);
	tb_DrawString(bufferBMP, 40, 75, theSentence);

	strcpy(theSentence, "");

	if (currentHour<10)
		strcat(theSentence, "0");

	tb_NumToString(currentHour, toStr);

	strcat(theSentence, toStr);

	strcat(theSentence, ":");

	if (currentMinute<10)
		strcat(theSentence, "0");

	tb_NumToString(currentMinute, toStr);

	strcat(theSentence, toStr);

	tb_DrawString(bufferBMP, 40, 93, theSentence);

	cpt=2501+currentCycle*8;

	cpt2=cycleDays[currentCycle]/8;

	while (cpt2<currentCalDay)
	{
		cpt++;
		cpt2+=cycleDays[currentCycle]/8;
	}

	tb_DrawPictureToBitmap(bufferBMP, cpt, 138, 0, &d);

	tb_SetRect(&r,0,0,238,100);

	if (windowActive[kCalendarWindow])
		tb_CopyBitmap(bufferBMP, tbCalendarWindow, &r, &CalendarWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbCalendarWindow, &r, &CalendarWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderPackContentsWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	int						line;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	UInt32				gold;
	char					goldStr[kStringLength]; //[64];
	char          corpse[kStringLength]; //[40];

	if (windowVisible[kPackContentsWindow]==false)
		return;

	if (pickUpInventory[0].itemID==60001)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 187, 0, 0, &d);	// pick up title

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	if (strcmp(localContainerName,"")!=0)
	{
		strcpy(corpse,"The corpse of ");
		strcat(corpse,localContainerName);
		//for (i=0;i<strlen(corpse);i++)
		//	if (corpse[i]=='*')
		//		corpse[i]=' ';
		tb_DrawString(bufferBMP, 666-547-(tb_StringWidth(bufferBMP,corpse)/2), 70+(0*lineSpace)-18, corpse);
		line=1;
	}
	else line=0;

	gold=in_GetGoldTotal(pickUpInventory[0].itemID, pickUpInventory[0].itemCount);

	if (gold>0)
	{
		tb_DrawPictureToBitmap(bufferBMP, 132, 553-547, 70+(line*lineSpace)-10-18, &pickUpRect[0]);
		//	tb_OffsetRect(&pickUpRect[0],horizontalOffset,verticalOffset);

		tb_IntegerToString(gold,goldStr);

		if (gold>1)
			strcat(goldStr," Gold Pieces");
		else
			strcat(goldStr," Gold Piece");

		tb_DrawString(bufferBMP, 570-547, 70+(line*lineSpace)-18, goldStr);

		line++;
	}

	for (i=1;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&pickUpRect[i],	0, 0, 0, 0);

		if ((pickUpInventory[i].itemID!=0) && (pickUpInventory[i].itemID<60000))
		{

			in_GetItemName(pickUpInventory[i].itemID, itemName, pickUpInventory[i].itemCount);

			tb_DrawPictureToBitmap(bufferBMP, 132, 553-547, 70+(line*lineSpace)-10-18, &pickUpRect[i]);
			//		tb_OffsetRect(&pickUpRect[i],horizontalOffset,verticalOffset);

			tb_DrawString(bufferBMP, 570-547, 70+(line*lineSpace)-18, itemName);

			line++;

		}
	}

	if (pickUpInventory[1].itemID==60000)
		tb_DrawString(bufferBMP,610-547, 70+(0*lineSpace)-18, "Can't Open.  Locked.");

	if (pickUpInventory[1].itemID==60001)
		tb_DrawString(bufferBMP,610-518, 70+(0*lineSpace)-18, "Can't Take.");

	if (windowActive[kPackContentsWindow])
		tb_CopyBitmap(bufferBMP, tbPackContentsWindow, &r, &packContentsWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbPackContentsWindow, &r, &packContentsWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderMakeListWindow(int skill, int category)
// ----------------------------------------------------------------- //

{
	int						i;
	int						line			=	0;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	TRect					r;
	TRect					s;
	TRect					d;
	TBoolean      			rightCategory;
	TBoolean				canMake;

	if (windowVisible[kMakeWindow]==false)
		return;

	if (skill>=99)
		return;

	for (i=0;i<kMaxInventorySize;i++)
	{
		makeList[i].itemID=0;
		makeList[i].itemCount=0;
	}

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	if (skill==kBlacksmithing)							tb_DrawPictureToBitmap(bufferBMP, 204, 0, 0, &d);	// inventory title

	if (skill==kWoodworking)							  tb_DrawPictureToBitmap(bufferBMP, 206, 0, 0, &d);

	if (skill==kPoisoning)								tb_DrawPictureToBitmap(bufferBMP, 207, 0, 0, &d);

	if (skill==kEnchanting)								tb_DrawPictureToBitmap(bufferBMP, 261, 0, 0, &d);

	if (skill==kAlchemyCleric)							tb_DrawPictureToBitmap(bufferBMP, 245, 0, 0, &d);

	if (skill==kAlchemyDruid)							  tb_DrawPictureToBitmap(bufferBMP, 246, 0, 0, &d);

	if (skill==kTinkering)								  tb_DrawPictureToBitmap(bufferBMP, 208, 0, 0, &d);

	if (skill==kCooking)									  tb_DrawPictureToBitmap(bufferBMP, 209, 0, 0, &d);

	if (skill==kTailoringLeatherworking)	  tb_DrawPictureToBitmap(bufferBMP, 210, 0, 0, &d);

	if (skill==kMageryWizard)							  tb_DrawPictureToBitmap(bufferBMP, 215, 0, 0, &d);

	if (skill==kMageryCleric)							  tb_DrawPictureToBitmap(bufferBMP, 247, 0, 0, &d);

	if (skill==kMageryDruid)							  tb_DrawPictureToBitmap(bufferBMP, 248, 0, 0, &d);

	if (skill==kSpecialFighter)							  tb_DrawPictureToBitmap(bufferBMP, 191, 0, 0, &d);

	if (skill==kSpecialRanger)							  tb_DrawPictureToBitmap(bufferBMP, 191, 0, 0, &d);

	if (skill==kSpecialRogue)							  tb_DrawPictureToBitmap(bufferBMP, 191, 0, 0, &d);

	currentSkillDisplay	=	skill;

	currentCategory			=	category;

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	if (skill==kTailoringLeatherworking)
		if (category==0)
		{
			tb_SetFont(bufferBMP, kFontIDGeneva);	//018
			tb_SetFontSize(bufferBMP, 10);
			tb_SetFontStyle(bufferBMP, tb_Plain);

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(0*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(0*lineSpace)-18, "Shirts");
			makeCategory[1]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(1*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(1*lineSpace)-18, "Pants");
			makeCategory[2]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(2*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(2*lineSpace)-18, "Robes");
			makeCategory[3]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(3*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(3*lineSpace)-18, "Leather Armor");
			makeCategory[4]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(4*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(4*lineSpace)-18, "Wizard Hats");
			makeCategory[5]=true;
			//Dave
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(5*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(5*lineSpace)-18, "Jester Hats");
			makeCategory[6]=true;
			
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(6*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(6*lineSpace)-18, "Magic Robes");
			makeCategory[7]=true;
			
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(7*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(7*lineSpace)-18, "Miscellaneous");
			makeCategory[8]=true;

			tb_CopyBitmap(bufferBMP, tbMakeWindow, &r, &makeWindowRect, false);
			return;
		}
	
	if (skill==kEnchanting)
		if (category==0)
		{
			tb_SetFont(bufferBMP, kFontIDGeneva);	//018
			tb_SetFontSize(bufferBMP, 10);
			tb_SetFontStyle(bufferBMP, tb_Plain);

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(0*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(0*lineSpace)-18, "Etherite");
			makeCategory[1]=true;
			
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(1*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(1*lineSpace)-18, "Infusing");
			makeCategory[2]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(2*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(2*lineSpace)-18, "Rings");
			makeCategory[3]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(3*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(3*lineSpace)-18, "Charms");
			makeCategory[4]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(4*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(4*lineSpace)-18, "Miscellaneous");
			makeCategory[5]=true;
			//Dave
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(5*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(5*lineSpace)-18, "Orbs");
			makeCategory[6]=true;
			
			tb_CopyBitmap(bufferBMP, tbMakeWindow, &r, &makeWindowRect, false);
			return;
		}
		
	if (skill==kBlacksmithing)
		if (category==0)
		{
			tb_SetFont(bufferBMP, kFontIDGeneva);	//018
			tb_SetFontSize(bufferBMP, 10);
			tb_SetFontStyle(bufferBMP, tb_Plain);

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(0*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(0*lineSpace)-18, "Armor"); //Dave "Armors"
			makeCategory[1]=true;
			
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(1*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(1*lineSpace)-18, "Weapons");
			makeCategory[2]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(2*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(2*lineSpace)-18, "Magic Weapons");
			makeCategory[3]=true;

			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(3*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(3*lineSpace)-18, "Miscellaneous");
			makeCategory[4]=true;
			//Dave
			tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(4*lineSpace)-10-18, &s);
			tb_DrawString(bufferBMP, 567-547, 70+(4*lineSpace)-18, "Tools");
			makeCategory[5]=true;
			
			tb_CopyBitmap(bufferBMP, tbMakeWindow, &r, &makeWindowRect, false);
			return;
		}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	for (i=1;i<kMaxItems;i++)
	{
		canMake=false;
		rightCategory=false;

		if ((itemList[i].itemType==category) || (category==kAllCategories))
			rightCategory=true;

		if (category==kMiscTailorType)
			if ((itemList[i].itemType==kBootsType) || (itemList[i].itemType==kWaistType) || (itemList[i].itemType==kTailoringLeatherworking))
				rightCategory=true;
		//Dave	
		if (category==kToolType)
			if ((itemList[i].itemType==kLumberjackToolType) || (itemList[i].itemType==kMiningToolType) || (itemList[i].itemType==kBlacksmithToolType))
				rightCategory=true;
		//Dave^
		if (category==kAnyWeaponType)
			if (it_IsWeapon(i) && itemList[i].magical==false)
				rightCategory=true;
			
		if (category==kMagicWeaponType)
			if (it_IsWeapon(i) && itemList[i].magical)
				rightCategory=true;

		if (rightCategory)
		{
			if ((localRightFinger!=0 && itemList[localInventory[localRightFinger].itemID].spell==i) || (localLeftFinger!=0 && itemList[localInventory[localLeftFinger].itemID].spell==i))
				if (it_CanMake(i,skill,500000))
					canMake=true;

			if (!canMake)
				if (it_CanMake(i,skill,localSkills[skill].level))
					canMake=true;

			if (canMake)
			{
				if (line<kMaxInventorySize)
				{
					makeList[line+1].itemID=i;
					makeList[line+1].itemCount=1;
				}

				if (in_CanGroup(i))
					in_GetItemName(i, itemName, 0);
				else
				{
					if ((localSkills[skill].level>=(itemList[i].makeSkillLevel*2)) && (!((skill==kPoisoning) || (skill==kAlchemyCleric) || (skill==kAlchemyDruid))))
						in_GetItemName(i, itemName, (kMaxInventoryItemCount+1));
					else
						in_GetItemName(i, itemName, -1);
				}

				if (it_HaveIngredients(i) || (localRightFinger!=0 && itemList[localInventory[localRightFinger].itemID].spell==i) || (localLeftFinger!=0 && itemList[localInventory[localLeftFinger].itemID].spell==i))
				{
					tb_DrawPictureToBitmap(bufferBMP, 140, 553-547, 70+(line*lineSpace)-10-18, &s);

					if (!playerDead)
						tb_SetForeColor(bufferBMP, 0, 0, 0);
				}
				else
				{
					//enchanting, don't show until you have discovered the reagents
					//Dave Commented out
					//if (skill==kEnchanting) 
					//	continue;
					
					if (!playerDead)
						tb_SetForeColor(bufferBMP, 48960, 44115, 20910);
				}
					
					
				if (localSpat==i)	tb_SetForeColor(bufferBMP, 0, 0, 65535);

				tb_DrawString(bufferBMP, 567-547, 70+(line*lineSpace)-18, itemName);

				line++;
			}
		}
	}

	if (windowActive[kMakeWindow])
		tb_CopyBitmap(bufferBMP, tbMakeWindow, &r, &makeWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbMakeWindow, &r, &makeWindowRect, false);
}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderInventoryWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	//int						j;
	int						line			=	1;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	//char 					itemName1[kStringLength]; //[48];
	//char 					itemName2[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	UInt32				gold;
	TBoolean			equipped;
	//int						itemIDTemp;
	//int						itemCountTemp;
	//int						lookUpTemp;

	if (windowVisible[kInventoryWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 185, 0, 0, &d);	// inventory title

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	tb_DrawPictureToBitmap(bufferBMP, 135, 553-547, 70+(0*lineSpace)-10-18, &inventoryDropRect[0]);
	tb_OffsetRect(&inventoryDropRect[0],horizontalOffset,verticalOffset);

	tb_DrawPictureToBitmap(bufferBMP, 146, 567-547, 70+(0*lineSpace)-10-18, &inventoryGiveRect[0]);
	tb_OffsetRect(&inventoryGiveRect[0],horizontalOffset,verticalOffset);

	tb_DrawString(bufferBMP, 595-547, 70+(0*lineSpace)-18, "Gold Pieces:");
	gold=in_GetGoldTotal(localInventory[0].itemID, localInventory[0].itemCount);

	tb_DrawNumber(bufferBMP, 763-547, 70+(0*lineSpace)-18, gold, tb_Right);

	//------------------------------

	for (i=1;i<kMaxInventorySize;i++)
	{
		localSortedInventory[i].itemID		=	localInventory[i].itemID;
		localSortedInventory[i].itemCount	=	localInventory[i].itemCount;
		localInventoryLookup[i]=i;
	}

	//------------------------------

	for (i=1;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&inventoryDropRect[i],	0, 0, 0, 0);
		tb_SetRect(&inventoryGiveRect[i],	0, 0, 0, 0);

		if (localSortedInventory[i].itemID!=0)
		{

			in_GetItemName(localSortedInventory[i].itemID, itemName, localSortedInventory[i].itemCount);

			tb_DrawPictureToBitmap(bufferBMP, 135, 553-547, 70+(line*lineSpace)-10-18, &inventoryDropRect[i]);
			tb_OffsetRect(&inventoryDropRect[i],horizontalOffset,verticalOffset);

			tb_DrawPictureToBitmap(bufferBMP, 146, 567-547, 70+(line*lineSpace)-10-18, &inventoryGiveRect[i]);
			tb_OffsetRect(&inventoryGiveRect[i],horizontalOffset,verticalOffset);

			equipped=false;

			if (in_ItemIsUsable(localSortedInventory[i].itemID))
			{
				if (in_ItemEquiped(localInventoryLookup[i]))
				{
					tb_DrawPictureToBitmap(bufferBMP, gr_ActiveButtonOf(localInventoryLookup[i])+1, 581-547, 70+(line*lineSpace)-10-18, &inventoryUseRect[i]);
					equipped=true;
				}
				else
					tb_DrawPictureToBitmap(bufferBMP, gr_ActiveButtonOf(localInventoryLookup[i]), 581-547, 70+(line*lineSpace)-10-18, &inventoryUseRect[i]);

				tb_OffsetRect(&inventoryUseRect[i],horizontalOffset,verticalOffset);
			}

			if (!playerDead)
				if (equipped)
					tb_SetForeColor(bufferBMP, 65535, 0, 0);

			tb_DrawString(bufferBMP, 595-547, 70+(line*lineSpace)-18, itemName);

			if (!playerDead)
				tb_SetForeColor(bufferBMP, 0, 0, 0);

			line++;

		}
	}

	if (windowActive[kInventoryWindow])
		tb_CopyBitmap(bufferBMP, tbInventoryWindow, &r, &inventoryWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbInventoryWindow, &r, &inventoryWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
int gr_ActiveButtonOf(int i)
// ----------------------------------------------------------------- //

{

	if (it_IsWeapon(localInventory[i].itemID))
		return (144);

	if (itemList[localInventory[i].itemID].itemType==kPotionType)
		return (147);

	if (it_IsArmor(localInventory[i].itemID, false))
		return (149);

	if ((itemList[localInventory[i].itemID].itemType==kCookedMeatType) || (itemList[localInventory[i].itemID].itemType==kCookedVegType))
		return (173);

	if (itemList[localInventory[i].itemID].itemType==kBeverageType)
		return (173);
	
	//Dave
	if (itemList[localInventory[i].itemID].itemType==kBeverageType2)
		return (173);
	if (itemList[localInventory[i].itemID].itemType==kBeverageType3)
		return (173);
	if (itemList[localInventory[i].itemID].itemType==kBeverageType4)
		return (173);
	//Dave add elixers
	if (itemList[localInventory[i].itemID].itemType==kElixirType1)
		return (173);
	
	if (itemList[localInventory[i].itemID].itemType==kElixirType2)
		return (173);
	
	if (itemList[localInventory[i].itemID].itemType==kElixirType3)
		return (173);
	
	if (itemList[localInventory[i].itemID].itemType==kElixirType4)
		return (173);
	
	if (itemList[localInventory[i].itemID].itemType==kElixirType5)
		return (173);

	if (itemList[localInventory[i].itemID].itemType==kReagentType)
		return (178);

	if (it_IsClothing(localInventory[i].itemID))
		return (180);

	if ((itemList[localInventory[i].itemID].itemType==kRawMeatType) || (itemList[localInventory[i].itemID].itemType==kRawVegType))
		return (182);

	return (140); // default check

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_CheckMouseOnUpButton(TPoint pt)
// ----------------------------------------------------------------- //

{
	TPoint            mpt;
	TRect             r;
	unsigned long     waitTime;

	r=chatUpRect;

	if (r.left==0)
		return (false);

// tb_OffsetRect(&r,horizontalOffset,verticalOffset);

	if (!tb_PointInRect(pt, &r))
		return (false);

	waitTime=TickCount()+3;

	if (startChatLine>gFirstLine)
		startChatLine--;

	gr_RenderChatDisplayWindow(1);

	while (TickCount()<waitTime)
		NullEvent();

	while (tb_IsButtonDown())
	{
		tb_GetLocalMousePosition(tbChatWindow,&mpt);

		if (tb_PointInRect(mpt, &r))
		{
			if (startChatLine>gFirstLine)
				startChatLine--;

			gr_RenderChatDisplayWindow(1);
		}
		else
			gr_RenderChatDisplayWindow(0);

		r=chatUpRect;

		//tb_OffsetRect(&r,horizontalOffset,verticalOffset);
		if (r.left==0)
			break;

		NullEvent();

		//   SystemTask();
		nw_GetMessages();
	}

	gr_RenderChatDisplayWindow(0);

	return (true);

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_CheckMouseOnDownButton(TPoint pt)
// ----------------------------------------------------------------- //

{
	TPoint            mpt;
	TRect             r;
	unsigned long     waitTime;


	r=chatDownRect;

	if (r.left==0)
		return (false);

	if (!tb_PointInRect(pt, &r))
		return (false);

	waitTime=TickCount()+3;

	if (startChatLine<(kMaxChatLogLines-40))  // can scroll down
		startChatLine++;

	gr_RenderChatDisplayWindow(2);

	while (TickCount()<waitTime)
		NullEvent();

	while (tb_IsButtonDown())
	{
		tb_GetLocalMousePosition(tbChatWindow,&mpt);

		if (tb_PointInRect(mpt, &r))
		{
			if (startChatLine<(kMaxChatLogLines-40))  // can scroll down
				startChatLine++;

			gr_RenderChatDisplayWindow(2);
		}
		else
			gr_RenderChatDisplayWindow(0);

		r=chatDownRect;

		// tb_OffsetRect(&r,horizontalOffset,verticalOffset);
		if (r.left==0)
			break;

		NullEvent();

		//   SystemTask();
		nw_GetMessages();
	}

	gr_RenderChatDisplayWindow(0);

	return (true);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_DragThumb(TPoint pt)
// ----------------------------------------------------------------- //

{
	TPoint            mpt;
	TRect             r;
	int               vOff;
	//float             pos;
	float             vCurrent;

	r=chatThumbRect;

	if (r.left==0)
		return (false);

	//tb_OffsetRect(&r,horizontalOffset,verticalOffset);

	if (!tb_PointInRect(pt, &r))
		return (false);

	tb_SetPort(tbChatWindow);

	tb_GetMousePosition(&mpt);

	while ((mpt.h==pt.h) && (mpt.v==pt.v))
	{
		gr_RenderChatDisplayWindow(3);
		NullEvent();
		tb_GetMousePosition(&mpt);
	}

	vOff      = pt.v-r.top;

	vCurrent  = pt.v-vOff-(74-18);

	if (vCurrent<0)     vCurrent=0;

	if (vCurrent>470)   vCurrent=470;

	startChatLine=gFirstLine+((vCurrent/470)*lastLine);

	if (startChatLine<0)
		startChatLine=0;

	if (startChatLine>(kMaxChatLogLines-40))
		startChatLine=kMaxChatLogLines-40;

	gr_RenderChatDisplayWindow(3);

	while (tb_IsButtonDown())
	{
		tb_SetPort(tbChatWindow);
		tb_GetMousePosition(&mpt);
		vCurrent  = mpt.v-vOff-(74-18);

		if (vCurrent<0)     vCurrent=0;

		if (vCurrent>470)   vCurrent=470;

		startChatLine=gFirstLine+((vCurrent/470)*lastLine);

		if (startChatLine<0)
			startChatLine=0;

		if (startChatLine>(kMaxChatLogLines-40))
			startChatLine=kMaxChatLogLines-40;

		gr_RenderChatDisplayWindow(3);

		NullEvent();

//      SystemTask();
		nw_GetMessages();
	}

	gr_RenderChatDisplayWindow(0);

	return (true);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_CheckMouseOnPageUpButton(TPoint pt)
// ----------------------------------------------------------------- //

{
	TPoint            mpt;
	TRect             r;
	unsigned long     waitTime;

	r=chatPageUpRect;

	if (r.left==0)
		return (false);

	//tb_OffsetRect(&r,horizontalOffset,verticalOffset);

	if (!tb_PointInRect(pt, &r))
		return (false);

	waitTime=TickCount()+3;

	if (startChatLine>gFirstLine)
		startChatLine=startChatLine-40;

	if (startChatLine<gFirstLine)
		startChatLine=gFirstLine;

	gr_RenderChatDisplayWindow(0);

	while (TickCount()<waitTime)
		NullEvent();

	r=chatPageUpRect;

	if (r.left==0)
		return (false);

	while (tb_IsButtonDown())
	{
		tb_GetLocalMousePosition(tbChatWindow,&mpt);

		if (tb_PointInRect(mpt, &r))
		{
			if (startChatLine>gFirstLine)
				startChatLine=startChatLine-40;

			if (startChatLine<gFirstLine)
				startChatLine=gFirstLine;

			gr_RenderChatDisplayWindow(0);
		}
		else
			gr_RenderChatDisplayWindow(0);

		r=chatPageUpRect;

		// tb_OffsetRect(&r,horizontalOffset,verticalOffset);
		if (r.left==0)
			break;

		NullEvent();

//      SystemTask();
		nw_GetMessages();
	}

	gr_RenderChatDisplayWindow(0);

	return (true);

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_CheckMouseOnPageDownButton(TPoint pt)
// ----------------------------------------------------------------- //

{
	TPoint            mpt;
	TRect             r;
	unsigned long     waitTime;


	r=chatPageDownRect;

	if (r.left==0)
		return (false);

	//tb_OffsetRect(&r,horizontalOffset,verticalOffset);

	if (!tb_PointInRect(pt, &r))
		return (false);

	waitTime=TickCount()+3;

	if (startChatLine<(kMaxChatLogLines-40))  // can scroll down
		startChatLine=startChatLine+40;

	if (startChatLine>(kMaxChatLogLines-40))
		startChatLine=kMaxChatLogLines-40;

	gr_RenderChatDisplayWindow(0);

	while (TickCount()<waitTime)
		NullEvent();

	r=chatPageDownRect;

	if (r.left==0)
		return (false);

	while (tb_IsButtonDown())
	{
		tb_GetLocalMousePosition(tbChatWindow,&mpt);

		if (tb_PointInRect(mpt, &r))
		{
			if (startChatLine<(kMaxChatLogLines-40))  // can scroll down
				startChatLine=startChatLine+40;

			if (startChatLine>(kMaxChatLogLines-40))
				startChatLine=kMaxChatLogLines-40;

			gr_RenderChatDisplayWindow(0);
		}
		else
			gr_RenderChatDisplayWindow(0);

		r=chatPageDownRect;

		//tb_OffsetRect(&r,horizontalOffset,verticalOffset);
		if (r.left==0)
			break;

		NullEvent();

//      SystemTask();
		nw_GetMessages();
	}

	gr_RenderChatDisplayWindow(0);

	return (true);

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_BuildHuman(int i, TBoolean onRaft, TBoolean maskOnly)
// ----------------------------------------------------------------- //

{
	int									frame;
	TRect								r;
	int                					sexBMP;
	int                 				sexMaskBMP;

	//char debug[kStringLength];



	if (human[i][13]==0)
	{
		if (i==90)
		{
			if ((xOffset==0) && (yOffset==0))
				frame=0;
			else
				frame=1;

			//if ((stepFrame==1) || (stepFrame==3)) // small step test
			//  frame=0;
			//else
			//  frame=1;

		}
		else
		{
			if (creatureDirection[i]==4)
				frame=0;
			else
				frame=1;
		}
	}
	else if (human[i][13]==1)
		frame=0;
	else
	{
		if (creatureHealth[i]==0)
			frame=0;
		else
			frame=2;
	}

	if (onRaft)
		if (frame==1)
			frame=0;

	r=humanBuilderRect;

	r.top=r.top+5;

	r.bottom=r.bottom-25;

	if (maskOnly==false)
	{
		tb_EraseRect(humanBuilderBMP,						&humanBuilderRect);
		tb_EraseRect(humanBuilderWorkSpaceBMP,	&humanBuilderRect);
	}

	tb_EraseRect(humanBuilderMaskBMP,				&humanBuilderRect);

	if (human[i][15]==0)
	{
		sexBMP      = manGridBMP;
		sexMaskBMP  = manGridMaskBMP;

		if (creatureHealth[i]==0)
		{
			if (maskOnly==false)
				tb_CopyBitmap(creatureGridBMP[4], humanBuilderBMP, &humanMaleGhost[creatureFacing[i]][frame], &r, false);

			tb_CopyBitmap(creatureGridMaskBMP[4], humanBuilderMaskBMP, &humanMaleGhost[creatureFacing[i]][frame], &r, false);

			return;
		}
		else
		{
			if (maskOnly==false)
				tb_CopyBitmap(sexBMP, humanBuilderBMP, &baseHuman[creatureFacing[i]][frame], &r, false);

			tb_CopyBitmap(sexMaskBMP, humanBuilderMaskBMP, &baseHuman[creatureFacing[i]][frame], &r, false);
		}
	}
	else
	{
		sexBMP      = womanGridBMP;
		sexMaskBMP  = womanGridMaskBMP;

		if (creatureHealth[i]==0)
		{
			if (maskOnly==false)
				tb_CopyBitmap(creatureGridBMP[4], humanBuilderBMP, &humanFemaleGhost[creatureFacing[i]][frame], &r, false);

			tb_CopyBitmap(creatureGridMaskBMP[4], humanBuilderMaskBMP, &humanFemaleGhost[creatureFacing[i]][frame], &r, false);

			return;
		}
		else
		{
			if (maskOnly==false)
			tb_CopyBitmap(sexBMP, humanBuilderBMP, &baseHuman[creatureFacing[i]][frame], &r, false);

		tb_CopyBitmap(sexMaskBMP, humanBuilderMaskBMP, &baseHuman[creatureFacing[i]][frame], &r, false);	
		}
	}

	// -- feet -------------------------------------------------------

	if ((itemList[human[i][kFeet]].itemType==kBootsType) || (itemList[human[i][kFeet]].itemType==kMountType))
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanBoots[creatureFacing[i]][frame], &r, false);

			if (itemList[human[i][kFeet]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kFeet]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanBoots[creatureFacing[i]][frame], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBoots[creatureFacing[i]][frame], &humanBoots[creatureFacing[i]][frame], &r);
	}

	// -- torso -------------------------------------------------------

	if (itemList[human[i][kTorso]].itemType==kRoyalRobeType)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(sexBMP, sexMaskBMP, humanBuilderBMP, &humanRoyalRobe[creatureFacing[i]][frame], &humanRoyalRobe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanRoyalRobe[creatureFacing[i]][frame], &humanRoyalRobe[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kTorso]].itemType==kGMRobeType)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(sexBMP, sexMaskBMP, humanBuilderBMP, &humanGMRobe[creatureFacing[i]][frame], &humanGMRobe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanGMRobe[creatureFacing[i]][frame], &humanGMRobe[creatureFacing[i]][frame], &r);
	}
	else 	if ((itemList[human[i][kTorso]].itemType==kRobeType) || (itemList[human[i][kTorso]].itemType==kMagicRobeType))
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(sexBMP, humanBuilderWorkSpaceBMP, &humanRobe[creatureFacing[i]][frame], &r, false);

			if (itemList[human[i][kTorso]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kTorso]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, sexMaskBMP, humanBuilderBMP, &r, &humanRobe[creatureFacing[i]][frame], &r);
		}

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanRobe[creatureFacing[i]][frame], &humanRobe[creatureFacing[i]][frame], &r);
	}
	else 	if ((itemList[human[i][kTorso]].itemType==kWhiteRobeType))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(sexBMP, sexMaskBMP, humanBuilderBMP, &humanWhiteRobe[creatureFacing[i]][frame], &humanWhiteRobe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanWhiteRobe[creatureFacing[i]][frame], &humanWhiteRobe[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kTorso]].itemType==kShirtType)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(sexBMP, humanBuilderWorkSpaceBMP, &humanShirt[creatureFacing[i]], &r, false);

			if (itemList[human[i][kTorso]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kTorso]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, sexMaskBMP, humanBuilderBMP, &r, &humanShirt[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanShirt[creatureFacing[i]], &humanShirt[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kTorso]].parent==kBlackBreastPlate)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanBlackBreastPlate[creatureFacing[i]], &humanBlackBreastPlate[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBlackBreastPlate[creatureFacing[i]], &humanBlackBreastPlate[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kTorso]].parent==kBreastChain)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanBreastChain[creatureFacing[i]], &r, false);

			if (itemList[human[i][kTorso]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kTorso]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanBreastChain[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBreastChain[creatureFacing[i]], &humanBreastChain[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kTorso]].parent==kBreastPlate || itemList[human[i][kTorso]].parent==kBurningPlateBreast || itemList[human[i][kTorso]].parent==kBluePlateBreast) //Dave || itemList[human[i][kTorso]].parent==kBluePlateBreast
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanBreastPlate[creatureFacing[i]], &r, false);

			if (itemList[human[i][kTorso]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kTorso]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanBreastPlate[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBreastPlate[creatureFacing[i]], &humanBreastPlate[creatureFacing[i]], &r);
	}

	else if (itemList[human[i][kTorso]].parent==kBrownBreastLeather)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanBrownBreastLeather[creatureFacing[i]], &humanBrownBreastLeather[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBrownBreastLeather[creatureFacing[i]], &humanBrownBreastLeather[creatureFacing[i]], &r);
	}

	//else if (itemList[human[i][kTorso]].itemType==kLeatherBreastType)
	//	{
	//		tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanBreastLeather[creatureFacing[i]], &r, false);
	//		if (itemList[human[i][kTorso]].itemColor!=kGray)
	//			tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kTorso]].itemColor);
	//		tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanBreastLeather[creatureFacing[i]], &r);
	//		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBreastLeather[creatureFacing[i]], &humanShirt[creatureFacing[i]], &r);
	//	}

	// -- legs -------------------------------------------------------

	if (itemList[human[i][kLegs]].itemType==kPantsType)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(sexBMP, humanBuilderWorkSpaceBMP, &humanPants[creatureFacing[i]][frame], &r, false);

			if (itemList[human[i][kLegs]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kLegs]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, sexMaskBMP, humanBuilderBMP, &r, &humanPants[creatureFacing[i]][frame], &r);
		}

		tb_CopyMaskBitmap(sexMaskBMP, sexMaskBMP, humanBuilderMaskBMP, &humanPants[creatureFacing[i]][frame], &humanPants[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kLegs]].parent==kLegsPlate || itemList[human[i][kLegs]].parent==kBurningPlateLegs)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanLegsPlate[creatureFacing[i]][frame], &r, false);

			if (itemList[human[i][kLegs]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kLegs]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanLegsPlate[creatureFacing[i]][frame], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanLegsPlate[creatureFacing[i]][frame], &humanLegsPlate[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kLegs]].parent==kBlackLegsPlate)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanBlackLegsPlate[creatureFacing[i]][frame], &humanBlackLegsPlate[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBlackLegsPlate[creatureFacing[i]][frame], &humanBlackLegsPlate[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kLegs]].parent==kChainLegs)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanLegsChain[creatureFacing[i]][frame], &humanLegsChain[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanLegsChain[creatureFacing[i]][frame], &humanLegsChain[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kLegs]].parent==kBrownLeatherLegs)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanLegsLeather[creatureFacing[i]][frame], &humanLegsLeather[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanLegsLeather[creatureFacing[i]][frame], &humanLegsLeather[creatureFacing[i]][frame], &r);
	}

	// ----- arms -----

	if ((human[i][kArms]==0) && (!((itemList[human[i][kTorso]].itemType==kMagicRobeType) || (itemList[human[i][kTorso]].itemType==kRobeType) || (itemList[human[i][kTorso]].itemType==kGMRobeType) || (itemList[human[i][kTorso]].itemType==kRoyalRobeType))))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanArms[creatureFacing[i]][frame], &humanArms[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanArms[creatureFacing[i]][frame], &humanArms[creatureFacing[i]][frame], &r);
	}
	else if ((itemList[human[i][kArms]].parent==kBurningPlateArms) || (itemList[human[i][kArms]].parent==kPlateArms) || (human[i][kArms]==kChainArms))
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanGrayArms[creatureFacing[i]][frame], &r, false);

			if (itemList[human[i][kArms]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kArms]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanGrayArms[creatureFacing[i]][frame], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanGrayArms[creatureFacing[i]][frame], &humanGrayArms[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kArms]].parent==kBlackPlateArms)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanBlackArms[creatureFacing[i]][frame], &humanBlackArms[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBlackArms[creatureFacing[i]][frame], &humanBlackArms[creatureFacing[i]][frame], &r);
	}
	else if (itemList[human[i][kArms]].parent==kBrownLeatherArms)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanBrownArms[creatureFacing[i]][frame], &humanBrownArms[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanBrownArms[creatureFacing[i]][frame], &humanBrownArms[creatureFacing[i]][frame], &r);
	}

	// -- head --------------------------------------------------------

	if (itemList[human[i][kHead]].parent==kOpenPlateHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanOpenPlateHelmet[creatureFacing[i]], &humanOpenPlateHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanOpenPlateHelmet[creatureFacing[i]], &humanOpenPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kSlottedPlateHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanSlottedPlateHelmet[creatureFacing[i]], &humanSlottedPlateHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanSlottedPlateHelmet[creatureFacing[i]], &humanSlottedPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kClosedPlateHelmet || itemList[human[i][kHead]].parent==kBurningPlateHelmet)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanClosedPlateHelmet[creatureFacing[i]], &r, false);

			if (itemList[human[i][kHead]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kHead]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanClosedPlateHelmet[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanClosedPlateHelmet[creatureFacing[i]], &humanClosedPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kVikingHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanVikingHelmet[creatureFacing[i]], &humanVikingHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanVikingHelmet[creatureFacing[i]], &humanVikingHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kChainMailHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanChainMailHelmet[creatureFacing[i]], &humanChainMailHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanChainMailHelmet[creatureFacing[i]], &humanChainMailHelmet[creatureFacing[i]], &r);
	}

	else if (itemList[human[i][kHead]].parent==kBlackOpenPlateHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanBlackOpenPlateHelmet[creatureFacing[i]], &humanBlackOpenPlateHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanBlackOpenPlateHelmet[creatureFacing[i]], &humanBlackOpenPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kBlackSlottedPlateHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanBlackSlottedPlateHelmet[creatureFacing[i]], &humanBlackSlottedPlateHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanBlackSlottedPlateHelmet[creatureFacing[i]], &humanBlackSlottedPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kBlackClosedPlateHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanBlackClosedPlateHelmet[creatureFacing[i]], &humanBlackClosedPlateHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanBlackClosedPlateHelmet[creatureFacing[i]], &humanBlackClosedPlateHelmet[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].parent==kBlackVikingHelmet)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanBlackVikingHelmet[creatureFacing[i]], &humanBlackVikingHelmet[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanBlackVikingHelmet[creatureFacing[i]], &humanBlackVikingHelmet[creatureFacing[i]], &r);
	}
	else if ((itemList[human[i][kHead]].parent==(kGoldCrown)) || (itemList[human[i][kHead]].parent==(kFirstCrown)) || (itemList[human[i][kHead]].parent==(kSecondCrown)) || (itemList[human[i][kHead]].parent==(kThirdCrown)))
	{
	  if (human[i][16]!=0)
	  	{
	  		if (human[i][15]==0)
			{
				if (maskOnly==false)
				{
				tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &r, false);
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
				tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
				}

			tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
			}
			else
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
			}
		}
		if (maskOnly==false)
			tb_CopyMaskBitmap(headGridBMP, headGridMaskBMP, humanBuilderBMP, &humanCrown[creatureFacing[i]], &humanCrown[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanCrown[creatureFacing[i]], &humanCrown[creatureFacing[i]], &r);
	}
	
	else if (itemList[human[i][kHead]].itemType==kWizardHatType)
	{
		if (human[i][16]!=0)
		{
			if (human[i][15]==0)
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
			}
			else
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
			}
		}

		if (maskOnly==false)
		{
			tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanWizardHat[creatureFacing[i]], &r, false);

			if (itemList[human[i][kHead]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kHead]].itemColor,&r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanWizardHat[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanWizardHat[creatureFacing[i]], &humanWizardHat[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kHead]].itemType==kJesterHatType)
	{
		if (human[i][16]!=0)
		{
			if (human[i][15]==0)
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
			}
			else
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
			}
		}

		if (maskOnly==false)
		{
			tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanJesterHat[creatureFacing[i]], &r, false);

			if (itemList[human[i][kHead]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kHead]].itemColor,&r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanJesterHat[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanJesterHat[creatureFacing[i]], &humanJesterHat[creatureFacing[i]], &r);
	}
	else  //nothing
	{
		if (human[i][16]!=0)
		{
			if (human[i][15]==0)
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanMaleHair[creatureFacing[i]][human[i][16]], &humanMaleHair[creatureFacing[i]][human[i][16]], &r);
			}
			else
			{
				if (maskOnly==false)
				{
					tb_CopyBitmap(headGridBMP, humanBuilderWorkSpaceBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r, false);
					tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,human[i][17],&r);
					tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, headGridMaskBMP, humanBuilderBMP, &r, &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
				}

				tb_CopyMaskBitmap(headGridMaskBMP, headGridMaskBMP, humanBuilderMaskBMP, &humanFemaleHair[creatureFacing[i]][human[i][16]], &humanFemaleHair[creatureFacing[i]][human[i][16]], &r);
			}
		}
	}

	// --------- armed ----------

	if (frame==1) frame=0;

	if (frame==2) frame=1;

	// --------- left hand ---------

	if (itemList[human[i][kLeftHand]].parent==kMedalShield || itemList[human[i][kLeftHand]].parent==kBlackMetalShield || itemList[human[i][kLeftHand]].parent==kBurningMetalShield)
	{
		if (maskOnly==false)
		{
			tb_CopyBitmap(armorGridBMP, humanBuilderWorkSpaceBMP, &humanMedalShield[creatureFacing[i]], &r, false);

			if (itemList[human[i][kLeftHand]].itemColor!=kGray)
				tb_ColorizeWorkSpace(humanBuilderWorkSpaceBMP,itemList[human[i][kLeftHand]].itemColor, &r);

			tb_CopyMaskBitmap(humanBuilderWorkSpaceBMP, armorGridMaskBMP, humanBuilderBMP, &r, &humanMedalShield[creatureFacing[i]], &r);
		}

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanMedalShield[creatureFacing[i]], &humanMedalShield[creatureFacing[i]], &r);
	}
	else if (itemList[human[i][kLeftHand]].parent==kWoodenShield)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanWoodenShield[creatureFacing[i]], &humanWoodenShield[creatureFacing[i]], &r);

		tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanWoodenShield[creatureFacing[i]], &humanWoodenShield[creatureFacing[i]], &r);
	}

	// --------- right hand ---------

	if ((itemList[human[i][kRightHand]].parent==kShortBow) || (itemList[human[i][kRightHand]].parent==kLongBow))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanBow[creatureFacing[i]][frame], &humanBow[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanBow[creatureFacing[i]][frame], &humanBow[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kSpear)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanSpear[creatureFacing[i]][frame], &humanSpear[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanSpear[creatureFacing[i]][frame], &humanSpear[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kBroadSword) || (itemList[human[i][kRightHand]].parent==kScimitar) || (itemList[human[i][kRightHand]].parent==kKnife)  || (itemList[human[i][kRightHand]].parent==kHolyDagger) || (itemList[human[i][kRightHand]].parent==kCarvingKnife) || (itemList[human[i][kRightHand]].parent==kBloodDagger))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanShortSword[creatureFacing[i]][frame], &humanShortSword[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanShortSword[creatureFacing[i]][frame], &humanShortSword[creatureFacing[i]][frame], &r);
	}

	if (itemList[human[i][kRightHand]].parent==kGlaive)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanGlaive[creatureFacing[i]][frame], &humanGlaive[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanGlaive[creatureFacing[i]][frame], &humanGlaive[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kClub)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanClub[creatureFacing[i]][frame], &humanClub[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanClub[creatureFacing[i]][frame], &humanClub[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kMace) || (itemList[human[i][kRightHand]].parent==kWarHammer) || (itemList[human[i][kRightHand]].parent==kBlacksmithHammer))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanMace[creatureFacing[i]][frame], &humanMace[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanMace[creatureFacing[i]][frame], &humanMace[creatureFacing[i]][frame], &r);
	}
	
/*	*///Dave 11-15-2012
	else if (itemList[human[i][kRightHand]].parent==kRoyalSceptre)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanSpect[creatureFacing[i]][frame], &humanSpect[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanSpect[creatureFacing[i]][frame], &humanSpect[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kFlail)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanFlail[creatureFacing[i]][frame], &humanFlail[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanFlail[creatureFacing[i]][frame], &humanFlail[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kBattleAxe) || (itemList[human[i][kRightHand]].parent==kFrostAxe))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanBattleAxe[creatureFacing[i]][frame], &humanBattleAxe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanBattleAxe[creatureFacing[i]][frame], &humanBattleAxe[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kFishingPole) || (itemList[human[i][kRightHand]].parent==kLobsterPole) || (itemList[human[i][kRightHand]].parent==kEelPole))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanFishingPole[creatureFacing[i]][frame], &humanFishingPole[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanFishingPole[creatureFacing[i]][frame], &humanFishingPole[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kMiningAxe) || (itemList[human[i][kRightHand]].parent==kCoalAxe) || (itemList[human[i][kRightHand]].parent==kCentaurMiningAxe))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanPickAxe[creatureFacing[i]][frame], &humanPickAxe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanPickAxe[creatureFacing[i]][frame], &humanPickAxe[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kSickle)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanSickle[creatureFacing[i]][frame], &humanSickle[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanSickle[creatureFacing[i]][frame], &humanSickle[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kHandAxe) || (itemList[human[i][kRightHand]].parent==kDwarvenHandAxe) || (itemList[human[i][kRightHand]].parent==kAmberAxe))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanSmallAxe[creatureFacing[i]][frame], &humanSmallAxe[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanSmallAxe[creatureFacing[i]][frame], &humanSmallAxe[creatureFacing[i]][frame], &r);
	}

	else if ((itemList[human[i][kRightHand]].parent==kLongSword) || (itemList[human[i][kRightHand]].parent==kKatana) ||  (itemList[human[i][kRightHand]].parent==kBigJackKatana) ||  (itemList[human[i][kRightHand]].parent==kRuneSword))
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanSword[creatureFacing[i]][frame], &humanSword[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanSword[creatureFacing[i]][frame], &humanSword[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kFireSword)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanFireSword[creatureFacing[i]][frame], &humanFireSword[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanFireSword[creatureFacing[i]][frame], &humanFireSword[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kNeptunesTrident)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanTrident[creatureFacing[i]][frame], &humanTrident[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanTrident[creatureFacing[i]][frame], &humanTrident[creatureFacing[i]][frame], &r);
	}

	else if (itemList[human[i][kRightHand]].parent==kStave)
	{
		if (maskOnly==false)
			tb_CopyMaskBitmap(weaponGridBMP, weaponGridMaskBMP, humanBuilderBMP, &humanStaff[creatureFacing[i]][frame], &humanStaff[creatureFacing[i]][frame], &r);

		tb_CopyMaskBitmap(weaponGridMaskBMP, weaponGridMaskBMP, humanBuilderMaskBMP, &humanStaff[creatureFacing[i]][frame], &humanStaff[creatureFacing[i]][frame], &r);
	}

	// --------- left hand ---------

//	if (human[i][kLeftHand]==kMedalShield)
//		{
//			tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanBuilderBMP, &humanMedalShield[creatureFacing[i]], &humanMedalShield[creatureFacing[i]], &r);
//			tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanBuilderMaskBMP, &humanMedalShield[creatureFacing[i]], &humanMedalShield[creatureFacing[i]], &r);
//		}

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_DrawButtonPicture(int bmp, int id, int x, int y, int button, int state, TBoolean drawOnScreen)
// ----------------------------------------------------------------- //

{
	TRect					r;

	if (displayAreaButtonCurrent[button]!=state)
	{
		tb_DrawPictureToBitmap(bmp, id, x, y, &displayAreaButton[button]);
		r=displayAreaButton[button];

		if (bmp==osBMP)
			tb_OffsetRect(&displayAreaButton[button],horizontalOffset,verticalOffset);

		if (drawOnScreen)
			tb_CopyBitmap(osBMP, tbWindow, &r, &displayAreaButton[button],false);
	}

	displayAreaButtonState		[button]=state;

	displayAreaButtonCurrent	[button]=state;

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderMapWindow(TBoolean /*drawOnScreen*/)
// ----------------------------------------------------------------- //

{

	int							x;
	int							y;
	int							startRow	=	playerRow-24;//25;
	int							startCol	=	playerCol-24;//28;
	int							id;
	int							dx=0;
	int							dy=75;
	int							startDy=dy;
	int							startDx=dx;
	TRect						r;
	int							red;
	int							green;
	int							blue;
	//TRect           			currentRect;
	int							offRow, offCol;
	unsigned long				flash, currentTime=TickCount();
	TBoolean					doFlash=false;

	//char test[128];

	if (windowVisible[kMapWindow]==false)
		return;

#ifndef _DEBUGGING
	if (((playerRow>1842 && playerCol>40) || (playerCol>1842)) || (!gotFirstRefresh))
	{
		tb_SetForeColor(mapViewBMP,0,0,0);
		tb_PaintRect(mapViewBMP,&mapViewRect);
	}
	else
#endif
	{
		for (x=74;x>=0;x--)
		{
			for (y=0;y<75;y++)
			{
				if ((startRow+x>=1986) || (startRow+x<0) || (startCol+y>=kMapHeight) || (startCol+y<0))
				{
					id		= 0;
					red		=	0;
					green	=	0;
					blue	=	0;
				}
				else if (((startRow+x>1846 && startRow+x<1986) && startCol+y>12) || (startRow>1846 && startRow+x<1964 && startCol+y<=40) || (startRow+x<13) || (startCol+y>1846) || (startCol+y<13)) //Dave (((startRow+x>1846 && startRow+x<1986) && startCol+y>40)
				{
					id=0;
					red=0;
					green=0;
					blue=0;
				}
				else
				{
					id=clientMap[startRow+x][startCol+y];	// client map
					red		=	0;
					green	=	65535;
					blue	=	0;
				}

				if (((id>=1110) && (id<=1141))	|| ((id>=1145) && (id<=1177)))// lava
				{
					red		=	65535;
					green	=	0;
					blue	=	0;
				}
				else if ((id>=359) && (id<=373))	// trees
				{
					red		=	0;
					green	=	32768;
					blue	=	0;
				}
				else if (id==535)	// ether
				{
					red		=	0;
					green	=	0;
					blue	=	0;
				}
				else if (id==503 || id==533 || (id>395 && id<400))	// ice
				{
					red		=	65535;
					green	=	65535;
					blue	=	65535;
				}
				else if ((mapColor[id]==kMapRoad) || (id==401) || ((id>=403) && (id<=438)))	// road
				{
					red		=	39321;
					green	=	26214;
					blue	=	0;
				}
				else if (id==402)	// sand
				{
					red		=	52428;
					green	=	52428;
					blue	=	39321;
				}
				else if (id==275)	// water
				{
					red		=	26214;
					green	=	52428;
					blue	=	52428;
				}
				else if ((id>=536) && (id<=544))	// rock
				{
					red		=	32768;
					green	=	32768;
					blue	=	32768;
				}
				else if (((id>=553) && (id<=602)) || (id==608) || (id==719))	// structure
				{
					red		=	17476;
					green	=	17476;
					blue	=	17476;
				}

				//------------------------------------------
				else if (mapColor[id]==kMapStructure)
				{
					red		=	17476;
					green	=	17476;
					blue	=	17476;
				}

				//------------------------------------------
				//------------------------------------------
				else if (mapColor[id]==kFlagStructure)
				{
					red		=	65535;
					green	=	0;
					blue	=	0;
				}

				//------------------------------------------

				if (((startRow+x)==(playerRow+13)) && ((startCol+y)==(playerCol+13)))	// player
				{
					red		=	65535;
					green	=	0;
					blue	=	0;
				}

				if ((red==0) && (green==65535) && (blue==0))
					if (gr_SnowAt(startRow+x, startCol+y))
					{
						red		=	65535;
						green	=	65535;
						blue	=	65535;
					}

				tb_SetPixelRGB(mapViewBMP, dx, dy, red, green, blue);

				dx=dx+1;
				dy=dy-1;

			}

			startDy=startDy+1;

			dy=startDy;
			startDx=startDx+1;
			dx=startDx;
		}
	}

	r=mapViewRect;

	tb_OffsetRect(&r,-1,-9);

	tb_CopyMaskBitmap(mapViewBMP, mapViewMaskBMP, mapWindowBMP, &mapViewRect, &mapViewRect, &r);
	tb_CopyMaskBitmap(mapCenterBMP, mapCenterMaskBMP, mapWindowBMP, &mapCenterRect, &mapCenterRect, &mapCenterDestRect);

	//-----------Tracking

	if (currentlyTracking>currentTime)
	{
		flash=currentlyTracking-currentTime;

		if (flash<24)
			doFlash=true;
		else if (flash>48 && flash<72)
			doFlash=true;
		else if (flash>96)
			doFlash=true;
	}
	else
		currentlyTracking=0;

	if (doFlash)
	{
		for (id=0; id<monstersTracked; id++)
		{
			offRow=localTracking[id][0];

			if (offRow>100)
				offRow=-(offRow-100);


			offRow+=(playerRow-trackingRow);

			offCol=localTracking[id][1];

			if (offCol>100)
				offCol=-(offCol-100);

			offCol+=(playerCol-trackingCol);

			if (offRow>27 || offCol>27 || offRow<-27 || offCol<-27)
				continue;

			x=y=0;

			x+=offRow;

			y+=offRow;

			x-=offCol;

			y+=offCol;

			r=mapCenterRect;

			tb_OffsetRect(&r,71,64);

			tb_OffsetRect(&r,x,y);

			tb_CopyMaskBitmap(mapTrackingBMP[localTracking[id][2]], mapTrackingMaskBMP[(localTracking[id][2])/4], mapWindowBMP, &mapCenterRect, &mapCenterRect, &r);
		}

	}



	//-------------


	if (windowActive[kMapWindow])
		tb_CopyBitmap(mapWindowBMP, tbMapWindow, &mapWindowRect, &mapWindowRect, false);
	else
		tb_CopyBitmapGray(mapWindowBMP, tbMapWindow, &mapWindowRect, &mapWindowRect, false);


}

#endif

// ----------------------------------------------------------------- //
void gr_GetCreatureRect(int i, TRect	*r)
// ----------------------------------------------------------------- //

{
	TRect						s;
	TBoolean        morphing=false;

	tb_SetRect(&s,0,0,0,0);
	tb_SetRect(r,0,0,0,0);

	//---- for morph 8/30/01

	if (state[i]!=kHuman)
		if (state[i]!=kNPCHuman && state[i]!=kNPCHumanFemale)
			if (i==90)
				if (!((xOffset==0) && (yOffset==0)))
				{
					morphing=true;
					// creatureDirection[i]=0;
					// tb_Beep();
				}

	//----------------------

	if (creatureInfo[state[i]].graphicsIndex==0)
		s=srcRect[state[i]];

	else if (state[i]==kSeaTurtle)
	{
		if (creatureDirection[i]==4)
		{
			if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// turtle is surfaced
				s=creatureRect[creatureInfo[state[i]].graphicsIndex][(creatureFacing[i]-10)][2];
			else
				s=creatureRect[creatureInfo[state[i]].graphicsIndex][creatureFacing[i]][0];
		}
		else
		{
			if (creatureDirection[i]==4)
				s=creatureRect[creatureInfo[state[i]].graphicsIndex][creatureFacing[i]][0];
			else
				s=creatureRect[creatureInfo[state[i]].graphicsIndex][creatureFacing[i]][1];
		}
	}

	else if ((state[i]==kLizardMan) || (state[i]==kLizardManShaman))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=lizard[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=lizard[creatureFacing[i]][0];
		else
			s=lizard[creatureFacing[i]][1];
	}

	else if (state[i]==kSkeleton)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=skeleton[(creatureFacing[i]-10)][2];
		else if ((creatureDirection[i]==4) && (morphing==false))
			s=skeleton[creatureFacing[i]][0];
		else
			s=skeleton[creatureFacing[i]][1];
	}
	
	
	else if (state[i]==kSkeletonArcher)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=skeletonarcher[(creatureFacing[i]-10)][2];
		else if ((creatureDirection[i]==4) && (morphing==false))
			s=skeletonarcher[creatureFacing[i]][0];
		else
			s=skeletonarcher[creatureFacing[i]][1];
	}


	else if ((state[i]==kBoneMage) || (state[i]==kHellMage))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=boneMage[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=boneMage[creatureFacing[i]][0];
		else
			s=boneMage[creatureFacing[i]][1];
	}

	else if (state[i]==kScorpion)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=scorpion[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=scorpion[creatureFacing[i]][0];
		else
			s=scorpion[creatureFacing[i]][1];
	}

	else if (state[i]==kHarpie)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=harpie[(creatureFacing[i]-10)][animationFrame2];
		else if (creatureDirection[i]==4)
			s=harpie[creatureFacing[i]][animationFrame2];
		else
			s=harpie[creatureFacing[i]][animationFrame2];
	}

	else if ((state[i]==kSlug) || (state[i]==kRustBeast) || (state[i]==kPoisonBeast))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=slug[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=slug[creatureFacing[i]][0];
		else
			s=slug[creatureFacing[i]][1];
	}

	else if (state[i]==kGazer)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=beholder[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=beholder[creatureFacing[i]][0];
		else
			s=beholder[creatureFacing[i]][1];
	}

	else if (state[i]==kHellHound)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=hellHound[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=hellHound[creatureFacing[i]][0];
		else
			s=hellHound[creatureFacing[i]][1];
	}

	else if (state[i]==kRat)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=rat[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=rat[creatureFacing[i]][0];
		else
			s=rat[creatureFacing[i]][1];
	}

	else if (state[i]==kBat)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=bat[(creatureFacing[i]-10)][animationFrame2];
		else if (creatureDirection[i]==4)
			s=bat[creatureFacing[i]][animationFrame2];
		else
			s=bat[creatureFacing[i]][animationFrame2];
	}

	else if (state[i]==kAnt || state[i]==kWorkAnt || state[i]==kWarriorAnt || state[i]==kForagerAnt)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=ant[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=ant[creatureFacing[i]][0];
		else
			s=ant[creatureFacing[i]][1];
	}

	else if (state[i]==kQueenAnt)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=queenant[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=queenant[creatureFacing[i]][0];
		else
			s=queenant[creatureFacing[i]][1];
	}

	else if (state[i]==kSnake)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=snake[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=snake[creatureFacing[i]][0];
		else
			s=snake[creatureFacing[i]][1];
	}

	else if ((state[i]==kTroll) || (state[i]==kForestTroll))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=troll[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=troll[creatureFacing[i]][0];
		else
			s=troll[creatureFacing[i]][1];
	}

	else if (state[i]==kZombie)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=zombie[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=zombie[creatureFacing[i]][0];
		else
			s=zombie[creatureFacing[i]][1];
	}

	else if (state[i]==kCentaur || state[i]==kElderCentaur)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=centaur[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=centaur[creatureFacing[i]][0];
		else
			s=centaur[creatureFacing[i]][1];
	}

	else if (state[i]==kCube)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=cube[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=cube[creatureFacing[i]][0];
		else
			s=cube[creatureFacing[i]][1];
	}

	else if (state[i]==kBlinkHound)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=blinkhound[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=blinkhound[creatureFacing[i]][0];
		else
			s=blinkhound[creatureFacing[i]][1];
	}
	
	else if (state[i]==kDwarf)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=dwarf1[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=dwarf1[creatureFacing[i]][0];
		else
			s=dwarf1[creatureFacing[i]][1];
	}
	
	else if (state[i]==kLion)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=lion[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=lion[creatureFacing[i]][0];
		else
			s=lion[creatureFacing[i]][1];
	}
	
	else if (state[i]==kGolem || state[i]==kIceGolem || state[i]==kMagmaGolem || state[i]==kSandGolem)//Dave state[i]==kIceGolem & magma & sand
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=golem[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=golem[creatureFacing[i]][0];
		else
			s=golem[creatureFacing[i]][1];
	}

	else if (state[i]==kRedDraco || state[i]==kIceDraco || state[i]==kForestDraco || state[i]==kBlackDraco || state[i]==kSandDraco)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=youngdragon[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=youngdragon[creatureFacing[i]][0];
		else
			s=youngdragon[creatureFacing[i]][1];
	}

	else if ((state[i]==kGiant) || (state[i]==kHillGiant) || (state[i]==kFrostGiant))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=giant[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=giant[creatureFacing[i]][0];
		else
			s=giant[creatureFacing[i]][1];
	}

	else if (state[i]==kSnail)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=snail[(creatureFacing[i]-10)][animationFrame2];
		else if (creatureDirection[i]==4)
			s=snail[creatureFacing[i]][animationFrame2];
		else
			s=snail[creatureFacing[i]][animationFrame2];
	}

	else if ((state[i]==kGreyWolf) || (state[i]==kBrownWolf) || (state[i]==kBlackWolf) || (state[i]==kGreyDog) || (state[i]==kBrownDog) || (state[i]==kBlackDog))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=wolf[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=wolf[creatureFacing[i]][0];
		else
			s=wolf[creatureFacing[i]][1];
	}

	else if (state[i]==kSheep)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=sheep[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=sheep[creatureFacing[i]][0];
		else
			s=sheep[creatureFacing[i]][1];
	}

	else if ((state[i]==kSerpent) || (state[i]==kPhaseSerpent))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=serpent[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=serpent[creatureFacing[i]][0];
		else
			s=serpent[creatureFacing[i]][1];
	}

	else if ((state[i]==kRedDragon) || (state[i]==kIceDragon) || (state[i]==kForestDragon) 
		 || (state[i]==kBlackDragon) || (state[i]==kPinkDragon)  || (state[i]==kSandDragon))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=dragon[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=dragon[creatureFacing[i]][0];
		else
			s=dragon[creatureFacing[i]][1];
	}

	else if ((state[i]==kSpider) || (state[i]==kFrostSpider) || (state[i]==kLavaSpider) || (state[i]==kSandSpider))
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=spider[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=spider[creatureFacing[i]][0];
		else
			s=spider[creatureFacing[i]][1];
	}

	else if (state[i]==kTusker)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=tusker[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=tusker[creatureFacing[i]][0];
		else
			s=tusker[creatureFacing[i]][1];
	}
	//Dave
	else if (state[i]==kPixie)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=pixie[(creatureFacing[i]-10)][animationFrame2];
		else if (creatureDirection[i]==4)
			s=pixie[creatureFacing[i]][animationFrame2];
		else
			s=pixie[creatureFacing[i]][animationFrame2];
	}
	//Dave^
	else if (state[i]==kForestDragonAncient || state[i]==kIceDragonAncient || state[i]==kBlackDragonAncient || state[i]==kRedDragonAncient)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=ancientdragon[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=ancientdragon[creatureFacing[i]][0];
		else
			s=ancientdragon[creatureFacing[i]][1];
	}
	else if (state[i]==kLich)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=lich[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=lich[creatureFacing[i]][0];
		else
			s=lich[creatureFacing[i]][1];
	}
	
	else if (state[i]==kBlackBear || state[i]==kBrownBear)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=bear[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=bear[creatureFacing[i]][0];
		else
			s=bear[creatureFacing[i]][1];
	}
	
	else if (state[i]==kShark)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=shark[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=shark[creatureFacing[i]][0];
		else
			s=shark[creatureFacing[i]][1];
	}
		else if (state[i]==kWrath)
	{
		if ((creatureFacing[i]>9) && (creatureFacing[i]<18))	// attacking
			s=wrath[(creatureFacing[i]-10)][2];
		else if (creatureDirection[i]==4)
			s=wrath[creatureFacing[i]][0];
		else
			s=wrath[creatureFacing[i]][1];
	}

	*r=s;

}

// ----------------------------------------------------------------- //
TBoolean gr_SnowAt(int row, int col)
// ----------------------------------------------------------------- //

{

	return (false);

	if ((row>=342) && (col>=304) && (row<=1110) && (col<=1260))
		return (true);

	if ((row>=1110) && (col>=1024) && (row<=1124) && (col<=1148))
		return (true);

	if ((row>=1110) && (col>=418) && (row<=1148) && (col<=878))
		return (true);

	if ((row>=1148) && (col>=418) && (row<=1472) && (col<=866))
		return (true);

	return (false);

}

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_SwapBitmaps(void)
// ----------------------------------------------------------------- //

{
	TBoolean							inSnow=false;
	TRect									r;

	//return;

	/*inSnow=gr_SnowAt(playerRow, playerCol);

	//--------------------------------------------------------------------

	if ((inSnow) && (currentMainBitmap!=5217))	// switch to ice island bmp
		{
			currentMainBitmap=5217;
			tb_DrawPictureToBitmap(smallGridBMP, currentMainBitmap, 0, 0, &r);
			#ifdef _DEBUGGING
			gr_AddText("Switching BMP",true,true); // beta
			#endif
		}

	if ((!inSnow) && (currentMainBitmap==5217))	// switch to standard bmp
		{
			currentMainBitmap=5128;//128;
			tb_DrawPictureToBitmap(smallGridBMP, currentMainBitmap, 0, 0, &r);
			#ifdef _DEBUGGING
			gr_AddText("Switching BMP",true,true); // beta
			#endif
		}

	//--------------------------------------------------------------------

	if ((playerRow>=340-13) && (playerRow<=440-13) && (playerCol>=60-13) && (playerCol<=197-13)) // switch to cave bmp
		{
	    if (currentDungeonBitmap!=5235)	// switch to cave bmp
	      {
				inDungeon=true;
				currentDungeonBitmap=5235;
				tb_DrawPictureToBitmap(havenGridBMP, 5235, 0, 0, &r);
				tb_DrawPictureToBitmap(havenGridMaskBMP, 5236, 0, 0, &r);
				#ifdef _DEBUGGING
				gr_AddText("Switching BMP",true,true); // beta
				#endif
	    }
		}
	else*/

	if ((playerRow>=1864-13) && (playerRow<=1978-13) && (playerCol>=1010-13) && (playerCol<=1111-13)) // switch to ice dungeon bmp
	{
		if (currentDungeonBitmap!=5224)	// switch to ice dungeon bmp
		{
			//inDungeon=true;
			currentDungeonBitmap=5224;
			tb_DrawPictureToBitmap(dungeonGridBMP, 5224, 0, 0, &r);
			tb_DrawPictureToBitmap(dungeonGridMaskBMP, 5222, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridBMP, 5224, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridMaskBMP, 5222, 0, 0, &r);
			//gr_AddText("Switching BMP",true,true); // beta

		}
	}
	else
	{
		if (currentDungeonBitmap!=5221)
		{
			currentDungeonBitmap=5221;
			tb_DrawPictureToBitmap(dungeonGridBMP, 5221, 0, 0, &r);
			tb_DrawPictureToBitmap(dungeonGridMaskBMP, 5222, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridBMP, 5148, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridMaskBMP, 5149, 0, 0, &r);

			//gr_AddText("Switching BMP",true,true); // beta

		}
	}

	/*
	else if ((playerRow>=448-13) && (playerRow<=577-13) && (playerCol>=69-13) && (playerCol<=209-13)) // switch to fire dungeon bmp
		{
	    if (currentDungeonBitmap!=5221)	// switch to ice dungeon bmp
	      {
					inDungeon=true;
					currentDungeonBitmap=5221;
					tb_DrawPictureToBitmap(havenGridBMP, 5221, 0, 0, &r);
					tb_DrawPictureToBitmap(havenGridMaskBMP, 5222, 0, 0, &r);
					#ifdef _DEBUGGING
					gr_AddText("Switching BMP",true,true); // beta
					#endif
	    }
		}
	else 	if ((playerRow<227) && (currentDungeonBitmap!=5221))	// switch to dungeon bmp
		{
			inDungeon=true;
			currentDungeonBitmap=5221;
			tb_DrawPictureToBitmap(havenGridBMP, 5221, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridMaskBMP, 5222, 0, 0, &r);
			#ifdef _DEBUGGING
			gr_AddText("Switching BMP",true,true); // beta
			#endif
		}
	else if ((playerRow>835) && (playerRow<897) && (playerCol>208) && (playerCol<255))	// in lizard stronghold
		{
			if (currentDungeonBitmap!=5152)
				{
					inDungeon=false;
					currentDungeonBitmap=5152;
					tb_DrawPictureToBitmap(havenGridBMP, 5152, 0, 0, &r);
					tb_DrawPictureToBitmap(havenGridMaskBMP, 5153, 0, 0, &r);
					#ifdef _DEBUGGING
					gr_AddText("Switching BMP",true,true); // beta
					#endif
				}
		}
	else if ((playerRow>506) && (playerRow<635) && (playerCol>1723) && (playerCol<1852))	// in ruins
		{
			if (currentDungeonBitmap!=5237)
				{
					inDungeon=false;
					currentDungeonBitmap=5237;
					tb_DrawPictureToBitmap(havenGridBMP, 5237, 0, 0, &r);
					tb_DrawPictureToBitmap(havenGridMaskBMP, 5238, 0, 0, &r);
					#ifdef _DEBUGGING
					gr_AddText("Switching BMP",true,true); // beta
					#endif
				}
		}
	else if ((playerRow>=227) && (currentDungeonBitmap!=5148))	// switch to castle bmp
		{
			inDungeon=false;
			currentDungeonBitmap=5148;
			tb_DrawPictureToBitmap(havenGridBMP, 5148, 0, 0, &r);
			tb_DrawPictureToBitmap(havenGridMaskBMP, 5149, 0, 0, &r);
			#ifdef _DEBUGGING
			gr_AddText("Switching BMP",true,true); // beta
			#endif
		}

	//--------------------------------------------------------------------
		*/
}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderPlayArea(int xOff, int yOff,TBoolean drawOnScreen)
// ----------------------------------------------------------------- //

{
	int								i;
	int								j;
	TRect							s;
	TRect							d;
	TRect							u;
	TRect							c;
	TRect							a;
	int								sH;
	int								sW;
	int								dH;
	int								dW;
	float							delta;
	int								row;
	int								col;
	int								id;
	TRect							hitsRect;
	unsigned long			currentTime=TickCount();
	int								stringWidth;
	int								stringXPosition;
	int								stringYPosition;
	int								stringLeft;
	int								stringRight;
	TBoolean					doubleElevated;
	int								theBitmap;
	TPoint						lightSources			[kMaxLightSources];
	int								lightSourcesSize	[kMaxLightSources];
	int								lightSourcePtr=0;
	TRect							r;
	TRect							tr;
	int								cx;
	int								cy;
	int								sx;
	int								sy;
	int								dir;
	int								tmpClass;
	int								theSound;
	TBoolean					secondPass[kMaxScreenElements];
	TBoolean					localEffectAt[kMaxScreenElements];
	int								doorCount=0;
	int								treeCount=0;
	//int								centerX;
	//int								centerY;
	//unsigned int			rr;
	//unsigned int			gg;
	//unsigned int			bb;
	TRect             raftRect;
	int               txOff;
	int               tyOff;
	char              creatureName[kStringLength]; //[32];
	TBoolean					foundGrass;
	int								thisCreatureChat;
	int					barWidth;

	unsigned long			timeStart;
	unsigned long			totalTime;

	if (!gotFirstRefresh)
		return;

	timeStart=TickCount();

	if (gotFirstDrawMessage==false)
		gotFirstDrawMessage=true;

	gr_Animate();

	standingOnContainer		=	false;

	//standingOnActionSpot	= false;
	actionValue						= 0;

	prevxOff=xOff;

	prevyOff=yOff;

	for (i=0;i<kMaxScreenElements;i++)
	{
		row=mapToScreenRow[i]+playerRow;
		col=mapToScreenCol[i]+playerCol;

		if ((clientMap[row][col]>=359) && (clientMap[row][col]<=373))
			treeCount++;

		localEffectAt[i]=false;

		secondPass[i]=false;

		if (elevated[clientMap[row][col]]==true)	// client map
			secondPass[i]=true;
		else if (containerState[i]!=0)
			secondPass[i]=true;
		else if (tracksState[i]!=0)
			secondPass[i]=true;
		else if (state[i]!=0)
			secondPass[i]=true;

		tb_SetRect(&clickBaseRect[i],0,0,0,0);

		tb_SetRect(&clickObjectRect[i],0,0,0,0);

		tb_SetRect(&clickCreatureRect[i],0,0,0,0);

		tb_SetRect(&clickTerrainRect[i],0,0,0,0);

		destCreatureRect[i]=destRect[i];

		//-----------------------------------
		destScreenRect[i]=destRect[i];

		tb_OffsetRect(&destScreenRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

		//-----------------------------------

		clickTerrainID[i]=0;

		//-------------------------------------------------------	// sound effects

		if (clientMap[row][col]==719) // on pier ... ambient seagull sound	// client map
		{
			if (tb_Rnd(1,20000)==1)
			{
				do
					{ theSound=soundList[kSeagull][tb_Rnd(0,(kMaxSoundTypes-1))]; }
				while (theSound==0);

				tb_PlaySound(theSound);
			}
		}
		
		//Dave
		if (clientMap[row][col]>=1106 && clientMap[row][col]<=1108) // pipe tiles water drip	// client map
		{
			if (tb_Rnd(1,800)==1)
			{
				do
					{ theSound=soundList[kWaterDripSnd][tb_Rnd(0,(kMaxSoundTypes-1))]; }
				while (theSound==0);

				tb_PlaySound(theSound);
			}
		}
		
	//	if (tb_Rnd(0,1000)<10)
	//			tb_PlaySound(tb_Rnd(225,227));
/**/
		//-------------------------------------------------------

		if (i==90)	// check for standing in shop
		{
			//standingOnActionSpot	=	false;

			//if ((clientMap[row][col]==929) || (clientMap[row][col]==939) || (clientMap[row][col]==979))
			//	{
			//		standingOnActionSpot=true;	// action spot // client map
			//		actionValue=929;
			//	}

			//if ((wasStandingOnActionSpot) && (!standingOnActionSpot))
			//	{
			//		for (j=0;j<96;j++)
			//      localChess[j]=0;
			//    gr_CloseFloatingWindow(tbChessWindow);
			//	}

			if ((wasStandingInShop) && (!standingInShop))
			{
				sh_ClearShopContents();
				sh_ClearSellList();
				gr_CloseFloatingWindow(tbShopContentsWindow);
				gr_CloseFloatingWindow(tbSellListWindow);

				if ((playerAction==kSelling) || (playerAction==kBuying))
					ClearPlayerAction();
			}

			if ((wasStandingInBank) && (!standingInBank))
			{
				in_ClearBank();
				gr_CloseFloatingWindow(tbBankWindow);

				if ((playerAction==kWithdrawing) || (playerAction==kDepositing))
					ClearPlayerAction();
			}

			if ((!wasStandingInBank) && (standingInBank))
			{
				in_ClearBank();
			}

			if ((wasStandingOnActionSpot) && (!standingOnActionSpot))
			{
				for (j=0;j<96;j++)
					localChess[j]=0;

				gr_CloseFloatingWindow(tbChessWindow);
			}

			wasStandingInBank	=	standingInBank;

			wasStandingInShop	=	standingInShop;
			wasStandingOnActionSpot = standingOnActionSpot;
		}

		//------------------------------------------------------

		if (elevated[clientMap[row][col]]==false)	// client map
		{
			s=srcRect[clientMap[row][col]];
			theBitmap=clientMap[row][col];
		}
		else
		{
			if (base[clientMap[row][col]]==0) continue;

			if ((clientMap[row][col]>=536) && (clientMap[row][col]<=544))
			{
				foundGrass=false;

				if ((clientMap[row-1][col-1]<536) || (clientMap[row-1][col-1]>544))				foundGrass=true;
				else if ((clientMap[row][col-1]<536) || (clientMap[row][col-1]>544))			foundGrass=true;
				else if ((clientMap[row+1][col-1]<536) || (clientMap[row+1][col-1]>544))	foundGrass=true;

				else if ((clientMap[row-1][col]<536) || (clientMap[row-1][col]>544))			foundGrass=true;
				else if ((clientMap[row+1][col]<536) || (clientMap[row+1][col]>544))			foundGrass=true;

				else if ((clientMap[row-1][col+1]<536) || (clientMap[row-1][col+1]>544))	foundGrass=true;
				else if ((clientMap[row][col+1]<536) || (clientMap[row][col+1]>544))			foundGrass=true;
				else if ((clientMap[row+1][col+1]<536) || (clientMap[row+1][col+1]>544))	foundGrass=true;

				if (!foundGrass) { continue; }
			}

			s=srcRect[base[clientMap[row][col]]];

			theBitmap=base[clientMap[row][col]];
		}

		sH=s.bottom-s.top;

		sW=s.right-s.left;

		d=destRect[i];
		dH=d.bottom-d.top;
		dW=d.right-d.left;

		if (sH!=dH)
			d.top=d.bottom-sH;

		if (sourceGrid[theBitmap]==kMainBMP)
			tb_FastCopyMaskBitmap(smallGridBMP, smallGridMaskBMP, playBMP, &s, &s, &d);
		else if (sourceGrid[theBitmap]==kHavenBMP)
			tb_FastCopyMaskBitmap(havenGridBMP, havenGridMaskBMP, playBMP, &s, &s, &d);
		else if (sourceGrid[theBitmap]==kTownBMP)
			tb_FastCopyMaskBitmap(townGridBMP, townGridMaskBMP, playBMP, &s, &s, &d);
		else if (sourceGrid[theBitmap]==kIceBMP)
			tb_FastCopyMaskBitmap(iceGridBMP, iceGridMaskBMP, playBMP, &s, &s, &d);
		else if (sourceGrid[theBitmap]==kDungeonBMP)
			tb_FastCopyMaskBitmap(dungeonGridBMP, dungeonGridMaskBMP, playBMP, &s, &s, &d);
		else if (sourceGrid[theBitmap]==kCaveBMP)
			tb_FastCopyMaskBitmap(caveGridBMP, caveGridMaskBMP, playBMP, &s, &s, &d);

		clickBaseRect[i]=d;

		tb_OffsetRect(&clickBaseRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

		//blood stains ---------
		if (bloodStainState[i]!=0)
		{
			tb_FastCopyMaskBitmap(containerBMP, containerMaskBMP, playBMP, &bloodTrail[bloodStainState[i]], &bloodTrail[bloodStainState[i]], &destRect[i]);

			/*
								s=bloodTrail[bloodStainState[i]];

								sH=s.bottom-s.top;
								sW=s.right-s.left;

								d=destRect[i];
								dH=d.bottom-d.top;
								dW=d.right-d.left;

								if (sH!=dH)
									d.top=d.bottom-sH;

								if (sW>dW)
									{
										delta=(sW-dW);
										delta=delta/2;
										d.left=d.left-delta;
										d.right=d.right+delta;
									}
								else if (sW<dW)
									{
										delta=(dW-sW);
										delta=delta/2;
										d.left=d.left+delta;
										d.right=d.right-delta;
									}

								tb_FastCopyMaskBitmap(containerBMP, containerMaskBMP, playBMP, &s, &s, &d);
								*/
		}

	}

	for (j=0;j<kMaxLocalFX;j++)
		if (localEffectsDestination[j]!=255)
		{
			localEffectAt[localEffectsDestination[j]]=true;
			secondPass[localEffectsDestination[j]]=true;
		}

	for (i=0;i<kMaxScreenElements;i++)
		if (secondPass[i])
		{

			row=mapToScreenRow[i]+playerRow;
			col=mapToScreenCol[i]+playerCol;

			doubleElevated=false;

			if (elevated[clientMap[row][col]]==true)	// client map
			{
				id=clientMap[row][col];

				if ((id==1085) && (state[i]!=0))		// check for creature standing in doorway
					{ id=1087;		doorCount++; }

				if ((id==1088) && (state[i]!=0))		// check for creature standing in doorway
					{ id=1090;		doorCount++; }

				if ((id==629) && (state[i]!=0))		// check for creature standing in doorway
					{ id=631;		doorCount++; }

				if ((id==633) && (state[i]!=0))		// check for creature standing in doorway
					{ id=635;		doorCount++; }

				if ((id==693) && (state[i]!=0))		// check for creature standing in doorway
					{ id=688;		doorCount++; }

				if ((id==694) && (state[i]!=0))		// check for creature standing in doorway
					{ id=690;		doorCount++; }

				if ((id==713) && (state[i]!=0))		// check for creature standing in doorway
					{ id=711;		doorCount++; }

				if ((id==782) && (state[i]!=0))		// check for creature standing in doorway
					{ id=781;		doorCount++; }

				if ((id==810) && (state[i]!=0))		// check for creature standing in doorway
					{ id=811;		doorCount++; }

				if ((id==823) && (state[i]!=0))		// check for creature standing in doorway
					{ id=824;		doorCount++; }

				s=srcRect[id];

				sH=s.bottom-s.top;
				sW=s.right-s.left;

				d=destRect[i];
				dH=d.bottom-d.top;
				dW=d.right-d.left;

				if (sH!=dH)
					d.top=d.bottom-sH;

				d.right=d.left+sW;

				d.left=d.left-((sW-dW)/2);

				d.right=d.right-((sW-dW)/2);

				// check for light sources.. ex: torch --------------------------------------------
				if (true)
				{
					if ((id==618) || (id==1081) || (id==1083))	// torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+47;
							lightSources[lightSourcePtr].v		=	d.top+104;
							lightSourcesSize[lightSourcePtr]	= 35;
							lightSourcePtr++;
						}
					}
					else if ((id==619) || (id==1082) || (id==1084))	// torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+15;
							lightSources[lightSourcePtr].v		=	d.top+104;
							lightSourcesSize[lightSourcePtr]	= 35;
							lightSourcePtr++;
						}
					}
					else if (id==637)	// candle on table
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+26;
							lightSources[lightSourcePtr].v		=	d.top+14;
							lightSourcesSize[lightSourcePtr]	= 15;
							lightSourcePtr++;
						}
					}
					else if ((id==644)	&& (currentDungeonBitmap!=5237)) // castle stove
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+39;
							lightSources[lightSourcePtr].v		=	d.top+121;
							lightSourcesSize[lightSourcePtr]	= 30;
							lightSourcePtr++;
						}
					}
					else if ((id==1180)	&& (currentDungeonBitmap!=5237)) // castle stove S
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+52;
							lightSources[lightSourcePtr].v		=	d.top+131;
							lightSourcesSize[lightSourcePtr]	= 30;
							lightSourcePtr++;
						}
					}
					else if (id==1015)	// lizard fire
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+17;
							lightSourcesSize[lightSourcePtr]	= 20;
							lightSourcePtr++;
						}
					}
					else if (id==972)	// lizard fire
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+17;
							lightSourcesSize[lightSourcePtr]	= 30;
							lightSourcePtr++;
						}
					}
					else if (id==983)	// helena fire
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+17;
							lightSourcesSize[lightSourcePtr]	= 30;
							lightSourcePtr++;
						}
					}
					else if (id==683)	// log cabin fireplace
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+27;
							lightSources[lightSourcePtr].v		=	d.top+96;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==692) // log cabin fireplace
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+38;
							lightSources[lightSourcePtr].v		=	d.top+98;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==736) // log cabin torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+34;
							lightSources[lightSourcePtr].v		=	d.top+65;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==852) // log cabin torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+62;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==853) // log cabin torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+34;
							lightSources[lightSourcePtr].v		=	d.top+63;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==854) // log cabin torch
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+63;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==738) // forge
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+32;
							lightSources[lightSourcePtr].v		=	d.top+10;
							lightSourcesSize[lightSourcePtr]	= 25;
							lightSourcePtr++;
						}
					}
					else if (id==784) // log cabin stove
					{
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+43;
							lightSources[lightSourcePtr].v		=	d.top+101;
							lightSourcesSize[lightSourcePtr]	= 20;
							lightSourcePtr++;
						}
					}
				}

				// ---------------------------------------------------------------------------------

				if (sourceGrid[id]==1)
					tb_FastCopyMaskBitmap(smallGridBMP, smallGridMaskBMP, playBMP, &s, &s, &d);
				else if (sourceGrid[id]==kHavenBMP)
					tb_FastCopyMaskBitmap(havenGridBMP, havenGridMaskBMP, playBMP, &s, &s, &d);
				else if (sourceGrid[id]==kTownBMP)
					tb_FastCopyMaskBitmap(townGridBMP, townGridMaskBMP, playBMP, &s, &s, &d);
				else if (sourceGrid[id]==kIceBMP)
					tb_FastCopyMaskBitmap(iceGridBMP, iceGridMaskBMP, playBMP, &s, &s, &d);
				else if (sourceGrid[id]==kDungeonBMP)
					tb_FastCopyMaskBitmap(dungeonGridBMP, dungeonGridMaskBMP, playBMP, &s, &s, &d);
				else if (sourceGrid[id]==kCaveBMP)
					tb_FastCopyMaskBitmap(caveGridBMP, caveGridMaskBMP, playBMP, &s, &s, &d);

				//-----------------------------------------
				if ((id==968) || (id==969) || (id==970))
				{
					tb_SetRect(&s,0,0,52,34);
					tb_OffsetRect(&s,d.left+(((d.right-d.left)-52)/2),d.top+(((d.bottom-d.top)-34)/2)-25);
					tb_FastCopyMaskBitmap(containerBMP, containerMaskBMP, playBMP, &fliesRect[fliesPtr], &fliesRect[fliesPtr], &s);
				}

				//-----------------------------------------

				clickTerrainRect[i]	=	d;

				tb_OffsetRect(&clickTerrainRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

				clickTerrainID[i]		=	id;

				//---------------------------------------------------------------------
				for (j=0;j<i;j++)	// redraw creature if terrain overlaps
					if ((state[j]!=kTusker) && (state[j]!=kRedDragon) && (state[j]!=kIceDragon) && (state[j]!=kPinkDragon) && (state[j]!=kForestDragon) 
						&& (state[j]!=kBlackDragon) && (state[j]!=kBlackDragonAncient) && (state[j]!=kIceDragonAncient) && (state[j]!=kRedDragonAncient) 
							&& (state[j]!=kForestDragonAncient) && (state[j]!=kGolem) && (state[j]!=kIceGolem) && (state[j]!=kMagmaGolem) 
								&& (state[j]!=kSandGolem) && (state[j]!=kSandDragon) && (state[j]!=0))
						
						if (tb_IntersectRect(&u,&clickTerrainRect[i],&clickCreatureRect[j]))
							if (clickTerrainRect[i].bottom<clickCreatureRect[j].bottom)
							{
								gr_GetCreatureRect(j, &s);

								sH=s.bottom-s.top;
								sW=s.right-s.left;

								d=destRect[j];
								dH=d.bottom-d.top;
								dW=d.right-d.left;

								if (sH!=dH)
									d.top=d.bottom-sH;

								d.right=d.left+sW;

								d.left=d.left-((sW-dW)/2);

								d.right=d.right-((sW-dW)/2);

								if ((state[j]==kSpider) || (state[j]==kFrostSpider) || (state[j]==kLavaSpider) || (state[j]==kSandSpider))
									tb_OffsetRect(&d,0,21);

								if ((state[j]==kGiant) || (state[j]==kHillGiant) || (state[j]==kFrostGiant))
									tb_OffsetRect(&d,0,5);

								if (j==90) tb_OffsetRect(&d,xOff,yOff);
								else if (creatureDirection[j]==0) tb_OffsetRect(&d,0,-24);//-24
								else if (creatureDirection[j]==1) tb_OffsetRect(&d,-16,-16);
								else if (creatureDirection[j]==2) tb_OffsetRect(&d,-24,0);//-24
								else if (creatureDirection[j]==3) tb_OffsetRect(&d,16,-16);
								else if (creatureDirection[j]==5) tb_OffsetRect(&d,-16,16);
								else if (creatureDirection[j]==6) tb_OffsetRect(&d,24,0);//24
								else if (creatureDirection[j]==7) tb_OffsetRect(&d,16,16);
								else if (creatureDirection[j]==8) tb_OffsetRect(&d,0,24);//24

								clickCreatureRect[j]=d;

								tb_OffsetRect(&clickCreatureRect[j],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

								if (state[j]==kHuman)
								{
									gr_BuildHuman(j,false, false);
									tb_FastCopyMaskBitmap(humanBuilderBMP, humanBuilderMaskBMP, playBMP, &humanBuilderRect, &humanBuilderRect, &d);
								}
								else if (state[j]==kNPCHuman || state[j]==kNPCHumanFemale)
								{
									gr_BuildHuman(j,false, false);
									tb_FastCopyMaskBitmap(humanBuilderBMP, humanBuilderMaskBMP, playBMP, &humanBuilderRect, &humanBuilderRect, &d);
								}
								else
								{
									if (creatureInfo[state[j]].color!=0)
									{
										c.left=0;
										c.top=0;
										c.right=s.right-s.left;
										c.bottom=s.bottom-s.top;
										tb_CopyBitmap(creatureGridBMP[creatureInfo[state[j]].bmp], creatureColorizerBMP, &s, &c, false);
										tb_ColorizeCreature(creatureColorizerBMP,creatureInfo[state[j]].color,&c);
										tb_FastCopyMaskBitmap(creatureColorizerBMP, creatureGridMaskBMP[creatureInfo[state[j]].bmp], playBMP, &c, &s, &d);
									}
									else
										tb_FastCopyMaskBitmap(creatureGridBMP[creatureInfo[state[j]].bmp], creatureGridMaskBMP[creatureInfo[state[j]].bmp], playBMP, &s, &s, &d);
								}

							}

				//---------------------------------------------------------------------

			}

			if (containerState[i]!=0) // objects
			{
				if ((i==90) && (containerState[i]!=8))
					standingOnContainer=true;

				doubleElevated=true;

				s=containerRect[containerState[i]];

				sH=s.bottom-s.top;

				sW=s.right-s.left;

				d=destRect[i];

				dH=d.bottom-d.top;

				dW=d.right-d.left;

				if (sH!=dH)
					d.top=d.bottom-sH;

				if (sW>dW)
				{
					delta=(sW-dW);
					delta=delta/2;
					d.left=d.left-delta;
					d.right=d.right+delta;
				}
				else if (sW<dW)
				{
					delta=(dW-sW);
					delta=delta/2;
					d.left=d.left+delta;
					d.right=d.right-delta;
				}

				if (true)
				{
					if (containerState[i]==8) // kindling
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+30;
							lightSourcesSize[lightSourcePtr]	= 8;
							lightSourcePtr++;
						}

					if (containerState[i]==50) // fire prop
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+31;
							lightSources[lightSourcePtr].v		=	d.top+17;
							lightSourcesSize[lightSourcePtr]	= 30;
							lightSourcePtr++;
						}
					//Dave 
					if (containerState[i]==198) // candle
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+36;
							lightSources[lightSourcePtr].v		=	d.top+28;
							lightSourcesSize[lightSourcePtr]	= 10;
							lightSourcePtr++;
						}
					if (containerState[i]==199) // birthday cake
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+35;
							lightSources[lightSourcePtr].v		=	d.top+20;
							lightSourcesSize[lightSourcePtr]	= 10;
							lightSourcePtr++;
						}
						
					if (containerState[i]==59) // pumpkin
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+30;
							lightSources[lightSourcePtr].v		=	d.top+30;
							lightSourcesSize[lightSourcePtr]	= 12;
							lightSourcePtr++;
						}
					if (containerState[i]>=211 && containerState[i]<=227)// infused crystals
						if (lightSourcePtr<kMaxLightSources)
						{
							lightSources[lightSourcePtr].h		=	d.left+32;
							lightSources[lightSourcePtr].v		=	d.top+32;
							lightSourcesSize[lightSourcePtr]	= 8;
							lightSourcePtr++;
						}
				}

				//body offset area
				if ((containerState[i]==15) || (containerState[i]==20) || (containerState[i]==31) || (containerState[i]==46) || (containerState[i]==74) || (containerState[i]==194))// dragon corpse
					tb_OffsetRect(&d,0,36);

				if ((containerState[i]==104) || (containerState[i]==120) || (containerState[i]==121) || (containerState[i]==122) || (containerState[i]==195))// draco corpse
					tb_OffsetRect(&d,0,20);

				if ((containerState[i]==97)) //Snake corpse
					tb_OffsetRect(&d,0,35);

				if ((containerState[i]==125)) //Tusker corpse
					tb_OffsetRect(&d,-23,50);
				
				if ((containerState[i]==142) || (containerState[i]==183) || (containerState[i]==184) || (containerState[i]==193)) //Golem corpse
					tb_OffsetRect(&d, -5, 30);

				if (containerColor[containerState[i]]!=kNoCreatureColor)
				{
					c.left=0;
					c.top=0;
					c.right=s.right-s.left;
					c.bottom=s.bottom-s.top;
					tb_CopyBitmap(containerBMP, creatureColorizerBMP, &s, &c, false);
					tb_ColorizeCreature(creatureColorizerBMP,containerColor[containerState[i]],&c);
					tb_FastCopyMaskBitmap(creatureColorizerBMP, containerMaskBMP, playBMP, &c, &s, &d);
				}
				else
					tb_FastCopyMaskBitmap(containerBMP, containerMaskBMP, playBMP, &s, &s, &d);

				clickObjectRect[i]=d;

				tb_OffsetRect(&clickObjectRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

				if (fliesOnCorpse[containerState[i]])
				{
					tb_SetRect(&s,0,0,52,34);
					tb_OffsetRect(&s,d.left+(((d.right-d.left)-52)/2),d.top+(((d.bottom-d.top)-34)/2));
					tb_FastCopyMaskBitmap(containerBMP, containerMaskBMP, playBMP, &fliesRect[fliesPtr], &fliesRect[fliesPtr], &s);
				}
			}

			if (tracksState[i]!=0 && tracksState[i]<50) // tracks appearing underneath
			{
				doubleElevated=true;

				s=tracksRect[tracksState[i]];

				sH=s.bottom-s.top;
				sW=s.right-s.left;

				d=destRect[i];
				dH=d.bottom-d.top;
				dW=d.right-d.left;

				if (sH!=dH)
					d.top=d.bottom-sH;

				if (sW>dW)
				{
					delta=(sW-dW);
					delta=delta/2;
					d.left=d.left-delta;
					d.right=d.right+delta;
				}
				else if (sW<dW)
				{
					delta=(dW-sW);
					delta=delta/2;
					d.left=d.left+delta;
					d.right=d.right-delta;
				}


				tb_FastCopyMaskBitmap(tracksBMP, tracksMaskBMP, playBMP, &s, &s, &d);

			}

			if (state[i]!=0)	// creatures
			{
				doubleElevated=true;

				gr_GetCreatureRect(i, &s);

				sH=s.bottom-s.top;
				sW=s.right-s.left;

				d=destRect[i];
				dH=d.bottom-d.top;
				dW=d.right-d.left;

				if ((state[i]==kRedDragon) || (state[i]==kIceDragon) || (state[i]==kPinkDragon)  || (state[i]==kBlackDragon) || (state[i]==kForestDragon)  || (state[i]==kSandDragon))
				{
					d.bottom=d.top+sH;
					d.right=d.left+sW;
					dir=creatureFacing[i];

					if ((dir>9) && (dir<18))
						dir=dir-10;

					if (dir==0) tb_OffsetRect(&d,-63,-80);
					else if (dir==1) tb_OffsetRect(&d,-78,-78);
					else if (dir==2) tb_OffsetRect(&d,-75,-58);
					else if (dir==3) tb_OffsetRect(&d,-86,-56);
					else if (dir==4) tb_OffsetRect(&d,-60,-58);
					else if (dir==5) tb_OffsetRect(&d,-42,-60);
					else if (dir==6) tb_OffsetRect(&d,-38,-75);
					else if (dir==7) tb_OffsetRect(&d,-45,-99);
				}
			
				else if (state[i]==kTusker)
				{
					d.bottom=d.top+sH;
					d.right=d.left+sW;
					dir=creatureFacing[i];

					if ((dir>9) && (dir<18))
						dir=dir-10;

					if (dir==0) tb_OffsetRect(&d,-48,-80);
					else if (dir==1) tb_OffsetRect(&d,-63,-78);
					else if (dir==2) tb_OffsetRect(&d,-50,-70);
					else if (dir==3) tb_OffsetRect(&d,-65,-65);
					else if (dir==4) tb_OffsetRect(&d,-53,-60);
					else if (dir==5) tb_OffsetRect(&d,-40,-70);
					else if (dir==6) tb_OffsetRect(&d,-50,-82);
					else if (dir==7) tb_OffsetRect(&d,-55,-82);

				}
				else
				{
					if (sH!=dH)
						d.top=d.bottom-sH;

					d.right=d.left+sW;

					d.left=d.left-((sW-dW)/2);

					d.right=d.right-((sW-dW)/2);

					if ((state[i]==kSpider) || (state[i]==kFrostSpider) || (state[i]==kLavaSpider) || (state[i]==kSandSpider))
						tb_OffsetRect(&d,0,21);

					if ((state[i]==kGiant) || (state[i]==kHillGiant) || (state[i]==kFrostGiant))
						tb_OffsetRect(&d,0,5);
				}

				if (i==90) tb_OffsetRect(&d,xOff,yOff);
				else if (creatureDirection[i]==0) tb_OffsetRect(&d,0,-24);//-24
				else if (creatureDirection[i]==1) tb_OffsetRect(&d,-16,-16);
				else if (creatureDirection[i]==2) tb_OffsetRect(&d,-24,0);//-24
				else if (creatureDirection[i]==3) tb_OffsetRect(&d,16,-16);
				else if (creatureDirection[i]==5) tb_OffsetRect(&d,-16,16);
				else if (creatureDirection[i]==6) tb_OffsetRect(&d,24,0);//24
				else if (creatureDirection[i]==7) tb_OffsetRect(&d,16,16);
				else if (creatureDirection[i]==8) tb_OffsetRect(&d,0,24);//24

				clickCreatureRect[i]=d;

				destCreatureRect[i]=d;

				destCreatureRect[i].bottom=destCreatureRect[i].bottom-24;

				tb_OffsetRect(&clickCreatureRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

				if (state[i]==kHuman)
				{

					if (canSwimIn[clientMap[row][col]])
					{
						raftRect=destRect[i];

						if (i==90) tb_OffsetRect(&raftRect,xOff,yOff);
						else if (creatureDirection[i]==0) tb_OffsetRect(&raftRect,0,-24);//-24
						else if (creatureDirection[i]==1) tb_OffsetRect(&raftRect,-16,-16);
						else if (creatureDirection[i]==2) tb_OffsetRect(&raftRect,-24,0);//-24
						else if (creatureDirection[i]==3) tb_OffsetRect(&raftRect,16,-16);
						else if (creatureDirection[i]==5) tb_OffsetRect(&raftRect,-16,16);
						else if (creatureDirection[i]==6) tb_OffsetRect(&raftRect,24,0);//24
						else if (creatureDirection[i]==7) tb_OffsetRect(&raftRect,16,16);
						else if (creatureDirection[i]==8) tb_OffsetRect(&raftRect,0,24);//24

						tb_FastCopyMaskBitmap(smallGridBMP, smallGridMaskBMP, playBMP, &srcRect[961], &srcRect[961], &raftRect);

						gr_BuildHuman(i,true,false);
					}
					else
						gr_BuildHuman(i,false,false);

					tb_FastCopyMaskBitmap(humanBuilderBMP, humanBuilderMaskBMP, playBMP, &humanBuilderRect, &humanBuilderRect, &d);
				}
				else if (state[i]==kNPCHuman || state[i]==kNPCHumanFemale)
				{
					if (canSwimIn[clientMap[row][col]])
					{
						raftRect=destRect[i];

						if (i==90) tb_OffsetRect(&raftRect,xOff,yOff);
						else if (creatureDirection[i]==0) tb_OffsetRect(&raftRect,0,-24);//-24
						else if (creatureDirection[i]==1) tb_OffsetRect(&raftRect,-16,-16);
						else if (creatureDirection[i]==2) tb_OffsetRect(&raftRect,-24,0);//-24
						else if (creatureDirection[i]==3) tb_OffsetRect(&raftRect,16,-16);
						else if (creatureDirection[i]==5) tb_OffsetRect(&raftRect,-16,16);
						else if (creatureDirection[i]==6) tb_OffsetRect(&raftRect,24,0);//24
						else if (creatureDirection[i]==7) tb_OffsetRect(&raftRect,16,16);
						else if (creatureDirection[i]==8) tb_OffsetRect(&raftRect,0,24);//24

						tb_FastCopyMaskBitmap(smallGridBMP, smallGridMaskBMP, playBMP, &srcRect[961], &srcRect[961], &raftRect);

						gr_BuildHuman(i,true,false);
					}
					else
						gr_BuildHuman(i,false,false);

					tb_FastCopyMaskBitmap(humanBuilderBMP, humanBuilderMaskBMP, playBMP, &humanBuilderRect, &humanBuilderRect, &d);
				}
				else
				{
					if ((state[i]==kRedDragon) || (state[i]==kIceDragon) || (state[i]==kPinkDragon)  || (state[i]==kBlackDragon)  || (state[i]==kForestDragon) || (state[i]==kSandDragon))
					{
						if (creatureInfo[state[i]].color!=0)
						{
							c.left=0;
							c.top=0;
							c.right=s.right-s.left;
							c.bottom=s.bottom-s.top;
							tb_CopyBitmap(greendragonBMP, creatureColorizerBMP, &s, &c, false);
							tb_ColorizeCreature(creatureColorizerBMP,creatureInfo[state[i]].color,&c);
							tb_FastCopyMaskBitmap(creatureColorizerBMP, greendragonMaskBMP, playBMP, &c, &s, &d);
						}
						else
							tb_FastCopyMaskBitmap(greendragonBMP, greendragonMaskBMP, playBMP, &s, &s, &d);
					}
					else
					{
						if (creatureInfo[state[i]].color!=0)
						{
							c.left=0;
							c.top=0;
							c.right=s.right-s.left;
							c.bottom=s.bottom-s.top;
							tb_CopyBitmap(creatureGridBMP[creatureInfo[state[i]].bmp], creatureColorizerBMP, &s, &c, false);
							tb_ColorizeCreature(creatureColorizerBMP,creatureInfo[state[i]].color,&c);
							tb_FastCopyMaskBitmap(creatureColorizerBMP, creatureGridMaskBMP[creatureInfo[state[i]].bmp], playBMP, &c, &s, &d);
						}
						else
							tb_FastCopyMaskBitmap(creatureGridBMP[creatureInfo[state[i]].bmp], creatureGridMaskBMP[creatureInfo[state[i]].bmp], playBMP, &s, &s, &d);
					}
				}

				tb_SetRect(&healthBar[i],(d.left+((d.right-d.left)/2))-15,d.bottom-5-10,(d.left+((d.right-d.left)/2))+15,d.bottom-10);

				tb_SetRect(&afkTag[i],(d.left+((d.right-d.left)/2))-15,d.top-15,(d.left+((d.right-d.left)/2))+15,d.top+6);
				
				tb_SetRect(&secondaryBar[i], (d.left+((d.right-d.left)/2))-15,d.bottom-11,(d.left+((d.right-d.left)/2))+15,d.bottom-6);

			}

			if ((elevated[clientMap[row][col]]==true) && (doubleElevated))	// client map
			{
				id=clientMap[row][col];

				if ((id==693) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=688;

				if ((id==694) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=690;

				if ((id==713) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=711;

				if ((id==782) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=781;

				if ((id==810) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=811;

				if ((id==823) && (state[i]!=0))		// check for creature standing in cabin doorway
					id=824;

				if ((id!=629) && (id!=633) && (id!=1085) && (id!=1088))	// if not standing in castle doorway
				{
					s=srcRect[id];

					sH=s.bottom-s.top;
					sW=s.right-s.left;

					d=destRect[i];
					dH=d.bottom-d.top;
					dW=d.right-d.left;

					if (sH!=dH)
						d.top=d.bottom-sH;

					if (sW>dW)
					{
						delta=(sW-dW);
						delta=delta/2;
						d.left=d.left-delta;
						d.right=d.right+delta;
					}
					else if (sW<dW)
					{
						delta=(dW-sW);
						delta=delta/2;
						d.left=d.left+delta;
						d.right=d.right-delta;
					}

					if (sourceGrid[id]==1)
						tb_FastCopyMaskBitmap(smallGridBMP, smallGridMaskBMP, playBMP, &s, &s, &d);
					else if (sourceGrid[id]==kHavenBMP)
						tb_FastCopyMaskBitmap(havenGridBMP, havenGridMaskBMP, playBMP, &s, &s, &d);
					else if (sourceGrid[id]==kTownBMP)
						tb_FastCopyMaskBitmap(townGridBMP, townGridMaskBMP, playBMP, &s, &s, &d);
					else if (sourceGrid[id]==kIceBMP)
						tb_FastCopyMaskBitmap(iceGridBMP, iceGridMaskBMP, playBMP, &s, &s, &d);
					else if (sourceGrid[id]==kDungeonBMP)
						tb_FastCopyMaskBitmap(dungeonGridBMP, dungeonGridMaskBMP, playBMP, &s, &s, &d);
					else if (sourceGrid[id]==kCaveBMP)
						tb_FastCopyMaskBitmap(caveGridBMP, caveGridMaskBMP, playBMP, &s, &s, &d);

					clickTerrainRect[i]	=	d;

					tb_OffsetRect(&clickTerrainRect[i],(playScreenRect.left-24-xOff),(playScreenRect.top-24-yOff));

					clickTerrainID[i]		=	id;
				}

			}
			
			if (tracksState[i]!=0 && tracksState[i]>50) // tracks appearing over
			{
				doubleElevated=true;

				s=tracksRect[tracksState[i]];

				sH=s.bottom-s.top;
				sW=s.right-s.left;

				d=destRect[i];
				dH=d.bottom-d.top;
				dW=d.right-d.left;

				if (sH!=dH)
					d.top=d.bottom-sH;

				if (sW>dW)
				{
					delta=(sW-dW);
					delta=delta/2;
					d.left=d.left-delta;
					d.right=d.right+delta;
				}
				else if (sW<dW)
				{
					delta=(dW-sW);
					delta=delta/2;
					d.left=d.left+delta;
					d.right=d.right-delta;
				}

				if (tracksState[i]==51)
				{
					d.bottom-=20;
					d.top-=20;
				}
				
				tb_FastCopyMaskBitmap(tracksBMP, tracksMaskBMP, playBMP, &s, &s, &d);

			}

			//---------------------------------------------------------------------------------------

			if (localEffectAt[i])
				for (j=0;j<kMaxLocalFX;j++)
					if (localEffectsDestination[j]==i)
					{

						if (fxFrames[localEffectsType[j]]!=0)
						{
							if (localEffectsFrame[j]<fxFrames[localEffectsType[j]]-1)
								localEffectsFrame[j]++;
							else if (localEffectsType[j]!=kBrackenRootX)
							{
								localEffectsDestination[j]=255; //clearing routine will do the rest
							}
						}
							

						if ((localEffectsDestination[j]!=255) && (localEffectsLocation[j]!=255))
						{

							cx=destCreatureRect[i].left+(destCreatureRect[i].right-destCreatureRect[i].left)/2;
							cy=destCreatureRect[i].top+(destCreatureRect[i].bottom-14-destCreatureRect[i].top)/2;

							sx=destCreatureRect[localEffectsLocation[j]].left+(destCreatureRect[localEffectsLocation[j]].right-destCreatureRect[localEffectsLocation[j]].left)/2;
							sy=destCreatureRect[localEffectsLocation[j]].top+(destCreatureRect[localEffectsLocation[j]].bottom-14-destCreatureRect[localEffectsLocation[j]].top)/2;

							if (localEffectsType[j]==kLightEllipse) //kLightRing)
								fx_DrawFlash(playBMP,localEffectsIndex[j],sx,destCreatureRect[localEffectsLocation[j]].bottom-30);

							//tb_RenderLightRingEllipse(playBMP, sx, sy+14, localEffectsMisc[j], (localEffectsMisc[j]/2), 50, 0, 50);

							if (localEffectsType[j]==kFireField)
								fx_DrawFire(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left, destRect[localEffectsDestination[j]].top);

							if (localEffectsType[j]==kSmallFire)
								fx_DrawFire(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left, destRect[localEffectsDestination[j]].top);
							
							

							if (localEffectsType[j]==kTempleFire)
								fx_DrawFire(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left+16, destRect[localEffectsDestination[j]].top-2);

							if (localEffectsType[j]==kLightExplosion)
								fx_DrawFlash(playBMP,localEffectsIndex[j],cx,cy);
							//Dave
							
							//if (localEffectsType[j]==kExplosion2)
							//	fx_CreateParticleExplosion(playBMP,localEffectsIndex[j],cx,cy);
								
							if (localEffectsType[j]==kSparkle)
								fx_DrawSparkles(playBMP,localEffectsIndex[j],cx,cy+10);
							//Dave 12-15-2012
							if (localEffectsType[j]==kSparkle2)
								fx_DrawSparkles(playBMP,localEffectsIndex[j],cx,cy+10);
							if (localEffectsType[j]==kSparkle3)
								fx_DrawSparkles(playBMP,localEffectsIndex[j],cx,cy);
							if (localEffectsType[j]==kSparkle4)
								fx_DrawSparkles(playBMP,localEffectsIndex[j],cx,cy);
							if (localEffectsType[j]==kHealFountain)
								fx_DrawParticleHose(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left+29, destRect[localEffectsDestination[j]].top+0);
						
							if (localEffectsType[j]==kSteamFX)
								fx_DrawFire(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left, destRect[localEffectsDestination[j]].top-30);
							
							if (localEffectsType[j]==kWaterFountain)
								fx_DrawParticleHose(playBMP,localEffectsIndex[j],destRect[localEffectsDestination[j]].left+29, destRect[localEffectsDestination[j]].top+24);

							if (localEffectsType[j]==kLightningBoltX)
							{
								gr_GetCreatureRect(localEffectsLocation[j], &s);
								tb_RenderLightning(playBMP, destCreatureRect[localEffectsLocation[j]].left, destCreatureRect[localEffectsLocation[j]].top, destCreatureRect[localEffectsLocation[j]].right, destCreatureRect[localEffectsLocation[j]].bottom, 65535, 40000, 40000, 4, true);
								tb_RenderLightning(playBMP, destCreatureRect[localEffectsLocation[j]].right, destCreatureRect[localEffectsLocation[j]].top, destCreatureRect[localEffectsLocation[j]].left, destCreatureRect[localEffectsLocation[j]].bottom, 65535, 40000, 40000, 4, true);
							}

							//if (localEffectsType[j]>=kFireballX && localEffectsType[j]<kFireballX2)
							if (localEffectsType[j]==kFireballX)
								fx_DrawFireball(playBMP, fxFrames[kFireballX], localEffectsFrame[j], destRect[localEffectsDestination[j]].left, destRect[localEffectsDestination[j]].top);

							if (localEffectsType[j]==kFireballX2)
								fx_DrawFireball(playBMP, fxFrames[kFireballX2], localEffectsFrame[j], cx, cy+14);

							if (localEffectsType[j]==kFireblastX)
								fx_Fireblast(playBMP, fxFrames[kFireblastX], localEffectsFrame[j], cx, cy);
							
							if (localEffectsType[j]==kCreatureSummonX)
								fx_CreatureSummon(playBMP, fxFrames[kCreatureSummonX], localEffectsFrame[j], cx, cy);
								
							if (localEffectsType[j]==kSpatSwordX)
								fx_SpatSword(playBMP, fxFrames[kSpatSwordX], localEffectsFrame[j], cx, cy);
							
							if (localEffectsType[j]==kSpatSwordX)
								fx_SpatAxe(playBMP, fxFrames[kSpatSwordX], localEffectsFrame[j], cx, cy);
							
							if (localEffectsType[j]==kFBFieldX)
								fx_FBField(playBMP, fxFrames[kFBFieldX], localEffectsFrame[j], cx, cy);
							
							if (localEffectsType[j]==kBrackenRootX)
								fx_BrackenRoot(playBMP, fxFrames[kBrackenRootX], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kMMX)
								fx_MagicMissile(playBMP, fxFrames[kMMX], localEffectsFrame[j], destRect[localEffectsDestination[j]].left, destRect[localEffectsDestination[j]].top);

							if (localEffectsType[j]==kMMX2)
								fx_MagicMissile(playBMP, fxFrames[kMMX2], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kFireblast3X)
								fx_Fireblast3(playBMP, fxFrames[kFireblast3X], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kFireblast4X)
								fx_Fireblast4(playBMP, fxFrames[kFireblast4X], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kFireblast5X)
								fx_Fireblast5(playBMP, fxFrames[kFireblast5X], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kMassHealX)
								fx_MassHeal(playBMP, fxFrames[kMassHealX], localEffectsFrame[j], cx, cy+15);

							if (localEffectsType[j]==kDomeX || localEffectsType[j]==kDome2X)
								fx_Dome(playBMP, localEffectsType[j], fxFrames[localEffectsType[j]], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kExplosionX)
								fx_Explosion(playBMP, fxFrames[kExplosionX], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kBuffStrX || localEffectsType[j]==kBuffAgiX || localEffectsType[j]==kBuffMindX || localEffectsType[j]==kBuffMRX || localEffectsType[j]==kBuffRDX || localEffectsType[j]==kParaFX)
								fx_Buff(playBMP, localEffectsType[j], fxFrames[localEffectsType[j]], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kResX)
								fx_Res(playBMP, fxFrames[kResX], localEffectsFrame[j], cx, cy);

							if (localEffectsType[j]==kLightningBoltReverse)
							{
								gr_GetCreatureRect(localEffectsLocation[j], &s);
								tb_RenderLightning(playBMP, cx, cy, destRect[localEffectsLocation[j]].left+29+(tb_Rnd(0,50)-25), destRect[localEffectsLocation[j]].top+12+(tb_Rnd(0,50)-25), 65535, 40000, 40000, 4, true);
							}

							if (localEffectsType[j]==kLightningBolt)
							{
								tb_RenderLightning(playBMP, cx+(tb_Rnd(0,50)-25), cy+(tb_Rnd(0,50)-25), destRect[localEffectsLocation[j]].left+29, destRect[localEffectsLocation[j]].top+12, 65535, 40000, 40000, 4, true);
							}

						}
					}

			//---------------------------------------------------------------------------------------


		}

	s=playRect;

	tb_OffsetRect(&s,xOff,yOff);

	// -- Render light effects --

	if (true)
		for (i=0;i<lightSourcePtr;i++)
			tb_RenderLightSource(playBMP, lightSources[i].h, lightSources[i].v, lightSourcesSize[i], gameTime);

	// --------------------------

	tb_DarkenBitmap(playBMP, &s, gameTime);

	//--------------------------- show player names

	tb_SetFont(playBMP, kFontIDGeneva);

	tb_SetFontSize(playBMP, 9);

	tb_SetFontStyle(playBMP, tb_Plain);

	for (i=0;i<kMaxScreenElements;i++)
		if (state[i]!=0)
		{
			if (namesOn)
				if (state[i]!=kHuman && morphed[i]==false)
				{

					if (state[i]==kNPCHuman || state[i]==kNPCHumanFemale)
					{
						if (human[i][18]==kNPCGuard)
							strcpy(creatureName,"Guard");
						else if (human[i][18]==kNPCBrigand)
							strcpy(creatureName,"Brigand");
						else if (human[i][18]==kNPCMercenary)
							strcpy(creatureName,"Mercenary");
						//Dave
						else if (human[i][18]==kNPCPrivateer)
							strcpy(creatureName,"Privateer");
						else if (human[i][18]==kNPCDwarf1)
							strcpy(creatureName,"Brigand");
						else
							strcpy(creatureName,"Human");
					}
					else
						tx_GetCreatureName(state[i], creatureName);

					txOff=0;

					tyOff=0;

					if (i==90) { txOff=xOff; tyOff=yOff; }
					else if (creatureDirection[i]==0) { txOff=0;    tyOff=-24; }
					else if (creatureDirection[i]==1) { txOff=-16;  tyOff=-16; }
					else if (creatureDirection[i]==2) { txOff=-24;  tyOff=0;   }
					else if (creatureDirection[i]==3) { txOff=16;   tyOff=-16; }
					else if (creatureDirection[i]==5) { txOff=-16;  tyOff=16;  }
					else if (creatureDirection[i]==6) { txOff=24;   tyOff=0;   }
					else if (creatureDirection[i]==7) { txOff=16;   tyOff=16;  }
					else if (creatureDirection[i]==8) { txOff=0;    tyOff=24;  }

					stringWidth		=	tb_StringWidth(playBMP, creatureName)/2;

					stringXPosition	=	(((destRect[i].right-destRect[i].left)/2)+destRect[i].left)-stringWidth+txOff;

					if (state[i]==kRat)
						stringYPosition = destRect[i].bottom-8+tyOff;
					else
						stringYPosition = destRect[i].bottom-17+tyOff;

					stringLeft=stringXPosition;

					stringRight=stringXPosition+(stringWidth*2);

					if (stringLeft<s.left)
						stringXPosition=stringXPosition+(s.left-stringLeft)+3;

					if (stringRight>s.right)
						stringXPosition=stringXPosition-(stringRight-s.right)-3;

					if (stringYPosition<s.top)
						stringYPosition=s.top+15;

					if (stringYPosition>s.bottom)
						stringYPosition=s.bottom-15;

					tb_SetRect(&tr,stringXPosition-2,stringYPosition-8,stringXPosition+tb_StringWidth(playBMP, creatureName)+1,stringYPosition+1);

					tb_SetForeColor(playBMP, 4096, 4096, 4096);

					if (darkTextBacks)
						tb_PaintRect(playBMP,&tr);
					else
						tb_DarkenTextBackground(playBMP, &tr);

					tb_SetForeColor(playBMP,65535,65535,65535);
					
					//if (cursed[i])
						//tb_SetForeColor(playBMP, 28784,52171,65021);
					
					if (isPet[i])
						tb_SetForeColor(playBMP,0,39321,65535);

					if (questChar[i])  //GM pets
						tb_SetForeColor(playBMP,0,65535,0);

					if (creatureAttacking[i])
						tb_SetForeColor(playBMP,65535,0,0);
					
					
					//Dave morph name color
						//if (localClass==2 || localClass>=7)
						//	if (morphed[i])
								//tb_SetForeColor(playBMP,0,65535,0);

					tb_DrawString(playBMP, stringXPosition, stringYPosition, creatureName);

				}
				else if (state[i]==kHuman || morphed[i])
				{
					if (human[i][14]!=0)
						if (strcmp(playerNames[human[i][14]],"")==0)
						{
							nw_SendRequestName(human[i][14]);
							strcpy(playerNames[human[i][14]],"request");
						}
						else if (strcmp(playerNames[human[i][14]],"request")!=0)
						{
							if (playerOnline[i])
								tb_SetFontStyle(playBMP, tb_Plain);
							else
								tb_SetFontStyle(playBMP, tb_Italic);

							// -- AFK ---------------------------------------------------------
							tb_SetRect(&a,0,0,21,21);

							if (afk[i])
							{
								tb_EraseRect(humanAfkBMP, &a);
								tb_EraseRect(humanAfkMaskBMP, &a);

								tb_CopyMaskBitmap(armorGridBMP, armorGridMaskBMP, humanAfkBMP, &humanAFKTag, &humanAFKTag, &a);
								tb_CopyMaskBitmap(armorGridMaskBMP, armorGridMaskBMP, humanAfkMaskBMP, &humanAFKTag, &humanAFKTag, &a);
								tb_FastCopyMaskBitmap(humanAfkBMP, humanAfkMaskBMP, playBMP, &a, &a, &afkTag[i]);
							}

							// -----------------------------------------------------------------

							//if (afk[i])
							//strcpy(creatureName, "<afk>");


							//else
							strcpy(creatureName, "");

							if (creatureHealth[i]==0)
								strcat(creatureName,"Ghost of ");

							//else
							//strcat(creatureName,"");

							strcat(creatureName,playerNames[human[i][14]]);

							txOff=0;

							tyOff=0;

							if (i==90) { txOff=xOff; tyOff=yOff; }
							else if (creatureDirection[i]==0) { txOff=0;    tyOff=-24; }
							else if (creatureDirection[i]==1) { txOff=-16;  tyOff=-16; }
							else if (creatureDirection[i]==2) { txOff=-24;  tyOff=0;   }
							else if (creatureDirection[i]==3) { txOff=16;   tyOff=-16; }
							else if (creatureDirection[i]==5) { txOff=-16;  tyOff=16;  }
							else if (creatureDirection[i]==6) { txOff=24;   tyOff=0;   }
							else if (creatureDirection[i]==7) { txOff=16;   tyOff=16;  }
							else if (creatureDirection[i]==8) { txOff=0;    tyOff=24;  }

							stringWidth		=	tb_StringWidth(playBMP, creatureName)/2;

							stringXPosition	=	(((destRect[i].right-destRect[i].left)/2)+destRect[i].left)-stringWidth+txOff;;
							stringYPosition = destRect[i].bottom-17+tyOff;

							stringLeft=stringXPosition;
							stringRight=stringXPosition+(stringWidth*2);

							if (stringLeft<s.left)
								stringXPosition=stringXPosition+(s.left-stringLeft)+3;

							if (stringRight>s.right)
								stringXPosition=stringXPosition-(stringRight-s.right)-3;

							if (stringYPosition<s.top)
								stringYPosition=s.top+15;

							if (stringYPosition>s.bottom)
								stringYPosition=s.bottom-15;

							tb_SetRect(&tr,stringXPosition-2,stringYPosition-8,stringXPosition+tb_StringWidth(playBMP, creatureName)+1,stringYPosition+1);

							tb_SetForeColor(playBMP, 4096, 4096, 4096);

							if (darkTextBacks)
								tb_PaintRect(playBMP,&tr);
							else
								tb_DarkenTextBackground(playBMP, &tr);

							tb_SetForeColor(playBMP,65535,65535,65535);
							
							//if (cursed[i])
								//tb_SetForeColor(playBMP, 28784,52171,65021);
							
							//Morphed name color Dave 11-12-2012
							//if (localClass<=6 && morphed[i])
								//tb_SetForeColor(playBMP,0,39321,65535);
							
							if (questChar[i])
								tb_SetForeColor(playBMP,0,65535,0);

							if (gmChar[i])
								tb_SetForeColor(playBMP,52428,65535,0);

							if (i==90 && colorOwnName) //Self name Color Own Name
								tb_SetForeColor(playBMP, 15420, 65535, 24415);

							if (stealthChar[i])
								tb_SetForeColor(playBMP,65535,0,65535);

							if (creatureAttacking[i])
								tb_SetForeColor(playBMP,65535,0,0);
							
							
							
							tb_DrawString(playBMP, stringXPosition, stringYPosition, creatureName);

							if (!playerOnline[i])
								tb_SetFontStyle(playBMP, tb_Plain);

						}
				}
		}

	tb_SetFont(playBMP, chatFont);

	tb_SetFontSize(playBMP, 14);
	tb_SetFontStyle(playBMP, 0);

	for (j=0;j<kMaxChatSlots;j++)
		if (chatDecay[j]>0)
			if (chatDecay[j]<TickCount())
			{
				strcpy(chatString[j],"");
				chatCreatureID[j]=0;
				chatDecay[j]=0;
			}

	for (i=0;i<kMaxScreenElements;i++)
		if (state[i]!=0)
		{
			thisCreatureChat=0;

			for (j=0;j<kMaxChatSlots;j++)
				if (chatCreatureID[j]==creatureID[i])
					thisCreatureChat=j;

			if (thisCreatureChat==0)
				continue;

			txOff=0;

			tyOff=0;

			if (i==90) { txOff=xOff; tyOff=yOff; }
			else if (creatureDirection[i]==0) { txOff=0;    tyOff=-24; }
			else if (creatureDirection[i]==1) { txOff=-16;  tyOff=-16; }
			else if (creatureDirection[i]==2) { txOff=-24;  tyOff=0;   }
			else if (creatureDirection[i]==3) { txOff=16;   tyOff=-16; }
			else if (creatureDirection[i]==5) { txOff=-16;  tyOff=16;  }
			else if (creatureDirection[i]==6) { txOff=24;   tyOff=0;   }
			else if (creatureDirection[i]==7) { txOff=16;   tyOff=16;  }
			else if (creatureDirection[i]==8) { txOff=0;    tyOff=24;  }

			stringWidth		=	tb_StringWidth(playBMP, chatString[thisCreatureChat])/2;

			stringXPosition	=	(((destRect[i].right-destRect[i].left)/2)+destRect[i].left)-stringWidth+txOff;
			stringYPosition = destRect[i].top-25+tyOff;

			stringLeft=stringXPosition;
			stringRight=stringXPosition+(stringWidth*2);

			if (stringLeft<s.left)
				stringXPosition=stringXPosition+(s.left-stringLeft)+3;

			if (stringRight>s.right)
				stringXPosition=stringXPosition-(stringRight-s.right)-3;

			if (stringYPosition<s.top)
				stringYPosition=s.top+15;

			if (stringYPosition>s.bottom)
				stringYPosition=s.bottom-15;

			tb_SetRect(&tr,stringXPosition-2,stringYPosition-maxAscent+1,stringXPosition+tb_StringWidth(playBMP, chatString[thisCreatureChat])+2,stringYPosition+maxDescent);

			tb_SetForeColor(playBMP, 4096, 4096, 4096);

			if (darkTextBacks)
				tb_PaintRect(playBMP,&tr);
			else
				tb_DarkenTextBackground(playBMP, &tr);

			if (state[i]==kHuman)
				tb_SetForeColor(playBMP,65535,65535,65535);
			else
				tb_SetForeColor(playBMP,55295,55295,55295);

			tb_DrawString(playBMP, stringXPosition, stringYPosition, chatString[thisCreatureChat]);
		}

	//---------------------------


	if ((drawHealthBars)	|| (localTarget<255) || (standingOnContainer))// ---------- draw health bars -------------
		for (i=0;i<kMaxScreenElements;i++)
			if (state[i]!=0)
			{
				if ((creatureHealth[i]==0) && (state[i]==kHuman))
					continue;

				CheckKeys(false,0);

				if (localTarget==i)
				{
					hitsRect=healthBar[i];
					hitsRect.right=hitsRect.left+creatureHealth[i];

					if (state[i]==kRat)
					{
						tb_OffsetRect(&hitsRect,0,10);
						tb_OffsetRect(&healthBar[i],0,10);
					}

					r=targetRect;

					tb_OffsetRect(&r,healthBar[i].left-12,healthBar[i].top-3);
					tb_FastCopyMaskBitmap(targetBMP, targetMaskBMP, playBMP, &targetRect, &targetRect, &r);
				}

				if ((i==90) && (standingOnContainer))
				{
					hitsRect=healthBar[i];
					hitsRect.right=hitsRect.left+creatureHealth[i];

					if (state[i]==kRat)
					{
						tb_OffsetRect(&hitsRect,0,10);
						tb_OffsetRect(&healthBar[i],0,10);
					}

					r=targetRect;

					tb_OffsetRect(&r,healthBar[i].left-12,healthBar[i].top-3);
					tb_FastCopyMaskBitmap(getBMP, getMaskBMP, playBMP, &getRect, &getRect, &r);
				}

				if (drawHealthBars)
					if (creatureHealth[i]>0)
					{
						//health
						hitsRect=healthBar[i];
						hitsRect.right=hitsRect.left+creatureHealth[i];

						if (creatureHealth[i]>5)
						{
							if (poisoned[i])
								tb_SetForeColor(playBMP, 65535, 0, 65535);
							else if (regenerating[i])
								tb_SetForeColor(playBMP,65535,39578,0);
							else
								tb_SetForeColor(playBMP, 0, 65535, 0);
						}
						else
							tb_SetForeColor(playBMP, 65535, 0, 0);

						if (state[i]==kRat)
						{
							tb_OffsetRect(&hitsRect,0,10);
							tb_OffsetRect(&healthBar[i],0,10);
						}

						hitsRect.bottom=hitsRect.bottom-1;	// gs

						healthBar[i].bottom=healthBar[i].bottom-1;	// gs

						tb_PaintRect(playBMP,&hitsRect);
						tb_SetForeColor(playBMP, 0, 0,0);
						tb_FrameRect(playBMP,&healthBar[i]);
						
						
						if (i==90) //secondary bar/self only/no GMs original = if (i==90 && (!questChar[90] && localClass<7))
						{
							//mp/stam
							hitsRect=secondaryBar[i];	
							
							tmpClass=(localClass>10)?localClass-10:localClass;							

							if (tmpClass==1 || tmpClass==2 || tmpClass==6) //magic type class - show MP
							{
								barWidth=(((float)localMagicPoints/(float)localIntelligence)*30.0>30)?30:((float)localMagicPoints/(float)localIntelligence)*30.0;
								hitsRect.right=hitsRect.left+(int)barWidth;
								
								if (cursed[i])
									tb_SetForeColor(playBMP, 65535, 0, 65535);
								else
									tb_SetForeColor(playBMP, 8481,13621,64507); //Dave color change 51914,56283,65278 -- orig blue = 8224,13364,64507
							}								

							else //melee type class - show Stamina
							{
								barWidth=(((float)localStamina/(float)localDexterity)*30.0>30)?30:((float)localStamina/(float)localDexterity)*30.0;
								hitsRect.right=hitsRect.left+(int)barWidth;
								if (cursed[i])
									tb_SetForeColor(playBMP, 65535, 0, 65535);
								else
									tb_SetForeColor(playBMP, 65278,64507,14392); 
							}

							hitsRect.bottom=hitsRect.bottom-1;	// gs

							secondaryBar[i].bottom=secondaryBar[i].bottom-1;	// gs

							tb_PaintRect(playBMP,&hitsRect);
							tb_SetForeColor(playBMP, 0, 0,0);
							tb_FrameRect(playBMP,&secondaryBar[i]);
						}
						
					}
			}

	for (i=0;i<8;i++)
		if (theTextActive[i])
		{
			CheckKeys(false,0);

			tb_SetRect(&tr,27+xOff-2,415+(i*16)+yOff-maxAscent+1,27+xOff+tb_StringWidth(playBMP, theText[i])+2,415+(i*16)+yOff+maxDescent); //9 line : y = 383
			tb_SetForeColor(playBMP, 4096, 4096, 4096);

			if (darkTextBacks)
				tb_PaintRect(playBMP,&tr);
			else
				tb_DarkenTextBackground(playBMP, &tr);

			if (theTextSystem[i])
				tb_SetForeColor(playBMP,65535,0,0);
			else
				tb_SetForeColor(playBMP,65535,65535,65535);

			tb_DrawString(playBMP, 27+xOff, 415+(i*16)+yOff, theText[i]);

			if (TickCount()>theTextDecay[i])
			{
				strcpy(theText[i],"");
				theTextActive[i]	=	false;
				theTextDecay[i]		=	0;
				theTextSystem[i]	=	false;
			}
		}

	if ((doorCount>prevDoorCount) && (prevDoorCount!=-1))
		tb_PlaySound(141);

	prevDoorCount=doorCount;



	if (!inDungeon)
	{
		if (gr_SnowAt(mapToScreenRow[90]+playerRow, mapToScreenCol[90]+playerCol))
			treeCount=0;

		if (playerRow>800 && playerRow<1170 && playerCol>1900 && playerCol<1972) //Aborek
			{
			if (tb_Rnd(0,1000)<10)
				tb_PlaySound(tb_Rnd(225,227));
				
			}
	/*	
		else if (playerRow>1949 && playerRow<1962 && playerCol>1123 && playerCol<1152) //pipes //Dave added 11-19-2010
			{
			if (tb_Rnd(0,1000)<10)
				tb_PlaySound(tb_Rnd(336,338));
			
			}
		*/
		else if (tb_Rnd(0,3000)<treeCount)
			{
				
			
			if ((gameTime==0) || (gameTime==3))
				tb_PlaySound(tb_Rnd(151,157));
			else
				tb_PlaySound(tb_Rnd(158,165));
			}
	}

	//if (cursed)
	//	tb_SetForeColor(playBMP, 28784,52171,65021);
	
	if (playerDead)
		tb_GrayBMP(playBMP, &s);

	if (drawOnScreen)
		tb_CopyBitmap(playBMP,tbWindow,&s,&playScreenRect,false);
	else
		tb_CopyBitmap(playBMP,osBMP,&s,&playOSScreenRect,false);


	if ((!((xOff==0) && (yOff==0))) || (updateMapView))
	{
		gr_RenderMapWindow(true);
		updateMapView=false;
	}
	else if (thisIsFirstRefresh)
	{
		gr_RenderMapWindow(true);
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
		thisIsFirstRefresh=false;
	}

	//-------------------------------------------------------------

	if (((standingOnContainer) && ((previousPlayerRow!=playerRow) || (previousPlayerCol!=playerCol)) && (windowVisible[kPackContentsWindow])))
	{
		in_ClearPickUpPack();
		gr_CloseFloatingWindow(tbPackContentsWindow);
	}
	else if ((!standingOnContainer) && (windowVisible[kPackContentsWindow]))
	{
		in_ClearPickUpPack();
		gr_CloseFloatingWindow(tbPackContentsWindow);
	}

	//-------------------------------------------------------------

	previousPlayerRow=playerRow;

	previousPlayerCol=playerCol;

	totalTime=TickCount()-timeStart;

	if (totalTime<2)
		totalTime=2;
	else if (totalTime>15)
		totalTime=15;

	nextRender	=	TickCount()+totalTime;//2;

}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderTextLine(void)
// ----------------------------------------------------------------- //

{
	char              theText[kStringLength];
	int               i;
	int               cursorX;

// TXNUpdate(textObject);

	if (strlen(playerText)>(kStringLength-2)) // mod 7/31
		return;

	if (strcmp(playerText,"")==0)
		insertionPoint=0;

	tb_SetFont(textLineBMP, kFontIDGeneva);

	tb_SetFontSize(textLineBMP, 10);

	tb_SetFontStyle(textLineBMP, tb_Bold);

	strcpy(theText,"");

	for (i=0;i<insertionPoint;i++)
	{
		theText[i]=playerText[i];
		theText[i+1]='\0';
	}

	cursorX=tb_StringWidth(textLineBMP,theText)+2;

	strcpy(theText,"");
	strcpy(theText,playerText);
	//strcat(theText,"_");

	//tb_SetFont(textLineBMP, chatFont);
	//tb_SetFontSize(textLineBMP, 14);
	//tb_SetFontStyle(textLineBMP, 0);

	tb_SetForeColor(textLineBMP,49152,49152,49152);
	tb_PaintRect(textLineBMP,&textLineRect);
	tb_SetForeColor(textLineBMP,0,0,0);
	tb_Line(textLineBMP, 0, 0, 512, 0, 1);
	tb_DrawString(textLineBMP, 2, 15, theText);

	if (cursorBlink)
		tb_Line(textLineBMP, cursorX, 3, cursorX, 18, 1);

	tb_CopyBitmap(textLineBMP,tbWindow,&textLineRect,&textLineScreenRect,false);

	if (TickCount()>cursorBlinkTime)	// test comment out 11/3
	{
		cursorBlinkTime=TickCount()+20;
		cursorBlink=!cursorBlink;
	}

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
TBoolean gr_ClickedOnCreatureRegion(TPoint pt, int i)
// ----------------------------------------------------------------- //

{
	TRect							s;
	TRect							d;
	int								sH;
	int								sW;
	int								dH;
	int								dW;
	int								row;
	int								col;
	int								dir;
	RgnHandle		      clickCreatureRgn;
	int               xOff;
	int               yOff;


	xOff=prevxOff;
	yOff=prevyOff;

	if (state[i]!=0)	// creatures
	{

		clickCreatureRgn=NewRgn();

		row=mapToScreenRow[i]+playerRow;
		col=mapToScreenCol[i]+playerCol;

		gr_GetCreatureRect(i, &s);

		sH=s.bottom-s.top;
		sW=s.right-s.left;

		d=destRect[i];
		dH=d.bottom-d.top;
		dW=d.right-d.left;
		
	
		
		if ((state[i]==kRedDragon) || (state[i]==kIceDragon) || (state[i]==kPinkDragon) || (state[i]==kBlackDragon) || (state[i]==kForestDragon) || (state[i]==kSandDragon))
		{
			d.bottom=d.top+sH;
			d.right=d.left+sW;
			dir=creatureFacing[i];

			if ((dir>9) && (dir<18))
				dir=dir-10;

			if (dir==0) tb_OffsetRect(&d,-63,-80);
			else if (dir==1) tb_OffsetRect(&d,-78,-78);
			else if (dir==2) tb_OffsetRect(&d,-95,-78);
			else if (dir==3) tb_OffsetRect(&d,-86,-56);
			else if (dir==4) tb_OffsetRect(&d,-60,-58);
			else if (dir==5) tb_OffsetRect(&d,-42,-60);
			else if (dir==6) tb_OffsetRect(&d,-38,-75);
			else if (dir==7) tb_OffsetRect(&d,-45,-99);
		}
		else if (state[i]==kTusker)
		{
			d.bottom=d.top+sH;
			d.right=d.left+sW;
			dir=creatureFacing[i];

			if ((dir>9) && (dir<18))
				dir=dir-10;

			if (dir==0) tb_OffsetRect(&d,-48,-80);
			else if (dir==1) tb_OffsetRect(&d,-63,-78);
			else if (dir==2) tb_OffsetRect(&d,-50,-70);
			else if (dir==3) tb_OffsetRect(&d,-65,-65);
			else if (dir==4) tb_OffsetRect(&d,-53,-60);
			else if (dir==5) tb_OffsetRect(&d,-40,-70);
			else if (dir==6) tb_OffsetRect(&d,-50,-82);
			else if (dir==7) tb_OffsetRect(&d,-55,-82);

		}
		else
		{
			if (sH!=dH)
				d.top=d.bottom-sH;

			d.right=d.left+sW;

			d.left=d.left-((sW-dW)/2);

			d.right=d.right-((sW-dW)/2);

			if ((state[i]==kSpider) || (state[i]==kFrostSpider) || (state[i]==kLavaSpider) || (state[i]==kSandSpider))
				tb_OffsetRect(&d,0,21);

			if ((state[i]==kGiant) || (state[i]==kHillGiant) || (state[i]==kFrostGiant))
				tb_OffsetRect(&d,0,5);
		}

		if (i==90) tb_OffsetRect(&d,xOff,yOff);
		else if (creatureDirection[i]==0) tb_OffsetRect(&d,0,-24);//-24
		else if (creatureDirection[i]==1) tb_OffsetRect(&d,-16,-16);
		else if (creatureDirection[i]==2) tb_OffsetRect(&d,-24,0);//-24
		else if (creatureDirection[i]==3) tb_OffsetRect(&d,16,-16);
		else if (creatureDirection[i]==5) tb_OffsetRect(&d,-16,16);
		else if (creatureDirection[i]==6) tb_OffsetRect(&d,24,0);//24
		else if (creatureDirection[i]==7) tb_OffsetRect(&d,16,16);
		else if (creatureDirection[i]==8) tb_OffsetRect(&d,0,24);//24

		if (state[i]==kHuman)
		{
			if (canSwimIn[clientMap[row][col]])
				gr_BuildHuman(i,true,true);
			else
				gr_BuildHuman(i,false,true);

			tb_CopyBitmap(humanBuilderMaskBMP, regionBMP, &humanBuilderRect, &d, false);

			BitMapToRegion (clickCreatureRgn,GetPortBitMapForCopyBits( gw[regionBMP] ));

			OffsetRgn(clickCreatureRgn,horizontalOffset+xOff-24,verticalOffset+yOff-24);
		}
		else if (state[i]==kNPCHuman || state[i]==kNPCHumanFemale)
		{
			if (canSwimIn[clientMap[row][col]])
				gr_BuildHuman(i,true,true);
			else
				gr_BuildHuman(i,false,true);

			tb_CopyBitmap(humanBuilderMaskBMP, regionBMP, &humanBuilderRect, &d, false);

			BitMapToRegion (clickCreatureRgn,GetPortBitMapForCopyBits( gw[regionBMP] ));

			OffsetRgn(clickCreatureRgn,horizontalOffset+xOff-24,verticalOffset+yOff-24);
		}
		else
		{
			if ((state[i]==kRedDragon) || (state[i]==kIceDragon) || (state[i]==kPinkDragon)  || (state[i]==kBlackDragon) || (state[i]==kForestDragon)  || (state[i]==kSandDragon))
				tb_CopyBitmap(greendragonMaskBMP, regionBMP, &s, &d, false);
			else
				tb_CopyBitmap(creatureGridMaskBMP[creatureInfo[state[i]].bmp], regionBMP, &s, &d, false);

			BitMapToRegion (clickCreatureRgn,GetPortBitMapForCopyBits( gw[regionBMP] ));

			OffsetRgn(clickCreatureRgn,horizontalOffset+xOff-24,verticalOffset+yOff-24);
		}

		tb_EraseRect(regionBMP,				&d);

	}
	else
		return (false);

	//tb_SetGWorld(tbWindow);
	if (PtInRgn(pt, clickCreatureRgn))
	{
		if (clickCreatureRgn!=NULL)
			DisposeRgn(clickCreatureRgn);

		//tb_RestorePort();
		return (true);
	}

	//tb_RestorePort();

	if (clickCreatureRgn!=NULL)
		DisposeRgn(clickCreatureRgn);

	return (false);


}

#endif


#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_CreateBitmaps(void)
// ----------------------------------------------------------------- //

{
	TRect					r;
	TRect					a;
	//PicHandle			testPict;


	//------------------------------------//
	// windows

	mapWindowBMP            = tb_CreateBitmap(&mapWindowRect, 240, kScreenDepth);
//  currentMapWindowsRect = mapWindowRect;
//  tb_OffsetRect(&currentMapWindowsRect,700,100);

// mainWindowBMP           = tb_CreateBitmap(&mainWindowRect, 239, kScreenDepth);
// currentMainWindowsRect = mainWindowRect;
// tb_OffsetRect(&currentMainWindowsRect,10,10);
	//------------------------------------//
	tb_SetRect(&r,0,0,357,564);
	bufferBMP								=	tb_CreateBitmap(&r, kNull, kScreenDepth);

	tb_SetRect(&textLineRect,0,0,512,21);
	tb_SetRect(&textLineScreenRect,0,512,512,512+21);
	textLineBMP							=	tb_CreateBitmap(&textLineRect, kNull, kScreenDepth);

	tb_SetRect(&questTextRect,0,0,342,106);
	questTextBMP						=	tb_CreateBitmap(&questTextRect, kNull, 1);

	chatBMP						      =	tb_CreateBitmap(&chatRect, 231, kScreenDepth);

	targetBMP								=tb_CreateBitmap(&targetRect, 216, kScreenDepth);
	targetMaskBMP						=tb_CreateBitmap(&targetRect, 249, kMaskDepth);

	getBMP								  =tb_CreateBitmap(&getRect, 128, kScreenDepth);
	getMaskBMP						  =tb_CreateBitmap(&getRect, 130, kMaskDepth);

	mapCenterBMP						=	tb_CreateBitmap(&mapCenterRect, 2230, kScreenDepth);
	mapCenterMaskBMP				=	tb_CreateBitmap(&mapCenterRect, 2231, kMaskDepth);
	mapCenterDestRect=mapCenterRect;
	//tb_OffsetRect(&mapCenterDestRect,22,27);
	tb_OffsetRect(&mapCenterDestRect,71,64);

	mapTrackingMaskBMP[0]				=	tb_CreateBitmap(&mapCenterRect, 2240, kMaskDepth);
	mapTrackingMaskBMP[1]				=	tb_CreateBitmap(&mapCenterRect, 2250, kMaskDepth);
	mapTrackingMaskBMP[2]				=	tb_CreateBitmap(&mapCenterRect, 2260, kMaskDepth);

	mapTrackingBMP[0]						=	tb_CreateBitmap(&mapCenterRect, 2241, kScreenDepth);
	mapTrackingBMP[1]						=	tb_CreateBitmap(&mapCenterRect, 2242, kScreenDepth);
	mapTrackingBMP[2]						=	tb_CreateBitmap(&mapCenterRect, 2243, kScreenDepth);
	mapTrackingBMP[3]						=	tb_CreateBitmap(&mapCenterRect, 2244, kScreenDepth);

	mapTrackingBMP[4]						=	tb_CreateBitmap(&mapCenterRect, 2251, kScreenDepth);
	mapTrackingBMP[5]						=	tb_CreateBitmap(&mapCenterRect, 2252, kScreenDepth);
	mapTrackingBMP[6]						=	tb_CreateBitmap(&mapCenterRect, 2253, kScreenDepth);
	mapTrackingBMP[7]						=	tb_CreateBitmap(&mapCenterRect, 2254, kScreenDepth);

	mapTrackingBMP[8]						=	tb_CreateBitmap(&mapCenterRect, 2261, kScreenDepth);
	mapTrackingBMP[9]						=	tb_CreateBitmap(&mapCenterRect, 2262, kScreenDepth);
	mapTrackingBMP[10]						=	tb_CreateBitmap(&mapCenterRect, 2263, kScreenDepth);
	mapTrackingBMP[11]						=	tb_CreateBitmap(&mapCenterRect, 2264, kScreenDepth);

	tb_SetRect(&regionBMPRect,0,0,560,560);
	regionBMP									=	tb_CreateBitmap(&regionBMPRect, kNull, 1);
	tb_EraseRect(regionBMP,	&regionBMPRect);

	//testPict=tb_GetPicture(128);

	tb_SetRect(&r,0,0,256,256);
	creatureColorizerBMP			=	tb_CreateBitmap(&r, kNull, kScreenDepth);

	tb_SetRect(&humanBuilderRect,0,0,63,103);
	humanBuilderBMP						=	tb_CreateBitmap(&humanBuilderRect, kNull, kScreenDepth);
	humanBuilderMaskBMP				=	tb_CreateBitmap(&humanBuilderRect, kNull, kMaskDepth);
	humanBuilderWorkSpaceBMP	= tb_CreateBitmap(&humanBuilderRect, kNull, kScreenDepth);

	tb_SetRect(&a,0,0,21,21);
	humanAfkBMP 	= tb_CreateBitmap(&a, kNull, kScreenDepth);
	humanAfkMaskBMP = tb_CreateBitmap(&a, kNull, kScreenDepth);


	tb_SetRect(&osRect,0,0,800,600);
	osBMP									=	tb_CreateBitmap(&osRect, kNull, kScreenDepth);
	//tb_DrawPictureToBitmap(osBMP, 130, 0, 0, &r);

	tb_SetRect(&playRect,0,0,562,562);
	playBMP								=	tb_CreateBitmap(&playRect, kNull, kScreenDepth);
	tb_SetRect(&playRect,24,24,536,536);
	tb_SetForeColor(playBMP, 0, 0, 0);

	GetFNum("\pApple Chancery",&chatFont);

	if (chatFont==0)
	{
		tb_SetFont(playBMP, kFontIDGeneva);
		tb_SetFontSize(playBMP, 14);
		tb_SetFontStyle(playBMP, tb_Bold);
	}
	else
	{
		tb_SetFont(playBMP, chatFont);
		tb_SetFontSize(playBMP, 14);
		tb_SetFontStyle(playBMP, 0);
	}

	tb_GetFontInfo(playBMP, &maxAscent, &maxDescent);

	currentMainBitmap			= 5128;//128;
	smallGridBMP					=	tb_CreateBitmap(&r, 5128, kScreenDepth);
	smallGridMaskBMP			=	tb_CreateBitmap(&r, 5129, kMaskDepth);

	//humanGridBMP					=	tb_CreateBitmap(&r, 5144, kScreenDepth);
	//humanGridMaskBMP			=	tb_CreateBitmap(&r, 5145, kMaskDepth);
	weaponGridBMP					=	tb_CreateBitmap(&r, 5144, 8);// kScreenDepth); // gs test
	weaponGridMaskBMP			=	tb_CreateBitmap(&r, 5145, kMaskDepth);

	womanGridBMP					=	tb_CreateBitmap(&r, 5225, 8);//kScreenDepth);
	womanGridMaskBMP			=	tb_CreateBitmap(&r, 5226, kMaskDepth);

	manGridBMP					  =	tb_CreateBitmap(&r, 5227, 8);//kScreenDepth); // gs test
	manGridMaskBMP			  =	tb_CreateBitmap(&r, 5228, kMaskDepth);

	armorGridBMP					=	tb_CreateBitmap(&r, 5180, 8); // kScreenDepth); // gs test
	armorGridMaskBMP			=	tb_CreateBitmap(&r, 5181, kMaskDepth);

	headGridBMP					  =	tb_CreateBitmap(&r, 5184, 8); // kScreenDepth); // gs test
	headGridMaskBMP			  =	tb_CreateBitmap(&r, 5185, kMaskDepth);

	currentDungeonBitmap	= 5221;
	havenGridBMP					=	tb_CreateBitmap(&r, 5148, kScreenDepth);
	havenGridMaskBMP			=	tb_CreateBitmap(&r, 5149, kMaskDepth);

	dungeonGridBMP				=	tb_CreateBitmap(&r, 5221, kScreenDepth);
	dungeonGridMaskBMP		=	tb_CreateBitmap(&r, 5222, kMaskDepth);

	caveGridBMP						=	tb_CreateBitmap(&r, 5235, kScreenDepth);
	caveGridMaskBMP				=	tb_CreateBitmap(&r, 5236, kMaskDepth);

	iceGridBMP						=	tb_CreateBitmap(&r, 5217, kScreenDepth);	// gs 9/25
	//iceGridBMP						=	tb_CreateBitmap(&r, 5148, kScreenDepth);	// gs 9/25
	iceGridMaskBMP				=	tb_CreateBitmap(&r, 5239, kMaskDepth);		// gs 9/25

	creatureGridBMP[0]		=	tb_CreateBitmap(&r, 5150, 8);//kScreenDepth);
	creatureGridMaskBMP[0]=	tb_CreateBitmap(&r, 5151, kMaskDepth);

	creatureGridBMP[1]		=	tb_CreateBitmap(&r, 5231, kScreenDepth);
	creatureGridMaskBMP[1]=	tb_CreateBitmap(&r, 5232, kMaskDepth);

	creatureGridBMP[2]		=	tb_CreateBitmap(&r, 5233, kScreenDepth);
	creatureGridMaskBMP[2]=	tb_CreateBitmap(&r, 5234, kMaskDepth);

	creatureGridBMP[3]		=	tb_CreateBitmap(&r, 5500, kScreenDepth);
	creatureGridMaskBMP[3]=	tb_CreateBitmap(&r, 5501, kMaskDepth);

	creatureGridBMP[4]		=	tb_CreateBitmap(&r, 5550, kScreenDepth);
	creatureGridMaskBMP[4]=	tb_CreateBitmap(&r, 5551, kMaskDepth);
		
	creatureGridBMP[5]		=	tb_CreateBitmap(&r, 8080, kScreenDepth);
	creatureGridMaskBMP[5]=	tb_CreateBitmap(&r, 8081, kMaskDepth);

	greendragonBMP				=	tb_CreateBitmap(&r, 5218, 8); // kScreenDepth);		 // gs test
	greendragonMaskBMP		=	tb_CreateBitmap(&r, 5219, kMaskDepth);

	townGridBMP						=	tb_CreateBitmap(&r, 5173, kScreenDepth);
	townGridMaskBMP				=	tb_CreateBitmap(&r, 5174, kMaskDepth);

	containerBMP					=	tb_CreateBitmap(&r, 5182, kScreenDepth);
	containerMaskBMP			=	tb_CreateBitmap(&r, 5183, kMaskDepth);

//	statBMP								=	tb_CreateBitmap(&statRect, 128, kScreenDepth);
	scrollBMP							=	tb_CreateBitmap(&scrollRect, 133, kScreenDepth);

	statsBMP						  =	tb_CreateBitmap(&statsRect, 2411, kScreenDepth);

	CalendarBMP							=	tb_CreateBitmap(&CalendarWindowRect, 2500, kScreenDepth);

	tb_SetRect(&mapViewRect,0,0,150,150);
	mapViewBMP						=	tb_CreateBitmap(&mapViewRect, 0, kScreenDepth);
	tb_SetForeColor(playBMP, 16384, 16384, 16384);
	tb_PaintRect(mapViewBMP,&mapViewRect);
	mapViewMaskBMP				=	tb_CreateBitmap(&r, 134, kMaskDepth);

	chessBMP					    =	tb_CreateBitmap(&r, 218, kScreenDepth);
	chessMaskBMP			    =	tb_CreateBitmap(&r, 219, kMaskDepth);
	chessSelectBMP				=	tb_CreateBitmap(&r, 220, kScreenDepth);

	tracksBMP					=	tb_CreateBitmap(&r, 5300, kScreenDepth);
	tracksMaskBMP				=	tb_CreateBitmap(&r, 5301, kScreenDepth);

	//tb_DrawPictureToBitmap(dungeonGridBMP, 5224, 0, 0, &r);
	//tb_DrawPictureToBitmap(dungeonGridMaskBMP, 5222, 0, 0, &r);


}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderChatDisplayWindow(int buttonDown)
// ----------------------------------------------------------------- //

{
	int						i;
	TRect					r;
	//TRect					d;
	int						line			=	1;
	int						lineSpace	= 13;
	float         pos;

	if (windowVisible[kChatWindow]==false)
		return;

	tb_SetRect(&r,0,0,785-547,582-18);

	tb_CopyBitmap(chatBMP, bufferBMP, &chatRect, &r, false);

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);

	tb_SetFontStyle(bufferBMP, tb_Plain);

	tb_SetRect(&chatUpRect,0,0,0,0);

	tb_SetRect(&chatDownRect,0,0,0,0);

	tb_SetRect(&chatThumbRect,0,0,0,0);

	tb_SetRect(&chatPageUpRect,0,0,0,0);

	tb_SetRect(&chatPageDownRect,0,0,0,0);

	line=0;

	for (i=startChatLine;i<startChatLine+40;i++)
	{
		if (chatLogType[i]==kPlayerMessage)
			tb_SetForeColor(bufferBMP, 0, 0, 65535);
		else if (chatLogType[i]==kSystemMessage)
			tb_SetForeColor(bufferBMP, 65535, 0, 0);
		else
			tb_SetForeColor(bufferBMP, 0, 0, 0);

		tb_DrawString(bufferBMP, 553-547, 68+(line*lineSpace)-18, chatLog[i]);

		line++;
	}

	lastLine=kMaxChatLogLines-40;

	firstLine=gFirstLine;
	lastLine=lastLine-firstLine;
	currentLine=startChatLine-firstLine;
	firstLine=0;

	if ((kMaxChatLogLines-gFirstLine)>40)
	{

		pos=56+tb_Round(((float)currentLine/(float)lastLine)*470);    // draw thumb

		if (pos<56) pos=56;

		if (pos>526) pos=526;

		if (buttonDown==3)
			tb_DrawPictureToBitmap(bufferBMP, 237, 767-547, pos, &chatThumbRect);
		else
			tb_DrawPictureToBitmap(bufferBMP, 230, 767-547, pos, &chatThumbRect);

		tb_SetRect(&chatPageUpRect,767-547,74-18,782-547,pos);

		tb_SetRect(&chatPageDownRect,767-547,pos+17,782-547,561-18);

		if (buttonDown==2)
			tb_DrawPictureToBitmap(bufferBMP, 235, 767-547, 561-18, &chatDownRect);
		else
			tb_DrawPictureToBitmap(bufferBMP, 234, 767-547, 561-18, &chatDownRect);

		if (buttonDown==1)
			tb_DrawPictureToBitmap(bufferBMP, 233, 767-547, 58-18, &chatUpRect);
		else
			tb_DrawPictureToBitmap(bufferBMP, 232, 767-547, 58-18, &chatUpRect);

		/*
		      pos=74+tb_Round(((float)currentLine/(float)lastLine)*470)-18;    // draw thumb
		      if (pos<74) pos=74-18;
		      if (pos>544) pos=544-18;
		      if (buttonDown==3)
			      tb_DrawPictureToBitmap(bufferBMP, 237, 767-547, pos, &chatThumbRect);
			    else
			      tb_DrawPictureToBitmap(bufferBMP, 230, 767-547, pos, &chatThumbRect);
		    	tb_SetRect(&chatPageUpRect,767-547,74-18,782-547,pos);
		    	tb_SetRect(&chatPageDownRect,767-547,pos+17,782-547,561-18);

		      if (buttonDown==2)
			      tb_DrawPictureToBitmap(bufferBMP, 235, 767-547, 561-18, &chatDownRect);
			    else
			      tb_DrawPictureToBitmap(bufferBMP, 234, 767-547, 561-18, &chatDownRect);

		     if (buttonDown==1)
		        tb_DrawPictureToBitmap(bufferBMP, 233, 767-547, 58-18, &chatUpRect);
		      else
		        tb_DrawPictureToBitmap(bufferBMP, 232, 767-547, 58-18, &chatUpRect);
		*/

	}

	if (windowActive[kChatWindow])
		tb_CopyBitmap(bufferBMP, tbChatWindow, &r, &chatWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbChatWindow, &r, &chatWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderCharacterDisplayWindow(void)
// ----------------------------------------------------------------- //

{
	TRect					d;
	TRect					t;
	//int           			vOff=130;
	char					conditionStr1[kStringLength];//[32];
	char					conditionStr2[kStringLength];//[32];
	int						count;

	if (windowVisible[kCharacterWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 9);

	tb_SetFontStyle(bufferBMP, tb_Plain);

	tb_DrawPictureToBitmap(bufferBMP, 242, 0, 0, &d);//stats title

	if (localRightHand!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localRightHand].itemID].pictID, 33, 122, &t);

		if (in_CanGroup(localInventory[localRightHand].itemID)==false)
		{
			if ((int)((float)localInventory[localRightHand].itemCount)>127)
				count=(int)(((float)localInventory[localRightHand].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localRightHand].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 33+1, 122+37, conditionStr1);
		}
	}

	if (localLeftHand!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localLeftHand].itemID].pictID, 155, 159, &t);

		if (in_CanGroup(localInventory[localLeftHand].itemID)==false)
		{
			if ((int)((float)localInventory[localLeftHand].itemCount)>127)
				count=(int)(((float)localInventory[localLeftHand].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localLeftHand].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 155+1, 159+37, conditionStr1);
		}
	}

	if (localLegs!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localLegs].itemID].pictID, 99, 192, &t);

		if (in_CanGroup(localInventory[localLegs].itemID)==false)
		{
			if ((int)((float)localInventory[localLegs].itemCount)>127)
				count=(int)(((float)localInventory[localLegs].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localLegs].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 99+1, 192+37, conditionStr1);
		}
	}

	if (localTorso!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localTorso].itemID].pictID, 98, 71, &t);

		if (in_CanGroup(localInventory[localTorso].itemID)==false)
		{
			if ((int)((float)localInventory[localTorso].itemCount)>127)
				count=(int)(((float)localInventory[localTorso].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localTorso].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 98+1, 71+37, conditionStr1);
		}
	}

	if (localFeet!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localFeet].itemID].pictID, 94, 260, &t);

		if (in_CanGroup(localInventory[localFeet].itemID)==false)
		{
			if ((int)((float)localInventory[localFeet].itemCount)>127)
				count=(int)(((float)localInventory[localFeet].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localFeet].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 94+1, 260+37, conditionStr1);
		}
	}

	if (localRightFinger!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localRightFinger].itemID].pictID, 12, 174, &t);

		if (in_CanGroup(localInventory[localRightFinger].itemID)==false)
		{
			if ((int)((float)localInventory[localRightFinger].itemCount)>127)
				count=(int)(((float)localInventory[localRightFinger].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localRightFinger].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 12+1, 174+37, conditionStr1);
		}
	}

	if (localLeftFinger!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localLeftFinger].itemID].pictID, 167, 209, &t);

		if (in_CanGroup(localInventory[localLeftFinger].itemID)==false)
		{
			if ((int)((float)localInventory[localLeftFinger].itemCount)>127)
				count=(int)(((float)localInventory[localLeftFinger].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localLeftFinger].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 167+1, 209+37, conditionStr1);
		}
	}

	if (localHead!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localHead].itemID].pictID, 97, 5, &t);

		if (in_CanGroup(localInventory[localHead].itemID)==false)
		{
			if ((int)((float)localInventory[localHead].itemCount)>127)
				count=(int)(((float)localInventory[localHead].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localHead].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 97+1, 5+37, conditionStr1);
		}
	}

	if (localNeck!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localNeck].itemID].pictID, 162, 14, &t);

		if (in_CanGroup(localInventory[localNeck].itemID)==false)
		{
			if ((int)((float)localInventory[localNeck].itemCount)>127)
				count=(int)(((float)localInventory[localNeck].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localNeck].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 162+1, 14+37, conditionStr1);
		}
	}

	if (localHands!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localHands].itemID].pictID, 32, 64, &t);

		if (in_CanGroup(localInventory[localHands].itemID)==false)
		{
			if ((int)((float)localInventory[localHands].itemCount)>127)
				count=(int)(((float)localInventory[localHands].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localHands].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 32+1, 64+37, conditionStr1);
		}
	}

	if (localWaist!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localWaist].itemID].pictID, 97, 126, &t);

		if (in_CanGroup(localInventory[localWaist].itemID)==false)
		{
			if ((int)((float)localInventory[localWaist].itemCount)>127)
				count=(int)(((float)localInventory[localWaist].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localWaist].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 97+1, 126+37, conditionStr1);
		}
	}

	if (localArms!=0)
	{
		tb_DrawPictureToBitmap(bufferBMP, itemList[localInventory[localArms].itemID].pictID, 173, 85, &t);

		if (in_CanGroup(localInventory[localArms].itemID)==false)
		{
			if ((int)((float)localInventory[localArms].itemCount)>127)
				count=(int)(((float)localInventory[localArms].itemCount-128)/12);
			else
				count=(int)((float)localInventory[localArms].itemCount/12);

			tb_IntegerToString(count,	conditionStr2);

			strcpy(conditionStr1,"+");

			strcat(conditionStr1,conditionStr2);

			tb_SetForeColor(bufferBMP, 0, 65535, 0);

			tb_DrawString(bufferBMP, 173+1, 85+37, conditionStr1);
		}
	}

	tb_CopyBitmap(bufferBMP, tbCharacterWindow, &characterWindowRect, &characterWindowRect, false);

	if (windowActive[kCharacterWindow])
		tb_CopyBitmap(bufferBMP, tbCharacterWindow, &characterWindowRect, &characterWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbCharacterWindow, &characterWindowRect, &characterWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderChessWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	TRect					r;
	//TRect					d;


	if (windowVisible[kChessWindow]==false)
		return;

	tb_DrawPictureToBitmap(bufferBMP, 244, 0, 0, &r);

	for (i=0;i<96;i++)
		if (localChess[i]!=0)
		{
			if (chessSelectedPiece==i)
				tb_CopyMaskBitmap(chessSelectBMP, chessMaskBMP, bufferBMP, &chessPiece[localChess[i]], &chessPiece[localChess[i]], &chessRect[i]);
			else
				tb_CopyMaskBitmap(chessBMP, chessMaskBMP, bufferBMP, &chessPiece[localChess[i]], &chessPiece[localChess[i]], &chessRect[i]);
		}

	if (windowActive[kChessWindow])
		tb_CopyBitmap(bufferBMP, tbChessWindow, &r, &r, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbChessWindow, &r, &r, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderBankWindow(void)
// ----------------------------------------------------------------- //

{
	int						i;
	//int						j;
	int						line			=	1;
	int						lineSpace	= 13;
	char 					itemName[kStringLength]; //[48];
	//char 					itemName1[kStringLength]; //[48];
	//char 					itemName2[kStringLength]; //[48];
	TRect					r;
	TRect					d;
	UInt32				gold;
	//TBoolean			equipped;
	//int						itemIDTemp;
	//int						itemCountTemp;
	//int						lookUpTemp;
	char					goldStr[kStringLength]; //[64];

	if (windowVisible[kBankWindow]==false)
		return;

	tb_SetRect(&r,0,0,238,564);

	tb_CopyBitmap(scrollBMP, bufferBMP, &scrollRect, &r, false);

	tb_DrawPictureToBitmap(bufferBMP, 188, 0, 0, &d);	// inventory title

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont(bufferBMP, kFontIDGeneva);

	tb_SetFontSize(bufferBMP, 10);
	tb_SetFontStyle(bufferBMP, tb_Plain);

	gold=in_GetGoldTotal(bankInventory[0].itemID, bankInventory[0].itemCount);

	if (gold>0)
	{
		tb_DrawPictureToBitmap(bufferBMP, 132, 553-547, 70+(0*lineSpace)-10-18, &pickUpRect[0]);
		tb_OffsetRect(&pickUpRect[0],horizontalOffset,verticalOffset);

		tb_IntegerToString(gold,goldStr);

		if (gold>1)
			strcat(goldStr," Gold Pieces");
		else
			strcat(goldStr," Gold Piece");

		tb_DrawString(bufferBMP, 570-547, 70+(0*lineSpace)-18, goldStr);

		line=1;
	}
	else
		line=0;

	for (i=1;i<kMaxInventorySize;i++)
	{
		tb_SetRect(&pickUpRect[i],	0, 0, 0, 0);

		if (bankInventory[i].itemID!=0)
		{

			in_GetItemName(bankInventory[i].itemID, itemName, bankInventory[i].itemCount);

			tb_DrawPictureToBitmap(bufferBMP, 132, 553-547, 70+(line*lineSpace)-10-18, &pickUpRect[i]);
			tb_OffsetRect(&pickUpRect[i],horizontalOffset,verticalOffset);

			tb_DrawString(bufferBMP, 570-547, 70+(line*lineSpace)-18, itemName);

			line++;
		}
	}

	if (windowActive[kBankWindow])
		tb_CopyBitmap(bufferBMP, tbBankWindow, &r, &bankWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbBankWindow, &r, &bankWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderPartyWindow(void)
// ----------------------------------------------------------------- //

{
	int						lineSpace	= 13;
	int           line;
	TRect					r;
	TRect					d;
	//char 					skillName[kStringLength]; //[32];
	char          name[kStringLength];
	int						found=0;
	int           i;

	if (windowVisible[kPartyWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	tb_SetRect(&r,0,0,188,257);

	tb_DrawPictureToBitmap(bufferBMP, 250, 0, 0, &d);

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont			(bufferBMP, kFontIDGeneva);

	tb_SetFontSize	(bufferBMP, 10);
	tb_SetFontStyle	(bufferBMP, tb_Plain);

	line=0;

	for (i=0;i<16;i++)
		if (strcmp(localParty[i],"")!=0)
		{
			strcpy(name,localParty[i]);

			if (friendOnline[i])
			{
				if (!playerDead)
					tb_SetForeColor(bufferBMP, 65535, 0, 0);

				tb_DrawPictureToBitmap(bufferBMP, 251, 9, 70+(line*lineSpace)-25, &d);

				if (friendSharing[i])
					strcat(name," (S)");
			}
			else
			{
				if (!playerDead)
					tb_SetForeColor(bufferBMP, 48960*0.75, 44115*0.75, 20910*0.75);

				tb_DrawPictureToBitmap(bufferBMP, 252, 9, 70+(line*lineSpace)-25, &d);
			}

			tb_DrawString(bufferBMP, 570-547, 70+(line*lineSpace)-18, name);

			found++;
			line++;
		}

	if (!playerDead)
		tb_SetForeColor(bufferBMP, 0, 0, 0);

	if (found==0)
		tb_DrawString(bufferBMP, 570-547+30, 70+(0*lineSpace)-18, "- No Members -");

	if (windowActive[kPartyWindow])
		tb_CopyBitmap(bufferBMP, tbPartyWindow, &r, &partyWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbPartyWindow, &r, &partyWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderQuestWindow(void)
// ----------------------------------------------------------------- //

{
	int						lineSpace	= 13;
	int           line;
	TRect					r;
	TRect					d;
	TRect         box;
	//char          num[kStringLength];
	char          str[kStringLength];

	if (windowVisible[kQuestWindow]==false)
		return;	

	tb_SetRect(&r,0,0,357,199);
	
	if (!(currentQuest>3999 && currentQuest<4007)) //invalid scroll
	{
		gr_RemoveFromWindowStack(kQuestWindow);         
		HideWindow(questWindow);          
		windowActive[kQuestWindow]=false;         
		windowVisible[kQuestWindow]=false; 
	 	currentQuest=0; 
	}

	if (currentQuest==4000)
		tb_DrawPictureToBitmap(bufferBMP, 253, 0, 0, &d);
	else if (currentQuest<4007)
		tb_DrawPictureToBitmap(bufferBMP, 255+currentQuest-4001, 0, 0, &d);
		
	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont			  (bufferBMP, kFontIDGeneva);

	tb_SetFontSize	  (bufferBMP, 10);
	tb_SetFontStyle	  (bufferBMP, tb_Plain);

	if ((questGold>0) || (questExperience>0) || (questItem>0))
	{
		tb_SetFont			(questTextBMP, kFontIDGeneva);
		tb_SetFontSize	(questTextBMP, 10);
		tb_SetFontStyle	(questTextBMP, tb_Plain);
		tb_SetGWorld    (questTextBMP);
		TETextBox       (questDescription, strlen(questDescription), &questTextRect, teFlushLeft);
		tb_RestorePort  ();
		tb_SetRect      (&box,7,43,349,149);
		tb_CopyMaskBitmap(questTextBMP,questTextBMP,bufferBMP,&questTextRect,&questTextRect,&box);
		tb_DrawString   (bufferBMP, 7, 150, "Reward:");
		line=1;

		if (questItem>0)
		{
			if (in_CanGroup(questItem))
			{
				in_GetItemName(questItem,str,questCount);
			}
			else
			{
				in_GetItemName(questItem,str,-1);
			}

			tb_DrawString   (bufferBMP, 20, 150+(line*13), str);

			line++;
		}

		if (questGold>0)
		{
			tb_IntegerToString(questGold,str);
			strcat(str," gold pieces");
			tb_DrawString   (bufferBMP, 20, 150+(line*13), str);
			line++;
		}

		if (questExperience>0)
		{
			tb_IntegerToString(questExperience,str);
			strcat(str," experience points");
			tb_DrawString   (bufferBMP, 20, 150+(line*13), str);
			line++;
		}


	}

	if (windowActive[kQuestWindow])
		tb_CopyBitmap(bufferBMP, tbQuestWindow, &r, &questWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbQuestWindow, &r, &questWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderPetsWindow(void)
// ----------------------------------------------------------------- //

{
	int						lineSpace	= 13;
	int           line;
	TRect					r;
	TRect					d;
	char          s[kStringLength];
	int						found=0;
	int           i;

	if (windowVisible[kPetsWindow]==false)
		return;
	
	if (!gotFirstRefresh)
		return;

	tb_SetRect(&r,0,0,188,267);

	tb_SetForeColor(bufferBMP, 65535, 65535, 65535);

	tb_PaintRect(bufferBMP,&r);

	tb_DrawPictureToBitmap(bufferBMP, 254, 0, 0, &d);

	if (playerDead)
	{
		tb_SetForeColor(bufferBMP, 32768, 32768, 32768);
	}
	else
	{
		tb_SetForeColor(bufferBMP, 0, 0, 0);
	}

	tb_SetFont			(bufferBMP, kFontIDGeneva);

	tb_SetFontSize	(bufferBMP, 10);
	tb_SetFontStyle	(bufferBMP, tb_Plain);

	line=52;

	for (i=0;i<kMaxPets;i++)
	{
		if (localPetType[i]==0)
		{
			if (!playerDead)
				tb_SetForeColor(bufferBMP, 48960*0.75, 44115*0.75, 20910*0.75);

			tb_IntegerToString((i+1),s);

			strcat(s,") ");

			tb_DrawString   (bufferBMP, 4, line, s);

			tb_DrawString   (bufferBMP, 20, line, "NA/NA");

			line=line+13;

			tb_DrawString   (bufferBMP, 20, line, "Status:  NA");

			line=line+13;

			tb_DrawString   (bufferBMP, 20, line, "Hunger:  NA");

			line=line+13;

			localPetStatus[i]=0;

			localPetType[i]=0;

			localPetLoyalty[i]=0;

			//strcpy(localPetName[i],"");
		}
		else
		{
			if (!playerDead)
				tb_SetForeColor(bufferBMP, 0, 0, 0);

			tb_IntegerToString((i+1),s);

			strcat(s,") ");

			tb_DrawString   (bufferBMP, 4, line, s);

			tx_GetCreatureName(localPetType[i], s);

			strcat(s,"/");

			strcat(s,localPetName[i]);

			tb_DrawString   (bufferBMP, 20, line, s);

			line=line+13;

			strcpy(s,"Status:  ");

			if (localPetStatus[i]==1)    strcat(s,"Heel");

			if (localPetStatus[i]==2)    strcat(s,"Stay");

			if (localPetStatus[i]==3)    strcat(s,"Guard");

			if (localPetStatus[i]==4)    strcat(s,"Attack");

			if (localPetStatus[i]==99)   strcat(s,"In Stable");

			tb_DrawString   (bufferBMP, 20, line, s);

			line=line+13;

			if (!playerDead)
				if ((localPetLoyalty[i]==1) || (localPetLoyalty[i]==2))
					tb_SetForeColor(bufferBMP, 65535, 0, 0);
				else
					tb_SetForeColor(bufferBMP, 0, 0, 0);

			if (localPetStatus[i]==99)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  NA");
			else
			{
				if (localPetLoyalty[i]==7)        tb_DrawString   (bufferBMP, 20, line, "Hunger:  Very Full");
				else if (localPetLoyalty[i]==6)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  Very Full");
				else if (localPetLoyalty[i]==5)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  Full");
				else if (localPetLoyalty[i]==4)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  A bit hungry");
				else if (localPetLoyalty[i]==3)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  Hungry");
				else if (localPetLoyalty[i]==2)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  Very hungry");
				else if (localPetLoyalty[i]==1)   tb_DrawString   (bufferBMP, 20, line, "Hunger:  Starving");
				else
				{
					tb_SetForeColor(bufferBMP, 0, 0, 0);
					tb_DrawString   (bufferBMP, 20, line, "Hunger:  NA");
				}
			}

			line=line+13;
		}

		line=line+7;
	}

	if (windowActive[kPetsWindow])
		tb_CopyBitmap(bufferBMP, tbPetsWindow, &r, &petsWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbPetsWindow, &r, &petsWindowRect, false);

}

#endif

#ifndef _SERVERONLY
// ----------------------------------------------------------------- //
void gr_RenderAboutWindow(void)
// ----------------------------------------------------------------- //

{
	TRect							d;

	tb_DrawPictureToBitmap(bufferBMP, 4999, 0, 0, &d);

	tb_SetForeColor(bufferBMP, 0, 0, 0);
	tb_SetFont			(bufferBMP, kFontIDGeneva);
	tb_SetFontSize	(bufferBMP, 14);
	tb_SetFontStyle	(bufferBMP, tb_Bold);

	tb_DrawString(bufferBMP, 133-(tb_StringWidth(bufferBMP,"Oberin Client")/2), 160, "Oberin Client");
	tb_DrawString(bufferBMP, 133-(tb_StringWidth(bufferBMP,"Build date:")/2), 190, "Build Date:");
	tb_DrawString(bufferBMP, 133-(tb_StringWidth(bufferBMP,buildString)/2), 210, buildString);

	if (windowActive[kAboutWindow])
		tb_CopyBitmap(bufferBMP, tbAboutWindow, &d, &aboutWindowRect, false);
	else
		tb_CopyBitmapGray(bufferBMP, tbAboutWindow, &d, &aboutWindowRect, false);


}

#endif

#ifdef _SERVER
// ----------------------------------------------------------------- //
void	gr_CreateDistanceSpell(int cc, int ct, int theSpell)
// ----------------------------------------------------------------- //
{
	int dx, dy;
	int row, col;
	int steps;
	int nbFx, nbFx2;
	int duration, duration2;
	int fxIndex;

	switch (theSpell)
	{
		case kFireballSpell:
			nbFx=kFireballX;
			duration=15;
			nbFx2=kFireballX2;
			duration2=4*60;
			break;

		case kMagicMissileSpell:
			nbFx=kMMX;
			duration=5;
			nbFx2=kMMX2;
			duration2=1*60;
			break;

		default:
			return;
			break;
	}

	dx=creature[ct].row-creature[cc].row;

	dy=creature[ct].col-creature[cc].col;

	if (abs(dx)<=1 && abs(dy)<=1) //Point blank
	{
		fx_CreatureServerFX(ct, ct, nbFx2, duration2, 0);
		sk_CalculateDamage(theSpell, cc, ct);
		return;
	}

	row=creature[cc].row;

	col=creature[cc].col;

	if (abs(dx)>abs(dy))
		steps=abs(dx)-1;
	else
		steps=abs(dy)-1;

	if (steps>9)
		return;

	//Create route
	while (steps>0)
	{
		if (dx<0)	{ dx++; row--; }

		if (dx>0)	{ dx--; row++; }

		if (dy<0)	{ dy++; col--; }

		if (dy>0)	{ dy--; col++; }

		nbFx++;

		if (steps==1)
			fxIndex=fx_AddServerFX(nbFx, duration, row, col, cc,ct);
		else
			fxIndex=fx_AddServerFX(nbFx, duration, row, col, 0,0);

		/*if (fxIndex>0)
		{
			if ((fxMap[row][col]>0) && (fxMap[row][col]<kMaxFX))
			{
				serverFX[fxMap[row][col]]					=0;
				serverFXKillTime[fxMap[row][col]]			=0;
				serverFXRow[fxMap[row][col]]				=0;
				serverFXCol[fxMap[row][col]]				=0;
				serverFXMisc[fxMap[row][col]]				=0;
				serverFXMisc2[fxMap[row][col]]				=0;
			}

			fxMap[RC(row,kMapWidth)][RC(col,kMapHeight)]=fxIndex;
		}*/

		steps--;
	}
}

#endif

