#include "Globals.h"

#include "Toolbox.h"
#include "Common.h"
#include "Players.h"
#include "Network.h"
#include "FileIO.h"
#include "Graphics.h"
#include "Inventory.h"
#include "Creatures.h"
#include "Resource.h"
#include "Items.h"

//-----------------------------------------------
#ifdef _SERVER

UInt8								resources[kMapWidth][kMapHeight];
int									resourceXPtr;
int									resourceYPtr;
unsigned long				resourceNextRefresh;
unsigned long				reagentNextRefresh;
TBoolean					fullMoon;

#endif

unsigned long			currentDate;

UInt8					cycleDays[kMooncycles];
UInt16					currentYear;
UInt8					currentCycle;
UInt8					currentCalDay;
UInt8					currentHour;
UInt8					currentMinute;

#ifndef _SERVERONLY

unsigned long			lastCalendarCheck;

#endif

#ifdef _SERVER

//-----------------------------------------------

extern UInt16				                map[kMapWidth][kMapHeight];
extern int									        resourceTime; // beta
extern int                          ginsengSpawn;
extern int                          lotusFlowerSpawn;
extern int                          mandrakeRootSpawn;
extern int                          garlicSpawn;
extern int                          volcanicAshSpawn;
extern int                          mushroomSpawn;//Dave
//extern int                          eggSpawn;//Dave
extern int                          currentNumberOfContainers;

extern TBoolean						canWalkOn[kMaxIcons];

extern UInt8						dayNight;


//-----------------------------------------------

// ----------------------------------------------------------------- //
void rs_InitResources(void) // server
// ----------------------------------------------------------------- //

{
	int							x;
	int							y;
	//int             r;
	//int             c;

	for (x=0;x<kMapWidth;x++)
		for (y=0;y<kMapHeight;y++)
			resources[x][y]=tb_Rnd(0,100);

	resourceXPtr=0;

	resourceYPtr=0;

	resourceNextRefresh=TickCount()+1;

	reagentNextRefresh=resourceNextRefresh;


}

// ----------------------------------------------------------------- //
void rs_FailedHarvest(int i) // server
// ----------------------------------------------------------------- //

{
	TBoolean            SendMessage=true;

	if ((map[player[i].gatherRow][player[i].gatherCol]>=359) && (map[player[i].gatherRow][player[i].gatherCol]<=378))	// -- harvest trees --
	{
		if ((player[i].gatherRow==0) || (player[i].gatherCol==0))
			SendMessage=false;

		player[i].gatherType=	0;

		player[i].gathering	=	0;

		player[i].gatherRow	=	0;

		player[i].gatherCol	=	0;

		if (SendMessage)
			nw_SendDisplayMessage(i,kNoLumberHarvest);
	}

	if (((map[player[i].gatherRow][player[i].gatherCol]>=536) && (map[player[i].gatherRow][player[i].gatherCol]<=544)) || ((map[player[i].gatherRow][player[i].gatherCol]>=386) && (map[player[i].gatherRow][player[i].gatherCol]<=394)))	// -- mining ore --
	{
		if ((player[i].gatherRow==0) || (player[i].gatherCol==0))
			SendMessage=false;

		player[i].gatherType=	0;

		player[i].gathering	=	0;

		player[i].gatherRow	=	0;

		player[i].gatherCol	=	0;

		if (SendMessage)
			nw_SendDisplayMessage(i,kNoOreHarvest);
	}

	if (player[i].gatherType==kFishing)	// -- fishing --
	{
		player[i].gatherType=	0;
		player[i].gathering	=	0;
		player[i].gatherRow	=	0;
		player[i].gatherCol	=	0;
		nw_SendDisplayMessage(i,kNoFishCaught);
	}


}

// ----------------------------------------------------------------- //
void rs_RefreshResources(void) // server
// ----------------------------------------------------------------- //

