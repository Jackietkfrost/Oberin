#ifndef _NETWORK
#define _NETWORK

//#include <NetSprocket.h>
#include "OpenPlay.h"

#include "Keys.h"
#include "Remap.h"
#include "Inventory.h"
#include "Players.h"
#include "Globals.h"
#include "Action.h"
#include "Quest.h"
#include "Shell.h"
#include "Updater.h"
#include "GuildHalls.h" 

//---------------------------------------------------------------

//EXTERN_API( void )
//OTInitInetAddress               (InetAddress *          addr,
//                                 InetPort               port,
//                                 InetHost               host);
//                                 
//---------------------------------------------------------------
                                 
#define					kSendMethod									kNSpSendFlag_Registered
//#define					kSendMethod									kNSpSendFlag_Normal
                                 
#define					kNormalPriority							1
#define					kHighPriority								2

#define         validate                    101010

#define					kSendNow										true					// Send regardless of when last player action was
#define					kCanWait										false

#define					kInactivetyLogoff						15 //15					// minutes of inactively before auto log off
#define					kAfkTime								5

#ifdef _SERVER  
	#define				kStandardMessageSize				0//12000//512
	#define				kBufferSize									0//1000000//5120000
	#define				kQElements									0//5000//1000
	#define				kTimeout										5000
#endif

#ifndef _SERVER  
	#define				kStandardMessageSize				0//256
	#define				kBufferSize									0 //5000 //1000
	#define				kQElements									0//1000
	#define				kTimeout										5000
#endif

#define					kUserCancelled							-100

#define					kPlayerInputMessage								1
#define					kLeaveMessage											2
#define					kPlayerRefreshMessage							3
#define					kPlayerActionMessage							4

#define					kAuthorizeMessage									5
#define					kInventoryRefreshMessage					6
#define					kInventoryAddMessage							7
#define					kInventoryDropMessage							8

#define					kPlayerTargetMessage							9

#define					kSkillsRefreshMessage							10
#define					kSkillDirectionMessage						11

#define					kPlayerTargetResourceMessage			12
#define					kDisplayMessage										13
#define					kSkillChangeMessage								14

#define					kGoldAddMessage										16
#define					kDropGoldMessage									17
#define					kInventoryDeleteMessage						18
#define					kArmMessage												19
#define					kArmRefreshMessage								20
#define					kTextToServerMessage							21
#define					kTextToClientMessage							22

#define					kItemTransferMessage							23
#define					kInventoryDropToClientMessage			24

#define					kGiveGoldMessage									25
#define					kGoldRemoveMessage								26
#define					kContainerContentsMessage					27
#define					kTakeFromListMessage							28
#define					kShopContentsMessage							29
#define					kBuyFromListMessage								30
#define					kSellListMessage									31
#define					kMakeMessage											32
#define					kAlertMessage											33

#define					kIDMessage												34
#define					kNightVisionMessage								35

#define					kChessRefreshMessage							36
#define					kChessMoveMessage                 37
#define					kSystemTextToClientMessage				38
#define					kBroadcastTextToServerMessage			39
#define					kBugTextToServerMessage			      40
#define					kPathMessage                      41
#define					kPlayerNameMessage                42
#define					kRequestPlayerNameMessage         43
#define					kUDPMessage                       44
#define					kUDPOKMessage                     45
#define					kGMMessage                        46
#define					kMeetToClientMessage							47
#define					kMeetToServerMessage							48
#define					kClearNameMessage							    49
#define					kLocateMessage							      50
#define					kBankContentsMessage							51
#define					kTakeFromBankMessage							52
#define					kAddToPartyMessage                53
#define					kRemoveFromPartyMessage           54
#define					kQuestDescriptionMessage          55
#define					kRequestQuestDescriptionMessage   56
#define					kRepairMessage                    57
#define					kItemRepairedMessage              58
#define					kPetNameMessage                   59
#define					kPetNamesRefreshMessage           60
#define					kMacMessage                       61
#define					kClientCommandMessage							62
#define					kGuildToServerMessage             63
#define					kGiftToServerMessage              64
#define					kExpToServerMessage               65
#define					kSummonToServerMessage						66
#define					kSignToServerMessage              67
#define					kHallToServerMessage              68
#define					kLogonTextToServerMessage			    69
#define					kMsgToServerMessage               70


#define					kCompressedTextToClientMessage		71
#define					kWhisperToServerMessage				72
#define					kNoteToServerMessage				73
#define					kMsayToServerMessage				74
#define					kAnnounceToServerMessage			75
#define					kVersionToClientMessage				76
#define					kHallRequestToServerMessage			77
#define					kHallToClientMessage				78
#define					kHallGuildToServerMessage			79
#define					kNameChangeToServerMessage			80
#define					kCustomTitleToServerMessage			81
#define					kCalendarInfosToClientMessage		82
#define					kCalendarRequestToServerMessage		83
#define					kTrackingInfosToClientMessage		84
#define					kSortMessage						85
#define					kRExpToServerMessage				86
#define					kSpeedToClientMessage				87
#define					kSpecialAttackMessage				88
#define					kClearSpatMessage					89

//--- player actions --- (0-8) are move directions

