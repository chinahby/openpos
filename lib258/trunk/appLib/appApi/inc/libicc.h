/*
	Hardware Abstraction Layer
--------------------------------------------------------------------------
	FILE  halicc.h
--------------------------------------------------------------------------
    INTRODUCTION
    ============
    Created :		2003-02-22		Xiaoxi Jiang
    Last modified :	2003-02-22		Xiaoxi Jiang
    Module :
    Purpose :
        Header file.

    List of routines in file :

    File history :
*/

#ifndef __LIBICC_H__
#define __LIBICC_H__


#ifndef LIBICCDATA
#define LIBICCDATA extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define LIBICC_DEBUG




//#define true			1
#define flase			0

#define Test
#ifndef SUCCESS
#define SUCCESS							0x00
#endif
#define CARDCMD_READER00				0x00
#define CARDCMD_READER01				0x01
#define CARDCMD_READER02				0x02

#define CARDCMD_Invoice					0x01
#define CARDCMD_Return					0x02
#define CARDCMD_Invalid					0x03

#define KeyTypeErr						0x4F		// ��Կ���ʹ���

#define CARDRETURNERR					0xFF

#define LIBICCERR_SUCCESS               0x00
#define LIBICCERR_NOICC                 0x01
#define LIBICCERR_NOREADER              0x02
#define LIBICCERR_OVERRECVBUF           0x03
#define LIBICCERR_BADID                 0x04
#define LIBICCERR_POWERFAIL				0x05
#define LIBICCERR_CMDNOTIMPLEMENT       0x0B
#define LIBICCERR_BADCMDOUTPARAM        0x0C
#define LIBICCERR_WAITCAPDU				0x0F
#define LIBICCERR_BADTS					0x3C
#define LIBICCERR_ATRTOOLONG            0x12
#define LIBICCERR_CARDFAULT             0x15
#define LIBICCERR_BADCMD                0x16
#define LIBICCERR_BADCMDINPARAM         0x17
#define LIBICCERR_UNKNOWNPROTOCOL       0x18
#define LIBICCERR_PROTOCOLPARAM         0x19
#define LIBICCERR_APDUSENDTIMEOUT       0x1A
#define LIBICCERR_APDURECVPROCTIMEOUT   0x1B
#define LIBICCERR_APDURECVTIMEOUT1      0x1C
#define LIBICCERR_APDURECVTIMEOUT2      0x1D
#define LIBICCERR_APDUEXCHUNKNOWN       0x1E
#define LIBICCERR_UNKNOWNSTATUS         0x1F
#define LIBICCERR_NOTPOWERED            0x20
#define LIBICCERR_ATRPARITYERROR		0x21
#define LIBICCERR_APDUPARITYERROR		0x22
#define LIBICCERR_APDUTIMEOUT			0x23
#define LIBICCERR_BADTA1				0x24
#define LIBICCERR_BADTB1                0x25
#define LIBICCERR_BADTD1                0x26
#define LIBICCERR_BADTA2                0x27
#define LIBICCERR_BADTB2                0x28
#define LIBICCERR_BADTC2				0x3D
#define LIBICCERR_BADTD2                0x2A
#define LIBICCERR_BADTA3                0x2B
#define LIBICCERR_BADTB3				0x2C
#define LIBICCERR_BADTC3                0x2D
#define LIBICCERR_BADTCK                0x3E
#define LIBICCERR_BADPROCBYTE			0x41

#define LIBICCERR_EXECUTEOK             0x30
#define LIBICCERR_CARDREMOVE            0x31
#define LIBICCERR_VPPPROBLEM            0x32
#define LIBICCERR_VCCPROBLEM            0x33
#define LIBICCERR_ISASYNCCARD           0x35
#define LIBICCERR_ISSYNCCARD			0x44
#define LIBICCERR_CARDMUTE              0x3F
#define LIBICCERR_NEEDWARMRESET         0x39
#define LIBICCERR_NEEDCOLDRESET			0x3A
#define	LIBICCERR_NEEDSELECTCARD		0x3B
#define LIBICCERR_STANDARDERR			0x3C

#define LIBICCERR_BADSYNCCMD			0x40
#define LIBICCERR_SYNCEOC				0x41
#define LIBICCERR_SYNCSETDATA			0x42
#define LIBICCERR_SYNCMACRO				0x43

