/*
    Operating System
--------------------------------------------------------------------------
    FILE  osmifire.h
--------------------------------------------------------------------------
*/

#ifndef __OSMIFARE_H
#define __OSMIFARE_H

#include <osdriver.h>
#include <osdrv.h>


#define OSMIFARE_ERR_SUCCESS 				0 	//��ȷ
#define OSMIFARE_ERR_NOTAG					1 	//�޿�
#define OSMIFARE_ERR_CRC					2 	//��ƬCRC У�����
#define OSMIFARE_ERR_EMPTY 					3	//��ֵ�������
#define OSMIFARE_ERR_AUTH					4	//��֤���ɹ�
#define OSMIFARE_ERR_PARITY					5	//��Ƭ��żУ�����
#define OSMIFARE_ERR_CODE					6	//ͨѶ����(BCC У���)
#define OSMIFARE_ERR_SERNR					8	//��Ƭ���кŴ���(anti-collision ����)
#define OSMIFARE_ERR_SELECT					9	//��Ƭ���ݳ����ֽڴ���(SELECT ����)
#define OSMIFARE_ERR_NOTAUTH				10	//��Ƭû��ͨ����֤
#define OSMIFARE_ERR_BITCOUNT				11	//�ӿ�Ƭ���յ���λ������
#define OSMIFARE_ERR_BYTECOUNT				12	//�ӿ�Ƭ���յ����ֽ������󣨽���������Ч��
#define OSMIFARE_ERR_REST					13	//����restore ��������
#define OSMIFARE_ERR_TRANS					14	//����transfer ��������
#define OSMIFARE_ERR_WRITE					15	//����write ��������
#define OSMIFARE_ERR_INCR					16	//����increment ��������
#define OSMIFARE_ERR_DECR					17	//����decrement ��������
#define OSMIFARE_ERR_READ					18	//����read ��������
#define OSMIFARE_ERR_LOADKEY				19	//����LOADKEY ��������
#define OSMIFARE_ERR_FRAMING				20	//FM1715 ֡����
#define OSMIFARE_ERR_RESQ					21	//����req ��������
#define OSMIFARE_ERR_SEL					22	//����sel ��������
#define OSMIFARE_ERR_ANTICOLL				23	//����anticoll ��������
#define OSMIFARE_ERR_INTIVAL				24	//���ó�ʼ����������
#define OSMIFARE_ERR_READVAL				25	//���ø߼�����ֵ��������
#define OSMIFARE_ERR_DESELECT				26
#define OSMIFARE_ERR_CMD	 				42	//�������
#define OSMIFARE_ERR_INIT					43 	//��ʼ��ʧ��
#define OSMIFARE_ERR_UNKNOWN	 			44	//Ϊ�ҵ�Ӳ��
#define OSMIFARE_ERR_COLLISION	 			45	//λ��ͻ
#define OSMIFARE_ERR_FRAME		 			46	//Frame ����
#define OSMIFARE_ERR_ATS_LEN	 			47	//Ats ���ȴ���
#define OSMIFARE_ERR_GETKEYVER	 			48	//DESFire��ȡ��Կ�汾����
#define OSMIFARE_ERR_COMPARE	 			49	//�Ƚϳ���

#define OSMIFARE_CARDID_MAX_LEN				12
#define OSMIFARE_ATS_MAX_LEN				128

#define OSMIFARE_CARD_TYPE_A				0		//TypeA ģʽ
#define OSMIFARE_CARD_TYPE_B				1		//TypeB ģʽ

#define OSMIFARE_CARD_CLASS_MEMERY			0		//M1 M4
#define OSMIFARE_CARD_CLASS_CPU				1		//Desfire ,double interface card,mifare prox card

#define OSMIFARE_AUTH_TYPE_A				0		//TypeA ��׼ģʽ
#define OSMIFARE_AUTH_TYPE_SH				1		//�Ϻ�ģʽ

#define OSMIFARE_KEY_LEN					6
#define OSMIFARE_KEY_TYPE_A					0		//KeyA
#define OSMIFARE_KEY_TYPE_B					1		//KeyB

#define OSMIFARE_REQUEST_REQA				0x26	//request stand
#define OSMIFARE_REQUEST_WUPA				0x52	//wake up

#define OSMIFARE_CHECK_PARITYEN				0x01	//У����żλ
#define OSMIFARE_CHECK_ODDPARITY			0x02	//��У��
#define OSMIFARE_CHECK_TXCRCEN				0x04	//����ʱУ��crc
#define OSMIFARE_CHECK_RXCRCEN				0x08	//����ʱУ��crc

#define OSMIFARE_CHECK_RXTXPARITYEN			0x0F	//���ͽ���ʱУ��crc,����żУ��
#define OSMIFARE_CHECK_TXPARITYEN			0x07	//����ʱУ��crc,����żУ��,���ղ�У��crc

