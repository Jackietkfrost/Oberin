#include "XCommon.h"

short									xc_Hours;
short									xc_Minutes;
short									xc_Seconds;
unsigned long					xc_Timer;
TBoolean							xc_TimerOn;
short									xc_HiliteScore;

// ----------------------------------------------------------------- //
void xc_ResetTimer (void)
// ----------------------------------------------------------------- //

{

	xc_Hours		= 0;
	xc_Minutes	= 0;
	xc_Seconds	= 0;
	xc_Timer		= tb_CurrentTick()+60;

}

// ----------------------------------------------------------------- //
void xc_AdvanceTimer (void)
// ----------------------------------------------------------------- //

{
	/*
		if ((gm_GameOver) || (xc_TimerOn==false))
			return;

		if (tb_CurrentTick()>=xc_Timer)
			begin
				xc_Seconds++;
				if (xc_Seconds>59)
					begin
						xc_Seconds=0;
						xc_Minutes++;
						if (xc_Minutes>59)
							begin
								xc_Minutes=0;
								xc_Hours++;
								if (xc_Hours>99)
									xc_ResetTimer();
							end
					end
				xc_Timer=tb_CurrentTick()+60;
				xc_DrawStats(kOnAndOffScreen);
			end
	*/
}

// ----------------------------------------------------------------- //
void xc_DrawStats (short destination)
// ----------------------------------------------------------------- //

{
	// Modified (11/4/98)

	unsigned int						barRed		= kBarRed;
	unsigned int						barGreen	= kBarGreen;
	unsigned int						barBlue		= kBarBlue;
	unsigned int						barLRed		= kBarRed		*1.3;
	unsigned int						barLGreen	= kBarGreen	*1.3;
	unsigned int						barLBlue	= kBarBlue	*1.3;
	unsigned int						barDRed		= kBarRed		*0.7;
	unsigned int						barDGreen	= kBarGreen	*0.7;
	unsigned int						barDBlue	= kBarBlue	*0.7;
	TRect										r;
	TRect										c;
	TRect										t;
	char										s[tb_StringLength];
	char										n[tb_StringLength];


	// get toolbar rect and reset clip
	tb_GetClientRect(tbWindow, &r);
	tb_SetClip(osBMP, &r);
	r.bottom=kToolBarHeight;

	// clear old area
	c=r;
	c.left=maxButtonH;
	c.right--;
	c.bottom--;
	c.top++;
	tb_SetForeColor(osBMP, barRed, barGreen, barBlue);
	tb_PaintRect(osBMP,&c);

	// set font
	tb_SetForeColor(osBMP, 0, 0, 0);
	tb_SetFont(osBMP, kStatsFont);
	tb_SetFontSize(osBMP, 18);
	tb_SetFontStyle(osBMP, tb_Bold);

	// draw score
	tb_IntegerToString(gm_Score,n);
	tb_ConcatString(s, "Score: ", n);
	tb_ConcatString(s, s, "/");
	tb_IntegerToString(gm_MaxScore,n);
	tb_ConcatString(s, s, n);
	tb_DrawString(osBMP, (r.right-tb_StringWidth(osBMP, s)-10), (r.bottom-8), s);

	// draw deck count
	tb_IntegerToString(sf_StockCount(),n);
	tb_ConcatString(s, "Deck: ", n);
	tb_DrawString(osBMP, (r.right-tb_StringWidth(osBMP, s)-175), (r.bottom-8), s);

	// copy to screen

	if ((destination==kOnScreen) || (destination==kOnAndOffScreen))
		tb_CopyBitmap(osBMP, tbWindow, &c, &c, false);

	// reset clip area
	tb_GetClientRect(tbWindow, &r);

	r.top=kToolBarHeight+1;

	tb_SetClip(osBMP, &r);

}

// ----------------------------------------------------------------- //
void xc_DrawToolbar (short destination)
// ----------------------------------------------------------------- //