#define LIBICCERR_T1BADLEN				0x4F
#define LIBICCERR_T1BADEDC				0x50
#define LIBICCERR_T1BADPCB				0x51
#define LIBICCERR_T1BADRECVSEQ			0x52
#define LIBICCERR_T1UNKNOWNERR			0x53
#define LIBICCERR_T1BADNAD              0x54
#define LIBICCERR_T1SABORTREQUEST       0x55
#define LIBICCERR_T1BADIFRAMELEN        0x56
#define LIBICCERR_T1BADRFRAMELEN        0x57
#define LIBICCERR_T1BADSFRAMELEN        0x58
#define LIBICCERR_T1BADPROTOCOL         0x59
#define LIBICCERR_T1INVALIDSEQ          0x5A
#define LIBICCERR_T1RFRAMEERR           0x5B
#define LIBICCERR_T1SFRAMEERR           0x5C
#define LIBICCERR_T1ILINKERR            0x5D
#define LIBICCERR_T1RFRAMEBADEDC		0x5E
#define LIBICCERR_T1INVALIDBLOCK        0x5F

#define LIBICCERR_T1IFRAMERESEND        0xFA
#define LIBICCERR_APDUNONEXTCOMMAND     0xFB
#define LIBICCERR_ATRHASLASTCHAR		0xFC
#define LIBICCERR_ATRNOTFINISHED        0xFD
#define LIBICCERR_PROCESSEND            0xFE
#define LIBICCERR_PROCESSPENDING        0xFF

#define LIBICCERR_I2CNACK				0x70

#define ICCMETHOD_ASYNC					0x00
#define ICCMETHOD_SYNC					0x01

#define ICCSTANDARD_EMV                 0x00
#define ICCSTANDARD_ISO7816             0x01
#define ICCSTANDARD_MPEV5               0x02
#define ICCSTANDARD_MONDEX              0x03



#define ICCPROTOCOL_T1                  0x01
#define ICCPROTOCOL_T0                  0x00

#define ICCAPDU_CASE0                   0x00
#define ICCAPDU_CASE1                   0x01
#define ICCAPDU_CASE2                   0x02
#define ICCAPDU_CASE3                   0x03
#define ICCAPDU_CASE4                   0x04
#define ICCAPDU_CASE5                   0x05
#define ICCAPDU_CASE6                   0x06
#define ICCAPDU_CASE7                   0x07

#define ICCSTATUS_INSERT                0x01
#define ICCSTATUS_REMOVE                0x02
#define ICCSTATUS_POWERED               0x03
#define ICCSTATUS_ISSYNCCARD			0x04

#define ICCTYPE_CPUCARD					0x00
#define ICCTYPE_SLE4442CARD				0x01

#define ICCSESSION_IDLE                 0x00
#define ICCSESSION_COLDRESET            0x01
#define ICCSESSION_WARMRESET            0x02
#define ICCSESSION_DATAEXCH             0x03

#define ICCSESSIONSTEP_IDLE             0x00
#define ICCSESSIONSTEP_COLDRESET1       0x01
#define ICCSESSIONSTEP_COLDRESET2       0x02
#define ICCSESSIONSTEP_COLDRESET3       0x03

#define ICCSESSIONSTEP_DATAEXCHSEND1    0x04
#define ICCSESSIONSTEP_DATAEXCHRECV1    0x05
#define ICCSESSIONSTEP_DATAEXCHSEND2    0x06
#define ICCSESSIONSTEP_DATAEXCHRECV2    0x07
#define ICCSESSIONSTEP_DATAEXCHSEND3    0x08
#define ICCSESSIONSTEP_DATAEXCHRECV3    0x09
#define ICCSESSIONSTEP_DATAEXCHSEND4    0x0A
#define ICCSESSIONSTEP_DATAEXCHRECV4	0x0B

#define ICCCMD_SUCCESS					0x00
#define ICCCMD_BADPACKET				0x90
#define ICCCMD_NODATASEND				0x91

#define ICCCMD_MAXNB                    0x0C
#define ICCCMD_MAXBUF					256
#define LIBICC_MAXATRBUF                100
#define LIBICC_MAXCOMMBUF	            255	//zy 270

#define LIBICC_COLDRESET                0x00
#define LIBICC_WARMRESET                0x01

#define LIBICC_READERNB         		4

#define LIBICC_WWTOUT					0x00
#define LIBICC_BWTOUT					0x01
#define LIBICC_CWTOUT					0x02

#define	REV_TIME_OUT					100


    /******************************************************************************/
