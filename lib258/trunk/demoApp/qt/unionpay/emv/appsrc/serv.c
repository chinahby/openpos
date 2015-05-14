/*
	SAND China
--------------------------------------------------------------------------
	FILE  serv.c								(Copyright SAND 2001)
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2001-07-02		Xiaoxi Jiang
    Last modified :	2001-07-02		Xiaoxi Jiang
    Module :  Shanghai Golden Card Network
    Purpose :
        Source file for sevice menu process.

    List of routines in file :

    File history :

*/

#include <include.h>
#include <global.h>
#include <xdata.h>
#include <toolslib.h>

unsigned char ucSettleBatchSendFlag;

unsigned char SERV_Settle(unsigned char ucFlag)
{
	unsigned char ucResult;
	TRANSTOTAL	*pTransTotal;
	TRANSTOTAL	*pForeignTransTotal;


	ucResult = SUCCESS;
	/*�л���IP1*/
	DataSave0.ConstantParamData.ucSelIP = 1;
	XDATA_Write_Vaild_File(DataSaveConstant);

    if(DataSave0.ChangeParamData.ucCashierLogonFlag!=CASH_LOGONFLAG)
	{
		MSG_DisplayErrMsg(ERR_CASH_NOTLOGON);
		return(SUCCESS);
	}
	if(DataSave0.ConstantParamData.ucLogonModiBatch)
	{
#ifdef GUI_PROJECT	
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=1;	
	memcpy(ProUiFace.ProToUi.aucLine1,(uchar *)"���β�����ǩ��", strlen((char *)"���β�����ǩ��") );
	    sem_post(&binSem1);
    sem_wait(&binSem2);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"���β�����ǩ��");
		MSG_WaitKey(3);
#endif		
		return(SUCCESS);
	}
	if(DataSave0.ChangeParamData.ucStoreKeyRight)
	{
#ifdef GUI_PROJECT	
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=1;	
	memcpy(ProUiFace.ProToUi.aucLine1,(uchar *)"��Կ������ǩ��", strlen((char *)"��Կ������ǩ��") );
	    sem_post(&binSem1);
    sem_wait(&binSem2);
#else
	    	//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"��Կ������ǩ��");
		MSG_WaitKey(3);
#endif		
		return(SUCCESS);
	}

	if ((ucResult == SUCCESS) && (ucFlag != true)&&(DataSave0.ConstantParamData.ucEmptySettle !=0x31))
	{
		G_NORMALTRANS_ucTransType=TRANS_SETTLE;
		ucResult = UTIL_Is_Trans_Empty();
	}

	if(ucResult == SUCCESS)
	{
		if( DataSave0.ChangeParamData.ucDownLoadFlag==1 )
		{
			G_NORMALTRANS_ucTransType=TRANS_LOADPARAM;
			ucResult=LOGON_Online();
			if( ucResult==SUCCESS )
			{
				DataSave0.ChangeParamData.ucDownLoadFlag=0;
				XDATA_Write_Vaild_File(DataSaveChange);
			}
		}

		pTransTotal = &DataSave0.TransInfoData.TransTotal;
		pForeignTransTotal = &DataSave0.TransInfoData.ForeignTransTotal;

		G_NORMALTRANS_ucTransType=TRANS_SETTLE;

		
#ifndef GUI_PROJECT
		if((!ucFlag)
			&&(DataSave0.ConstantParamData.ucDisplay == 0x31)
			&&(DataSave0.ConstantParamData.ucDisAutoSettleFlag == 0x30)
			&&(DataSave0.ConstantParamData.ucDisAutoTiming == 0x30)
		    )
		{
			MSG_DisplayMsg(true,1,3,MSG_SETTLE);
			MSG_DisplayMsg(false,2,4,MSG_CONFIRM);
			ucResult = SUCCESS;

			//if(Os__xget_key() != KEY_ENTER)
			{
				//ucResult = ERR_CANCEL;
			}
    sem_post(&binSem1);
    sem_wait(&binSem2);			
		}
#endif		
		if( ucResult==SUCCESS)
		{
			COMMS_PreComm();
			ucResult = SERV_SettleProcess();
			COMMS_FinComm();
		}
	}


	if( (ucResult != SUCCESS)&&(ucResult != ERR_CANCEL) )
	{
		MSG_DisplayErrMsg(ucResult);
		ucResult = ERR_CANCEL;
	}

	return(ucResult);
}


unsigned char SERV_UpLoadComm(void)
{
    unsigned char aucBackBuf[20];
	unsigned short uiLen;
	unsigned char ucResult;
	short    usLen;

	util_Printf("\n���ݲɼ����++++++\n");
    ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
					(unsigned char *)&DataSave0.Trans_8583Data.SendISO8583Data);
    XDATA_Write_Vaild_File(DataSaveTrans8583);


    ucResult = ISO8583_PackData(ISO8583Data.aucCommBuf,&ISO8583Data.uiCommBufLen,
				    ISO8583_MAXCOMMBUFLEN);

    ISO8583_DumpData();

    ucResult = COMMS_SendCollectDataTCP(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
				ISO8583Data.aucCommBuf,&uiLen);

	if( ucResult != SUCCESS)
	{
		util_Printf("�ɼ��������緵���룺%02x\n",ucResult);
		MSG_DisplayNetworkErrMsg(ucResult);
		return(ERR_END);
	}
	ISO8583Data.uiCommBufLen = uiLen;
	ISO8583_UnpackData(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen);
   util_Printf("\n++++���������+++\n");
   ISO8583_DumpData();

    memset(aucBackBuf,0,sizeof(aucBackBuf));
    if(ucResult == SUCCESS)
    {
    	ucResult = ISO8583_GetBitValue(39,aucBackBuf,&usLen,sizeof(aucBackBuf));

		if(ucResult == SUCCESS)
		{
	        util_Printf("\naucBackBuf = %s,usLen = %d\n",aucBackBuf,usLen);

	        G_RUNDATA_ucErrorExtCode = (unsigned short)asc_long(aucBackBuf,4);

            util_Printf("aucBackBuf = %s\n",aucBackBuf);
	        if(memcmp(aucBackBuf,"0000",4))
	        {
	            return(ERR_HOSTCODE);
	        }
        }
    }
    return ucResult;
}

unsigned char SERV_ProcessData(void)
{
    unsigned char ucResult = SUCCESS;
    unsigned char aucBuf[1024];
    unsigned int ucOffset;
    unsigned int  uiNum,uit,uiI;
    unsigned short  uiIndex,uiTotalNb;
    unsigned char ucContinueFlag;
	unsigned char ucf;
	ucf=0;
	uiTotalNb = 0;

    memset(aucBuf,0,sizeof(aucBuf));
    ucContinueFlag = true;

	do
	{
		util_Printf("\n+++++��ʼ�������+++++\n");
		ISO8583_Clear();
		ISO8583_SetMsgID(200);

		memset(aucBuf,0,sizeof(aucBuf));
		switch(G_NORMALTRANS_ucTransType)
		{
			case TRANS_SENDCOLLECTDATA:
				aucBuf[0] = 0x24;
				aucBuf[1] = 0x60;
				break;
			default:
				return ERR_COMMS_SENDCHAR;
		}
		ISO8583_SetBitHexValue(3,aucBuf,3);

		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
		ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
		ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);

		memset(aucBuf,0,sizeof(aucBuf));
		uiNum = 0;
		ucOffset = 2;
		memset(aucBuf,0,sizeof(aucBuf));
		util_Printf("��������=[%3d]�ʽ���\n",DataSave0.ChangeParamData.uiSendTotalNb);

        for(uiIndex = uiTotalNb;uiIndex<DataSave0.ChangeParamData.uiSendTotalNb;uiIndex++)
		{
			util_Printf("uiIndex = %d\n",uiIndex);
			XDATA_Read_CollectTrans_File(uiIndex);

            //acclenth
            util_Printf("DataSave0.SaveTrans1.ucSourceAccLen = %02x\n",DataSave0.SaveTrans1.ucSourceAccLen);
            uit=DataSave0.SaveTrans1.ucSourceAccLen;
            int_asc(&aucBuf[ucOffset],2,&uit);
            ucOffset += 2;

            //accSourceAcc
            bcd_asc(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucSourceAcc,TRANS_ACCLEN*2);
            ucOffset += TRANS_ACCLEN*2;

			memcpy(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucComm,4);
            ucOffset += 4;

            long_asc(&aucBuf[ucOffset],12,&DataSave0.SaveTrans1.ulAmount);
            ucOffset += 12;

            memcpy(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucOrdersNo,TRANS_ORDERSNO);
            ucOffset += TRANS_ORDERSNO;

            long_asc(&aucBuf[ucOffset],6,&DataSave0.SaveTrans1.ulTraceNumber);
            ucOffset += 6;

            bcd_asc(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucDate,8);
            ucOffset += 8;

            bcd_asc(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucTime,6);
            ucOffset += 6;

            memcpy(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucRefNum,12);
            ucOffset += TRANS_REFNUMLEN;

            memcpy(&aucBuf[ucOffset],DataSave0.SaveTrans1.aucTargetAcc,6);
            ucOffset += 6;

            bcd_asc(&aucBuf[ucOffset],&DataSave0.SaveTrans1.ucTransType,2);
            ucOffset += 2;

			uiTotalNb ++;
			uiNum ++;
			util_Printf("\n==60������:\n");
			for(uiI=0;uiI<ucOffset;uiI++)
			{
				if (uiI%10==0)util_Printf("\n");
				util_Printf("%02x ",aucBuf[uiI]);
			}
			util_Printf("\n==60������=End=\n");
			if( uiNum >= 5)
            {
				break;
            }
       }

		util_Printf("success2!\n");
		util_Printf("uiNum = %d\n",uiNum);
		if( uiNum != 0)
		{
			int_asc(aucBuf,2,&uiNum);
		}
		else
		{
			util_Printf("Send End\n");
			break;
		}
	    ISO8583_SetBitValue(60,aucBuf,ucOffset);

		/* No offline transaction, break */
		if( !uiTotalNb )
		{
			return(SUCCESS);
		}
		ucResult = SERV_UpLoadComm();
	}while(ucResult == SUCCESS);

    util_Printf("!****ucResult = %02x\n",ucResult);

	if(ucResult == SUCCESS)
	{
		//��ӡ�ɼ�����
		ucResult=PRINT_DFDatalist();
	}

	if(ucResult == SUCCESS)
	{
		XDATA_Clear_CollectTrans_File();
	}
	if(ucResult == SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(unsigned char*)" ���ݲɼ��ɹ�!");
		UTIL_GetKey(5);
	}
	else if (ucResult !=ERR_HOSTCODE)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(unsigned char*)" ���ݲɼ�ʧ��");
		//Os__GB2312_display(2,1,(unsigned char*)" ���ֹ�����");
		UTIL_GetKey(5);
	}
	if (ucResult==ERR_HOSTCODE)
	{
		util_Printf("�������ͳ���\n");
		//Os__clr_display(255);
		if (G_RUNDATA_ucErrorExtCode==5)
			//Os__GB2312_display(1,1,(unsigned char*)" ���ݸ�ʽ��");
        //else if (G_RUNDATA_ucErrorExtCode==3)
			//Os__GB2312_display(1,1,(unsigned char*)"��̨���ݿ��");
        //else
        //{
			//Os__GB2312_display(1,1,(unsigned char*)"���ݲɼ�ʧ��");
			//Os__GB2312_display(2,1,(unsigned char*)" ���ֹ�����");
        //}

		UTIL_GetKey(5);
		ucResult=SUCCESS;
	}
	return ucResult;
}

unsigned char SERV_SendCollectData(void)
{
	unsigned char ucResult=SUCCESS,ucPress;

	//Os__clr_display(255);
	//Os__GB2312_display(0, 0, (uchar *) "�Ƿ����Ͳɼ�����");
	//Os__GB2312_display(2, 0, (uchar *) "����-[ȷ��]");
	//Os__GB2312_display(3, 0, (uchar *) "�˳�-[ȡ��]");

	do
	{
		ucPress=UTIL_GetKey(30);
	}while(ucPress !=KEY_ENTER && ucPress != KEY_CLEAR && ucPress !=99);

	switch(ucPress)
	{
		case KEY_ENTER:
			G_NORMALTRANS_ucTransType=TRANS_SENDCOLLECTDATA;
			G_NORMALTRANS_euCardSpecies=0xFF;				//��������ʹ������ISO���Խṹ
			//Os__clr_display(255);
			//Os__GB2312_display(1, 1, (uchar *) " ���ݴ�����");
			COMMS_PreComm();
			ucResult=SERV_ProcessData();
			break;
		case 99:
		case KEY_CLEAR:
			return(SUCCESS);
	}
	return(ucResult);
}

