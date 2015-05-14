
#ifndef _WK_MIFARE_H
#define _WK_MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define WK_MIFARE_ERR

#define WK_MIFARE_CARDERR_BADPARA		0x01		//��������
#define WK_MIFARE_CARDERR_NOCARD		0x04		//�޿�
#define WK_MIFARE_CARDERR_DECFAIL		0x05		//���ѿۿ�ʧ��
#define WK_MIFARE_CARDERR_KEYI1			0x06		//���ɽ��ף�д��ϸʧ�ܣ��������Ѿ��ɹ����гɹ���
#define WK_MIFARE_CARDERR_BADBACKUPEP	0x07		//����Ǯ����
#define WK_MIFARE_CARDERR_INVALIDCARD	0x08		//��Ч���򻵿�
#define WK_MIFARE_CARDERR_DEPFAIL		0x09		//����ʧ��
#define WK_MIFARE_CARDERR_BADTACPARAM	0x0A		//TAC������
#define WK_MIFARE_CARDERR_UNACCEPT		0x0C		//�Ǳ�ϵͳ��
#define WK_MIFARE_CARDERR_KEYI2			0x0D		//���ɽ��ף�����ȡ)
#define WK_MIFARE_CARDERR_AUTHFAIL		0x11		//������֤ʧ��
#define WK_MIFARE_CARDERR_CARDLOCK		0x12		//������

/* Smart Card */
#define MIFARE_OK					0		/* Return Code Ok  0x00 */
#define MIFARE_DRIVER_ERROR			1       /* Driver Error    0x01 */
#define MIFARE_CARD_ERROR			2       /* Card Error      0x02 */
#define MIFARE_OWERFLOW				3       /* Owerflow Buffer 0x03 */
#define MIFARE_ABSENT				4		/* Smart Absent    0x04 */

/* Smart Async Order Type */
#define MIFARE_TYPE0				0		/* Order Type0 LgIn,BufIn                     */
#define MIFARE_TYPE1				1		/* Order Type1 CLA,INS,P1,P2                  */
#define MIFARE_TYPE2				2		/* Order Type2 CLA,INS,P1,P2,LgOut            */
#define MIFARE_TYPE3				3		/* Order Type3 CLA,INS,P1,P2,LgIn,BufIn       */
#define MIFARE_TYPE4				4		/* Order Type4 CLA,INS,P1,P2,LgIn,BufIn,LgOut */

typedef struct
{
	unsigned char ucCla;
	unsigned char ucIns;
	unsigned char ucP1;
	unsigned char ucP2;
	unsigned char ucP3;
	unsigned char aucData[128];
}MIFARE_SO7816IN;

typedef struct
{
	unsigned short	uiLen;
	unsigned char	ucStatus;
	unsigned char	ucSW1;
	unsigned char	ucSW2;
	unsigned char	aucData[255];
}MIFARE_SO7816OUT;
unsigned char MIFARE_CloseRF( void );

unsigned char MIFARE_CheckCard(void);
unsigned char MIFARE_Menu(void);
unsigned char MIFARE_Process(void);
unsigned char MIFARE_RemoveCard(void);
unsigned char MIFARE_ReadCardInfo(void);
unsigned char MIFARE_WaitCard(void);
unsigned char MIFARE_CheckCardValid(void);
unsigned char MIFARE_FindErrCode(unsigned char ucStatus);
unsigned char MIFARE_ISO( unsigned char* pucIn,unsigned char* pucOut, 
            unsigned short* puiLengthOut,unsigned char ucOrderType);

#ifdef __cplusplus
}
#endif

#endif