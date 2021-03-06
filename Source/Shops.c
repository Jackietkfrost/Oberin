#include "Inventory.h"
#include "Toolbox.h"
#include "Common.h"
#include "Creatures.h"
#include "Players.h"
#include "Graphics.h"
#include "Items.h"
#include "Shops.h"
#include "FileIO.h"
#include "Npc.h"

commerceType				shopInventory				[kMaxInventorySize];
commerceType				sellList						[kMaxInventorySize];

commerceType				shop								[kMaxShops][kMaxInventorySize];
//int									shopID1							[kMaxShops];
//int									shopID2							[kMaxShops];

TRect								shopRect						[kMaxShops];
char								traderName					[kMaxShops][48];
TRect								stableRect					[kMaxStables];

unsigned long				nextInventoryRotate;

extern UInt16				map[kMapWidth][kMapHeight];
extern creatureType	creature						[kMaxCreatures];
extern int									rotateShopTime; // beta

#ifdef _SERVER
extern UInt16									creatureMap				[kMapWidth][kMapHeight];
#endif

// ----------------------------------------------------------------- //
TBoolean sh_InSellList(int item)	// client
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
		if (sellList[i].itemID==item)
			return (true);

	return (false);

}

// ----------------------------------------------------------------- //
int sh_ServerSellPriceOf(int i, int item)	// client
// ----------------------------------------------------------------- //

{
	int										j;
	int										theShop;
	int										cost;

	theShop=sh_GetShopID(i);

	if (theShop==0)
		return (0);

	for (j=0;j<kMaxInventorySize;j++)
	
		/**/
		//Dave add 04-25-2012 store tokens sell for same price to buy
		if (item==kStoreToken)
			//if (sellList[i].itemID==kStoreToken)
			{
				cost			= (shop[theShop][j].cost);
				
				if (cost<=0)
				cost=1;
				
				return (cost);
			}
	
		else if (shop[theShop][j].itemID==item)
			{
				if (in_CanGroup(shop[theShop][j].itemID))
					cost			= (shop[theShop][j].cost/2);
				else
					cost			= (shop[theShop][j].cost/3);
				
				if (cost<=0)
					cost=1;

				return (cost);
			}
		

	return (0);

}

// ----------------------------------------------------------------- //
int sh_SellPriceOf(int item)	// client
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
		if (sellList[i].itemID==item)
			return (sellList[i].cost);

	return (0);

}


// ----------------------------------------------------------------- //
void sh_ClearSellList(void)	// client
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
	{
		sellList[i].itemID			=	0;
		sellList[i].cost				=	0;
	}

}

// ----------------------------------------------------------------- //
void sh_ClearShopContents(void)	// client
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
	{
		shopInventory[i].itemID			=	0;
		shopInventory[i].cost				=	0;
	}

}

// ----------------------------------------------------------------- //
TBoolean sh_AnythingInShopList(void)
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
		if (shopInventory[i].itemID	!= 0)
			return (true);

	return (false);

}

// ----------------------------------------------------------------- //
TBoolean sh_AnythingInSellList(void)
// ----------------------------------------------------------------- //

{
	int					i;

	for (i=0;i<kMaxInventorySize;i++)
		if (sellList[i].itemID	!= 0)
			return (true);

	return (false);

}

#ifdef _SERVER

// ----------------------------------------------------------------- //
TBoolean sh_StandingInInn(int i)	// server - version 0.1.2
// ----------------------------------------------------------------- //

