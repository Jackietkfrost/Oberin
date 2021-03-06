#ifndef _XCOMMON
#define _XCOMMON

#include "Keys.h"
#include "Remap.h"
#include "Games.h"
#include "Toolbox.h"
#include "Solfunc.h"
#include "Common.h"
#include "AdToolbox.h"

extern	TBoolean									xc_TimerOn;
extern	short											xc_HiliteScore;
extern	TBoolean									cm_Registered;

void xc_DrawStats 								(short destination);
void xc_ClearHighScores 					(void);
void xc_ClearPreferences 					(void);
void xc_SavePreferencesAndScores 	(void);
void xc_ReadPreferencesAndScores 	(void);
void xc_ShowHighScores						(void);
void xc_InputHighScore						(void);
void xc_DrawToolbar 							(short destination);
void xc_ResetTimer 								(void);
void xc_AdvanceTimer 							(void);
void xc_Swap											(short i, short j);
long xc_TotalSeconds							(short hours, short minutes, short seconds);
void xc_OrderDialog								(void);

#endif