unsigned char SERV_SettleProcess(void)
{
	unsigned char ucResult,ucR;
	ULONG_C51 ulBatchNumber;
	unsigned char aucBuf[8],date[8];
	short	iLen;
	unsigned short uiTransTotal,searchadr;
	unsigned short uiI,uiJ;

 	ucResult = SERV_SettleOnline();
	if(ucResult == SUCCESS)
	{
		/* Clear transaction & all information */
		/* Start Work */
		ucResult = ISO8583_GetBitValue(12,aucBuf,&iLen,sizeof(aucBuf));
		if( ucResult == SUCCESS )
		{
			asc_bcd(G_NORMALTRANS_aucTime,3,aucBuf,6);
		}
		ucResult = ISO8583_GetBitValue(13,aucBuf,&iLen,sizeof(aucBuf));
		if( ucResult == SUCCESS )
		{
			asc_bcd(&G_NORMALTRANS_aucDate[2],2,aucBuf,4);
			Os__read_date(date);
			asc_bcd(&G_NORMALTRANS_aucDate[1],1,&date[4],2);
			G_NORMALTRANS_aucDate[0] = (G_NORMALTRANS_aucDate[1] > 0x90) ?  0x19 :  0x20 ;
		}
		/*�������ʱ��*/
		Os__saved_copy(G_NORMALTRANS_aucDate,DataSave0.ChangeParamData.ucSettleData,TRANS_DATELEN);
		Os__saved_copy(G_NORMALTRANS_aucTime,DataSave0.ChangeParamData.ucSettleTime,TRANS_TIMELEN);
		Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucFunctStep,
						0,sizeof(unsigned char));
		/*ȡ�������־*/
		Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.aucSettleFlag,
						0,sizeof(unsigned char));
		Os__saved_set((unsigned char *)&DataSave0.ChangeParamData.uiTotalNumber,0,
						sizeof(unsigned int));
		Os__saved_set(&DataSave0.ChangeParamData.ucTicketPrintFlag,1,1);
	       Os__saved_set((unsigned char*)&DataSave0.ChangeParamData.uiEMVICLogNum,0x00,sizeof(unsigned short));
		Os__saved_set((unsigned char*)&DataSave0.ChangeParamData.ucScriptInformValid,0x00,sizeof(unsigned char));

		Os__saved_copy((unsigned char *)&DataSave0.TransInfoData.TransTotal,
						(unsigned char *)&DataSave0.TransInfoData.LastTransTotal,
						sizeof(TRANSTOTAL));
		Os__saved_copy((unsigned char *)&DataSave0.TransInfoData.ForeignTransTotal,
						(unsigned char *)&DataSave0.TransInfoData.LastForeignTransTotal,
						sizeof(TRANSTOTAL));
		Os__saved_copy((unsigned char *)&DataSave0.TransInfoData.TransDetail,
						(unsigned char *)&DataSave0.TransInfoData.LastTransDetail,
						sizeof(TRANSDETAIL)*6);
		DataSave0.TransInfoData.ulLastBatchNumber=DataSave0.ChangeParamData.ulBatchNumber;
		memcpy(DataSave0.TransInfoData.aucLastCashierNo,DataSave0.Cashier_SysCashier_Data.aucCashierNo[DataSave0.ChangeParamData.ucCashierLogonIndex],
						CASH_CASHIERNOLEN);
		memcpy( DataSave0.TransInfoData.ucLasSettleData ,
						DataSave0.ChangeParamData.ucSettleData ,TRANS_DATELEN);
		memcpy( DataSave0.TransInfoData.ucLasSettleTime ,
						DataSave0.ChangeParamData.ucSettleTime ,TRANS_TIMELEN);
		DataSave0.TransInfoData.ucLastSettleFlag=true;
		ucResult =XDATA_Write_Vaild_File(DataSaveConstant);
		if(ucResult) return ucResult;
		ucResult =XDATA_Write_Vaild_File(DataSaveChange);
		if(ucResult) return ucResult;
	       ucResult =XDATA_Write_Vaild_File(DataSaveTransInfo);
		if(ucResult) return ucResult;
		/***��ӡ����ͳ�Ƶ�***/
		PRINT_List(NORMAL_PRINT_FLAG|PRINT_SETTLE_FLAG);
		/*��ӡ����δ���͵��ѻ���ϸ*/
		PRINT_OfflineDetial();

		uiTransTotal=DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+ DataSave0.TransInfoData.TransTotal.uiTotalNb;
		util_Printf("�ڿ��ܱ���:%d\n",DataSave0.TransInfoData.TransTotal.uiTotalNb);
		util_Printf("�⿨�ܱ���:%d\n",DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb);

		if (DataSave0.ConstantParamData.ucCollectFlag=='1')
		{
			searchadr=0;
			for(uiI=0;uiI<uiTransTotal;uiI++)
			{
				for(uiJ=searchadr;uiJ<TRANS_MAXSENDNB;uiJ++)
				{
					util_Printf("�ɼ����ݵ�[%d]������ֵ:%02x\n",uiJ,DataSave0.ChangeParamData.uiSendIndex[uiJ]);
					if(DataSave0.ChangeParamData.uiSendIndex[uiJ]==0)
						break;
				}

				ucR = XDATA_Read_SaveTrans_File(uiI);
				util_Printf("��������:%02x\n",DataSave1.SaveTrans.ucTransType);
				if(ucR == SUCCESS
					&&(DataSave1.SaveTrans.ucTransType == TRANS_PURCHASE
						||DataSave1.SaveTrans.ucTransType == TRANS_OFFPURCHASE
						||DataSave1.SaveTrans.ucTransType == TRANS_MOBILEAUTH
						||DataSave1.SaveTrans.ucTransType == TRANS_VOIDPURCHASE
						||DataSave1.SaveTrans.ucTransType == TRANS_VOIDMOBILEAUTH)
				)
				{
					memcpy(&DataSave0.SaveTrans1,&DataSave1.SaveTrans,sizeof(NORMALTRANS));
					long_asc(DataSave0.SaveTrans1.aucTargetAcc,6,&DataSave0.ChangeParamData.ulBatchNumber);
					memcpy(&DataSave0.SaveTrans1.aucComm[10],DataSave0.ConstantParamData.aucMerchantID,PARAM_MERCHANTIDLEN);
					memcpy(&DataSave0.SaveTrans1.aucComm[10+PARAM_MERCHANTIDLEN],DataSave0.ConstantParamData.aucTerminalID,PARAM_TERMINALIDLEN);

					DataSave0.ChangeParamData.uiSendIndex[uiJ] = 1;
					bcd_asc(DataSave0.ChangeParamData.aucSendDate,&G_NORMALTRANS_aucDate[1],6);

					if(DataSave0.TransInfoData.auiTransIndex[uiI]==TRANS_NIIVOID)
						DataSave0.SaveTrans1.ucVoidFlag='1';
					else
						DataSave0.SaveTrans1.ucVoidFlag='0';

					searchadr=uiJ;

					DataSave0.ChangeParamData.uiSendTotalNb+= 1;
					util_Printf("DataSave0.ChangeParamData.uiSendTotalNb�ܱ���=%d\n",DataSave0.ChangeParamData.uiSendTotalNb);
					XDATA_Write_CollectTrans_File(uiJ);
					XDATA_Write_Vaild_File(DataSaveChange);
				}
			}
			DataSave0.ConstantParamData.ucSendCollectFlag = '1';
			XDATA_Write_Vaild_File(DataSaveConstant);
		}

		//�����ǰ����ͳ������
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.ForeignTransTotal,
					0,sizeof(TRANSTOTAL));
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.TransTotal,
					0,sizeof(TRANSTOTAL));
		Os__saved_set((unsigned char *)DataSave0.TransInfoData.auiTransIndex,
					0,sizeof(unsigned short)*TRANS_MAXNB);
		Os__saved_set((unsigned char *)&DataSave0.TransInfoData.TransDetail,
					0,sizeof(TRANSDETAIL)*6);

		ucResult =XDATA_Write_Vaild_File(DataSaveTransInfo);
		if(ucResult) return ucResult;
			ucResult = XDATA_Clear_SaveTrans_File();
		if(ucResult) return ucResult;
			ucResult =UTIL_DeleteAllData((PUCHAR)ERRICCTRANS);
		if(ucResult) return ucResult;
			ucResult =UTIL_DeleteAllData((PUCHAR)TRANSLOG);
		if(ucResult) return ucResult;
			ucResult =XDATA_Write_Vaild_File(DataSaveTransInfo);
		if(ucResult)	return ucResult;

		DataSave0.ChangeParamData.ucEMVICTransFailNum = 0;
		ulBatchNumber = DataSave0.ChangeParamData.ulBatchNumber;
		ulBatchNumber ++;
		if( ulBatchNumber > 999999)
		{
			ulBatchNumber = 1;
		}
		Os__saved_copy((unsigned char *)&ulBatchNumber,
					(unsigned char *)&DataSave0.ChangeParamData.ulBatchNumber,
					sizeof(ULONG_C51));
		Os__saved_set(&DataSave0.ChangeParamData.ucTicketPrintFlag,0,1);
		ucResult =XDATA_Write_Vaild_File(DataSaveChange);

		G_RUNDATA_ucReversalFlag = 0x30;
		Os__saved_set((unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid),
						0x00,sizeof(unsigned char));
		ucResult = XDATA_Write_Vaild_File(DataSaveTrans8583);
		util_Printf("�����8583:[%02x]\n",ucResult);
		if(ucResult==SUCCESS)
		{
			if(DataSave0.ConstantParamData.ucSelfLogoff == '1')
			{
				/*��ǩ�˱�־*/
				//Os__clr_display(255);
				//Os__GB2312_display(1,0,(unsigned char *)"    �������");
				//Os__GB2312_display(2,0,(unsigned char *)"    ����ǩ��...");
				MSG_WaitKey(3);
				G_NORMALTRANS_ucTransType = TRANS_LOGONOFF;
				G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
				G_RUNDATA_ucDialFlag = 1;
				ucResult = LOGON_Online();
				if(ucResult == SUCCESS)
				{
					Os__saved_set(&DataSave0.ChangeParamData.ucCashierLogonFlag,0,1);
					UTIL_IncreaseTraceNumber();
				}
			}
			else
			{	/*��ǩ����־*/
				//Os__clr_display(255);
				//Os__GB2312_display(1,0,(unsigned char *)"    �������");
				//Os__GB2312_display(2,0,(unsigned char *)"    ����ǩ��...");
				MSG_WaitKey(3);
				G_NORMALTRANS_ucTransType = TRANS_LOGONON;
				G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
				G_RUNDATA_ucDialFlag = 1;
				ucResult = LOGON_CashierProcess(0);
			}
		}
		if (ucResult==SUCCESS
			&&DataSave0.ConstantParamData.ucCollectFlag=='1')
		{
			ucResult=SERV_SendCollectData();
		}
	}
	return(ucResult);
}

