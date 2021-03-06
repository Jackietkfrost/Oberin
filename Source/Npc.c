#include "Globals.h"

#include "Toolbox.h"
#include "Common.h"
#include "Npc.h"
#include "Items.h"
#include "Creatures.h"
#include "Players.h"
#include "Skills.h"
#include "FileIO.h"

#define asking(q)															(strstr(creature[player[i].creatureIndex].talk,q)!=NULL)

npcType														npc								[kMaxNPCs];
npcQuestionType										npcQuestion				[kMaxNPCQuestions];
npcTradeType											npcTrade					[kMaxNPCTrades];

extern		creatureType						creature					[kMaxCreatures];
extern		UInt16									creatureMap				[kMapWidth][kMapHeight];
extern		playerType							player						[kMaxPlayers];
extern 		UInt32                  chatMessageID;

int				currentQuestion;
int				currentTrade;
//-----------------------------------------------

// ----------------------------------------------------------------- //
void np_Converse(int i)
// ----------------------------------------------------------------- //

{
	int					row;
	int					col;
	int					theNPC;
	int					theCreature;
	//int					theId;
	TBoolean		response=false;

	for (row=-2;row<=2;row++)
		for (col=-2;col<=2;col++)
		{
			if (creature[creatureMap[creature[player[i].creatureIndex].row+row][creature[player[i].creatureIndex].col+col]].npcIndex!=0)
			{
				theCreature=creatureMap[creature[player[i].creatureIndex].row+row][creature[player[i].creatureIndex].col+col];
				theNPC=creature[theCreature].npcIndex;
				//creature[creatureMap[creature[player[i].creatureIndex].row+row][creature[player[i].creatureIndex].col+col]].npcIndex;


				if (!creature[theCreature].mute)
					np_GetReponse(theNPC,i,theCreature);

				//if (response)
				//	npc[theNPC].talkTime	=	TickCount();

				return;
			}
		}

}

// ----------------------------------------------------------------- //
void np_ClearAllNPCs(void)
// ----------------------------------------------------------------- //

{
	int						i;
	int						j;

	np_InitNPCs();

	for (i=5000;i<kMaxCreatures;i++)
		if (creature[i].npcIndex!=0)
		{

			for (j=0;j<kMaxCreatureEffects;j++)
				if (creature[i].fx[j]>0)
				{
					serverFX[creature[i].fx[j]]	=	0;
					creature[i].fx[j]						=	0;
					creature[i].fxKillTime[j]		=	0;
					creature[i].fxTarget[j]			=	0;
					creature[i].fxTargetRow[j]	=	0;
					creature[i].fxTargetCol[j]	=	0;
					creature[i].activeFX				= false;
				}

			creatureMap[creature[i].row][creature[i].col]	= 0;

			cr_ClearCreatureRecord(i);
		}

}

// ----------------------------------------------------------------- //
void np_DressupNpc(int i, int p)
// ----------------------------------------------------------------- //
{
	int dress=player[i].npcDressup;
	int j;
	//int theColor;
	
	if (creature[p].npcIndex==0)
		return;
	
	j=creature[p].npcIndex;
	
	if (dress==9990) //remove torso
	{
		npc[j].torso=0;
		return;
	}
	
	if (dress==9991) //remove legs
	{
		npc[j].legs=0;
		return;
	}
	
	if (dress==9998) //change sex, male
	{
		npc[j].NPCSex=0;
		return;
	}
	
	if (dress==9999) //change sex, female
	{
		npc[j].NPCSex=1;
		return;
	}
	
	if (itemList[dress].itemType==kDyeType) //change hair color
	{
		npc[j].NPCHairColor=itemList[dress].itemColor;
		return;
	}
	
	switch (itemList[dress].useLocation)
	{		
		case kLegs:
			npc[j].legs=dress;
			if (itemList[npc[j].torso].itemType==kRobeType)	npc[j].torso=0;
			return;
		break;
		
		case kTorso:
			npc[j].torso=dress;
			if (itemList[dress].itemType==kRobeType)	npc[j].legs=0;
			return;
		break;
	}
	
	nw_SendTextToClient(i, "Invalid item", 0);
}

