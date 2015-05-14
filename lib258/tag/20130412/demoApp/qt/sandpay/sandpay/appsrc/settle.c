#include <include.h>
#include <global.h>
#include <xdata.h>
#include <osgraph.h>

extern const SINGLEERRMSG NetErrMsg[];



unsigned char STL_Settle(void)
{
	unsigned char ucResult=SUCCESS;

	
	if(DataSave0.ChangeParamData.ucInilizeFlag!=FLAG_INIT_OK)
		return ERR_NOTINIT;
	if(DataSave0.ChangeParamData.ucIsLogonFlag!=FLAG_LOGON_OK)
		return ERR_NOTLOGON;
	
	NORMALTRANS_ucTransType=TRANS_S_BATCHUP;	

	if(DataSave0.ConstantParamData.ucKFCFlag)
	{
		ucResult=Collect_Send_Manage(1);
		if( ucResult != SUCCESS)
		{
			MSG_DisplaySINGLEERRMSG(NetErrMsg,ucResult);
			ucResult=ERR_CANCEL;
		}
		if(ucResult!=SUCCESS)
			Os__GB2312_xprint((unsigned char *)"Ρݲɼʧ!",0);
		else
			Os__GB2312_xprint((unsigned char *)"Ρݲɼɹ!",0);
			
		if(DataSave0.ConstantParamData.prnflag)
			Os__xlinefeed(5*LINENUM);
		else
		{
			Os__xlinefeed(5*LINENUM);
			Os__xlinefeed(5*LINENUM);
			Os__xlinefeed(5*LINENUM);
		}
		
		if( ucResult != SUCCESS)
			return ucResult;
		ucResult=Collect_Send_Manage(2);
		if( ucResult != SUCCESS)
		{
			MSG_DisplaySINGLEERRMSG(NetErrMsg,ucResult);
			ucResult=ERR_CANCEL;
		}
		if(ucResult!=SUCCESS)
			Os__GB2312_xprint((unsigned char *)"˹ݲɼʧ!",0);
		else
			Os__GB2312_xprint((unsigned char *)"˹ݲɼɹ!",0);
			
		if(DataSave0.ConstantParamData.prnflag)
			Os__xlinefeed(5*LINENUM);
		else
		{
			Os__xlinefeed(5*LINENUM);
			Os__xlinefeed(5*LINENUM);
			Os__xlinefeed(5*LINENUM);
		}
		
		if( ucResult != SUCCESS)
			return ucResult;
	}
#ifdef GUI_PROJECT
    memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
    ProUiFace.ProToUi.uiLines=1;
    memcpy(ProUiFace.ProToUi.aucLine1,"正在处理...",22);
    sleep(1);
#else
	//Os__clr_display(255);
    //Os__GB2312_display(0,0,(uchar *)"正在处理...");
#endif
	if(DataSave0.ConstantParamData.Pinpadflag)
	{
#ifdef GUI_PROJECT
        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
        ProUiFace.ProToUi.uiLines=2;
        memcpy(ProUiFace.ProToUi.aucLine1,"正在处理...",22);
        memcpy(ProUiFace.ProToUi.aucLine2,"请等待",12);
        sleep(1);
#else
		//Os__clr_display_pp(255);
        //Os__GB2312_display_pp(0,0,(uchar *)"正在处理...");
        //Os__GB2312_display_pp(1,0,(uchar *)"请等待");
#endif
	}

	if(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35)
		COMMS_PreComm();

	if(DataSave0.RedoTrans.ucRedoFlag!=0)
		ucResult=SAV_KyTran(1);
	
	if(ucResult==SUCCESS)
		ucResult=STL_SettleProcess();
	
	if(ucResult==SUCCESS)
	{
		if(Commun_flag==0x32&&DataSave0.ConstantParamData.ucConnectType=='2')
		{
			TCP_Close(RunData.iTCPHandle);
			RunData.ucTCPConnectFlag = false;
		}
		NORMALTRANS_ucTransType=TRANS_S_LOGONON;
		DataSave0.ChangeParamData.ucIsLogonFlag=FLAG_LOGON_UNDO;
		ucResult=XDATA_Write_Vaild_File(DataSaveChange);
		if(ucResult!=SUCCESS)
		{
			if(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35)
				COMMS_FinComm();
			return ucResult;
		}

		LOGON_DownApp();

		ucResult=LOGON_LogonOnline();
		if(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35)
			COMMS_FinComm();
		if(ucResult==SUCCESS)
		{
			DataSave0.ChangeParamData.ucIsLogonFlag=FLAG_LOGON_OK;
			ucResult=XDATA_Write_Vaild_File(DataSaveChange);
		}
		if(ucResult==SUCCESS)
		{
#ifdef GUI_PROJECT
			memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
			ProUiFace.ProToUi.uiLines=1;	
            memcpy(ProUiFace.ProToUi.aucLine1,"签到成功!",18);
			sleep(1);
#else
			//Os__clr_display(255);
            //Os__GB2312_display(0,0,(uchar *)"签到成功!");
			UTIL_GetKey(3);
#endif			
		}
		return ucResult;
	}else
    {
        if(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35)
            COMMS_FinComm();
        return ucResult;
	}
}