#define					kRefreshInventory									9
#define					kPickUp														10
#define					kRefreshSkills										11
#define					kHide															12
#define					kFish															13
#define					kEat															14
#define					kMeditate													15
#define					kGetContainerContents							16
#define					kGetShopContents									17
#define					kGetSellList											18
#define					kDrinkLesserHealPotion						19
#define					kDrinkManaPotion									20
#define					kAttackOff												21

//#define					kGotoDesert												22
//#define					kGotoIceIsland										23
//#define					kGotoDungeon											24
//#define					kGotoBank													25
//#define					kGotoCemetary											26

#define					kLogOff														27

#define					kDetectHidden											28

#define					kCastCreateFoodSpell							29
#define					kCastSummonPetSpell								30
#define					kCastHealPetSpell							31
#define					kCastMagicTrapSpell								32
#define					kCastMagicDisarmSpell							33

#define					kDrinkLesserDetoxifyPotion				34
#define					kDrinkStrengthPotion							35
#define					kDrinkAgilityPotion								36
#define					kDrinkElevateMindPotion						37
#define					kDrinkNightVisionPotion						38

#define					kCastSummonCreatureSpell					39
#define					kInsult														40

#define					kGetActionUpdate									41
#define					kCastRevealSpell							    42
#define					kTrack												  43
#define					kPickLock												  44
#define					kDisarmTrap												45
#define					kRes												      46
#define					kUsers												    47
#define					kHelp												      48
#define					kStealthOn												49
#define					kStealthOff												50
#define					kMeetNext													51
#define					kGetBankContents									52

#define					kDrinkGreaterHealPotion						53
#define					kDrinkFullHealPotion							54
#define					kDrinkGreaterDetoxifyPotion				55
#define					kDrinkFullDetoxifyPotion					56
#define					kDrinkInvisibilityPotion					57
#define					kDrinkNourishPotion								58
#define					kCastTurnUndeadSpell							59
#define					kSharingOn							          60
#define					kSharingOff							          61

#define					kFace1							              62
#define					kFace2							              63
#define					kFace3							              64
#define					kFace4							              65
#define					kFace5							              66
#define					kFace6							              67
#define					kFace7							              68
#define					kFace8							              69

#define					kCastParalyzeFieldSpell						70
#define					kCastPoisonFieldSpell							71
//#define					kGotoLizard											  72
#define					kSpawnRat											    73
#define					kCastProvocationSpell							74
#define					kCastCalmSpell							      75
#define					kCastGreaterSummonCreatureSpell					76



//#define					kGotoCastle											  76
//#define					kGotoSewer											  77
//#define					kGotoCarth											  78
//#define					kGotoTrince											  79
//#define					kGotoMince											  80
#define					kReturn												    81
//#define					kGotoPrison												82
//#define					kGotoInPrison											83
#define					kPosition												  84
#define					kRefreshStats										  85
#define					kRefreshArm										    86
#define					kGetFX										        87
#define					kKillFX										        88
#define					kStatus												    89
#define					kGMInit												    90
#define					kGMInfo												    91
#define					kGMHome												    93
#define					kNews												      94
#define					kCCOn												      95
#define					kCCOff												    96
#define					kGoodChar												  97
#define					kEvilChar												  98
#define					kBeep												      99
#define					kFanfare												  100
#define					kThunder												  101
#define					kSlow												      102
#define					kFast												      103
#define					kMsgOn												    104
#define					kMsgOff												    105
#define					kMsgShow												  106
#define					kShutdown												  107
#define					kReload												  	108
#define					kScream													109
//#define					kGetIP												  	108
#define					kLightBlueHairDye										110
#define					kRedHairDye										111
#define					kBlueHairDye										112
#define					kBrownHairDye										113
#define					kBlackHairDye										114
#define					kGreenHairDye										115
#define					kPurpleHairDye										116
#define					kYellowHairDye										117
#define					kEnterStealth										118
#define					kBoom												119
#define					kNoTrackOn												120
#define					kNoTrackOff												121
#define					kGodModeOn												122
#define					kGodModeOff												123
#define					kAutoLockOn												124
#define					kAutoLockOff											125
#define					kMeetLast												126
#define					kCastMassHealSpell							127
#define					kCastDisorientFieldSpell					128
#define					kDrinkOpportunityPotion						129
#define					kNoDeathOn									130
#define					kNoDeathOff									131
#define					kFlute										132
#define					kHorn										133
#define					kDragon										134
#define					kCastShapeshiftLesserSpell					135
#define					kCastShapeshiftAllSpell						136
#define					kCastShapeshiftDragonSpell					137
//Dave
#define					kGrayHairDye									138
#define					kCastChaosFieldSpell							139
#define					kLichLaugh							    		140
#define					k1d20								    		141
#define					k1d100								    		142
#define					kAFK								    		143
#define					kGasp								    		144
#define					kFemaleGasp								    	145
#define					kWisdomRoll								    	146
#define					kStrengthRoll								    147
#define					kDexRoll								    	148
#define					kGKarma								    		149
#define					kBKarma									    	150
#define					kGMCurse									    151
#define					kGMCurseTarget								    152
#define					kSplash										    153
#define					k1d6										    154
#define					kJaws										    155
#define					kCastShapeshiftSpell							156
#define					kCastShapeshiftGreaterSpell						157
#define					kDev											158
//- Client Command Messages ------------------------------------

