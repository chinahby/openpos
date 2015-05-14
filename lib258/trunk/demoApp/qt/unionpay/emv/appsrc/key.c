/*
	SAND China
--------------------------------------------------------------------------
	FILE  key.c										(Copyright SAND 2001)
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2001-07-02		Xiaoxi Jiang
    Last modified :	2001-07-02		Xiaoxi Jiang
    Module :  Shanghai Golden Card Network
    Purpose :
        Source file for key management.

    List of routines in file :

    File history :

*/

#include <include.h>
#include <global.h>
#include <xdata.h>
#include <new_drv.h>


unsigned char KEY_InputKeyIndex(void)
{
	unsigned char aucBuf[50];
	unsigned short uiKeyIndex;
	unsigned char ucI;

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��Կ����(0-2)");

	while(1)
	{
		memset(aucBuf,0,sizeof(aucBuf));
		aucBuf[0]=DataSave0.ConstantParamData.ucUseKeyIndex+'0';
		if( UTIL_Input(1,true,1,1,'N',aucBuf,(uchar *)"012") != KEY_ENTER )
			return(ERR_CANCEL);
		else
		{
			uiKeyIndex = aucBuf[0]-'0';
			//if((uiKeyIndex>0)&&(uiKeyIndex<10))
			{
				ucI = (unsigned char)uiKeyIndex;
				Os__saved_copy(&ucI,&DataSave0.ConstantParamData.ucUseKeyIndex,
							sizeof(unsigned char));
				XDATA_Write_Vaild_File(DataSaveConstant);
				return(SUCCESS);
			}
		}
	}
}
unsigned char KEY_InputMasterKey(void)
{
	unsigned char ucI,ucResult;
	unsigned char aucBuf[21];

	memset(aucBuf,0,sizeof(aucBuf));
	while(1)
	{
		memset( aucBuf,0,sizeof(aucBuf) );
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"��������Կ����:");
		//Os__GB2312_display(1,0,(unsigned char*)"(0-2)");
		if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char*)"012") != KEY_ENTER )
		{
			ucResult=MSG_CANCEL;
			break;
		}
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"����������Կ:");
		ucI=aucBuf[0]-'0';
		//if (DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_SINGLE)
		//	ucResult = KEY_InputKey_Single(ucI);
		//else
			ucResult = KEY_InputKey_Double(ucI);

		if(ucResult != SUCCESS)
			break;
		MSG_DisplayMsg( true,0,0,MSG_GOON);
		if( Os__xget_key()!=KEY_ENTER )
			break;
	}
	//KEY_InputKeyIndex();
	return(SUCCESS);
}

unsigned char KEY_InputKey_Single(unsigned char ucKeyIndex)
{
	unsigned char aucAscData[17];
	unsigned char aucHexData[9];
	unsigned char ucResult;
	unsigned char ucArrayIndex;

	ucResult = SUCCESS;
	/*ǰ���鷽�ڵ�һarray�������鷽�ڶ�array
		0,1�ֱ���pinkey��mackey
		2��3��4��5��6�ֱ�master key
	*/
	ucArrayIndex = KEYARRAY_GOLDENCARDSH;


	memset(aucAscData,0,sizeof(aucAscData));
	if( (ucResult = OSUTIL_Input(0,1,16,16,'A',aucAscData,NULL)) != SUCCESS )
    {
#ifdef TEST
    util_Printf("OSUTIL_Input=%02x\n",ucResult);
#endif
		return(ERR_CANCEL);
	}
#ifdef TEST
    util_Printf("aucAscData=%s\n",aucAscData);
#endif
	asc_hex(aucHexData,8,aucAscData,16);

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex,aucHexData,aucHexData);

	if(ucResult != SUCCESS)
		return(ucResult);

	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
					ucKeyIndex,ucKeyIndex,aucHexData);

#ifdef TEST
	memcpy(aucHexData,"88888888",8);
	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex,aucHexData,aucHexData);
	util_Printf("----------Key---single---00----\n");
	for(ucI=0;ucI<8;ucI++)
	{
		util_Printf("%02x,",aucHexData[ucI]);
	}
	util_Printf("\n");
#endif
	return(ucResult);
}


unsigned char KEY_InputKey_Double(unsigned char ucKeyIndex)
{
	unsigned char aucAscData[33];
	unsigned char aucAscData1[17];
	unsigned char aucAscData2[17];
	unsigned char aucHexData[9];
	unsigned char aucHexData1[9];
	unsigned char ucResult;
	unsigned char ucArrayIndex;

	/*ǰ���鷽�ڵ�һarray�������鷽�ڶ�array
		0,1�ֱ���pinkey��mackey
		2��3��4��5��6�ֱ�master key
	*/

	ucArrayIndex = KEYARRAY_GOLDENCARDSH;

	memset(aucAscData,0,sizeof(aucAscData));
	memset(aucAscData1,0,sizeof(aucAscData1));
	memset(aucAscData2,0,sizeof(aucAscData2));
	memset(aucHexData,0,sizeof(aucHexData));
	memset(aucHexData1,0,sizeof(aucHexData1));

	if( (ucResult = OSUTIL_Input(0,1,16,32,'A',aucAscData,NULL)) != SUCCESS )
	{
		util_Printf("OSUTIL_Input-DoubleKey=%02x\n",ucResult);
		return(ERR_CANCEL);
	}
    util_Printf("aucAscData=%s\n",aucAscData);

	memcpy(aucAscData1 , aucAscData , 16);
	memcpy(aucAscData2 , &aucAscData[16] , 16);

	asc_hex(aucHexData,8,aucAscData1,16);
	asc_hex(aucHexData1,8,aucAscData2,16);

	//---------------
	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex,aucHexData,aucHexData);
	if(ucResult != SUCCESS)
		return(ucResult);
	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
					ucKeyIndex,ucKeyIndex,aucHexData);
	if(ucResult != SUCCESS)
		return(ucResult);
	//--------------------

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex+3,aucHexData1,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
					ucKeyIndex+3,ucKeyIndex+3,aucHexData1);
	//-------------------

	memcpy(aucHexData,"88888888",8);
	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex,aucHexData,aucHexData);