// ----------------------------------------------------------------- //
void np_InitNPCs(void)
// ----------------------------------------------------------------- //

{
	int					i;

	currentQuestion=0;
	currentTrade=0;

	for (i=0;i<kMaxNPCs;i++)
	{
		npc[i].startRow								= 0;
		npc[i].startCol								= 0;
		npc[i].torso									= 0;
		npc[i].legs										= 0;
		npc[i].range									= 0;
		strcpy(npc[i].talk, "");
		npc[i].talkTime								=	0;
		npc[i].type								=0;
		
	}

	for (i=0;i<kMaxNPCQuestions;i++)
	{
		npcQuestion[i].id								= 0;
		strcpy(npcQuestion[i].question, "");
		strcpy(npcQuestion[i].answer, "");
	}

	for (i=0;i<kMaxNPCTrades;i++)
	{
		npcTrade[i].id								= 0;
		npcTrade[i].giveItem					= 0;
		npcTrade[i].giveCount					= 0;
		npcTrade[i].takeItem					= 0;
		npcTrade[i].takeCount					= 0;
		npcTrade[i].experience				= 0;
		strcpy(npcTrade[i].response,"");
	}

}

// ----------------------------------------------------------------- //
int np_SpawnNPC(int id, UInt16 row, UInt16 col, UInt8 range, UInt8 torso, UInt8 legs, UInt8 theType, UInt16 theSex, UInt16 hairstyle, UInt16 haircolor, UInt16 spawnId, TBoolean spawned)
// ----------------------------------------------------------------- //

{
	int					i;
	int					gold;
	int					gold1;
	int					gold2;
	
	i=cr_SpawnCreature(spawnId, row, col);

	if (i>=0)
	{
		creature[i].respawn				= 	false;
		creature[i].respawnTime			= 	0;
		creature[i].taunted				=	0;
		creature[i].npcIndex			=	id;
		creature[i].level			    =	1;
		creature[i].alignment			=	kGood;
		creature[i].magicResist			=	0;
		npc[id].torso					=	torso;
		npc[id].legs					=	legs;
		npc[id].startRow				=	row;
		npc[id].startCol				=	col;
		npc[id].range					=	range;
		npc[id].NPCSex					=	theSex;
		npc[id].NPCHairStyle			=	hairstyle;
		npc[id].NPCHairColor			=	haircolor;
		npc[id].spawned					=	spawned;
		npc[id].forceMove				=	false;


		//inventory setup

		npc[id].type=theType;

		if (theType==kNPCBrigand) //brigand //Dave edit gold=0;
		{
			creature[i].alignment=kEvil;
			gold=tb_Rnd(50,100);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;
			creature[i].magicResist=0.8;
			
			//in_AddtoInventory(i, kVikingHelmet, 127);
			in_AddtoInventory(i, kLongSword, 127);
			//in_AddtoInventory(i, kWoodenShield, 127);
			in_AddtoInventory(i, kBrigandHead, 1);
		}
		else if (theType==kNPCMercenary) //Mercenary
		{
			creature[i].alignment=kEvil;
			gold=tb_Rnd(100,500);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;
			creature[i].magicResist=0.5;
			//in_AddtoInventory(i, kChainMailHelmet, 127);
			in_AddtoInventory(i, kGlaive, 127);
		}
		else if (theType==kNPCGuard)
		{
			gold=tb_Rnd(10,30);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;

			in_AddtoInventory(i, kLongSword, 127);
		}
		//Dave
		else if (theType==kNPCPrivateer) //Privateer
		{
			creature[i].alignment=kEvil;
			gold=tb_Rnd(500,800);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;
			creature[i].magicResist=0.2;
		
			in_AddtoInventory(i, kKatana, 127);
			in_AddtoInventory(i, kNPCPrivateerHead, 1);
		}
		else if (theType==kNPCDwarf1) //Privateer
		{
			creature[i].alignment=kEvil;
			gold=tb_Rnd(50,100);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;
			creature[i].magicResist=0.2;
		
			//in_AddtoInventory(i, kVikingHelmet, 127);
			in_AddtoInventory(i, kLongSword, 127);
			//in_AddtoInventory(i, kWoodenShield, 127);
			in_AddtoInventory(i, kBrigandHead, 1);
			//in_AddtoInventory(i, kFireSword, 127);
			//in_AddtoInventory(i, kNPCPrivateerHead, 1);
		}
		else
		{
			gold=tb_Rnd(1,3);
			in_SetGoldTotal(gold, &gold1, &gold2);
			creature[i].inventory[0].itemID			=	gold1;
			creature[i].inventory[0].itemCount	=	gold2;
		}

		//in_AddtoInventory(i, torso, 127);
		//in_AddtoInventory(i, legs, 127);
		//in_AddtoInventory(i, kBlackBoots, 1);

		return i;
	}

	return -1;

}