{
	int								i;
	div_t							d;
	int								min;
	unsigned long			currentTime;
	int               x;
	int               y;
	int								rare;
	//int								k;
	int               breakOut;
	unsigned long			breakOutTime;

	currentTime=TickCount();
	breakOutTime=currentTime+5;

	if (currentTime<resourceNextRefresh)
		return;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("rs_RefreshResources Entry");

#endif
	//fi_WriteToErrorLog("rs_RefreshResources Entry");

	//---------------------------------------------------------------------------------------------------

	if (currentTime>reagentNextRefresh)
	{
		reagentNextRefresh=currentTime+(1000);

		if (currentNumberOfContainers<(kMaxContainers-1))
		{

			// gs questionable 2/26/03 //  nw_GetMessages();

			rare=it_GetRare(0);

			if (rare>0)
			{
				x=tb_Rnd(5,kMapWidth-5);
				y=tb_Rnd(5,kMapHeight-5);

				if (map[x][y]>255 && map[x][y]<263)
					if (in_CanGroup(rare))
						in_DropItem(0,1,rare, 1, x, y, 0, false, "");
					else
						in_DropItem(0,1,rare, 127, x, y, 0, false, "");
			}

			if (currentCycle==kLuckyMoon && tb_Rnd(1,10)<5 && fullMoon)  //Lucky Moon... 1.5 more rares
			{
				rare=it_GetRare(0);

				if (rare>0)
				{
					x=tb_Rnd(5,kMapWidth-5);
					y=tb_Rnd(5,kMapHeight-5);

					if (map[x][y]>255 && map[x][y]<263)
						if (in_CanGroup(rare))
							in_DropItem(0,1,rare, 1, x, y, 0, false, "");
						else
							in_DropItem(0,1,rare, 127, x, y, 0, false, "");
				}
			}
			//Dave
			breakOut=0;

			for (i=0;i<mushroomSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - mushroomSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500) break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						mushroomSpawn=mushroomSpawn-in_DropItem(0,10,kMushroom, 1, x, y, 0, false, "");
			}
			
			//Dave
			/*
			breakOut=0;

			for (i=0;i<eggSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - eggSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500) break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						eggSpawn=eggSpawn-in_DropItem(0,10,(tb_Rnd(kRedEgg,kBlueEgg)), 1, x, y, 0, false, "");
			}
			//Dave^
			*/
			breakOut=0;

			for (i=0;i<ginsengSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - ginsengSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500) break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						ginsengSpawn=ginsengSpawn-in_DropItem(0,10,kGinseng, 1, x, y, 0, false, "");
			}

			breakOut=0;

			for (i=0;i<lotusFlowerSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - lotusFlowerSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500)  break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						lotusFlowerSpawn=lotusFlowerSpawn-in_DropItem(0,7,kLotusFlower, 1, x, y, 0, false, "");
			}

			breakOut=0;

			for (i=0;i<mandrakeRootSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - mandrakeRootSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500)  break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						mandrakeRootSpawn=mandrakeRootSpawn-in_DropItem(0,9,kMandrakeRoot, 1, x, y, 0, false, "");
			}

			breakOut=0;

			for (i=0;i<garlicSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - garlicSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500)  break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						garlicSpawn=garlicSpawn-in_DropItem(0,6,kGarlic, 1, x, y, 0, false, "");
			}

			breakOut=0;

			for (i=0;i<volcanicAshSpawn;i++)
			{
				if (TickCount()>breakOutTime)
				{
					fi_WriteToErrorLog("rs_RefreshResources Exceeded Breakout Time - volcanicAshSpawn\n");
					resourceNextRefresh=TickCount()+10;
					return;
				}

				breakOut++;

				if (breakOut>500)  break;

				x=tb_Rnd(5,kMapWidth-5);

				y=tb_Rnd(5,kMapHeight-5);

				if ((map[x][y]>=256) && (map[x][y]<=263))
					if (containerMap[x][y]==0)
						volcanicAshSpawn=volcanicAshSpawn-in_DropItem(0,11,kVolcanicAsh, 1, x, y, 0, false, "");
			}
			
		}
	}

	//---------------------------------------------------------------------------------------------------
	//temp=TickCount();

	for (i=0;i<2400;i++)
	{

		d		=div(i,10);
		min	=d.rem*10;

		//if ((resourceXPtr<0) || (resourceXPtr>=kMapWidth) || (resourceYPtr<0) || (resourceYPtr>=kMapHeight))			// beta debug
		//	fi_WriteToErrorLog("Out of Range - rs_RefreshResources");

		if ((map[resourceXPtr][resourceYPtr]>=359) && (map[resourceXPtr][resourceYPtr]<=373))	// -- replenish trees --
		{
			if (resources[resourceXPtr][resourceYPtr]>5)
				resources[resourceXPtr][resourceYPtr]=resources[resourceXPtr][resourceYPtr]-5;
			else
				resources[resourceXPtr][resourceYPtr]=0;

			if (resources[resourceXPtr][resourceYPtr]<min)
				resources[resourceXPtr][resourceYPtr]=min;
		}

		if ((map[resourceXPtr][resourceYPtr]>=536) && (map[resourceXPtr][resourceYPtr]<=544))	// -- replenish mining --
		{
			if (resources[resourceXPtr][resourceYPtr]>5)
				resources[resourceXPtr][resourceYPtr]=resources[resourceXPtr][resourceYPtr]-5;
			else
				resources[resourceXPtr][resourceYPtr]=0;

			if (resources[resourceXPtr][resourceYPtr]<min)
				resources[resourceXPtr][resourceYPtr]=min;
		}

		//---------------------------------
		resourceXPtr++;

		if (resourceXPtr>=kMapWidth)
		{
			resourceXPtr=0;
			resourceYPtr++;

			if (resourceYPtr>=kMapHeight)
				resourceYPtr=0;
		}

		//---------------------------------

	}

	resourceNextRefresh=TickCount()+10;	// used to be 1 ... changed 10/10

	//fi_WriteToErrorLog("rs_RefreshResources Exit");