#ifdef TEST
util_Printf("----------Key---double---00--aucHexData--\n");
	for(ucI=0;ucI<8;ucI++)
	{
		util_Printf("%02x,",aucHexData[ucI]);
	}
	util_Printf("\n");
#endif
	memcpy(aucHexData1,"88888888",8);
	ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
					ucKeyIndex+3,aucHexData1,aucHexData1);
#ifdef TEST
util_Printf("----------Key---double---00--aucHexData1--\n");
	for(ucI=0;ucI<8;ucI++)
	{
		util_Printf("%02x,",aucHexData1[ucI]);
	}
	util_Printf("\n");
#endif
	return(ucResult);
}

unsigned char KEY_InputSYSTrans(void)
{
	unsigned short uiTempShort;
	unsigned long  uiTempLong;
	unsigned char aucBuf[50];
	unsigned char aucTmpBuf[13];
	unsigned char ucChar,ucKey;
	unsigned char aucTransTAB[3];
	unsigned char ucDispCh;

	CFG_ChangParaFlag();
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"���ճ�ʱʱ��");
	memset(aucBuf,0,sizeof(aucBuf));
	int_asc(aucBuf,2,&DataSave0.ConstantParamData.uiReceiveTimeout);
	if( UTIL_Input(1,true,2,2,'N',aucBuf,NULL) != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		uiTempShort = (unsigned short)asc_long(aucBuf,2);
		Os__saved_copy(	(unsigned char *)&uiTempShort,
			(unsigned char *)&DataSave0.ConstantParamData.uiReceiveTimeout,
			sizeof(unsigned short));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�������Դ���");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucDialRetry+0x30;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,NULL) != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		ucChar -= 0x30;
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucDialRetry,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�Ƿ�֧��С��");
	//Os__GB2312_display(3,0,(unsigned char *)"1:֧�� 0:��֧��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucXorTipFee+0x30;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		memcpy(aucTransTAB,DataSave0.ConstantParamData.aucGlobalTransEnable,PARAM_GLOBALTRANSENABLELEN);
		ucChar = aucBuf[0];
		ucChar -= 0x30;
		if(ucChar == 1)
			aucTransTAB[1] |= 0x40;
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucXorTipFee,sizeof(unsigned char));
		if(ucChar == 1)
			Os__saved_copy(	aucTransTAB,DataSave0.ConstantParamData.aucGlobalTransEnable,
				PARAM_GLOBALTRANSENABLELEN);
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
	if(XorTipFee==1)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"С�Ѱٷֱ�");
		memset(aucBuf,0,sizeof(aucBuf));
		short_asc(aucBuf,2,&DataSave0.ConstantParamData.usTipFeePre);
		if( UTIL_Input(1,true,2,2,'N',aucBuf,NULL) != KEY_ENTER )
			return(ERR_CANCEL);
		else
		{
			uiTempShort = (unsigned short)asc_long(aucBuf,2);
			Os__saved_copy(	(unsigned char *)&uiTempShort,
				(unsigned char *)&DataSave0.ConstantParamData.usTipFeePre,
				sizeof(unsigned short));
			XDATA_Write_Vaild_File(DataSaveConstant);
		}
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"����˻����");
	memset(aucTmpBuf,0,sizeof(aucTmpBuf));
	uiTempLong = DataSave0.ConstantParamData.ulRefundMaxAmount;
	if(UTIL_InputAmount(1,&uiTempLong,0,0x3B9AC9FF) != SUCCESS)
		return(ERR_CANCEL);
	else
	{
		Os__saved_copy(	(unsigned char *)&uiTempLong,
			(unsigned char *)&DataSave0.ConstantParamData.ulRefundMaxAmount,
			sizeof(unsigned long));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"���俨������");
	//Os__GB2312_display(1,0,(unsigned char *)"0:ȫ֧��(������)");
	//Os__GB2312_display(2,0,(unsigned char *)"1:ȫ��֧��");
	//Os__GB2312_display(3,0,(unsigned char *)"2:��֧����Ȩ���");
	memset(aucBuf,0,sizeof(aucBuf));

	aucBuf[0] = DataSave0.ConstantParamData.ucInputCardNo;
	//Os__GB2312_display(2,7,aucBuf);

	while(1)
	{
		ucKey = Os__xget_key();
		switch(ucKey)
		{
			case '0':
			case '1':
			case '2':
				ucChar = ucKey;
				Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucInputCardNo,
								sizeof(unsigned char));
				XDATA_Write_Vaild_File(DataSaveConstant);
				break;
			case KEY_CLEAR:
				return(ERR_CANCEL);
			case KEY_ENTER:
				ucDispCh = 0x31;
				break;
			default:
				continue;
		}
		aucBuf[0] = DataSave0.ConstantParamData.ucInputCardNo;
		//Os__GB2312_display(2,7,aucBuf);

		if(ucDispCh == 0x31)
			break;
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"֧���Զ�ǩ��");
	//Os__GB2312_display(1,0,(unsigned char *)"���Զ�ǩ��");
	//Os__GB2312_display(3,0,(unsigned char *)"1:ǩ��  0:ǩ��");
	memset(aucBuf,0,sizeof(aucBuf));

	aucBuf[0] = DataSave0.ConstantParamData.ucSelfLogoff;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucSelfLogoff,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	/***********Auto Settle Flag*********************/
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�Զ����㹦��");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ���");
	//Os__GB2312_display(3,0,(unsigned char *)"0:������ 1:����");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucDisAutoSettleFlag;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucDisAutoSettleFlag,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
		XDATA_Read_Vaild_File(DataSaveConstant);

		if(DataSave0.ConstantParamData.ucDisAutoSettleFlag == '1')
		{

			if(DataSave0.ConstantParamData.ucDisplay == 0x31 )
			{
				DataSave0.ConstantParamData.ucDisplay = '0';
				XDATA_Write_Vaild_File(DataSaveConstant);
			}
		}
	}
	/***********Auto Timing Settle Flag***********/

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��ʱ���㹦��");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ���");
	//Os__GB2312_display(3,0,(unsigned char *)"0:������ 1:����");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucDisAutoTiming;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucDisAutoTiming,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
		XDATA_Read_Vaild_File(DataSaveConstant);
		if(DataSave0.ConstantParamData.ucDisAutoTiming == '1')
		{
			if(DataSave0.ConstantParamData.ucDisplay == 0x31 )
			{
				DataSave0.ConstantParamData.ucDisplay = '0';
				XDATA_Write_Vaild_File(DataSaveConstant);
			}
			UITL_SetAutoSettleTime();
		}
	}

