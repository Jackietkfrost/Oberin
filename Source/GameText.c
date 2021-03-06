#include "Toolbox.h"
#include "Globals.h"
#include "Common.h"
#include "Creatures.h"
#include "Items.h"
#include "Npc.h"
#include "Players.h"
#include "FileIO.h"
#include "FX.h"
#include "Graphics.h"
#include "Skills.h"
#include "Resource.h"
#include "GameText.h"

extern UInt32                       chatMessageID;

// ----------------------------------------------------------------- //
void tx_AddText(int m)
// ----------------------------------------------------------------- //

{

	if (m==kDropCancelledMsg)                            	gr_AddText("Drop cancelled.",false,false);
	else if (m==kGiveCancelledMsg)                       	gr_AddText("Give cancelled.",false,false);
	else if (m==kTakeCancelledMsg)                       	gr_AddText("Take cancelled.",false,false);
	else if (m==kBuyCancelledMsg)                        	gr_AddText("Buy cancelled.",false,false);
	else if (m==kSellCancelledMsg)                       	gr_AddText("Sell cancelled.",false,false);
	else if (m==kDepositCancelledMsg)                    	gr_AddText("Deposit cancelled.",false,false);
	else if (m==kWithdrawalCancelledMsg)                 	gr_AddText("Withdrawal cancelled.",false,false);
	else if (m==kLocateCancelledMsg)                     	gr_AddText("Locate cancelled.",false,false);
	else if (m==kWelcomeMsg)                             	gr_AddText("Welcome.",false,true);
	else if (m==kPetNotFoundMsg)                         	gr_AddText("Pet not found.",false,true);
	else if (m==kPetNamedMsg)                            	gr_AddText("Your pet has been named.",false,true);
	else if (m==kNameToLongMsg)                          	gr_AddText("Name too long.",true,true);
	else if (m==kAlreadyInPartyMsg)                      	gr_AddText("Already in party.",true,true);
	else if (m==kPartyFullMsg)                      		gr_AddText("Party full.",true,true);
	else if (m==kInvalidNameMsg)                      		gr_AddText("Invalid name.",true,true);
	else if (m==kNotFoundInPartyMsg)                      	gr_AddText("Not found in party.",true,true);
	else if (m==kInvalidPlayerNameMsg)                    	gr_AddText("Invalid player name.",true,true);

	//else if (m==kCommentLoggedMsg)                    		gr_AddText("Comment logged.  Thanks!",true,true);
	else if (m==kCommentLoggedMsg)                    		gr_AddText("Please post bugs at:  http://www.oberin.com/",true,true);

	else if (m==kSharingOnMsg)                    			gr_AddText("Sharing is on.",false,true);
	else if (m==kSharingOffMsg)                    			gr_AddText("Sharing is off.",false,true);
	else if (m==kTargetRecepientMsg)                    	gr_AddText("Target the recepient.",false,false);
	else if (m==kCantCarryMoreGoldMsg)                    	gr_AddText("You can't carry any more gold.",false,true);
	else if (m==kPackIsFullMsg)                    			gr_AddText("Your pack is full.",false,true);
	else if (m==kNotEnoughGoldMsg)                    		gr_AddText("You don't have enough gold.",false,true);
	else if (m==kMustTargetTreeMsg)                    		gr_AddText("You must target a tree to harvest wood.",false,true);
	else if (m==kTooFarFromTreeMsg)                    		gr_AddText("You are too far from the tree.",false,true);
	else if (m==kStartChoppingTreeMsg)                    	gr_AddText("You start chopping the tree...",false,true);
	else if (m==kMustTargetRockMsg)                    		gr_AddText("You must target a rock to mine for ore.",false,true);
	else if (m==kTooFarFromRockMsg)                    		gr_AddText("You are too far from the rock.",false,true);
	else if (m==kStartMiningOreMsg)                    		gr_AddText("You start mining for ore...",false,true);
	else if (m==kHowMuchGoldToTakeMsg)                    	gr_AddText("How much gold do you want to take?",false,true);
	else if (m==kHowManyToTakeMsg)                    		gr_AddText("How many do you want to take?",false,true);
	else if (m==kInvalidPlayerNameMsg)						gr_AddText("Invalid player name.",true,true);
	else if (m==kEnterBankMsg)								gr_AddText("You enter a bank.",false,true);
	else if (m==kEnterShopMsg)								gr_AddText("You enter a shop.",false,true);
	else if (m==kMeetTraderMsg)								gr_AddText("You meet a trader.",false,true);
	else if (m==kMustHaveTargetMsg)							gr_AddText("You must have a target.",false,true);
	else if (m==kCantCancelMsg)								gr_AddText("You cannot cancel this action.", false, true);
	else if (m==kMustBeBehindTargetMsg)						gr_AddText("You must be behind your target.",false,true);
	else if (m==kMustWearDaggerMsg)							gr_AddText("You must have a dagger equipped.",false,true);
	else if (m==kMustHaveWeaponMsg)							gr_AddText("You must have a weapon equipped.",false,true);
	else if (m==kMustHaveBowMsg)							gr_AddText("You must have a bow equipped.",false,true);
	else if (m==kMustHaveShieldMsg)							gr_AddText("You must have a shield equipped.",false,true);
	else if (m==kMustBeNearTargetMsg)						gr_AddText("Target too far.",false,true);
	else if (m==kMustHaveMeleeWeaponMsg)					gr_AddText("You must have a melee weapon equipped.",false,true);
	else if (m==kMustBeStealthed)							gr_AddText("You must be stealthy before you can do this action.",false,true);
	else if (m==kYouRolled)									gr_AddText("You rolled a die!",true,true);	
}

