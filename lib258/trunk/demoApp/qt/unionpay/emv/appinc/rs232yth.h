#ifndef _RS232YTH_H
#define _RS232YTH_H

//#include <oslib.h>
//#include <typelib.h>

//#define     IDENTIFER       "M1470323"//������
//#define     IDENTIFER       "M1570526"  // �й�ũ��
#define     IDENTIFER       "B0000526"  // �й�����


#define     RSLEN_LEN				2
#define     IDENTIFER_LEN    		8
#define		OBLIGATE_LEN 		 	5
#define 	VER_LEN					3
#define 	OPERTYPE_LEN			2
#define		TRANSTYPE_LEN			2
#define 	CARDTYPE_LEN			2
#define 	CASHREGNO_LEN			6
#define 	CASHIER_LEN				6
#define     SETTLEAMOUNT_LEN		12
#define 	CASHIERTRACE_LEN		6 
#define 	OLDTICKETNUMBER_LEN		6
#define 	RESERVERD_LEN 			48
#define 	RESPCODE_LEN 			2
#define 	RESPINFO_LEN 			40
#define 	MERCHID_LEN 			15
#define 	MERCHANTNAME_LEN 		40
#define 	TERMID_LEN 				8
#define 	CARDID_LEN  			19
#define 	VALIDDATE 				4
#define 	BANKTYPE_LEN 			6
#define 	TRANSDATE_LEN 			8
#define 	TRANSTIME_LEN 			6
#define 	AUTHCODE_LEN  			6
#define 	SYSREFNO_LEN  			12
#define 	TRACENO_LEN 			6
#define 	ORGTRACENO_LEN 			6
#define     BATCHNUM_LEN            6

#define     ERR_RESULT            0x01
#define     SUCCESS_RESULT        0



typedef struct
{
	 unsigned char    aucIdentifer[IDENTIFER_LEN];  //Ӧ�ñ�ʶ��
	 unsigned char    aucObligate[OBLIGATE_LEN];   //�����루����Ӧ��֮��Լ���룩
	 unsigned char    aucVer[VER_LEN];  //�汾��
}HEADDATA;//16  ��Ϣͷ


typedef struct
{
	unsigned char	aucOperType[OPERTYPE_LEN]; //��������
	unsigned char	aucTransType[TRANSTYPE_LEN];//���״���
	unsigned char aucCardType[CARDTYPE_LEN];//������
	unsigned char	aucCashRegNo[CASHREGNO_LEN];		/*���������*/
	unsigned char	aucCashier[CASHIER_LEN];					/*��Ա��(��ѯʱΪ��ˮ��)*/
	unsigned char	aucSettleAmount[SETTLEAMOUNT_LEN];				/*���׽��*/
	unsigned char aucCashierTrace[CASHIERTRACE_LEN];				/*��������ˮ��*/
	unsigned char	aucOldTicketNumber[OLDTICKETNUMBER_LEN];		/*������������ˮ��*/
	unsigned char	aucReserved[RESERVERD_LEN];				/*�����ֶ�*/
}RECEIVETRANS;//66


typedef struct
{
	unsigned char	aucOperType[OPERTYPE_LEN];
	unsigned char	aucTransType[TRANSTYPE_LEN];
	unsigned char aucCardType[CARDTYPE_LEN];
	unsigned char	aucRespCode[RESPCODE_LEN];				/*���׷�����*/
	unsigned char	aucRespInfo[RESPINFO_LEN];				/*���׷�����Ϣ*/
	unsigned char	aucCashRegNo[CASHREGNO_LEN];			/*���������*/
	unsigned char	aucCashier[CASHIER_LEN];				/*��Ա��*/
	unsigned char aucSettleAmount[SETTLEAMOUNT_LEN];
	unsigned char	aucBatchNum[BATCHNUM_LEN];				/*�������κ�*/
	unsigned char	aucMerchID[MERCHID_LEN];				/*�̻���*/
	unsigned char	aucMerchantName[MERCHANTNAME_LEN];		/*�̻�����*/
	unsigned char	aucTermID[TERMID_LEN];				/*�ն˺�*/
	unsigned char	aucCardId[CARDID_LEN];				/*����*/
	unsigned char	aucValidDate[VALIDDATE];			/*����Ч��*/	
	unsigned char	aucBankType[BANKTYPE_LEN];				/*�����б���*/
	unsigned char	aucTransDate[TRANSDATE_LEN];			/*��������*/
	unsigned char	aucTransTime[TRANSTIME_LEN];			/*����ʱ��*/
	unsigned char	aucAuthCode[AUTHCODE_LEN];				/*��Ȩ��*/
	unsigned char	aucSysRefNo[SYSREFNO_LEN];			/*ϵͳ���պ�*/
	unsigned char aucCashTraceNo[CASHIERTRACE_LEN];			/*��������ˮ��*/
	unsigned char	aucOrgCashTraceNo[OLDTICKETNUMBER_LEN];		/*ԭ��������ˮ��*/
	unsigned char	aucTraceNo[TRACENO_LEN];				/*ϵͳ��ˮ��*/
	unsigned char	aucOrgTraceNo[ORGTRACENO_LEN];			/*ԭϵͳ��ˮ��*/
	unsigned char aucReserved[RESERVERD_LEN];       //�����ֶ�
}SENDTRANS;//251




unsigned char Rs232yth_UnPackPortData(unsigned char* aucInData,RECEIVETRANS *ReceiveData,unsigned char *aucIdentifer);
unsigned char Rs232yth_PackPortData(SENDTRANS *SendTransData,unsigned char *aucOutData,unsigned char ucFlag);



#endif 