/********************************************************************/
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�������Դ���");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucTransRetry+0x30;

	if( UTIL_Input(1,true,1,1,'N',aucBuf,NULL) != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		ucChar -= 0x30;
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucTransRetry,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}


	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�������ͷ�ʽ");
	//Os__GB2312_display(2,0,(unsigned char *)"1:�±����� ");
	//Os__GB2312_display(3,0,(unsigned char *)"0:����ǰ����");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucOFFLineSendType;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucOFFLineSendType,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��ӡ���ϱ��浥");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��ӡ 0:����ӡ");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucPrnErrTicket;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucPrnErrTicket,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"���ѳ���ˢ��");
	//Os__GB2312_display(3,0,(unsigned char *)"1:ˢ��  0:��ˢ��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucUnSaleSwipe;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucUnSaleSwipe,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"Ԥ��Ȩ��ɳ���");
	//Os__GB2312_display(3,0,(unsigned char *)"1:ˢ��  0:��ˢ��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucUnAuthFinSwipe;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucUnAuthFinSwipe,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�����ཻ��");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ�������");
	//Os__GB2312_display(3,0,(unsigned char *)"1:����  0:������");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucVoidInputPin;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucVoidInputPin,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"���ѳ�����������");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��Ҫ  0:����Ҫ");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucUnManagePwd;
	if( UTIL_Input(1,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucUnManagePwd,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"Ԥ��Ȩ�������");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ�������");
	//Os__GB2312_display(3,0,(unsigned char *)"1:����  0:������");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucPREAUTHFINISHInputPin;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucPREAUTHFINISHInputPin,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ȱʡ��������");
	//Os__GB2312_display(1,0,(unsigned char *)"1:������");
	//Os__GB2312_display(2,0,(unsigned char *)"0:Ԥ��Ȩ");
	memset(aucBuf,0,sizeof(aucBuf));
	if(DataSave0.ConstantParamData.ucDefaultTransParam==TRANS_PURCHASE)
		aucBuf[0]='1';
	else
		aucBuf[0]='0';
	if( UTIL_Input(3,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		if(ucChar=='1')
			DataSave0.ConstantParamData.ucDefaultTransParam=TRANS_PURCHASE;
		else
			DataSave0.ConstantParamData.ucDefaultTransParam=TRANS_PREAUTH;
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�ն˼��ܷ�ʽ����");
	//Os__GB2312_display(1,0,(unsigned char *)"1:��������Կ");
	//Os__GB2312_display(2,0,(unsigned char *)"0:˫������Կ");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ENCRYPTTYPEParam;

	if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_SINGLE)
		aucBuf[0]='1';
	else
		aucBuf[0]='0';
	if( UTIL_Input(3,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		if(ucChar=='1')
		{
			DataSave0.ConstantParamData.ENCRYPTTYPEParam=ENCRYPTTYPE_SINGLE;
			DataSave0.ConstantParamData.ucENCFlag =0x01;
		}
		else
		{
			DataSave0.ConstantParamData.ENCRYPTTYPEParam=ENCRYPTTYPE_DOUBLE;
			DataSave0.ConstantParamData.ucENCFlag =0x00;
		}
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�ش�ӡ");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ�ʹ������");
	//Os__GB2312_display(3,0,(unsigned char *)"1:ʹ��  0:��ʹ��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucRePAuthority;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucRePAuthority,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"����ǰ");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ���ʾͳ����Ϣ");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucDisplay;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucDisplay,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�����");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ��ӡ������ϸ");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));

	aucBuf[0] = DataSave0.ConstantParamData.ucList;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucList,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��ӡ");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ��������⿨");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucCNOREN;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucCNOREN,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�Ƿ�֧��[F3]��");
	//Os__GB2312_display(1,0,(unsigned char *)"�������ѳ�������");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucQuickUndo;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucQuickUndo,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ǩ���Ƿ�����EMV");
	//Os__GB2312_display(1,0,(unsigned char *)"����/��Կ");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucLoadEMVParam;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucLoadEMVParam,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

    //Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"�Ƿ�֧��");
	//Os__GB2312_display(1,0,(unsigned char *)"�ս���");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucEmptySettle;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucEmptySettle,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"������IC����ʧ��");
	//Os__GB2312_display(1,0,(unsigned char *)"�Ƿ��ӡ���ϱ���");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucBatchErrTicket;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucBatchErrTicket,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ǩ���Ƿ�ͬ����̨");
	//Os__GB2312_display(1,0,(unsigned char *)"״̬/������Ϣ");
	//Os__GB2312_display(3,0,(unsigned char *)"1:��  0:��");
	memset(aucBuf,0,sizeof(aucBuf));
	aucBuf[0] = DataSave0.ConstantParamData.ucInforFlag;
	if( UTIL_Input(2,true,1,1,'N',aucBuf,(unsigned char *)"01") != KEY_ENTER )
		return(ERR_CANCEL);
	else
	{
		ucChar = aucBuf[0];
		Os__saved_copy(&ucChar,&DataSave0.ConstantParamData.ucInforFlag,
			sizeof(unsigned char));
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
	return(SUCCESS);
}


unsigned char KEY_Load_Default_Key(void)
{
	unsigned char aucHexData[9],aucHexData1[9];
	unsigned char ucResult ,ucKeyIndex ,aucBuf[2];
	unsigned int  i;

	util_Printf("KEY_Load_Default_Key of abnkID=%d\n",DataSave0.ConstantParamData.BankID);
	switch(DataSave0.ConstantParamData.BankID)
	{
		case 1:
			/*������Կ*/
			memcpy(aucHexData,"\x54\x79\xAD\x40\x38\x13\x16\x75",8);
			memcpy(aucHexData1,"\xF4\x7C\x01\xB6\x92\x29\x68\x19",8);
			break;
		case 2:
			/*������Կ*/
			memcpy(aucHexData,"\xBA\x19\x83\x20\x54\x80\xDA\xA7",8);
			memcpy(aucHexData1,"\x19\x7A\x5E\x08\xB0\xBC\xC1\xAB",8);
			break;
		case 3:
			/*������Կ*///	5D5ED6C764C83820   264646C87F9B706E
			memcpy(aucHexData,"\x5D\x5E\xD6\xC7\x64\xC8\x38\x20",8);
			memcpy(aucHexData1,"\x26\x46\x46\xC8\x7F\x9B\x70\x6E",8);
			break;
		case 4:
			/*��ҵ������Կ*///	9B6197627C15C19E  F740545EF7679126
			memcpy(aucHexData,"\x9B\x61\x97\x62\x7C\x15\xC1\x9E",8);
			memcpy(aucHexData1,"\xF7\x40\x54\x5E\xF7\x67\x91\x26",8);
			break;
		case 5:
			/*����������Կ*///	139176DF34D03213   F101FD9DD354499B
			memcpy(aucHexData,"\x13\x91\x76\xDF\x34\xD0\x32\x13",8);
			memcpy(aucHexData1,"\xF1\x01\xFD\x9D\xD3\x54\x49\x9B",8);
			break;
		case 6:
			/*ũ����Կ*///	A86164C834D3BF7A   34752026D6AD37DC
			memcpy(aucHexData,"\xA8\x61\x64\xC8\x34\xD3\xBF\x7A",8);
			memcpy(aucHexData1,"\x34\x75\x20\x26\xD6\xAD\x37\xDC",8);
			break;
		case 7:
			/*�չ��Կ*///7FABF1897973859E   102070B3940D7315
			memcpy(aucHexData,"\x7f\xAB\xF1\x89\x79\x73\x85\x9E",8);
			memcpy(aucHexData1,"\x10\x20\x70\xB3\x94\x0D\x73\x15",8);
			break;
		case 8:
			/*�����Կ*///FE79F4F432A17601   B5F49E52BC3BFED0
			memcpy(aucHexData,"\xFE\x79\xF4\xF4\x32\xA1\x76\x01",8);
			memcpy(aucHexData1,"\xB5\xF4\x9E\x52\xBC\x3B\xFE\xD0",8);
			break;
		default:
			break;
	}
	/***load master key***/
	util_Printf("\nMASTER KEY:\n");
	for(i=0;i<8;i++)
	{
		util_Printf("%02x,",aucHexData1[i]);
	}

	//---------------
	memset( aucBuf,0,sizeof(aucBuf) );

	ucKeyIndex= 0;

	Os__saved_copy(&ucKeyIndex ,&DataSave0.ConstantParamData.ucUseKeyIndex,
							sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveConstant);

	//---------------
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��������������Կ");
	//Os__GB2312_display(1,0,(unsigned char *)"   ���Ժ�....");

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,aucHexData1,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,ucKeyIndex,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	//--------------------
	return(ucResult);
}


unsigned char KEY_Load_Default_Key1(void)
{

	unsigned char aucHexData[9],aucHexData1[9];
	unsigned char ucResult,ucKeyIndex ,aucBuf[2];
	unsigned char i;

	/***load cashier key***/
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"���������ն���Կ");
	//Os__GB2312_display(1,0,(unsigned char *)"   ���Ժ�....");

	util_Printf("KEY_Load_Default_Key1 of abnkID=%d\n",DataSave0.ConstantParamData.BankID);

	switch(DataSave0.ConstantParamData.BankID)
	{
		case 1:
			/*�й�������Կ*///	91019EFE758F5E37   D5C42AF713833E97
			memcpy(aucHexData,"\x91\x01\x9E\xFE\x75\x8F\x5E\x37",8);
			memcpy(aucHexData1,"\xD5\xC4\x2A\xF7\x13\x83\x3E\x97",8);

			util_Printf("...........WE HAVE SELECT CHINABANK..........................\n");

			break;
		case 2:
			/*���������Ϻ�������Կ*/
			memcpy(aucHexData,"\xFD\x91\x94\x5D\x32\x54\x98\x98",8);
			memcpy(aucHexData1,"\xD5\x34\x3E\xD6\x3D\xE5\xC8\xC2",8);
			break;
		case 3:
			/*�Ϻ�������Կ*/
			memcpy(aucHexData,"\xB5\xCE\x7F\x58\xF7\xAE\xE0\x8C",8);
			memcpy(aucHexData1,"\xD6\x0B\x6D\x38\x3E\x02\xF2\xF1",8);
			break;
		case 4:
			/*���������Ϻ�������Կ*/
			memcpy(aucHexData,"\x9B\x61\x97\x62\x7C\x15\xC1\x9E",8);
			memcpy(aucHexData1,"\xF7\x40\x54\x5E\xF7\x67\x91\x26",8);
			break;
		case 5:
			/*�㷢�����Ϻ�������Կ*/
			memcpy(aucHexData,"\xFD\x91\x94\x5D\x32\x54\x98\x98",8);
			memcpy(aucHexData1,"\xD5\x34\x3E\xD6\x3D\xE5\xC8\xC2",8);
			break;
		case 6:
			/*���������Ϻ�������Կ*/
			memcpy(aucHexData,"\x70\xEC\xD3\x4A\x3E\x62\x9D\x01",8);
			memcpy(aucHexData1,"\xA7\xB0\x5E\x02\xF1\x8F\x75\x29",8);
			break;
		case 7:
			/*�ַ�����*/
			memcpy(aucHexData1,"\x76\x75\x6E\x58\xFD\xA7\x31\xF8",8);
			memcpy(aucHexData,"\xF8\x25\x85\x57\xFD\x34\x57\x38",8);
			break;
		case 8:
			/*ԭũ�������磬��ũ����ҵ����*/
			memcpy(aucHexData1,"\x04\x7F\xB0\xE3\x13\x51\x98\x13",8);
			memcpy(aucHexData,"\x70\x19\x6D\xB3\x1F\xA7\xB5\x61",8);
			break;
		default:
			break;
	}

	/***load master key***/
	util_Printf("\nMASTER KEY:\n");
	for(i=0;i<8;i++)
	{
		util_Printf("%02x,",aucHexData1[i]);
	}
	//---------------
	memset( aucBuf,0,sizeof(aucBuf) );

	ucKeyIndex = 0;


	Os__saved_copy(&ucKeyIndex ,&DataSave0.ConstantParamData.ucUseKeyIndex,
							sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveConstant);

	//---------------
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��������������Կ");
	//Os__GB2312_display(1,0,(unsigned char *)"   ���Ժ�....");

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,aucHexData1,aucHexData);
	if(ucResult != SUCCESS)
		return(ucResult);
	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,ucKeyIndex,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	//--------------------
	return(ucResult);

}