unsigned char SERV_SettleOnline(void)
{
	unsigned char ucResult=SUCCESS ,ucSettleFlag;
	unsigned char aucBuf[200];
	short iLen;
	unsigned int ucICOnlineTransNum,ucICSpecTransNum,ucMagTransNum,ucOffTransNum;


	ucICOnlineTransNum=0;
	ucICSpecTransNum=0;
	ucMagTransNum=0;
	ucOffTransNum=0;

	util_Printf("����ʱ������־:%02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
	util_Printf("\n�����жϱ�־ = %02x\n",DataSave0.ConstantParamData.ucFunctStep);
	if(DataSave0.ChangeParamData.ucScriptInformValid)
	{
		ucResult=Trans_IssuerScriptInformSend();
		util_Printf("----B000-----settle---Trans_IssuerScriptInformSend()-ucResult = %02x\n",ucResult);
		if(ucResult) return ucResult;
	}
	if (DataSave0.ConstantParamData.ucFunctStep)
	{
		DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid = 0;
	}
	/* Check Reversal Exist? */
	if( DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid == DATASAVEVALIDFLAG)
	{
		ucResult=Trans_SendReversalTrans();
		util_Printf("----B001-----settle---Trans_SendReversalTrans()-ucResult = %02x\n",ucResult);
		if(ucResult) return ucResult;
	}
	else
	{
		DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag = 0;
		XDATA_Write_Vaild_File(DataSaveTrans8583);
	}
	util_Printf("\n�����жϱ�־ = %02x\n",DataSave0.ConstantParamData.ucFunctStep);
	if (DataSave0.ConstantParamData.ucFunctStep==0)
	{
		Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucFunctStep,
			1,sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
	/*���ʹ����������߽��㡢�������*/
	if(!ucResult) ucResult=Trans_MagOffLineSettle(false);
	/*����IC�����ѻ�����*/
	if(!ucResult) ucResult=Trans_SendICOfflineTrans();

	if(ucResult)	return ucResult;

	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;

	util_Printf("\neee����ʱ������־:%02x\n",G_RUNDATA_ucReversalFlag);
	if(DataSave0.ConstantParamData.ucFunctStep==1)
	{
		Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucFunctStep,
			2,sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
	G_NORMALTRANS_ucTransType = TRANS_SETTLE;

	util_Printf("ConstantParamData.ucFunctStep=%02x\n",DataSave0.ConstantParamData.ucFunctStep);
	//Os__clr_display(255);
	//Os__GB2312_display(1,0,(unsigned char*)"   ����������");
	//Os__GB2312_display(2,0,(unsigned char*)"     ���Ժ�");
	UTIL_WaitGetKey(2);
	if( (DataSave0.ConstantParamData.ucFunctStep == 2)
	  ||(DataSave0.ConstantParamData.ucFunctStep == 0)
	  )
	{
		ISO8583_Clear();
		ISO8583_SetMsgID(500);
		/* Set system trace audit number bit 11 */
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
	    ISO8583_SetBitValue(11,aucBuf,6);
	  	/* Set Card Acceptor Terminal Identification bit 41 */
		ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
		/* Set Card Acceptor Identification Code bit 42 */
		ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);
		/* Set bit 48 */
		memset(aucBuf,0,sizeof(aucBuf));
	#if 0
		DataSave0.TransInfoData.TransTotal.ulDebitAmount+=100;
		DataSave0.TransInfoData.TransTotal.uiDebitNb+=1;
		//yj
	#endif
		long_asc(aucBuf,12,&DataSave0.TransInfoData.TransTotal.ulDebitAmount);
		int_asc(&aucBuf[12],3,&DataSave0.TransInfoData.TransTotal.uiDebitNb);
		long_asc(&aucBuf[15],12,&DataSave0.TransInfoData.TransTotal.ulCreditAmount);
		int_asc(&aucBuf[27],3,&DataSave0.TransInfoData.TransTotal.uiCreditNb);
		aucBuf[30] = 0x30;
		long_asc(&aucBuf[31],12,&DataSave0.TransInfoData.ForeignTransTotal.ulDebitAmount);
		int_asc(&aucBuf[43],3,&DataSave0.TransInfoData.ForeignTransTotal.uiDebitNb);
		long_asc(&aucBuf[46],12,&DataSave0.TransInfoData.ForeignTransTotal.ulCreditAmount);
		int_asc(&aucBuf[58],3,&DataSave0.TransInfoData.ForeignTransTotal.uiCreditNb);
		aucBuf[61] = 0x30;
		ISO8583_SetBitValue(48,aucBuf,62);

		/* Set bit 49 */
		ISO8583_SetBitValue(49,(uchar *)"156",3);
		/* Set bit 60 */
		memset(aucBuf,0,sizeof(aucBuf));
		memcpy(aucBuf,"00",2);
		long_asc(&aucBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
		memcpy(&aucBuf[8],"2010",4);
	       ISO8583_SetBitValue(60,aucBuf,11);
		/* Set bit 63 */
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',3);
		memcpy(aucBuf,DataSave0.Cashier_SysCashier_Data.aucCashierNo[DataSave0.ChangeParamData.ucCashierLogonIndex],
	        			CASH_CASHIERNOLEN);
		ISO8583_SetBitValue(63,aucBuf,3);
		ucResult = COMMS_GenSendReceive();
		if( ucResult == SUCCESS )
		{
			/*�����յ��к�,�����к�*/
			memset(aucBuf,0,sizeof(aucBuf));
			ucResult = ISO8583_GetBitValue(44,aucBuf,&iLen,sizeof(aucBuf));
			if(ucResult == SUCCESS)
			{
				memcpy(DataSave0.ChangeParamData.aucIssuerID,aucBuf,11);
				memcpy(DataSave0.ChangeParamData.aucAcquereID,&aucBuf[11],11);

				memset(aucBuf,0x00,sizeof(aucBuf));
				memcpy(aucBuf,DataSave0.ChangeParamData.aucIssuerID,11);
				util_Printf("Settle IssuserID %s",aucBuf);
				memcpy(aucBuf,DataSave0.ChangeParamData.aucAcquereID,11);
				util_Printf("Settle aucAcquereID %s",aucBuf);
			}
			else
			{
				memset(aucBuf,0x00,sizeof(aucBuf));
				memcpy(aucBuf,DataSave0.ChangeParamData.aucIssuerID,11);
				util_Printf("Settle11 IssuserID %s",aucBuf);
				memcpy(aucBuf,DataSave0.ChangeParamData.aucAcquereID,11);
				util_Printf("Settle11 aucAcquereID %s",aucBuf);
				/*
				memset(DataSave0.ChangeParamData.aucIssuerID,
					0,sizeof(DataSave0.ChangeParamData.aucIssuerID) );

				memset(DataSave0.ChangeParamData.aucAcquereID,
					0,sizeof(DataSave0.ChangeParamData.aucAcquereID) );
				*/
			}
			XDATA_Write_Vaild_File(DataSaveChange);

	   		memset(aucBuf,0,sizeof(aucBuf));
	   		ucResult = ISO8583_GetBitValue(48,aucBuf,&iLen,sizeof(aucBuf));
#ifdef TEST
			util_Printf("ISO8583_GetBitValue=%02x\n",ucResult);
#endif
			if(ucResult == SUCCESS)
			{
				ucSettleFlag = aucBuf[30];

				if(ucSettleFlag == '1')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucInternalSettleFlag,
							SETTLE_OK,sizeof(unsigned char));
				else
				if(ucSettleFlag == '2')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucInternalSettleFlag,
							SETTLE_FAIL,sizeof(unsigned char));
				else
				if(ucSettleFlag == '3')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucInternalSettleFlag,
							SETTLE_ERROR,sizeof(unsigned char));
#ifdef TEST
				util_Printf("ucSettleFlag=%02x,ucInternalSettleFlag=%02x\n",ucSettleFlag,DataSave0.ConstantParamData.ucInternalSettleFlag);
#endif
				ucSettleFlag = aucBuf[61];
				if(ucSettleFlag == '1')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucForgienSettleFlag,
							SETTLE_OK,sizeof(unsigned char));
				else
				if(ucSettleFlag == '2')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucForgienSettleFlag,
							SETTLE_FAIL,sizeof(unsigned char));
				else
				if(ucSettleFlag == '3')
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucForgienSettleFlag,
							SETTLE_ERROR,sizeof(unsigned char));
#ifdef TEST
				util_Printf("ucSettleFlag=%02x,ucForgienSettleFlag=%02x\n",ucSettleFlag,DataSave0.ConstantParamData.ucForgienSettleFlag);
#endif

				XDATA_Write_Vaild_File(DataSaveConstant);
				UTIL_IncreaseTraceNumber();
				if(DataSave0.ConstantParamData.ucFunctStep==2)
				{
					Os__saved_set((unsigned char *)&DataSave0.ConstantParamData.ucFunctStep,
								3,sizeof(unsigned char));
					XDATA_Write_Vaild_File(DataSaveConstant);
				}
			}
		}
	}
#ifdef TEST
	util_Printf("ConstantParamData.ucFunctStep=%02x\n",DataSave0.ConstantParamData.ucFunctStep);
#endif

	if(ucResult==SUCCESS)
	{
//		DataSave0.ConstantParamData.ucForgienSettleFlag=SETTLE_FAIL;
//		DataSave0.ConstantParamData.ucInternalSettleFlag=SETTLE_FAIL;

		util_Printf("\nEMV_OnlineTransSend");
		util_Printf("\nucForgienSettleFlag  %d",DataSave0.ConstantParamData.ucForgienSettleFlag);
		util_Printf("\nucInternalSettleFlag  %d",DataSave0.ConstantParamData.ucInternalSettleFlag);
		ucSettleFlag=(DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
			          &(DataSave0.ConstantParamData.ucInternalSettleFlag ==SETTLE_OK);
		G_RUNDATA_ucSettleFlag = ucSettleFlag;
		DataSave0.TransInfoData.ForeignTransTotal.ucSettleFlag = DataSave0.ConstantParamData.ucForgienSettleFlag;
		DataSave0.TransInfoData.TransTotal.ucSettleFlag = DataSave0.ConstantParamData.ucInternalSettleFlag;
		if(!ucResult)  ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);

	}
	if (ucResult == SUCCESS)
	{
	  	if(ucSettleFlag)
	  	{
	  	ucBatchSandTransNum =0;
			//Send IC Online
			//����IC����������
			if(!ucResult)  ucResult=Serv_OnlineTransSend(ucSettleFlag,&ucICOnlineTransNum);
			util_Printf("\nucResult ==1001=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1001=%d",ucBatchSandTransNum);
			util_Printf("\nucICOnlineTransNum==1001=%d",ucICOnlineTransNum);
			//Send Offline Error or ARPC Error
			if(!ucResult)  ucResult=Trans_SpecInfoSend(ucSettleFlag,&ucICSpecTransNum);
			util_Printf("\nucResult ==1002=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1002=%d",ucBatchSandTransNum);
			util_Printf("\nucICSpecTransNum==1002=%d",ucICSpecTransNum);
			if(!ucResult)
			ucResult = SERV_SettleBatchEnd(ucSettleFlag,ucBatchSandTransNum);
	  	}
		else
		{
#if 1
			ucSettleBatchSendFlag=1;
			ucBatchSandTransNum=0;
			G_RUNDATA_ucTransType = TRANS_SETTLE;
			//���ʹſ����߽���(ʵ�ʰ���IC/MAG�����ߵ��������߽��㡢�˻���Ԥ��Ȩ���(����))
			ucResult=SERV_SettleOfflineSend(TRANS_BATCHSEND,&ucOffTransNum);
			util_Printf("\nucResult ==1003=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1003=%d",ucBatchSandTransNum);
			util_Printf("\nucICOnlineTransNum==1003=%d",ucOffTransNum);
			//����IC�����߽���(����IC���ѻ�����)
			if(!ucResult)  ucResult=SERV_BatchSendICOfflineTrans();
			util_Printf("\nucResult ==1004=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1004=%d",ucBatchSandTransNum);
			//���ʹſ���������(����MAG�����ѡ����ѳ�����Ԥ��Ȩ���(����)��Ԥ��Ȩ���(����)����)
			if(!ucResult)  ucResult=SERV_MAGTransBatchSend();
			util_Printf("\nucResult ==1005=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1005=%d",ucBatchSandTransNum);
			//����֪ͨ�ཻ��(ʵ�ʰ���IC�����˻���Ԥ��Ȩ���(����))
			if(!ucResult)  ucResult = SERV_ICInformTransSend(ucSettleFlag,&ucICOnlineTransNum);
			util_Printf("\nucResult ==1006=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1006=%d",ucBatchSandTransNum);
			util_Printf("\nucICOnlineTransNum==1006=%d",ucICOnlineTransNum);
			//����IC����������(����IC�����Ѻ��ֽ�Ȧ��)
			if(!ucResult)  ucResult=Serv_OnlineTransSend(ucSettleFlag,&ucICOnlineTransNum);
			util_Printf("\nucResult ==1007=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1007=%d",ucBatchSandTransNum);
			util_Printf("\nucICOnlineTransNum==1007=%d",ucICOnlineTransNum);
			//Send Offline Error or ARPC Error
			if(!ucResult)  ucResult=Trans_SpecInfoSend(ucSettleFlag,&ucICSpecTransNum);
			util_Printf("\nucResult ==1008=%d",ucResult);
			util_Printf("\nucBatchSandTransNum==1008=%d",ucBatchSandTransNum);
			util_Printf("\nucICOnlineTransNum==1008=%d",ucICSpecTransNum);
			//�����ͽ���
			if(!ucResult)  ucResult = SERV_SettleBatchEnd2(ucBatchSandTransNum);
#else
			ucResult = SERV_ICInformTransSend(ucSettleFlag,&ucICOnlineTransNum);
			if(!ucResult)  ucResult = SERV_SettleBatchSend(ucICOnlineTransNum+ucICSpecTransNum);
#endif
		}
	}
util_Printf("rrrr����ʱ������־:%02x\n",G_RUNDATA_ucReversalFlag);
	return(ucResult);
}