#define					kClearNamesCommand								1

//- Inventory Change Reasons -----------------------------------

#define					kTransferItem											1
#define					kDropItem													2
#define					kBoughtItem												3
#define					kNoMessage												4
#define					kTakeItem													5
#define					kCaughtItem												6
#define					kSoldItem													7
#define					kPaid															8
#define					kDepositItem											9
#define					kWithdrawItem											10
#define					kGivenItem												11
#define					kStoleItem												12
#define					kDamageItem												13
#define					kRewardItem												14
#define					kFinedItem												15

//- Drag'n'Drop sorting ----------------------------------------

#define					kSortInventory										1
#define					kSortBank											2

//--------------------------------------------------------------
typedef struct UDPMessage
{
	NSpMessageHeader				h;
} UDPMessage;

typedef struct GotUDPMessage
{
	NSpMessageHeader				h;
} GotUDPMessage;

typedef struct NightVisionMessage
{
	NSpMessageHeader				h;
} NightVisionMessage;

typedef struct RequestPlayerNameMessage
{
	NSpMessageHeader				h;
	UInt16                  data;
} RequestPlayerNameMessage;

typedef struct ClientCommandMessage
{
	NSpMessageHeader				h;
	UInt8                   command;
	UInt32									checksum;
} ClientCommandMessage;

typedef struct RequestQuestDescriptionMessage
{
	NSpMessageHeader				h;
	UInt16                  quest;
} RequestQuestDescriptionMessage;

typedef struct MacMessage
{
	NSpMessageHeader				h;
	char                    macID   [18];
} MacMessage;

typedef struct PetNameRefreshMessage
{
	NSpMessageHeader				h;
	char                    petName   [kMaxPets][15];
} PetNameRefreshMessage;

typedef struct RemoveFromPartyMessage
{
	NSpMessageHeader				h;
	UInt8                   remove;
} RemoveFromPartyMessage;

typedef struct PlayerClearMessage
{
	NSpMessageHeader				h;
	UInt16                  data;
} PlayerClearMessage;

typedef struct GMMessage
{
	NSpMessageHeader				h;
	UInt16                  data1;
	UInt16                  data2;
	UInt16									data3;
} GMMessage;

typedef struct HallRequestMessage
{
	NSpMessageHeader				h;
	UInt8							theHall;
} HallRequestMessage;

typedef struct CalendarRequestMessage
{
	NSpMessageHeader				h;
	unsigned long					theDate;
} CalendarRequestMessage;

typedef struct CalendarInfosMessage
{
	NSpMessageHeader				h;
	unsigned long					theDate;
} CalendarInfosMessage;

//------------------------------------------------

typedef struct PlayerNameMessage
{
	NSpMessageHeader				h;
	UInt16                  index;
	UInt32                  thePlayerID;
	char										theName[1];
} PlayerNameMessage;

typedef struct PlayerNameMessage2
{
	NSpMessageHeader				h;
	UInt16                  index;
	UInt32                  thePlayerID;
	char										theName[kStringLength];
} PlayerNameMessage2;

//------------------------------------------------

typedef struct BroadcastTextToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} BroadcastTextToServerMessage;

typedef struct BroadcastTextToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} BroadcastTextToServerMessage2;

//------------------------------------------------

typedef struct LogonTextToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} LogonTextToServerMessage;

typedef struct LogonTextToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} LogonTextToServerMessage2;

//------------------------------------------------

typedef struct BugTextToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} BugTextToServerMessage;

typedef struct BugTextToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} BugTextToServerMessage2;

//-----------------------------------------------

typedef struct TextToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} TextToServerMessage;

typedef struct TextToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} TextToServerMessage2;

//------------------------------------------------

typedef struct CustomTitleToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} CustomTitleToServerMessage;

typedef struct CustomTitleToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} CustomTitleToServerMessage2;

//------------------------------------------------


typedef struct SystemTextToClientMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} SystemTextToClientMessage;

typedef struct SystemTextToClientMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} SystemTextToClientMessage2;


//------------------------------------------------

typedef struct TextToClientMessage
{
	NSpMessageHeader				h;
	UInt16									checksum;
	UInt16									location;
	UInt8                   id;
	char										theText[1];
} TextToClientMessage;

typedef struct TextToClientMessage2
{
	NSpMessageHeader				h;
	UInt16									checksum;
	UInt16									location;
	UInt8                   id;
	char										theText[kStringLength];
} TextToClientMessage2;

//------------------------------------------------

typedef struct CompressedTextToClientMessage
{
	NSpMessageHeader				h;
	UInt8										theStream[1];
} CompressedTextToClientMessage;

typedef struct CompressedTextToClientMessage2
{
	NSpMessageHeader				h;
	UInt8										theStream[256];
} CompressedTextToClientMessage2;

//------------------------------------------------

//------------------------------------------------

typedef struct TrackingInfoToClientMessage
{
	NSpMessageHeader				h;
	UInt8										trackInfo[1];
} TrackingInfoToClientMessage;

typedef struct TrackingInfoToClientMessage2
{
	NSpMessageHeader				h;
	UInt8										trackInfo[(3*kMaxTracked)+1];
} TrackingInfoToClientMessage2;