{
	unsigned int						barRed		= kBarRed;
	unsigned int						barGreen	= kBarGreen;
	unsigned int						barBlue		= kBarBlue;
	unsigned int						barLRed		= kBarRed		*1.3;
	unsigned int						barLGreen	= kBarGreen	*1.3;
	unsigned int						barLBlue	= kBarBlue	*1.3;
	unsigned int						barDRed		= kBarRed		*0.7;
	unsigned int						barDGreen	= kBarGreen	*0.7;
	unsigned int						barDBlue	= kBarBlue	*0.7;
	TRect										r;
	TRect										topBar;
	TRect										bottomBar;
	char										s[tb_StringLength];
	TRect										d;


	tb_GetClientRect(tbWindow, &r);

	tb_SetClip(osBMP, &r);

	r.bottom=kToolBarHeight;
	//r.right=ad_BannerRect.left;

	tb_SetForeColor(osBMP, barRed, barGreen, barBlue);
	tb_PaintRect(osBMP,&r);

	//tb_GetClientRect(tbWindow, &r);

	tb_SetForeColor(osBMP, barLRed, barLGreen, barLBlue);
	tb_Line(osBMP, r.left, r.top, r.left, kToolBarHeight, 1);
	tb_Line(osBMP, r.left, r.top, r.right, r.top, 1);

	tb_SetForeColor(osBMP, barDRed, barDGreen, barDBlue);
	tb_Line(osBMP, r.left, kToolBarHeight, r.right, kToolBarHeight, 1);
	tb_Line(osBMP, r.right, r.top, r.right, kToolBarHeight, 1);

//	GetGWorld(&tbsavedPort, &tbsavedDevice);
//	tb_SetPort(osBMP);
//	DrawPicture(adPict[currentFrame], &ad_BannerRect);
//	SetGWorld(tbsavedPort, tbsavedDevice);

	//if (!adOff)
	//	begin
	//		if (currentFrame>=frameCount)
	//			currentFrame=0;
	//		tb_CopyBitmap(adBMP[currentFrame], osBMP, &adRect, &ad_BannerRect, false);
	//	end

	topBar=r;

	//-----------------------------------------

	buttonID[kNewGameButton]=kNewGameButtonID;
	tb_DrawPictureToBitmap(osBMP, buttonID[kNewGameButton]+buttonState[kNewGameButton], 1, 1, &buttonRect[kNewGameButton]);

//	buttonID[kZoomInButton]=kZoomInButtonID;
//	tb_DrawPictureToBitmap(osBMP, buttonID[kZoomInButton]+buttonState[kZoomInButton], 33, 1, &buttonRect[kZoomInButton]);

//	buttonID[kZoomOutButton]=kZoomOutButtonID;
//	tb_DrawPictureToBitmap(osBMP, buttonID[kZoomOutButton]+buttonState[kZoomOutButton], 65, 1, &buttonRect[kZoomOutButton]);

	buttonID[kUndoButton]=kUndoButtonID;
	tb_DrawPictureToBitmap(osBMP, buttonID[kUndoButton]+buttonState[kUndoButton], 33, 1, &buttonRect[kUndoButton]);

	buttonID[kRedoButton]=kRedoButtonID;
	tb_DrawPictureToBitmap(osBMP, buttonID[kRedoButton]+buttonState[kRedoButton], 65, 1, &buttonRect[kRedoButton]);

	buttonID[kScoresButton]=kScoresButtonID;
	tb_DrawPictureToBitmap(osBMP, buttonID[kScoresButton]+buttonState[kScoresButton], 97, 1, &buttonRect[kScoresButton]);

//	buttonID[kPrefsButton]=kPrefsButtonID;
//	tb_DrawPictureToBitmap(osBMP, buttonID[kPrefsButton]+buttonState[kPrefsButton], 193, 1, &buttonRect[kPrefsButton]);

	buttonID[kGiveUpButton]=kGiveUpButtonID;
	tb_DrawPictureToBitmap(osBMP, buttonID[kGiveUpButton]+buttonState[kGiveUpButton], 129, 1, &buttonRect[kGiveUpButton]);

//	buttonID[kHelpButton]=kHelpButtonID;
//	tb_DrawPictureToBitmap(osBMP, buttonID[kHelpButton]+buttonState[kHelpButton], 161, 1, &buttonRect[kHelpButton]);

	maxButtonH=257+32; // used to create clear area for stats

	//-----------------------------------------

	/*
		tb_GetClientRect(tbWindow, &r);

		r.top=r.bottom-25;

		bottomBar=r;

		tb_SetForeColor(osBMP, barRed, barGreen, barBlue);
		tb_PaintRect(osBMP,&r);

		tb_SetForeColor(osBMP, barLRed, barLGreen, barLBlue);
		tb_Line(osBMP, r.left, r.top, r.left, r.bottom, 1);
		tb_Line(osBMP, r.left, r.top, r.right, r.top, 1);

		tb_SetForeColor(osBMP, barDRed, barDGreen, barDBlue);
		tb_Line(osBMP, r.left, r.bottom, r.right, r.bottom, 1);
		tb_Line(osBMP, r.right, r.top, r.right, r.bottom, 1);

		// set font
		tb_SetForeColor(osBMP, 22000, 22000, 22000);
		tb_SetFont(osBMP, kStatsFont);
		tb_SetFontSize(osBMP, 10);
		tb_SetFontStyle(osBMP, tb_Bold);

		// draw text
		strcpy(s, "Check online for updates and new game releases!");
		tb_DrawString(osBMP, ((r.right/2)-tb_StringWidth(osBMP, s)/2), (r.bottom-15), s);

		strcpy(s, "http://www.CrescentVision.com");
		tb_DrawString(osBMP, ((r.right/2)-tb_StringWidth(osBMP, s)/2), (r.bottom-2), s);

		tb_DrawPictureToBitmap(osBMP, 130, 1, r.bottom-23, &d);
		tb_DrawPictureToBitmap(osBMP, 128, r.right-88, r.bottom-23, &d);
	*/
	xc_DrawStats(kOffScreen);

	tb_SetPort(tbWindow);

	if ((destination==kOnScreen) || (destination==kOnAndOffScreen))
		tb_CopyBitmap(osBMP, tbWindow, &topBar, &topBar, false);

//	if ((destination==kOnScreen) || (destination==kOnAndOffScreen))
//		tb_CopyBitmap(osBMP, tbWindow, &bottomBar, &bottomBar, false);

	tb_GetClientRect(tbWindow, &r);

	r.top=kToolBarHeight+1;

	//r.bottom=r.bottom-25;

	tb_SetClip(osBMP, &r);

}