// ----------------------------------------------------------------- //
void np_DumpNPCInfo(int i, int c)
// ----------------------------------------------------------------- //

{
	char	test[kStringLength];
	char	test2[kStringLength];

	int		j=creature[c].npcIndex;

	if (pl_AuthorizedTo(player[i].creatureIndex, kRegularPlayer))
		return;

	tb_NumToString(j, test);

	strcat(test, " ; name=");

	strcat(test, npc[j].name);

	strcat(test, " ; startrow=");
	
	tb_NumToString(npc[j].startRow, test2);

	strcat(test, test2);

	strcat(test, " ; startcol=");

	tb_NumToString(npc[j].startCol, test2);

	strcat(test, test2);

	strcat(test, " ; range=");

	tb_NumToString(npc[j].range, test2);

	strcat(test, test2);
	
	strcat(test, " ; type=");

	tb_NumToString(npc[j].type, test2);

	strcat(test, test2);
	
	strcat(test, " ; sex=");

	tb_NumToString(npc[j].NPCSex, test2);

	strcat(test, test2);
	
	strcat(test, " ; hairstyle=");

	tb_NumToString(npc[j].NPCHairStyle, test2);

	strcat(test, test2);
	
	strcat(test, " ; haircolor=");

	tb_NumToString(npc[j].NPCHairColor, test2);

	strcat(test, test2);
	
	strcat(test, " ; RM=");

	tb_NumToString(npc[j].magicResist, test2);

	strcat(test, test2);

	
	
	nw_SendTextToClient(i, test, 0);

}


//---------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------- //
void np_GetReponse(int id,int i,int theCreature)
// ----------------------------------------------------------------- //

{
	char									talk[kStringLength];
	int										j;

	if (id>=kMaxNPCs)
		return;

	strcpy(talk,"");

	for (j=0;j<kMaxNPCQuestions;j++)
		if (npcQuestion[j].id==id)
		{
			if (asking(npcQuestion[j].question))
			{
				strcpy(talk, npcQuestion[j].answer);
				break;
			}
		}
		else if (npcQuestion[j].id>id)
			break;


	if (strcmp(talk,"")!=0)
	{
		creature[theCreature].talkWhen			= TickCount()+20;//45;
		creature[theCreature].talkTime			= TickCount();
		creature[theCreature].talkID			  = chatMessageID;
		chatMessageID++;
		strcpy(creature[theCreature].talk,talk);
		creature[theCreature].hiding=false;
	}

}

// ----------------------------------------------------------------- //
void np_AddQuestion(int id, char question[kStringLength], char answer[kStringLength])
// ----------------------------------------------------------------- //

{	
	if (id>=kMaxNPCs)
		return;

	if (currentQuestion>=kMaxNPCQuestions)
		return;

	npcQuestion[currentQuestion].id	= id;

	question[kNPCQuestionLength-1]='/0';

	strcpy(npcQuestion[currentQuestion].question, question);

	strcpy(npcQuestion[currentQuestion].answer, answer);

	tb_MakeLowerCase(npcQuestion[currentQuestion].question);

	currentQuestion++;

}