unsigned char KEY_Load_Default_Key2(void)
{
	unsigned char aucHexData[9],aucHexData1[9];
	unsigned char ucResult ,ucKeyIndex ,aucBuf[2];
	unsigned int  i;

	util_Printf("KEY_Load_Default_Key of abnkID=%d\n",DataSave0.ConstantParamData.BankID);
	switch(DataSave0.ConstantParamData.BankID)
	{
		case 1:
			/*����������Կ*/
			memcpy(aucHexData,"\x13\x91\x76\xDF\x34\xD0\x32\x13",8);
			memcpy(aucHexData1,"\xF1\x01\xFD\x9D\xD3\x54\x49\x9B",8);

			util_Printf("...........WE HAVE SELECT NINGBOBANK..........................\n");
			break;
		case 2:
			/*��������*/
			memcpy(aucHexData1,"\x4D\xCA\x7F\xD2\xD8\x89\xF0\xDA",8);
			memcpy(aucHexData,  "\x40\x6C\xEC\x67\xF8\x18\x63\xE5",8);
			break;
		default:
			break;
	}
	/***load master key***/
	util_Printf("\nMASTER KEY:\n");
	for(i=0;i<8;i++)
	{
		util_Printf("%02x,",aucHexData1[i]);
	}

	//---------------
	memset( aucBuf,0,sizeof(aucBuf) );

	ucKeyIndex= 0;

	Os__saved_copy(&ucKeyIndex ,&DataSave0.ConstantParamData.ucUseKeyIndex,
							sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveConstant);

	//---------------
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"��������������Կ");
	//Os__GB2312_display(1,0,(unsigned char *)"   ���Ժ�....");

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,aucHexData1,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(KEYARRAY_GOLDENCARDSH,
					ucKeyIndex,ucKeyIndex,aucHexData1);
	if(ucResult != SUCCESS)
		return(ucResult);
	//--------------------
	return(ucResult);
}
unsigned char KEY_ReceiveByte_COM2(unsigned char *pucByte,unsigned short *puiTimeout)
{
	unsigned short uiResult;

	uiResult = Os__rxcar3(*puiTimeout);

	switch	( uiResult / 256 )
	{
		case OK :
			*pucByte = uiResult % 256;
			return(OK);
		case COM_ERR_TO:
			return(ERR_COMMS_RECVTIMEOUT);
		default :
			return(ERR_COMMS_RECVCHAR);
	}
}
unsigned char KEY_InitCom2(void)
{
  unsigned char ucResult;

	Os__end_com3();

	ucResult=Os__init_com3(0x0303, 0x0C00, 0x0C);

	if ( ucResult != OK )
	{
	  Os__end_com3();
	  return(ERR_OSFUNC);
	}
  return(SUCCESS);
}

