#ifndef _GAMETEXT
#define _GAMETEXT

#include "Keys.h"
#include "Remap.h"
#include "Inventory.h"

#define		kAttackingTalk													1
#define		kChasingTalk													2
#define		kRetreatingTalk													3
#define		kKillingTalk													4
#define		kHidingTalk														5
#define		kJustHidTalk													6
#define		kMorphedTalk													7

#define		kDropCancelledMsg												1
#define		kGiveCancelledMsg												2
#define		kTakeCancelledMsg												3
#define		kBuyCancelledMsg												4
#define		kSellCancelledMsg												5
#define		kDepositCancelledMsg											6
#define		kWithdrawalCancelledMsg											7
#define		kLocateCancelledMsg												8
#define		kWelcomeMsg												        9
#define		kPetNotFoundMsg													10
#define		kPetNamedMsg													11
#define		kNameToLongMsg													12
#define		kAlreadyInPartyMsg												13
#define		kPartyFullMsg													14
#define		kInvalidNameMsg													15
#define		kNotFoundInPartyMsg												16
#define		kCommentLoggedMsg												18
#define		kSharingOnMsg													19
#define		kSharingOffMsg													20
#define		kTargetRecepientMsg												21
#define		kCantCarryMoreGoldMsg											22
#define		kPackIsFullMsg													23
#define		kNotEnoughGoldMsg												24
#define		kMustTargetTreeMsg												25
#define		kTooFarFromTreeMsg												26
#define		kStartChoppingTreeMsg											27
#define		kMustTargetRockMsg												28
#define		kTooFarFromRockMsg												29
#define		kStartMiningOreMsg												30
#define		kHowMuchGoldToTakeMsg											31
#define		kHowManyToTakeMsg												32
#define		kInvalidPlayerNameMsg											33
#define		kEnterBankMsg													34
#define		kEnterShopMsg													35
#define		kMeetTraderMsg													36
#define		kMustHaveTargetMsg												37
#define		kMustBeBehindTargetMsg											38
#define		kMustWearDaggerMsg												39
#define		kMustHaveWeaponMsg												40
#define		kMustBeNearTargetMsg											41
#define		kMustHaveBowMsg													42
#define		kMustHaveShieldMsg												43
#define		kCantCancelMsg													44
#define		kMustHaveMeleeWeaponMsg											45
//#define		kMustBeStealthed												46
//#define		kYouRolled														47

void tx_AddText             (int m);
void tx_GetCreatureName     (int c, char creatureName[kStringLength]);
void tx_GetSkillName        (int skill, char skillName[kStringLength]);
void tx_CreatureTalkText		(int c1, int c2, int situation);





#endif