// ----------------------------------------------------------------- //
void xc_ClearHighScores (void)
// ----------------------------------------------------------------- //

{
	short				i;
	short				j;


	for (i=0;i<kGameCount;i++)
		for (j=0;j<kMaxScoreList;j++)
			begin
			tb_CopyString(gm_HighScores[i][j].name,"Anonymous");

	gm_HighScores[i][j].score=0;

	gm_HighScores[i][j].hours=0;

	gm_HighScores[i][j].minutes=0;

	gm_HighScores[i][j].seconds=0;

	end

}

// ----------------------------------------------------------------- //
void xc_ClearPreferences (void)
// ----------------------------------------------------------------- //

{

	tb_CopyString(cm_DefaultName,"Anonymous");
	sf_FlashValidPlays			= false;
	sf_DoAutoPlay						= true;
	sf_AutoEnd					 		= true;
	tb_SoundOn							= true;
	sf_RestartAfterGameOver	= true;
	sf_ScoresAfterGameOver	= true;
	sf_ConfirmNewGame				= false;
	sf_ConfirmGiveUp				= false;
	sf_HilightColor					=	0;
	sf_BackgroundPattern		=	505;
	sf_CardBackID						=	4;
	gm_CurrentGame					=	kKlondikeCommon;
	cm_Registered						= false;

}

// ----------------------------------------------------------------- //
void xc_SavePreferencesAndScores (void)
// ----------------------------------------------------------------- //

{
	int					test=1234;


	if (tb_OpenPrefsForOutput()==0)
		begin
		tb_WriteToFile(&gm_CurrentGame, 					sizeof(gm_CurrentGame));

	tb_WriteToFile(&sf_HilightColor, 					sizeof(sf_HilightColor));

	tb_WriteToFile(&sf_BackgroundPattern, 		sizeof(sf_BackgroundPattern));

	tb_WriteToFile(&sf_CardBackID, 						sizeof(sf_CardBackID));

	tb_WriteToFile(&sf_RestartAfterGameOver, 	sizeof(sf_RestartAfterGameOver));

	tb_WriteToFile(&sf_ScoresAfterGameOver, 	sizeof(sf_ScoresAfterGameOver));

	tb_WriteToFile(&sf_FlashValidPlays, 			sizeof(sf_FlashValidPlays));

	tb_WriteToFile(&sf_DoAutoPlay,						sizeof(sf_DoAutoPlay));

	tb_WriteToFile(&sf_AutoEnd,								sizeof(sf_AutoEnd));

	tb_WriteToFile(&tb_SoundOn,								sizeof(tb_SoundOn));

	tb_WriteToFile(&sf_ConfirmNewGame,				sizeof(sf_ConfirmNewGame));

	tb_WriteToFile(&sf_ConfirmGiveUp,					sizeof(sf_ConfirmGiveUp));

	tb_WriteToFile(&gm_HighScores[0],					sizeof(gm_HighScores));

	tb_WriteToFile(&cm_DefaultName[0],				sizeof(cm_DefaultName));

	tb_WriteToFile(&cm_Registered,						sizeof(cm_Registered));

	tb_WriteToFile(&test,											sizeof(test));

	tb_CloseFile();

	end

}

