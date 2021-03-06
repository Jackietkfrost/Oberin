// ----------------------------------------------------------------- //
void nw_SendRefresh(int i, TBoolean forceRefresh) // server
// ----------------------------------------------------------------- //

{
	// Sent from the server to refresh the display of a single player
	//
	// forceRefresh - send data even if there was no change from previous frame.  This if for when
	// the player moves with no objects or creatures in view.

	int										j;
	int										k;
	int										row;
	int										col;
	OSStatus 							status;
	int										dataPtr;
	TBoolean							sendData;
	int										hits;
	float									fhits;
	float									fmhits;
	unsigned long					currentTime=TickCount();
	int										raiseChance;

	if (targetFrame==0)
		targetFrame=1;
	else
		targetFrame=0;

	if (player[i].gathering>0)	// check status of player resource harvest
		begin
		player[i].gathering--;

	if (player[i].gathering==0)
		begin
		if (player[i].gatherType==kLumberjacking)
			sk_Lumberjacking(i);
		else if (player[i].gatherType==kFishing)
			sk_Fishing(i);

	end
	end

	// ------- raise magic points
	if ((creature[player[i].creatureIndex].magicPoints<creature[player[i].creatureIndex].intelligence) && (tb_Rnd(1,4)==1))
		begin
		if (player[i].skill[kMeditation].level<10) 		  raiseChance=8;
		else if (player[i].skill[kMeditation].level<20) raiseChance=7;
		else if (player[i].skill[kMeditation].level<35) raiseChance=6;
		else if (player[i].skill[kMeditation].level<50) raiseChance=5;
		else if (player[i].skill[kMeditation].level<65) raiseChance=4;
		else if (player[i].skill[kMeditation].level<80) raiseChance=3;
		else if (player[i].skill[kMeditation].level<95) raiseChance=2;
		else raiseChance=1;

	if (tb_Rnd(1,raiseChance)==1)
		begin
		creature[player[i].creatureIndex].magicPoints++;

	if (player[i].meditating)
		creature[player[i].creatureIndex].magicPoints++;

	if (creature[player[i].creatureIndex].magicPoints>creature[player[i].creatureIndex].intelligence)
		creature[player[i].creatureIndex].magicPoints=creature[player[i].creatureIndex].intelligence;

	nw_SendStatsRefresh(i);

	end
	end

	// ------- raise health points
	if ((creature[player[i].creatureIndex].hitPoints<creature[player[i].creatureIndex].strength) && (tb_Rnd(1,4)==1))
		begin
		if (player[i].skill[kHealing].level<10) 		 raiseChance=8;
		else if (player[i].skill[kHealing].level<20) raiseChance=7;
		else if (player[i].skill[kHealing].level<35) raiseChance=6;
		else if (player[i].skill[kHealing].level<50) raiseChance=5;
		else if (player[i].skill[kHealing].level<65) raiseChance=4;
		else if (player[i].skill[kHealing].level<80) raiseChance=3;
		else if (player[i].skill[kHealing].level<95) raiseChance=2;
		else raiseChance=1;

	if (tb_Rnd(1,raiseChance)==1)
		begin
		creature[player[i].creatureIndex].hitPoints++;

	if (player[i].healing)
		creature[player[i].creatureIndex].hitPoints++;

	if (creature[player[i].creatureIndex].hitPoints>creature[player[i].creatureIndex].strength)
		creature[player[i].creatureIndex].hitPoints=creature[player[i].creatureIndex].strength;

	nw_SendStatsRefresh(i);

	end
	end
	if (creature[player[i].creatureIndex].hitPoints>=creature[player[i].creatureIndex].strength)
		player[i].healing=false;

	if (gNetGame)
		if ((player[i].active) && (!player[i].online))	// ------ player logged off ... check for remove time
			begin
			if ((player[i].lastCheckIn+3600)<currentTime)
				begin
				fi_Logout(i);

	creatureMap[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col]=0;

	cr_ClearCreatureRecord(player[i].creatureIndex);

	pl_ClearPlayerRecord(i);

	end
	end
	else if ((player[i].lastCheckIn+(3600*kInactivelyLogoff))<currentTime)	// ------ check for player inactivity
		begin
		nw_RemovePlayer(player[i].id);

	fi_Logout(i);

	creatureMap[creature[player[i].creatureIndex].row][creature[player[i].creatureIndex].col]=0;

	cr_ClearCreatureRecord(player[i].creatureIndex);

	pl_ClearPlayerRecord(i);

	end
	else if (player[i].active)	// ------ everything ok ... send refresh
		begin

		if (player[i].hunger>0)	// ------ decrement hunger counter
			player[i].hunger--;

	if (currentTime>(player[i].talkTime+(60*10)))	// ------ check for talk erase time
		begin
		strcpy(player[i].talk, "");

	player[i].talkTime=0;

	end

	sendData=false;

	dataPtr=0;

	for (j=0;j<180;j++)	// build creature list
		begin
		row=mapToScreenRow[j]+creature[player[i].creatureIndex].row-13;

	col=mapToScreenCol[j]+creature[player[i].creatureIndex].col-10;

	if ((creatureMap[row][col]!=0) && (creature[creatureMap[row][col]].hiding==false))
		begin
		outData[dataPtr]				=	j;

	if (playerData[i][dataPtr]!=j) sendData=true;

	playerData[i][dataPtr]	=	j;

	dataPtr++;

	outData[dataPtr]				=	creature[creatureMap[row][col]].id; // creatureMap[row][col];

	if (playerData[i][dataPtr]!=creature[creatureMap[row][col]].id) sendData=true;

	playerData[i][dataPtr]	=	creature[creatureMap[row][col]].id;

	dataPtr++;

	outData[dataPtr]				=	creature[creatureMap[row][col]].facing; // creatureMap[row][col];

	if (playerData[i][dataPtr]!=creature[creatureMap[row][col]].facing) sendData=true;

	playerData[i][dataPtr]	=	creature[creatureMap[row][col]].facing;

	dataPtr++;

	if (creature[creatureMap[row][col]].id==kHuman)
		begin

		// ------- check for chat text
		if ((player[creature[creatureMap[row][col]].playerIndex].talkTime>=player[i].lastRefresh) && (player[i].lastRefresh!=0))
			nw_SendTextToClient(i, creature[creatureMap[row][col]].playerIndex, j);

	// ----------------------------

	if (player[creature[creatureMap[row][col]].playerIndex].arm[kRightHand]==0)
		outData[dataPtr]=0;
	else
		outData[dataPtr]				=	creature[creatureMap[row][col]].inventory[player[creature[creatureMap[row][col]].playerIndex].arm[kRightHand]].itemID;

	if (playerData[i][dataPtr]!=outData[dataPtr]) sendData=true;

	playerData[i][dataPtr]	=	outData[dataPtr];

	dataPtr++;

	if (player[creature[creatureMap[row][col]].playerIndex].arm[kLegs]==0)
		outData[dataPtr]=0;
	else
		outData[dataPtr]				=	creature[creatureMap[row][col]].inventory[player[creature[creatureMap[row][col]].playerIndex].arm[kLegs]].itemID;

	if (playerData[i][dataPtr]!=outData[dataPtr]) sendData=true;

	playerData[i][dataPtr]	=	outData[dataPtr];

	dataPtr++;

	if (player[creature[creatureMap[row][col]].playerIndex].arm[kTorso]==0)
		outData[dataPtr]=0;
	else
		outData[dataPtr]				=	creature[creatureMap[row][col]].inventory[player[creature[creatureMap[row][col]].playerIndex].arm[kTorso]].itemID;

	if (playerData[i][dataPtr]!=outData[dataPtr]) sendData=true;

	playerData[i][dataPtr]	=	outData[dataPtr];

	dataPtr++;

	end


	if ((creature[creatureMap[row][col]].timeStamp>=player[i].lastRefresh) && (player[i].lastRefresh!=0))
		begin // creature moved since last player refresh
		if (j==80)
			outData[dataPtr]=4;
		else
			outData[dataPtr]	=	creature[creatureMap[row][col]].moveDirection;

	fhits=creature[creatureMap[row][col]].hitPoints;

	fmhits=creatureInfo[creature[creatureMap[row][col]].id].maxHitPoints;

	hits=(((fhits/fmhits)*100)/7)+1;

	if (hits>15) hits=15;

	if (hits>7) 	{ hits=hits-8; outData[dataPtr]=outData[dataPtr]+128; 	}

	if (hits>3) 	{ hits=hits-4; outData[dataPtr]=outData[dataPtr]+64; 		}

	if (hits>1) 	{ hits=hits-2; outData[dataPtr]=outData[dataPtr]+32; 		}

	if (hits>0) 	{ hits=hits-1; outData[dataPtr]=outData[dataPtr]+16; 		}

	if (playerData[i][dataPtr]!=outData[dataPtr]) sendData=true;

	playerData[i][dataPtr]	=	outData[dataPtr];

	dataPtr++;

	end
	else
		begin
		outData[dataPtr]=4;

	fhits=creature[creatureMap[row][col]].hitPoints;

	fmhits=creatureInfo[creature[creatureMap[row][col]].id].maxHitPoints;

	hits=(((fhits/fmhits)*100)/7)+1;

	if (hits>15) hits=15;

	if (hits>7) 	{ hits=hits-8; outData[dataPtr]=outData[dataPtr]+128; 	}

	if (hits>3) 	{ hits=hits-4; outData[dataPtr]=outData[dataPtr]+64; 		}

	if (hits>1) 	{ hits=hits-2; outData[dataPtr]=outData[dataPtr]+32; 		}

	if (hits>0) 	{ hits=hits-1; outData[dataPtr]=outData[dataPtr]+16; 		}

	if (playerData[i][dataPtr]!=outData[dataPtr]) sendData=true;

	playerData[i][dataPtr]	=	outData[dataPtr];

	dataPtr++;

	end

	end
	playerState[i][targetFrame][j]=creatureMap[row][col];

	end

	outData[dataPtr]=255;

	if (playerData[i][dataPtr]!=255) sendData=true;

	playerData[i][dataPtr]=255;

	dataPtr++;

	for (j=0;j<180;j++)	// build container list
		begin
		row=mapToScreenRow[j]+creature[player[i].creatureIndex].row-13;

	col=mapToScreenCol[j]+creature[player[i].creatureIndex].col-10;

	if (containerMap[row][col]!=0)
		begin
		outData[dataPtr]	=	j;

	if (playerData[i][dataPtr]!=j) sendData=true;

	playerData[i][dataPtr]	= j;

	dataPtr++;

	outData[dataPtr]	=	1; // chest;

	if (playerData[i][dataPtr]!=1) sendData=true;

	playerData[i][dataPtr]	=	1;

	dataPtr++;

	end
	end

	player[i].lastRefresh=TickCount();

	outData[dataPtr]=255;

	if (playerData[i][dataPtr]!=255) sendData=true;

	playerData[i][dataPtr]	=	255;

	if (dataPtr>maxDataSize)
		maxDataSize=dataPtr;

	if ((sendData) || (forceRefresh))	// send the data
		begin

		if (dataPtr<20)
			begin
			// if need <= 20 bytes -------------
			NSpClearMessageHeader(&gPlayerRefreshMessage3.h);

	gPlayerRefreshMessage3.h.what 				= kPlayerRefreshMessage3;

	gPlayerRefreshMessage3.h.to 					= player[i].id;

	for (j=0;j<=dataPtr;j++)
		gPlayerRefreshMessage3.data[j]=outData[j];

	gPlayerRefreshMessage3.row						=	creature[player[i].creatureIndex].row-13;

	gPlayerRefreshMessage3.col						=	creature[player[i].creatureIndex].col-10;

	gPlayerRefreshMessage3.h.messageLen 	= sizeof(gPlayerRefreshMessage3);

	status = NSpMessage_Send(gNetGame, &gPlayerRefreshMessage3.h, kNSpSendFlag_Normal); //kNSpSendFlag_Junk);

	// ---------------------------------
	end
	else if (dataPtr<50)
		begin
		// if need <= 50 bytes -------------
		NSpClearMessageHeader(&gPlayerRefreshMessage2.h);

	gPlayerRefreshMessage2.h.what 				= kPlayerRefreshMessage2;

	gPlayerRefreshMessage2.h.to 					= player[i].id;

	for (j=0;j<=dataPtr;j++)
		gPlayerRefreshMessage2.data[j]=outData[j];

	gPlayerRefreshMessage2.row						=	creature[player[i].creatureIndex].row-13;

	gPlayerRefreshMessage2.col						=	creature[player[i].creatureIndex].col-10;

	gPlayerRefreshMessage2.h.messageLen 	= sizeof(gPlayerRefreshMessage2);

	status = NSpMessage_Send(gNetGame, &gPlayerRefreshMessage2.h, kNSpSendFlag_Normal); //kNSpSendFlag_Junk);

	// ---------------------------------
	end
	else
		begin
		// if need 2048 bytes -------------
		NSpClearMessageHeader(&gPlayerRefreshMessage1.h);

	gPlayerRefreshMessage1.h.what 				= kPlayerRefreshMessage1;

	gPlayerRefreshMessage1.h.to 					= player[i].id;

	for (j=0;j<=dataPtr;j++)
		gPlayerRefreshMessage1.data[j]=outData[j];

	gPlayerRefreshMessage1.row						=	creature[player[i].creatureIndex].row-13;

	gPlayerRefreshMessage1.col						=	creature[player[i].creatureIndex].col-10;

	gPlayerRefreshMessage1.h.messageLen 	= sizeof(gPlayerRefreshMessage1);

	status = NSpMessage_Send(gNetGame, &gPlayerRefreshMessage1.h, kNSpSendFlag_Normal); //kNSpSendFlag_Junk);

	// ---------------------------------
	end
	end

	end

}