{
	int           r=creature[player[i].creatureIndex].row;
	int           c=creature[player[i].creatureIndex].col;
	int           firstRow;
	int           lastRow;
	int           firstCol;
	int           lastCol;
	int           scanRow;
	int           scanCol;
	TBoolean      isTarget=false;

	/*if ((r>=2189) && (r<=2195))
	  if ((c>=1825) && (c<=1830))
	    return (true);
	  
	if ((r>=2182) && (r<=2193))
	  if ((c>=1827) && (c<=1849))
	    return (true);*/

	firstRow=creature[player[i].creatureIndex].row-7;
	lastRow=creature[player[i].creatureIndex].row+7;
	firstCol=creature[player[i].creatureIndex].col-7;
	lastCol=creature[player[i].creatureIndex].col+7;

	if (firstRow<0)            firstRow=0;

	if (lastRow>=kMapWidth)    lastRow=kMapWidth-1;

	if (firstCol<0)            firstCol=0;

	if (lastCol>=kMapHeight)   lastCol=kMapHeight-1;

	for (scanRow=firstRow;scanRow<=lastRow;scanRow++)
		for (scanCol=firstCol;scanCol<=lastCol;scanCol++)
			if (creatureMap[scanRow][scanCol]!=0)
				if (creature[creatureMap[scanRow][scanCol]].attackTarget==player[i].creatureIndex)
					isTarget=true;

	if (!isTarget)
		return (true);

	return (false);

}

// ----------------------------------------------------------------- //
void sh_RotateShopInventory(void)	// server
// ----------------------------------------------------------------- //

{
	int								j;
	//int								i;
	int								r;
	int								s;
	int								c;
	//int								shopCount;
	int								shopMin;
	//float							costRange;
	//unsigned long			temp;

	if (TickCount()>nextInventoryRotate)
	{

#ifdef _FUNCTIONTRACEDEBUGGING
		fi_WriteToErrorLog("sh_RotateShopInventory Entry");
#endif

		//temp=TickCount();

		for (j=0;j<kMaxShops;j++)
		{

			// -- select items for sale
			shopMin=0;
			r=0;

			while (shopMin<3)
			{
				for (s=0;s<kMaxInventorySize;s++)
					shop[j][s].forSale	=	false;

				shopMin=0;

				r++;

				for (s=0;s<kMaxInventorySize;s++)
					if (shop[j][s].itemID!=0)
					{
						if ((shop[j][s].transactions==kSellOnly) || (shop[j][s].transactions==kBuyAndSell))
							if (tb_Rnd(0,100)<shop[j][s].availibility)
							{
								shop[j][s].forSale	=	true;
								shopMin++;
							}
					}

				if (r>=25) break;
			}

			// -- select items to buy
			for (s=0;s<kMaxInventorySize;s++)
				if (shop[j][s].itemID!=0)
				{
					shop[j][s].willBuy	=	false;

					if ((shop[j][s].transactions==kBuyOnly) || (shop[j][s].transactions==kBuyAndSell))
						if (tb_Rnd(0,100)<shop[j][s].availibility)
							shop[j][s].willBuy	=	true;
				}




			/*
								shopMin=0;
								shopCount=0;
								for (i=0;i<kMaxInventorySize;i++)
									{
										shop[j][i].forSale	=	false;
										shop[j][i].willBuy	=	false;
										if (shop[j][i].itemID!=0)
											{
												costRange=((float)tb_Rnd(90,110))/100;
												shop[j][i].cost = (UInt16)(((float)itemList[shop[j][i].itemID].price)*costRange);
												if (shop[j][i].cost<1)
													shop[j][i].cost=1;
												shopCount++;
												if (shop[j][i].availibility>=100)
													shopMin++;
											}
									}

								if (shopCount!=0)
									{

										if (shopMin<3)
											shopMin=3;

										r=tb_Rnd(shopMin,shopCount);
										while (r>0)
											{
												s=tb_Rnd(0,(shopCount-1));
												if (shop[j][s].itemID!=0)
													{
														if ((shop[j][s].transactions==kSellOnly) || (shop[j][s].transactions==kBuyAndSell))
														  if (tb_Rnd(0,100)<shop[j][s].availibility)
															  shop[j][s].forSale	=	true;
														r--;
													}
											}

										r=tb_Rnd(3,shopCount);
										while (r>0)
											{
												s=tb_Rnd(0,(shopCount-1));
												if (shop[j][s].itemID!=0)
													{
														if ((shop[j][s].transactions==kBuyOnly) || (shop[j][s].transactions==kBuyAndSell))
														  shop[j][s].willBuy	=	true;
														r--;
													}
											}

									}
									*/

			//Update players buy list

			for (c=1; c<5000; c++)  //1..5000 reserved to players
			{
				if (creature[c].id==kHuman)
					if (sh_GetShopID(c)==j)
						nw_SendShopContents(c);
			}

		}

		nextInventoryRotate=TickCount()+(3600*10);	// rotate stock every 10 minutes

		//rotateShopTime=rotateShopTime+(TickCount()-temp);
#ifdef _FUNCTIONTRACEDEBUGGING
		fi_WriteToErrorLog("sh_RotateShopInventory Exit");
#endif

	}


}