unsigned char STL_SettleProcess(void)
{
	unsigned char ucResult=SUCCESS;
	int i=0;
	
	DataSave0.ChangeParamData.ucSettleFlag=FLAG_SETTLE_FAIL;
	ucResult=XDATA_Write_Vaild_File(DataSaveChange);

	if(ucResult==SUCCESS)
		ucResult=CFG_SetBatchNumber1();
	
	if(ucResult==SUCCESS)
		ucResult=STL_OnlineProcess();
	
	
	if(ucResult==SUCCESS)
	{
		if(DataSave0.ConstantParamData.prnflagL&&(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35))
			COMMS_FinComm();
		if(DataSave0.TransInfoData.TransTotal.uiZshPurchaseNb+DataSave0.TransInfoData.TransTotal.uiZshOnlinePurchaseNb+DataSave0.TransInfoData.TransTotal.uiJsZshOnlinePurchaseNb==0)
			PRT_PrintSettle();
		else
			PRT_ZSH_SettleResult();
		if(RUNDATA_ucSettleStatus[0]==SETTLE_UNBALANCE
		||RUNDATA_ucSettleStatus[1]==SETTLE_UNBALANCE
		||RUNDATA_ucSettleStatus[2]==SETTLE_UNBALANCE)
			PRT_Terminal();	
		if(DataSave0.ConstantParamData.prnflagL&&(Commun_flag==0x31||Commun_flag==0x33||Commun_flag==0x35))
			COMMS_PreComm();
		ucResult=XDATA_Clear_LastIssueInfo_File();
		if(ucResult!=SUCCESS)
			return ucResult;
			
		while(1)
		{
			memset(&IssueInfo,0,sizeof(ISSUETRANSINFO));
			if(XDATA_Read_IssueInfo_File(i)==SUCCESS)
			{
				if(IssueInfo.ucTranstype==0)
					break;
			}
			else
				break;
			XDATA_Write_LastIssueInfo_File(i);
			i++;
		}
		ucResult=XDATA_Clear_IssueInfo_File();
		if(ucResult!=SUCCESS)
			return ucResult;
		DataSave0.ChangeParamData.ulTraceNumber=1;	
		
		
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.auiTransIndex,
					0,
					sizeof(unsigned short)*TRANS_MAXNB);
		Os__saved_set(DataSave0.TransInfoData.aucTotalIndex,0,50);
		Os__saved_copy((unsigned char *)&DataSave0.TransInfoData.TransTotal,
					(unsigned char *)&DataSave0.TransInfoData.LastTransTotal,
					sizeof(TRANSTOTAL));
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.TransTotal,
					0,sizeof(TRANSTOTAL));
		Os__saved_copy((unsigned char *)&DataSave0.TransInfoData.PtInfoData,
					(unsigned char *)&DataSave0.TransInfoData.LastPtInfoData,
					3*sizeof(TRANSTOTAL));
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.PtInfoData,
					0,3*sizeof(TRANSTOTAL));
					
		memcpy(DataSave0.TransInfoData.TransTotal.aucBatchNumber,
					DataSave0.ChangeParamData.aucBatchNumber,BATCH_LEN);
		
		memset(DataSave0.ChangeParamData.aucBatchNumber,0,BATCH_LEN);
		memcpy(DataSave0.ChangeParamData.aucSettleDateTime,RunData.aucSettleDateTime,7);			
				
		Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag, 
					0,sizeof(unsigned char)*TRANS_MAXNB);
		
		Os__saved_set((unsigned char *)&ExpTransData,0,sizeof(EXPDATA));
		
		Os__saved_set((unsigned char *)&DataSave0.ChangeParamData.ucIsLogonFlag,
					FLAG_LOGON_UNDO,sizeof(unsigned char));
		Os__saved_set((unsigned char *)&DataSave0.ChangeParamData.ucSettleFlag,
					FLAG_SETTLE_UNDO,sizeof(unsigned char));
		Os__saved_set((unsigned char *)&DataSave0.ChangeParamData.ucExpFlag,
					FLAG_LOGON_UNDO,sizeof(unsigned char));
		Os__saved_set((unsigned char *)&DataSave0.ChangeParamData.ucExpFlag1,
					FLAG_LOGON_UNDO,sizeof(unsigned char));
		DataSave0.ChangeParamData.ucIsCashierLogonFlag = FLAG_LOGON_UNDO;

		if(ucResult==SUCCESS)
			ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);
		if(ucResult==SUCCESS)
			ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
		if(ucResult==SUCCESS)
			ucResult=XDATA_Clear_SaveTrans_File();
		if(ucResult==SUCCESS)
			ucResult=XDATA_Write_Vaild_File(DataSaveChange);
		if(ucResult==SUCCESS)
			ucResult=XDATA_Write_Vaild_File(DataSaveExpInfo);
	}		
	if(DataSave0.ChangeParamData.ucSwitchIPFlag==1)
	{
		Trans_SwitchInform();
	}
	return ucResult;
}