unsigned char KEY_ReceiveData_COM2(unsigned char *pucOutData,unsigned short *puiOutLen)
{
	unsigned char ucResult = SUCCESS;
	unsigned char aucBuf[64];
	unsigned short uiI;
	unsigned short uiTimeout;
	unsigned char ucByte;

	uiTimeout = 300*100;

	while(1)
	{
		ucResult = KEY_ReceiveByte_COM2(&ucByte,&uiTimeout);
		if( ucResult != OK)
		{
			return(ucResult);
		}
		if( ucByte != CHAR_STX )
		{
			continue;
		}else
		{
			break;
		}
	}

	memset(aucBuf,0,sizeof(aucBuf));
	for( uiI=0;uiI<16;uiI++)
	{
		ucResult = KEY_ReceiveByte_COM2(&ucByte,&uiTimeout);
		if( ucResult != OK)
		{
			return(ucResult);
		}
		aucBuf[uiI] = ucByte;
	}

	*puiOutLen = 16;

	Os__xdelay(100);
	memcpy(pucOutData ,aucBuf ,16);

	return ucResult;

}
unsigned char KEY_UpdataMKeyFRS232(unsigned char *pucOutData)
{
	unsigned char aucRecBuf[64],aucBuf[64];
	unsigned char ucResult;
	unsigned char ucKey;
	unsigned short uiLen;

  //Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"������Կ��װ");
	//Os__GB2312_display(2,0,(unsigned char *)"[ȷ ��]�� ��");
	//Os__GB2312_display(3,0,(unsigned char *)"[ȡ ��]�� ��");
	while(1)
	{
		ucKey = UTIL_GetKey(30);
		if((ucKey == 99)||(ucKey == KEY_CLEAR))
		{
			return ERR_CANCEL;
		}else
		if(ucKey == KEY_ENTER)
		{
                //Os__clr_display(255);
                //Os__GB2312_display(0,0,(unsigned char *)"�ȴ���Կ��װ");
                ucResult = SUCCESS;
                break;
		}
	}

        if(ucResult == SUCCESS)
        {
            uiLen = 34;
            memset(aucBuf,0,sizeof(aucBuf));
            memset(aucRecBuf,0,sizeof(aucRecBuf));
            ucResult = KEY_InitCom2();
            if(ucResult == SUCCESS)
                ucResult = KEY_ReceiveData_COM2(aucRecBuf,&uiLen);
            Os__end_com3();
	 }
	if(ucResult == SUCCESS)
	{
            if((uiLen != 16) && (uiLen != 32))
            return ERR_CANCEL;
            else
            {
                hex_asc(aucBuf,aucRecBuf,32);
                memcpy(pucOutData,aucBuf,32);
            }
            //Os__clr_display(255);
            //Os__GB2312_display(0,0,aucBuf);
            //Os__GB2312_display(1,0,&aucBuf[16]);
            MSG_WaitKey(30);
	}
	return ucResult;
}
unsigned char KEY_DownKeyFromCard(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned char aucCardPass[33];
	unsigned char aucInputCode[17];
	unsigned char aucInputCode1[9];
	unsigned char aucInputCode2[9];
	unsigned char ucKeyIndex,ucArrayIndex;
	unsigned int  uiTimesOut;
	unsigned char ucKey;
	DRV_OUT *pxIcc;
	DRV_OUT *pxKey;

	NEW_DRV_TYPE  new_drv;

	if(!(ucResult = UTIL_Is_Trans_Empty()))
	{
		//Os__clr_display(255);
		//Os__GB2312_display(1, 0, (uchar *)"POS���н�������");
		//Os__GB2312_display(2, 0, (uchar *)"���Ƚ���");
		//Os__GB2312_display(2, 0, (uchar *)"�������������");
		Os__xget_key();
		return ERR_CANCEL;
	}
	ucResult=SUCCESS;
	ucArrayIndex = KEYARRAY_GOLDENCARDSH;

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char*)"��������Կ����:");
	//Os__GB2312_display(1,0,(unsigned char*)"(0-2)");
	memset(aucCardPass,0,sizeof(aucCardPass));
	if( UTIL_Input(2,true,1,1,'N',aucCardPass,(unsigned char*)"012") != KEY_ENTER )
	{
		return ERR_CANCEL;
	}

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"��忨>>>");
	//Os__GB2312_display(3,0,(uchar *)"[ȡ ��]�� ��");