//------------------------------------------------

typedef struct PetNameMessage
{
	NSpMessageHeader				h;
	UInt8										pet;
	char										theText[1];
} PetNameMessage;

typedef struct PetNameMessage2
{
	NSpMessageHeader				h;
	UInt8										pet;
	char										theText[kStringLength];
} PetNameMessage2;

//------------------------------------------------

typedef struct NameChangeToServerMessage
{
	NSpMessageHeader				h;
	UInt8										perm;
	char										newName[1];
} NameChangeToServerMessage;

typedef struct NameChangeToServerMessage2
{
	NSpMessageHeader				h;
	UInt8										perm;
	char										newName[kStringLength];
} NameChangeToServerMessage2;

//------------------------------------------------

typedef struct RepairMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										slot;
	UInt32                  checksum;
} RepairMessage;

typedef struct ItemRepairedMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										slot;
	UInt8										count;
	UInt32                  checksum;
} ItemRepairedMessage;

//--------------------------------------------------------------

typedef struct SignToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} SignToServerMessage;

typedef struct SignToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} SignToServerMessage2;

//--------------------------------------------------------------

typedef struct MsayToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} MsayToServerMessage;

typedef struct MsayToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} MsayToServerMessage2;

//--------------------------------------------------------------

typedef struct NoteToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} NoteToServerMessage;

typedef struct NoteToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} NoteToServerMessage2;

//--------------------------------------------------------------
typedef struct HallToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} HallToServerMessage;

typedef struct HallToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} HallToServerMessage2;

//--------------------------------------------------------------
typedef struct HallToClientMessage
{
	NSpMessageHeader				h;
	TBoolean						saved;
	UInt8							size;
	UInt16							row;
	UInt16  						col;
	extraType						extras[kMaxExtras];
	
} HallToClientMessage;

//--------------------------------------------------------------

typedef struct GuildToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} GuildToServerMessage;

typedef struct GuildToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} GuildToServerMessage2;

//--------------------------------------------------------------

typedef struct MsgToServerMessage
{
	NSpMessageHeader				h;
	UInt8                   line;
	char										theText[1];
} MsgToServerMessage;

typedef struct MsgToServerMessage2
{
	NSpMessageHeader				h;
	UInt8                   line;
	char										theText[kStringLength];
} MsgToServerMessage2;

//--------------------------------------------------------------

typedef struct WhisperToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} WhisperToServerMessage;

typedef struct WhisperToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} WhisperToServerMessage2;

//--------------------------------------------------------------

typedef struct ExpToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} ExpToServerMessage;

typedef struct ExpToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} ExpToServerMessage2;

//--------------------------------------------------------------

typedef struct VersionToClientMessage
{
	NSpMessageHeader				h;
	UInt8										version;
	char										url[kStringLength];
} VersionToClientMessage;

//--------------------------------------------------------------

typedef struct AnnounceToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} AnnounceToServerMessage;

typedef struct AnnounceToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} AnnounceToServerMessage2;

//--------------------------------------------------------------

typedef struct RExpToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} RExpToServerMessage;

typedef struct RExpToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} RExpToServerMessage2;

//--------------------------------------------------------------

typedef struct GiftToServerMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} GiftToServerMessage;

typedef struct GiftToServerMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} GiftToServerMessage2;

//--------------------------------------------------------------

typedef struct SummonToServerMessage
{
	NSpMessageHeader				h;
	UInt8                   direction;
	char										theText[1];
} SummonToServerMessage;

typedef struct SummonToServerMessage2
{
	NSpMessageHeader				h;
	UInt8                   direction;
	char										theText[kStringLength];
} SummonToServerMessage2;

//--------------------------------------------------------------

typedef struct MeetToServerMessage
{
	NSpMessageHeader				h;
	TBoolean                sound;
	char										theText[1];
} MeetToServerMessage;

typedef struct MeetToServerMessage2
{
	NSpMessageHeader				h;
	TBoolean                sound;
	char										theText[kStringLength];
} MeetToServerMessage2;

//--------------------------------------------------------------

typedef struct AddToPartyMessage
{
	NSpMessageHeader				h;
	UInt8                   index;
	char										theText[1];
} AddToPartyMessage;

typedef struct AddToPartyMessage2
{
	NSpMessageHeader				h;
	UInt8                   index;
	char										theText[kStringLength];
} AddToPartyMessage2;

//--------------------------------------------------------------

typedef struct QuestDescriptionMessage
{
	NSpMessageHeader				h;
	UInt16                  gold;
	UInt16                  experience;
	UInt16                  item;
	UInt16                  count;
	char										theText[1];
} QuestDescriptionMessage;

typedef struct QuestDescriptionMessage2
{
	NSpMessageHeader				h;
	UInt16                  gold;
	UInt16                  experience;
	UInt16                  item;
	UInt16                  count;
	char										theText[kQuestStringLength];
} QuestDescriptionMessage2;
//--------------------------------------------------------------

typedef struct HallGuildToServerMessage
{
	NSpMessageHeader				h;
	char										guild[1];
} HallGuildToServerMessage;