unsigned char STL_OnlineProcess(void)
{
	unsigned char ucResult=SUCCESS,aucBuf[255];
	short iLen;
	if(ucResult == SUCCESS)
		ucResult = Trans_ChangeExp();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendBatchTAC();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendOfflineBatch();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendReturn();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendOfflineBatch_P();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendReturn_P();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendPay();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendPayNobar();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendBarCode();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendZSHPurchase();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendSale();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendTransferPurchase();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendExpPurchaseBatch();
	if(ucResult==SUCCESS)	
		ucResult=STL_SendPTCPurchase();
	
	if(ucResult==SUCCESS)	
		ucResult=STL_SendKeyi();	
	if(ucResult==SUCCESS)	
	{
		NORMALTRANS_ucTransType = TRANS_S_SETTLE_ONE;
		ucResult=STL_SettleOnline(1);
	}
	if(ucResult==SUCCESS)	
		ucResult=STL_SettleOnline(2);
	if(ucResult==SUCCESS)	
		ucResult=STL_SettleOnline(3);
	
	return ucResult;
}
unsigned char STL_GetTotalDomain(uchar ucTransType,uchar *ucDm)
{
	int i;
	SAV_TotalDomain(ucTransType);
	for(i=0;i<50;i++)
	{
		if(DataSave0.TransInfoData.aucTotalIndex[i]==ucTransType)
			break;
			
	}
	i++;
	int_bcd(ucDm,1,&i);
	return SUCCESS;
}
unsigned char STL_SendBatchTAC(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[300];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[3];
	unsigned long 	ulAmount;
	
	
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{		
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);	
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"260000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		UTIL_IncreaseTraceNumber(0);
		
		NormalTransData.ulTraceNumber=DataSave0.ChangeParamData.ulTraceNumber;
		//Set bit 22 inputmodel
		ISO8583_SetBitValue(22,(uchar *)"0005",4);
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);

		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		// Set bit 47 cashier id
		//ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		memset(aucBuf,0,sizeof(aucBuf));
		memcpy(aucBuf,DataSave0.ChangeParamData.aucCashierNo,6);
		ISO8583_SetBitValue(47,aucBuf,6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);	
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				memset(&NormalTransData,0,sizeof(NORMALTRANS));
				XDATA_Read_SaveTrans_File(uiIndex);
		 		if((DataSave1.ucBatchFlag[uiIndex]!=0x01)
		 			&&(NormalTransData.ucTransType == TRANS_S_PRELOAD
		 			||NormalTransData.ucTransType == TRANS_S_REFUND
		 			||NormalTransData.ucTransType == TRANS_S_CASHLOAD
		 			||(NormalTransData.ucTransType == TRANS_S_LOADONLINE&&NormalTransData.ucCardLoadFlag==1)
		 			||(NormalTransData.ucTransType == TRANS_S_CHANGEEXP&&NormalTransData.ucPackFlag=='1'))
		 			)
				{
					uiStartIndex=uiIndex;
					auioffset[uiNum]=uiIndex;
					
					aucBuf[offset]=NormalTransData.ucPtCode;
					offset +=1;				
					STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
					offset +=1;				
					//ˮ
					long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);
					offset +=3;
					//
					memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);	offset +=4;
					if(NormalTransData.ucCardType==CARD_M1||NormalTransData.ucCardType==CARD_MEM)
					{
						aucBuf[offset]=0x68;offset+=1;
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x05;
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//ͱ־
						if(NormalTransData.ucTransType == TRANS_S_CHANGEEXP)
							aucBuf[offset]=0x21;
						else
							aucBuf[offset]=0x62;
						offset +=1;				
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);offset +=4;
						
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);
						offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
					}
					else
					{
						aucBuf[offset]=0x74;offset+=1;
						//Կʶ
						aucBuf[offset]=0x00;
						offset +=1;				
						
						//ɢ
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						offset +=8;
						//׺
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAfterEDBalance);	offset +=4;
						//
						memcpy(&aucBuf[offset],&NormalTransData.aucSamTace[2],2);offset +=2;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//ͱ־
						if(NormalTransData.ucTransType == TRANS_S_CHANGEEXP)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x02;
						offset +=1;				
						//ն˱
						asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	offset +=6;
						
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);
						offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
					}
					uiNum++;
					if(uiNum==4)
					{
						break;		
					}
				}
			}

		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
		uiStartIndex++;	
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				uiI++;
			}
			uiI=0;
			ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
			if(ucResult!=SUCCESS)
				return ucResult;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
}


unsigned char STL_SendOfflineBatch(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{

#ifdef GUI_PROJECT
        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
        ProUiFace.ProToUi.uiLines=1;
        memcpy(ProUiFace.ProToUi.aucLine1,"冲正出错",16);
        sleep(1);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
#endif
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"216000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_PURCHASE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						if(NormalTransData.ucBonusflag)
							ulamount=NormalTransData.ulAmount+NormalTransData.ulRateAmount;
						else
							ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount-NormalTransData.ulAmount*bcd_long(NormalTransData.aucDiscount,4)/1000;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}

//xiaobin,2011-05-09,add
unsigned char STL_SendOfflineBatch_M(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"276000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// Set bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//Set bit 62
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_MOBILECHARGE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						if(NormalTransData.ucBonusflag)
							ulamount=NormalTransData.ulAmount+NormalTransData.ulRateAmount;
						else
							ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount-NormalTransData.ulAmount*bcd_long(NormalTransData.aucDiscount,4)/1000;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;						
#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
}


unsigned char STL_SendPTCPurchase(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"AB6000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_PTCPURCHASE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount-NormalTransData.ulAmount*bcd_long(NormalTransData.aucDiscount,4)/1000;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}


unsigned char STL_SendExpPurchaseBatch(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"386000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_EXPPURCHASE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}

unsigned char STL_SendReturn(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"236000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_RETURNCARD&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulAmount*bcd_long(NormalTransData.aucDiscount,4)/1000;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}
unsigned char STL_SendOfflineBatch_P(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"176000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_PURCHASE_P&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//㿨
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucOldCardNo,10);	offset += 10;
						uiNum++;
						
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}


unsigned char STL_SendReturn_P(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"186000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_RETURNCARD_P&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//㿨
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucOldCardNo,10);	offset += 10;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}



unsigned char STL_SendPayNobar(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"886000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_PAYNOBAR&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}

unsigned char STL_SendPay(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"226000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_PAY&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}


unsigned char STL_SendBarCode(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[300];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 	ulAmount;
	int i;
	
	
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{		
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);	
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"290000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		ISO8583_SetBitValue(44,(uchar*)"00007",5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);	
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType == TRANS_S_BARCODE&&NormalTransData.ucSuccessflag=='0')
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&NormalTransData.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						aucBuf[offset]=0;offset+=1;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);
						offset +=3;
						memcpy(&aucBuf[offset],NormalTransData.aucBank,4);
						offset +=4;
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);
						offset +=7;
						aucBuf[offset]=NormalTransData.ucBarLenth;
						offset +=1;
						memcpy(&aucBuf[offset],NormalTransData.aucBarCode,17);
						offset +=17;
						ulAmount=NormalTransData.ulAmount;
						long_bcd(&aucBuf[offset],6,&ulAmount);
						offset +=6;
						ulAmount=NormalTransData.ulAmount+NormalTransData.ulRateAmount;
						long_bcd(&aucBuf[offset],6,&ulAmount);
						offset +=6;
						aucBuf[offset]='0';
						offset +=1;
						uiNum++;
						if(uiNum==5)
						{
							break;		
						}
					}
				}
			}

		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
		uiStartIndex++;	
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
}