// ----------------------------------------------------------------- //
int np_QuestComplete(int thePlayer, int theNPC, int theItem, int theCount)
// 0 = quest complete
// 1 = quest complete but pack full
// 2 = doesn't want that item
// ----------------------------------------------------------------- //

{
	int							i;
	int							j;
	int							k;
	int							itemsAdded;
	//int							index;
	char						name[kStringLength];
	char						temp[kStringLength];
	char						rewardStr[kStringLength];
	char						expStr[kStringLength];
	int							theIndex;
	float       				split=1;
	int         				theFriend[17];
	int         				fPtr;
	float       				shareExperience;
	float       				shareExperienceRoundOff;

	for (i=0;i<=currentTrade;i++)
		if (npcTrade[i].id==theNPC)
			if (in_CanGroup(npcTrade[i].giveItem))
			{

				for (j=0;j<17;j++)
					theFriend[j]=0;

				theFriend[0]=thePlayer;

				fPtr=1;

				if (player[thePlayer].sharing)
				{
					for (k=1;k<kMaxPlayers;k++)
						if (player[k].online)
							for (j=0;j<16;j++)
								if (strcmp(player[thePlayer].friendName[j],player[k].playerNameLC)==0)
									if (strcmp(player[thePlayer].friendName[j],player[thePlayer].playerNameLC)!=0)
									{
										theFriend[fPtr]=k;
										fPtr++;
										split++;
									}
				}

				shareExperience= tb_Trunc(npcTrade[i].experience/split);

				shareExperienceRoundOff = npcTrade[i].experience-(shareExperience*split);

				if ((npcTrade[i].giveItem==theItem) && (npcTrade[i].giveCount==theCount))
				{

					if (npcTrade[i].takeItem!=0)
					{
						if (itemList[npcTrade[i].takeItem].itemType==kSpellType) //Won a spell
						{
							for (k=5000; k<kMaxCreatures; k++)
								if (creature[k].npcIndex==theNPC)
									theIndex=k;

							sk_CastSpellOnCreature(theIndex, player[thePlayer].creatureIndex, npcTrade[i].takeItem, false);
						}
						else if (in_CanGroup(npcTrade[i].takeItem))
						{
							itemsAdded=in_AddtoInventory(player[thePlayer].creatureIndex, npcTrade[i].takeItem, npcTrade[i].takeCount);

							if (itemsAdded>0)
								nw_SendInventoryAdd(thePlayer, npcTrade[i].takeItem, npcTrade[i].takeCount, kRewardItem,0);
							else
							{
								return (1);
							}
						}
						else
						{
							for (j=0;j<npcTrade[i].takeCount;j++)
							{
								itemsAdded=in_AddtoInventory(player[thePlayer].creatureIndex, npcTrade[i].takeItem, 127);

								if (itemsAdded>0)
									nw_SendInventoryAdd(thePlayer, npcTrade[i].takeItem, 127, kRewardItem,0);
								else
								{
									return (1);
								}
							}
						}
					}

					if (npcTrade[i].experience>0)
					{
						//player[thePlayer].title=player[thePlayer].title+npcTrade[i].experience;
						//strcpy(rewardStr, "You gain ");
						//tb_IntegerToString(npcTrade[i].experience, expStr);
						//strcat(rewardStr, expStr);
						//strcat(rewardStr, " experience points.");
						//nw_SendTextToClient(thePlayer, rewardStr, 0);

						// -------------------- divy up experience points

						if (npcTrade[i].experience<split) // not enough exp to split
						{
							player[thePlayer].title=player[thePlayer].title+npcTrade[i].experience;
							strcpy(rewardStr, "You gain ");
							tb_IntegerToString(npcTrade[i].experience, expStr);
							strcat(rewardStr, expStr);
							strcat(rewardStr, " experience points.");
							nw_SendTextToClient(thePlayer, rewardStr, 0);
						}
						else
						{
							for (j=0;j<17;j++)
								if (theFriend[j]!=0)
								{
									player[theFriend[j]].title=player[theFriend[j]].title+shareExperience;
									tb_IntegerToString(shareExperience, expStr);

									if (j==0)
									{
										player[theFriend[j]].title=player[theFriend[j]].title+shareExperienceRoundOff;
										tb_IntegerToString(shareExperience+shareExperienceRoundOff, expStr);
										strcpy(rewardStr, "You gain ");
									}
									else
									{
										strcpy(rewardStr, player[thePlayer].playerName);
										strcat(rewardStr, " completed a quest!");
										nw_SendTextToClient(theFriend[j],rewardStr,0);
										strcpy(rewardStr, "Shared experience: ");
									}

									strcat(rewardStr, expStr);

									strcat(rewardStr, " experience points.");
									nw_SendTextToClient(theFriend[j], rewardStr, 0);
								}
						}
					}


					if (strcmp(npcTrade[i].response,"")!=0)
					{
						strcpy(name,"(");
						np_GetNPCName(theNPC, temp);
						strcat(name,temp);
						strcat(name,"): ");
						strcat(name,npcTrade[i].response);
						nw_SendTextToClient(thePlayer, name, 0);
					}

					return (0);
				}
			}
			else
			{
				if (npcTrade[i].giveItem==theItem)
				{

					for (j=0;j<17;j++)
						theFriend[j]=0;

					theFriend[0]=thePlayer;

					fPtr=1;

					if (player[thePlayer].sharing)
					{
						for (k=1;k<kMaxPlayers;k++)
							if (player[k].online)
								for (j=0;j<16;j++)
									if (strcmp(player[thePlayer].friendName[j],player[k].playerNameLC)==0)
										if (strcmp(player[thePlayer].friendName[j],player[thePlayer].playerNameLC)!=0)
										{
											theFriend[fPtr]=k;
											fPtr++;
											split++;
										}
					}

					shareExperience= tb_Trunc(npcTrade[i].experience/split);

					shareExperienceRoundOff = npcTrade[i].experience-(shareExperience*split);

					if (npcTrade[i].takeItem!=0)
					{
						if (itemList[npcTrade[i].takeItem].itemType==kSpellType) //Won a spell
						{
							for (k=0; k<kMaxCreatures; k++)
								if (creature[k].npcIndex==theNPC)
									break;

							sk_CastSpellOnCreature(k, player[thePlayer].creatureIndex, theItem, false);
						}

						else if (in_CanGroup(npcTrade[i].takeItem))
						{
							itemsAdded=in_AddtoInventory(player[thePlayer].creatureIndex, npcTrade[i].takeItem, npcTrade[i].takeCount);

							if (itemsAdded>0)
								nw_SendInventoryAdd(thePlayer, npcTrade[i].takeItem, npcTrade[i].takeCount, kRewardItem,0);
							else
								{
									return (1);
								}
						}
						else
						{
							for (j=0;j<npcTrade[i].takeCount;j++)
							{
								itemsAdded=in_AddtoInventory(player[thePlayer].creatureIndex, npcTrade[i].takeItem, 127);

								if (itemsAdded>0)
									nw_SendInventoryAdd(thePlayer, npcTrade[i].takeItem, 127, kRewardItem,0);
								else
								{
									return (1);
								}
							}
						}
					}

					if (npcTrade[i].experience>0)
					{

						//player[thePlayer].title=player[thePlayer].title+npcTrade[i].experience;
						//strcpy(rewardStr, "You gain ");
						//tb_IntegerToString(npcTrade[i].experience, expStr);
						//strcat(rewardStr, expStr);
						//strcat(rewardStr, " experience points.");
						//nw_SendTextToClient(thePlayer, rewardStr, 0);

						// -------------------- divy up experience points

						if (npcTrade[i].experience<split) // not enough exp to split
						{
							player[thePlayer].title=player[thePlayer].title+npcTrade[i].experience;
							strcpy(rewardStr, "You gain ");
							tb_IntegerToString(npcTrade[i].experience, expStr);
							strcat(rewardStr, expStr);
							strcat(rewardStr, " experience points.");
							nw_SendTextToClient(thePlayer, rewardStr, 0);
						}
						else
						{
							for (j=0;j<17;j++)
								if (theFriend[j]!=0)
								{
									player[theFriend[j]].title=player[theFriend[j]].title+shareExperience;
									tb_IntegerToString(shareExperience, expStr);

									if (i==0)
									{
										player[theFriend[j]].title=player[theFriend[j]].title+shareExperienceRoundOff;
										tb_IntegerToString(shareExperience+shareExperienceRoundOff, expStr);
										strcpy(rewardStr, "You gain ");
									}
									else
									{
										strcpy(rewardStr, player[thePlayer].playerName);
										strcat(rewardStr, " completed a quest!");
										nw_SendTextToClient(theFriend[j],rewardStr,0);
										strcpy(rewardStr, "Shared experience: ");
									}

									strcat(rewardStr, expStr);

									strcat(rewardStr, " experience points.");
									nw_SendTextToClient(theFriend[j], rewardStr, 0);
								}
						}
					}

					if (strcmp(npcTrade[i].response,"")!=0)
					{
						strcpy(name,"(");
						np_GetNPCName(theNPC, temp);
						strcat(name,temp);
						strcat(name,"): ");
						strcat(name,npcTrade[i].response);
						nw_SendTextToClient(thePlayer, name, 0);
					}

					return (0);
				}
			}

	return (2);

}