// commands to execute in the mifare reader
#define OSMIFARE_ANTI_ORDER		        	0
#define OSMIFARE_SELECT_ORDER		        1
#define OSMIFARE_RATS_ORDER		        	3
#define OSMIFARE_HALT_ORDER		        	4
#define OSMIFARE_OPENRF_ORDER		        5
#define OSMIFARE_CLOSERF_ORDER		        6
#define OSMIFARE_CLASSIC_ORDER	        	7
#define OSMIFARE_DESFIRE_ORDER      		8
#define OSMIFARE_DICARD_ORDER	      		9
#define OSMIFARE_DIGITAL_ORDER	10
#define OSMIFARE_DESELECT_ORDER    11
#define OSMIFARE_REMOVE_ORDER		12

//order for select card
typedef struct {
    unsigned char 	ucInDataLen;
    unsigned char 	aucInData[OSMIFARE_CARDID_MAX_LEN];
}BASE_ORDER_TYPE;

//order for Classic card
#define OSMIFARE_CLASSIC_BLOCK_LEN		16

typedef struct {
    unsigned char	order_type;
    unsigned char	ucBlockIndex;
    unsigned char 	ucAuthType;			//��֤ģʽ:0 ��׼	1 �Ϻ�
    unsigned char 	aucInData[OSMIFARE_CLASSIC_BLOCK_LEN];
}CLASSIC_ORDER_TYPE;

// order for desfire card
typedef struct {
    unsigned char   order_type;
    unsigned int 	uiInDataLen;
    unsigned char 	aucInData[512];
}DESFIRE_ORDER_TYPE;

//order for double interface
typedef struct {
    unsigned int 	uiInDataLen;
    unsigned char 	aucInData[512];
}DICARD_ORDER_TYPE;

// order to be transmitted to mifare reader
typedef union {
    CLASSIC_ORDER_TYPE		classic_order;	//order to m1 m4 card
    DESFIRE_ORDER_TYPE     	desfire_order;  //order to desfire card
    DICARD_ORDER_TYPE    	dicard_order;   //order to double interface card
    BASE_ORDER_TYPE		    base_order;
}MIFARE_ORDER_IN_TYPE;

// command to execute in the ICC or SIM reader
typedef struct {
    unsigned char order;                      // order number to execute
    MIFARE_ORDER_IN_TYPE *pt_order_in;  		// order to be transmitted
    unsigned char *ptout;                      // output buffer
}MIFIRE_ORDER;

// execution report
typedef struct {
    unsigned char   drv_status;
    unsigned char   card_status;
    unsigned short  Len;
}MIFARE_ANSWER;


// execute a command of the mifare driver
MIFARE_ANSWER * Os__MIFARE_command( MIFIRE_ORDER *pOrder );

//DRV_OUT *Os__MIFARE_Request(unsigned char ucCardType,unsigned char ucReqMode);

unsigned char Os__MIFARE_Request(unsigned char ucCardType,unsigned char ucReqMode);

int  OSMIFARE_Open(void);
void OSMIFARE_Close(void);
MIFARE_ANSWER * OSMIFARE_command( MIFIRE_ORDER * pOrder );

//DRVOUT *OSMIFARE_Request( unsigned char ucCardType,unsigned char ucReqMode );
unsigned char OSMIFARE_Request( unsigned char ucCardType,unsigned char ucReqMode );

unsigned char OSMIFARE_Anti( unsigned char ucAntiEn,unsigned char *pucCardId,
                             unsigned char *pucCardIdLen,unsigned char *pucCardClass );
unsigned char OSMIFARE_Select( unsigned char *pucCardID,unsigned char ucCardIDLen,unsigned char *pucCardClass );
unsigned char OSMIFARE_Auth( unsigned char ucAuthType,unsigned char ucSectorIndex,unsigned char ucKeyType,
                             unsigned char *pucKey );
unsigned char OSMIFARE_GetAts( unsigned char *pucAts,unsigned char *pucAtsLen );
unsigned char OSMIFARE_SendRevData( unsigned char ucCheckMode,unsigned char *pucSendData,unsigned int uiSendLen,
                                    unsigned char *pucRevData,unsigned char*pucRevLen,unsigned int uiTimeDelay );
unsigned char OSMIFARE_SendData( unsigned char ucCheckMode,unsigned char *pucSendData,unsigned int uiSendLen );
unsigned char OSMIFARE_SendRevFrame( unsigned char *pucSendFrame,unsigned int uiSendLen,
                                     unsigned char *pucRevFrame,unsigned int *puiRevLen,unsigned int uiTimeDelay );
unsigned char OSMIFARE_Halt(void);
unsigned char OSMIFARE_AntiProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_SelectProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_RatsProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_HaltProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_CloseRfProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_DeSelect();
unsigned char OSMIFARE_DeSelectProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_DigitalProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
unsigned char OSMIFARE_Remove();
unsigned char OSMIFARE_PowerRF(unsigned char ucOnOf);
unsigned char OSMIFARE_RemoveProc( MIFARE_ANSWER *pAnswer,MIFIRE_ORDER * pOrder );
MIFARE_ANSWER * OSMIFARE_command( MIFIRE_ORDER * pOrder );
void OSMIFARE_SetReport(unsigned char ucMifareStatus);
DRVOUT *OSMIFARE_Polling( void );
unsigned char OSMIFARE_Active( void );
unsigned char OSMIFARE_Ioctl(unsigned char ucType,unsigned char *pucData);
unsigned char OSMIFARE_TestWRBlock(char select);


#endif