#endif

#ifdef _SERVER
// ----------------------------------------------------------------- //
void sh_AddItemToShop(int theShop, int itemID, int transactions, int availibility, float times)	// server
// ----------------------------------------------------------------- //

{
	int								i;

	if (theShop>=kMaxShops)			// beta debug
		fi_WriteToErrorLog("Out of Range - sh_AddItemToShop");

	for (i=0;i<kMaxInventorySize;i++)
		if (shop[theShop][i].itemID==0)
		{
			shop[theShop][i].itemID				= (UInt16) 	itemID;
			shop[theShop][i].cost					= (UInt16) 	(itemList[itemID].price*times);
			shop[theShop][i].transactions	=	(UInt8)		transactions;
			shop[theShop][i].availibility	=	(UInt8)		availibility;
			return;
		}

}

#endif

#ifdef _SERVER
// ----------------------------------------------------------------- //
void sh_AdjustShopPrices(void)	// server
// ----------------------------------------------------------------- //

{
	int								i;
	int								j;

	for (j=0;j<kMaxShops;j++)
		for (i=0;i<kMaxInventorySize;i++)
			if (shop[j][i].itemID!=0)
				shop[j][i].cost	= (UInt16) 	itemList[shop[j][i].itemID].price;

}

#endif

// ----------------------------------------------------------------- //
void sh_InitializeShops(void)	// server
// ----------------------------------------------------------------- //

{


}

#ifdef _SERVER
// ----------------------------------------------------------------- //
TBoolean	sh_StandingNextToTrader(int i)	// server
// ----------------------------------------------------------------- //

{
	int						j;
	char					trader[kNPCNameLength];
	int						row;
	int						col;

	row=creature[player[i].creatureIndex].row;
	col=creature[player[i].creatureIndex].col;

	strcpy(trader,"");

	if ((creature[creatureMap[row-1][col-1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row-1][col-1]].npcIndex].name);

	if ((creature[creatureMap[row-1][col]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row-1][col]].npcIndex].name);

	if ((creature[creatureMap[row-1][col+1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row-1][col+1]].npcIndex].name);

	if ((creature[creatureMap[row][col-1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row][col-1]].npcIndex].name);

	if ((creature[creatureMap[row][col+1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row][col+1]].npcIndex].name);

	if ((creature[creatureMap[row+1][col-1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row+1][col-1]].npcIndex].name);

	if ((creature[creatureMap[row+1][col]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row+1][col]].npcIndex].name);

	if ((creature[creatureMap[row+1][col+1]].npcIndex!=0))
		strcpy(trader,npc[creature[creatureMap[row+1][col+1]].npcIndex].name);

	if (strcmp(trader,"")==0)
		return (false);

	for (j=1;j<kMaxShops;j++)
		if (shopRect[j].right!=0)
		{
			if (strcmp(trader,traderName[j])==0)
				return (true);
		}
		else
			break;

	return (false);

}