//˰�غ궨��
#define  SIMcardResetErr  	 0x01 	/*˰�ؿ��ϵ����	   01H ( WatchCore���� )*/
#define  CPUcardResetErr  	 0x02	/*�û����ϵ����	   02H*/
#define  SPVcardResetErr  	 0x03 	/*˰������ϵ���� 03H*/
#define  NoCPUcardErr     	 	 0x04	/*���û���	   04H*/
#define  NoSIMcardErr	   	 0x05	/*��˰�ؿ�	   05H*/
#define  NoSPVcardErr	   	 0x06 	/*��˰�����	   06H*/
#define  ReadSIMcardErr   	 0x07 	/*��˰�ؿ�����	   07H*/
#define  ReadCPUcardErr   	 0x08 	/*���û������� 	   08H*/
#define  ReadSPVcardErr   	 0x09 	/*��˰���������   09H*/
#define  WriteSIMcardErr  	 0x0a 	/*д˰�ؿ�����	   0AH*/
#define  WriteCPUcardErr  	 0x0b	/*д�û������� 	   0BH*/
#define  WriteSPVcardErr     	 0x0c	/*д˰���������   0CH*/
#define  InvalidSIMcardErr 	 0x0d	/*�Ƿ�˰�ؿ�	   0DH*/
#define  InvalidCPUcardErr	 0x0e	/*�Ƿ��û���	   0EH*/
#define  InvalidSPVcardErr	 0x0f	/*�Ƿ�˰�����	   0FH*/
#define  UnTaxInitStatus		 0x10	/*δ��ʼ��״̬	   10H*/
#define  TaxDateLimitOver	 0x11	/*�ѹ���ֹ��Ʊ����   11H*/
#define  TaxValLimitOver     	 0x12	/*�ѳ������ŷ�Ʊ��Ʊ����޶�  12H*/
#define  TaxTotalLimitOver	 0x13	/*�ѳ�����Ʊ�ۼƽ���޶�	   13H*/
#define  TaxReturnValLimitOver 0x14	/*�ѳ�����Ʊ�ۼƽ���޶�	   14H*/
#define  RegistID_NoMatch	 0x15	/*˰�ؿ���ע�����˰������ע��Ų�һ��  15H*/
#define  CPUcardMACchkErr	 0x16	/*�û���У��MAC����  16H*/
#define  SIMcardMACchkErr	 0x17	/*˰�ؿ�У��MAC����  17H*/
#define  PINchkErr		        0x18	/*���PINʧЧ	   18H*/
#define  InvalidTaxTypeErr	 0x19	/*�Ƿ�˰��˰Ŀ	   19H*/
#define  RptDateCurDate	     	 0x1a	/*�걨��ֹ���ڴ��ڵ�ǰ����	 1AH (�걨ʱ�ж�)*/
#define  RptDateLastRptDateErr 0x1b	/*�걨��ֹ����С���ϴ��걨����  1BH (��˰�ز�����ж�)*/
#define  CPUcardLocked		 0x1c	/*�û�������	   1CH*/

#define  TaxCtrlCardLocked      	0x1d 	/*˰�ؿ�����*/
#define  TaxDeviceInited         		 0x1e 	/*˰�����ѳ�ʼ�������ٴγ�ʼ��*/
#define  CheckSumByteErr       	 0x1f	 /*У��λ����ȷ*/
#define  CmdTypeErr             		 0x20 	/*�����������*/
#define  FrameHeadFormatErr 	 0x21	 /*��ͷ����*/
#define  FrameLengthErr         		  0x22 	/*�����ȴ���*/
#define  InvoicePaperEmpty    	  0x23 	/*��Ʊ������,��ַ���Ʊ*/
#define  InvoiceMemoryErr      	  0x24 	/*��Ʊ�洢���쳣*/
#define  FiscalMemoryErr        		  0x25	 /*˰�ش洢���쳣*/
#define  DataOverflow           		0x26 	/*ʱ�䡢���ڡ��������*/
#define  TaxCtrlCardRegisted   	 0x27 	/*˰�ؿ���ע��*/
#define  CardSpaceFull          		0x28 	/*����������������*/
#define  NoInvoicePaperCanBeUse 	0x29 /*û���ѷַ���δʹ�õķ�Ʊ��*/
#define  NoInvoiceRollCanBeUse  		0x2a /*�û������޷�Ʊ�ַ�*/
#define  InvalidUserNo          			0x2b /*�Ƿ��ֻ����*/
#define  InvoiceNo_DataNotFind  		0x2c /*�˷�Ʊ�����������*/
#define  InvoiceNo_UseToReturn  		0x2d /*�˷�Ʊ������Ʊ����*/
#define  InvoiceNo_UseToBlankOut 	0x2e /*�˷�Ʊ������Ʊ����*/
#define  BLCurrDateLiSysDate	 	0x2f /*ϵͳ����С��˰�ص�ǰ����*/
#define  BLCollectMust			0x30 /*����ܷ�Ʊ��*/
#define  CustPinError				 0x31 /*�����������*/
#define  CustDeclare				 0x32 /*�������걨*/
#define  CustReload			 	0x33 /*�������걨*/
#define  NoStatisticsData			0x34 /*��ͳ������*/
#define  CanNotDeclare			0x36
#define  TaxNameNoChange		0x37