typedef struct HallGuildToServerMessage2
{
	NSpMessageHeader				h;
	char										guild[kQuestStringLength];
} HallGuildToServerMessage2;
//--------------------------------------------------------------

typedef struct LocateMessage
{
	NSpMessageHeader				h;
	char										theText[1];
} LocateMessage;

typedef struct LocateMessage2
{
	NSpMessageHeader				h;
	char										theText[kStringLength];
} LocateMessage2;

//--------------------------------------------------------------

typedef struct ChessMoveMessage
{
	NSpMessageHeader				h;
	UInt8										moveFrom;
	UInt8										moveTo;
} ChessMoveMessage;

typedef struct ChessRefreshMessage
{
	NSpMessageHeader				h;
	UInt8										square[96];
} ChessRefreshMessage;

typedef struct SkillsRefreshMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										skill			[kMaxSkills];
	UInt32                  checksum;
} SkillsRefreshMessage;

typedef struct SkillChangeMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										skill;
	UInt8										level;
	UInt32                  checksum;
} SkillChangeMessage;

//--------------------------------------------------------------
typedef struct MakeMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt16									item;
	UInt32                  checksum;
} MakeMessage;

//--------------------------------------------------------------
typedef struct SpatMessage // ---------- checksum in
{
	NSpMessageHeader		h;
	UInt16					item;
	UInt32                  checksum;
} SpatMessage;

typedef struct BuyFromListMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										item;
	UInt16									count;
	UInt32                  checksum;
} BuyFromListMessage;

typedef struct TakeFromListMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										item;
	UInt32									count;
	UInt32                  checksum;
} TakeFromListMessage;

typedef struct ContainerContentsMessage
{
	NSpMessageHeader				h;
	UInt32									checksum;
	char										corpseName[15];
	UInt16									item	[kMaxInventorySize];
	UInt16									count	[kMaxInventorySize];
} ContainerContentsMessage;

typedef struct ShopMessage
{
	NSpMessageHeader				h;
	UInt16									item	[kMaxInventorySize];
	UInt16									cost	[kMaxInventorySize];
} ShopMessage;

typedef struct GiveGoldMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										target;
	UInt32									amount;
	UInt32                  checksum;
} GiveGoldMessage;

typedef struct GoldRemoveMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										reason;
	UInt32									amount;
	UInt16							to;
	UInt32                  checksum;
} GoldRemoveMessage;

typedef struct ItemTransferMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt16									target;
	UInt8										slot;
	UInt16									count;
	UInt32                  checksum;
} ItemTransferMessage;

typedef struct InventoryRefreshMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt16									item	[kMaxInventorySize];
	UInt16									count	[kMaxInventorySize];
	UInt32                  checksum;
} InventoryRefreshMessage;

typedef struct InventoryAddMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt32									checksum;
	UInt8										reason;
	UInt8										from;
	UInt16									item;
	UInt16									count;
	UInt32                  check;
} InventoryAddMessage;

typedef struct InventoryDropMessage
{
	NSpMessageHeader				h;
	UInt32									checksum;
	UInt8										reason;
	UInt8										slot;
	UInt16									count;
	UInt16							to;
} InventoryDropMessage;

typedef struct InventoryDeleteMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										slot;
	UInt32                  checksum;
} InventoryDeleteMessage;

typedef struct GoldAddMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										reason;
	UInt8										from;
	UInt16									gold1;
	UInt8										gold2;
	UInt32                  checksum;
} GoldAddMessage;

typedef struct ArmMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										location;
	UInt8										item;
	UInt32                  checksum;
} ArmMessage;

typedef struct ArmRefreshMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										arm			[13];
	UInt32                  checksum;
} ArmRefreshMessage;

typedef struct IDMessage
{
	NSpMessageHeader				h;
	UInt16									id;
	UInt8										title;
	UInt8                   gender;
	char										playerName[15];
	char										guildName [20];
	char										customTitle[20];
	UInt8                   characterClass;
	UInt8                   characterLevel;
	UInt16									arm				[13];
	UInt8										karma;
} IDMessage;

//--------------------------------------------------------------

//--------------------------------------------------------------

typedef struct PlayerRefreshMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt16									row;
	UInt16									col;
	UInt32                  checksum;
	UInt16									dataLength;
	UInt8										data[1];
} PlayerRefreshMessage;

typedef struct PlayerRefreshMessage2 // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt16									row;
	UInt16									col;
	UInt32                  checksum;
	UInt16									dataLength;
	UInt8										data[kDataSize];
} PlayerRefreshMessage2;

//--------------------------------------------------------------

typedef struct PlayerActionMessage
{
	NSpMessageHeader				h;
	UInt8										data;
} PlayerActionMessage;

typedef struct PlayerTargetMessage
{
	NSpMessageHeader				h;
	UInt16										action;
	UInt16									target;
	//UInt8										target;
} PlayerTargetMessage;

typedef struct PlayerTargetResourceMessage
{
	NSpMessageHeader				h;
	UInt8										gatherType;
	UInt16									row;
	UInt16									col;
} PlayerTargetResourceMessage;

typedef struct PathMessage
{
	NSpMessageHeader				h;
	UInt16									row;
	UInt16									col;
} PathMessage;

//--------------------------------------------------------------