#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("rs_RefreshResources Exit");
#endif

	//resourceTime=resourceTime+(TickCount()-temp);

}

#endif

// ----------------------------------------------------------------- //
void rs_InitCalendar(void)
// ----------------------------------------------------------------- //
{


	cycleDays[kWakingMoon]=239;
	cycleDays[kGrowingMoon]=250;
	cycleDays[kBleedingMoon]=230;
	cycleDays[kRedMoon]=242;
	cycleDays[kHarvestMoon]=233;
	cycleDays[kDancingMoon]=253;
	cycleDays[kLuckyMoon]=232;
	cycleDays[kBlueMoon]=249;
	cycleDays[kFightingMoon]=230;
	cycleDays[kSleepingMoon]=242;

	currentYear=currentCycle=currentCalDay=currentHour=currentMinute=0;

	currentDate=0;

#ifdef _SERVER
	fullMoon=false;
	rs_GetCurrentCalendar();
#endif
}


// ----------------------------------------------------------------- //
void rs_GetCurrentCalendar(void)
// ----------------------------------------------------------------- //

{
	int					calc;
	int					cpt, totaldays=0, cycle, cpt2, cpt3;
	unsigned long		theTime, currentTime, currentDate2;


	UInt8				oldMinute=currentMinute;

	for (cpt=0; cpt<kMooncycles; cpt++)
		totaldays+=cycleDays[cpt];


#ifdef _SERVER
	GetDateTime(&currentDate); 				//Second since rl year 2001

#else
	if (currentDate==0)
		return;

	currentTime=TickCount();

	theTime=currentTime-lastCalendarCheck;

	if (theTime<60)
		return;

	theTime/=60;

	currentDate+=theTime;

	lastCalendarCheck=currentTime;

#endif

	currentDate2=currentDate/5;							//Oberin minutes since Oberin year 0

	calc=currentDate2/1440;					//Oberin days

	currentYear=calc/totaldays;


	calc%=totaldays;					//Remaining days

	cpt=cycle=0;

	while (true)
	{
		if (cpt+cycleDays[cycle]<=calc)
		{
			cpt+=cycleDays[cycle];
			cycle++;
		}
		else
			break;
	}

	if (currentCycle!=cycle)
		currentCycle=cycle;

	calc-=cpt;

	currentCalDay=calc+1;						//Days of the current cycle

	currentDate2=currentDate/5;

	currentDate2-=(totaldays*currentYear*1440);

	currentDate2-=((cpt+calc)*1440);

	currentHour=currentDate2/60;

	currentMinute=currentDate2%60;

#ifdef _SERVER

	cpt3=0;

	cpt2=cycleDays[currentCycle]/8;

	while (cpt2<currentCalDay)
	{
		cpt3++;
		cpt2+=cycleDays[currentCycle]/8;
	}

	if (cpt3==4)
		fullMoon=true;
	else
		fullMoon=false;

	if (currentHour<4)
		dayNight=2;  //Night
	else if (currentHour<7)
		dayNight=3;  //Dawn
	else if (currentHour<19)
		dayNight=0;  //Day
	else if (currentHour<22)
		dayNight=1;  //Dusk
	else
		dayNight=2; //Night

#endif

#ifndef _SERVERONLY

	if (currentMinute!=oldMinute)
	{
		nw_SendCalendarRequest(); //Resynch
		gr_RenderCalendarWindow();
	}

#endif




}