unsigned char STL_SendZSHPurchase(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"A06000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_ZSHPURCHASE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
							memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
						else
							asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//˾ʹ
						aucBuf[offset]=NormalTransData.ucDriverCard+'0';
						offset+=1;
						//˾
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucOldCardNo,10);	offset += 10;
						//׺
						if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
							ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
						else
							ulamount=0;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//Set bit 32 Card NO
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,NormalTransData.aucBarCode,20);
		ISO8583_SetBitValue(32,&aucBuf[1],19);
		memset(aucBuf,0,sizeof(aucBuf));
		long_bcd(&aucBuf[16],3,&NormalTransData.ulTicketNumber);
		ISO8583_SetBitValue(57,aucBuf,22);	
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}
unsigned char STL_SendSale(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[300];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[3];
	unsigned long 	ulAmount,uiDiscount;
	
	
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{		
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);	
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"366000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		UTIL_IncreaseTraceNumber(0);
		
		NormalTransData.ulTraceNumber=DataSave0.ChangeParamData.ulTraceNumber;
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
	
		// Set bit 47 cashier id
		//ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		memset(aucBuf,0,sizeof(aucBuf));
		memcpy(aucBuf,DataSave0.ChangeParamData.aucCashierNo,6);
		ISO8583_SetBitValue(47,aucBuf,6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);	
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
		              memset(&NormalTransData,0,sizeof(NORMALTRANS));
	                     XDATA_Read_SaveTrans_File(uiIndex);
		 		if((DataSave1.ucBatchFlag[uiIndex]!=0x01)&&(NormalTransData.ucTransType == TRANS_S_SALE))
				{
					uiStartIndex=uiIndex;
					auioffset[uiNum]=uiIndex;
					
					aucBuf[offset]=NormalTransData.ucPtCode;
					offset +=1;				
					STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
					offset +=1;				
					//
					aucBuf[offset]=0x16;offset+=1;
					memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
					//ǰ
					long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
					//ˮ
					long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);
					offset +=3;					
					//ʱ
					memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);
					offset +=7;
					//ֵˮ
					asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
					//
					memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
					//
					long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
					//cashier
					memcpy(&aucBuf[offset],NormalTransData.aucCashierNo,6);	offset +=6;
					//
					aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
					offset+=1;
					aucBuf[offset]=NormalTransData.ucCardType;
					offset+=1;
					//ۿ
					memcpy(&aucBuf[offset],NormalTransData.aucDiscount,2);
					offset +=2;
					//뷽ʽ
					offset+=1;
					uiNum++;
					if(uiNum==4)
					{
						break;		
					}
				}
			}

		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
		uiStartIndex++;	
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				uiI++;
			}
			uiI=0;
			ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
			if(ucResult!=SUCCESS)
				return ucResult;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
}


unsigned char STL_SendChangeExp(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"856000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_CHANGEEXP&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulPrevEDBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
							
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						ulamount=NormalTransData.ulAfterEDBalance;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//ԭЧ
						memcpy(&aucBuf[offset],NormalTransData.aucExpiredDate,4);	offset +=4;
						//Ч
						memcpy(&aucBuf[offset],NormalTransData.aucNewExpiredDate,4);	offset +=4;
						//ʶ
						aucBuf[offset]=NormalTransData.ucPackFlag;
						offset+=1;
						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}

