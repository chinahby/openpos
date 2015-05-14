/*
	SAND China
--------------------------------------------------------------------------
	FILE  LoadKey.c								(Copyright SAND 2009)
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2009-03-02		Lote
    Last modified :	2009-03-02		Lote
    Module :  Shanghai Golden Card Network
    Purpose :
        Head file for LoadKey.

    List of routines in file :

    File history :
*/

#define COM_ONE			1
#define COM_TWO		2
#define ERRName_Len								32

#define ERR_RECVTIMEOUT					0x01
#define ERR_COMMS_DATALEN			0x02
#define ERR_DATA_ETX							0x03
#define ERR_REVDATA_LEN					0x04
#define ERR_COMMS_STX						0x05
#define ERR_COMMS_DATA					0x06
#define ERR_COMMS_DATAETX			0x07
#define ERR_COMMS_SANDLEN			0x08

typedef struct
	{
		unsigned char ucErrorType;
		unsigned char ucErrorNo;		
		unsigned char aucErrorName[ERRName_Len];
	}LoadKEYError;

const	LoadKEYError	COMMErr[]=
	{
/*1*/{'0',0x01,"���ճ�ʱ"},
/*2*/{'0',0x02,"���ݹ���"},
/*3*/{'0',0x03,"���ݳ���"},
/*4*/{'0',0x04,"�������ݹ���"},
/*5*/{'0',0x05,"STX����"},
/*6*/{'0',0x06,"���ݽ��ճ���"},
/*7*/{'0',0x07,"����У���"},
/*8*/{'0',0x08,"Ӧ�����ݹ���"},
/*8*/{'0',0xFF,"���ڲ���δ֪����"}
/*8*/
	};