#define  FileNotFound				0x40 /*δ�ҵ��ļ�*/
#define  VerifyFail				0x41 /*��֤ʧ��*/
#define  KeyLockedErr			0x42 /*��Կ����*/
#define  UseConditionErr			0x43 /*ʹ��״̬������*/
#define  FileTypeErr				0x44 /*�ļ����ʹ���*/
#define  SafeConditionErr			0x45 /*��ȫ״̬������*/
#define  MessageDataErr			0x46 /*��ȫ�������ݴ�*/
#define  DataParamErr			0x47 /*���ݲ�����*/
#define  FunctionNotSupported	0x48 /*��֧�ִ˹���*/
#define  FileSpaceInsufficient		0x49 /*�ļ��洢�ռ䲻��*/
#define  KeyNotFound				0x4A /*��Կδ�ҵ�*/
#define  CardNoDataErr			0x4B /*���������ݷ���*/
#define  MacErr					0x4C /*MAC��*/
#define  ApplicationLocked		0x4D /*Ӧ�ñ���������*/
#define  KeyVersionNotSupported	0x4E /*��Կ�汾��֧��*/
#define  KeyTypeErr				0x4F /*��Կ���ʹ���*/

#define  USER_CANCEL			0x50
#define  WriteRecordEnd			0x51	/* ��Ʊ��¼�ļ�����*/
#define  TaxInitStatus			 0x80	/*�ѳ�ʼ��״̬	   80H*/
#define  NoInvoiceRoll          		0x81   /*�û������޷�Ʊ�ɷַ�*/
#define  IssInvoiceValLimitOver	 0x82   /*����*/
//#define  CmdParaErr				 0x83   /*������˵:���������*/
#define  CommPortOpenErr      	  0x84   /*�򿪴���ʧ��*/
#define  Eeprom_RW_err        		  0x85   /*Eeprom��дʧ��*/
#define  FlashRom_RW_err      		  0x86   /*Flashҳдʧ��*/
#define  TaxCtrlCardResetErr    	0x87   /*˰�ؿ���λ����*/
#define  TaxCtrlCardFileReadErr1 	0x88   /*��˰�ؿ�������Ϣ�ļ�����*/
#define  TaxCtrlCardFileReadErr2 	0x89  /*��˰�ؿ���ع��������ļ�����*/
#define  TaxCtrlCardFileReadErr3	 0x8A  /*��˰�ؿ���Ʊ�洢�ļ�����*/
#define  UserCardResetErr 	 	0x8B   /*�û�����λ����*/
#define  UserCardFileReadErr1	 0x8C   /*���û���������Ϣ�ļ�����*/
#define  UserCardFileReadErr2   	0x8D   /*���û���˰��˰Ŀ�����ļ�����*/
#define  WorkStatusShakeErr    	 0x8E   /*����״̬����ʧ��*/
#define  DiagStatusSHakeErr     	0x8F   /*����״̬����ʧ��*/
#define  TaxReportFinished     		 0x90   /*�Ѿ�����˰*/
#define  ReadRecFileErr			 0x91   /*�� ������¼ ʧ��*/
#define  TaxReportNotEnd       	 	0x92   /*��δ��˰*/
#define  DateParaErr		 		0x93   /*˰�س�ʼ��ʱ���ڲ�����*/
#define  CheckDataDoNotNeed     	0x94   /*�޺˲�����*/
#define  TaxCommunicationErr    	0x95   /*˰����ͨѶ����*/
#define  NowInvoiceNotEndErr       0x96   /*��ǰ��Ʊ��δ�������*/
#define  SellParameterErr       		0x97   /*���۲�������*/
#define  SevenYearNoteDateErr   	0x98   /*Flash��5���¼���ڴ���*/
#define  InvoiceTypeParameterErr   	0x99   /*'��Ʊ����'��������*/
#define  DeclareParamErr 		0x9a  /*�걨���ʹ���*/
#define  InvROllNotEmpty        		0x9b   /*��Ʊ��δ����,�������ɷ�Ʊ�������*/
#define  ChkCardTreateErr_0     	0x9c   /*�˲����Ϊ ��������,�������ɺ˲��¼*/
#define  CannotDistriboteInvoice 	0x9d  /*˰�ؿ��ڷ�Ʊ�洢�ļ� ��Ʊδ����*/
#define  RecordNotFound          	0x9e  /*δ�ҵ���¼*/
#define  ReportDateParaErr       	0x9f  /*��˰�걨ʱ���ڴ�*/
#define  ChkNeedContinure        	0xa0  /*�˲� ��Ҫ �ٲ忨*/
#define  ChkTypeErr		        	0xa1  /*�˲�����*/
#define  InvNoErr        			0xa2  /*�ַ���Ʊ�Ŵ���*/
#define  CheckDataErr        	0xa3  /*����˲����ڴ���*/
#define  COM_TRANS_ERR        	0xa4  /*��������ͨѶ����1*/
#define  COM_TRANS_ERR1        	0xa5  /*��������ͨѶ����1*/
#define  COM_TRANS_ERR2       	0xa6  /*��������ͨѶ����2*/
#define  COM_TRANS_ERR3        	0xa7  /*��������ͨѶ����3*/
#define  COM_TRANS_ERR4        	0xa8  /*��������ͨѶ����4*/
#define  COM_TRANS_ERR5       	0xa9  /*��������ͨѶ����5*/
#define  TCKERR			       	0xaa  /*��������ͨѶ����5*/
#define  TIMEOUTERR			       	0xAB  /*��������ͨѶ����5*/
#define  TSERR			       	0xAC  /*��������ͨѶ����5*/
#define  TC2ERR			       	0xAD  /*��������ͨѶ����5*/
#define  UNKNOWERR			    0xAE  /*δ֪����*/
#define  IC_ID_NoMatch	 	0xAF	/*�û���ע�����˰�ػ�ע��Ų���*/
#define  OneInvAmtLimitOver	 0xb0   /*���ŷ�Ʊ����*/
#define  TotInvAmtLimitOver	 0xb1   /*�ۼƿ�Ʊ����*/
#define  TotRefAmtLimitOver	 0xb2   /*�ۼ���Ʊ����*/
#define  TimeLimitOver	 0xb3   /*�����ѵ�ʹ������*/
#define  ModifyWareList		0xb4	/* �޸���Ʒ���۱� */

    /*--------------------------------------------------------------------------------------------------*/

    typedef struct
    {
        unsigned char aucBuf[256];
    }UARTSENDBUF;

    typedef struct
    {
        unsigned char (*pfnProc)(void);
    }ICCCMD;