// ----------------------------------------------------------------- //
void xc_ReadPreferencesAndScores (void)
// ----------------------------------------------------------------- //

{
	int					test;


	xc_ClearHighScores();
	xc_ClearPreferences();

	if (tb_OpenPrefsForInput()==0)
		begin
		if (tb_ReadFromFile(&gm_CurrentGame, 					sizeof(gm_CurrentGame))!=0)						goto fileError;

	if (tb_ReadFromFile(&sf_HilightColor, 				sizeof(sf_HilightColor))!=0)					goto fileError;

	if (tb_ReadFromFile(&sf_BackgroundPattern, 		sizeof(sf_BackgroundPattern))!=0)			goto fileError;

	if (tb_ReadFromFile(&sf_CardBackID,						sizeof(sf_CardBackID))!=0)						goto fileError;

	if (tb_ReadFromFile(&sf_RestartAfterGameOver, sizeof(sf_RestartAfterGameOver))!=0)	goto fileError;

	if (tb_ReadFromFile(&sf_ScoresAfterGameOver, 	sizeof(sf_ScoresAfterGameOver))!=0)		goto fileError;

	if (tb_ReadFromFile(&sf_FlashValidPlays, 			sizeof(sf_FlashValidPlays))!=0)				goto fileError;

	if (tb_ReadFromFile(&sf_DoAutoPlay,						sizeof(sf_DoAutoPlay))!=0)						goto fileError;

	if (tb_ReadFromFile(&sf_AutoEnd,							sizeof(sf_AutoEnd))!=0)								goto fileError;

	if (tb_ReadFromFile(&tb_SoundOn,							sizeof(tb_SoundOn))!=0)								goto fileError;

	if (tb_ReadFromFile(&sf_ConfirmNewGame,				sizeof(sf_ConfirmNewGame))!=0)				goto fileError;

	if (tb_ReadFromFile(&sf_ConfirmGiveUp,				sizeof(sf_ConfirmGiveUp))!=0)					goto fileError;

	if (tb_ReadFromFile(&gm_HighScores[0],				sizeof(gm_HighScores))!=0)						goto fileError;

	if (tb_ReadFromFile(&cm_DefaultName[0],				sizeof(cm_DefaultName))!=0)						goto fileError;

	if (tb_ReadFromFile(&cm_Registered,						sizeof(cm_Registered))!=0)						goto fileError;

	if (tb_ReadFromFile(&test,										sizeof(test))!=0)											goto fileError;

	tb_CloseFile();

	if (test!=1234)
		begin
		xc_ClearHighScores();

	xc_ClearPreferences();

	end
	xc_SavePreferencesAndScores();

	return;

	end

fileError:
	tb_CloseFile();

	xc_ClearHighScores();

	xc_ClearPreferences();

	xc_SavePreferencesAndScores();

}

// ----------------------------------------------------------------- //
void xc_ShowHighScores(void)
// ----------------------------------------------------------------- //

{
	DialogPtr				Dialog;
	short						itemHit;


	itemHit=0;
	//GetPort(&(GrafPtr)tbsavedPort);
	tb_SetCursor(0);
	Dialog = GetNewDialog(129, nil, (WindowPtr)-1L);

	if (Dialog!=NULL)
		begin
		GetGWorld(&tbsavedPort, &tbsavedDevice);

	SetPort(Dialog);

	ShowWindow(Dialog);

	while (itemHit==0)
		ModalDialog((ModalFilterUPP)myScoresDialogFilter, &itemHit);

	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(Dialog);

	end
	//SetPort((GrafPtr)tbsavedPort);
	xc_HiliteScore=kNull;

	sf_DrawEverything (kNull, kNull);

}