unsigned char STL_SendKeyi(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"500000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		//ISO8583_SetBitValue(44,COM_VERSION,5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 62
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				//chenzx modify 2005-9-5 10:48
				XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if( NormalTransData.ucKYFlag== 1)
				{
					uiStartIndex=uiIndex;
					//жͣ׿ܲ					
					auioffset[uiNum]=uiIndex;
					//memcpy(&NormalTransData,&DataSave1.SaveTrans[uiIndex],sizeof(SMC_NORMALTRANS));
					aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
					//STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
					offset +=1;				
					//
					aucBuf[offset]=0x16;offset+=1;
					memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
					//ǰ
					long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
					//ˮ
					long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
					//
					Uart_Printf("ֵֻ:%02x\n",NormalTransData.ucTransType);
					switch(NormalTransData.ucTransType)
					{
						case TRANS_S_PURCHASE:
							aucBuf[offset]=0x21;break;
						case TRANS_S_RETURNCARD:
							aucBuf[offset]=0x23;break;
						case TRANS_S_REFUND:
							aucBuf[offset]=0x64;break;
						case TRANS_S_PURCHASE_P:
							aucBuf[offset]=0x17;break;
						case TRANS_S_RETURNCARD_P:
							aucBuf[offset]=0x18;break;
						case TRANS_S_PAY:
							aucBuf[offset]=0x22;break;
						case TRANS_S_PRELOAD:
							aucBuf[offset]=0x62;break;
						case TRANS_S_LOADONLINE:
							aucBuf[offset]=0x67;break;
						case TRANS_S_CASHLOAD:
							aucBuf[offset]=0x59;break;
						case TRANS_S_TRANSPURCHASE:
							aucBuf[offset]=0xC8;break;
						case TRANS_S_EXPPURCHASE:
							aucBuf[offset]=0x38;break;
						case TRANS_S_PAYNOBAR:
							aucBuf[offset]=0x88;break;
						case TRANS_S_ZSHPURCHASE:
							aucBuf[offset]=0xA0;break;
						case TRANS_S_MOBILECHARGE://xiaobin,2011-05-25,add
							aucBuf[offset]=0x27;break;
						default:
							aucBuf[offset]=0x21;break;
						
					}
					offset +=1;
					//ʻ
					aucBuf[offset]=0x60;
					offset +=1;
					if(NormalTransData.ucCardType==CARD_M1)
					{
						memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
						memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
						memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
						aucBuf[offset+7]=0x02;
					}
					else if(NormalTransData.ucCardType==CARD_CPU)
					{
						memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
						memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
					}
					else
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
					offset +=8;
					//
					long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
					//Ӧͱʶ
					if(NormalTransData.ucCardType==CARD_CPU)
						aucBuf[offset]=0x06;
					else
						aucBuf[offset]=0x21;
					offset += 1;
					//ն˱
					if(NormalTransData.ucPtCode==0x01&&NormalTransData.ucNewOrOld==0&&(NormalTransData.aucUserCardNo[0]>>4)==0x09)
						memcpy(&aucBuf[offset],&NormalTransData.aucPsamTerminal[2],4);	
					else
						asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
					offset +=6;
					//
					long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
					offset +=4;
					//ʱ
					memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
					//TAC
					memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
					//ֵˮ
					asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
					//
					memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
					//
					ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
					long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
					//
					memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
					//
					aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
					offset+=1;
					aucBuf[offset]=NormalTransData.ucCardType;
					offset+=1;
					//׺
					if(NormalTransData.ulTrueBalance>NormalTransData.ulAmount)
						ulamount=NormalTransData.ulTrueBalance-NormalTransData.ulAmount;
					else
						ulamount=0;
					long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
					asc_bcd(&aucBuf[offset],3,"111111",6);	    offset+=3;
					uiNum++;					
#ifdef TEST
					Uart_Printf("uiIndex=%d\n",uiIndex);
					Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
					Uart_Printf("uiNum=%d\n",uiNum);
#endif
					if(uiNum==4)
					{
						break;		
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		ucResult=COMMS_TransSendReceive();
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				//chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif
				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{
			return ucResult;
		}
	}
	return SUCCESS;
		
}



unsigned char STL_SendTransferPurchase(void)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[600];
	unsigned short 	uiStartIndex,uiIndex,uiI,uiNum,offset,auioffset[5];
	unsigned long 		ulamount;
	unsigned int cardlen;
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
#ifdef GUI_PROJECT
        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
        ProUiFace.ProToUi.uiLines=1;
        memcpy(ProUiFace.ProToUi.aucLine1,"冲正出错",16);
        sleep(1);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
        UTIL_GetKey(3);
#endif
		return ucResult;
	}
	
	uiStartIndex=0;
	uiI=0;
	while(1)
	{
		ISO8583_Clear();
		// Set Msgid 
		ISO8583_SetMsgID(200);
		// set processcode
		ISO8583_SetBitValue(3,(uchar *)"C86000",6);
		
		memset(aucBuf,0,sizeof(aucBuf));
		
		long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ucResult=UTIL_IncreaseTraceNumber(0);
		if(ucResult!=SUCCESS)
			return ucResult;
		// chenzx add 2005-9-5 10:31
		NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		// end
		
		// Set Card Acceptor Terminal Identification bit 41
		ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
		// Set Card Acceptor Identification Code bit 42 
		ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);
		// Set bit 44
		ISO8583_SetBitValue(44,(uchar*)"00008",5);
		
		memset(aucBuf,0,sizeof(aucBuf));
		//Ա
		ISO8583_SetBitValue(47,(unsigned char *)"111111",6);
		// bit 48 Batchnumber
		memset(aucBuf,0,sizeof(aucBuf));
		bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
		ISO8583_SetBitValue(48,aucBuf,24);
		
		//bit 60
		memset(aucBuf,0,sizeof(aucBuf));
		memset(auioffset,0,sizeof(auioffset));
		offset=1;
		uiNum=0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if(DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				// chenzx modify 2005-9-5 10:48
				ucResult=XDATA_Read_SaveTrans_File(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(NormalTransData.ucTransType==TRANS_S_TRANSPURCHASE&&NormalTransData.ucKYFlag!=1)
				{
					if(DataSave1.ucBatchFlag[uiIndex]!=0x01)
					{
						uiStartIndex=uiIndex;
						//жͣ׿ܲ					
						auioffset[uiNum]=uiIndex;
						aucBuf[offset]=NormalTransData.ucPtCode;offset+=1;
						STL_GetTotalDomain(NormalTransData.ucTransType,&aucBuf[offset]);
						offset +=1;				
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,10);	offset += 10;
						//ǰ
						long_tab(&aucBuf[offset],4,&NormalTransData.ulTrueBalance);	offset +=4;
						//ˮ
						long_bcd(&aucBuf[offset],3,&NormalTransData.ulTraceNumber);		offset +=3;
						//ɢ
						if(NormalTransData.ucCardType==CARD_M1)
						{
							memcpy(&aucBuf[offset],&NormalTransData.aucIssueDate[2],2);
							memcpy(&aucBuf[offset+2],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+6],NormalTransData.aucVerifyCode,1);
							aucBuf[offset+7]=0x02;
						}
						else
						if(NormalTransData.ucCardType==CARD_CPU)
						{
							memcpy(&aucBuf[offset],NormalTransData.aucCardSerial,4);
							memcpy(&aucBuf[offset+4],&NormalTransData.aucUserCardNo[4],4);
						}
						else
							memcpy(&aucBuf[offset],NormalTransData.aucUserCardNo,8);
						offset +=8;
						//
						long_tab(&aucBuf[offset],4,&NormalTransData.ulAmount);	offset +=4;
						//Ӧͱʶ
						if(NormalTransData.ucCardType==CARD_CPU)
							aucBuf[offset]=0x06;
						else
							aucBuf[offset]=0x21;
						offset += 1;
						//ն˱
						asc_bcd(&aucBuf[offset],4,DataSave0.ConstantParamData.aucOnlytermcode,8);	
						offset +=6;
						//
						long_bcd(&aucBuf[offset+1],3,&NormalTransData.ulTraceNumber);
						offset +=4;
						//ʱ
						memcpy(&aucBuf[offset],NormalTransData.aucDateTime,7);	offset +=7;
						//TAC
						memcpy(&aucBuf[offset],NormalTransData.aucTraceTac,4);	offset +=4;
						//ֵˮ
						asc_bcd(&aucBuf[offset],8,NormalTransData.aucLoadTrace,16);	offset +=8;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucZoneCode,3);	offset +=3;
						//
						ulamount=NormalTransData.ulAmount-NormalTransData.ulRateAmount;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						memcpy(&aucBuf[offset],NormalTransData.aucSamTace,4);offset +=4;
						//
						aucBuf[offset]=NormalTransData.ucNewOrOld+1+'0';
						offset+=1;
						aucBuf[offset]=NormalTransData.ucCardType;
						offset+=1;
						//׺
						ulamount=NormalTransData.ulAfterEDBalance;
						long_tab(&aucBuf[offset],4,&ulamount);	offset +=4;
						//
						aucBuf[offset]=0x16;offset+=1;
						memcpy(&aucBuf[offset],NormalTransData.aucOldCardNo,10);	offset += 10;
						//success flag
						if(NormalTransData.ucSuccessflag==0)
							aucBuf[offset]='1';
						else
							aucBuf[offset]=NormalTransData.ucSuccessflag;	
						offset+=1;

						uiNum++;
						
	#ifdef TEST
						Uart_Printf("uiIndex=%d\n",uiIndex);
						Uart_Printf("uiStartIndex=%d\n",uiStartIndex);
						Uart_Printf("uiNum=%d\n",uiNum);
	#endif
						if(uiNum==4)
						{
							break;		
						}
					}
				}
			}
		}
		if(uiNum!=0)
			aucBuf[0]=uiNum;
		else
			return SUCCESS;
			
		uiStartIndex++;			
		ISO8583_SetBitValue(62,aucBuf,offset);
		//˾
		memset(aucBuf,0,sizeof(aucBuf));
		//bcd_asc(aucBuf,DataSave0.ChangeParamData.aucJohnsonCashier,16);
		ISO8583_SetBitValue(32,aucBuf,16);
		//SET BIT 64	MAC
		ISO8583_SetBitValue(64,(uchar *)"000000000",8);
		memcpy(RunData.aucComVersion,(uchar *)"00002",5);
		ucResult = COMMS_TransSendReceive();
		memcpy(RunData.aucComVersion,COM_VERSION,5);
		if(ucResult==SUCCESS)
		{
			for(;;)
			{
				if(uiI==uiNum)
					break;
				// chenzx modify 2005-9-5 11:09
				Os__saved_set((unsigned char *)&DataSave1.ucBatchFlag[auioffset[uiI]],
								0x01,sizeof(unsigned char));
				ucResult=XDATA_Write_Vaild_File(DataSaveBatchFlags);//chenzx add 2005-9-4 15:25
				if(ucResult!=SUCCESS)
					return ucResult;
#ifdef TEST							
				Uart_Printf("%02x ",auioffset[uiI]);
#endif

				uiI++;
			}
			uiI=0;
		}
		if(ucResult!=SUCCESS)
		{

			return ucResult;
		}
	}
	return SUCCESS;
		
}

