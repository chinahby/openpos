#include <include.h>
#include <global.h>
#include <xdata.h>
#include <smart.h>
#include <osmifare.h>
#include "new_drv.h"


// Untouch_Menu
unsigned char EXTrans_CheckSandCard(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned char ucStatus,ucLen,aucOutData[300],aucBuf[20];
	unsigned int    uiTimesOut;
	unsigned int    uiI;

	Uart_Printf("\n 20120625-000001  Untouch_Menu  .... Begin\n");
	
	if(Untouch_CheckCardApp()!=SUCCESS)
		return ERR_NOTSUCHAPP;

	if(ucResult==SUCCESS)
		ucResult=CFG_CheckFlag();

	Uart_Printf("\n 20120625-000002  Untouch_Menu  .... [%02x]\n",ucResult);	
	
	if(ucResult==SUCCESS)
		ucResult=Untouch_ReadCardInfo();

	Uart_Printf("\n 20120625-000003  Untouch_Menu  .... [%02x]\n",ucResult);	
	//if(ucResult==ERR_NOTSUCHAPP)
	//	return ucResult;
	if(ucResult==SUCCESS)
		ucResult=CFG_SetBatchNumber();

	Uart_Printf("\n 20120625-000004  Untouch_Menu  .... [%02x]\n",ucResult);	
	
	if(ucResult==SUCCESS)
		ucResult=CFG_CheckCardValid();

	Uart_Printf("\n 20120625-000005  Untouch_Menu  .... [%02x]\n",ucResult);	
	if(ucResult==SUCCESS)
		ucResult=Untouch_CheckRedo();

	Uart_Printf("\n 20120625-000006  Untouch_Menu  .... [%02x]\n",ucResult);	

	if(ucResult==SUCCESS)
	{
		if(RunData.ucMustReturn)
		{
			if(DataSave1.CardTable.ucCountType!='1'||RunData.aucFunFlag[2]!=1)
				ucResult= ERR_MUSTRETURN;
		}
	}
	Uart_Printf("\n 20120625-000007  Untouch_Menu  .... [%02x]\n",ucResult);	

	return ucResult;
}



/*

1.�ǽӴ�������, ��Card
if (success){
3.��ʾ�˵�
4.ѡ����
5.��ʾ����Ի���
6.�����߳̽���
}else
{
 ��ʾ��������
}
*/
