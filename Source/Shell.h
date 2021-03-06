#include <Menus.h>
//#include <Windows.h>
#include <MacWindows.h>
#include <TextEdit.h>
#include <Dialogs.h>
#include <OSUtils.h>
#include <ToolUtils.h>
#include <SegLoad.h>
#include <AppleEvents.h>
#include <sound.h>
#include <stdio.h>
#include <Movies.h>
#include <Gestalt.h>
#include <QDOffscreen.h>
//#include <MacTypes.h>
#include <Quickdraw.h>
#include <Devices.h>

#include "Common.h"
#include "Toolbox.h"
#include "Graphics.h"
#include "Creatures.h"
#include "Splash.h"
#include "MacTextEditor.h"

#define kOKButton					1
#define kCancelButton			2

void DoDrawNumber(long n, int h, int v, int justify);
void DoDrawString(char s[tb_StringLength], int h, int v, int justify);
void DrawScores(void);
void SetSizeAndPositionOfWindow(void);
void main(void);
void OpenWindow(void);
void Initialize(void);
void SetUpMenus(void);
void MainLoop();
void DoMouseDown (EventRecord *theEvent);
void DoMenus (long menuResult);
void DoUpdate (WindowPtr wp);
void DrawEverything (void);
void DoKey (EventRecord *theEvent,TBoolean repeatKey);
void DoActivate (EventRecord *theEvent);
pascal OSErr 	sh_MyHandleOApp 								(AEDescList *aevt, AEDescList *reply, long refCon);
pascal OSErr 	sh_MyHandleODoc 								(AEDescList *aevt, AEDescList *reply, long refCon);
pascal OSErr 	sh_MyHandlePDoc 								(AEDescList *aevt, AEDescList *reply, long refCon);
pascal OSErr 	sh_MyHandleQuit 								(AEDescList *aevt, AEDescList *reply, long refCon);
pascal TBoolean GenericFilter (DialogPtr theDialog, EventRecord *theEvent, SInt16 *itemHit);
pascal TBoolean ScoresDialogFilter (DialogPtr theDialog, EventRecord *theEvent, SInt16 *itemHit);
pascal TBoolean CardBackDialogFilter (DialogPtr theDialog, EventRecord *theEvent, SInt16 *itemHit);
pascal TBoolean PatternDialogFilter (DialogPtr theDialog, EventRecord *theEvent, SInt16 *itemHit);
pascal TBoolean HilightDialogFilter (DialogPtr theDialog, EventRecord *theEvent, SInt16 *itemHit);
void System7Dialog(void);
void tb_DrawSplashPictureToDialog(void);
void DoSuspendResume(Boolean InForeground);
void startMusic (void);
void SetWindowMenuChecks(void);
void sh_CheatCheck(void);
void sh_EndSession(void);
void InitMLTE(void);
void DoIdle();
Boolean IsAppWindow(WindowPtr window, TXNObject *object);
void MyQTModelessCallbackCallback (EventRecord *theEvent, DialogPtr theDialog, SInt16 *itemHit);
TBoolean EventFilter(EventRecord *theEvent, WindowPtr theFrontWindow);
void DoDialogEvent(EventRecord *theEvent);
void GetKeyEvents(void);
void sh_CloseResFiles(void);