// ----------------------------------------------------------------- //
void np_AddTrade(int id, int giveCount, int giveItem, int takeCount, int takeItem, int experience, char response[kStringLength])
// ----------------------------------------------------------------- //

{

	if (id>=kMaxNPCs)
		return;

	if (currentTrade>=kMaxNPCTrades)
		return;

	npcTrade[currentTrade].id								= id;

	npcTrade[currentTrade].giveItem					= giveItem;

	npcTrade[currentTrade].giveCount				= giveCount;

	npcTrade[currentTrade].takeItem					= takeItem;

	npcTrade[currentTrade].takeCount				= takeCount;

	npcTrade[currentTrade].experience				= experience;

	strcpy(npcTrade[currentTrade].response,response);

	currentTrade++;

}

// ----------------------------------------------------------------- //
void np_GetNPCName(int id, char name[kStringLength])
// ----------------------------------------------------------------- //

{

	if (id<kMaxNPCs)
		strcpy(name,npc[id].name);
	else
		strcpy(name,"");

}

// ----------------------------------------------------------------- //
TBoolean np_IsRegularNPC(int i)
// ----------------------------------------------------------------- //

{

	if (npc[creature[i].npcIndex].type==kNPCRegular)
		return (true);

	return (false);

}

// ----------------------------------------------------------------- //
void np_CheckNPCs()
// ----------------------------------------------------------------- //

