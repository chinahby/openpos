/*
	Test System
--------------------------------------------------------------------------
	FILE  pboc.c
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2003-08-04		Xiaoxi Jiang
    Last modified :	2003-08-04		Xiaoxi Jiang
    Module :
    Purpose :
        Source file.

    List of routines in file :

    File history :
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osicc.h"
#include "smart.h"
#include "libicc.h"

ISO7816IN	Iso7816In;
ISO7816OUT	Iso7816Out;


/*
����������������������
��ڲ�������
����ֵ����
*/
void PBOC_SetIso7816Out(void)
{
    Iso7816Out.uiLen = sizeof(Iso7816Out.aucData);
    memset(Iso7816Out.aucData,0,sizeof(Iso7816Out.aucData));
}


/*
�����������ж�IC���ķ���ֵ
��ڲ�����ucReaderΪ����λ��
����ֵ������ִ��״̬
*/
unsigned char PBOC_ISOCheckReturn(unsigned char ucReader)
{
    unsigned char ucResult,ucI;

    Iso7816Out.ucSW1 = Iso7816Out.aucData[Iso7816Out.uiLen-2];
    Iso7816Out.ucSW2 = Iso7816Out.aucData[Iso7816Out.uiLen-1];
#if 1
    Uart_Printf("Iso7816Out.ucSW1=%02x\n",Iso7816Out.ucSW1);
    Uart_Printf("Iso7816Out.ucSW2=%02x\n",Iso7816Out.ucSW2);
#endif

    switch (Iso7816Out.ucSW1)
    {
    case 0x90:
#if 1
        Uart_Printf("PBOC_ISOCheckReturn return %x\n", Iso7816Out.uiLen);
        for (ucI=0;ucI<Iso7816Out.uiLen;ucI++)
            Uart_Printf("%02x ",Iso7816Out.aucData[ucI]);
        Uart_Printf("\n********\n");
#endif
        return(SUCCESS);
    case 0x6C:
        PBOC_SetIso7816Out();
        Iso7816In.ucP3  = Iso7816Out.ucSW2;
        if ( (ucResult = SMART_ISO(ucReader,
                                   SMART_ASYNC,
                                   (unsigned char *)&Iso7816In,
                                   Iso7816Out.aucData,
                                   &Iso7816Out.uiLen,
                                   TYPE2)) != OK)
            return(ucResult);
        return(PBOC_ISOCheckReturn(ucReader));

    case 0x61:
    case 0x9F:
        return(PBOC_ISOGetResponse(ucReader,Iso7816Out.ucSW2));
        //return(UTIL_CheckCardErr(Iso7816Out.ucSW1,Iso7816Out.ucSW2));
    default:
        //return(UTIL_CheckCardErr(Iso7816Out.ucSW1,Iso7816Out.ucSW2));
        break;
    }
}



/*
����������ѡ��IC�����ļ�
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ���͸�IC�������ݣ�ucDataLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_SelectFile(unsigned char ucFlag,unsigned char ucReader,
                              unsigned char *pucData,unsigned char ucDataLen)
{
    unsigned char	ucResult;
    unsigned char ucI;
    unsigned char * p ;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xA4;
    if (ucFlag)
        Iso7816In.ucP1  = 0x04;
    else
        Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 00;
    Iso7816In.ucP3 = ucDataLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucDataLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

#if 1
    p = (unsigned char *)&Iso7816In;
    for (ucI=0;ucI<5+ucDataLen;ucI++)
        Uart_Printf("%02x",p[ucI]);
    Uart_Printf("\n");
#endif

    PBOC_SetIso7816Out();

    // Uart_Printf("=========1 SMART_ISO\n");
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
    {
        Uart_Printf("!!!!!!SMART_ISO ret=%d\n", ucResult);
        return(ucResult);
    }
    //Uart_Printf("==========2 SMART_ISO\n");

#if 1
    Uart_Printf("%x\n", Iso7816Out.uiLen);
    for (ucI=0;ucI<Iso7816Out.uiLen+2;ucI++)
        Uart_Printf("%02x ",Iso7816Out.aucData[ucI]);
    Uart_Printf("\n********\n");
#endif

    return(PBOC_ISOCheckReturn(ucReader));
}



/*
������������ȡIC����������
��ڲ�����ucReaderΪ����λ�ã�ucLenΪ���ص����ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_ISOGetResponse(unsigned char ucReader,
                                  unsigned char ucLen)
{
    unsigned char ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xC0;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3  = ucLen;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);
    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������ݶ��ļ���־����ȡ�ļ���¼
��ڲ�����ucReaderΪ����λ�ã�ucSFIΪ���ļ���־����ucRecordNumberΪ��¼��
����ֵ������ִ��״̬
*/
unsigned char PBOC_ISOReadRecordbySFI(unsigned char ucReader,
                                      unsigned char ucSFI,unsigned char ucRecordNumber)
{
    unsigned char	ucResult;


    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xB2;
    Iso7816In.ucP1  = ucRecordNumber;
    Iso7816In.ucP2  = (ucSFI << 3) | 0x04;
    Iso7816In.ucP3  = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);



    return(PBOC_ISOCheckReturn(ucReader));
}