// ----------------------------------------------------------------- //
void tx_GetCreatureName(int c, char creatureName[kStringLength])
// ----------------------------------------------------------------- //

{
	if (c==kNPCHuman || c==kNPCHumanFemale)
		/*{
			if (npc[creature[c].npcIndex].type==kNPCGuard)
				strcpy(creatureName, "Guard");
			else if (npc[creature[c].npcIndex].type==kNPCBrigand)
				strcpy(creatureName, "Brigand");
			else if (npc[creature[c].npcIndex].type==kNPCMercenary)
				strcpy(creatureName, "Mercenary");
			else*/
		strcpy(creatureName,"Human");

	//}
	else if (c==kGazer)										strcpy(creatureName,"Gaper");
	else if (c==kAirElemental)						strcpy(creatureName,"Air Elemental");
	else if (c==kWaterElemental)					strcpy(creatureName,"Water Elemental");
	else if (c==kFireElemental)						strcpy(creatureName,"Fire Elemental");
	else if (c==kPoisonElemental)					strcpy(creatureName,"Poison Elemental");
	//Dave
	else if (c==kEarthElemental)					strcpy(creatureName,"Earth Elemental");
	else if (c==kSeaTurtle)								strcpy(creatureName,"Sea Turtle");
	else if (c==kSkeleton)								strcpy(creatureName,"Skeleton");
	else if (c==kSkeletonArcher)								strcpy(creatureName,"Skeleton Archer");
	else if (c==kSlug)										strcpy(creatureName,"Sludge Beast");
	else if (c==kHellHound)								strcpy(creatureName,"Hell Hound");
	else if (c==kGreyWolf)								strcpy(creatureName,"Wolf");
	else if (c==kBrownWolf)								strcpy(creatureName,"Wolf");
	else if (c==kBlackWolf)								strcpy(creatureName,"Wolf");
	else if (c==kGreyDog)									strcpy(creatureName,"Dog");
	else if (c==kBrownDog)								strcpy(creatureName,"Dog");
	else if (c==kBlackDog)								strcpy(creatureName,"Dog");
	else if (c==kRedDragon)								strcpy(creatureName,"Red Dragon");
	else if (c==kBlackDragon)							strcpy(creatureName,"Black Dragon");
	else if (c==kPinkDragon)							strcpy(creatureName, "Abyss Dragon");
	else if (c==kIceDragon)								strcpy(creatureName,"Ice Dragon");
	else if (c==kForestDragon)						strcpy(creatureName,"Forest Dragon");
	else if (c==kReaper)									strcpy(creatureName,"Bracken");
	else if (c==kSheep)										strcpy(creatureName,"Sheep");
	else if (c==kScorpion)							  strcpy(creatureName,"Giant Scorpion");
	else if (c==kHarpie)							  	strcpy(creatureName,"Harpie");
	else if (c==kBoneMage)							  strcpy(creatureName,"Bone Mage");
	else if (c==kRat)							        strcpy(creatureName,"Giant Rat");
	else if (c==kLizardMan)							  strcpy(creatureName,"Lizard Man");
	else if (c==kBat)							        strcpy(creatureName,"Giant Bat");
	else if (c==kSpider)							    strcpy(creatureName,"Giant Spider");
	else if (c==kFrostSpider)							strcpy(creatureName,"Frost Spider");
	else if (c==kLavaSpider)							strcpy(creatureName,"Lava Spider");
	else if (c==kSandSpider)							strcpy(creatureName,"Sand Spider");
	else if (c==kSnail)										strcpy(creatureName,"Giant Snail");
	else if (c==kAnt)											strcpy(creatureName,"Giant Ant");
	else if (c==kWorkAnt)											strcpy(creatureName,"Worker Ant");
	else if (c==kWarriorAnt)											strcpy(creatureName,"Warrior Ant");
	else if (c==kForagerAnt)											strcpy(creatureName,"Forager Ant");
	else if (c==kTroll)									  strcpy(creatureName,"Troll");
	else if (c==kForestTroll)							strcpy(creatureName,"Forest Troll");
	else if (c==kGiant)									  strcpy(creatureName,"Giant");
	else if (c==kHillGiant)								strcpy(creatureName,"Hill Giant");
	else if (c==kFrostGiant)							strcpy(creatureName,"Frost Giant");
	else if (c==kElderCentaur)									strcpy(creatureName,"Elder Centaur");
	else if (c==kCentaur)									strcpy(creatureName,"Centaur");
	else if (c==kSerpent)									strcpy(creatureName,"Sea Serpent");
	else if (c==kZombie)									strcpy(creatureName,"Zombie");
	else if (c==kRustBeast)								strcpy(creatureName,"Rust Beast");
	else if (c==kPoisonBeast)							strcpy(creatureName,"Poison Beast");
	else if (c==kBrigandShelter1)					strcpy(creatureName,"Shelter");
	else if (c==kBrigandShelter2)					strcpy(creatureName,"Shelter");
	else if (c==kBrigandShelter3)					strcpy(creatureName,"Shelter");
	else if (c==kQueenAnt)					strcpy(creatureName,"Queen Ant");
	else if (c==kSnake)					strcpy(creatureName,"Giant Snake");
	else if (c==kBlinkHound)					strcpy(creatureName,"Blink Hound");
	else if (c==kDwarf)				strcpy(creatureName,"Dwarf");
	else if (c==kCube)					strcpy(creatureName,"Gooey");
	else if (c==kRedDraco)					strcpy(creatureName,"Red Draco");
	else if (c==kIceDraco)					strcpy(creatureName,"Ice Draco");
	else if (c==kForestDraco)					strcpy(creatureName,"Forest Draco");
	else if (c==kBlackDraco)					strcpy(creatureName,"Black Draco");
	else if (c==kTusker)					strcpy(creatureName,"Tusker");
	else if (c==kBlackDragonAncient)					strcpy(creatureName,"Ancient Black Dragon");
	else if (c==kIceDragonAncient)					strcpy(creatureName,"Ancient Ice Dragon");
	else if (c==kRedDragonAncient)					strcpy(creatureName,"Ancient Red Dragon");
	else if (c==kForestDragonAncient)					strcpy(creatureName,"Ancient Forest Dragon");
	else if (c==kLich)									strcpy(creatureName,"Lich");
	else if (c==kLion)									strcpy(creatureName,"Lion");
	else if (c==kGolem)									strcpy(creatureName,"Stone Golem");
	//Dave
	else if (c==kIceGolem)									strcpy(creatureName,"Ice Golem");
	else if (c==kSandGolem)									strcpy(creatureName,"Sand Golem");
	else if (c==kMagmaGolem)									strcpy(creatureName,"Magma Golem");
	else if (c==kPixie)							  			strcpy(creatureName,"Pixie");
	else if (c==kSandDragon)							  		strcpy(creatureName,"Sand Dragon");
	else if (c==kSandDraco)							  		strcpy(creatureName,"Sand Draco");
	else if (c==kPhaseSerpent)							  		strcpy(creatureName,"Phase Serpent");
	else if (c==kBlackBear)							  		strcpy(creatureName,"Black Bear");
	else if (c==kBrownBear)							  		strcpy(creatureName,"Brown Bear");
	else if (c==kShark)							  		strcpy(creatureName,"Shark");
	else if (c==kLizardManShaman)							  		strcpy(creatureName,"Lizard Shaman");
	else if (c==kHellMage)							  		strcpy(creatureName,"Hell Mage");
	else if (c==kWrath)									strcpy(creatureName,"Wrath");
	//else if (c==kSnapTurtle)							  	strcpy(creatureName,"Snapping Turtle");
	
	
	else strcpy(creatureName,"<unknown>");

}