// ----------------------------------------------------------------- //
int	sh_GetTrader(int i)	// server
// ----------------------------------------------------------------- //
{
	int						row;
	int						col;
	int 					theCreature=0;

	row=creature[player[i].creatureIndex].row;
	col=creature[player[i].creatureIndex].col;

	if (creatureMap[row-1][col-1]!=0)
		if ((creature[creatureMap[row-1][col-1]].npcIndex!=0))
			theCreature=creatureMap[row-1][col-1];

	if (creatureMap[row-1][col]!=0)
		if ((creature[creatureMap[row-1][col]].npcIndex!=0))
			theCreature=creatureMap[row-1][col];

	if (creatureMap[row-1][col+1]!=0)
		if ((creature[creatureMap[row-1][col+1]].npcIndex!=0))
			theCreature=creatureMap[row-1][col+1];

	if (creatureMap[row][col-1]!=0)
		if ((creature[creatureMap[row][col-1]].npcIndex!=0))
			theCreature=creatureMap[row][col-1];

	if (creatureMap[row][col+1]!=0)
		if ((creature[creatureMap[row][col+1]].npcIndex!=0))
			theCreature=creatureMap[row][col+1];

	if (creatureMap[row+1][col-1]!=0)
		if ((creature[creatureMap[row+1][col-1]].npcIndex!=0))
			theCreature=creatureMap[row+1][col-1];

	if (creatureMap[row+1][col]!=0)
		if ((creature[creatureMap[row+1][col]].npcIndex!=0))
			theCreature=creatureMap[row+1][col];

	if (creatureMap[row+1][col+1]!=0)
		if ((creature[creatureMap[row+1][col+1]].npcIndex!=0))
			theCreature=creatureMap[row+1][col+1];

	return (theCreature);
}

// ----------------------------------------------------------------- //
int	sh_GetShopID(int i)	// server
// ----------------------------------------------------------------- //

{
	int						j;
	char					trader[kNPCNameLength];
	int						row;
	int						col;

	for (j=0;j<kMaxBanks;j++)
		if (bankRect[j].right!=0)
		{
			if ((creature[player[i].creatureIndex].row>=bankRect[j].left) && (creature[player[i].creatureIndex].row<=bankRect[j].right))
				if ((creature[player[i].creatureIndex].col>=bankRect[j].top) && (creature[player[i].creatureIndex].col<=bankRect[j].bottom))
					return (kBank);
		}
		else
			break;

	for (j=1;j<kMaxShops;j++)
		if (shopRect[j].right!=0)
		{
			if ((creature[player[i].creatureIndex].row>=shopRect[j].left) && (creature[player[i].creatureIndex].row<=shopRect[j].right))
				if ((creature[player[i].creatureIndex].col>=shopRect[j].top) && (creature[player[i].creatureIndex].col<=shopRect[j].bottom))
					return (j);
		}
		else
			break;

	row=creature[player[i].creatureIndex].row;

	col=creature[player[i].creatureIndex].col;

	strcpy(trader,"");

	if (creatureMap[row-1][col-1]!=0)
		if ((creature[creatureMap[row-1][col-1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row-1][col-1]].npcIndex].name);

	if (creatureMap[row-1][col]!=0)
		if ((creature[creatureMap[row-1][col]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row-1][col]].npcIndex].name);

	if (creatureMap[row-1][col+1]!=0)
		if ((creature[creatureMap[row-1][col+1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row-1][col+1]].npcIndex].name);

	if (creatureMap[row][col-1]!=0)
		if ((creature[creatureMap[row][col-1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row][col-1]].npcIndex].name);

	if (creatureMap[row][col+1]!=0)
		if ((creature[creatureMap[row][col+1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row][col+1]].npcIndex].name);

	if (creatureMap[row+1][col-1]!=0)
		if ((creature[creatureMap[row+1][col-1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row+1][col-1]].npcIndex].name);

	if (creatureMap[row+1][col]!=0)
		if ((creature[creatureMap[row+1][col]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row+1][col]].npcIndex].name);

	if (creatureMap[row+1][col+1]!=0)
		if ((creature[creatureMap[row+1][col+1]].npcIndex!=0))
			strcpy(trader,npc[creature[creatureMap[row+1][col+1]].npcIndex].name);

	if (strcmp(trader,"")==0)
		return (0);

	for (j=1;j<kMaxShops;j++)
		if (shopRect[j].right!=0)
		{
			if (strcmp(trader,traderName[j])==0)
				return (j);
		}
		else
			break;

	return (0);

	//if ((map[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col]>=747)	&& (map[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col]<=750)) // bank
	//	return (kBank);

	//for (j=2;j<kMaxShops;j++)
	//	if (shopID1[j]!=0)
	//		if (shopID1[j]==map[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col])
	//			return (j);

	//for (j=2;j<kMaxShops;j++)
	//	if (shopID2[j]!=0)
	//		if (shopID2[j]==map[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col])
	//			return (j);

	//return (0);

}

#endif