unsigned char SERV_SettleBatchSend(unsigned int ucSentTransNum)
{
	unsigned char ucResult,ucI;
	unsigned char ucContinueFlag;
	unsigned char aucBuf[500];
	unsigned char aucCardNo[20];
	unsigned char aucTmpBuf[12];
	unsigned short uiOffset;
	unsigned short uiStartIndex;
	unsigned short uiIndex,uiNII;
	unsigned int uiNum;
	unsigned int uiTotalNb;
	unsigned int ucIndex,ucOffTransNum;
	SAVESENDINDEX  SaveSendIndex[ISO8583_BIT48NUM+1];
	BACKUPISO8583		ISO8583BACKUP;
	ucContinueFlag = true;

	uiStartIndex = 0;
	util_Printf("\n------------------���㲻ƽ------����������-------2----------------\n");
	G_RUNDATA_ucTransType = TRANS_SETTLE;
	/*������㲻ƽ���赥�������˻������߽��㣬�������
		��������ʱ��MSGID��Ϊ�µ�320
	*/
	uiTotalNb=ucSentTransNum;
	ucResult = SUCCESS;

	do{
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(unsigned char *)"    ���㲻ƽ");
		//Os__GB2312_display(2,0,(unsigned char *)"   ����������");
		//Os__GB2312_display(3,0,(unsigned char *)"     ���Ժ�");
		UTIL_WaitGetKey(2);
		ISO8583_Clear();
		ISO8583_SetMsgID(320);
		G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		/* Set system trace audit number bit 11 */
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucTmpBuf,0,sizeof(aucTmpBuf));
		long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
	       ISO8583_SetBitValue(11,aucBuf,6);
	  	/* Set Card Acceptor Terminal Identification bit 41 */
		ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
		/* Set Card Acceptor Identification Code bit 42 */
		ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);
		/* Set bit 48 */
    memset(aucBuf,'0',sizeof(aucBuf));
		uiOffset = 2;
		uiNum = 0;
		ucIndex = 0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++,uiStartIndex++)
		{
			if(uiIndex>=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+
			    DataSave0.TransInfoData.TransTotal.uiTotalNb))
					break;
#ifdef TEST
			util_Printf("TransInfoData.auiTransIndex111=%d\n",(DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND));
#endif
			if(G_NORMALTRANS_euCardSpecies!=CARDSPECIES_MAG) continue;

			GetOldTransData(uiIndex);
			OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
			if( (G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
			  ||(G_NORMALTRANS_ucTransType==TRANS_PREAUTHFINISH)
			  ||(G_NORMALTRANS_ucTransType==TRANS_VOIDPURCHASE)
			  ||(G_NORMALTRANS_ucTransType==TRANS_VOIDPREAUTHFINISH)
			  ||(G_NORMALTRANS_ucTransType==TRANS_EC_ASSIGNLOAD)
			  ||(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
			  ||(G_NORMALTRANS_ucTransType==TRANS_EC_CASHLOAD)
			  )
			{
				{
					if( (DataSave0.ConstantParamData.ucInternalSettleFlag == SETTLE_OK)
					  &&(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
					  )
					  {
					  		continue;
					  }
					if( (DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
					  &&(G_NORMALTRANS_ucCardType != TRANS_CARDTYPE_INTERNAL)
					  )
					 {
					  		continue;
					 }
				}
				uiTotalNb++;
				if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND) == TRANS_BATCHSEND)
					continue;

				SaveSendIndex[ucIndex++].uiSendIndex = uiIndex;
				if(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
					memcpy(&aucBuf[uiOffset],"00",2);
				else
					memcpy(&aucBuf[uiOffset],"01",2);
				uiOffset+=2;
				long_asc(&aucBuf[uiOffset],6,&G_NORMALTRANS_ulTraceNumber);
				uiOffset+=6;
				memset(aucCardNo,0,sizeof(aucCardNo));
				if(G_NORMALTRANS_ucSourceAccLen%2)
				{
					bcd_asc(aucCardNo,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
				}else
				{
					bcd_asc(aucCardNo,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
				}
				memset(&aucBuf[uiOffset],0x30,20-G_NORMALTRANS_ucSourceAccLen);
				uiOffset += (20-G_NORMALTRANS_ucSourceAccLen);
				memcpy(&aucBuf[uiOffset],aucCardNo,G_NORMALTRANS_ucSourceAccLen);
				uiOffset+=G_NORMALTRANS_ucSourceAccLen;
				long_asc(&aucBuf[uiOffset],12,&G_NORMALTRANS_ulAmount);
				uiOffset+=12;
				uiNum++;

				if(uiNum >= ISO8583_BIT48NUM)
                {
                    Os__saved_swap(DATASAVEPAGE_NORMAL);
                    uiStartIndex ++;
					break;
                }
			}
		}
		G_NORMALTRANS_ucTransType = TRANS_SETTLE;
		memcpy(aucTmpBuf,"00",2);
		long_asc(&aucTmpBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
		if(uiNum!=0)
		{
			int_asc(aucBuf,2,&uiNum);
			ISO8583_SetBitValue(48,aucBuf,uiOffset);
			memcpy(&aucTmpBuf[8],"201",3);
		}else
		{
			ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
						(unsigned char *)&ISO8583BACKUP);
			ucResult=SERV_BatchSendICOfflineTrans();
			if(!ucResult) SERV_SettleOfflineSend(TRANS_BATCHSEND,&ucOffTransNum);
			uiTotalNb+=ucOffTransNum;

			ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
				(unsigned char *)&ISO8583BACKUP);

			int_asc(aucBuf,4,&uiTotalNb);
			ISO8583_SetBitValue(48,aucBuf,4);
			memcpy(&aucTmpBuf[8],"2020",4);
			ucContinueFlag = false;
		}
		ISO8583_SetBitValue(60,aucTmpBuf,11);
		ucResult = COMMS_GenSendReceive();
		/************************************************************************/
		if( (ucResult == SUCCESS)
		  ||(ucResult == ERR_HOSTCODE)
		  )
		{
			for(ucI=0;ucI<ucIndex;ucI++)
			{
				uiNII = (DataSave0.TransInfoData.auiTransIndex[SaveSendIndex[ucI].uiSendIndex])|TRANS_BATCHSEND;
				Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[SaveSendIndex[ucI].uiSendIndex],
					sizeof(unsigned short));
				XDATA_Write_Vaild_File(DataSaveTransInfo);
				Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&DataSave0.ChangeParamData.uiTotalNumber,
					sizeof(unsigned int));
				XDATA_Write_Vaild_File(DataSaveChange);
			}
		}
		UTIL_IncreaseTraceNumber();
		G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		/************************************************************************/
	}while( ( (ucResult == SUCCESS)
					      	||(ucResult == ERR_HOSTCODE)
					      	)&&(ucContinueFlag == true)
					 );
	util_Printf("\n------------------���㲻ƽ------����������-------2----END------------\n");
	return(ucResult);
}

unsigned char SERV_SettleOfflineSend(unsigned short uiSendFlag,unsigned int *pucOfflineNum)
{
	unsigned char ucResult,ucReFile;
	unsigned short uiIndex,uiNII;
       unsigned int ucOfflineNum;
	unsigned char ucDispIndex[17],aucBuf[2];

	//Os__clr_display(255);
	//Os__GB2312_display(1,0,(unsigned char*)"�����������߽���");
	//Os__GB2312_display(3,0,(unsigned char*)"     ���Ժ�");
	util_Printf("\n------------------�������ʹ��������߽���-------1----------------\n");
	ucResult = SUCCESS;
	ucOfflineNum=0;
	for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
	{

		if(uiIndex>=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+
		    	DataSave0.TransInfoData.TransTotal.uiTotalNb))
				break;
		GetOldTransData(uiIndex);
   	OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
					(unsigned char *)&NormalTransData,
					sizeof(NORMALTRANS));
		if(uiSendFlag == TRANS_NIISENDFINISH)
		{
			if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_NIISENDFINISH) == TRANS_NIISENDFINISH)
				continue;
		}else
		{
			if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND) == TRANS_BATCHSEND)
			{
				if(  (G_NORMALTRANS_ucTransType == TRANS_TIPADJUST)
   		   		||(G_NORMALTRANS_ucTransType == TRANS_OFFPREAUTHFINISH)
   		   		||(G_NORMALTRANS_ucTransType == TRANS_TIPADJUSTOK)
   		   		||(G_NORMALTRANS_ucTransType == TRANS_REFUND)
   		   		||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHSETTLE)
   		   		)
   		   		ucOfflineNum++;
				continue;
			}
		}

		if(uiSendFlag != TRANS_NIISENDFINISH)
		{
			if( (DataSave0.ConstantParamData.ucInternalSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
			  )
			  		continue;
			if( (DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType != TRANS_CARDTYPE_INTERNAL)
			  )
			  		continue;
		}
		if(  (G_NORMALTRANS_ucTransType == TRANS_TIPADJUST)
   		   ||(G_NORMALTRANS_ucTransType == TRANS_OFFPREAUTHFINISH)
   		   ||(G_NORMALTRANS_ucTransType == TRANS_TIPADJUSTOK)
   		   ||(G_NORMALTRANS_ucTransType == TRANS_OFFPURCHASE&&uiSendFlag == TRANS_NIISENDFINISH)
   		   //||(G_NORMALTRANS_ucTransType == TRANS_REFUND&&uiSendFlag == TRANS_BATCHSEND)
   		   //||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHSETTLE&&uiSendFlag == TRANS_BATCHSEND)
  			)
  		  {
  		  	memset(ucDispIndex , 0, sizeof(ucDispIndex));
  		  	memcpy(ucDispIndex , "    ��[  ]��", 12 );
			memset(aucBuf , '0',sizeof(aucBuf));
			int_asc(aucBuf, 2, (unsigned int *)(&ucOfflineNum));
			memcpy(&ucDispIndex[7] , aucBuf ,2);
			//Os__clr_display(2);
  		  	//Os__GB2312_display(2,0,(unsigned char*)ucDispIndex);

			ucResult = TRANS_Online();

			if(uiSendFlag == TRANS_NIISENDFINISH)
			{
				uiNII = (DataSave0.TransInfoData.auiTransIndex[uiIndex]|TRANS_NIISENDFINISH);
				if(ucResult != SUCCESS)
                			Os__saved_set((unsigned char *)&(DataSave1.SaveTrans.ucNotSendOkFlag),true,1);
			}
			else
			{
				uiNII = (DataSave0.TransInfoData.auiTransIndex[uiIndex]|TRANS_BATCHSEND);
				ucOfflineNum++;
			}
			Os__saved_copy((unsigned char *)&uiNII,
						(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiIndex],
						sizeof(unsigned short));
			ucResult = SUCCESS;

		  }
	}
	ucReFile = XDATA_Write_Vaild_File(DataSaveTransInfo);
	if (ucReFile != SUCCESS)
	{
		MSG_DisplayErrMsg(ucReFile);
		return(ERR_CANCEL);
	}
	*pucOfflineNum=ucOfflineNum;
	if(ucSettleBatchSendFlag)
		ucBatchSandTransNum=ucOfflineNum;
	util_Printf("\n------------------�����������߽���-------1---END-------------\n");
	return(SUCCESS);

}

unsigned char SERV_BatchSendICOfflineTrans(void)
{
	unsigned char 	ucResult=SUCCESS,ucReFile=SUCCESS;
	unsigned short  uiTransIndex,uiNII,uiTotalNb;

	util_Printf("\n**************************************************\n");
	util_Printf("\n		   Send IC Offline Trans");
	util_Printf("\n**************************************************\n");
	ucResult=SUCCESS;


	uiTotalNb=DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+DataSave0.TransInfoData.TransTotal.uiTotalNb;
	for(uiTransIndex=0;uiTransIndex<=uiTotalNb;uiTransIndex++)
    {
    	//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"����IC�����߽���");

		GetOldTransData(uiTransIndex);
       	OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
		if((DataSave1.SaveTrans.ucTransType==TRANS_OFFPURCHASE)
		  &&(((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])&TRANS_BATCHSEND) != TRANS_BATCHSEND)
		  &&((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])!= 0 ) )
		{
			if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_MAG) continue;
			if( (DataSave0.ConstantParamData.ucInternalSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
			  )
			  	continue;
			if( (DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType != TRANS_CARDTYPE_INTERNAL)
			  )
			  	continue;

			G_EXTRATRANS_ucInputPINLen=0;
			ucResult = TRANS_Online();
			if( ucResult == SUCCESS)
			{
				ucBatchSandTransNum++;
				uiNII = DataSave0.TransInfoData.auiTransIndex[uiTransIndex]|TRANS_BATCHSEND;
				Os__saved_copy((unsigned char *)&uiNII,
								(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiTransIndex],
								sizeof(unsigned short));

				ucReFile=XDATA_Write_Vaild_File(DataSaveTransInfo);
				if (ucReFile != SUCCESS)
				{
					MSG_DisplayErrMsg(ucReFile);
					return(ERR_CANCEL);
				}
			}
			else
				break;
		}
	}

	util_Printf("\n*******************SEND IC OFFLINE END*******************\n");

	return ucResult;

}