// ----------------------------------------------------------------- //
void xc_Swap(short i, short j)
// ----------------------------------------------------------------- //

{
	gm_HighScoresStruct					temp;


	temp=gm_HighScores[gm_CurrentGame-1][i];
	gm_HighScores[gm_CurrentGame-1][i]=gm_HighScores[gm_CurrentGame-1][j];
	gm_HighScores[gm_CurrentGame-1][j]=temp;

}

// ----------------------------------------------------------------- //
long xc_TotalSeconds(short hours, short minutes, short seconds)
// ----------------------------------------------------------------- //

{

	return ((hours*3600)+(minutes*60)+seconds);

}

// ----------------------------------------------------------------- //
void xc_InputHighScore(void)
// ----------------------------------------------------------------- //

{
	// Modified (10/04/98)
	// Modified (10/27/98)

	DialogPtr				NameDialog;
	short						itemHit=0;
	short						dummyint;
	Handle					dummyh;
	Rect						TextRect;
	Str255					playerName;
	short						i;
	long						t1;
	long						t2;


	if (gm_Score<gm_HighScores[gm_CurrentGame-1][kMaxScoreList-1].score)		// this line was modified (10/04/98)
		return;

	if (gm_Score==gm_HighScores[gm_CurrentGame-1][kMaxScoreList-1].score)		// this line was added (10/04/98)
		begin																																	// this line was added (10/04/98)
		t1=xc_TotalSeconds(gm_HighScores[gm_CurrentGame-1][kMaxScoreList-1].hours,gm_HighScores[gm_CurrentGame-1][kMaxScoreList-1].minutes,gm_HighScores[gm_CurrentGame-1][kMaxScoreList-1].seconds);		// this line was added (10/04/98)

	t2=xc_TotalSeconds(xc_Hours,xc_Minutes,xc_Seconds);									// this line was added (10/04/98)

	if (t1<=t2)	return;																									// this line was added (10/04/98)

	end																																		// this line was added (10/04/98)

	itemHit=0;

	GetPort(&(GrafPtr)tbsavedPort);

	tb_SetCursor(0);

	NameDialog = GetNewDialog(134, nil, (WindowPtr)-1L);

	if (NameDialog!=NULL)
		begin
		GetGWorld(&tbsavedPort, &tbsavedDevice);

	SetPort(NameDialog);

	tb_CtoPStr255(cm_DefaultName,playerName);

	GetDialogItem(NameDialog, 3, &dummyint, &dummyh, &TextRect);

	SetDialogItemText(dummyh, playerName);

	SelectDialogItemText(NameDialog,3,0,255);

	ShowWindow(NameDialog);

	while (!((itemHit==1) || (itemHit==2)))
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	if (itemHit==1)
		begin
		GetDialogItem(NameDialog, 3, &dummyint, &dummyh, &TextRect);

	GetDialogItemText(dummyh, playerName);

	end
	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(NameDialog);

	end
	//SetPort((GrafPtr)tbsavedPort);

	sf_DrawEverything (kNull, kNull);

	if (itemHit==1) // OK button ... put name in list
		begin
		tb_PascaltoCStr(playerName,gm_HighScores[gm_CurrentGame-1][9].name); // put name

	tb_CopyString(cm_DefaultName,gm_HighScores[gm_CurrentGame-1][9].name);

	gm_HighScores[gm_CurrentGame-1][9].score=gm_Score; // put score

	gm_HighScores[gm_CurrentGame-1][9].hours=xc_Hours;

	gm_HighScores[gm_CurrentGame-1][9].minutes=xc_Minutes;

	gm_HighScores[gm_CurrentGame-1][9].seconds=xc_Seconds;

	xc_HiliteScore=9;			// Modified (10/27/98)

	for (i=9;i>0;i--)
		if (gm_HighScores[gm_CurrentGame-1][i].score>gm_HighScores[gm_CurrentGame-1][i-1].score)
			begin
			xc_Swap(i-1,i);

	xc_HiliteScore=i-1;

	end

	for (i=9;i>0;i--)
		begin
		t1=xc_TotalSeconds(gm_HighScores[gm_CurrentGame-1][i].hours,gm_HighScores[gm_CurrentGame-1][i].minutes,gm_HighScores[gm_CurrentGame-1][i].seconds);

	t2=xc_TotalSeconds(gm_HighScores[gm_CurrentGame-1][i-1].hours,gm_HighScores[gm_CurrentGame-1][i-1].minutes,gm_HighScores[gm_CurrentGame-1][i-1].seconds);

	if ((gm_HighScores[gm_CurrentGame-1][i].score==gm_HighScores[gm_CurrentGame-1][i-1].score) && (t1<t2))
		begin
		xc_Swap(i-1,i);

	xc_HiliteScore=i-1;

	end
	end
	end

	xc_SavePreferencesAndScores();

}