// ----------------------------------------------------------------- //
void tx_GetSkillName(int skill, char skillName[kStringLength])
// ----------------------------------------------------------------- //

{

	if (skill==kAlchemyDruid)											strcpy(skillName,"Alchemy/Druid");
	else if (skill==kAlchemyCleric)								strcpy(skillName,"Alchemy/Cleric");
	else if (skill==kPoisoning)								strcpy(skillName,"Poisoning");
	else if (skill==kAnatomy)											strcpy(skillName,"Anatomy");
	else if (skill==kArchery)											strcpy(skillName,"Archery");
	else if (skill==kBlacksmithing)								strcpy(skillName,"Blacksmithing");
	else if (skill==kCooking)											strcpy(skillName,"Cooking");
	else if (skill==kDetectingHidden)							strcpy(skillName,"Detecting Hidden");
	else if (skill==kEnchanting)										strcpy(skillName,"Enchanting");
	else if (skill==kFishing)											strcpy(skillName,"Fishing");
	else if (skill==kHealing)											strcpy(skillName,"Healing");
	else if (skill==kHiding)											strcpy(skillName,"Hiding");
	else if (skill==kLockpicking)									strcpy(skillName,"Lockpicking");
	else if (skill==kLumberjacking)								strcpy(skillName,"Lumberjacking");

	//else if (skill==kMaceFighting)								strcpy(skillName,"Mace Fighting");
	else if (skill==kMageryDruid)									strcpy(skillName,"Magery/Druid");
	else if (skill==kMageryCleric)								strcpy(skillName,"Magery/Cleric");
	else if (skill==kMageryWizard)								strcpy(skillName,"Magery/Wizard");
	else if (skill==kMeditation)									strcpy(skillName,"Meditation");
	else if (skill==kMelee)										strcpy(skillName,"Melee");
	else if (skill==kMining)											strcpy(skillName,"Mining");
	else if (skill==kTracking)									strcpy(skillName,"Tracking");
	else if (skill==kParrying)										strcpy(skillName,"Parrying");

	//else if (skill==kPoleFighting)								strcpy(skillName,"Pole Fighting");
	else if (skill==kRemoveTrap)									strcpy(skillName,"Removing Traps");
	else if (skill==kResistingMagic)							strcpy(skillName,"Resisting Magic");
	else if (skill==kSpecialFighter)									strcpy(skillName,"Special/Fighter");
	else if (skill==kSpecialRanger)									strcpy(skillName,"Special/Ranger");
	else if (skill==kSpecialRogue)									strcpy(skillName,"Special/Rogue");

	//else if (skill==kSwordsmanship)								strcpy(skillName,"Swordsmanship");
	//else if (skill==kDaggers)								strcpy(skillName,"Daggers");
	else if (skill==kTailoringLeatherworking)			strcpy(skillName,"Tailoring");
	else if (skill==kTaming)									  	strcpy(skillName,"Taming");
	else if (skill==kStealth)										strcpy(skillName,"Stealth");
	else if (skill==kTinkering)										strcpy(skillName,"Tinkering");
	else if (skill==kWoodworking)									strcpy(skillName,"Woodcrafting");
	else if (skill==kWrestling)										strcpy(skillName,"Wrestling");
	else strcpy(skillName,"<unknown>");

}