/*
������������ȡ��ǰ�ļ��ļ�¼
��ڲ�����ucReaderΪ����λ�ã�ucSFIΪ���ļ���־����ucRecordNumberΪ��¼��
����ֵ������ִ��״̬
*/
unsigned char PBOC_ReadRecord(unsigned char ucReader,unsigned char ucRecordNumber)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xB2;
    Iso7816In.ucP1  = ucRecordNumber;
    Iso7816In.ucP2  = 0x04;
    Iso7816In.ucP3  = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);
    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������ݶ��ļ���־����ȡ�ļ�
��ڲ�����ucReaderΪ����λ�ã�ucSFIΪ���ļ���־����ucOffsetΪƫ������ucInLenΪ��ȡ�����ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_ISOReadBinarybySFI(unsigned char ucReader,
                                      unsigned char ucSFI,unsigned char ucOffset,unsigned char ucInLen)
{
    unsigned char	ucResult;
    unsigned char ucI;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xB0;
    Iso7816In.ucP1  = ucSFI; //zy
    //Iso7816In.ucP1  = ucSFI | 0x80;
    Iso7816In.ucP2  = ucOffset;
    Iso7816In.ucP3  = ucInLen;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);


    return(PBOC_ISOCheckReturn(ucReader));
}

/*
������������SAM����ȡ˰�ؿ���ţ��������mac1
��ڲ�����ucReaderΪ����λ��
����ֵ������ִ��״̬
*/
unsigned PBOC_Get_RegisterNb(unsigned char ucReader)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF0;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3  = 0x10;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������û����в���MAC2
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_Get_RegisterSign(unsigned char ucReader,
                               unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xE4;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x04;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
����������˰�ؿ�ע�ắ��
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_Terminal_Register(unsigned char ucReader,
                                unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF1;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x08;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
����������˰�ؿ���������
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_Pin_UNBlock(unsigned char ucReader,
                          unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x84;
    Iso7816In.ucIns = 0x24;
    Iso7816In.ucP1  = 0x01;
    Iso7816In.ucP2  = 0x01;
    Iso7816In.ucP3 = 0x0c;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x08;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
����������У��˰�ؿ���PIN
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_VerifyFiscal_Pin(unsigned char ucReader,
                               unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF9;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x08;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}



/****added by guo.wl*/
//�ַ��û����еķ�Ʊ�ţ�һ�ηַ�һ��Ʊ��

/*
�����������ַ��û����еķ�Ʊ�ţ�һ�ηַ�һ��Ʊ��
��ڲ�����ucReaderΪ����λ��
����ֵ������ִ��״̬
*/
unsigned char PBOC_DitributeInvoice(unsigned char ucReader)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xE9;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = 0x16;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);

    //��Ϊ ISSUE_INVOICE ʱ ���޺� DISTRIBUTE_INVOICE_NB ʱ �޷�Ʊ�ַ� �ķ����붼�� 0x94 + 0x01
    //���������ȼ�һ��Ԥ�ж�
    Iso7816Out.ucSW1 = Iso7816Out.aucData[Iso7816Out.uiLen-2];
    Iso7816Out.ucSW2 = Iso7816Out.aucData[Iso7816Out.uiLen-1];
    if (Iso7816Out.ucSW1==0x94 && Iso7816Out.ucSW2==0x01)
    {
        return NoInvoiceRoll;
    }
    else
    {
        return(PBOC_ISOCheckReturn(ucReader));
    }
}
//����˰�ؿ�¼�뷢Ʊ��

/*
��������������˰�ؿ�¼�뷢Ʊ��
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_InputInvoiceNB(unsigned char ucReader,
                                  unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF7;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
����������У���û�����PIN
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�
ucPinIndexΪ����ֵ��ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_VerifyUser_Pin(unsigned char ucReader,
                             unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0x20;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = ucPinIndex;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    //��Ϊ VERIFY ʱ ��Կ���ʹ��� �� READ BINARY, READ RECORD ʱ �ļ����ʹ��� �ķ����붼�� 0x69 + 0x81
    //���������ȼ�һ��Ԥ�ж�
    Iso7816Out.ucSW1 = Iso7816Out.aucData[Iso7816Out.uiLen-2];
    Iso7816Out.ucSW2 = Iso7816Out.aucData[Iso7816Out.uiLen-1];
    if (Iso7816Out.ucSW1==0x69 && Iso7816Out.ucSW2==0x81)
    {
        return KeyTypeErr;
    }
    else
    {
        return(PBOC_ISOCheckReturn(ucReader));
    }
}

/*
����������ͨ��˰�ؿ����ɷ�α˰����
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_IssueInvoice(unsigned char ucReader,
                                unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF2;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x08;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    //��Ϊ ISSUE_INVOICE ʱ ���޺� DISTRIBUTE_INVOICE_NB ʱ �޷�Ʊ�ַ� �ķ����붼�� 0x94 + 0x01
    //���������ȼ�һ��Ԥ�ж�
    Iso7816Out.ucSW1 = Iso7816Out.aucData[Iso7816Out.uiLen-2];
    Iso7816Out.ucSW2 = Iso7816Out.aucData[Iso7816Out.uiLen-1];
    if (Iso7816Out.ucSW1==0x94 && Iso7816Out.ucSW2==0x01)
    {
        return IssInvoiceValLimitOver;
    }
    else
    {
        return(PBOC_ISOCheckReturn(ucReader));
    }
}

/*
����������ͨ��˰�ؿ��������ս���ǩ��
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_DailyCollectSign(unsigned char ucReader,
                                    unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xFB;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x80;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������¼�¼
��ڲ�����ucReaderΪ����λ�ã�ucSFIΪ���ļ���־����ucRecordNumberΪ��¼�ţ�
pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_UpdateRecord(unsigned char ucReader,
                           unsigned char ucSFI,unsigned char ucRecordNumber,
                           unsigned char *pucData,unsigned char ucLen,unsigned char ucFlag)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xDC;
    Iso7816In.ucP1  = ucRecordNumber;
    if (ucFlag)
        Iso7816In.ucP2  = (ucSFI << 3) | 0x04;
    else
        Iso7816In.ucP2  = 0x04;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}


/*
������������˰�����ȡ8�ֽ������
��ڲ�����ucReaderΪ����λ�ã�ucLenΪ���ص����ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_GetChallenge(unsigned char ucReader,
                                unsigned char ucLen)
{
    unsigned char ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0x84;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3  = ucLen;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE2)) != OK)
        return(ucResult);
    return(PBOC_ISOCheckReturn(ucReader));
}


/****************************************************************************
* ���ƣ�CARDCMD_UpdateBinary
* ���ܣ�Update Binary �޸Ķ������ļ�
* ��ڲ�����ucReader�������ţ�ucSFI�����ļ���ʾ����usOffset��ƫ����
			*pucData���޸�����ָ�룻ucLen�����ݳ��ȣ�ucFlag���Ƿ񰴶��ļ���ʾ��
* ���ڲ�����0����������0���쳣
****************************************************************************/
unsigned char PBOC_UpdateBinary(unsigned char ucReader,unsigned char ucSFI,
                                unsigned short usOffset,unsigned char *pucData,unsigned char ucLen,unsigned char  ucFlag)
{
    unsigned char ucResult;
    unsigned char aucBuf[2];
    unsigned short uiI;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0xD6;

    if ( ucFlag )
    {
        Iso7816In.ucP1 = ucSFI | 0x80;
        Iso7816In.ucP2 = usOffset;
    }
    else
    {
        short_tab(&Iso7816In.ucP1,2,&usOffset);
        Uart_Printf("Iso7816In.ucP1=%02x\n",Iso7816In.ucP1);
        Uart_Printf("Iso7816In.ucP2=%02x\n",Iso7816In.ucP2);
    }

    Uart_Printf("***************PBOC*************************\n");
    Uart_Printf("ucLen=%d\n",ucLen);
    for (uiI=0;uiI<ucLen;uiI++)
    {
        Uart_Printf("%02x ",pucData[uiI]);
        if (uiI%30==0)
            Uart_Printf("\n");
    }
    Uart_Printf("\n");


    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}


/*
����������ͨ���û������������걨
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_DeclareDuty(unsigned char ucReader,
                          unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF4;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x8E;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������걨����д���û���
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_DataCollect(unsigned char ucReader,
                          unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xE6;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������¼�ػض���Ϣ
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_UpdateControls(unsigned char ucReader,
                             unsigned char *pucData,unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0xC0;
    Iso7816In.ucIns = 0xF6;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = 0x00;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}


/*
�������������ڲ���֤
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�
ucPinIndexΪ����ֵ��ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned PBOC_InternalAuthentication(unsigned char ucReader,
                                     unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0x88;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = ucPinIndex;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;//0x08;
    Iso7816In.aucData[Iso7816In.ucP3+1] = 0x00;
    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE4)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

/*
�������������ⲿ��֤
��ڲ�����ucReaderΪ����λ�ã�pucDataΪ�����������ݣ�
ucPinIndexΪ����ֵ��ucLenΪ���ݳ���
����ֵ������ִ��״̬
*/
unsigned char PBOC_ExternalAuthentication(unsigned char ucReader,
        unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen)
{
    unsigned char	ucResult;

    Iso7816In.ucCla = 0x00;
    Iso7816In.ucIns = 0x82;
    Iso7816In.ucP1  = 0x00;
    Iso7816In.ucP2  = ucPinIndex;
    Iso7816In.ucP3 = ucLen;
    memcpy(&Iso7816In.aucData[0],pucData,ucLen);
    Iso7816In.aucData[Iso7816In.ucP3] = 0x00;

    PBOC_SetIso7816Out();
    if ( (ucResult = SMART_ISO(ucReader,
                               SMART_ASYNC,
                               (unsigned char *)&Iso7816In,
                               Iso7816Out.aucData,
                               &Iso7816Out.uiLen,
                               TYPE3)) != OK)
        return(ucResult);

    return(PBOC_ISOCheckReturn(ucReader));
}

#if 0
/*
�������������IC��������Ϣ������IC��������ʱʹ��
��ڲ�����ucReaderΪ����λ��
����ֵ������ִ��״̬
*/
extern unsigned char respone_data[256];
unsigned char PBOC_ISOCheckReturn_test(unsigned char ucReader)
{
    unsigned char ucResult,ucI,buf[100];

    memset(buf,0,sizeof(buf));
    Iso7816Out.ucSW1 = Iso7816Out.aucData[Iso7816Out.uiLen-2];
    Iso7816Out.ucSW2 = Iso7816Out.aucData[Iso7816Out.uiLen-1];

    Uart_Printf("Iso7816Out.ucSW1=%02x\n",Iso7816Out.ucSW1);
    Uart_Printf("Iso7816Out.ucSW2=%02x\n",Iso7816Out.ucSW2);

    display_array(respone_data, respone_data[3]+2);
    Os__clr_display(255);
    Os__display_recvdata(0, 2,  Iso7816Out.aucData, Iso7816Out.uiLen);
    MSG_WaitKey(3);

    switch (Iso7816Out.ucSW1)
    {
    case 0x90:
        if (Iso7816Out.ucSW2 == 0x60)
            return UNKNOWERR;
        return(SUCCESS);
    case 0x6C:
        PBOC_SetIso7816Out();
        Iso7816In.ucP3  = Iso7816Out.ucSW2;
        if ( (ucResult = SMART_ISO(ucReader,
                                   SMART_ASYNC,
                                   (unsigned char *)&Iso7816In,
                                   Iso7816Out.aucData,
                                   &Iso7816Out.uiLen,
                                   TYPE2)) != OK)
            return(ucResult);
        return(PBOC_ISOCheckReturn_test(ucReader));
    case 0x61:
    case 0x9F:
        if (Iso7816Out.ucSW2 == 0xAF)
        {
            return UNKNOWERR;
        }
        else
            return(PBOC_ISOGetResponse(ucReader,Iso7816Out.ucSW2));
    case 0x60:
        if (Iso7816Out.ucSW2 == 0x60)
            return(SUCCESS);
    default:
        return(UTIL_CheckCardErr(Iso7816Out.ucSW1,Iso7816Out.ucSW2));
    }
}

#endif