unsigned char SERV_MAGTransBatchSend(void )
{
	unsigned char ucResult,ucReFile,ucI;
	unsigned char ucContinueFlag;
	unsigned char aucBuf[500];
	unsigned char aucCardNo[20];
	unsigned char aucTmpBuf[12];
	unsigned short uiOffset;
	unsigned short uiStartIndex;
	unsigned short uiIndex,uiNII;
	unsigned int uiNum;
	unsigned int uiTotalNb;
	unsigned char ucIndex;
	SAVESENDINDEX  SaveSendIndex[ISO8583_BIT48NUM+1];

	ucContinueFlag = true;

	uiStartIndex = 0;
	util_Printf("\n------------------���㲻ƽ------����������-------2----------------\n");
	G_RUNDATA_ucTransType = TRANS_SETTLE;
	/*������㲻ƽ���赥�������˻������߽��㣬�������
		��������ʱ��MSGID��Ϊ�µ�320
	*/
	uiTotalNb=0;
	ucResult = SUCCESS;

	do{
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(unsigned char *)"    ���㲻ƽ");
		//Os__GB2312_display(2,0,(unsigned char *)"   ����������");
		//Os__GB2312_display(3,0,(unsigned char *)"     ���Ժ�");
		ISO8583_Clear();
		ISO8583_SetMsgID(320);
		G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		/* Set system trace audit number bit 11 */
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucTmpBuf,0,sizeof(aucTmpBuf));
		long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
	  ISO8583_SetBitValue(11,aucBuf,6);
	  	/* Set Card Acceptor Terminal Identification bit 41 */
		ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
		/* Set Card Acceptor Identification Code bit 42 */
		ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);
		/* Set bit 48 */
    memset(aucBuf,'0',sizeof(aucBuf));
		uiOffset = 2;
		uiNum = 0;
		ucIndex = 0;
		for(uiIndex=uiStartIndex;uiIndex<TRANS_MAXNB;uiIndex++,uiStartIndex++)
		{
			if(uiIndex>=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+
			    DataSave0.TransInfoData.TransTotal.uiTotalNb))
					break;
#ifdef TEST
			util_Printf("TransInfoData.auiTransIndex111=%d\n",(DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND));
#endif
			GetOldTransData(uiIndex);
		//	OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
		//					(unsigned char *)&NormalTransData,
		//					sizeof(NORMALTRANS));

			if(G_NORMALTRANS_euCardSpecies!=CARDSPECIES_MAG) continue;

			if( (G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
			  ||(G_NORMALTRANS_ucTransType==TRANS_PREAUTHFINISH)
			  ||(G_NORMALTRANS_ucTransType==TRANS_VOIDPURCHASE)
			  ||(G_NORMALTRANS_ucTransType==TRANS_VOIDPREAUTHFINISH)
			  )
			{
				{
					if( (DataSave0.ConstantParamData.ucInternalSettleFlag == SETTLE_OK)
					  &&(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
					  )
					  {
					  		continue;
					  }
					if( (DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
					  &&(G_NORMALTRANS_ucCardType != TRANS_CARDTYPE_INTERNAL)
					  )
					 {
					  		continue;
					 }
				}
				uiTotalNb++;
				ucBatchSandTransNum++;
				if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND) == TRANS_BATCHSEND)
					continue;

				SaveSendIndex[ucIndex++].uiSendIndex = uiIndex;
				if(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
					memcpy(&aucBuf[uiOffset],"00",2);
				else
					memcpy(&aucBuf[uiOffset],"01",2);
				uiOffset+=2;
				long_asc(&aucBuf[uiOffset],6,&G_NORMALTRANS_ulTraceNumber);
				uiOffset+=6;
				memset(aucCardNo,0,sizeof(aucCardNo));
				if(G_NORMALTRANS_ucSourceAccLen%2)
				{
					bcd_asc(aucCardNo,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
				}else
				{
					bcd_asc(aucCardNo,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
				}
				memset(&aucBuf[uiOffset],0x30,20-G_NORMALTRANS_ucSourceAccLen);
				uiOffset += (20-G_NORMALTRANS_ucSourceAccLen);
				memcpy(&aucBuf[uiOffset],aucCardNo,G_NORMALTRANS_ucSourceAccLen);
				uiOffset+=G_NORMALTRANS_ucSourceAccLen;
				long_asc(&aucBuf[uiOffset],12,&G_NORMALTRANS_ulAmount);
				uiOffset+=12;
				uiNum++;

				if(uiNum >= ISO8583_BIT48NUM)
        {
            uiStartIndex ++;
						break;
        }
			}
		}
		G_NORMALTRANS_ucTransType = TRANS_SETTLE;
		memcpy(aucTmpBuf,"00",2);
		long_asc(&aucTmpBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
		if(uiNum!=0)
		{
			int_asc(aucBuf,2,&uiNum);
			ISO8583_SetBitValue(48,aucBuf,uiOffset);
			memcpy(&aucTmpBuf[8],"201",3);
		}else
		{
			break;
		}
		ISO8583_SetBitValue(60,aucTmpBuf,11);
		ucResult = COMMS_GenSendReceive();
		/************************************************************************/
		if( (ucResult == SUCCESS)
		  ||(ucResult == ERR_HOSTCODE)
		  )
		{
			for(ucI=0;ucI<ucIndex;ucI++)
			{
				uiNII = (DataSave0.TransInfoData.auiTransIndex[SaveSendIndex[ucI].uiSendIndex])|TRANS_BATCHSEND;
				Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[SaveSendIndex[ucI].uiSendIndex],
					sizeof(unsigned short));
				ucReFile = XDATA_Write_Vaild_File(DataSaveTransInfo);
				if (ucReFile != SUCCESS)
				{
					MSG_DisplayErrMsg(ucReFile);
					return(ERR_CANCEL);
				}
				//ucBatchSandTransNum +=uiTotalNb;
				Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&DataSave0.ChangeParamData.uiTotalNumber,
					sizeof(unsigned int));
				XDATA_Write_Vaild_File(DataSaveChange);
			}
			//ucBatchSandTransNum+=uiTotalNb;
		}
		UTIL_IncreaseTraceNumber();
		G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
		/************************************************************************/
	}while( ( (ucResult == SUCCESS)
					      	||(ucResult == ERR_HOSTCODE)
					      	)&&(ucContinueFlag == true)
					 );
	util_Printf("\n------------------���㲻ƽ------����������-------2----END------------\n");
	return(ucResult);
}


unsigned char Serv_OnlineTransSend(unsigned char ucSettleFlag,unsigned int *pucTransNum)
{
	unsigned char ucResult,ucReFile,aucBuf[100];
	unsigned short uiIndex,uiNII;
	unsigned short uiI;
       unsigned char ucTransNum=0;

	util_Printf("\n**************************************************\n");
	util_Printf("\n	          Send Online ");
	util_Printf("\n**************************************************\n");
	ucResult = SUCCESS;
	util_Printf("\nEMV_OnlineTransSend In");
	G_RUNDATA_ucTransType = TRANS_SETTLE;
	uiI=0;

	if( DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb==0
		&& DataSave0.TransInfoData.TransTotal.uiTotalNb==0)
		return SUCCESS;

	for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
	{
		if(uiIndex>=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+
		    DataSave0.TransInfoData.TransTotal.uiTotalNb))
				return SUCCESS;
		GetOldTransData(uiIndex);
		OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
						(unsigned char *)&NormalTransData,
						sizeof(NORMALTRANS));
		if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_MAG) continue;
		if(!((G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
			||G_NORMALTRANS_ucTransType==TRANS_PREAUTH
			//||G_NORMALTRANS_ucTransType==TRANS_EC_ASSIGNLOAD
			//||G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD
			||G_NORMALTRANS_ucTransType==TRANS_EC_CASHLOAD)
			)
			continue;
		if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_BATCHSEND) == TRANS_BATCHSEND)
		{
				ucTransNum++;
				continue;
		}
//		if((DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_NIIVOID) == TRANS_NIIVOID)
//			continue;
//		if(uiI++>=DataSave0.ChangeParamData.uiEMVICLogNum) return EMV_NOSUCHTRACE;

		util_Printf("\nG_NORMALTRANS_ucTransType %02X",G_NORMALTRANS_ucTransType);
		if((G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
			||G_NORMALTRANS_ucTransType==TRANS_PREAUTH
			//||G_NORMALTRANS_ucTransType==TRANS_EC_ASSIGNLOAD
			//||G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD
			||G_NORMALTRANS_ucTransType==TRANS_EC_CASHLOAD
			)
		{

			ISO8583_Clear();
			ISO8583_SetMsgID(320);
			if(G_NORMALTRANS_ucSourceAccLen)
			{
				if(G_NORMALTRANS_ucSourceAccLen%2)
					bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
				else
					bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
				ISO8583_SetBitValue(2,aucBuf,G_NORMALTRANS_ucSourceAccLen);
			}

#ifdef CCB
			memset(aucBuf,0,sizeof(aucBuf));
			switch(G_NORMALTRANS_ucTransType)
			{

			case TRANS_PURCHASE:
				aucBuf[0] = 0x00;
				break;
			case TRANS_PREAUTH:
				aucBuf[0] = 0x03;
				break;

			default:
				break;
			}
			aucBuf[1] = aucBuf[1] | (((G_NORMALTRANS_ucSourceAccType - 0x30) << 4 ) & 0xF0);
			hex_asc(G_RUNDATA_RevChack_ucProcessCode, aucBuf, 6);
		       ISO8583_SetBitHexValue(3,aucBuf,3);
#endif
			memset(aucBuf,0,sizeof(aucBuf));
			long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
			ISO8583_SetBitValue(11,aucBuf,6);

			long_asc(aucBuf,12,&G_NORMALTRANS_ulAmount);
			ISO8583_SetBitValue(4,aucBuf,12);

			memcpy(aucBuf,"0500",4);
			ISO8583_SetBitValue(22,aucBuf,4);

	 		if(G_NORMALTRANS_ucPANSeqExist)
	 		{
				memset(aucBuf,0x00,sizeof(aucBuf));
			 	ISO8583_SetBitHexValue(23,&G_NORMALTRANS_ucPANSeq,sizeof(G_NORMALTRANS_ucPANSeq));
	 		}
			ISO8583_SetBitValue(41, G_RUNDATA_aucTerminalID, 8);

			ISO8583_SetBitValue(42, G_RUNDATA_aucMerchantID, 15);

			ISO8583_SetBitValue(55, G_NORMALTRANS_ucField55Data, G_NORMALTRANS_uiField55Len);

                     memset(aucBuf,0,sizeof(aucBuf));
			memcpy(aucBuf,"00",2);
#if 0               //�˴��ڱ����������������Ҫ��,���������Ϻ��ֹ�˾���Բ�Ҫ�������´���
			switch(G_NORMALTRANS_ucTransType)
			{
				case TRANS_PREAUTH:
					memcpy(aucBuf,"10",2);
					break;
				case TRANS_PURCHASE:
					//memcpy(aucBuf,"22",2);
					memcpy(aucBuf,"00",2);
					break;
				case TRANS_EC_CASHLOAD:
					memcpy(aucBuf,"46",2);
					break;
				default:
					break;
			}
#endif
			long_asc(&aucBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
			if(ucSettleFlag)
				memcpy(&aucBuf[8],"203",3);
			else
				memcpy(&aucBuf[8],"205",3);
			aucBuf[11]='5';
			aucBuf[12]='0';

		       ISO8583_SetBitValue(60,aucBuf,strlen((char*)aucBuf));

                     memset(aucBuf,0,sizeof(aucBuf));
			aucBuf[0]=6;
			aucBuf[1]=1;

			if(G_NORMALTRANS_ucCardType==TRANS_CARDTYPE_INTERNAL)
				memcpy(&aucBuf[2],"00",2);
			else
				memcpy(&aucBuf[2],"01",2);
			memcpy(&aucBuf[4],"00",2);

			long_asc(&aucBuf[6],12,&G_NORMALTRANS_ulAmount);
			memcpy(&aucBuf[18],"156",3);
			ISO8583_SetBitValue(62,aucBuf,21);

			ucResult = COMMS_GenSendReceive();
			if( ucResult == SUCCESS)
			{
				ucBatchSandTransNum++;
				uiNII = (DataSave0.TransInfoData.auiTransIndex[uiIndex]|TRANS_BATCHSEND);
				Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiIndex],
					sizeof(unsigned short));
				ucReFile = XDATA_Write_Vaild_File(DataSaveTransInfo);
				if (ucReFile != SUCCESS)
				{
					MSG_DisplayErrMsg(ucReFile);
					return(ERR_CANCEL);
				}
				ucTransNum++;
			}
		}
		if(ucResult) break;
	}
	*pucTransNum=ucTransNum;
	util_Printf("\n***********************Send Online END   ucResult=%02x***************************\n",ucResult);
	return(ucResult);
}