/*
	pxIcc = Os__ICC_insert();
	pxKey = Os__get_key();

	uiTimesOut = 60*100;
	Os__timer_start(&uiTimesOut);
	util_Printf("success1\n");
	while(  (uiTimesOut)
	 		&&(pxIcc->gen_status != DRV_ENDED)
      			&&(pxKey->gen_status != DRV_ENDED));
	Os__timer_stop(&uiTimesOut);

	util_Printf("success2\n");
	if(!uiTimesOut)
	{
		Os__abort_drv(drv_icc);
		Os__abort_drv(drv_mmi);
		return ERR_CANCEL;
	}
	else if( pxKey->gen_status == DRV_ENDED)
	{
		Os__abort_drv(drv_icc);
		return ERR_CANCEL;
	}
	else if( pxIcc->gen_status == DRV_ENDED)
	{
		Os__abort_drv(drv_mmi);
	}
*/

	util_Printf("\n1. KEY_DownKeyFromCard Os_Wait_Event begin...");
	ucResult = Os_Wait_Event(KEY_DRV | ICC_DRV , &new_drv, 1000*60);
	//util_Printf("\n2. Os_Wait_Event:%02x",ret);
	//util_Printf("\n3. new_drv.drv_type:%02x",new_drv.drv_type);
	if(ucResult == 0)                                                              
	{                                                                         
		if(new_drv.drv_type == KEY_DRV)                                     
		{                                                                   
			if(new_drv.drv_data.gen_status== DRV_ENDED){
				ucKey=new_drv.drv_data.xxdata[1];
				util_Printf("\n ucKey:%02x.", ucKey);
				if(ucKey==KEY_CLEAR)
				{
					return ERR_CANCEL;
				}
			}
		}  		
		else if(new_drv.drv_type == DRV_TIMEOUT)                            
		{    
			return ERR_CANCEL;			
		}	        
		else if(new_drv.drv_type == ICC_DRV)                                
		{                                                                   
		}                                                                   
		util_Printf("\n OWE_NewDrvtts End.....\n");
	}       

  if(ucResult == SUCCESS)
  {
        ucKeyIndex = aucCardPass[0]-'0';

        //Os__clr_display(255);
        //Os__GB2312_display(0,0,(unsigned char*)"�����뿨����:");
        memset(aucCardPass,0,sizeof(aucCardPass));
        if( UTIL_Input(1,true,8,8,'P',aucCardPass,NULL) != KEY_ENTER )
        {
    	    ucResult = ERR_CANCEL;
        }
    }

	if(ucResult == SUCCESS)
      {
          memset(aucInputCode,0,sizeof(aucInputCode));
          asc_bcd(aucInputCode,4,aucCardPass,8);
          memcpy(&aucInputCode[4],"\xFF\xFF\xFF\xFF",4);
          {
              int i;
              util_Printf("\n+++���뿨�����:\n");
              for(i=0;i<8;i++)
              {
                  util_Printf("%02x ",aucInputCode[i]);
              }
              util_Printf("\n");
          }
      }

  /*ѡ��MF�ļ�*/
  util_Printf("\n+++ѡ��MF�ļ�:%02x\n",ucResult);
    if(ucResult == SUCCESS)
    {
        ucResult = PBOC_ISOSelectFilebyAID(0,(uchar *)"1PAY.SYS.DDF01",14);
        util_Printf("1PAY.SYS.DDF01=%02x\n",ucResult);
    }

    /*������֤*/
    if(ucResult == SUCCESS)
    {
        ucResult = PBOC_ISOVerifyPIN(0,0x08,aucInputCode);
        util_Printf("PBOC_ISOVerifyPIN = %02x\n",ucResult);
    }

	if(ucResult == SUCCESS)
	{
		while(1)
	    {
			//Os__clr_display(255);
			//Os__GB2312_display(0,0,(unsigned char *)"��Կ��װ��ʽ:");
			//Os__GB2312_display(1,0,(unsigned char *)"1--���� ");
			//Os__GB2312_display(2,0,(unsigned char *)"2--���� ");
			ucKey = Os__xget_key();
			switch(ucKey)
			{
				util_Printf("��Կ��װ��ʽ:%02x\n",ucKey);
			case '1':
				while(1)
			  {
					//Os__clr_display(255);
		      //Os__GB2312_display(0,0,(unsigned char*)"��������Կ:");
		      memset(aucCardPass,0,sizeof(aucCardPass));
		      ucResult = OSUTIL_Input(0,1,16,32,'A',aucCardPass,NULL);
		      if(ucResult != SUCCESS )
			    {
				    ucResult = ERR_CANCEL;
				    break;
			    }else
			    if((strlen((char *)aucCardPass) != 16)
					 &&(strlen((char *)aucCardPass) != 32))
					{
						//Os__clr_display(255);
					  //Os__GB2312_display(1, 0, (uchar * )"������Կ��������");
					  //Os__GB2312_display(2, 0, (uchar * )"   ����������");
					  MSG_WaitKey(8);
	            	  continue;
					}else
					  break;
				}
				break;
			case '2':
				ucResult=KEY_UpdataMKeyFRS232(aucCardPass);
				if(ucResult != SUCCESS)
				{
					UTIL_Beep();
					//Os__clr_display(255);
					//Os__GB2312_display(1, 0, (uchar * )"  ��Կ¼��ʧ��");
					//Os__GB2312_display(2, 0, (uchar * )"   ������¼��");
					MSG_WaitKey(8);
					continue;
				}else
				{
					//Os__clr_display(255);
					//Os__GB2312_display(1, 0, (uchar * )"  �ɹ�¼����Կ");
				}
				break;
			case KEY_CLEAR:
				ucResult = ERR_CANCEL;
				break;
			default:
				continue;
			}
			break;
		 }
	}

    if(ucResult == SUCCESS)
    {
        memset(aucInputCode,0,sizeof(aucInputCode));
        asc_hex(aucInputCode,16,aucCardPass,32);

        {
            int i;
            util_Printf("++ASC_HEX\n");
            for(i=0;i<8;i++)
            {
                util_Printf("%02x",aucInputCode[i]);
            }
            util_Printf("\n");
        }
    }

    /*�ڲ���֤*/
    util_Printf("++�ڲ���֤=%02x\n",ucResult);
    if(ucResult == SUCCESS)
    {
        ucResult = PBOC_ISOInternalAttestation(0,aucInputCode,0x16);
        util_Printf("PBOC_ISOInternalAttestation = %02x\n",ucResult);
    }

    if(ucResult == SUCCESS)
    {
        memset(aucInputCode,0,sizeof(aucInputCode));
        memcpy(aucInputCode,Iso7816Out.aucData,16);
        {
            int i;
            util_Printf("++Iso7816Out\n");
            for(i=0;i<8;i++)
            {
                util_Printf("%02x ",aucInputCode[i]);
            }
            util_Printf("\n");
        }
    }

  memset(aucInputCode1,0,sizeof(aucInputCode1));
  memset(aucInputCode2,0,sizeof(aucInputCode2));
  memcpy(aucInputCode1,aucInputCode,8);
  memcpy(aucInputCode2,&aucInputCode[8],8);
  util_Printf("��װ��Կ+++\n");
  if(ucResult == SUCCESS)
	{
		ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
						ucKeyIndex,aucInputCode1,aucInputCode1);
	}
	if(ucResult == SUCCESS)
	{
		ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
						ucKeyIndex,ucKeyIndex,aucInputCode1);
	}

	if(ucResult == SUCCESS)
	{
		ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
						ucKeyIndex+3,aucInputCode2,aucInputCode2);
	}
	if(ucResult == SUCCESS)
	{
		ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
						ucKeyIndex+3,ucKeyIndex+3,aucInputCode2);
	}

	/********************************************************************/
	{
		int usI;
		unsigned char aucHexData1[8];
		memset(aucHexData1,0,sizeof(aucHexData1));
		memcpy(aucHexData1,"12345678",8);
		PINPAD_47_Encrypt8ByteSingleKey(
						ucArrayIndex,
						ucKeyIndex,
						aucHexData1,
						aucHexData1);
  		util_Printf("У����Կ+++\n");
		for(usI=0;usI<8;usI++)
			util_Printf("%02x ",aucHexData1[usI]);
		util_Printf("\n");
	}
	/********************************************************************/
	util_Printf("У����Կ+++%02x\n",ucResult);
	if(ucResult == SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"��Կ���سɹ�!");
	}
	else
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"��Կ����ʧ��!");
	}
	UTIL_GetKey(10);
  return ucResult;
}



