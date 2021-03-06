#ifndef _INVENTORY
#define _INVENTORY

#include "Keys.h"
#include "Remap.h"
#include "Globals.h"

//- constants ---------------------------------------------
#define									kMaxContainers									50000
#define									kMaxTracks										50000


#define									kMaxInventorySize								39 //Dave 11-23-2012 --- 39
#define									kMaxInventoryItemCount					50000 //250
#define									kMaxPotionInventoryItemCount		20 //250

#define									kRandomDecay					    			-1
#define									kThreeMinuteDecay								3
#define									kTenMinuteDecay									10
#define									kTwentyMinuteDecay							20

#define									kMaxGold												10000000

#define									kMaxBanks												100
#define									kMaxTemples											100
#define									kMaxPortals											1000
//----------------------------------------------------------

typedef struct inventoryType {
		UInt16								itemID;
		UInt16								itemCount;
	} inventoryType;

//----------------------------------------------------------�

extern	TRect							bankRect							[kMaxBanks];
extern	TRect							templeRect						[kMaxTemples];
extern	TRect							portalRect						[kMaxPortals];
extern	int								portalItem						[kMaxPortals];
extern	int								portalRow							[kMaxPortals];
extern	int								portalCol							[kMaxPortals];

extern	inventoryType			makeList							[kMaxInventorySize];
extern	inventoryType			localInventory				[kMaxInventorySize];
extern	inventoryType			pickUpInventory				[kMaxInventorySize];
extern	UInt8							containerID						[kMaxContainers];
extern	UInt8							containerLock					[kMaxContainers];
extern	UInt8							containerTrap					[kMaxContainers];
extern	char							containerTrapName			[kMaxContainers][20];
extern	UInt8							containerMagicLock		[kMaxContainers];
extern	UInt8							containerMagicTrap		[kMaxContainers];
extern	char							containerMagicTrapName[kMaxContainers][20];

extern UInt16							containerMap					[kMapWidth][kMapHeight];

//----------------------------------------------------------
UInt32 			in_InventoryCheckSum_c				(void); // client
UInt32 			in_InventoryCheckSum_s				(int i); // server
int 				in_CanAddtoInventory_c				(int item, int count); // client
UInt32 			in_CanAddGoldtoInventory_c		(UInt32 gold);
int         in_GetEmptyContainer          (void);

void        in_DamageToolInHand           (int i); // server
void 				in_CreaturePickUp							(int c); // server
void 				in_GetItemName								(int id, char itemName[kStringLength], int count);
void 				in_InitializeContainers				(void);
void 				in_CheckContainerDecay				(void);
int 				in_DropItem										(int c, int id, int theItem, int count, int row, int col, int decay, TBoolean dead, char name[kStringLength]);
UInt32 			in_DropGold										(int id, long amount, int row, int col, int decay, TBoolean dead, char name[kStringLength]);
void 				in_DropLocalGold							(long amount); // client
UInt32 			in_AddGoldtoInventory					(int i, UInt32 gold); // server
int  				in_AddtoInventory							(int i, int item, int count);
void 				in_PickUp											(int i);
void 				in_PickUpFromList							(int i, int item, int count); // server
void 				in_ClearLocalInventory				(void);
void 				in_ClearContainer							(int i);
void 				in_ClearTracks							(int i);
TBoolean 		in_CanGroup										(int item);
void 				in_DropFromInventorySlot			(int slot,int count); // client
void 				in_DamageInventorySlot				(int slot, int damage); // client // mod 8/9
void 				in_UseItem										(int item); // server
TBoolean 		in_ItemTypeEquiped						(int theType);
TBoolean 		in_ItemEquiped								(int i);
UInt32 			in_GetGoldTotal								(int gold1, int gold2);
void 				in_SetGoldTotal								(UInt32 total, int *gold1, int *gold2);
void 				in_UnEquipItem								(int i);
void 				in_ConsumeInventorySlot				(int slot); // client
int 				in_PlayerHas									(int item); // client
void 				in_RemoveFromInventorySlot		(int i, int slot, int count); // server
void 				in_ClearPickUpPack						(void);
TBoolean 		in_AnythingInPickUpList				(void);
void 				in_BuyFromList								(int i, int item, int count); // server
int 				in_QuantityOf_c								(int item); // client
int 				in_QuantityOf_s								(int i, int item); // server
int 				in_BankItem										(int i, int theItem, int count);
void 				in_PickUpFromBank							(int i, int item, int count); // server
UInt32 			in_BankGold										(int i, long amount); // server
TBoolean 		in_ContainerLocked						(int c); // server
TBoolean 		in_ContainerTrapped						(int c); // server
void 				in_ItemDestroyed							(int i, int count); // server
int         in_WeightOf                   (int item, int count); // server
TBoolean    in_PlayerCanCarryWeight       (int i, int weight); // server
int         in_PlayerHasItemType          (int theType); // client
void 				in_ClearBank									(void);
TBoolean 		in_AnythingInBankList					(void);
TBoolean    it_IsQuest                    (int id);
int         in_GetSlotOf_s                (int i, int item); // server
TBoolean    in_IsItemInParty              (int i, int item); // server
TBoolean    in_IsQuestInParty             (int i, int theQuest); // server
TBoolean 		in_ItemEquipedServer					(int i, int item);
void 				in_MakeContainer							(int row, int col, int container, TBoolean gmSpawn);
int 				in_MakeTracks							(int row, int col, int tracks);
int 				in_DropProp										(int prop, int row, int col);
TBoolean    in_ContainerOwner             (int p);
TBoolean 	in_TracksOwner					(int p);
void		in_EquipSwap					(int, int);
void		in_SwapEquipServer				(int, int, int);
int			in_GetEquipSlot					(int);
void		in_EquipLocal					(int, int);
int			in_GetEquipSlotServer			(int, int);
void			in_EquipServer					(int, int, int);
#endif