{
	int c;
	
	char debug[255];
	
	
	for (c=0; c<kMaxNPCs; c++)
	{
		if (npc[c].torso!=0)
		{
			if (npc[c].startRow<=0 || npc[c].startRow>=kMapWidth)
			{
				fi_WriteToErrorLog("Bad startRow");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].startRow, debug);
				fi_WriteToErrorLog(debug);	
			};
			
			if (npc[c].startCol<=0 || npc[c].startCol>=kMapWidth)
			{
				fi_WriteToErrorLog("Bad startCol");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].startCol, debug);
				fi_WriteToErrorLog(debug);	
			};
			
			if (npc[c].legs>255)
			{
				fi_WriteToErrorLog("Bad legs");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].legs, debug);
				fi_WriteToErrorLog(debug);
			}
			
			if (npc[c].torso>255)
			{
				fi_WriteToErrorLog("Bad legs");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].legs, debug);
				fi_WriteToErrorLog(debug);
			}
			
			if (npc[c].range==0 || npc[c].range>100)
			{
				fi_WriteToErrorLog("Bad range");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].range, debug);
				fi_WriteToErrorLog(debug);
			}
			
			if (npc[c].type>4)
			{
				fi_WriteToErrorLog("Bad type");
				fi_WriteToErrorLog(npc[c].name);
				tb_NumToString(npc[c].range, debug);
				fi_WriteToErrorLog(debug);
			}
			
		}
	}

}