#if 0 //0714
    typedef struct
    {
        volatile unsigned char ucStatus;
        volatile unsigned char ucSession;
        volatile unsigned char ucSessionStep;
        volatile unsigned char ucLastError;
        volatile unsigned char ucPowerType;
        volatile unsigned char ucResetType;
        volatile unsigned char ucProtocol;
        unsigned char ucFi;
        unsigned char ucDi;
        unsigned char ucRate;
        unsigned char ucGTR;
        unsigned long ulWWT;
        unsigned short uiCWT;
        unsigned char ucWTX;
        unsigned long ulBWT;
        unsigned char ucT1IFSReqFlag;
        unsigned char ucT1IFSC;
        unsigned char ucT1SendSeq;
        unsigned char ucT1RecvSeq;
        unsigned char ucT1EDCMode;
        unsigned char ucCardType;				// ��������
    }ICCINFO;
#else
    typedef struct
    {
        volatile unsigned char ucStatus;
        volatile unsigned char ucSession;
        volatile unsigned char ucSessionStep;
        volatile unsigned char ucLastError;
        volatile unsigned char ucStandard;
        volatile unsigned char ucPowerType;
        volatile unsigned char ucResetType;
        volatile unsigned char ucAutoPower;
        volatile unsigned char ucATRLen;
        unsigned char aucATR[LIBICC_MAXATRBUF];
        volatile unsigned char ucProtocol;
        volatile unsigned char ucAPDUCase;
        volatile unsigned short uiRecvLen;
        volatile unsigned short uiRecvedLen;
        unsigned char aucRecvBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned char ucAPDUOneByteFlag;
        volatile unsigned short uiAPDUSendLen;
        unsigned char aucAPDUSendBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned short uiAPDUSentLen;
        volatile unsigned short uiAPDURecvLen;
        unsigned char aucAPDURecvBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned short uiAPDURecvedLen;
        volatile unsigned short uiT1FrameSendLen;
        unsigned char aucT1FrameSendBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned short uiT1FrameSentLen;
        volatile unsigned short uiT1FrameLastSendLen;
        unsigned char aucT1FrameLastSendBuf[LIBICC_MAXCOMMBUF];
        unsigned char aucSYNCSendBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned short uiSYNCSendLen;
        unsigned char ucSYNCSendByte;
        unsigned char ucSYNCSentBit;
        unsigned char aucSYNCRecvBuf[LIBICC_MAXCOMMBUF];
        volatile unsigned short uiSYNCRecvedLen;
        unsigned char ucSYNCRecvedByte;
        unsigned char ucSYNCRecvedBit;
        unsigned char ucFi;
        unsigned char ucDi;
        unsigned char ucGTR;
        unsigned long ulWWT;
        unsigned short uiCWT;
        unsigned char ucWTX;
        unsigned long ulBWT;
        unsigned char ucT1Status;
        unsigned char ucT1ErrorCount;
        unsigned char ucT1IFSReqFlag;
        unsigned char ucT1IFSC;
        unsigned char ucT1NAD;
        unsigned char ucT1SendSeq;
        unsigned char ucT1RecvSeq;
        unsigned char ucT1EDCMode;
    }ICCINFO;