unsigned char SERV_ICInformTransSend(unsigned char ucSettleFlag,unsigned int *pucTransNum)
{
	unsigned char 	ucResult,ucReFile;
	unsigned short  uiTransIndex,uiNII,uiTotalNb;

	util_Printf("\n**************************************************\n");
	util_Printf("\n	   Send IC Inform Trans Trans");
	util_Printf("\n**************************************************\n");
	ucResult=SUCCESS;

	G_RUNDATA_ucTransType = TRANS_SETTLE;
	uiTotalNb=DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+DataSave0.TransInfoData.TransTotal.uiTotalNb;
	for(uiTransIndex=0;uiTransIndex<=uiTotalNb;uiTransIndex++)
       {
       //	//Os__clr_display(255);
	//	//Os__GB2312_display(0,0,(unsigned char*)"����IC��֪ͨ����");
		GetOldTransData(uiTransIndex);
		OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
						(unsigned char *)&NormalTransData,
						sizeof(NORMALTRANS));
		if(((DataSave1.SaveTrans.ucTransType==TRANS_REFUND)
			||(DataSave1.SaveTrans.ucTransType==TRANS_PREAUTHSETTLE))
		  &&(((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])&TRANS_BATCHSEND) != TRANS_BATCHSEND)
		  &&((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])!= 0 )
		  )
		{

			if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_MAG) continue;
			if( (DataSave0.ConstantParamData.ucInternalSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
			  )
			  {
			  		continue;
			  }
			if( (DataSave0.ConstantParamData.ucForgienSettleFlag == SETTLE_OK)
			  &&(G_NORMALTRANS_ucCardType != TRANS_CARDTYPE_INTERNAL)
			  )
			 {
			  		continue;
			 }
			G_EXTRATRANS_ucInputPINLen=0;
			ucResult = TRANS_Online();
			if( ucResult == SUCCESS)
			{
				ucBatchSandTransNum++;
				uiNII = DataSave0.TransInfoData.auiTransIndex[uiTransIndex]|TRANS_BATCHSEND;
				Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiTransIndex],
					sizeof(unsigned short));
				ucReFile=XDATA_Write_Vaild_File(DataSaveTransInfo);
				if (ucReFile != SUCCESS)
				{
					MSG_DisplayErrMsg(ucReFile);
					return(ERR_CANCEL);
				}
				Os__saved_copy((unsigned char *)&NormalTransData,
				(unsigned char *)&(DataSave1.SaveTrans),
				sizeof(NORMALTRANS));
				XDATA_Write_SaveTrans_File(uiTransIndex);
			}
			else
				break;


		}
	}

	return ucResult;

}


unsigned char SERV_SettleBatchEnd2(unsigned int ucSentTransNum)
{
	unsigned char ucResult;
	unsigned char ucContinueFlag;
	unsigned char aucBuf[500];
	unsigned char aucTmpBuf[12];
	unsigned short uiStartIndex;
	unsigned int uiTotalNb;
	BACKUPISO8583		ISO8583BACKUP;
	ucContinueFlag = true;

	uiStartIndex = 0;
	util_Printf("\n------------------���㲻ƽ------����������-------2----------------\n");
	G_RUNDATA_ucTransType = TRANS_SETTLE;
	/*������㲻ƽ���赥�������˻������߽��㣬�������
		��������ʱ��MSGID��Ϊ�µ�320
	*/
	uiTotalNb=ucSentTransNum;
	ucResult = SUCCESS;
	G_NORMALTRANS_ucTransType = TRANS_SETTLE;
	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
	//Os__clr_display(255);
	//Os__GB2312_display(1,0,(unsigned char *)"    ���㲻ƽ");
	//Os__GB2312_display(2,0,(unsigned char *)"   ���ڷ��ͽ���");
	//Os__GB2312_display(3,0,(unsigned char *)"     ���Ժ�");

	ISO8583_Clear();
	ISO8583_SetMsgID(320);

	/* Set system trace audit number bit 11 */
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucTmpBuf,0,sizeof(aucTmpBuf));
	long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
       ISO8583_SetBitValue(11,aucBuf,6);

  	/* Set Card Acceptor Terminal Identification bit 41 */
	ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);

	/* Set Card Acceptor Identification Code bit 42 */
	ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);

	/* Set bit 48 */
  	memset(aucBuf,'0',sizeof(aucBuf));
	memcpy(aucTmpBuf,"00",2);
	long_asc(&aucTmpBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
	ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
				(unsigned char *)&ISO8583BACKUP);
	//uiTotalNb+=ucBatchSandTransNum;
	ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
		(unsigned char *)&ISO8583BACKUP);


	util_Printf("\nuiTotalNb =%d",uiTotalNb);
	util_Printf("\nucBatchSandTransNum =%d",ucBatchSandTransNum);
	int_asc(aucBuf,4,&uiTotalNb);
	ISO8583_SetBitValue(48,aucBuf,4);
	memcpy(&aucTmpBuf[8],"2020",4);
	ucContinueFlag = false;
	ISO8583_SetBitValue(60,aucTmpBuf,11);
	ucResult = COMMS_GenSendReceive();
	/************************************************************************/
	if( (ucResult == SUCCESS)
	  ||(ucResult == ERR_HOSTCODE)
	  )
	{
		Os__saved_copy((unsigned char *)&uiTotalNb,
			(unsigned char *)&DataSave0.ChangeParamData.uiTotalNumber,
			sizeof(unsigned int));
		XDATA_Write_Vaild_File(DataSaveChange);
	}
	UTIL_IncreaseTraceNumber();
	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
	/************************************************************************/
	util_Printf("\n------------------���㲻ƽ------����������-------2----END------------\n");
	return(ucResult);
}

unsigned char SERV_Reprint(void)
{
	unsigned short  	uiIndex;
	unsigned char 	ucResult;
	unsigned int i,j;

	ucResult = UTIL_Is_Trans_Empty();

	if(ucResult != SUCCESS)
	{
		return ucResult;
	}
	uiIndex = DataSave0.ChangeParamData.uiLastTransIndex;

	if(ucResult == SUCCESS)
	{
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		{
			G_RUNDATA_ucReprintFlag = 1;
			GetOldTransData(uiIndex);
	       	OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
#ifdef GUI_PROJECT
	PRINT_Ticket(RE_PRINT_FLAG,2);
#else
			MSG_DisplayMenuMsg( true,0,0,MSG_PRINTING);
			if (DataSave0.ConstantParamData.prnflag)
			{
				for(i=0,j=0;i<PRINTTIMES;i++,j++)
				{
					if(j)
					{
						//Os__clr_display(255);
						//Os__GB2312_display(1,0,(uchar *)"    ��˺СƱ");
						//Os__GB2312_display(2,0,(uchar *)"�������������ӡ");
						MSG_WaitKey(5);
					}
					//Os__clr_display(255);
					//Os__GB2312_display(1,0,(uchar *)" ���ڴ�ӡ....");
					PRINT_Ticket(RE_PRINT_FLAG,j);
				}
			}
			else
			{
				PRINT_Ticket(RE_PRINT_FLAG,2);
			}
#endif			
		}else
		{
			MSG_DisplayMenuMsg( true,0,0,MSG_NOTRANS);
			ucResult = ERR_NOTRANS;
		}
	}
	return(ucResult);
}

unsigned char SERV_ReprintByTrace(void)
{
	unsigned char aucTraceNum[10],ucResult=SUCCESS;
	unsigned short uiIndex,i,j;
	unsigned long ulTraceNum;
	unsigned char ucTransNLen,aucTempBuf[6];

	ucResult = UTIL_Is_Trans_Empty();

	if(ucResult != SUCCESS)
	{
		return ucResult;
	}
	memset(aucTraceNum,0x00,sizeof(aucTraceNum));
#ifdef GUI_PROJECT
	memcpy(aucTraceNum, ProUiFace.UiToPro.aucOldTrace, strlen((char*) ProUiFace.UiToPro.aucOldTrace));
#else
	MSG_DisplayMenuMsg( true,0,0,MSG_TRACENUM);
	if( UTIL_Input(1,true,1,TRANS_TRACENUMLEN,'N',
				aucTraceNum,NULL) != KEY_ENTER )
	{
		return(ERR_CANCEL);
	}
#endif
	memset(aucTempBuf,0,sizeof(aucTempBuf));
        ucTransNLen = strlen((char*)aucTraceNum);
        memcpy(aucTempBuf,aucTraceNum,ucTransNLen);
        util_Printf("ԭƾ֤�ų���= %d\n",ucTransNLen);
        util_Printf("��[%d]��\n",(6-ucTransNLen));
        if(ucTransNLen < 6)
        {
            memset(&aucTraceNum[0],0x30,(6-ucTransNLen));
            memcpy(&aucTraceNum[6-ucTransNLen],aucTempBuf,ucTransNLen);
        }
        util_Printf("ԭƾ֤�� =%s\n",aucTraceNum);

	ulTraceNum=asc_long(aucTraceNum,6);
	for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
	{
	    util_Printf("����:%02x\n",DataSave0.TransInfoData.auiTransIndex[uiIndex]);
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		{
			GetOldTransData(uiIndex);
			OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
			if(ulTraceNum != DataSave1.SaveTrans.ulTraceNumber)
			{
				continue;
			}
			break;
		}
		else
		    return(MSG_NOTRANS);
	}
	if(uiIndex >= TRANS_MAXNB)
	{
		return(MSG_NOTRANS);
	}
	else
	{
#ifdef GUI_PROJECT
		PRINT_Ticket(RE_PRINT_FLAG,2);
#else
		MSG_DisplayMenuMsg( true,0,0,MSG_PRINTING);
		//PRINT_Ticket(RE_PRINT_FLAG,2);
			if (DataSave0.ConstantParamData.prnflag)
			{
				for(i=0,j=0;i<PRINTTIMES;i++,j++)
				{
					if(j)
					{
						//Os__clr_display(255);
						//Os__GB2312_display(1,0,(uchar *)"    ��˺СƱ");
						//Os__GB2312_display(2,0,(uchar *)"�������������ӡ");
						MSG_WaitKey(5);
					}
					//Os__clr_display(255);
					//Os__GB2312_display(1,0,(uchar *)" ���ڴ�ӡ....");
					PRINT_Ticket(RE_PRINT_FLAG,j);
				}
			}
			else
			{
				PRINT_Ticket(RE_PRINT_FLAG,2);
			}
#endif
			
	}


	return ucResult;


}


unsigned char SERV_ReprintSettle(void)
{
	unsigned char ucResult;
	TRANSTOTAL	*pTransTotal;
	TRANSTOTAL	*pForeignTransTotal;


#ifdef GUI_PROJECT		
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=2;
	memcpy(ProUiFace.ProToUi.aucLine1,"���׳ɹ�",14);
	memcpy(ProUiFace.ProToUi.aucLine2,"���ڴ�ӡ...",13);
	if(!g_ThreadFlag)
		sleep(1);
	else{
    sem_post(&binSem1);
    sem_wait(&binSem2);
  }
#endif

	pTransTotal = &DataSave0.TransInfoData.TransTotal;
	pForeignTransTotal = &DataSave0.TransInfoData.ForeignTransTotal;

	//Os__clr_display(255);
	//Os__GB2312_display(1,0,(uchar*)"��[ȷ��]��ʼ��ӡ");
	//Os__GB2312_display(2,0,(uchar*)"     ��ȷ��");

	ucResult = SUCCESS;

//	if(Os__xget_key() != KEY_ENTER)
//	{
//		return ERR_CANCEL;
//	}

	if(!ucResult)
	{
		PRINT_List(PRINT_TOTAL_FLAG|PRINT_SETTLE_FLAG);
	}

	if(DataSave0.ConstantParamData.prnflagOld != 1
		||DataSave0.ConstantParamData.prnflagZD != 1)
		Os__xlinefeed(LINENUM*1);

	if(DataSave0.ConstantParamData.prnflag != 0
		&&DataSave0.ConstantParamData.prnflag != 0
		&&DataSave0.ConstantParamData.prnflag != 0)
		Os__xlinefeed(LINENUM*1);
	if(DataSave0.ConstantParamData.prnflagZD)
		Os__xlinefeed(LINENUM*1);
	return SUCCESS;
}

