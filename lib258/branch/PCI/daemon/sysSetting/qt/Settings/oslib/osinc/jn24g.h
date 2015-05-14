#ifndef __JN24G_H__
#define __JN24G_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef JN24GDATA
#define JN24GDATA extern
#endif

//define error type
#define JN24GERR_SUCCESS		0//�ɹ�
#define JN24GERR_NOCARD			1//�޿�
#define JN24GERR_INVALIDCARD	2//��Ч��
#define JN24GERR_PARAM			3//��Ч����
#define JN24GERR_TIMEOUT		4//��ʱ
#define JN24GERR_RFINT			5//rf�ж�
#define JN24GERR_NOOP			6//����ִ�иö���
#define JN24GERR_BUSY			7//�ӻ�æµ
#define JN24GERR_NOSP			8//��֧�ָò���
#define JN24GERR_UNKNOW			9//δ֪����
#define JN24GERR_RFU			10//����

#define JN24GERR_CRC			20
#define JN24GERR_ADDRESS		21
#define JN24GERR_COMMAND		22
#define JN24GERR_LENGTH			23
#define JN24GERR_WAIT 			0xFF

//define reader command 
#define JN24G_CMD_COMTEST		0//����ͨѶ����
#define JN24G_CMD_CHECKCARD		1//�ҿ�
#define JN24G_CMD_TRANSFER		2//���ݴ���
#define JN24G_CMD_ENDTRANS		3//�������ݴ���
#define JN24G_CMD_READERCFG		4//���ôӻ�����
#define JN24G_CMD_CARDCFG		5//���ÿ�Ƭ����

//define pos command
#define JN24G_POS_CMD_COMTEST		0x00
#define JN24G_POS_CMD_CHECKCARD		0x10
#define JN24G_POS_CMD_TRANSFER		0x11
#define JN24G_POS_CMD_ENDTRANS		0x12
#define JN24G_POS_CMD_READERCFG		0x30
#define JN24G_POS_CMD_CARDCFG		0x31


#define JN24G_CMD_HEAD_LEN			0x02
#define JN24G_DEFAULT_WAIT_TIME		100




typedef struct{
	unsigned int uiSendLen;
	unsigned char aucSendData[256];
}JN24G_CMD;

JN24GDATA JN24G_CMD g_jn24g_cmd;

typedef struct{
	unsigned char ucAddr;
	unsigned int uiLen;
	unsigned char aucData[256];
}JN24G_REVCMD;

JN24GDATA JN24G_REVCMD JN24gRevCmd;

typedef struct{
	unsigned char ucAddr;
	unsigned int uiSendLen;
	unsigned char aucSendData[256];
}JN24G_SENDCMD;

JN24GDATA JN24G_SENDCMD JN24gSendCmd;

typedef struct{
	unsigned char ucAddr;
	unsigned char ucRetStatus;
	unsigned int uiLen;
	unsigned char aucData[256];
}JN24G_RETDATA;

JN24GDATA JN24G_RETDATA JN24gRetData;

typedef struct
{
	unsigned char ucCmd;
	unsigned char (*pFct)( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
}JN24GCMDTBL;

#if 0
unsigned char JN24G_CalcLRC( unsigned char *pucData,unsigned int uiLen );

unsigned char JN24G_ComTest( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
unsigned char JN24G_CheckCard( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
unsigned char JN24G_TransFer( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
unsigned char JN24G_EndTransFer( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
unsigned char JN24G_ReaderCfg( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );
unsigned char JN24G_CardCfg( unsigned char ucAddr,unsigned char *pucInData,unsigned int uiInLen );

unsigned char JN24G_Proc( unsigned char ucAddr,unsigned char ucCmd,unsigned char *pucInData,unsigned int uiInLen,
                          unsigned char *pucRetData,unsigned int *pucRetLen );
unsigned char JN24G_CheckCardEx( unsigned char *pucOutData );
#endif
int JN24_Proc(unsigned char *pin, int in_len, unsigned char *pout, int *out_len);


#ifdef __cplusplus
}
#endif

#endif

////////////// END //////////////////////