#endif
    /*
    EN:
    ZH_CN:
        ���ܿ��ӿ����õ��ı���   */
    LIBICCDATA ICCINFO ICCInfo[LIBICC_READERNB];


    LIBICCDATA char bAbnoFiscalFlag;			// �����쳣��־
    LIBICCDATA char bAbnoUserFlag;
    LIBICCDATA char bAbnoControlFlag;

    LIBICCDATA char bRemoveCardFlag;			// ���γ���־
    LIBICCDATA char bRemoveFiscalFlag;

    LIBICCDATA char bTO3;
    LIBICCDATA char bTO2;
    LIBICCDATA char bTO1;
    LIBICCDATA char bFinished;
    LIBICCDATA char bSentByte;
    LIBICCDATA char bRecvByte;

    unsigned char ucCardReader;
    unsigned char ucSW1;
    unsigned char ucSW2;
    unsigned char ucCardRecvLen;
    unsigned char ucCardRecvStatus;
    unsigned char aucCardRecvBuf[256];

    unsigned char ucCardSendLen;
    unsigned char aucCardSendBuf[256];

    unsigned char aucATR[LIBICC_MAXATRBUF];

    unsigned char uiRecvLen;
    unsigned char uiRecvedLen;
    unsigned char ucAPDUCase;
    unsigned char ucAPDUOneByteFlag;
    unsigned char uiAPDUSendLen;
    unsigned char uiAPDUSentLen;
    unsigned char uiAPDURecvLen;
    unsigned char uiAPDURecvedLen;
    unsigned char aucAPDURecvBuf[LIBICC_MAXCOMMBUF];
    LIBICCDATA volatile unsigned char ucATRLen;

    LIBICCDATA volatile unsigned short uiT1FrameLastSendLen;
    LIBICCDATA volatile unsigned short uiT1FrameSendLen;
    LIBICCDATA volatile unsigned short uiT1FrameSentLen;
    unsigned char aucT1FrameSendBuf[LIBICC_MAXCOMMBUF];

    /* timer.c */
    unsigned char aucCtrlTimer[7];

    /* ��Ʊ������Ϣ */
    LIBICCDATA unsigned short usRefInvSectNo;			/* �˻�ʱԭ���ķ�Ʊ��� */
    unsigned char ucRefInvSectNo;			/* �˻�ʱԭ���ķ�Ʊ��� */

    unsigned char ucReaderNO;


    /* pboc.c add by zy */
    void PBOC_SetIso7816Out(void);
    unsigned char PBOC_SelectFile(unsigned char ucFlag,unsigned char ucReader,
                                  unsigned char *pucData,unsigned char ucDataLen);
    unsigned char PBOC_ISOGetResponse(unsigned char ucReader,
                                      unsigned char ucLen);
    unsigned char PBOC_ISOReadRecordbySFI(unsigned char ucReader,
                                          unsigned char ucSFI,unsigned char ucRecordNumber);
    unsigned char PBOC_ReadRecord(unsigned char ucReader,unsigned char ucRecordNumber);
    unsigned char PBOC_ISOReadBinarybySFI(unsigned char ucReader,
                                          unsigned char ucSFI,unsigned char ucOffset,unsigned char ucInLen);
    unsigned PBOC_Get_RegisterNb(unsigned char ucReader);
    unsigned PBOC_Get_RegisterSign(unsigned char ucReader,
                                   unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_Terminal_Register(unsigned char ucReader,
                                    unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_Pin_UNBlock(unsigned char ucReader, unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_VerifyFiscal_Pin(unsigned char ucReader, unsigned char *pucData,unsigned char ucLen);
    unsigned char PBOC_ISOCheckReturn(void);//zy 0716
    unsigned char PBOC_DitributeInvoice(unsigned char ucReader);
    unsigned char PBOC_InputInvoiceNB(unsigned char ucReader,
                                      unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_VerifyUser_Pin(unsigned char ucReader,
                                 unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen);
    unsigned char PBOC_IssueInvoice(unsigned char ucReader,
                                    unsigned char *pucData,unsigned char ucLen);
    unsigned char PBOC_DailyCollectSign(unsigned char ucReader,
                                        unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_UpdateRecord(unsigned char ucReader,
                               unsigned char ucSFI,unsigned char ucRecordNumber,
                               unsigned char *pucData,unsigned char ucLen,unsigned char ucFlag);
    unsigned char PBOC_UpdateBinary(unsigned char ucReader,unsigned char ucSFI,
                                    unsigned short usOffset,unsigned char *pucData,unsigned char ucLen,unsigned char  ucFlag);
    unsigned PBOC_DeclareDuty(unsigned char ucReader, unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_DataCollect(unsigned char ucReader, unsigned char *pucData,unsigned char ucLen);
    unsigned PBOC_UpdateControls(unsigned char ucReader, unsigned char *pucData,unsigned char ucLen);
    unsigned char PBOC_GetChallenge(unsigned char ucReader, unsigned char ucLen);
    unsigned PBOC_InternalAuthentication(unsigned char ucReader,
                                         unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen);
    unsigned char PBOC_ExternalAuthentication(unsigned char ucReader,
            unsigned char *pucData, unsigned char ucPinIndex, unsigned char ucLen);
    unsigned char PBOC_ISOCheckReturn_test(unsigned char ucReader);

    /* delay.a51 */

    /* i2c.a51 */
    unsigned char I2C_ReadNByte(unsigned char ucWordByte,unsigned char ucLen);
    unsigned char I2C_WriteNByte(unsigned char ucWordByte,unsigned char ucLen);

    /* cardcmd.c */
    unsigned char CARDCMD_DetectCard(unsigned char ucReader);
    unsigned char CARDCMD_GetResponse(unsigned char ucReader,unsigned char ucDataLen);
    unsigned char CARDCMD_VerifyUserPin(unsigned char ucReader,unsigned char *pucData,unsigned char ucPinIndex, unsigned char ucLen);
    unsigned char CARDCMD_Reset(unsigned char ucReader);
    unsigned char CARDCMD_SelectFile(unsigned char ucReader,unsigned char ucFlag,
                                     unsigned char *pucData,unsigned char ucDataLen);
    unsigned char CARDCMD_ReadRecord(unsigned char ucReader,
                                     unsigned char ucSFI,unsigned char ucRecordNumber);
    unsigned char CARDCMD_ReadBinary(unsigned char ucReader,
                                     unsigned char ucSFI,unsigned char ucOffset,unsigned char ucInLen);
    unsigned char CARDCMD_ExternalAuthentication(unsigned char ucReader,
            unsigned char *pucData, unsigned char ucPinIndex);
    unsigned char CARDCMD_GetRegisterNb(void);
    unsigned char CARDCMD_GetChallenge(unsigned char ucReader,unsigned char ucLen);
    unsigned char CARDCMD_TerminalRegister(unsigned char *pucMAC2);
    unsigned char CARDCMD_IssueInvoice(unsigned char *pucData);
    unsigned char CARDCMD_DeclareDuty(unsigned char *pucData);
    unsigned char CARDCMD_UpdateControls(unsigned char *pucData);
    unsigned char CARDCMD_InputInvoiceNb(unsigned char *pucData);
    unsigned char CARDCMD_VerifyFiscalPin(unsigned char *pucData);
    unsigned char CARDCMD_DailyCollectSign(unsigned char *pucData,unsigned char ucLen);
    unsigned char CARDCMD_RegisterSign(unsigned char *pucData);
    unsigned char CARDCMD_DataCollect(unsigned char *pucData);
//unsigned char CARDCMD_EchoplexControls(unsigned char *pucData);
    unsigned char CARDCMD_DistributeInvoiceNb(void);
    unsigned char CARDCMD_InternalAuthentication(unsigned char ucReader,
            unsigned char *pucData, unsigned char ucPinIndex);
    unsigned char CARDCMD_UpdateRecord(unsigned char ucReader,unsigned char ucSFI,
                                       unsigned char ucRecordNumber,unsigned char *pucData, unsigned char ucLen);
    unsigned char CARDCMD_CheckReturn(unsigned char ucReader);
    unsigned char CARDCMD_CheckCardErr(unsigned short usSW);


    /* libicc.c */
    void LIBICC_CallbackCardRemove(unsigned char ucReader);
//void LIBICC_CallbackTimeout(unsigned char ucReader,unsigned char ucTimeoutType);

    /* icccmd.c */
    unsigned char ICCCMD_Detect(void);
    unsigned char ICCCMD_Select(void);
    unsigned char ICCCMD_Insert(unsigned char ucReader);
    unsigned char ICCCMD_Remove(void);
    unsigned char ICCCMD_PowerOff(void);
    unsigned char ICCCMD_WarmReset(void);
    unsigned char ICCCMD_ReadTimer(void);
    unsigned char ICCCMD_WriteTimer(void);
    unsigned char ICCCMD_ChangeProtocol(void);

    /* iso7816.c */
    unsigned char ISO7816_WarmReset(unsigned char ucReader,unsigned char ucRate);
    unsigned char ISO7816_AsyncReset(unsigned char ucReader,unsigned char ucRate);
    unsigned char ISO7816_SetAPDUNextCommand(unsigned char ucReader);
    void ISO7816_ResetRecvCharProc(unsigned char ucReader);
    void ISO7816_TimeoutProc(unsigned char ucReader,unsigned char ucTimeoutType);
    void ISO7816_ResetTimeoutProc(unsigned char ucReader,unsigned char ucTimeoutType);

    /* atr.c */
    unsigned char ATR_ColdReset(unsigned char ucReader,unsigned char ucRate);
    unsigned char ATR_WarmReset(unsigned char ucReader,unsigned char ucRate);
    unsigned char ATR_Process(unsigned char ucReader);
    unsigned char ATR_FinishedProc(unsigned char ucReader);
    unsigned char MISC_GetCharBitNB(unsigned char ucValue,char ucBit);
    unsigned char ATR_CheckFinished(unsigned char *pucATR,unsigned short uiATRLen);
    void ATR_CallbackCardRemove(unsigned char ucReader);

    /* protocol.c */
    /* protocol.c */
#if 0
    unsigned char PROT_AsyncCommand(unsigned char ucReader,
                                    unsigned char *pucInData,
                                    unsigned short uiInLen);
#endif
    unsigned char PROT_CallbackT0SendChar(unsigned char ucReader);
    unsigned char PROT_AsyncCommand(unsigned char ucReader);
    unsigned char PROT_T0Command(unsigned char ucReader);
    unsigned char PROT_T0SendReceive(unsigned char ucReader);
    unsigned char PROT_T0RecvChar(unsigned char ucReader,unsigned char *pucCh);
    unsigned char PROT_T1Command(unsigned char ucReader);
    unsigned char PROT_T1SendReceiveFrame(unsigned char ucReader,unsigned char ucFrameType,unsigned char ucLastFrameType);
    unsigned char PROT_T1ReceiveFrame(unsigned char ucReader);
    unsigned char PROT_T0FinishedProc(unsigned char ucReader);
    unsigned char PROT_T1FinishedProc(unsigned char ucReader);
    unsigned char PROT_T1ComputeEDC(unsigned char ucMode,unsigned char *pucInData,unsigned short uiInLen);
    unsigned char PROT_T1PackIFrame(unsigned char ucReader);
    unsigned char PROT_T1PackRFrame(unsigned char ucReader,unsigned char ucSEQ,unsigned char ucError);
    unsigned char PROT_T1PackSFrameIFSReq(unsigned char ucReader);
    unsigned char PROT_T1PackSFrameSyncReply(unsigned char ucReader);
    unsigned char PROT_T1PackSFrameIFSReply(unsigned char ucReader);
    unsigned char PROT_T1PackSFrameBWTReply(unsigned char ucReader);
    unsigned char PROT_T1PackFrame(unsigned char ucReader,unsigned char ucPCB,
                                   unsigned char *pucInData,
                                   unsigned char ucInLen);
    void PROT_CallbackSendChar(unsigned char ucReader);
    void PROT_CallbackT1SendChar(unsigned char ucReader);
    void PROT_CallbackRecvChar(unsigned char ucReader,unsigned char ucCh);
    void PROT_CallbackT0RecvChar(unsigned char ucReader,unsigned char ucCh);
    void PROT_CallbackT1RecvChar(unsigned char ucReader,unsigned char ucCh);
    void PROT_CallbackTimeout(unsigned char ucReader,unsigned char ucTimeoutType);
    void PROT_CallbackT0Timeout(unsigned char ucReader,unsigned char ucTimeoutType);
    void PROT_CallbackT1Timeout(unsigned char ucReader,unsigned char ucTimeoutType);
    void PROT_CallbackCardRemove(unsigned char ucReader);
    void PROT_CallbackT0CardRemove(unsigned char ucReader);
    void PROT_CallbackT1CardRemove(unsigned char ucReader);
    void PROT_CallbackFault(unsigned char ucReader);
    void PROT_CallbackParityError(unsigned char ucReader);
    void PROT_CallbackT0ParityError(unsigned char ucReader);
    void PROT_CallbackT1ParityError(unsigned char ucReader);
    /* timer.c */
    unsigned char TIMER_Read();
    unsigned char TIMER_Step(unsigned char *ptTime);
    unsigned char TIMER_StepTimer(unsigned char *pTime);



#ifdef __cplusplus

#endif

#endif