unsigned char EX_KEY_InputKey_Double(unsigned char ucKeyIndex)
{
    unsigned char aucAscData[33];
    unsigned char aucAscData1[17];
    unsigned char aucAscData2[17];
    unsigned char aucHexData[9];
    unsigned char aucHexData1[9];
    unsigned char ucResult;
    unsigned char ucArrayIndex;

    /*ǰ���鷽�ڵ�һarray�������鷽�ڶ�array
        0,1�ֱ���pinkey��mackey
        2��3��4��5��6�ֱ�master key
    */

    ucArrayIndex = KEYARRAY_GOLDENCARDSH;

    memset(aucAscData,0,sizeof(aucAscData));
    memset(aucAscData1,0,sizeof(aucAscData1));
    memset(aucAscData2,0,sizeof(aucAscData2));
    memset(aucHexData,0,sizeof(aucHexData));
    memset(aucHexData1,0,sizeof(aucHexData1));

    //memcpy(aucAscData, "\x34\x33\x42\x39\x34\x43\x46\x44\x43\x32\x34\x33\x45\x43\x32\x41\x35\x44\x32\x33\x33\x37\x33\x42\x36\x31\x43\x44\x34\x46\x38\x35", 32);
    memcpy(aucAscData, "\x32\x43\x44\x43\x33\x44\x41\x44\x42\x39\x37\x35\x41\x37\x35\x44\x41\x32\x33\x45\x45\x36\x43\x31\x36\x34\x45\x30\x30\x45\x46\x34",	32);
    util_Printf("aucAscData=%s\n",aucAscData);

    memcpy(aucAscData1 , aucAscData , 16);
    memcpy(aucAscData2 , &aucAscData[16] , 16);

    asc_hex(aucHexData,8,aucAscData1,16);
    asc_hex(aucHexData1,8,aucAscData2,16);

    //---------------
    ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
                    ucKeyIndex,aucHexData,aucHexData);
    if(ucResult != SUCCESS)
        return(ucResult);
    ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
                    ucKeyIndex,ucKeyIndex,aucHexData);
    if(ucResult != SUCCESS)
        return(ucResult);
    //--------------------

    ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
                    ucKeyIndex+3,aucHexData1,aucHexData1);
    if(ucResult != SUCCESS)
        return(ucResult);
    ucResult = PINPAD_42_LoadSingleKeyUseSingleKey(ucArrayIndex,
                    ucKeyIndex+3,ucKeyIndex+3,aucHexData1);
    //-------------------

    memcpy(aucHexData,"88888888",8);
    ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
                    ucKeyIndex,aucHexData,aucHexData);
#ifdef TEST
util_Printf("----------Key---double---00--aucHexData--\n");
    for(ucI=0;ucI<8;ucI++)
    {
        util_Printf("%02x,",aucHexData[ucI]);
    }
    util_Printf("\n");
#endif
    memcpy(aucHexData1,"88888888",8);
    ucResult = PINPAD_47_Encrypt8ByteSingleKey(ucArrayIndex,
                    ucKeyIndex+3,aucHexData1,aucHexData1);
#ifdef TEST
util_Printf("----------Key---double---00--aucHexData1--\n");
    for(ucI=0;ucI<8;ucI++)
    {
        util_Printf("%02x,",aucHexData1[ucI]);
    }
    util_Printf("\n");
#endif
    return(ucResult);
}
