//Oberin Updater.  Last modified by Michael.
#include "Updater.h"
#include "Globals.h"
#include "Toolbox.h"
#include "Keys.h"
#include "Remap.h"
#include "Players.h"
#include "Creatures.h"
#include "Common.h"
#include "FileIO.h"

#ifndef _SERVER

//#define _FUNCTIONTRACEDEBUGGING

URLNotifyUPP 		gMyNotifierUPP;
TBoolean			gDlDone, gProcessing, gAuto, gAbort, gUrlAborted, gBusy;
DialogRef 			gProgressWindow;
ListHandle			gList;
EventLoopTimerUPP	gMyIdleTimerUPP;
FSSpec				gFile;
URLReference		gURLRef;
char 				gCurrentUrl[kStringLength], gToDownload[kStringLength], gPostOutput[kStringLength], gLocalPath[100][kStringLength], gFileName[100][kStringLength], gCompressedFileName[100][kStringLength], gDownload[100][kStringLength];
FILE				*mainFile;
int					gPreviousStates[100], gStates[100], totalNumberOfFiles, currentFileNumber;
long				gFileSize[100], gTimeout;
long				gOldSize;
UInt8				gVersion;
ListDefUPP			gListDefFunctionUPP;
Str255				gLatestMessage;
ControlRef			gAbortButton, gQuitButton, gResumeButton;
long				gSize[100], gHowMany[100];
AEIdleUPP 			MyIdleUPP;
SInt32				MacVersion;//Mac version  defined
OSErr				versionErr;//Get the error code if one is there
char				debug[kStringLength], debug2[kStringLength];//Debug info



// ----------------------------------------------------------------- //
void up_Update(UInt8 version, char *url)
// ----------------------------------------------------------------- //

