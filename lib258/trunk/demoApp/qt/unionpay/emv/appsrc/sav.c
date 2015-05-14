/*
	SAND China
--------------------------------------------------------------------------
	FILE  sav.c									(Copyright SAND 2001)
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2001-07-02		Xiaoxi Jiang
    Last modified :	2001-07-02		Xiaoxi Jiang
    Module :  Shanghai Golden Card Network
    Purpose :
        Source file of save transaction.

    List of routines in file :

    File history :

*/

#include <include.h>
#include <global.h>
#include <xdata.h>

unsigned char SAV_TransSave(void)
{
	unsigned short uiNII;
	unsigned char ucResult;
	UINT_C51	uiTotalNb,uiUndoOffSum;
	ULONG_C51	ulTotalAmount,ulAmount ,ulTotalTrueAmount,ulTotalTIPAmount,ulUndoOffAmount;
	TRANSTOTAL	*pTransTotal;
	UINT_C51	uiTempRate;
	ULONG_C51	ulOldTrueAmount;
	unsigned short	uiTransIndex;
	NORMALTRANS OldNormalTrans;

	//�������ʶ
    G_RUNDATA_ucReversalFlag = 0x30;
	Os__saved_copy((unsigned char *)"96",	(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucRevealEvent),2); /*POS�յ����׵��޷����*/

	/*******************************************	
	*G_RUNDATA_bConectFlag ��ʾ�����Ѿ�����
	*�ҳɹ��ؽ��뽻�ױ��溯����˵���˱ʽ����Ѿ�ͨ����֤
	* �ʿ��ж������������Ӱ����ڳ���ʱ�������ѻ����ף��ѻ�����ʱG_RUNDATA_bConnectFlag��ֵΪfalse
	********************************************
	*/
	if (((G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
		&&(G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
		&&(G_NORMALTRANS_ucTransType != TRANS_UNDOOFF)
        &&(G_NORMALTRANS_ucTransType != TRANS_OFFPURCHASE))
		||(G_RUNDATA_bConectFlag))
	{
		memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));
		memset(&DataSave0.Trans_8583Data.SendISO8583Data,0x00,sizeof(BACKUPISO8583));
		memset(&DataSave0.Trans_8583Data.SaveISO8583Data,0x00,sizeof(BACKUPISO8583));
	}
	ucResult=XDATA_Write_Vaild_File(DataSaveTrans8583);
    util_Printf("\n�洢���׺������־= %02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
	if (ucResult)   return(ucResult);

	util_Printf("���� =%02x\n",G_NORMALTRANS_ucCardType);
	switch(G_NORMALTRANS_ucCardType)
	{
		case 0:		//RMB
			uiTempRate = DataSave0.ConstantParamData.ulRMBRate;
			break;
		case 1:		//VISA
			uiTempRate = DataSave0.ConstantParamData.ulVISARate;
			break;
		case 2:		//Master Card
			uiTempRate = DataSave0.ConstantParamData.ulMASTERRate;
			break;
		case 3:		//JCB
			uiTempRate = DataSave0.ConstantParamData.ulJCBRate;
			break;
		case 4:		//Dinner Club
			uiTempRate = DataSave0.ConstantParamData.ulDINERRate;
			break;
		case 5:		//America Express
			uiTempRate = DataSave0.ConstantParamData.ulAMEXRate;
			break;
	}

	if(  (G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
	   ||(G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNALSMART)
	  )
	{
		pTransTotal = &DataSave0.TransInfoData.TransTotal;
	}
	else
	{
		pTransTotal = &DataSave0.TransInfoData.ForeignTransTotal;
	}
	if(G_NORMALTRANS_ucTransType != TRANS_UNDOOFF)
	{
		pTransTotal->uiTotalNb++;
	}

	util_Printf("�����ܱ���:%d\n",pTransTotal->uiTotalNb);
	util_Printf("�������� =%02x\n",G_NORMALTRANS_ucTransType);

	switch(G_NORMALTRANS_ucTransType)/*��������*/
	{
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDOFFPURCHASE:
		case TRANS_VOIDPREAUTHFINISH:
		/*����Ϊ���ͽ���*/
			uiNII = DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex]|TRANS_NIIVOID;
			Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex],
					sizeof(unsigned short));
			uiNII = TRANS_NIIVOID;		/*�²����Ľ���*/
			Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiTransIndex],
					sizeof(unsigned short));
//			ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);
//			if (ucResult)   return(ucResult);
			break;
		case TRANS_UNDOOFF:
		case TRANS_TIPADJUST:		/*��������Ľ���*/
			break;
		default:
			uiNII = TRANS_NIINORMAL;	/*����ɵ��½���*/
			Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiTransIndex],
					sizeof(unsigned short));