// ----------------------------------------------------------------- //
void xc_OrderDialog(void)
// ----------------------------------------------------------------- //

{
	DialogPtr				OrderDialog;
	short						itemHit=0;
	short						dummyint;
	Handle					dummyh;
	Rect						TextRect;
	Str255					usercode;
	char						Cusercode[50];
	short						i;
	long						t1;
	long						t2;
	TBoolean				looping=true;
	short						kind;
	Handle					item;
	TRect						box;


	if (cm_Registered)
		return;

	itemHit=0;

	GetPort(&(GrafPtr)tbsavedPort);

	tb_SetCursor(0);

	OrderDialog = GetNewDialog(131, nil, (WindowPtr)-1L);

	if (OrderDialog!=NULL)
		begin
		GetGWorld(&tbsavedPort, &tbsavedDevice);

	SetPort(OrderDialog);

	tb_CtoPStr255("",usercode);

	GetDialogItem(OrderDialog, 10, &dummyint, &dummyh, &TextRect);

	SetDialogItemText(dummyh, usercode);

	//SelectDialogItemText(OrderDialog,10,0,255);
	ShowWindow(OrderDialog);

	while (looping)
		begin
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	if ((itemHit==2) || (itemHit==3) || (itemHit==6))
		looping=false;

	if (itemHit==1)
		begin
		GetDialogItem(OrderDialog, 10, &dummyint, &dummyh, &TextRect);

	GetDialogItemText(dummyh, usercode);

	tb_PascaltoCStr(usercode,Cusercode);

	if (strcmp(Cusercode,"3381-6444-0068")==0)
		looping=false;
	else if (strcmp(Cusercode,"338164440068")==0)
		looping=false;
	else
		begin
		tb_AlertMessage3(132,"Invalid user code.  Please try again.");

	sf_DrawEverything (kNull, kNull);

	GetDialogItem(OrderDialog, 1, &kind, &item, &box);

	HiliteControl((ControlHandle)item, 0);

	end
	end
	end
	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(OrderDialog);

	end
	//SetPort((GrafPtr)tbsavedPort);

	if (itemHit==2) // MORE INFO
		begin
		itemHit=0;

	looping=true;

	GetPort(&(GrafPtr)tbsavedPort);

	tb_SetCursor(0);

	OrderDialog = GetNewDialog(135, nil, (WindowPtr)-1L);

	if (OrderDialog!=NULL)
		begin
		GetGWorld(&tbsavedPort, &tbsavedDevice);

	SetPort(OrderDialog);

	ShowWindow(OrderDialog);

	while (looping)
		begin
		ModalDialog((ModalFilterUPP)myGenericFilter, &itemHit);

	if (itemHit==1)
		looping=false;

	end
	SetGWorld(tbsavedPort, tbsavedDevice);

	DisposeDialog(OrderDialog);

	end
	itemHit=0;

	end

	sf_DrawEverything (kNull, kNull);

	if (itemHit==1) // ENTER CODE
		begin
		cm_Registered=true;

	xc_SavePreferencesAndScores();

	tb_AlertMessage3(132,"Thank you for purchasing iSolitaire!");

	sf_DrawEverything (kNull, kNull);

	end

	if (itemHit==6) // URL BUTTON
		begin
		if (tb_LaunchURL("\phttp://www.crescentvision.com/")==noErr)
			begin
			sf_DrawEverything (kNull, kNull);

	end
		else
			begin
			tb_AlertMessage3(132,"I could not access your Internet settings file.  You will have to manually type \"www.crescentvision.com\" into your browser.");

	sf_DrawEverything (kNull, kNull);

	end
	end

}