typedef struct AuthorizeMessage
{
	NSpMessageHeader				h;
	char										password[15];
	char										name[15];
	int											version;
	UInt8										clientType;
} AuthorizeMessage;

//--------------------------------------------------------------

typedef struct AlertMessage
{
	NSpMessageHeader				h;
	char										text[48];
} AlertMessage;

//--------------------------------------------------------------

typedef struct SpeedMessage
{
	NSpMessageHeader				h;
	UInt8							speed;
} SpeedMessage;

//--------------------------------------------------------------

typedef struct DisplayMessage // ---------- checksum in
{
	NSpMessageHeader				h;
	UInt8										display;
	UInt32                  checksum;
} DisplayMessage;

//--------------------------------------------------------------

typedef struct SortMessage
{
	NSpMessageHeader			h;
	UInt8						what;
	UInt8						start;
	UInt8						stop;
} SortMessage;

//--------------------------------------------------------------

typedef struct ClearSpatMessage
{
	NSpMessageHeader			h;
} ClearSpatMessage;

//--------------------------------------------------------------

extern UInt8					serverFX[kMaxFX];
extern int						maxDataSize; // beta variable
extern int						lastMessageSize;// beta variable
extern int						numberOfPlayersOnline;
extern long						numberOfLogons;
extern int						maxPlayersOnline;
extern long						numberOfLogoffs;
extern long						gracefulLogoffs;
extern long						failedLogons;
extern long						serverFull;
extern int						messagesReceived;
extern int						messagesSent;
extern long						bytesSent;
extern long						bytesReceived;
extern int						maxMessageSize;
extern long						exceededNT;
extern long						skillGain;
extern int						gameTime;
extern TBoolean					playerDead;
extern NSpGameReference			gNetGame;
extern int						displayAreaButtonState[11];
extern TRect					displayAreaButton[11];
//extern UInt8					localChess[80];

void                      		nw_CheckForFriends          (int p, int *b1, int *b2, int *b3, int *b4); // server
void                      		nw_GetMACAddress            (void);
void                      		nw_DisplayEnetAddr          (UInt8 *enetaddr);
void                      		nw_TestParseRefresh         (int length);
TBoolean                  		nw_RefreshDataOkay          (int length);
static pascal Boolean			MessageHandler							(NSpGameReference inGameRef, NSpMessageHeader *inMessage, void *inContext);
void                      		nw_GetStartDate             (void);

OSStatus 						nw_InitNetworking						(void);
void 							nw_InitNetworkingVariables	(void); // server/client
void 							nw_ShutdownNetworking				(void);

void 							nw_AddToWaitQueue						(int waitForID); // client
void 							nw_ClearQueueOf							(int waitForID); // client

OSStatus 						nw_DoHost										(void);
OSStatus 						nw_DoJoin										(void);

void 							nw_SendAuthorization				(void);
void 							nw_RemovePlayer							(NSpPlayerID player);