unsigned char SERV_TransList(unsigned char flag)
{
	unsigned char aucbuf[25];
	unsigned char aucBufCard[20];
	unsigned int ucI;

	memset(aucBufCard,0,sizeof(aucBufCard));
	if(G_NORMALTRANS_ucSourceAccLen%2)
	{
		bcd_asc(&aucBufCard[0],&G_NORMALTRANS_aucSourceAcc[0],G_NORMALTRANS_ucSourceAccLen+1);
	}else
	{
		bcd_asc(&aucBufCard[0],&G_NORMALTRANS_aucSourceAcc[0],G_NORMALTRANS_ucSourceAccLen);
	}
	aucBufCard[G_NORMALTRANS_ucSourceAccLen]=0x00;
	util_Printf("List=%02x\n",DataSave0.ConstantParamData.ucPBeforeSix);
	if (DataSave0.ConstantParamData.ucPrintCardNo==0x31)
	{
		if ((G_NORMALTRANS_ucTransType != TRANS_PREAUTH)&&
			(G_NORMALTRANS_ucTransType != TRANS_TIPADJUST) &&
			(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)&&
			(G_NORMALTRANS_ucTransType != TRANS_UNDOOFF)&&
                    	(G_NORMALTRANS_ucTransType != TRANS_EC_UNASSIGNLOAD)&&
                    	(G_NORMALTRANS_ucECTrans !=TRANS_EC_PURCHASE)
			)
		{
			if (DataSave0.ConstantParamData.ucPBeforeSix==0x30)
			{
				for(ucI=6;ucI<G_NORMALTRANS_ucSourceAccLen-4;ucI++)
				{
					aucBufCard[ucI]='*';
				}
			}
			if (DataSave0.ConstantParamData.ucPBeforeSix==0x31)
			{
				for(ucI=0;ucI<G_NORMALTRANS_ucSourceAccLen-4;ucI++)
				{
					aucBufCard[ucI]='*';
				}
			}
		}
	}
#ifdef GUI_PROJECT
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=4;
	memcpy(ProUiFace.ProToUi.aucLine1,aucBufCard,strlen((char*)aucBufCard));
#else
	//Os__clr_display(255);
	if(G_NORMALTRANS_ucSourceAccLen>16)
		OSMMI_DisplayASCII(0x30,0,0, aucBufCard);
	else
		Os__display(0,0,aucBufCard);
#endif
	memset(aucbuf,' ',sizeof(aucbuf));
	if(G_NORMALTRANS_ucTransType!=TRANS_UNDOOFF)
		UTIL_Form_Montant(&aucbuf[0],G_NORMALTRANS_ulAmount+G_NORMALTRANS_ulFeeAmount,2);
	else if((G_NORMALTRANS_ucTransType==TRANS_UNDOOFF)
		&&(G_NORMALTRANS_ucOldTransType!=TRANS_OFFPREAUTHFINISH)
		)
		UTIL_Form_Montant(&aucbuf[0],G_NORMALTRANS_ulFeeAmount,2);
	else
		UTIL_Form_Montant(&aucbuf[0],G_NORMALTRANS_ulAmount,2);
	
#ifdef GUI_PROJECT
	memcpy(ProUiFace.ProToUi.aucLine2,"���:",5);
	memcpy(&ProUiFace.ProToUi.aucLine2[5],aucbuf,strlen((char*)aucbuf));
#else
	Os__display(1,4,aucbuf);
	//Os__GB2312_display(1, 0, (uchar * )"���");
#endif

	memset(aucbuf,0x00,sizeof(aucbuf));
	switch( G_NORMALTRANS_ucTransType )
	{
		case TRANS_PURCHASE:
			if((G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_HB_PUTCARD)
				||(G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_SAND_PUTCARD))
//				//Os__GB2312_display(2, 0, (uchar * )" (qPBOC����)");
				memcpy(aucbuf, (uchar * )" (qPBOC����)",strlen((char*)" (qPBOC����)"));

			else if(G_NORMALTRANS_ucECTrans ==TRANS_EC_PURCHASE)
//				//Os__GB2312_display(2, 0, (uchar * )" (EC����)");
				memcpy(aucbuf, (uchar * )" (EC����)",strlen((char*)" (EC����)"));
			else
				////Os__GB2312_display(2, 0, (uchar * )" (����)");
				memcpy(aucbuf, (uchar * )" (����)",strlen((char*)" (����)"));
			break;
		case TRANS_CUPMOBILE:
			////Os__GB2312_display(2, 0, (uchar * )" (�ƶ�֧������)");
				memcpy(aucbuf, (uchar * )" (�ƶ�֧������)",strlen((char*)" (�ƶ�֧������)"));
   			break;
		case TRANS_VOIDPURCHASE:
			////Os__GB2312_display(2, 0, (uchar * )" (���ѳ���)");
				memcpy(aucbuf, (uchar * )" (���ѳ���)",strlen((char*)" (���ѳ���)"));
			break;
		case TRANS_VOIDCUPMOBILE:
			////Os__GB2312_display(2, 0, (uchar * )" (�ƶ�֧������)");
				memcpy(aucbuf, (uchar * )" (�ƶ�֧������)",strlen((char*)" (�ƶ�֧������)"));
   			break;
		case TRANS_UNDOOFF:
			////Os__GB2312_display(2, 0, (uchar * )" (���߳���)");
				memcpy(aucbuf, (uchar * )" (���߳���)",strlen((char*)" (���߳���)"));
			break;
		case TRANS_PREAUTH:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ)",strlen((char*)"(Ԥ��Ȩ)"));
   			break;
   		case TRANS_VOIDPREAUTH:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ����)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ����)",strlen((char*)"(Ԥ��Ȩ����)"));
			break;
		case TRANS_VOIDPREAUTHFINISH:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ��ɳ���)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ��ɳ���)",strlen((char*)"(Ԥ��Ȩ��ɳ���)"));
			break;
		case TRANS_PREAUTHFINISH:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ�������)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ�������)",strlen((char*)"(Ԥ��Ȩ�������)"));
			break;
		case TRANS_REFUND:
			////Os__GB2312_display(2, 0, (uchar * )" (�˻�)");
				memcpy(aucbuf, (uchar * )" (�˻�)",strlen((char*)" (�˻�)"));
			break;
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
			////Os__GB2312_display(2, 0, (uchar * )" (���ߵ���)");
				memcpy(aucbuf, (uchar * )" (���ߵ���)",strlen((char*)" (���ߵ���)"));
			break;
		case TRANS_OFFPREAUTHFINISH:
			////Os__GB2312_display(2, 0, (uchar * )" (���߽���)");
				memcpy(aucbuf, (uchar * )" (���߽���)",strlen((char*)" (���߽���)"));
			break;
		case TRANS_PREAUTHSETTLE:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ�������)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ�������)",strlen((char*)"(Ԥ��Ȩ�������)"));
			break;
		case TRANS_OFFPURCHASE:
			if((G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_HB_PUTCARD)
				||(G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_SAND_PUTCARD))
				////Os__GB2312_display(2, 0, (uchar * )" (qPBOC�ѻ�����)");
				memcpy(aucbuf, (uchar * )" (qPBOC�ѻ�����)",strlen((char*)" (qPBOC�ѻ�����)"));
			else if(G_NORMALTRANS_ucECTrans ==TRANS_EC_PURCHASE)
				////Os__GB2312_display(2, 0, (uchar * )" (EC�ѻ�����)");
				memcpy(aucbuf, (uchar * )" (EC�ѻ�����)",strlen((char*)" (EC�ѻ�����)"));
			else
				////Os__GB2312_display(2, 0, (uchar * )" (�ѻ�����)");
				memcpy(aucbuf, (uchar * )" (�ѻ�����)",strlen((char*)" (�ѻ�����)"));
			break;
		case TRANS_VOIDOFFPURCHASE:
			////Os__GB2312_display(2, 0, (uchar * )" (�ѻ����ѳ���)");
				memcpy(aucbuf, (uchar * )" (�ѻ����ѳ���)",strlen((char*)" (�ѻ����ѳ���)"));
			break;
		case TRANS_QUERYBAL:
			////Os__GB2312_display(2, 0, (uchar * )" (����ѯ)");
				memcpy(aucbuf, (uchar * )" (����ѯ)",strlen((char*)" (����ѯ)"));
			break;
		case TRANS_PREAUTHADD:
			////Os__GB2312_display(2, 0, (uchar * )"(Ԥ��Ȩ׷��)");
				memcpy(aucbuf, (uchar * )"(Ԥ��Ȩ׷��)",strlen((char*)"(Ԥ��Ȩ׷��)"));
			break;
		case TRANS_EC_CASHLOAD:
			////Os__GB2312_display(2, 0, (uchar * )"(�ֽ�Ȧ��)");
				memcpy(aucbuf, (uchar * )"(�ֽ�Ȧ��)",strlen((char*)"(�ֽ�Ȧ��)"));
			break;
		case TRANS_EC_ASSIGNLOAD:
			////Os__GB2312_display(2, 0, (uchar * )"(ָ���˻�Ȧ��)");
				memcpy(aucbuf, (uchar * )"(ָ���˻�Ȧ��)",strlen((char*)"(ָ���˻�Ȧ��)"));
			break;
		case TRANS_EC_UNASSIGNLOAD:
			////Os__GB2312_display(2, 0, (uchar * )"(��ָ���˻�Ȧ��)");
				memcpy(aucbuf, (uchar * )"(��ָ���˻�Ȧ��)",strlen((char*)"(��ָ���˻�Ȧ��)"));
			break;
		case TRANS_MOBILEAUTH:
			////Os__GB2312_display(2, 0, (uchar * )"�ֻ��޿�ԤԼ����");
				memcpy(aucbuf, (uchar * )"�ֻ��޿�ԤԼ����",strlen((char*)"�ֻ��޿�ԤԼ����"));
			break;
		case TRANS_VOIDMOBILEAUTH:
			////Os__GB2312_display(2, 0, (uchar * )"�ֻ�ԤԼ����");
				memcpy(aucbuf, (uchar * )"�ֻ�ԤԼ����",strlen((char*)"�ֻ�ԤԼ����"));
			break;
		case TRANS_MOBILEREFUND:
			////Os__GB2312_display(2, 0, (uchar * )"�ֻ�ԤԼ�˻�");
				memcpy(aucbuf, (uchar * )"�ֻ�ԤԼ�˻�",strlen((char*)"�ֻ�ԤԼ�˻�"));
			break;
		case TRANS_CREDITSALE:
			////Os__GB2312_display(2, 0, (uchar * )"(���ڸ�������)");
				memcpy(aucbuf, (uchar * )"(���ڸ�������)",strlen((char*)"(���ڸ�������)"));
			break;
		case TRANS_CREDITSVOID:
			////Os__GB2312_display(2, 0, (uchar * )"(���ڸ����)");
				memcpy(aucbuf, (uchar * )"(���ڸ����)",strlen((char*)"(���ڸ����)"));
			break;
		case TRANS_CREDITSREFUND:
			////Os__GB2312_display(2, 0, (uchar * )"(���ڸ����˻�)");
				memcpy(aucbuf, (uchar * )"(���ڸ����˻�)",strlen((char*)"(���ڸ����˻�)"));
			break;
		default:
			////Os__GB2312_display(2, 0, (uchar * )" δ֪��������");
				memcpy(aucbuf, (uchar * )" δ֪��������",strlen((char*)" δ֪��������"));
			break;
	}
#ifdef GUI_PROJECT
	memcpy(&ProUiFace.ProToUi.aucLine3[0],aucbuf,strlen((char*)aucbuf));
#else
	//Os__GB2312_display(2, 0, (uchar * )aucbuf);
#endif
 
#ifdef GUI_PROJECT
	memset(ProUiFace.ProToUi.aucLine4,0x20,sizeof(ProUiFace.ProToUi.aucLine2));
	memcpy(&ProUiFace.ProToUi.aucLine4[0],"ƾ֤��:",7);
#else
	//Os__GB2312_display(2, 0, (uchar * )"ƾ֤��:");
#endif

	memset(aucbuf,' ',sizeof(aucbuf));
	long_asc(&aucbuf[0],6,&G_NORMALTRANS_ulTraceNumber);
	aucbuf[6] = 0;
#ifdef GUI_PROJECT
	memcpy(&ProUiFace.ProToUi.aucLine4[7],aucbuf,6);
#else
	Os__display(3,7,aucbuf);
#endif

	if (flag)
	{
		aucbuf[0] = '*';
		aucbuf[1] = 0;
#ifdef GUI_PROJECT
		ProUiFace.ProToUi.aucLine4[16]='*';
#else
		Os__display(3,15,aucbuf);
#endif
	}
	
#ifdef GUI_PROJECT
sleep(1);
#endif	
	return(SUCCESS);
}