{
	OSStatus			status;
	int					cpt;
	Str255				where="\p:Updater temp:";
	long				dummy;
	char				fileName[kStringLength], newUrl[kStringLength];

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_Update Entry");
#endif

	tb_SetCursor(0);

	gDlDone=false;
	gProcessing=false;
	gAuto=true;
	currentFileNumber=-1;
	totalNumberOfFiles=99;
	gVersion=version;
	gAbort=false;
	gUrlAborted=false;

	for (cpt=0; cpt<100; cpt++)
	{
		gStates[cpt]=0;
		gHowMany[cpt]=0;
	}

	MyIdleUPP = NewAEIdleUPP (&MyIdleCallback);

	PLstrcpy(gLatestMessage, "\pLatest message : ");

	FSMakeFSSpec(0,0, where, &gFile);

	FSpDirCreate(&gFile, smSystemScript, &dummy);
	//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
	versionErr = Gestalt(gestaltSystemVersion, &MacVersion);
	if (versionErr != noErr)
	{
		strcpy(debug, "Version Fetch Error : ");
		tb_NumToString(versionErr, debug2);
		strcat(debug, debug2);
		fi_WriteToErrorLog(debug);
	}
	else
	{
		if(MacVersion >=0x1000){
				PLstrcpy(where, "\p:Updater temp:mainx.obu");
				strcpy(newUrl, "http://oberin.com/update/mainx.obu");
		}
		else if (MacVersion <= 0x0999){
				PLstrcpy(where, "\p:Updater temp:main9.obu");
				strcpy(newUrl, "http://oberin.com/update/main9.obu");
		}
	}
	//End of version get.

	FSMakeFSSpec(0,0, where, &gFile);

	gMyNotifierUPP = NewURLNotifyUPP((URLNotifyProcPtr)MyNotifier);
	gMyIdleTimerUPP = NewEventLoopTimerUPP((EventLoopTimerProcPtr)MyIdleTimer);

	URLNewReference(newUrl, &gURLRef);

	status = URLOpen(gURLRef, &gFile, kUrlFlags, gMyNotifierUPP, kURLAllEventsMask, &gDlDone);

	if (status == noErr)
	{
		gTimeout=TickCount();
		//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
		versionErr = Gestalt(gestaltSystemVersion, &MacVersion);
		if (versionErr != noErr)
		{
			strcpy(debug, "Version Fetch Error : ");
			tb_NumToString(versionErr, debug2);
			strcat(debug, debug2);
			fi_WriteToErrorLog(debug);
		}
		else
		{
			if(MacVersion >=0x1000){
				strcpy(fileName, "mainx.obu");
			}
			else if (MacVersion <= 0x0999){
				strcpy(fileName, "main9.obu");
			}
		}
		//End of version get.
		up_ShowProgressWindow(-1);

		InstallEventLoopTimer (GetMainEventLoop() , 0, kEventDurationMillisecond, gMyIdleTimerUPP, NULL, NULL);
	}
	else
	{
		up_ShowResultsAndDisposeURLRef(&gURLRef);
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_Update Exit");

#endif

}

// ----------------------------------------------------------------- //
void up_ContinueUpdate(int which)
// ----------------------------------------------------------------- //

{
	OSStatus			status;

	Str255				where="\p:Updater temp:", temp;
	//char				download[kStringLength], tempsize[kStringLength];


#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ContinueUpdate Entry");
#endif

	/*if (strcmp(url, gCurrentUrl)==0)
	{
		currentFileNumber--;
		return;
	}

	strcpy(gCurrentUrl, url);*/

	PLstrcpy(where, "\p:Updater temp:");
	CopyCStringToPascal(gCompressedFileName[which], temp);
	PLstrcat(where, temp);

	FSMakeFSSpec(0,0, where, &gFile);

	gDlDone=false;

	URLDisposeReference(gURLRef);
	URLNewReference(gDownload[which], &gURLRef);

	status = URLOpen(gURLRef, &gFile, kUrlFlags, gMyNotifierUPP, kURLAllEventsMask, &gDlDone);

	if (status == noErr)
	{
		gTimeout=TickCount();
		gProcessing=false;
		gPreviousStates[which]=gStates[which];
		gStates[which]=kDownloading;


		up_ShowProgressWindow(which);


		InstallEventLoopTimer (GetMainEventLoop() , 0, kEventDurationMillisecond, gMyIdleTimerUPP, NULL, NULL);

	}
	else
	{
		up_ShowResultsAndDisposeURLRef(&gURLRef);
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ContinueUpdate Exit");

#endif

}

// ----------------------------------------------------------------- //
OSStatus up_ShowProgressWindow(int which)
// ----------------------------------------------------------------- //

{
	Str255 		tempFileName;
	Str255		outputText, temp;
	ControlRef		control;
	OSStatus		err;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ShowProgressWindow Entry");
#endif


	if (gProgressWindow == NULL)
	{
		gProgressWindow=GetNewDialog(130, NULL, (WindowRef)-1);
		GetDialogItemAsControl(gProgressWindow, 4, &gAbortButton);
		GetDialogItemAsControl(gProgressWindow, 5, &gQuitButton);
		GetDialogItemAsControl(gProgressWindow, 6,&control);
		GetDialogItemAsControl(gProgressWindow, 8,&gResumeButton);
		DeactivateControl(gResumeButton);
		GetControlData(control,kControlEntireControl,kControlListBoxListHandleTag,
					   sizeof(gList),&gList,NULL);



		gListDefFunctionUPP = NewListDefUPP((ListDefProcPtr) up_listDefFunction);
		ShowWindow(GetDialogWindow(gProgressWindow));
	}

	if (gProgressWindow != NULL)
	{
		up_UpdateList(which);

		/*if (currentFileNumber!=0)
		{
			PLstrcpy(outputText, "\pFile ");
			NumToString(currentFileNumber, temp);
			PLstrcat(outputText, temp);
			PLstrcat(outputText, "\p/");
			NumToString(totalNumberOfFiles, temp);
			PLstrcat(outputText, temp);
			GetDialogItemAsControl(gProgressWindow, 4, &control);
			SetControlData(control, kControlEntireControl, kControlStaticTextTextTag, outputText[0], &outputText[1]);
			Draw1Control(control);
		}

		DrawControls(GetDialogWindow(gProgressWindow));

		ShowWindow(GetDialogWindow(gProgressWindow));*/
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ShowProgressWindow Exit");

#endif

	return err;

}

// ----------------------------------------------------------------- //
void up_UpdateList(int which)
// ----------------------------------------------------------------- //
{

	char		cellData[kStringLength], totalsize[kStringLength];
	Cell		thePoint;
	Str255		theString, theOtherString;
	int			size;
	ControlRef	theControl;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_UpdateList Entry");
#endif

	GetDialogItemAsControl(gProgressWindow, 1, &theControl);

	strcpy(cellData, "");

	switch (gStates[which])
	{
		case kDownloading:
			strcat(cellData, "Downloading...");
			PLstrcpy(theString, "\pDownloading ");
			CopyCStringToPascal(gFileName[which], theOtherString);
			PLstrcat (theString, theOtherString);
			break;

		case kDownloadStalled:
			strcat(cellData, "Downloading...");
			PLstrcpy(theString, "\pStalled: ");
			CopyCStringToPascal(gFileName[which], theOtherString);
			PLstrcat (theString, theOtherString);
			break;

		case kDownloaded:
			strcat(cellData, "Downloaded");
			PLstrcpy(theString, "\pIdle");
			break;

		case kUnstuffing:
			strcat(cellData, "Extracting...");
			PLstrcpy(theString, "\pUnstuffing ");
			CopyCStringToPascal(gFileName[which], theOtherString);
			PLstrcat (theString, theOtherString);
			break;

		case kStuffitStalled:
			strcat(cellData, "Extracting...");
			PLstrcpy(theString, "\pStalled: ");
			CopyCStringToPascal(gFileName[which], theOtherString);
			PLstrcat (theString, theOtherString);
			break;

		case kUnstuffed:
			strcat(cellData, "Extracted");
			PLstrcpy(theString, "\pIdle");
			break;
		/*case kInstalling:
			strcat(cellData, "Installing...");
			PLstrcpy(theString, "\pMoving new file to location.");
			break;
*/
		case kCompleted:
			strcat(cellData, "Installed");
			PLstrcpy(theString, "\pIdle");
			break;

		case kAborted:
			strcat(cellData, "Aborted");
			PLstrcpy(theString, "\pIdle");
			break;

		default:
			strcat(cellData, "Idle");
			PLstrcpy(theString, "\pIdle");
			break;
	}

	SetControlData(theControl, kControlEntireControl, kControlStaticTextTextTag, theString[0], &theString[1]);

	SetPt (&thePoint,0,which);
	CopyCStringToPascal(cellData, theString);
	LSetCell(theString+1,(SInt16) StrLength(theString), thePoint, gList);

	strcpy(cellData,gFileName[which]);
	SetPt (&thePoint,1,which);
	CopyCStringToPascal(cellData, theString);
	//StandardAlert(kAlertStopAlert, NULL, theString, NULL, NULL);
	LSetCell(theString+1,(SInt16) StrLength(theString), thePoint, gList);

	if (gStates[which]!=0)
	{
		size=gHowMany[which]/1024;
		tb_NumToString(size, cellData);
		strcat(cellData, " Kb / ");
		size=gSize[which]/1024;
		tb_NumToString(size, totalsize);
		strcat(cellData, totalsize);
		strcat(cellData, " Kb");

		SetPt (&thePoint,2,which);
		CopyCStringToPascal(cellData, theString);
		LSetCell(theString+1,(SInt16) StrLength(theString), thePoint, gList);
	}

	GetDialogItemAsControl(gProgressWindow, 7, &theControl);

	SetControlData(theControl, kControlEntireControl, kControlStaticTextTextTag, gLatestMessage[0], &gLatestMessage[1]);

	DrawControls(GetDialogWindow(gProgressWindow));

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_UpdateList Exit");
#endif
}

// ----------------------------------------------------------------- //
void up_ShowResultsAndDisposeURLRef(URLReference * urlRef)
// ----------------------------------------------------------------- //

{
	URLState		dlState = kURLNullState;
	OSStatus 		urlError = noErr;
	OSStatus 		err = noErr;
	Str255			title="\pOberin Auto Updater", text="\pThe update server is busy right now. Please try again later.", error;


#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ShowResultsAndDisposeURLRef Entry");
#endif

	err = URLGetCurrentState(*urlRef, &dlState);

	if (err == noErr)
	{
		switch (dlState)
		{
			case kURLErrorOccurredState:

				URLGetError (*urlRef, &urlError);

				PLstrcpy(text, kTransferFailedError);

				NumToString(urlError, error);

				PLstrcat(text, error);

				StandardAlert(kAlertStopAlert, title, text, NULL, NULL);

				err = FSpDelete(&gFile);

				break;

			case kURLCompletedState:

				PLstrcpy(text, kTransferSuccessfulError);
				StandardAlert(kAlertStopAlert, title, text, NULL, NULL);
				break;

			default:
				//This may happen if running on Mac OS 9 because of a bug in URL Access.
				PLstrcpy(text, kURLAccessConfusedError);
				StandardAlert(kAlertStopAlert, title, text, NULL, NULL);
				break;
		}


	}

	tb_Halt();

	//URLDisposeReference(*urlRef);
#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ShowResultsAndDisposeURLRef Exit");
#endif
}


// ----------------------------------------------------------------- //
OSStatus MyNotifier(void* userContext, URLEvent event, URLCallbackInfo *callbackinfo)
// ----------------------------------------------------------------- //

{
	Boolean			state;
	ControlRef 		control;
	OSStatus 		err;
	static long 	percentComplete;
	SInt16			theRef, theRF;
	long			size1, size2, theTotal;
	Str255			errt="\pError", timem="\pLatest message :\nDownload looks stalled... Connection problems?.", errm="\pAn unknown error occurred. The application will now terminate. Please go to http://www.oberin.com and update your client manually.";
	Str255			where, theString;
	FSSpec			theFile;
	Size			theSize;
	float			theDivide, theUpvide;

	char			debug[kStringLength];

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("MyNotifier Entry");
#endif

	DrawControls(GetDialogWindow(gProgressWindow));

	switch (event)
	{
		case kURLDataAvailableEvent:
			up_ProcessData(&callbackinfo->urlRef);
			break;
		case kURLCompletedEvent:

			if (!gProcessing)
			{
				//HideWindow(GetDialogWindow(gProgressWindow));
				gDlDone = true;
			}

			break;

		case kURLInitiatedEvent:
			percentComplete = 0;
			err = GetDialogItemAsControl(gProgressWindow, 2, &control);

			if (err == noErr)
			{
				SetControlData(control, 0, kControlStaticTextTextTag, sizeof(gFileName[currentFileNumber]), &gFileName[currentFileNumber]);
				up_UpdateList(currentFileNumber);
			}

			break;

		case kURLErrorOccurredEvent:

			if (!gAbort && !gUrlAborted)
			{
				StandardAlert(kAlertStopAlert, errt, errm, NULL, NULL);
				tb_Halt();
			}

			gAbort=false;

			break;
		case kURLAbortInitiatedEvent:
			break;
		case kURLResourceFoundEvent:
		case kURLDownloadingEvent:
		case kURLTransactionCompleteEvent:
		case kURLUploadingEvent:
		case kURLSystemEvent:
		case kURLPropertyChangedEvent:
			break;
		case kURLPeriodicEvent:
			//up_Idle();
			IdleControls(GetDialogWindow(gProgressWindow));

			if (TickCount()>gTimeout+kDlTimeout)
			{
				gTimeout=TickCount();
				gStates[currentFileNumber]=kDownloadStalled;
				PLstrcpy(gLatestMessage, timem);
				up_UpdateList(currentFileNumber);
			}

			break;

		case kURLPercentEvent:

			URLGetProperty(gURLRef, kURLResourceSize, &gSize[currentFileNumber], sizeof(gSize[currentFileNumber]));

			PLstrcpy(where, "\p:Updater temp:");
			CopyCStringToPascal(gCompressedFileName[currentFileNumber], theString);
			PLstrcat(where, theString);

			FSMakeFSSpec(0,0, where, &theFile);

			FSpOpenDF(&theFile, fsRdPerm, &theRef);
			FSpOpenRF(&theFile, fsRdPerm, &theRF);
			GetEOF(theRef,&size1);
			GetEOF(theRF, &size2);
			FSClose(theRef);
			FSClose(theRF);

			gHowMany[currentFileNumber]=(float)(size1+size2);

			up_UpdateList(currentFileNumber);

			gTimeout=TickCount();

			if (gStates[currentFileNumber]==kDownloadStalled)
			{
				gStates[currentFileNumber]=kDownloading;
				PLstrcpy(gLatestMessage, "\pLatest message :\nAh, just received some data. Phew!");
				up_UpdateList(currentFileNumber);
			}

			break;

		default:
			break;
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("MyNotifier Exit");

#endif

	return noErr;
}

// ----------------------------------------------------------------- //
OSStatus MyIdleTimer(EventLoopTimerRef inTimer, void *inUserData )
// ----------------------------------------------------------------- //

{
	OSStatus	err = eventNotHandledErr, waitErr;

	char		inString[kStringLength], versionString[kStringLength], toDownload[kStringLength];
	TBoolean	versionFound=false, dum;

	int			cpt, cpt2;
	Str255		error="\pError", versionError="\pVersion not found. Aborting";

	char		url[kStringLength], versionStr[kStringLength], debug[kStringLength];
	Str255		where, theString, where2;
	int			cpt3;
	long		dummy;
	char		cellData[kStringLength], tempsize[kStringLength];


	int			wait, sub, spept;
	TBoolean	state, already=false, complete=true;
	ControlRef	control;

	DialogRef	theDialog;

	Str255		titleEnd="\pUpdate Complete", textEnd="\pYour version of Oberin was successfully updated.";
	Str255		versionm="\pYou are using too old of a version of Oberin to update. Please go to http://www.oberin.com and download the client again.";

	OSStatus	theErr;

	Point		thePoint;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("MyIdleTimer Entry");
#endif

	URLIdle();

	if (gUrlAborted)
	{
		gUrlAborted=false;
		RemoveEventLoopTimer(inTimer);
	}


	if (gDlDone)
	{
		gProcessing=true;
		//gDlDone = false;
		RemoveEventLoopTimer(inTimer);

		if (currentFileNumber<totalNumberOfFiles || currentFileNumber==-1) //Update not done yet
		{
			if (currentFileNumber==-1) //Just downloaded main.obu, building up list
			{
				tb_NumToString(kClientVersion, versionString);

				
				//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
				versionErr = Gestalt(gestaltSystemVersion, &MacVersion);
				if (versionErr != noErr)
				{
					strcpy(debug, "Version Fetch Error : ");
					tb_NumToString(versionErr, debug2);
					strcat(debug, debug2);
					fi_WriteToErrorLog(debug);
				}
				else
				{
					if(MacVersion >=0x1000){
						mainFile=fopen(":Updater temp:mainx.obu", "r");
					}
					else if (MacVersion <= 0x0999){
						mainFile=fopen(":Updater temp:main9.obu", "r");
					}
				}
				//End of version get.

				

				if (mainFile != NULL)
				{
					fseek( mainFile, 0, SEEK_SET );totalNumberOfFiles=0;

					while (!versionFound && !feof(mainFile))
					{
						fgets(inString,kStringLength,mainFile);

						if (inString[0]=='*')
						{
							versionFound=true;
							cpt=1;

							while (inString[cpt]!='\n')
							{
								versionStr[cpt-1]=inString[cpt];
								cpt++;
							}

							for (cpt2=cpt-1; cpt2<kStringLength; cpt2++)
								versionStr[cpt2]='\0';

							if (strcmp(versionStr, versionString)!=0)
								versionFound=false;
						}
					}

					if (!versionFound)
					{
						StandardAlert(kAlertStopAlert, error, versionm, NULL, NULL);
						tb_Halt();
					}

					while (!feof(mainFile))
					{
						fgets(inString,kStringLength,mainFile);
						cpt3=0;

						if (inString[0]!='*' && inString[0]!='$')
						{
							strcpy(url, inString);
							strcpy(gToDownload, inString);

							gDlDone=false;

							sub=0;


							while (url[sub]!='*')
								sub++;

							cpt2=0;

							for (spept=0; spept<kStringLength; spept++)
								gLocalPath[totalNumberOfFiles][spept]='\0';


							for (cpt=sub+1; cpt<strlen(url)-1; cpt++)
								gLocalPath[totalNumberOfFiles][cpt2++]=url[cpt];

							gLocalPath[totalNumberOfFiles][cpt-1]='\0';


							cpt=strlen(gLocalPath[totalNumberOfFiles]);

							while (gLocalPath[totalNumberOfFiles][cpt--]!='*');


							gLocalPath[totalNumberOfFiles][cpt+1]='\0';


							cpt2=cpt+2;

							cpt=0;

							for (spept=0; spept<kStringLength; spept++)
								tempsize[spept]='\0';

							while (gLocalPath[totalNumberOfFiles][cpt2]!='\0')
								tempsize[cpt++]=gLocalPath[totalNumberOfFiles][cpt2++];

							tempsize[cpt]='\0';


							tb_StringToNum(tempsize, &gFileSize[totalNumberOfFiles]);

							for (cpt=0; cpt<sub; cpt++)
								gDownload[totalNumberOfFiles][cpt]=url[cpt];

							gDownload[totalNumberOfFiles][sub]='\0';

							sub=strlen(gLocalPath[totalNumberOfFiles]);

							while (sub!=0 && gLocalPath[totalNumberOfFiles][sub]!=':')
								sub--;



							for (spept=0; spept<kStringLength; spept++)
								gFileName[totalNumberOfFiles][spept]='\0';

							cpt2=0;

							for (cpt=sub+1; cpt<strlen(gLocalPath[totalNumberOfFiles]); cpt++)
								gFileName[totalNumberOfFiles][cpt2++]=gLocalPath[totalNumberOfFiles][cpt];

							gFileName[totalNumberOfFiles][cpt]='\0';


							cpt2=strlen(gDownload[totalNumberOfFiles]);

							while (gDownload[totalNumberOfFiles][cpt2]!='/')
								cpt2--;

							for (spept=0; spept<kStringLength; spept++)
								gCompressedFileName[totalNumberOfFiles][spept]='\0';

							for (cpt=cpt2+1; cpt<strlen(gDownload[totalNumberOfFiles]); cpt++)
								gCompressedFileName[totalNumberOfFiles][cpt3++]=gDownload[totalNumberOfFiles][cpt];

							gCompressedFileName[totalNumberOfFiles][cpt3]='\0';

							totalNumberOfFiles++;
						}

					}

					totalNumberOfFiles--;  //Last \n

					LAddRow(totalNumberOfFiles, 0, gList);

					for (cpt=0; cpt<totalNumberOfFiles; cpt++)
					{
						gStates[cpt]=0;
						up_UpdateList(cpt);
					}

					currentFileNumber=0;

					up_ContinueUpdate(currentFileNumber);

				}
			}
			else
			{

				gStates[currentFileNumber]=kDownloaded;
				up_DecompressFile(currentFileNumber);
			}

		}
		else
		{
			up_FinishUpdate();
		}
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("MyIdleTimer Exit");

#endif

	return err;
}

// ----------------------------------------------------------------- //
void up_ProcessData(URLReference * urlRef)
// ----------------------------------------------------------------- //

{
	URLState currentState;
	char * tempBuffer;
	void * bufferPtr;
	Size bufferAllocated;
	Size bufferSize;
	Size bufferUsed;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ProcessData Entry");
#endif

	URLGetDataAvailable(*urlRef,  &bufferAllocated);
	tempBuffer = malloc(bufferAllocated);
	tempBuffer[0] = '\0';

	bufferUsed = 0;

	do
	{
		// The URLGetBuffer function obtains the next buffer of data in a download operation.
		// URLGetBuffer does not enable you to retain or modify the transferred data.
		URLGetBuffer(*urlRef, &bufferPtr, &bufferSize);

		// This is where you would do stuff to the buffer
		strncat(tempBuffer, bufferPtr, bufferSize);
		bufferUsed += bufferSize;

		// The URLReleaseBuffer function releases the buffer obtained by calling the function
		// URLGetBuffer. To prevent the URL Access Manager from running out of buffers, you should
		// call URLReleaseBuffer after each call to URLGetBuffer.
		URLReleaseBuffer(*urlRef, bufferPtr);

		// The URLGetCurrentState function determines the current status of a data transfer operation. You may
		// wish to call URLGetCurrentState periodically to monitor the status of a download or upload operation.
		URLGetCurrentState (*urlRef, &currentState);

	}
	while (currentState == kURLDataAvailableState);

	up_ParseBuffer(tempBuffer);

	free(tempBuffer);

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ProcessData Exit");

#endif
}

// ----------------------------------------------------------------- //
void up_ParseBuffer(char * bufferIn)
// ----------------------------------------------------------------- //

{
	const char startResults[] = "<!--SEARCH_RESULTS_START-->";
	const char endResults[] = "<!--SEARCH_RESULTS_END-->";
	const char startUL[] = "<UL>";
	const char endUL[] = "</UL>";
	char * BeginPointer;
	char * EndPointer;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ParseBuffer Entry");
#endif

	EndPointer = strstr(bufferIn, endResults);
	*EndPointer = '\0';
	BeginPointer = strstr(bufferIn, startResults);

	if (strlen(BeginPointer) > 35)
	{
		EndPointer = strstr(BeginPointer, endUL);
		*EndPointer = '\0';

		BeginPointer = strstr(BeginPointer, startUL);

		strcpy(gPostOutput, BeginPointer+strlen(startUL));

		if (strlen(gPostOutput) > 150)
		{
			strcpy(gPostOutput + 150, "...");
		}
	}
	else
	{
		gPostOutput[0] = '\0';
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ParseBuffer Exit");

#endif
}

// ----------------------------------------------------------------- //
OSErr up_FinderLaunch(long nTargets, FSSpec *targetList)
// ----------------------------------------------------------------- //
{
	OSErr err;
	AppleEvent theAEvent, theReply;
	AEAddressDesc fndrAddress;
	AEDescList targetListDesc;
	OSType fndrCreator;
	Boolean wasChanged;
	AliasHandle targetAlias;
	long index;

	char debug[kStringLength], debug2[kStringLength];

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_FinderLaunch Entry");
#endif


	
	/* verify parameters */

	if ((nTargets == 0) || (targetList == NULL)) return paramErr;

	/* set up locals  */
	AECreateDesc(typeNull, NULL, 0, &theAEvent);

	AECreateDesc(typeNull, NULL, 0, &fndrAddress);

	AECreateDesc(typeNull, NULL, 0, &theReply);

	AECreateDesc(typeNull, NULL, 0, &targetListDesc);

	targetAlias = NULL;

	fndrCreator = 'MACS';

	/* create an open documents event targeting the finder */
	err = AECreateDesc(typeApplSignature, (Ptr) &fndrCreator,
					   sizeof(fndrCreator), &fndrAddress);

	if (err != noErr) goto bail;

	err = AECreateAppleEvent(kCoreEventClass, kAEOpenDocuments,
							 &fndrAddress, kAutoGenerateReturnID,
							 kAnyTransactionID, &theAEvent);

	if (err != noErr) goto bail;

	/* create the list of files to open */
	err = AECreateList(NULL, 0, false, &targetListDesc);

	if (err != noErr) goto bail;

	for ( index=0; index < nTargets; index++)
	{
		if (targetAlias == NULL)
			err = NewAlias(NULL, (targetList + index), &targetAlias);
		else err = UpdateAlias(NULL, (targetList + index), targetAlias, &wasChanged);

		if (err != noErr) goto bail;

		HLock((Handle) targetAlias);

		err = AEPutPtr(&targetListDesc, (index + 1), typeAlias, *targetAlias, GetHandleSize((Handle) targetAlias));

		HUnlock((Handle) targetAlias);

		if (err != noErr) goto bail;
	}

	/* add the file list to the apple event */
	err = AEPutParamDesc(&theAEvent, keyDirectObject, &targetListDesc);

	if (err != noErr) goto bail;
	//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
	if(MacVersion >=0x1000){//If OSX
		err = AESend(&theAEvent, &theReply, kAENoReply,
					 kAEHighPriority, kAEDefaultTimeout, MyIdleUPP, NULL);
	}
	else if (MacVersion <= 0x0999){//If OS 9 and below
	/* Send the event to the Finder and wait reply*/
		err = AESend(&theAEvent, &theReply, kAEWaitReply,
					 kAEHighPriority, kAEDefaultTimeout, MyIdleUPP, NULL);
	}

	/* clean up and leave */
bail:
	if (targetAlias != NULL) DisposeHandle((Handle) targetAlias);

	AEDisposeDesc(&targetListDesc);

	AEDisposeDesc(&theAEvent);

	AEDisposeDesc(&fndrAddress);

	AEDisposeDesc(&theReply);

	if (err!=noErr)
	{
		strcpy(debug, "up_FinderLaunch : ");
		tb_NumToString(err, debug2);
		strcat(debug, debug2);
		fi_WriteToErrorLog(debug);
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_FinderLaunch Exit");

#endif

	return err;
}

// ----------------------------------------------------------------- //
void up_FinishUpdate()
// ----------------------------------------------------------------- //
{

	Str255		titleEnd="\pUpdate Complete", textEnd="\pYour version of Oberin was successfully updated.";

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_FinishUpdate Entry");
#endif

	up_ClearUpdateFolder();

	StandardAlert(kAlertStopAlert, titleEnd, textEnd, NULL, NULL);

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_FinishUpdate Exit");
#endif

	tb_Halt();

}

// ----------------------------------------------------------------- //
void up_DecompressFile(int which)
// ----------------------------------------------------------------- //
{
	int 			cpt, cpt2, getLoopError=0;
	Str255 			where, theString, where2, error="\pError";
	OSStatus		waitErr;
	long			size1, size2;
	FSSpec			theFile, theFile2, theDirSpec;
	EventRecord		theEvent;
	FInfo			theInfo;
	char			test[kStringLength];
	SInt16			theRef, theRF;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_DecompressFile Entry");
#endif
	

	
	gPreviousStates[which]=gStates[which];
	gStates[which]=kUnstuffing;
	up_UpdateList(which);

	PLstrcpy(where, "\p:Updater temp:");
	CopyCStringToPascal(gCompressedFileName[which], theString);
	PLstrcat(where, theString);

	if (FSMakeFSSpec(0,0, where, &theFile)!=noErr)
	{
		PLstrcpy(gLatestMessage, "\pLatest Message : Downloaded file not found. Download the file before trying to extract it.");
		gBusy=false;
		up_UpdateList(which);
		return;
	}

	PLstrcpy(where, "\p:Updater temp:");

	CopyCStringToPascal(gFileName[which], theString);
	PLstrcat(where, theString);

	PLstrcpy(where2, "\p:Updater temp:");
	CopyCStringToPascal(gFileName[which], theString);
	PLstrcat(where2, theString);

	FSMakeFSSpec(0,0, where, &theFile2);
	up_FinderLaunch(1, &theFile);
	//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
	if(MacVersion >=0x1000){//If OSX
		tb_WaitTick(120);
		up_InstallFile(which);
	}
	else if (MacVersion <= 0x0999){//If OS 9 and below
		if(!gAbort)
		{
			gTimeout=TickCount();
		do
		{
			up_Idle();
			if(TickCount()>gTimeout+kSizeTimeout){
				getLoopError=100;
			}
		}
		while(getLoopError!=100 && gAbort==false);
		up_InstallFile(which);
	}
#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_DecompressFile Exit");

#endif

}
}
// ----------------------------------------------------------------- //
void up_InstallFile(int which)
// ----------------------------------------------------------------- //
{
	Str255 		theString, where, where2, error="\pError", oldob="\pOldOberin", oldc="\p:OldOberin";
	OSStatus 	theErr;
	char		debug[kStringLength], theDir[kStringLength];
	FSSpec		theFile, theFile2, theFile3, theDirSpec;
	int			cpt, cpt2, cpt3;
	long		dummy;
	Str255		fsperr="\pA file-system related error occured. Please Send your error log to jinker@oberin.com";
	TBoolean 	finish;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_InstallFile Entry");
#endif
	PLstrcpy(where2, "\p:Updater temp:");
	CopyCStringToPascal(gFileName[which], theString);
	PLstrcat(where2, theString);
	theErr=FSMakeFSSpec(0,0, where2, &theFile);
	if (theErr!=noErr)
	{
		PLstrcpy(gLatestMessage, "\pLatest Message : File not found. Will have to redownload.");
		gBusy=false;
		up_UpdateList(which);
		return;
	}

	CopyCStringToPascal(gLocalPath[which], where);

	theErr=FSMakeFSSpec(0,0, where, &theFile2);

	if (theErr!=noErr)
	{
		fi_WriteToErrorLog("up_Update : Destination file not found");
		fi_WriteToErrorLog(gFileName[which]);
	}
	else
	{
		if (strcmp(gFileName[which], "Oberin")==0)
		{

			FSMakeFSSpec(0,0, oldc, &theFile3);
			FSpDelete(&theFile3);
			FSpRename(&theFile2, oldob);

		}
		else
		{
			theErr=FSpDelete(&theFile2);

			if (theErr!=noErr)
			{
				fi_WriteToErrorLog("up_Update : Unable to delete destination file");
				fi_WriteToErrorLog(gFileName[which]);
				NumToString(theErr, where);
				CopyPascalStringToC(where, debug);
				fi_WriteToErrorLog(debug);
				StandardAlert(kAlertStopAlert, error, fsperr, NULL, NULL);
				tb_Halt();
			}
		}

	}

	cpt2=strlen(gLocalPath[which]);

	while (cpt2!=0 && gLocalPath[which][cpt2]!=':')
		cpt2--;

	if (cpt2!=0)
	{
		for (cpt3=0; cpt3<=cpt2; cpt3++)
			theDir[cpt3]=gLocalPath[which][cpt3];

		theDir[cpt3]='\0';

		CopyCStringToPascal(theDir, where);
	}
	else
		PLstrcpy(where, "\p:");

	FSMakeFSSpec(0,0, where, &theDirSpec);

	FSpDirCreate(&theDirSpec, smSystemScript, &dummy);

	theErr=FSpCatMove(&theFile, &theDirSpec);

	if (theErr!=noErr)
	{
		fi_WriteToErrorLog("up_Update : Unable to move file");
		fi_WriteToErrorLog(gFileName[which]);
		NumToString(theErr, where);
		CopyPascalStringToC(where, debug);
		fi_WriteToErrorLog(debug);
		StandardAlert(kAlertStopAlert, error, fsperr, NULL, NULL);
		tb_Halt();
	}

	FSMakeFSSpec(0,0, where2, &theFile);

	FSpDelete(&theFile);

	gStates[which]=kCompleted;
	up_UpdateList(which);

	finish=true;

	for (cpt=0; cpt<totalNumberOfFiles; cpt++)
	{
		if (gStates[cpt]!=kCompleted)
			finish=false;
	}

	if (finish)
		up_FinishUpdate();

	if (gAuto)
	{
		currentFileNumber++;
		up_ContinueUpdate(currentFileNumber);
	}
	else
		gBusy=false;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_InstallFile Exit");

#endif
}


// ----------------------------------------------------------------- //
void up_ClearUpdateFolder()
// ----------------------------------------------------------------- //
{
	OSStatus	waitErr;
	Str255		dir="\p:Updater temp:", obu, del, dsstore="\p:Updater temp:.DS_Store", xobu="\p:Updater temp:mainx.obu", vobu="\p:Updater temp:main9.obu";
	FSSpec		theDir, theObu, theDSS;
	int 		cpt;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ClearUpdateFolder Entry");
#endif
	//Get the version of OS.  If it cannot fetch and there is an error, write to log.  Also write Mac OS version to log - This is to test the fetching method.
		if(MacVersion >=0x1000){
			PLstrcpy(obu, xobu);
		}
		else if (MacVersion <= 0x0999){
			PLstrcpy(obu, vobu);
		}
	//End of version get.
	fclose(mainFile);

	FSMakeFSSpec(0,0, obu, &theObu);
	FSpDelete(&theObu);

	for (cpt=0; cpt<totalNumberOfFiles; cpt++)
	{
		CopyCStringToPascal(gCompressedFileName[cpt], obu);
		PLstrcpy(del, dir);
		PLstrcat(del, obu);
		FSMakeFSSpec(0,0, del, &theObu);
		waitErr=FSpDelete(&theObu);
		

		CopyCStringToPascal(gFileName[cpt], obu);
		PLstrcpy(del, dir);
		PLstrcat(del, obu);
		FSMakeFSSpec(0,0, dir, &theObu);
		waitErr=FSpDelete(&theObu);
	}
	
	FSMakeFSSpec(0,0, dsstore, &theDSS);
	FSpDelete(&theDSS);
	
	FSMakeFSSpec(0,0, dir, &theDir);
	
	FSpDelete(&theDir);

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_ClearUpdateFolder Exit");
#endif

}

// ----------------------------------------------------------------- //
void up_Idle()
// ----------------------------------------------------------------- //
{
	EventRecord						theEvent;
	static  long					sleepTime = 1L;
	TBoolean						hiEvent=false;
	int								match=-1;

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_Idle Entry");
#endif


	if (WaitNextEvent(everyEvent, &theEvent, sleepTime, NULL))
	{
		if ( IsDialogEvent(&theEvent) == true )
			if (theEvent.what==mouseDown)
			{
				up_DialogEvent(&theEvent);
				hiEvent=true;
			}

		switch (theEvent.what)
		{

			case mouseDown:
			{
				DoMouseDown(&theEvent);
				hiEvent=true;
			}

			break;

			case keyDown:
			{
				DoKey(&theEvent,false);
				hiEvent=true;
			}

			break;

			case autoKey:
			{
				DoKey(&theEvent,true);
				hiEvent=true;
			}

			break;

			case activateEvt:
			{
				DoActivate(&theEvent);
				hiEvent=true;
			}

			break;

			case osEvt:
			{
				if ((theEvent.message >> 24) & suspendResumeMessage )
					DoSuspendResume((theEvent.message & resumeFlag)!=0);

				hiEvent=true;
			}

			break;

			case kHighLevelEvent:
			{
				(void)AEProcessAppleEvent(&theEvent);
				hiEvent=true;
			}

		}
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_Idle Exit");

#endif
}

// ----------------------------------------------------------------- //
void up_DialogEvent(EventRecord *eventStrucPtr)
// ----------------------------------------------------------------- //
{
	ControlRef		controlRef, theControl, control;
	SInt16			controlPartCode;
	Point			thePoint, theCell;
	Boolean			state;
	OSStatus		err;
	EventRecord		dummy;



	char test[kStringLength];

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_DialogEvent Entry");
#endif

	SetPortWindowPort(GetDialogWindow(gProgressWindow));
	SetPt(&thePoint, eventStrucPtr->where.h, eventStrucPtr->where.v);
	GlobalToLocal(&thePoint);

	GetDialogItemAsControl(gProgressWindow, 6, &theControl);


	if (controlPartCode = FindControl(thePoint,GetDialogWindow(gProgressWindow),&controlRef))
	{

		if (controlRef==gAbortButton)
		{
			if (TrackControl(controlRef, thePoint, NULL))
			{
				gAbort=true;
				/*err = GetDialogItemAsControl(gProgressWindow, 2, &control);
				if (err == noErr)
				{
					state = false;
					SetControlValue(control, 0);
					SetControlData(control, 0, kControlProgressBarIndeterminateTag, sizeof(state), &state);
					DrawControls(GetDialogWindow(gProgressWindow));
				}*/
				PLstrcpy(gLatestMessage, "\pLatest Message :");
				up_UpdateList(currentFileNumber);

				switch (gStates[currentFileNumber])
				{
					case kDownloading:
					case kDownloadStalled:
						gUrlAborted=true;
						URLAbort(gURLRef);
						gStates[currentFileNumber]=kAborted;

						if (gAuto)
						{
							if (currentFileNumber==totalNumberOfFiles-1)
							{
								gAuto=false;
								gBusy=false;
								PLstrcpy(gLatestMessage, "\pLatest Message : Looks like the update didn't go so well. Click on an aborted element and on 'Retry' to try again.");
								up_UpdateList(currentFileNumber);
							}
							else
							{
								up_UpdateList(currentFileNumber);
								currentFileNumber++;
								up_ContinueUpdate(currentFileNumber);
							}
						}

						gBusy=false;

						up_UpdateList(currentFileNumber);
						break;

					case kUnstuffing:
					case kStuffitStalled:
						gStates[currentFileNumber]=kDownloaded;
						up_UpdateList(currentFileNumber);
						break;

					default:

						if (!gBusy)
						{
							PLstrcpy(gLatestMessage, "\pLatest Message :\nNothing to abort!");
							tb_Beep(0);
						}
						else
							gBusy=false;

						up_UpdateList(currentFileNumber);

						break;
				}
			}
		}

		if (controlRef==gQuitButton)
		{
			if (TrackControl(controlRef, thePoint, NULL))
			{
				gUrlAborted=true;
				URLAbort(gURLRef);
				up_ClearUpdateFolder();
				tb_Halt();
			}
		}

		if (controlRef==theControl)
		{
			if (!gAuto && !gBusy)
				if (HandleControlClick(controlRef, thePoint, NULL, NULL))
				{
					theCell=LLastClick(gList);

					if (theCell.v>=totalNumberOfFiles)
					{
						DeactivateControl(gResumeButton);
						return;
					}

					if (gStates[theCell.v]==kCompleted)
					{
						DeactivateControl(gResumeButton);
						return;
					}


					ActivateControl(gResumeButton);
				}
		}

		if (controlRef==gResumeButton)
		{
			theCell=LLastClick(gList);

			if (HandleControlClick(gResumeButton, thePoint, NULL, NULL))
			{
				currentFileNumber=theCell.v;
				PLstrcpy(gLatestMessage, "\pLatest Message :");
				up_UpdateList(currentFileNumber);
				gBusy=true;
				DeactivateControl(gResumeButton);
				up_ContinueUpdate(theCell.v);
			}

		}
	}

#ifdef _FUNCTIONTRACEDEBUGGING
	fi_WriteToErrorLog("up_DialogEvent Exit");

#endif
}

// ----------------------------------------------------------------- //
pascal Boolean MyIdleCallback(EventRecord * theEvent, SInt32 * sleepTime, RgnHandle * mouseRgn)
// ----------------------------------------------------------------- //
{
	return true;
}

// ----------------------------------------------------------------- //
void  up_listDefFunction(SInt16 message,Boolean selected,Rect *cellRect,Cell theCell, SInt16 dataOffset,SInt16 dataLen,ListHandle theList)
// ----------------------------------------------------------------- //
{
	if (message==lDrawMsg)
		up_doLDEFDraw(selected,cellRect,theCell,dataLen,theList);

}

// ----------------------------------------------------------------- //
void  up_doLDEFDraw(Boolean selected,Rect *cellRect,Cell theCell,SInt16 dataLen, ListHandle theList)
// ----------------------------------------------------------------- //
{
	/*GrafPtr		oldPort;
	Rect		drawRect, theRect;
	Handle		iconSuiteHdl;
	Str255		theString;
	RGBColor	theColor;

	char	state[kStringLength];

	GetPort(&oldPort);

	SetPort(GetListPort(theList));

	EraseRect(cellRect);

	drawRect = *cellRect;

	drawRect.top += 10;
	drawRect.left += 10;
	drawRect.right -= 10;
	drawRect.bottom -= 10;

	if(dataLen == sizeof(Handle))
	{
		LGetCell(&iconSuiteHdl,&dataLen,theCell,theList);

		if(GetListActive(theList))
			PlotIconSuite(&drawRect,kAlignNone,kTransformNone,iconSuiteHdl);
		else
			PlotIconSuite(&drawRect,kAlignNone,kTransformDisabled,iconSuiteHdl);
	}

	theColor.red=0;
	theColor.green=0;
	theColor.blue=0;

	switch (gStates[theCell.v])
	{
		case kDownloading:
			strcpy(state, "Downloading...");
			theColor.blue=255;
			break;
			
		case kDownloaded:
			strcpy(state, "Downloaded");
			theColor.red=255;
			theColor.blue=255;
			break;
			
		case kUnstuffing:
			strcpy(state, "Unstuffing");
			theColor.red=255;
			theColor.green=255;
			break;
			
		case kUnstuffed:
			strcpy(state, "Unstuffed");
			theColor.green=255;
			thheColor.blue=255;
			break;
			
	case kCompleted:
			strcpy(state, "Installed");
			theColor.green=255;
			break;
		
		default:
			strcpy(state, "");
			theColor.red=255;
			theColor.green=255;
			theColor.blue=255;
			break;
	}

	//State rectangle


	SetRect(&theRect,drawRect.left,drawRect.top+5,drawRect.right -410, drawRect.bottom+5);
	FrameRect(&theRect);
	RGBForeColor(&theColor);
	FillRect(&theRect);

	theColor.red=0;
	theColor.green=0;
	theColor.blue=0;
	RGBForeColor(&theColor);


	//File Name

	CopyCStringToPascal(gFileNames[theCell.v+1], theString);
	SetRect(&drawRect,drawRect.left + 50,drawRect.top,drawRect.right -288, drawRect.bottom);
	TETextBox(&theString[1],theString[0],&drawRect,teCenter);

	//File State
		
	CopyCStringToPascal(state, theString);
	SetRect(&drawRect,drawRect.left - 10,drawRect.top,drawRect.right -138, drawRect.bottom);
	TETextBox(&theString[1],theString[0],&drawRect,teCenter);

	//File Size

	CopyCStringToPascal(gFileSizes[theCell.v+1], theString);
	SetRect(&drawRect,drawRect.left - 10,drawRect.top,drawRect.right, drawRect.bottom);
	TETextBox(&theString[1],theString[0],&drawRect,teCenter);

	SetPort(oldPort);*/
}

#endif