unsigned char STL_SettleOnline(uchar ucPtcode)
{
	unsigned char 	ucResult=SUCCESS;
	unsigned char 	aucBuf[300];
	unsigned short	uiOffset=0; 
	TRANSTOTAL	*pTransTotal;	
	unsigned int	uiTotalNb,uiNumber;
	unsigned long 	ulTotalAmount,ulAmount;
	short  			iLen;
	int i,j;
	
	ucResult=TRS_CheckReversal();
	if(ucResult!=SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"");
		UTIL_GetKey(3);
		return ucResult;
	}
	
	
	ISO8583_Clear();
	// Set Msgid 
	ISO8583_SetMsgID(800);
	// set processcode
	ISO8583_SetBitValue(3,(uchar *)"930000",6);
	
	memset(aucBuf,0,sizeof(aucBuf));
	long_asc(aucBuf,6,&NORMALTRANS_ulTraceNumber);
	ISO8583_SetBitValue(11,aucBuf,6);
	UTIL_IncreaseTraceNumber(0);
	NormalTransData.ulTraceNumber=DataSave0.ChangeParamData.ulTraceNumber;
	//Set bit 26 
	ISO8583_SetBitHexValue(26,&ucPtcode,1);
	
	// Set Card Acceptor Terminal Identification bit 41
	ISO8583_SetBitValue(41,DataSave0.ConstantParamData.aucTerminalID,8);
	// Set Card Acceptor Identification Code bit 42 
	ISO8583_SetBitValue(42,DataSave0.ConstantParamData.aucMerchantID,15);

	// Set bit 44
	//ISO8583_SetBitValue(44,COM_VERSION,5);
	
	

	// bit 48 Batchnumber
	memset(aucBuf,0,sizeof(aucBuf));
	bcd_asc(aucBuf,DataSave0.ChangeParamData.aucBatchNumber,24);
	ISO8583_SetBitValue(48,aucBuf,24);	
	
	//set bit 60 total
	memset(aucBuf,0,sizeof(aucBuf));
	pTransTotal=&DataSave0.TransInfoData.PtInfoData[ucPtcode-1];
	
	uiOffset=1;
	for(i=0;i<50;i++)
	{
		if(DataSave0.TransInfoData.aucTotalIndex[i]==0)
			break;
		aucBuf[0]++;
		j=i+1;
		int_bcd(&aucBuf[uiOffset],1,&j);
		uiOffset+=1;
		switch(DataSave0.TransInfoData.aucTotalIndex[i])
		{
			case TRANS_S_PURCHASE:
				//ѽ
				aucBuf[uiOffset]=0x01;uiOffset+=1;
				aucBuf[uiOffset]=0x21;uiOffset+=1;
				ulAmount=pTransTotal->ulPurchaseAmount;
				uiNumber=pTransTotal->uiPurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PURCHASE_P:
				//ѽ
				aucBuf[uiOffset]=0x53;uiOffset+=1;
				aucBuf[uiOffset]=0x17;uiOffset+=1;
				ulAmount=pTransTotal->ulPurchasepAmount;
				uiNumber=pTransTotal->uiPurchasepNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PRELOAD:
				//ֵ
				aucBuf[uiOffset]=0x24;uiOffset+=1;
				aucBuf[uiOffset]=0x62;uiOffset+=1;
				ulAmount=pTransTotal->ulLoadAmount;
				uiNumber=pTransTotal->uiLoadNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_LOADONLINE:
				//ֵ
				aucBuf[uiOffset]=0x05;uiOffset+=1;
				aucBuf[uiOffset]=0x67;uiOffset+=1;
				ulAmount=pTransTotal->ulLoadOnlineAmount;
				uiNumber=pTransTotal->uiLoadOnlineNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_ONLINEPURCHASE:
				//ѽ
				aucBuf[uiOffset]=0x21;uiOffset+=1;
				aucBuf[uiOffset]=0x70;uiOffset+=1;
				ulAmount=pTransTotal->ulOnlinePurchaseAmount;
				uiNumber=pTransTotal->uiOnlinePurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_ONLINEPURCHASE_P:
				//ѽ
				aucBuf[uiOffset]=0x54;uiOffset+=1;
				aucBuf[uiOffset]=0x70;uiOffset+=1;
				ulAmount=pTransTotal->ulOnlinePurchasepAmount;
				uiNumber=pTransTotal->uiOnlinePurchasepNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_RETURNCARD:
				//˿
				aucBuf[uiOffset]=0x01;uiOffset+=1;
				aucBuf[uiOffset]=0x23;uiOffset+=1;
				ulAmount=pTransTotal->ulReturnAmount;
				uiNumber=pTransTotal->uiReturnNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_RETURNCARD_P:
				//˿
				aucBuf[uiOffset]=0x53;uiOffset+=1;
				aucBuf[uiOffset]=0x18;uiOffset+=1;
				ulAmount=pTransTotal->ulReturnpAmount;
				uiNumber=pTransTotal->uiReturnpNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PAY:
				//ɷѽ
				aucBuf[uiOffset]=0x02;uiOffset+=1;
				aucBuf[uiOffset]=0x22;uiOffset+=1;
				ulAmount=pTransTotal->ulPayAmount;
				uiNumber=pTransTotal->uiPayNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PAYNOBAR:
				//ɷѽ
				aucBuf[uiOffset]=0x14;uiOffset+=1;
				aucBuf[uiOffset]=0x88;uiOffset+=1;
				ulAmount=pTransTotal->ulPayNobarAmount;
				uiNumber=pTransTotal->uiPayNobarNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PAYONLINE:
				//ɷѽ
				aucBuf[uiOffset]=0x22;uiOffset+=1;
				aucBuf[uiOffset]=0x72;uiOffset+=1;
				ulAmount=pTransTotal->ulOnlinePayAmount;
				uiNumber=pTransTotal->uiOnlinePayNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_MOBILECHARGE:
				aucBuf[uiOffset]=0x27;uiOffset+=1;
				aucBuf[uiOffset]=0x27;uiOffset+=1;
				ulAmount=pTransTotal->ulMobileAmount;
				uiNumber=pTransTotal->uiMobileNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_MOBILECHARGEONLINE:
				aucBuf[uiOffset]=0x27;uiOffset+=1;
				aucBuf[uiOffset]=0x76;uiOffset+=1;
				ulAmount=pTransTotal->ulOnlineMobileAmount;
				uiNumber=pTransTotal->uiOnlineMobileNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PREAUTH:
				aucBuf[uiOffset]=0x48;uiOffset+=1;
				aucBuf[uiOffset]=0x32;uiOffset+=1;
				ulAmount=pTransTotal->ulPreAuthAmount;
				uiNumber=pTransTotal->uiPreAuthNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PREAUTHVOID:
				aucBuf[uiOffset]=0x48;uiOffset+=1;
				aucBuf[uiOffset]=0x33;uiOffset+=1;
				ulAmount=pTransTotal->ulPreAuthVoidAmount;
				uiNumber=pTransTotal->uiPreAuthVoidNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PREAUTHFINISH:
				aucBuf[uiOffset]=0x48;uiOffset+=1;
				aucBuf[uiOffset]=0x34;uiOffset+=1;
				ulAmount=pTransTotal->ulPreAuthFinishAmount;
				uiNumber=pTransTotal->uiPreAuthFinishNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_ZSHPURCHASE:
				aucBuf[uiOffset]=0x49;uiOffset+=1;
				aucBuf[uiOffset]=0xA0;uiOffset+=1;
				ulAmount=pTransTotal->ulZshPurchaseAmount;
				uiNumber=pTransTotal->uiZshPurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_ZSHONLINEPURCHASE:
				aucBuf[uiOffset]=0x49;uiOffset+=1;
				aucBuf[uiOffset]=0xA1;uiOffset+=1;
				ulAmount=pTransTotal->ulZshOnlinePurchaseAmount;
				uiNumber=pTransTotal->uiZshOnlinePurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_JSZSHONLINEPURCHASE:
				aucBuf[uiOffset]=0x56;uiOffset+=1;
				aucBuf[uiOffset]=0xA2;uiOffset+=1;
				ulAmount=pTransTotal->ulJsZshOnlinePurchaseAmount;
				uiNumber=pTransTotal->uiJsZshOnlinePurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_SALE:
				//ۿ
				aucBuf[uiOffset]=0x04;uiOffset+=1;
				aucBuf[uiOffset]=0x28;uiOffset+=1;
				ulAmount=pTransTotal->ulSaleAmount;
				uiNumber=pTransTotal->uiSaleNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_REFUND:
				//˻
				aucBuf[uiOffset]=0x28;uiOffset+=1;
				aucBuf[uiOffset]=0x64;uiOffset+=1;
				ulAmount=pTransTotal->ulRefundAmount;
				uiNumber=pTransTotal->uiRefundNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_CHANGECD:
				aucBuf[uiOffset]=0x44;uiOffset+=1;
				aucBuf[uiOffset]=0x83;uiOffset+=1;
				ulAmount=pTransTotal->ulChangeCardAmount;
				uiNumber=pTransTotal->uiChangeCardNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_CHANGEEXP:
				aucBuf[uiOffset]=0x45;uiOffset+=1;
				aucBuf[uiOffset]=0x84;uiOffset+=1;
				ulAmount=pTransTotal->ulChangeExpAmount;
				uiNumber=pTransTotal->uiChangeExpNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_TRANSPURCHASE:
				aucBuf[uiOffset]=0x29;uiOffset+=1;
				aucBuf[uiOffset]=0xC8;uiOffset+=1;
				ulAmount=pTransTotal->ulTransferPurchaseAmount;
				uiNumber=pTransTotal->uiTransferPurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_TRANSFER:
				aucBuf[uiOffset]=0x29;uiOffset+=1;
				aucBuf[uiOffset]=0x65;uiOffset+=1;
				ulAmount=pTransTotal->ulTransferAmount;
				uiNumber=pTransTotal->uiTransferNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_EXPPURCHASE:
				aucBuf[uiOffset]=0x47;uiOffset+=1;
				aucBuf[uiOffset]=0x38;uiOffset+=1;
				ulAmount=pTransTotal->ulExpPurchaseAmount;
				uiNumber=pTransTotal->uiExpPurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_REFAPL:
				aucBuf[uiOffset]=0x30;uiOffset+=1;
				aucBuf[uiOffset]=0x45;uiOffset+=1;
				ulAmount=pTransTotal->ulRefAplAmount;
				uiNumber=pTransTotal->uiRefAplNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_SALEONLINE:
				aucBuf[uiOffset]=0x11;uiOffset+=1;
				aucBuf[uiOffset]=0x24;uiOffset+=1;
				ulAmount=pTransTotal->ulSaleOnlineAmount;
				uiNumber=pTransTotal->uiSaleOnlineNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_CASHLOAD:
				//ֵ
				aucBuf[uiOffset]=0x31;uiOffset+=1;
				aucBuf[uiOffset]=0x59;uiOffset+=1;
				ulAmount=pTransTotal->ulCashLoadAmount;
				uiNumber=pTransTotal->uiCashLoadNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_BATCHLOAD:
				aucBuf[uiOffset]=0x10;uiOffset+=1;
				aucBuf[uiOffset]=0xA3;uiOffset+=1;
				ulAmount=pTransTotal->ulBatchLoadAmount;
				uiNumber=pTransTotal->uiBatchLoadNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_ONLINEREFUND:
				aucBuf[uiOffset]=0x12;uiOffset+=1;
				aucBuf[uiOffset]=0xA4;uiOffset+=1;
				ulAmount=pTransTotal->ulOnlineRefundAmount;
				uiNumber=pTransTotal->uiOnlineRefundNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			case TRANS_S_PTCPURCHASE:
				aucBuf[uiOffset]=0x09;uiOffset+=1;
				aucBuf[uiOffset]=0xAB;uiOffset+=1;
				ulAmount=pTransTotal->ulPTCPurchaseAmount;
				uiNumber=pTransTotal->uiPTCPurchaseNb;
				long_bcd(&aucBuf[uiOffset],6,&ulAmount);	uiOffset +=6;
				int_bcd(&aucBuf[uiOffset],2,&uiNumber);	uiOffset +=2;
				break;
			default:
				//uiOffset+=10;
				uiOffset-=1;
				break;
		}
	}
	ISO8583_SetBitValue(62,aucBuf,uiOffset);
	
	ucResult=COMMS_TransSendReceive();
	if(ucResult==SUCCESS)
	{
		memset(aucBuf,0,sizeof(aucBuf));
		ucResult=ISO8583_GetBitValue(62,aucBuf,&iLen,sizeof(aucBuf));
		if(ucResult!=SUCCESS)
			return ucResult;

		else if(memcmp(aucBuf,"SSSSSSSSSSSSSSSSSSSS",20))
			RUNDATA_ucSettleStatus[ucPtcode-1]=SETTLE_UNBALANCE;
		else
			RUNDATA_ucSettleStatus[ucPtcode-1]=0;
	}

	return ucResult;
}
unsigned char STL_ByCom(void)
{
	unsigned char ucInput,ucResult=SUCCESS;
	if(DataSave0.ChangeParamData.ucInilizeFlag!=FLAG_INIT_OK)
		return ERR_NOTINIT;
	if(DataSave0.ChangeParamData.ucIsLogonFlag!=FLAG_LOGON_OK)
		return ERR_NOTLOGON;
	
	NORMALTRANS_ucTransType=TRANS_S_BATCHUP;	
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"ѡ񴮿");
	//Os__GB2312_display(1,0,(uchar *)"1.1");
	//Os__GB2312_display(2,0,(uchar *)"2.2");
	ucInput=Os__xget_key();
	if(ucInput==KEY_CLEAR)
	{
		return ERR_CANCEL;
	}
	if(ucInput=='2')
		RunData.ucComType=2;
	else
		RunData.ucComType=1;

	ucResult=STL_SettleProcess();
		
	return ucResult;
}