//			ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);
//			if (ucResult)   return(ucResult);
			break;
	}

	if (G_RUNDATA_ucPTicketFlag==1
            &&G_NORMALTRANS_ucTransType==TRANS_OFFPURCHASE)
            G_NORMALTRANS_ucPTicketFlag=1;          //��ʶPBOC�Ƿ��ѿ���Ʊ
        else
            G_NORMALTRANS_ucPTicketFlag=0;

	switch(G_NORMALTRANS_ucTransType)/*�������ʣ�*/
	{
		case TRANS_CREDITSALE:
		case TRANS_PURCHASE:
		case TRANS_CUPMOBILE:					//changed 09-1-6
		case TRANS_PREAUTHFINISH:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_OFFPURCHASE:
		case TRANS_MOBILEAUTH:
			G_NORMALTRANS_ulTrueAmount = (G_NORMALTRANS_ulAmount/10000)*(10000-uiTempRate)
	    			    			 +((G_NORMALTRANS_ulAmount%10000)*(10000-uiTempRate)/1000+5)/10;
			break;
		case TRANS_REFUND:
		case TRANS_UNDOOFF:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDOFFPURCHASE:
		case TRANS_VOIDPREAUTHFINISH:
			break;
		default:
			break;
	}
	//if(G_NORMALTRANS_ucTransType != TRANS_ACCOUNTS)/*���潻��*/
	{
    	if((G_NORMALTRANS_ucTransType !=TRANS_TIPADJUST)
    		&&(G_NORMALTRANS_ucTransType !=TRANS_UNDOOFF)
    		)
    	{
    		/*�²����Ľ���*/
    		OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
    		ucResult = XDATA_Write_SaveTrans_File(G_RUNDATA_uiTransIndex);
    		if (ucResult)   return(ucResult);
    		/*�����ָ��Ǻʹ洢���ַ�ʽ*/
    	}
    	else if (G_NORMALTRANS_ucTransType == TRANS_TIPADJUST)
    	{
    		/*��־�ý����ѵ��������������ڶ��ε���*/
    		G_NORMALTRANS_ucTIPAdjustFlag = 1;
   			/*ԭ���ײ����ٵ���*/
    		DataSave1.SaveTrans.ucTIPAdjustFlag = 1;
    		util_Printf("Saving = %02x\n",G_RUNDATA_ucTIPSaving);
    		if(G_RUNDATA_ucTIPSaving == 0)
    		{
    			/*����ԭ����*/
    			pTransTotal->uiTotalNb--;	/*�����ϵı�����ȥ*/
    			ulAmount = G_NORMALTRANS_ulFeeAmount;
    			G_NORMALTRANS_ulFeeAmount = 0;
    			G_NORMALTRANS_ucTransType = TRANS_OFFPREAUTHFINISH;
    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
    			ucResult=XDATA_Write_SaveTrans_File(G_RUNDATA_uiOldTransIndex);
    			if (ucResult)
    			    return(ucResult);

    			/*�����ܴ洢��������*/
    			G_RUNDATA_uiTransIndex = G_RUNDATA_uiOldTransIndex;

    			/*ԭ�������ͱ��ֲ���*/
    		       //DataSave1.SaveTrans.ucTransType = TRANS_OFFPREAUTHFINISH;

    			/*���ǵ����ֽ��׵������ڽ�����ϸ��Ҫ���ӡ*/
    			DataSave1.SaveTrans.ucTIPAdjustFlag = 2;
    			G_NORMALTRANS_ulFeeAmount = ulAmount;
    			G_NORMALTRANS_ucTransType = TRANS_TIPADJUST;//changed LOTE 2008-02-22
    		}else
    		{
#if PS300
    			memset( (unsigned char *)&OldNormalTrans, 0, sizeof(NORMALTRANS));
    			GetOldTransData_O(G_RUNDATA_uiOldTransIndex,&OldNormalTrans);
    			OldNormalTrans.ucTIPCode = 1;			//��־ԭ����������С�ѵ���
    			OldNormalTrans.ucTIPAdjustFlag = 1;
    			XDATA_Write_SaveTrans_File_O(G_RUNDATA_uiOldTransIndex ,&OldNormalTrans);

    			util_Printf("С�ѵ�����־3:%02x\n",OldNormalTrans.ucTIPCode);
#endif
#if PS100|PS400
				memset( (unsigned char *)&OldNormalTrans, 0, sizeof(NORMALTRANS));
     			GetOldTransData_O(G_RUNDATA_uiOldTransIndex,&OldNormalTrans);
    			OldNormalTrans.ucTIPAdjustFlag = 1;				//Changed 08-06-06
    			OldNormalTrans.ucTIPCode = 1;						//��־ԭ����������С�ѵ���
    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&OldNormalTrans,sizeof(NORMALTRANS));
    			ucResult=XDATA_Write_SaveTrans_File(G_RUNDATA_uiOldTransIndex);
    			if (ucResult)   return(ucResult);

    			util_Printf("С�ѵ�����־:%02x\n",OldNormalTrans.ucTIPCode);
    			util_Printf("С�ѵ��� =[%02x] [%02x]\n",G_RUNDATA_ucTIPSaving,G_NORMALTRANS_ucTIPAdjustFlag);
#endif
    			/*�����µĽ���,�����ѻ������ͣ����ؿ������ͱ�־
    			�ѻ�����������С�ѣ����������豣��*/
    			if(G_RUNDATA_ucAdjustFlag == 3)
    				G_NORMALTRANS_ulAmount += G_NORMALTRANS_ulFeeAmount;
    			if(G_RUNDATA_ucAdjustFlag)
    			{
    				ulAmount = G_NORMALTRANS_ulFeeAmount;
    				G_NORMALTRANS_ulFeeAmount = 0;
    			}
    			uiNII = TRANS_NIINORMAL;
    			Os__saved_copy((unsigned char *)&uiNII,
    				(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiTransIndex],
    				sizeof(unsigned short));
    			ucResult = XDATA_Write_Vaild_File(DataSaveTransInfo);
    			if (ucResult)   return(ucResult);
    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));

    			if(G_RUNDATA_ucTIPSaving == 2)
    			{
    				/*�������͵��������׸�ΪС�ѵ���׷��*/
    				DataSave1.SaveTrans.ucTransType = TRANS_TIPADJUSTOK;
    			}
    			ucResult = XDATA_Write_SaveTrans_File(G_RUNDATA_uiTransIndex);
    			if (ucResult)   return(ucResult);
    			if(G_RUNDATA_ucAdjustFlag)
    			{
    				G_NORMALTRANS_ulFeeAmount = ulAmount;
    			}
    		}
    	}
    	else if(G_NORMALTRANS_ucTransType == TRANS_UNDOOFF)
    	{
    #if 1
		//��С�ѳ���:�ֽ���Ϊ[���߳���],ԭ����Ϊ[С�ѵ������������],�������Ϊ[����]
    		if(G_NORMALTRANS_ucOldOldTransType == TRANS_PURCHASE)
    		{
    #if 1
    /*��1��:*/
    			uiTransIndex = G_NORMALTRANS_uiOldOldTransIndex;

    			util_Printf("�����������2:%d\n",uiTransIndex);
    			GetOldTransData_O(uiTransIndex,&OldNormalTrans);
    			OldNormalTrans.ucTIPCode = 0;
    			OldNormalTrans.ucTIPAdjustFlag = 0;
    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&OldNormalTrans,sizeof(NORMALTRANS));
    			ucResult = XDATA_Write_SaveTrans_File(uiTransIndex);
    			if (ucResult)   return(ucResult);
    /*��2��:*/
    			/*����ԭ����*/
    			G_NORMALTRANS_ucTransType = TRANS_UNDOOFF;		//changed LOTE 2008-06-23
    			uiNII = TRANS_NIIVOID;
    			Os__saved_copy((unsigned char *)&uiNII,
    					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex],
    					sizeof(unsigned short));
    			ucResult = XDATA_Write_Vaild_File(DataSaveTransInfo);
    			if (ucResult)   return(ucResult);

    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
    			ucResult = XDATA_Write_SaveTrans_File(G_RUNDATA_uiOldTransIndex);
    			if (ucResult)   return(ucResult);
    			/*�����ܴ洢��������*/
    			G_RUNDATA_uiTransIndex = G_RUNDATA_uiOldTransIndex;
    #endif
    		}
    #endif
			// �����߽��㳷��:�ֽ���Ϊ[���߳���],ԭ����Ϊ[���߽���],��ԭ���׸���Ϊ�ѳ�������
    		if(G_NORMALTRANS_ucOldTransType==TRANS_OFFPREAUTHFINISH)
    		{
				 /*1:����ԭ����,��ԭ���׸��� Ϊ���߳�������*/
    			/*����ԭ����*/
    			G_NORMALTRANS_ucTransType = TRANS_UNDOOFF;
    			uiNII = TRANS_NIIVOID;
			Os__saved_copy((unsigned char *)&uiNII,
    					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex],
    					sizeof(unsigned short));
    			ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);
    			if (ucResult)
    			    return(ucResult);

    			OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
    			ucResult = XDATA_Write_SaveTrans_File(G_RUNDATA_uiOldTransIndex);
    			if (ucResult)   return(ucResult);
    			/*�����ܴ洢��������*/
    			G_RUNDATA_uiTransIndex = G_RUNDATA_uiOldTransIndex;
    		}
    	}
    }

	/* Write Count information �ֱ�洢��Ǻʹ��ǽ����ֶ�*//*���ױ����ͽ��׽��*/
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_CREDITSALE:
		case TRANS_PURCHASE:
		case TRANS_CUPMOBILE:				//Changed 09-1-6
		case TRANS_PREAUTHFINISH:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_OFFPURCHASE:
		case TRANS_MOBILEAUTH:
			uiTotalNb = pTransTotal->uiDebitNb+1;
			ulTotalAmount = pTransTotal->ulDebitAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiDebitNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulDebitAmount,
					sizeof(ULONG_C51));

			/*�洢���࿨����ϸ*/
			//fangbo --modify--date:070618
			ulOldTrueAmount = 0;
			G_NORMALTRANS_ulTrueAmount = (G_NORMALTRANS_ulAmount/10000)*(10000-uiTempRate)
	    			     +((G_NORMALTRANS_ulAmount%10000)*(10000-uiTempRate)/1000+5)/10;
			uiTotalNb = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb+1 ;
			ulTotalAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount+ G_NORMALTRANS_ulAmount;
			ulTotalTrueAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount
			 			+ G_NORMALTRANS_ulTrueAmount ;
			Os__saved_copy((unsigned char *)&uiTotalNb,
						(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb,
						sizeof(unsigned int));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
							(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount,
							sizeof(unsigned long));
			Os__saved_copy((unsigned char *)&ulTotalTrueAmount,
							(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount,
							sizeof(ULONG_C51));
			//-----------------end---------
			break;
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDOFFPURCHASE:
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_REFUND:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_EC_CASHLOAD:/*�ֽ��ֵ*/
			uiTotalNb = pTransTotal->uiCreditNb+1;
			ulTotalAmount = pTransTotal->ulCreditAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiCreditNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulCreditAmount,
					sizeof(ULONG_C51));

			//fangbo --modify--date:070618
			if(G_NORMALTRANS_ucTransType!=TRANS_REFUND
			    && G_NORMALTRANS_ucTransType!=TRANS_EC_CASHLOAD)
			{
				/*�洢���࿨����ϸ*/
				uiTotalNb = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb-1 ;
				ulTotalAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount- G_NORMALTRANS_ulAmount;
				ulTotalTrueAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount
				 			- G_NORMALTRANS_ulTrueAmount ;

				Os__saved_copy((unsigned char *)&uiTotalNb,
								(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb,
								sizeof(unsigned int));
				Os__saved_copy((unsigned char *)&ulTotalAmount,
								(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount,
								sizeof(unsigned long));
				Os__saved_copy((unsigned char *)&ulTotalTrueAmount,
								(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount,
								sizeof(ULONG_C51));

				ulTotalAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount
						- G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
							(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount,
							sizeof(unsigned long));
			}
			//-----------end---------------
			break;
		case TRANS_UNDOOFF:	//���߳���[���¸���������]
			util_Printf("\n�����߳��������ϸ���й���\nStart to story TRANS_UNDOOFF\n");
			util_Printf("|\n");
			util_Printf("|	������׵���=02/0Dʱ,Ϊ���ѽ���/Ϊ���߽��㽻��\n");
			util_Printf("|	ԭ���׵���=0Dʱ��	Ϊ���߽��㽻��\n");
			util_Printf("|\n");
			util_Printf("G_NORMALTRANS_ucOldOldTransType=%02x\n",G_NORMALTRANS_ucOldOldTransType);
			util_Printf("G_NORMALTRANS_ucOldTransType=%02x\n",G_NORMALTRANS_ucOldTransType);
			util_Printf("С��:%d\n",G_NORMALTRANS_ulFeeAmount);
			if(G_NORMALTRANS_ucOldOldTransType==TRANS_PURCHASE)/*��������������ѵ�����*/
			{
				ulTotalAmount = pTransTotal->ulPurchaseAmount-G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulPurchaseAmount,
						sizeof(ULONG_C51));
				ulTotalTIPAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount
								- G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalTIPAmount,
						(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount,
						sizeof(unsigned long));
				ulTotalAmount = pTransTotal->ulDebitAmount-G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulDebitAmount,
						sizeof(ULONG_C51));

				util_Printf("=			С�ѳ�����ͳ��			=\n");
				util_Printf("|	�����ѽ��:%d\n",ulTotalAmount);
				util_Printf("|	���:%d\n",pTransTotal->ulPurchaseAmount);
				util_Printf("|	�⿨С�ѽ��:%d\n",ulTotalTIPAmount);
				util_Printf("|	�ܽ�ǽ��:%d\n",ulTotalAmount);
				util_Printf("|	�ܽ��ױ���:%d\n",pTransTotal->uiTotalNb);
			}  //���½���
			util_Printf("G_NORMALTRANS_ulAmount=%d\n",G_NORMALTRANS_ulAmount);
			if(G_NORMALTRANS_ucOldTransType==TRANS_OFFPREAUTHFINISH) //����ԭ�������߽�������
			{
				ulTotalAmount = pTransTotal->ulDebitAmount-G_NORMALTRANS_ulAmount;
				util_Printf("�ܽ�ǽ��uiTotalNb=%d\n",ulTotalAmount);

				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulDebitAmount,
						sizeof(ULONG_C51));

				/*�洢���࿨����ϸ*/
				G_NORMALTRANS_ulTrueAmount = (G_NORMALTRANS_ulAmount/10000)*(10000-uiTempRate)
	    					 +((G_NORMALTRANS_ulAmount%10000)*(10000-uiTempRate)/1000+5)/10;
				util_Printf("G_NORMALTRANS_ulTrueAmount=%d\n",G_NORMALTRANS_ulTrueAmount);

				uiTotalNb = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb-1 ;

				util_Printf("�⿨�ܱ���uiTotalNb=%d\n",uiTotalNb);

				ulTotalAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount- G_NORMALTRANS_ulAmount;

				util_Printf("�⿨�ܽ��ulTotalAmount=%d\n",ulTotalAmount);

				ulTotalTrueAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount
			 				-G_NORMALTRANS_ulTrueAmount ;

				util_Printf("���⿨�����ulTotalTrueAmount=%d\n",ulTotalTrueAmount);

				Os__saved_copy((unsigned char *)&uiTotalNb,
							(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].uiTotalNb,
							sizeof(unsigned int));
				Os__saved_copy((unsigned char *)&ulTotalAmount,
								(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount,
								sizeof(unsigned long));
				Os__saved_copy((unsigned char *)&ulTotalTrueAmount,
								(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount,
								sizeof(ULONG_C51));
				/**/
				uiTotalNb = pTransTotal->uiOfflineNb-1;//�����߱�����ȥ
				util_Printf("�����ܱ���:%d\n",uiTotalNb);
				ulTotalAmount = pTransTotal->ulOfflineAmount-G_NORMALTRANS_ulAmount;//�����߽���ȥ
				util_Printf("�����ܽ��:%d\n",ulTotalAmount);
				Os__saved_copy((unsigned char *)&uiTotalNb,
						(unsigned char *)&pTransTotal->uiOfflineNb,
						sizeof(UINT_C51));
				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulOfflineAmount,
						sizeof(ULONG_C51));
			}
			break;
		case TRANS_TIPADJUST:
				/*���߽��ף�С�ѽ��ֻ��δ��ʱ����ʹ��*/
			if(G_RUNDATA_ucAdjustFlag==1)
				ulTotalAmount = pTransTotal->ulDebitAmount-G_NORMALTRANS_ulFeeAmount;
			else
				ulTotalAmount = pTransTotal->ulDebitAmount+G_NORMALTRANS_ulFeeAmount;
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulDebitAmount,
					sizeof(ULONG_C51));

			if(G_RUNDATA_ucAdjustFlag == 0)	/*����һ�������ӵ������ܽ��*/
			{
				ulTotalAmount = pTransTotal->ulPurchaseAmount+G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulPurchaseAmount,
						sizeof(ULONG_C51));
				ulTotalTIPAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount
								+ G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalTIPAmount,
						(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTipAmount,
						sizeof(unsigned long));
			}else
			{
				/*�����߽����ۼӵ������ۼ�����*/
				if(G_RUNDATA_ucAdjustFlag==1)
					ulTotalAmount = pTransTotal->ulOfflineAmount-G_NORMALTRANS_ulFeeAmount;
				else
					ulTotalAmount = pTransTotal->ulOfflineAmount+G_NORMALTRANS_ulFeeAmount;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
						(unsigned char *)&pTransTotal->ulOfflineAmount,
						sizeof(ULONG_C51));
			}
			if(G_RUNDATA_ucTIPSaving != 2)	/*���ߵ���ʱ��ӡС����ӦΪ��*/
				G_NORMALTRANS_ulFeeAmount = 0;
			if((G_RUNDATA_ucTIPSaving == 0)||(G_RUNDATA_ucTIPSaving == 1)) //changed...LOTE  2008-02-22
			{
				/*�洢���࿨����ϸ*/
				//fangbo --modify--date:070618
				ulOldTrueAmount = 0;
				G_NORMALTRANS_ulTrueAmount = (G_NORMALTRANS_ulAmount/10000)*(10000-uiTempRate)
		    			     			+((G_NORMALTRANS_ulAmount%10000)*(10000-uiTempRate)/1000+5)/10;
				ulTotalAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount
								-G_NORMALTRANS_ulOldAmount
								+ G_NORMALTRANS_ulAmount;
				ulTotalTrueAmount = DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount
								-G_NORMALTRANS_ulOldTrueAmount
				 				+ G_NORMALTRANS_ulTrueAmount ;
				Os__saved_copy((unsigned char *)&ulTotalAmount,
								(unsigned char *)& DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulAmount,
								sizeof(unsigned long));
				Os__saved_copy((unsigned char *)&ulTotalTrueAmount,
								(unsigned char *)&DataSave0.TransInfoData.TransDetail[G_NORMALTRANS_ucCardType].ulTrueAmount,
								sizeof(ULONG_C51));
				//-----------------end---------
			}
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			uiTotalNb = pTransTotal->uiAuthNb+1;
			ulTotalAmount = pTransTotal->ulAuthAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiAuthNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulAuthAmount,
					sizeof(ULONG_C51));
			break;
		default:
			break;
	}
	/*�洢ÿһ�ֽ����ܶ�*/
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_CREDITSALE:
		case TRANS_PURCHASE:
		case TRANS_CUPMOBILE:				//09-01-6
		case TRANS_OFFPURCHASE:
		case TRANS_MOBILEAUTH:
			uiTotalNb = pTransTotal->uiPurchaseNb+1;
			ulTotalAmount = pTransTotal->ulPurchaseAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiPurchaseNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulPurchaseAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_EC_CASHLOAD:

			uiTotalNb = pTransTotal->uiCashLoadNb+1;
			ulTotalAmount = pTransTotal->uiCashLoadAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiCashLoadNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->uiCashLoadAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_PREAUTHFINISH:
			uiTotalNb= pTransTotal->uiAuthFinishNb+1;
			ulTotalAmount = pTransTotal->ulAuthFinishAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiAuthFinishNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulAuthFinishAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_PREAUTHSETTLE:
			uiTotalNb= pTransTotal->uiPreauthSettleNb+1;
			ulTotalAmount = pTransTotal->ulPreauthSettleAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiPreauthSettleNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulPreauthSettleAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			uiTotalNb = pTransTotal->uiAuthNb-1;
			ulTotalAmount = pTransTotal->ulAuthAmount-G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiAuthNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulAuthAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDOFFPURCHASE:
			uiTotalNb = pTransTotal->uiPurchaseNb-1;
			ulTotalAmount = pTransTotal->ulPurchaseAmount - G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiPurchaseNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulPurchaseAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_VOIDPREAUTHFINISH:
			uiTotalNb= pTransTotal->uiAuthFinishNb-1;
			ulTotalAmount = pTransTotal->ulAuthFinishAmount - G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiAuthFinishNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulAuthFinishAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_REFUND:
			uiTotalNb = pTransTotal->uiRefundNb+1;
			ulTotalAmount = pTransTotal->ulRefundAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiRefundNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulRefundAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_OFFPREAUTHFINISH:
			uiTotalNb = pTransTotal->uiOfflineNb+1;
			ulTotalAmount = pTransTotal->ulOfflineAmount+ G_NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
					(unsigned char *)&pTransTotal->uiOfflineNb,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
					(unsigned char *)&pTransTotal->ulOfflineAmount,
					sizeof(ULONG_C51));
			break;
		case TRANS_UNDOOFF:
			/*ͳ�����߽�������*/
			uiUndoOffSum=pTransTotal->uiUndoOffSum+1;
			if(G_NORMALTRANS_ucOldTransType==TRANS_OFFPREAUTHFINISH)
				ulUndoOffAmount=pTransTotal->ulUndoOffAmount+G_NORMALTRANS_ulAmount;
			else
				ulUndoOffAmount=pTransTotal->ulUndoOffAmount+G_NORMALTRANS_ulFeeAmount;

			util_Printf("||	���߽��ױ���:%d\n",uiUndoOffSum);
			util_Printf("||	���߽��׽��:%d\n",ulUndoOffAmount);

			Os__saved_copy((unsigned char *)&uiUndoOffSum,
					(unsigned char *)&pTransTotal->uiUndoOffSum,
					sizeof(UINT_C51));
			Os__saved_copy((unsigned char *)&ulUndoOffAmount,
					(unsigned char *)&pTransTotal->ulUndoOffAmount,
					sizeof(ULONG_C51));
			break;
		default:
			break;
	}
	if(G_NORMALTRANS_ucTransType == TRANS_TIPADJUST)
	{
		if(G_RUNDATA_ucTIPSaving==0)
			G_NORMALTRANS_ucTransType = TRANS_OFFPREAUTHFINISH;
	}

	Os__saved_copy((unsigned char *)&G_RUNDATA_uiTransIndex,
			(unsigned char *)&DataSave0.ChangeParamData.uiLastTransIndex,
			sizeof(unsigned short));

	if (DataSave0.ConstantParamData.ucMBFEFlag==0x31
        &&(G_NORMALTRANS_ucTransType==TRANS_PURCHASE
            ||G_NORMALTRANS_ucTransType==TRANS_VOIDPURCHASE)
        )   //��ת����ǰ�û��Ķ������MBFE_ISO8583�������
   {
		memset(xDATA_Change.aucMBFEBuf,0,sizeof(xDATA_Change.aucMBFEBuf));
   }
	ucResult = XDATA_Write_Vaild_File(DataSaveChange);
	if (ucResult)   return(ucResult);       

	ucResult=XDATA_Write_Vaild_File(DataSaveTransInfo);
	if (ucResult)return(ucResult);
	return(SUCCESS);
}