void                      		nw_SendMAC                  (void); // client
void                      		nw_GetMac                   (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendUDPTest              (int i); // server
void                      		nw_GetUDPTest               (NSpMessageHeader *inMessage); // client

void                      		nw_SendUDPOK                (void); // client
void                      		nw_GetUDPOK                 (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendGMCommand            (UInt16 data1, UInt16 data2, UInt16 data3); // client
void                      		nw_GetGMCommand             (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendSystemTextToClient   (int toID, char SendText[kStringLength]); // server
void                      		nw_GetSystemTextFromServer  (NSpMessageHeader *inMessage); // client

void 							nw_SendMake									(UInt16 item); // client
void 							nw_GetMake									(int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendQuestDescription     (int toID, int gold, int experience, int item, int count, char textMessage[kQuestStringLength]); // server
void                      		nw_GetQuestDescription      (NSpMessageHeader *inMessage); // client

void                      		nw_SendClearName            (int name); // server
void                      		nw_SendClearOneName         (int i, int name); // server add to carbon 11/24
void                      		nw_GetClearName             (NSpMessageHeader *inMessage); // client
void                     		nw_SendPlayerLeft           (NSpPlayerID id); // server

void 							nw_SendTakeFromList					(int item, int count); // client
void 							nw_GetTakeFromList					(int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendTakeFromBank         (int item, int count); // client
void                      		nw_GetTakeFromBank          (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendPetNameRefresh       (int i); // server
void                      		nw_GetRefreshPetNames       (NSpMessageHeader *inMessage); // client

void                      		nw_SendRequestName          (UInt16 p); // client
void                      		nw_GetRequestName           (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendLocateToHost         (char SendText[15]); // client
void                      		nw_GetLocateFromClient      (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendItemRepaired         (int i, int slot, int count); // server
void                      		nw_GetItemRepaired          (NSpMessageHeader *inMessage); // client

void 							nw_SendBuyFromList					(int item, int count); // client
void 							nw_GetBuyFromList						(int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendPlayerName           (int i, int p); // server
void                      		nw_GetPlayerName            (NSpMessageHeader *inMessage);

void 							nw_SendBroadcastTextToHost	(char SendText[kStringLength]); // client
void 							nw_GetBroadcastTextFromClient(int i, NSpMessageHeader *inMessage); // server

void 							nw_GetMessages							(void);
void 							nw_DoHandleMessage					(NSpMessageHeader *inMessage);

void                      		nw_SendRemoveFromParty      (UInt8 p); // client
void                      		nw_GetRemoveFromParty       (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendRequestQuestDescription  (UInt16 quest); // client
void                      		nw_GetRequestQuestDescription   (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendBankContents					(int i); // server
void 							nw_GetBankContents					(NSpMessageHeader *inMessage); // client

TBoolean 						nw_CanAct										(void); // client
TBoolean 						nw_CanSend									(void); // client

void 							nw_SendRefresh							(int i, TBoolean forceRefresh); // server
void	 						nw_SendRefreshToAllPlayers	(void);
void 							nw_GetRefresh								(NSpMessageHeader *inMessage); // client variable bytes
void 							nw_ParseRefresh							(void);

void 							nw_SendContainerContents		(int i); // server
void 							nw_GetContainerContents			(NSpMessageHeader *inMessage); // client

void 							nw_SendShopContents					(int i); // server
void 							nw_GetShopContents					(NSpMessageHeader *inMessage); // client

void                      		nw_SendPetName              (int pet, char SendText[kStringLength]); // client
void                      		nw_GetPetName               (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendSellList							(int i); // server
void 							nw_GetSellList							(NSpMessageHeader *inMessage); // client

void 							nw_SendTextToHost						(char theText[kStringLength]); // client
void 							nw_GetTextFromClient				(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendTextToClient					(int toID, char textMessage[kStringLength], int location); // server
void 							nw_SendNPCTextToClient			(int toID, int theTextID, int location); // server
void 							nw_GetTextFromServer				(NSpMessageHeader *inMessage); // client

void 							nw_SendCompressedTextToClient(int toID, char textMessage[kStringLength], int location); // server

void 							nw_SendDeleteFromInventorySlot			(int slot); // client
void 							nw_GetDeleteFromInventorySlot				(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendAction								(UInt8 playerAction, int priority, TBoolean mustSendNow, TBoolean addToQueue);
void 							nw_GetPlayerAction					(int i, NSpMessageHeader *inMessage);

void 							nw_SendToClientDropFromInventorySlot	(int i, int reason, int slot, int count, int to); // server
void 							nw_GetToClientDropFromInventorySlot		(NSpMessageHeader *inMessage); // client

void 							nw_SendInventoryRefresh			(int i);
void 							nw_GetRefreshInventory			(NSpMessageHeader *inMessage);

void 							nw_SendSkillsRefresh				(int i); // server
void 							nw_GetRefreshSkills					(NSpMessageHeader *inMessage); // client

void 							nw_SendSkillChange					(int i, int skill, int level); // server
void 							nw_GetSkillChange						(NSpMessageHeader *inMessage); // client

void 							nw_SendItemTransferMessage	(int playerTarget, int slot, int count); // client
void 							nw_GetItemTransferMessage		(int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendInventoryAdd         (int i, int item, int count, int reason, int from); // server
void 							nw_GetInventoryAdd					(NSpMessageHeader *inMessage);

void                      		nw_SendRepair               (int slot); // client
void                      		nw_GetRepair                (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendGiveGold							(UInt8 playerTarget, UInt32 amount); // client
void 							nw_GetGiveGold							(int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendGoldAdd              (int i, int reason, int from, int gold1, int gold2); // server
void 							nw_GetGoldAdd								(NSpMessageHeader *inMessage); // client

void 							nw_SendGoldRemove						(int i, UInt8 reason, UInt32 amount, UInt8 to); // server
void 							nw_GetGoldRemove						(NSpMessageHeader *inMessage); // client

//void 							nw_SendTargetMessage				(UInt8 playerTarget, int playerAction);
void 							nw_SendTargetMessage				(int playerTarget, int playerAction);
void 							nw_GetTargetMessage					(int i, NSpMessageHeader *inMessage);

void 							nw_SendTargetResourceMessage(int row, int col, int gatherType); // client
void 							nw_GetTargetResourceMessage	(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendDisplayMessage				(int i, UInt8 message); // server
void 							nw_GetDisplayMessage				(NSpMessageHeader *inMessage); // client

void 							nw_SendMeetToHost						(char SendText[kStringLength], TBoolean sound); // client
void 							nw_GetMeetFromClient				(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendSummonToHost					(char SendText[kStringLength], int direction); // client
void 							nw_GetSummonFromClient			(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendGuildToHost					(char SendText[kStringLength]); // client
void 							nw_GetGuildFromClient				(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendExpToHost					  (char SendText[kStringLength]); // client
void 							nw_GetExpFromClient				  (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendGiftToHost					  (char SendText[kStringLength]); // client
void 							nw_GetGiftFromClient				(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendArm									(UInt8 location, UInt8 item); // client
void 							nw_GetArm										(int i, NSpMessageHeader *inMessage); // server

void 							nw_SendArmRefresh						(int i); // server
void 							nw_GetArmRefresh						(NSpMessageHeader *inMessage); // client

void 							nw_GetPlayerLeft						(NSpMessageHeader *inMessage);

void 							nw_SendAlertMessage					(NSpPlayerID player, char	message[kStringLength]); // server
void 							nw_GetAlertMessage					(NSpMessageHeader *inMessage); // client

void 							nw_SendID										(int i, int c); // server
void 							nw_GetID										(NSpMessageHeader *inMessage); // client

void                      		nw_SendAddToParty           (int  index, char theSendText[kStringLength]); // client
void                      		nw_GetAddToParty            (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendPathMessage          (int row, int col); // client
void                      		nw_GetPathMessage           (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendBugTextToHost        (char SendText[kStringLength]); // client
void                      		nw_GetBugTextFromClient     (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendClientCommand				(int i, int command); // server
void 							nw_GetClientCommand					(NSpMessageHeader *inMessage); // client

void                      		nw_SendSignToHost           (char SendText[kStringLength]); // client
void                      		nw_GetSignFromClient        (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendMsayToHost           (char SendText[kStringLength]); // client
void                      		nw_GetMsayFromClient        (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendNoteToHost           (char SendText[kStringLength]); // client
void                      		nw_GetNoteFromClient        (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendHallToHost           (char SendText[kStringLength]); // client
void                      		nw_GetHallFromClient        (int i, NSpMessageHeader *inMessage); // server

void							nw_SendHallRequest			(int);
void							nw_GetHallRequest			(int , NSpMessageHeader*);

void							nw_SendHallGuildToServer	(char SendText[kStringLength]);
void							nw_GetHallGuildFromClient	(int, NSpMessageHeader*);

void							nw_SendHallInfos			(int, int);
void							nw_GetHallInfos				(NSpMessageHeader*);

void                      		nw_SendLogonTextToHost      (char SendText[kStringLength]); // client
void                      		nw_GetLogonTextFromClient   (int i, NSpMessageHeader *inMessage); // server

void                      		nw_SendMsgToHost            (int line, char SendText[kStringLength]); // client
void                      		nw_GetMsgFromClient         (int i, NSpMessageHeader *inMessage); // server

void 							nw_SendWhisperToHost		(char SendText[kStringLength]);  //client
void 							nw_GetWhisperFromClient		(int i, NSpMessageHeader *inMessage);  //server

void 							nw_SendAnnounceToHost		(char SendText[kStringLength]);  //client
void 							nw_GetAnnounceFromClient		(int i, NSpMessageHeader *inMessage);  //server

void 							nw_SendRExpToHost		(char SendText[kStringLength]);  //client
void 							nw_GetRExpFromClient		(int i, NSpMessageHeader *inMessage);  //server

void 							nw_SendNightVision					(int i); // server
void 							nw_SendTextRefresh					(int i); // server

void							nw_SendUpdateRequired		(int i);
void							nw_GetUpdateRequired		(NSpMessageHeader *inMessage);

void							nw_SendCalendarRequest		(void);
void							nw_GetCalendarRequest		(int i, NSpMessageHeader *inMessage);
void							nw_SendCalendarInfo			(int i);
void							nw_GetCalendarInfo			(NSpMessageHeader *inMessage);

void							nw_SendNameChangeToHost		(char SendText[kStringLength], UInt8 perma);
void 							nw_GetNameChangeFromClient	(int i, NSpMessageHeader *inMessage);

TBoolean						nw_SendTrackingInfosToClient (int i);
void							nw_GetTrackingInfosFromServer (NSpMessageHeader *inMessage);

void 							nw_SendSortMessageToServer  (WindowRef, int, int);
void							nw_GetSortMessage 			(int, NSpMessageHeader*);

void							nw_SendCustomTitleToHost	(char SendText[kStringLength]);
void 							nw_GetCustomTitleFromClient	(int i, NSpMessageHeader *inMessage);

void							nw_SendSpeedRefresh			(int i);
void							nw_GetSpeed					(NSpMessageHeader*);

void							nw_SendClearSpat			(int i);
void							nw_GetClearSpat				(NSpMessageHeader*);

void							nw_SendSpecialAttack		(int item);
void							nw_GetSpecialAttack			(int i, NSpMessageHeader*);

void 							nw_BuildTrail								(int Ax, int Ay, int Bx, int By, int *length);

void 							nw_SendActionUpdate					(int i); // server
void 							nw_GetChessUpdate						(NSpMessageHeader *inMessage); // client
void                      		nw_SendChessMove            (int moveFrom, int moveTo); // client
void                      		nw_GetChessMove             (int i, NSpMessageHeader *inMessage); // server
TBoolean                  		nw_IsGameOkay               (void);
void 							nw_RestartServer						(void);
void                      		nw_CompressDataTest         (int p, int length); // server
static pascal unsigned int 		nw_imReadProc              (char *buf, unsigned int size, void *Param);
static pascal void        		nw_imWriteProc              (char *buf, unsigned int size, void *Param);
static pascal unsigned int 		nw_exReadProc              (char *buf, unsigned int size, void *Param);
static pascal void        		nw_exWriteProc              (char *buf, unsigned int size, void *Param);
static pascal unsigned int 		nw_ReadProc(char *buf, unsigned int size, void *Param);
static pascal void 				nw_WriteProc(char *buf, unsigned int size, void *Param);
void 							nw_CompressData(char *inBuffer, int inSize, char *outBuffer, int *outSize, unsigned int type);
void 							nw_DecompressData(char *inBuffer, int inSize, char *outBuffer, int *outSize);

//--------------------------------------------------------------

#endif