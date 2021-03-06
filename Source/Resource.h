#ifndef _RESOURCES
#define _RESOURCES

#include "Keys.h"
#include "Remap.h"
#include "Network.h"
#include "Inventory.h"
#include "Globals.h"

#define				kWakingMoon							0
#define				kGrowingMoon						1
#define				kBleedingMoon						2
#define				kRedMoon							3
#define				kHarvestMoon						4
#define				kDancingMoon						5
#define				kLuckyMoon							6
#define				kBlueMoon							7
#define				kFightingMoon						8
#define				kSleepingMoon						9
	
#define				kMooncycles							10	

//- functions ----------------------------------------------

void 				rs_RefreshResources							(void);
void 				rs_InitResources							(void);
void 				rs_HarvestResource							(int i);
void 				rs_FailedHarvest							(int i); // server

//----------------------------------------------------------

//- Cal}ar related functions -----------------------------

void				rs_InitCalendar								(void); 
void				rs_GetCurrentCalendar						(void);

//----------------------------------------------------------

#endif