unsigned char SERV_SettleBatchEnd(unsigned char ucSettleFlag,unsigned int ucTransNum)
{
	unsigned char ucResult;
	unsigned char aucBuf[100],aucTmpBuf[100];
       unsigned int uiTotalNb;

	uiTotalNb =ucTransNum;

	G_RUNDATA_ucTransType = TRANS_SETTLE;
	ucResult = SUCCESS;

//	//Os__clr_display(255);
//	//Os__GB2312_display(1,0,(unsigned char *)"    ���㲻ƽ");
//	//Os__GB2312_display(2,0,(unsigned char *)"   ����������");
//	//Os__GB2312_display(3,0,(unsigned char *)"     ���Ժ�");
	//util_Printf("\n------------------���㲻ƽ------����������------1----SERV_SettleBatchEnd-------------\n");
	ISO8583_Clear();
	ISO8583_SetMsgID(320);
	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
	/* Set system trace audit number bit 11 */
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucTmpBuf,0,sizeof(aucTmpBuf));
	long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
       ISO8583_SetBitValue(11,aucBuf,6);
  	/* Set Card Acceptor Terminal Identification bit 41 */
	ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
	/* Set Card Acceptor Identification Code bit 42 */
	ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);

	memcpy(aucTmpBuf,"00",2);
	long_asc(&aucTmpBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);

	int_asc(aucBuf,4,&uiTotalNb);
	ISO8583_SetBitValue(48,aucBuf,4);
	memcpy(&aucTmpBuf[8],"2070",4);

	ISO8583_SetBitValue(60,aucTmpBuf,11);
	ucResult = COMMS_GenSendReceive();

	return(ucResult);
}

unsigned char SERV_DispTotal(void)
{
	unsigned char aucBuf[40],ucResult;
	unsigned long ultotalamount;
	unsigned int uitotalnb;
	unsigned char ucKey;
	TRANSTOTAL	*pTransTotal;
	TRANSTOTAL	*pForeignTransTotal;

util_Printf("\n 1. SERV_DispTotal: --------------------------\n");
	XDATA_Read_SaveTrans_File(DataSaveTransInfo);
	
	ucResult = UTIL_Is_Trans_Empty();
	
	util_Printf("\n 2. SERV_DispTotal: --------------------------[%02x]\n",ucResult);
	
	if(ucResult != SUCCESS)
	{
   		MSG_DisplayMsgKey( true,1,2,MSG_NOTRANS,3);
		return(SUCCESS);
	}

util_Printf("\n 2. SERV_DispTotal: --------------------------\n");

	pTransTotal = &DataSave0.TransInfoData.TransTotal;
	pForeignTransTotal = &DataSave0.TransInfoData.ForeignTransTotal;
#ifdef GUI_PROJECT
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=8;
	memcpy(ProUiFace.ProToUi.aucLine1,"------�ڿ�------",16);
	memcpy(ProUiFace.ProToUi.aucLine2,"����",4);
#else
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
	//Os__GB2312_display(1,0,(unsigned char *)"����");
#endif
	uitotalnb = pTransTotal->uiPurchaseNb;
	ultotalamount = pTransTotal->ulPurchaseAmount;
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,' ',16);
	memcpy(aucBuf,"����:",5);
	int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT
	memcpy(ProUiFace.ProToUi.aucLine3,aucBuf,strlen((char*)aucBuf));
#else
	//Os__GB2312_display(2,0, aucBuf);
#endif
util_Printf("\n 3. SERV_DispTotal: --------------------------\n");
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,' ',16);
	memcpy(aucBuf,"���:",5);
	UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT
	memcpy(ProUiFace.ProToUi.aucLine4,aucBuf,strlen((char*)aucBuf));
#else
	//Os__GB2312_display(3,0, aucBuf);
#endif
	
//add
//	ucKey = Os__xget_key();//---------------------------------------------------
//	if (ucKey == KEY_ENTER)
//	{
//		//Os__clr_display(255);
//		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
//		//Os__GB2312_display(1,0,(unsigned char *)"Ԥ��Ȩ");
//		uitotalnb = pTransTotal->uiAuthNb;
//		ultotalamount = pTransTotal->ulAuthAmount;
//		memset(aucBuf,0,sizeof(aucBuf));
//		memset(aucBuf,' ',16);
//		memcpy(aucBuf,"����:",5);
//		int_asc(&aucBuf[5],3,&uitotalnb);
//		//Os__GB2312_display(2,0, aucBuf);
//		memset(aucBuf,0,sizeof(aucBuf));
//		memset(aucBuf,' ',16);
//		memcpy(aucBuf,"���:",5);
//		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
//		//Os__GB2312_display(3,0,aucBuf,FONT);
//	}
//	else
//		return(ucKey);
//end
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
util_Printf("\n 4. SERV_DispTotal: --------------------------\n");
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT
		memcpy(ProUiFace.ProToUi.aucLine5,"Ԥ��Ȩ���(����)",strlen((char*)"Ԥ��Ȩ���(����)"));
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
		//Os__GB2312_display(1,0, (unsigned char *)"Ԥ��Ȩ���(����)");
#endif
		
		uitotalnb = pTransTotal->uiAuthFinishNb;
		ultotalamount = pTransTotal->ulAuthFinishAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT
		memcpy(ProUiFace.ProToUi.aucLine6,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,' ',sizeof(aucBuf));
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT
		memcpy(ProUiFace.ProToUi.aucLine7,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(3,0, aucBuf);
#endif
		
	}
	else
		return (ucKey);

util_Printf("\n 5. SERV_DispTotal: --------------------------\n");
		
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif

	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
//	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
//	ProUiFace.ProToUi.uiLines=8;
	memcpy(ProUiFace.ProToUi.aucLine8,"Ԥ��Ȩ���(����)",16);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
		//Os__GB2312_display(1,0,(unsigned char *)"Ԥ��Ȩ���(����)");
#endif
		
		uitotalnb = pTransTotal->uiPreauthSettleNb;
		ultotalamount = pTransTotal->ulPreauthSettleAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine9,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,' ',sizeof(aucBuf));
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine10,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,(uchar *)aucBuf);
#endif
		
	}
	else
		return(ucKey);

util_Printf("\n 6. SERV_DispTotal: --------------------------\n");
		
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine11,"����:",5);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
		//Os__GB2312_display(1,0,(unsigned char *)"����:");
#endif
	
		uitotalnb = pTransTotal->uiOfflineNb;
		ultotalamount = pTransTotal->ulOfflineAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine12,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(2,0,(uchar *)aucBuf);
#endif
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',sizeof(aucBuf));
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine13,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,(uchar *)aucBuf);
#endif
	}else
		return (ucKey);
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine14,"�˻�:",5);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
		//Os__GB2312_display(1,0,(unsigned char *)"�˻�:");
#endif
		uitotalnb = pTransTotal->uiRefundNb;
		ultotalamount = pTransTotal->ulRefundAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
		//Os__GB2312_display(2,0, aucBuf);
#ifdef GUI_PROJECT_tts
		memcpy(ProUiFace.ProToUi.aucLine15,aucBuf,strlen((char*)aucBuf));
#else
       	//Os__GB2312_display(2,0,(uchar *)aucBuf);
#endif
		
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
        //Os__GB2312_display(3,0,aucBuf);
#ifdef GUI_PROJECT_tts
	//memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	//ProUiFace.ProToUi.uiLines=8;
	memcpy(ProUiFace.ProToUi.aucLine16,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,(uchar *)aucBuf);
#endif
		
	}else
		return (ucKey);

util_Printf("\n 7. SERV_DispTotal: --------------------------\n");
		
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine17,"�ֽ�Ȧ��:",9);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�ڿ�------");
		//Os__GB2312_display(1,0,(unsigned char *)"�ֽ�Ȧ��:");
#endif
	
		uitotalnb = pTransTotal->uiCashLoadNb;
		ultotalamount = pTransTotal->uiCashLoadAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine18,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine19,aucBuf,strlen((char*)aucBuf));
#else
        	//Os__GB2312_display(3,0,(uchar *)aucBuf);
#endif
			
	}else
		return (ucKey);
	
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine20,"------�⿨------",16);
	memcpy(ProUiFace.ProToUi.aucLine21,"����",4);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
		//Os__GB2312_display(1,0,(unsigned char *)"����");
#endif
	
		uitotalnb = pForeignTransTotal->uiPurchaseNb;
		ultotalamount = pForeignTransTotal->ulPurchaseAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine22,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine23,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,aucBuf);
#endif
		
	}else
		return (ucKey);

util_Printf("\n 8. SERV_DispTotal: --------------------------\n");
		
//add
//	ucKey = Os__xget_key();//---------------------------------------------------
//	if (ucKey == KEY_ENTER)
//	{
//		//Os__clr_display(255);
//		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
//		//Os__GB2312_display(1,0,(unsigned char *)"Ԥ��Ȩ");
//		uitotalnb = pForeignTransTotal->uiAuthNb;
//		ultotalamount = pForeignTransTotal->ulAuthAmount;
//		memset(aucBuf,0,sizeof(aucBuf));
//		memset(aucBuf,' ',16);
//		memcpy(aucBuf,"����:",5);
//		int_asc(&aucBuf[5],3,&uitotalnb);
//		//Os__GB2312_display(2,0, aucBuf);
//		memset(aucBuf,0,sizeof(aucBuf));
//		memset(aucBuf,' ',16);
//		memcpy(aucBuf,"���:",5);
//		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
//		//Os__GB2312_display(3,0,aucBuf,FONT);
//	}else
//		return (ucKey);

//end
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	//memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	//ProUiFace.ProToUi.uiLines=8;
	memcpy(ProUiFace.ProToUi.aucLine24,"Ԥ��Ȩ���(����)",16);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
		//Os__GB2312_display(1,0,(unsigned char *)"Ԥ��Ȩ���(����)");
#endif
		uitotalnb = pForeignTransTotal->uiAuthFinishNb;
		ultotalamount = pForeignTransTotal->ulAuthFinishAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);

#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine25,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine26,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,aucBuf);
#endif
		
	}else
			return (ucKey);
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine27,"Ԥ��Ȩ���(����)",16);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
        	//Os__GB2312_display(1,0,(unsigned char *)"Ԥ��Ȩ���(����)");
#endif
	
		uitotalnb = pForeignTransTotal->uiPreauthSettleNb;
		ultotalamount = pForeignTransTotal->ulPreauthSettleAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine28,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine29,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,aucBuf);
#endif
		
	}else
			return (ucKey);
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine30,"����:",5);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
        	//Os__GB2312_display(1,0,(unsigned char *)"����:");
#endif
	
		uitotalnb = pForeignTransTotal->uiOfflineNb;
		ultotalamount = pForeignTransTotal->ulOfflineAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine31,aucBuf,strlen((char*)aucBuf));
//sleep(1);
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
// memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
// ProUiFace.ProToUi.uiLines=8;
	memcpy(ProUiFace.ProToUi.aucLine32,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,aucBuf);
#endif
		
	}
	else
		return (ucKey);
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
#else
	ucKey = UTIL_WaitGetKey(10);
#endif
	if (ucKey == KEY_ENTER)
	{
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine33,"���߳���:",9);
#else
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
        	//Os__GB2312_display(1,0,(unsigned char *)"���߳���:");
#endif

		uitotalnb = pForeignTransTotal->uiUndoOffSum;
		ultotalamount = pForeignTransTotal->ulUndoOffAmount;
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"����:",5);
		int_asc(&aucBuf[5],3,&uitotalnb);

#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine34,aucBuf,strlen((char*)aucBuf));
#else
		//Os__GB2312_display(2,0, aucBuf);
#endif

		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucBuf,' ',16);
		memcpy(aucBuf,"���:",5);
		UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
#ifdef GUI_PROJECT_tts
	memcpy(ProUiFace.ProToUi.aucLine35,aucBuf,strlen((char*)aucBuf));
#else
        //Os__GB2312_display(3,0,aucBuf);
#endif
	}
#ifdef GUI_PROJECT_tts
	ucKey = KEY_ENTER;
//sleep(1);
#else
	ucKey = UTIL_WaitGetKey(10);
#endif

util_Printf("\n 9. SERV_DispTotal: --------------------------\n");

	return (ucKey);
	//---------------------------------------------------
   /*
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"------�⿨------");
	//Os__GB2312_display(1,0,(unsigned char *)"�˻�:",FONT);
	uitotalnb = pForeignTransTotal->uiRefundNb;
	ultotalamount = pForeignTransTotal->ulRefundAmount;
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,' ',16);
	memcpy(aucBuf,"����:",5);
	int_asc(&aucBuf[5],3,&uitotalnb);
	//Os__GB2312_display(2,0, aucBuf);
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,' ',16);
	memcpy(aucBuf,"���:",5);
	UTIL_Form_Montant(&aucBuf[4],ultotalamount,2);
	//Os__GB2312_display(3,0,aucBuf,FONT);

	Os__xget_key();//---------------------------------------------------
	*/

}