unsigned char SAV_SavedTransIndex(unsigned char ucFlag)
{
	unsigned short uiIndex;
	unsigned char	ucTransType,ucInputMode,ucReadType;
	unsigned long	ulTraceNumber,ulOldTraceNumber;
	unsigned char	ucResult,ucTransNLen,aucTempBuf[6];

        ucResult = XDATA_Write_Vaild_File(TempData);
        if(ucResult == SUCCESS)
        {
            ucResult = XDATA_Clear_TempFile();
            if(ucResult != SUCCESS)
            {
                return ucResult;
            }
        }
        else
        {
            ucResult = XDATA_Clear_TempFile();
            return ERR_WRITEFILE;
        }
	switch( ucFlag )
	{
	case 0:
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex] == 0)
				break;
		}
		if( uiIndex == TRANS_MAXNB)
		{
			return(ERR_TRANSFILEFULL);
		}
		G_RUNDATA_uiTransIndex = uiIndex;
		break;
	case 1:
		ucResult = UTIL_Is_Trans_Empty();
		if(ucResult) return ucResult;


util_Printf("\n.1001.ucTransType...........[%02x]",G_NORMALTRANS_ucTransType);
util_Printf("\n.1002.ucInputMode...........[%02x]",G_NORMALTRANS_ucInputMode);
util_Printf("\n.1003.TraceNumber...........[%6d]\n",G_NORMALTRANS_ulTraceNumber);	


		ucInputMode = G_NORMALTRANS_ucInputMode;
		ucTransType = G_NORMALTRANS_ucTransType;
		ulTraceNumber = G_NORMALTRANS_ulTraceNumber;

		ucReadType = G_NORMALTRANS_euCardSpecies;
              memset(aucTempBuf,0,sizeof(aucTempBuf));

		ucTransNLen = strlen((char*)G_RUNDATA_aucOriginalTraceNum);
		memcpy(aucTempBuf,G_RUNDATA_aucOriginalTraceNum,ucTransNLen);
		util_Printf("==+1ԭƾ֤�ų���= %d\n",ucTransNLen);
		util_Printf("��[%d]��\n",(6-ucTransNLen));
		if(ucTransNLen < 6)
		{
			memset(&G_RUNDATA_aucOriginalTraceNum[0],0x30,(6-ucTransNLen));
			memcpy(&G_RUNDATA_aucOriginalTraceNum[6-ucTransNLen],aucTempBuf,ucTransNLen);
		}
		util_Printf("ԭƾ֤�� =%s\n",G_RUNDATA_aucOriginalTraceNum);


		ulOldTraceNumber=asc_long(G_RUNDATA_aucOriginalTraceNum,6);
		util_Printf("\nulOldTraceNumber %u",ulOldTraceNumber);
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				GetOldTransData(uiIndex);
				OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
				if(ulOldTraceNumber != DataSave1.SaveTrans.ulTraceNumber)
				{
					continue;
				}

				memcpy(G_NORMALTRANS_aucOldRefNumber,G_NORMALTRANS_aucRefNum,TRANS_REFNUMLEN);
				G_NORMALTRANS_ucOldTransType = G_NORMALTRANS_ucTransType;
				G_RUNDATA_uiOldTransIndex = uiIndex;
				G_NORMALTRANS_ulOldTraceNumber = G_NORMALTRANS_ulTraceNumber;
				memcpy(G_NORMALTRANS_aucOldOrdersNo,G_NORMALTRANS_aucOrdersNo,TRANS_ORDERSNO);
				memcpy(G_NORMALTRANS_aucOldDate ,G_NORMALTRANS_aucDate , TRANS_DATELEN);
				if (ucTransType == TRANS_VOIDPREAUTHFINISH)
				{
					memcpy(G_NORMALTRANS_aucAuthCode,G_NORMALTRANS_aucOldAuthCode,TRANS_AUTHCODELEN);
					util_Printf("++Ԥ��Ȩ��ɳ���ԭԭԤ�ںţ�%s\n",G_NORMALTRANS_aucAuthCode);
				}

				util_Printf("\nԭ��ˮ��: %ld",G_NORMALTRANS_ulOldTraceNumber);
				util_Printf("\nԭ������: %s",G_NORMALTRANS_aucOldOrdersNo);
				break;
			}
			else
			    return(MSG_NOTRANS);
		}
		util_Printf("\nuiIndex %d",uiIndex);
		if(uiIndex == TRANS_MAXNB)
		{
			return(MSG_NOTRANS);
		}
		util_Printf("\n�ֽ�������:%02x\n",ucTransType);
		util_Printf("ԭ��������:%02x\n",G_NORMALTRANS_ucTransType);

		/*���ѳ��������ǳ���ԭ���ѽ���*/
		if(ucTransType == TRANS_VOIDPURCHASE)
		{
			if(DataSave0.ConstantParamData.ucInstallment=='1')
			{
				if(G_NORMALTRANS_ucTransType != TRANS_PURCHASE
					&&G_NORMALTRANS_ucTransType != TRANS_CREDITSALE)
					return(ERR_OLDNOTVOID);
				if (G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
				{
					ucTransType=TRANS_VOIDPURCHASE;
				}
				else if (G_NORMALTRANS_ucTransType==TRANS_CREDITSALE)
				{
					ucTransType=TRANS_CREDITSVOID;
				}
			}
			else
		    {
				if(G_NORMALTRANS_ucTransType != TRANS_PURCHASE)
					return(ERR_OLDNOTVOID);
			}
	    }else if (ucTransType == TRANS_VOIDCUPMOBILE)
		{
		    	if(G_NORMALTRANS_ucTransType != TRANS_CUPMOBILE)
				return (ERR_OLDNOTVOID);
		}else if(ucTransType == TRANS_VOIDPREAUTHFINISH)	/*Ԥ��Ȩ��ɳ��������ǳ���ԭԤ��Ȩ��ɽ���*/
		{
		    if(G_NORMALTRANS_ucTransType != TRANS_PREAUTHFINISH)
				return(ERR_OLDNOTVOID);
		}else if(ucTransType == TRANS_CREDITSVOID)
		{
		    if(G_NORMALTRANS_ucTransType != TRANS_CREDITSALE)
				return(ERR_OLDNOTVOID);
		}

		if((DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex]&TRANS_NIIVOID) == TRANS_NIIVOID)
		{
			return(ERR_ALREADYVOID);
		}

		if(G_NORMALTRANS_ucTIPAdjustFlag)
		{
			return(ERR_TRANSADJUSTED);
		}

		G_NORMALTRANS_ucTransType = ucTransType;
		G_NORMALTRANS_ulTraceNumber = ulTraceNumber;
		G_NORMALTRANS_ucInputMode = ucInputMode;
		G_NORMALTRANS_euCardSpecies = ucReadType;

		memset(G_RUNDATA_aucOldCardPan,0,sizeof(G_RUNDATA_aucOldCardPan));
		bcd_asc(G_RUNDATA_aucOldCardPan,G_NORMALTRANS_aucSourceAcc,((G_NORMALTRANS_ucSourceAccLen*2)+1)/2);

		if((ucResult = UTIL_displayCardNum())!= SUCCESS)
			return ucResult;
		break;
	case 2:
	    util_Printf("���ߵ���\n");
		ucTransType = G_NORMALTRANS_ucTransType;
		ulTraceNumber = G_NORMALTRANS_ulTraceNumber;

		memset(aucTempBuf,0,sizeof(aucTempBuf));

		ucTransNLen = strlen((char*)G_RUNDATA_aucOriginalTraceNum);
		memcpy(aucTempBuf,G_RUNDATA_aucOriginalTraceNum,ucTransNLen);
		util_Printf("==+2ԭƾ֤�ų���= %d\n",ucTransNLen);
		util_Printf("��[%d]��\n",(6-ucTransNLen));
		if(ucTransNLen < 6)
		{
			memset(&G_RUNDATA_aucOriginalTraceNum[0],0x30,(6-ucTransNLen));
			memcpy(&G_RUNDATA_aucOriginalTraceNum[6-ucTransNLen],aucTempBuf,ucTransNLen);
		}
		util_Printf("ԭƾ֤�� =%s\n",G_RUNDATA_aucOriginalTraceNum);

		ulOldTraceNumber=asc_long(G_RUNDATA_aucOriginalTraceNum,6);
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			//if( (DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_NIIVOID) != TRANS_NIIVOID)
			util_Printf("����:%02x\n",DataSave0.TransInfoData.auiTransIndex[uiIndex]);
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				GetOldTransData(uiIndex);
				OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
				if(ulOldTraceNumber != DataSave1.SaveTrans.ulTraceNumber)
				{
					continue;
				}
				G_NORMALTRANS_ulOldTrueAmount = G_NORMALTRANS_ulTrueAmount;
				G_NORMALTRANS_ulOldAmount = G_NORMALTRANS_ulAmount;
				G_NORMALTRANS_ucOldTransType = G_NORMALTRANS_ucTransType;
				G_NORMALTRANS_ulOldTraceNumber = G_NORMALTRANS_ulTraceNumber;
				memcpy(G_NORMALTRANS_aucOldRefNumber ,G_NORMALTRANS_aucRefNum ,TRANS_REFNUMLEN);
				memcpy(G_NORMALTRANS_aucOldDate , G_NORMALTRANS_aucDate, TRANS_DATELEN);

				G_RUNDATA_uiOldTransIndex = uiIndex;
				G_NORMALTRANS_uiOldOldTransIndex = uiIndex ;
				break;
			}
			else
			    return(MSG_NOTRANS);
		}
		if(uiIndex == TRANS_MAXNB)
		{
			return(MSG_NOTRANS);
		}
		if( (G_NORMALTRANS_ucTransType != TRANS_PURCHASE)
		  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
		  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPURCHASE)
		  )
		{
		  	return(ERR_TRANSNotADJUST);
		}
		if(G_NORMALTRANS_ucTIPAdjustFlag)//�ж��Ƿ��ѵ���
		{
			return(ERR_TRANSADJUSTED);
		}
		if( (G_NORMALTRANS_ucCardType == TRANS_CARDTYPE_INTERNAL)
		  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
		  )
		{
			return(ERR_INTERNOTTIP);
		}


		G_NORMALTRANS_ucTransType = ucTransType;
		G_NORMALTRANS_ulTraceNumber = ulTraceNumber;
		break;
	case 3:
		ucInputMode = G_NORMALTRANS_ucInputMode;
		ucTransType = G_NORMALTRANS_ucTransType;
		ulTraceNumber = G_NORMALTRANS_ulTraceNumber;

		memset(aucTempBuf,0,sizeof(aucTempBuf));

		ucTransNLen = strlen((char*)G_RUNDATA_aucOriginalTraceNum);
		memcpy(aucTempBuf,G_RUNDATA_aucOriginalTraceNum,ucTransNLen);
		util_Printf("==+3ԭƾ֤�ų���= %d\n",ucTransNLen);
		util_Printf("��[%d]��\n",(6-ucTransNLen));
		if(ucTransNLen < 6)
		{
			memset(&G_RUNDATA_aucOriginalTraceNum[0],0x30,(6-ucTransNLen));
			memcpy(&G_RUNDATA_aucOriginalTraceNum[6-ucTransNLen],aucTempBuf,ucTransNLen);
		}
		util_Printf("ԭƾ֤�� =%s\n",G_RUNDATA_aucOriginalTraceNum);

		ulOldTraceNumber=asc_long(G_RUNDATA_aucOriginalTraceNum,6);

		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
		    util_Printf("����:%02x\n",DataSave0.TransInfoData.auiTransIndex[uiIndex]);
		    if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		    {
		        GetOldTransData(uiIndex);
			 OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
						(unsigned char *)&NormalTransData,
						sizeof(NORMALTRANS));
			if(ulOldTraceNumber != DataSave1.SaveTrans.ulTraceNumber)
			{
				continue;
			}
			G_NORMALTRANS_ucOldOldTransType = G_NORMALTRANS_ucOldTransType;	//�����������
			G_NORMALTRANS_ulOldOldTraceNumber=G_NORMALTRANS_ulOldTraceNumber;//���������ˮ

			G_NORMALTRANS_ucOldTransType = G_NORMALTRANS_ucTransType;
			G_NORMALTRANS_ulOldTraceNumber = G_NORMALTRANS_ulTraceNumber;
			memcpy(G_NORMALTRANS_aucOldDate ,G_NORMALTRANS_aucDate , TRANS_DATELEN);

			G_RUNDATA_uiOldTransIndex = uiIndex;						  //�������������
			break;
		    }
		    else
		        return(MSG_NOTRANS);
		}

		if(uiIndex == TRANS_MAXNB)
		{
			return(MSG_NOTRANS);
		}
		util_Printf("\nG_RUNDATA_uiOldTransIndex=%d\n",G_RUNDATA_uiOldTransIndex);
		 /*���߳��������ǳ��������е�С��[�������������]���ѻ����ѡ����߽��������߽���Ľ������*/
		if(ucTransType == TRANS_UNDOOFF)
		{
			if((G_NORMALTRANS_ucTransType !=TRANS_TIPADJUST)
				&&(G_NORMALTRANS_ucTransType!=TRANS_TIPADJUSTOK)
				&&(G_NORMALTRANS_ucTransType != TRANS_OFFPURCHASE)
				&&(G_NORMALTRANS_ucTransType!=TRANS_OFFPREAUTHFINISH)
			  )
				return(ERR_OLDNOTVOID);
		}
		util_Printf("\n�����Ƿ���=%d\n",DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex]);
		if((DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex]&TRANS_NIIVOID) == TRANS_NIIVOID)
		{
			util_Printf("ERR_ALREADYVOID[�����ѳ���][0x14]\n");
			return(ERR_ALREADYVOID);
		}
		util_Printf("G_NORMALTRANS_ucVoidTrans[1.1]=%02x\n",G_NORMALTRANS_ucVoidTrans);
		if(G_NORMALTRANS_ucVoidTrans==1)
		{
			util_Printf("ERR_TRANSADJUSTED[�����ѵ���][0x54]\n");
			return(ERR_TRANSADJUSTED);
		}
		G_NORMALTRANS_ucTransType = ucTransType;
		G_NORMALTRANS_ulTraceNumber = ulTraceNumber;
		G_NORMALTRANS_ucInputMode = ucInputMode;
		if((ucResult = UTIL_displayCardNum())!= SUCCESS)
			return ucResult;
		break;
	default:
		return(ERR_TRANS_SAVEINDEX);
	}
	return(SUCCESS);
}

unsigned char SAV_CheckTransMaxNb(void)
{
	unsigned short uiIndex;

	for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
	{
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex] == 0)
			break;
	}
	if( uiIndex == TRANS_MAXNB)
	{
		return(ERR_TRANSFILEFULL);
	}
	return(SUCCESS);
}