// ----------------------------------------------------------------- //
void tx_CreatureTalkText(int c1, int c2, int situation)
// ----------------------------------------------------------------- //

{
	char            talk[kStringLength];
	int             r;
	int							hits[kMaxTalkScripts];
	int							hitPtr=0;
	int							i;
	TBoolean				overridePause=false;

	if (creature[c1].id==kHuman)
		return;

	if (creature[c1].mute)
		return;

	if (situation==creature[c1].previousSituation)
		return; //09/04/07  Laying low on creature speech

	if ((situation==kKillingTalk) && (creature[c1].previousSituation!=kKillingTalk))
		overridePause=true;

	if ((situation==kJustHidTalk) && (creature[c1].previousSituation!=kJustHidTalk))
		overridePause=true;

	if ((situation==kChasingTalk) && (creature[c1].previousSituation==kJustHidTalk))
		overridePause=true;

	if ((situation==kChasingTalk) && (creature[c1].previousSituation==kHidingTalk))
		overridePause=true;

	if ((situation==kAttackingTalk) && (creature[c1].previousSituation==kJustHidTalk))
		overridePause=true;

	if ((situation==kAttackingTalk) && (creature[c1].previousSituation==kHidingTalk))
		overridePause=true;

	strcpy(talk,"");

	if (!overridePause)
		if (creature[c1].talkTime!=0)
			//if (!((creature[c1].previousSituation==0) || (creature[c1].previousSituation!=situation)))
			if (TickCount()<(creature[c1].talkTime+(5*60)))//was 15*60 speech time
				return;

	//-----------------------------------------------------//

	creature[c1].previousSituation=situation;

	if (creature[c2].master!=0)
	{

		for (i=0;i<kMaxTalkScripts;i++)
			if (strcmp(creatureInfo[creature[c1].id].talkScript[i].say,"")!=0)
			{
				if (creatureInfo[creature[c1].id].talkScript[i].state!=situation)
					continue;

				if (creatureInfo[creature[c1].id].talkScript[i].target!=kPetTarget)
					continue;

				hits[hitPtr]=i;

				hitPtr++;
			}

		if (hitPtr>0)
		{
			r=tb_Rnd(0,(hitPtr-1));
			strcpy(talk,creatureInfo[creature[c1].id].talkScript[hits[r]].say);
		}

	}

	if (creature[c2].id==kHuman)
	{

		for (i=0;i<kMaxTalkScripts;i++)
			if (strcmp(creatureInfo[creature[c1].id].talkScript[i].say,"")!=0)
			{
				if (creatureInfo[creature[c1].id].talkScript[i].state!=situation)
					continue;

				if (situation!=kRetreatingTalk)
					if (creatureInfo[creature[c1].id].talkScript[i].target!=kHumanTarget)
						if (creatureInfo[creature[c1].id].talkScript[i].target!=player[creature[c2].playerIndex].characterClass)
							continue;

				hits[hitPtr]=i;

				hitPtr++;
			}

		if (hitPtr>0)
		{
			r=tb_Rnd(0,(hitPtr-1));
			strcpy(talk,creatureInfo[creature[c1].id].talkScript[hits[r]].say);
		}

	}

	//-----------------------------------------------------//

	if (strcmp(talk,"")!=0)
	{
		creature[c1].talkWhen			= TickCount();
		creature[c1].talkTime			= TickCount();
		creature[c1].talkID			  = chatMessageID;
		chatMessageID++;
		strcpy(creature[c1].talk,talk);
		creature[c1].hiding=false;
	}

}
