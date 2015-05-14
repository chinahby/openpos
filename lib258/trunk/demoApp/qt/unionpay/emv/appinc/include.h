
#ifndef		_INCLUDE_H
#define		_INCLUDE_H

#include	<oslib.h>
#include	<EMV41.h>
#include  <new_drv.h>

#include  <rs232yth.h>
#define SANDREADER 1
#define WKPINPAD 1
#if SANDREADER
#include    <QPBOC.h>
#endif


#define ERR_TRANS_NOK      0x0F
#define ERR_TRANS_OK       0xF0

#define GUI_TTS_DATA
#define GUI_PROJECT
#define GUI_DELAY_TIMEOUT	1500
#define GUI_GETCARD_TIMES   1000

/*2009-10-12 16:08*/
/********************************************ǰ̨������Ϣ��ʾ**********************/
//  1 Ϊ��ǰ���ͣ�ͬʱֻ����һ����Ϊ1��
#define PS100	    0
#define PS300	    0			// REMOVED
#define PS400	    1

#if PS100
#define UpData				"YYYYMMDD"				//�汾��������
#define CURRENT_PRJ		"FFFFFFFFFFFFFFF"				//��Ŀ��ţ���������Ŀ��
#define SOFTWARE_VER		"x.x.x"					//����汾��
#endif

#if PS300
#define CURRENT_PRJ		"FFFFFFFFFFFFFFF"		//��Ŀ���
#define SOFTWARE_VER		"x.x.x"			//����汾
#endif

#if PS400
#define AppSoftId				"\x00\x68"
#define CURRENT_PRJ		"B000-MEGP-MPT4-001"				//��Ŀ���
#define UpData				"20111013"						//�汾��������
#define SOFTWARE_VER		"5.0.9"							//����汾
#endif

#define HARDWARE_VER		"v2.72"				//Ӳ���汾
#define HARDWARE_NAME		"�й�����PBOC"		//��Ŀ����

#define USEINSERTCARD				//�ú�򿪼���ʹ�ò���IC������
//#define USEUPDATADATATIME	//ֱ����˰����Ҫ�򿪸ú��������ʱ�䣬˰�ز������������ʱʱ������˰��ֱ����رոú�
//#define EMVTEST						//ʹ�øú꼴ΪEMV����״̬

#ifndef _TEST
#define _TEST
#endif

 #define util_Printf  printf

#define READER_HONGBAO 1
#define READER_SAND    2


#define COM1		0x31
#define COM2		0x32

#ifndef TMS
//#define TMS
#endif

#define UCHAR   unsigned char
#define UINT    unsigned int
#define ULONG   unsigned long

#define uchar   unsigned char
#define uint    unsigned int
#define ulong   unsigned long					        //�ú�Ϊ�й�����EMV2.0�������Ƿ�֧��TMSģ��
#define ushort    unsigned short

//#define UNTOUCH					//�ú�Ϊ�Ƿ�֧���ֻ��ƶ�֧������ģ��

#define FILEWRITENUMBER	3
/*����CJ��������*/

#define TRANS_MAXSENDNB				500
//#define TRANS_SAVEMAXNB				500
#define TRANS_SENDDATAONLINE            0x90
#define TRANS_SENDDATAPORT            	0x91
/*End*/
#define UnTest							//�����ֻ��ƶ�֧��ģ�����
/********************************************ǰ̨������Ϣ��ʾ**********************/
#define SUCCESS								0x00
#define SUCCESS_TRACKDATA					0x00
#define ERR_END								0x01
#define ERR_CANCEL							0x02
#define ERR_DATASAVE							0x03
#define ERR_DRIVER							0x04
#define ERR_OSFUNC							0x05
#define ERR_NOTPROC							0x06
#define ERR_UNKNOWNTRANTYPE			       0x07
#define ERR_RESETDATASAVEDATA			       0x08
#define ERR_DIAL								0x09
#define ERR_HOSTCODE						0x0A
#define ERR_DATAINVALID						0x0B
#define ERR_INVALIDXDATAPAGE			0x0C
#define ERR_INVALIDTRANS					0x0D
#define ERR_TRANSFILEFULL					0x0E
#define ERR_NOPSAM							0x0F
#define ERR_NOTVALIDPSAM					0x10
#define ERR_NOOLDTRANS						0x11
#define ERR_PRINT							0x12
#define ERR_NOPIN							0x13
#define ERR_ALREADYVOID						0x14
#define ERR_NOTRANS							0x15
#define ERR_CommPortOpenErr				       0x16
#define ERR_TRANS_UNKNOWOPERTYPE			0x17
#define ERR_TRANS_UNKNOWTRANSTYPE			0x18
#define ERR_NOTPHONECARD                                0x19
#define ERR_VERIFY_PINONE						0x20
#define ERR_VERIFY_PINTOW						0x21
#define ERR_VERIFY_PINTHREE						0x22
#define ERR_NOFUNCT								0x30
#define ERR_NOZEOR									0x31
#define ERR_AMOUNTERR							0x41
#define ERR_WRITEMEMORY						0x42
#define ERR_CHECKSECRET							0x43
#define ERR_MODIFYSECRET						0x44
#define ERR_CARDLOCK								0x45
#define ERR_CARDCRC									0x46
#define ERR_TAGNOTEXIST							0x47
#define ERR_NOTSUPEAPP							0x48
#define ERR_INVALDDATE							0x49
#define ERR_PLSLOADPARAM						0x50
#define ERR_INTERNOTTIP							0x51
#define ERR_TRANSNOTSUP						0x52
#define ERR_INVAILDAMOUNT			0x53
#define ERR_TRANSADJUSTED				0x54
#define ERR_USEICCARDFIRST				0x70
#define ERR_EMV_MAGTRANS				0x71
#define ERR_EMV_ICTRANS					0x72
#define ERR_NOTRETURN                                0x7E
#define ERR_TRANSTIEMOUT				0x55
#define ERR_TRANSNotADJUST				0x56
#define ERR_PROUNTOUCH					0x85
#define ERR_OLDNOTVOID					0xFB
/********ERROR RETURN VALUE*********/

#define EMV_ERRDATALEN	        0x61
#define	EMV_ERRTAG					0x62
#define	EMV_ERRLENGTH				0x63
#define	EMV_ERRAPPNUM		    0x64
#define	EMV_ERRDATA					0x65
#define EMV_SETTLEFIRST			0x66
#define EMV_NOSUCHTRACE      0x67
#define EMV_DATAOVERFLOW    0x68
#define EMV_TRANSFAIL				0x69
#define ERR_APP_TIMEOUT			0xFF

#define ERR_ISO83										0x80
#define ERR_ISO8583_INVALIDVALUE			0x81
#define ERR_ISO8583_INVALIDBIT				0x82
#define ERR_ISO8583_INVALIDPACKLEN		0x83
#define ERR_ISO8583_OVERBITBUFLIMIT		0x84
#define ERR_ISO8583_BITNOTEXIST				0x85
#define ERR_ISO8583_INVALIDPARAM		0x86
#define ERR_ISO8583_NODATAPACK			0x87
#define ERR_ISO8583_OVERBUFLIMIT			0x88
#define ERR_ISO8583_INVALIDBITATTR		0x89
#define ERR_ISO8583_UNPACKDATA			0x8A
#define ERR_ISO8583_SETBITLEN					0x8B
#define ERR_ISO8583_COMPARE					0x8C
#define ERR_ISO8583_MACERROR				0x8D
#define ERR_ISO8583_INVALIDLEN				0x8E

#define ERR_UTIL_OVERBUFLIMIT				0xA0

#define ERR_MSG_INVALIDINDEX				0xB0
#define ERR_MSG_INVALIDVALUE				0xB1

#define ERR_MAG_TRACKDATA					0xC0

#define ERR_TRANS_UNKNOWNACCTYPE	0xC8
#define ERR_TRANS_SETTLE							0xC9
#define ERR_TRANS_SAVEINDEX					0xCA
#define ERR_TRANS_CANNOTADJUST			0xCB
#define ERR_COMMS_SENDCHAR				0xD0
#define ERR_COMMS_RECVCHAR				0xD1
#define ERR_COMMS_PROTOCOL				0xD2
#define ERR_COMMS_RECVBUFFOVERLIMIT	0xD3
#define ERR_COMMS_RECVTIMEOUT				0xD4
#define ERR_COMMS_LRC								0xD5
#define ERR_COMMS_ERRTPDU						0xD6
#define ERR_COMMS_NOMAC						0xD7
#define ERR_SANDMAGCARD                      		0xD8

#define ERR_CASH_EXIST				0xE0
#define ERR_CASH_NOTEXIST		0xE1
#define ERR_CASH_PASS				0xE2
#define ERR_CASH_FILEFULL		0xE3
#define ERR_CASH_NOTLOGON				0xE4
#define ERR_CASH_ALREADYLOGON		0xE5
#define ERR_ADMIN_CMD						0xE8
#define ERR_OFFLINE								0xE9
#define ERR_SYS_CASHIERNO					0xEA
#define ERR_READANDWRITEICCARD 		0xEB
#define ERR_EXPIREDATECARD					0xEC
#define ERR_ICCARD								0xED
#define ERR_CHECKAOUNMT					0xEE
#define ERR_CAPKCHECK							0xEF
#define ERR_TIMEOUTOFF						0xF0
#define ERR_SUPERKEY								0xE2

//----------------------------------------
#define ERR_RECVMAXLEN 					0xF1
#define ERR_TAGLEN 							0xF2
#define ERR_TAGVAL 							0xF3
#define ERR_COMMANDBYTE				0xF4
#define ERR_COMMINIT						0xF5
#define ERR_TAGDATA							0xF6
#define ERR_NOLASTSETTLE     			0xF7
#define ERR_CARDNO							0xF8
#define ERR_BALANCE							0xF9
#define ERR_MULTICARD						0xFA
#define ERR_TRANSEMPTY					0xe6		/*������ˮΪ��*/
#define ERR_SETTLE_FIRST					0xe7
#define ERR_FALLBACK              			0xe8
#define ERR_FORMAT							0x60
#define ERR_NOOLDCARD					0x8F
#define ERR_WRITEFILE					        0xAA
#define ERR_CHECKKEY        				    0xA0
/********************************************ǰ̨������Ϣ��ʾend**********************/

//��λ��Ϊ1
#define SETBIT8 		0x80
#define SETBIT7 		0x40
#define SETBIT6 		0x20
#define SETBIT5 		0x10
#define SETBIT4 		0x08
#define SETBIT3 		0x04
#define SETBIT2 		0x02
#define SETBIT1 		0x01
//��λ��Ϊ0
#define UNBIT8 		0x7F
#define UNBIT7 		0xBF
#define UNBIT6 		0xDF
#define UNBIT5 		0xEF
#define UNBIT4 		0xF7
#define UNBIT3 		0xFB
#define UNBIT2 		0xFD
#define UNBIT1 		0xFE


#define Display_MAXNB					18
#define ACQUERELEN						11
#define ISSUERIDLEN						11
#define MAXLINECHARNUM			16

#define MAXGBCHARNUM				12
/* Global define */
#define ONESECOND						100
#define MAXMENUITEM					20
#define DATASAVEVALIDFLAG			0x55
#define DATASAVEPAGE_NORMAL			0
#define DATASAVEPAGE_TRANS				1
#define DATASAVEPAGE_BLACKLIST		2
#define EMV_MAXTAGNB						20
//#define EMV_MAXTRANSNUM                 300
#define CFG_MAXDISPCHAR					16
#define DISPCOLFIRST								0
#define CFG_MAXAMOUNTLEN				10
#define PARAM_SWITCHLEN					20
#define PARAM_APN								30
/* Waits for the end of the background dialling during 'TimeOut' x 10 ms */
#define CFG_CHECKDIALTIMEOUT			100

/* PINPAD Key define */
#if PS100
#define PINPAD     /*if define PINPAD ,there is external pinpad*/
#endif
#define KEYARRAY_GOLDENCARDSH		0x01

#define KEYINDEX_GOLDENCARDSH_PINKEY			0x07
#define KEYINDEX_GOLDENCARDSH_PINKEY2		0x08
#define KEYINDEX_GOLDENCARDSH_MACKEY		0x09

#define KEYINDEX_GOLDENCARDSH_CASHIERKEY	0x01
#define KEYARRAY_PURSECARD								0x00
#define KEYINDEX_PURSECARD_MASTERKEY			0x00
#define KEYINDEX_PURSECARD_CASHIERKEY			0x01
#define KEYINDEX_PURSECARD_PINKEY					0x02
#define KEYINDEX_PURSECARD_MACKEY				0x03
#define KEYINDEX_PURSECARD_ISSKEY					0x04
#define KEYINDEX_PURSECARD_WRITEKEY				0x06
#define KEYINDEX_PURSECARD_CRCKEY					0x08

#define KEYINDEX_GOLDENCARDSH_KEK			0x24

/* Trans info define */
#define TRANS_DATELEN					4
#define TRANS_TIMELEN						3
#define TRANS_ACCLEN						10
#define TRANS_GROUPLEN				3
#define TRANS_UintCodeLEN			11
#define	TRANS_REMARK						60
#define TRANS_AUTHCODELEN		6
#define TRANS_AUTHCODEMIN		2
#define TRANS_BANKIDLEN				6
#define TRANS_REFNUMLEN				12
#define TRANS_CASHIERNOLEN		4
#define TRANS_COMMLEN					41
#define TRANS_ISO2MINLEN				12
#define TRANS_ISO2LEN						37
#define TRANS_ISO3LEN						104
#define TRANS_PINDATALEN				8
#define TRANS_BATCHNUMLEN		6
#define TRANS_TRACENUMLEN		6
#define TRANS_TICKETNUMLEN		6
#define  TRANS_ORDERSNO			10 //�������
#define TRANS_INPUTMODE_SWIPECARD				0x01
#define TRANS_INPUTMODE_MANUALINPUT			0x02
#define TRANS_INPUTMODE_INSERTCARD 				0x04
#define TRANS_INPUTMODE_FULLBACK         			0x08
#define TRANS_INPUTMODE_PUTCARD						0x10
#define TRANS_INPUTMODE_HB_PUTCARD      		0x11
#define TRANS_INPUTMODE_SAND_PUTCARD 		0x12

#define TRANS_TERMINALTYPE_NORMAL		0x00
#define TRANS_TERMINALTYPE_FOREIGN		0x01

#define TRANS_ACCTYPE_DEFAULT			0
#define TRANS_ACCTYPE_ED					4
#define TRANS_ACCTYPE_EP					5

#define TRANS_CARDTYPE_INTERNAL		0
#define TRANS_CARDTYPE_VISA				1
#define TRANS_CARDTYPE_MASTER			2
#define TRANS_CARDTYPE_JCB				3
#define TRANS_CARDTYPE_DINNER			4
#define TRANS_CARDTYPE_AE					5
#define TRANS_CARDTYPE_FOREIGN		6

#define TRANS_CARDTYPE_INTERNALSMART	10

#define TRANS_BATCHSEND_INTERNAL	0x01
#define TRANS_BATCHSEND_FOREIGN		0x02


// ���߳�ʹ�ý�������������ԭ��POS����

#define TRANS_AID_PARAM	0xA3
#define TRANS_RID_PARAM	0xA4
#define TRANS_PRINT_BYTRACE	0xA1
#define TRANS_PRINT_LASTTRANS	0xA2
#define TRANS_PRINT_LASTSETTLE		0xA5
#define TRANS_PRINT_TRANSDETAIL	0xA6	
#define TRANS_PRINT_STATIC			0xA7



#define TRANS_SHOW_STATIC			0xA8
#define TRANS_SHOW_VERSION		0xA9

#define TRANS_SHOW_TRNSDETAIL		0xAA

#define TRANS_PRINT_TTS			0xAB

#define TRANS_QUERYBAL						0x01	//����ѯ
#define TRANS_PURCHASE						0x02	//����
#define TRANS_CASHOUT						0x03
#define TRANS_REFUND						0x04	// �˻�
#define TRANS_DEPOSIT						0x05
#define TRANS_TRANSFER						0x06
#define TRANS_AUTH							0x07
#define TRANS_PREAUTH						0x08	//Ԥ��Ȩ
#define TRANS_PREAUTHFINISH				       0x09	//Ԥ��Ȩ�������
#define TRANS_OFFPURCHASE					0x0A
#define TRANS_OFFCASHOUT					0x0B
#define TRANS_TIPADJUST						0x0C	//���ߵ���
#define TRANS_OFFPREAUTHFINISH		              0x0D	//���߽���
#define TRANS_TIPADJUSTOK					0x0E
#define TRANS_OFFPREAUTHFINISHOK	              0x0F
#define TRANS_PREAUTHSETTLE                              0x10	//Ԥ��Ȩ�������
#define TRANS_PREAUTHADD                                  0x11	//׷��Ԥ��Ȩ
#define TRANS_UNDOOFF						 			0x12	//���߳���
///////////////////////////////////////////////////////
#define TRANS_EC_PURCHASE				    		0x13  //�����ֽ�����2009-9-4 17:25 cbf
#define TRANS_EC_ASSIGNLOAD				    	0x14  //ָ���˻�Ȧ��
#define TRANS_EC_UNASSIGNLOAD			    0x15  //��ָ���˻�Ȧ��
#define TRANS_EC_CASHLOAD 				    	0x16  //�ֽ��ֵ
#define TRANS_Q_PURCHASE				    		0x17	//��������,�ǽӴ�
#define TRANS_OPENINGCREDITS				    0x18	//���ö�ȿ�ͨ
#define TRANS_CREDITSALE							    0x19	//��������
#define TRANS_CREDITSVOID						 0x24	//�������ѳ���
#define TRANS_CREDITSQUERY					    0x25	//��������ѯ
#define TRANS_CREDITSREFUND					    0x23	//�����˻�
/////////////////////////////////////////////////////
#define TRANS_REPRINT							0x79
#define TRANS_VOID								0x80
#define TRANS_VOIDPURCHASE				       0x82	//���ѳ���
#define TRANS_VOIDCASHOUT					0x83
#define TRANS_VOIDREFUND					0x84
#define TRANS_VOIDDEPOSIT					0x85
#define TRANS_VOIDTRANSFER				0x86
#define TRANS_VOIDAUTH						0x87
#define TRANS_VOIDPREAUTH					0x88	//Ԥ��Ȩ����
#define TRANS_VOIDPREAUTHFINISH		              0x89	//Ԥ��Ȩ��ɳ���
#define TRANS_VOIDOFFPURCHASE				0x8A
#define TRANS_LOGONON						0xF0
#define TRANS_LOGONOFF						0xF1
#define TRANS_CHANGESUPERPASS				0xF2
#define TRANS_BATCHUP						0xF3
#define TRANS_SETTLE							0xF4
#define TRANS_REVERSAL						0xF5
#define TRANS_CASHIERLOGON					0xF6
#define TRANS_CASHIERLOGOFF					0xF7
#define TRANS_LOADPARAM					0xF8
#define TRANS_ONLINETEST					0xF9
#define TRANS_SENDSTATS						0xFA
#define TRANS_SCRIPTINFORM              			0xFB
#define TRANS_CUPMOBILE						0x1A //�ƶ�֧������
#define TRANS_VOIDCUPMOBILE					0x1B //�ƶ�֧�����ѳ���
#define TRANS_SENDCOLLECTDATA				0x1C
#define TRANS_OPERATOR						0x1E //����Ա��֤����
#define TRANS_UPTMKEY						0x1F //��Կ��������

#define TRANS_MOBILEAUTH					0x20 //�ֻ��޿�ԤԼ����
#define TRANS_VOIDMOBILEAUTH				0x21 //�ֻ��޿�ԤԼ���ѳ���
#define TRANS_MOBILEREFUND					0x22 //�ֻ��޿�ԤԼ�˻�

#define TRANS_SAVEMAXNB					500

#define TRANS_NIINORMAL					0x000F	/*��ͨ����*/
#define TRANS_NIISENDFINISH				0x00F0	/*������ɽ��׵����߻���ʽ���*/
#define TRANS_NIIVOID					0x0F00	/*ȡ������*/
#define TRANS_BATCHSEND					0xF000	/*�����ͽ���*/

#define PARAM_DOWNKEYCOMM_HDLC		1
#define PARAM_DOWNKEYCOMM_GPRS		2
#define PARAM_DOWNKEYCOMM_TCPIP		3
#define PARAM_DOWNKEYCOMM_COMM		4

/* Terminal Parameter */
#define PARAM_PASSWORDLEN				6
#define PARAM_TERMINALIDLEN			8
#define PARAM_MERCHANTIDLEN			15
#define PARAM_MERCHANTNAMELEN	40
#define PARAM_TERMINALAPP				2
#define PARAM_TELNUMBERLEN				30
#define PARAM_HOSTNUMBERLEN			14
#define PARAM_GLOBALTRANSENABLELEN	2
#define PARAM_DATELEN						3
#define PARAM_CONFIGLEN					10
#define PARAM_DISPLAYMINLINE			2
#define PARAM_DISPLAYMAXLINE			4

#define PARAM_COMMMODE_MODEM			0
#define PARAM_COMMMODE_RS232				1
#define PARAM_COMMMODE_HDLC				2
#define PARAM_COMMMODE_GPRS				3
#define PARAM_COMMMODE_CDMA				4
#define PARAM_COMMMODE_TCPIP				5


#define PARAM_ISSUEIDLEN							8
/* Printer Parameter */
#define PRN_MAXCHAR									24

/* comms.c */
#define CHAR_STX				0x02
#define CHAR_ETX				0x03

/* Mirror Space Define */
#define MIRROR_MAXLEN			3072
#define MIRROR_NOTEXISTFLAG 	0x00
#define MIRROR_EXISTFLAG		0x55
#define MIRROR_INCLUDECHKFLAG	0xff
#define MIRROR_CLRFLAG			0xff

/* ISO8583 Define */
#define ISO8583_MSGIDLEN			2
#define ISO8583_BITMAPLEN			16
#define ISO8583_MAXBITNUM			128
#define ISO8583_MAXBITBUFLEN		1024	/* Max unpacked ISO8583 buffer */
#define ISO8583_MAXCOMMBUFLEN	1024    //512		/* Max Iso8583 communication buffer */
#define ISO8583_BITNOTEXISTFLAG 	0x00
#define ISO8583_BITEXISTFLAG		0xff
#define ISO8583_BIT48NUM			8
#define ISO8583_ICTRANSINFOLEN  	0xff

#define ATTR_N					0x01
#define ATTR_AN					0x02
#define ATTR_NS					0x03
#define ATTR_XN					0x04
#define ATTR_ANS				0x05
#define ATTR_LLVARN			0x06
#define ATTR_LLVARNS		0x07
#define ATTR_LLVARAN		0x08
#define ATTR_LLVARANS		0x09
#define ATTR_LLLVARANS	0x0A
#define ATTR_LLVARZ			0x0B
#define ATTR_LLLVARZ			0x0C
#define ATTR_BIN					0x0D
#define ATTR_LLLVARN		0x0E
#define ATTR_LLVARANZS	0x10
#define ATTR_LLLVARANZS 0x11

/* cash.c */
#define CASH_MAXSUPERNO             			2
#define CASH_MAXSUPERPASSWDLEN		6
#define CASH_MAXCASHIER						25
#define CASH_CASHIERNOLEN			    	2
#define CASH_CASHIERPASSLEN				4
#define CASH_SYSCASHIERNOLEN			2
#define CASH_SYSCASHIERPASSLEN		6

#define CASH_MAXTime					4
#define CASH_MINTime					4

#define CASH_LOGONFLAG			0x55
#define TRANS_NumStages			2
#define TRANS_NumProject			30
/* msg.c */
#define MSG_TYPEZHCN	'0'
#define MSG_TYPEEN		'1'

/* serv.c*/
#define SETTLE_OK		0x00
#define SETTLE_FAIL  	0x01
#define SETTLE_ERROR	0x02

/* sev.c*/
#define OldAuthType_POS		0x30
#define OldAuthType_TEL		0x31
#define OldAuthType_OTHER	0x32

#define EMVFIRSTMAG		0x11
#define EMVFIRSTICC			0x22

#define TYPE_HOTEL	0X32
#define TYPE_SHOP   0x31
#define RESTAURANT	0x33

/****************************************��ʾ��ʾ��Ϣ***********************/

#define MSG_NAME						0		/*�Ϻ���*/
#define MSG_QUERYBAL				1       /*����ѯ*/
#define MSG_INPUTNEWPIN		2       /*��������*/
#define MSG_BALANCE				3       /*"���*/
#define MSG_MASTERLOAD		4       /*����Կ��װ*/
#define MSG_EDQUERYBAL			5       /*���Ӵ������*/
#define MSG_EPQUERYBAL			6       /*����Ǯ�����*/
#define MSG_PURCHASE				7       /*����*/
#define MSG_SENDSTATS			8       /*״̬�ϴ�*/
#define MSG_LOADPARAM			9       /*��������*/
#define MSG_ONLINETEST			10      /*�������*/
#define MSG_SETDATETIME			11      /*����ϵͳʱ��*/
#define MSG_FPHONENO			12      /*�ֻ�����*/
#define MSG_INQUIRYVER			13      /*�汾��ѯ*/
#define MSG_CLEARTRANS			14      /*�뽻����ˮ*/
#define MSG_CHANGESTATUS	15      /*״̬�л�*/
#define MSG_KEYBOARDLOCK		16      /*��ǿ�ȡ��*/
#define MSG_PRINTTEST				17      /*��ӡ����*/
#define MSG_EPCASHOUT			18      /*Ǯ��ȡ��*/
#define MSG_REFUND					19      /*�˻�*/
#define MSG_MAGREFUND			20      /*�ſ��˻�*/
#define MSG_CREDITREFUND		21      /*���ÿ��˻�*/
#define MSG_DEBITREFUND		22      /*��ǿ��˻�*/
#define MSG_EDREFUND				23      /*�����˻�*/
#define MSG_EPREFUND				24      /*Ǯ���˻�*/
#define MSG_DEPOSIT					25      /*���*/
#define MSG_MAGDEPOSIT			26      /*�ſ����*/
#define MSG_CREDITDEPOSIT		27      /*���ÿ����*/
#define MSG_DEBITDEPOSIT		28      /*��ǿ����*/
#define MSG_EDDEPOSIT				29      /*���۴��*/
#define MSG_EPDEPOSIT				30      /*Ǯ�����*/
#define MSG_TRANSFER				31      /*ת��*/
#define MSG_MAGTRANSFER		32      /*�ſ�ת��*/
#define MSG_SYSTEMINIT			33      /*ϵͳ��ʼ��*/
#define MSG_DEBITTRANSFER		34      /*��ǿ�ת��*/
#define MSG_EDTRANSFER			35      /*����ת��*/
#define MSG_EPTRANSFER			36      /*Ǯ��ת��*/
#define MSG_AUTH						37      /*��Ȩ*/
#define MSG_MAGAUTH				38      /*�ſ���Ȩ*/
#define MSG_CREDITAUTH			39      /*���ÿ���Ȩ*/
#define MSG_DEBITAUTH				40      /*��ǿ���Ȩ*/
#define MSG_EDAUTH					41      /*������Ȩ*/
#define MSG_EPAUTH					42      /*Ǯ����Ȩ*/
#define MSG_PREAUTH				43      /*Ԥ��Ȩ*/
#define MSG_PREAUTHFINISH		44      /*Ԥ��Ȩ���*/
#define MSG_CREDITPREAUTH		45     	/*���ÿ�Ԥ��Ȩ*/
#define MSG_DEBITPREAUTH		46      /*��ǿ�Ԥ��Ȩ*/
#define MSG_EDPREAUTH			47      /*����������֤*/
#define MSG_EPPREAUTH			48      /*Ǯ��������֤*/
#define MSG_VOID						49      /*����*/
#define MSG_VOIDPURCHASE		50      /*���ѳ���*/
#define MSG_VOIDCASHOUT		51      /*ȡ���*/
#define MSG_VOIDREFUND			52      /*�˻�����*/
#define MSG_VOIDDEPOSIT		53      /*����*/
#define MSG_VOIDTRANSFER		54      /*ת�ʳ���*/
#define MSG_VOIDAUTH				55      /*��Ȩ����*/
#define MSG_VOIDPREAUTH				56      /*Ԥ��Ȩ����*/
#define MSG_VOIDPREAUTHFINISH	57      /*��Ȩ��ɳ���*/

#define MSG_DATETIME				58      /*ʱ��*/
#define MSG_REFERENCE				59      /*��ע:*/
#define MSG_HOLDERSIGN			60      /*�ֿ���ǩ��*/
#define MSG_REVERSALFAIL		61      /*����ʧ��*/
#define MSG_REVERSALSUCC		62      /*�����ɹ�*/
#define MSG_HOLDERCONFIRM			63      /*����ȷ�ϸý���*/
#define MSG_HOLDERCONFIRM2		64      /*ͬ����뱾�ʻ�*/

#define MSG_CONFIGBASEDATA			65     	/*��������*/
//#define MSG_CONFIGBASEDATA	65     /*�ն˺��̻���*/
#define MSG_CONFIGPBOCDATA		66      /*��ͨ������*/
#define MSG_DOAPPSELECT				67      /*Ӧ���Ƿ�ѡ��*/
#define MSG_PSAMREADER					68      /*��Կ������*/

#define MSG_SETTLE				69      /*����*/

#define MSG_APPSELECT					70    /*Ӧ��ѡ��*/
#define MSG_INPUTMASTERKEY		71    /*��������Կ*/
#define MSG_INPUTKEYINDEX			72    /*������Կ����*/
#define MSG_INPUTPIN					73	/*�����������*/
#define MSG_AMOUNT					74     /*"���:*/
#define MSG_ADDCASHIER				75     /*��Ա����*/
#define MSG_DELCASHIER				76     /*��Աɾ��*/
#define MSG_MODICASHIER			77     /*��Ա�޸�*/
#define MSG_CASHIERNO				78     /*��Ա��:*/
#define MSG_CASHIERPASS				79     /*��Ա����:*/
#define MSG_CASHIERNEWPASS		80     /*��Ա������:*/
#define MSG_POSLOGON			81     /*�豸ǩ��*/
#define MSG_POSLOGOFF			82     /*�豸ǩ��*/
#define MSG_SUPERPASS			83     /*ϵͳ����Ա����:*/
#define MSG_LOGONSUCCESS	84     /*ǩ���ɹ�*/
#define MSG_LOGONFAIL			85     /*ǩ��ʧ��*/
#define MSG_TERMINALID			86     /*�ն˺�:*/
#define MSG_MERCHANTID			87     /*�̻���:*/
#define MSG_COMMMODE			88     /*M 1-HDLC,2-232*/
#define MSG_DIALRETRY				89     /*"Dial Retry Time */
#define MSG_DIALWAITTIME		90       /*Dial Wait Times*/
#define MSG_COMMSPEED			91       /*Comm Speed:*/
#define MSG_RECEIVETIMEOUT		92       /*Receive Timeout*/
#define MSG_HOSTTEL1			93       /*���ĺţ�:*/
#define MSG_HOSTTEL2			94      /*���ĺţ�:*/
#define MSG_HOSTTEL3			95       /*I���ĺ���3 */
#define MSG_HOSTTEL4			96       /*�������:*/

#define MSG_REFNUM				97      /*�ο���:*/
#define MSG_AUTHCODE			98      /*��Ȩ��:*/
#define MSG_BATCHNUM			99      /*���κ�:*/
#define MSG_TRACENUM			100      /*ƾ֤��:*/
#define MSG_TRANSDATE			101      /*��������(MMDD)*/
#define MSG_CASHIERPINRELOAD	102      /*��Ա��������*/
#define MSG_CHANGESUPERPASS		103      /*�޸���������*/
#define MSG_GOON				104      /*����?*/
#define MSG_CANCEL				105      /*ȡ��?*/
#define MSG_CASH_EXIST			106      /*��Ա�Ѵ���*/
#define MSG_NOTEXIST			107      /*��Ա������*/
#define MSG_CASHIERNOTLOGON		108      /*��Աδǩ��*/
#define MSG_MERCHANTNAME		109      /*�̻�����:*/
#define MSG_DIALING				110      	/*�����У�����*/
#define MSG_SENDING				111      	/*�����У�����*/
#define MSG_RECEIVING			112      	/*�����У�����*/
#define MSG_SAVING				113     	/*����*/
#define MSG_TPDU            	114    	 /*TPDU:*/
#define MSG_REVERSAL			115    	 /*����������*/

#define MSG_DATE				116			/*����:*/
#define MSG_TIME				117     	/*ʱ��:*/
#define MSG_CARDNO				118     	/*�����뿨��:*/
#define MSG_ISSUERID			119     	/*�����к�:*/
#define MSG_TRANSTYPE			120     	/*��������:*/
#define MSG_SPDB				121     	/*���������Ϻ�����*/
#define MSG_ALREADYVOID			122     	/*�����ѳ���*/
#define MSG_NOTRANS				123     	/*���ײ�����*/
#define MSG_REPRINT				124     	/*�ش����һ��*/
#define MSG_PRINTNUMBER			125     	/*��ӡ�ڼ���?*/
#define MSG_CASHIERLOGON		126     	/*��Աǩ��*/
#define MSG_CASHIERLOGOFF		127		/*��Աǩ��*/
#define MSG_LIST				128     	/*����ͳ�Ƶ�*/
#define MSG_TOTALNB				129     	/*�����ܼ�*/
#define MSG_TOTALAMOUNT			130     	/*����ܼ�*/
#define MSG_REFUNDNB			131     	/*�˻��ܱ���*/
#define MSG_PURCHASEAMOUNT		132     	/*�����ܽ��*/
#define MSG_AUTHFINISHAMOUNT	133 		/*��Ȩ�ܽ��*/
#define MSG_REFUNDAMOUNT		134      	/*�˻��ܽ��*/
#define MSG_LISTPRINT			135      	/*��ӡ������ϸ*/
#define MSG_LOGOFFSUCCESS		136      	/*ǩ�˳ɹ�*/
#define MSG_SYSPASS				137	  	/*ϵͳ����*/
#define MSG_EXPDATE				138	  	/*����Ч��(MMYY):*/
#define MSG_SETTLEOK			139       	/*����ɹ�!*/
#define MSG_SETTLEFAIL			140       	/*����ʧ��!*/
#define MSG_CONFIRM				141	  	/*��ȷ��*/
#define MSG_PRINTLASTERONE		142        	/*��ӡ���һ��?*/

#define MSG_SPECIALTRANS		143        	/*ˢת����*/
#define MSG_PRINTING			144	  	/*���ڴ�ӡ...*/
#define MSG_CARDLOKE        	145	  	/*��Ƭ����*/
#define MSG_TIPADJUST			146	  	/*���ߵ���*/
#define MSG_FEEAMOUNT			147	  	/*С��*/
#define MSG_OFFPREAUTHFINISH	148        	/*���߽���*/
#define MSG_IFINPUTPIN			149	   	/*��������ȷ��*/
#define MSG_INPUTDT				150		/*ϵͳʱ������*/
/********************�������ѿ�����*****************************************/
#define MSG_READCARD			151		/*��ͬ����*/
#define MSG_WRITECARD			152		/*дͬ����*/
#define MSG_SECRETCARD			153		/*����Կ��֤*/
#define MSG_MODIFYCARD			154		/*�Ŀ���Կ*/
#define MSG_DIAL_T				155		/*DAIL T=*/
#define MSG_SWITCHBOARD			156		/*SWITCHBOARD=*/
#define MSG_TERMINALTOTAL		157		/*�ն�ͳ��*/
/******************************************************************************/
#define MSG_INIT_SUPER_PASSWORD 	158		/*��ʼ����������*/
#define MSG_INPUT_MERCHANT_NAME 	159		/*�����̻���*/
#define MSG_DELETE_REVERSAL_FLAG 	160		/*ɾ��������־*/
#define MSG_LOADDEFUALTKEY		161		/*����Ĭ����Կ*/
#define MSG_SETTLE_UNBALANCE 		162 		/*���㲻ƽ*/
#define MSG_TRANS_LIST				163		/*��ˮ��ϸ��ѯ*/
#define MSG_WELCOME				164		/*��ӭʹ��*/
#define MSG_CLEAR_TRANS 			165		/*���������ˮ*/

#define MSG_SYS_CHANGEPASS 		166		/*ϵͳ����Ա����*/
#define MSG_ADD_SUPVCASHIERNO 	167		/*���ܲ���Ա����*/
#define MSG_DEL_SUPVCASHIERNO 	168		/*���ܲ���Աɾ��*/
#define MSG_CHG_SUPVCASHIERNO 	169		/*���ܲ���Ա����*/
#define MSG_SYS_CASHIERNO 			170		/*�����ܲ���Ա��*/
#define MSG_SYS_CASHIERPASS 		171		/*����������*/
#define MSG_SET_SYSTRANS	 		172		/*ϵͳ�����趨*/
#define MSG_PREAUTHSETTLE     		173    	/*Ԥ��Ȩ����*/
#define MSG_OFFPURCHASE       		174    	/*�ѻ�����*/
#define MSG_VOIDOFFPURCHASE   		175    	/*�ѻ����ѳ���*/
#define MSG_DELETE_SCRIPT_FLAG 	176 		/*ɾ���ű�֪ͨ��־*/
#define MSG_PKDOWNLOAD          		177   	/*��Կ����*/
#define MSG_EMVPARAMDOWNLOAD   	178   	/*EMV��������*/
#define SET_PRINTER					179 		/* ��ӡ������ */
#define MSG_REPRINTBYTRACE			180    	/* �ش�����һ��*/
#define MSG_TANSTOTAL				181    	/* ��ӡ���׻���*/
#define MSG_PREAUTHADD            		182     	/* Ԥ��Ȩ׷��*/
#define MSG_CLEARMEMERY			183		/* ���ڴ� */
#define MSG_SETPINPAD				184		/*�����������*/
#define MSG_CASHIERQUERY			185		/*��Ա��ѯ*/
#define MSG_AUTHADDAMOUNT 		186 		/*�ۼƽ��*/
#define MSG_READINGCARD			187 		/*���ڶ���*/
#define MSG_SETREADER				188		/*�ⲿ����������*/
#define MSG_REPRINTSETTLE			189		/*�ش���㵥*/
#define MSG_RATESET 				190		/*��������*/
#define MSG_DEFAULTKEY				191		/*������������Կ*/
#define MSG_MUNUALSETKEY			192		/*�ֹ�������Կ*/

/****************************************��ʾBANKIC***********************/
#define BANK_TRAFFICBANK			193   	/*��ͨ�����Ϻ�����*/
#define BANK_CONSTRUCT  			194 		/*���������Ϻ�����*/
#define BANK_INDUSCOMMERCE		195   	/*���������Ϻ�����*/
#define BANK_XINGYE					196    	/*��ҵ�����Ϻ�����*/
#define BANK_HUAXIA					197 		/*���������Ϻ�����*/
#define BANK_AGRICULTURE		198   	/*ũҵ�����Ϻ�����*/
#define BANK_SHENGDEVELOP	199       /*�չ�Ϻ�����*/
#define BANK_GUANGDA     		200       /*��������Ϻ�����*/
#define BANK_CHINABANK   		201       /*�й������Ϻ�����*/
#define BANK_MINSHENG			202       /*���������Ϻ�����*/
#define BANK_SHANGHAI			203		/*�Ϻ�����*/
#define BANK_ZHAOSHANG		204		/*���������Ϻ�����*/
#define BANK_GUANGFA				205		/*�㷢�����Ϻ�����*/
#define BANK_ZHONGXIN			206		/*���������Ϻ�����*/
#define BANK_OTHERS					207		/*��������*/
#define BANK_PUFA						208		/*�ַ�����*/
#define BANK_COUNTRY				209		/*ũ��������*///ԭũ�������磬��ũ����ҵ����
#define BANK_POSTSAVE            	210		/*��������*///ADD BY JIANG.GS
#define MSG_SetOutoSettleTime	211		/*�����Զ�����ʱ��*/
#define MSG_SUPERKEY				212		/*��������ָ�*/
#define MSG_SUPERMOD			213		/*��������*/
#define BANK_NINGBO				214		/*���������Ϻ�����*/
#define MSG_SwitchComm			215		/*�л�ͨѶ*/
#define MSG_QuanCun				216		/*Ȧ��*/
#define MSG_EnqBalance				217		/*����ѯ*/
#define MSG_TestSignal				218		/*�������*/
#define MSG_ModManKey			219		/*�޸���������*/
#define MSG_DOWNKEY				220		/*��ĸPOS�·�����Կ*/
#define MSG_INITDATA				221		/*�������*/
#define MSG_VOIDCUP				222		/*�ƶ�֧������*/
#define MSG_KEYLOAD				223		/*������Կ����*/
#define MSG_SETKEYPARA			224		/*����Զ����Կ���ز���*/
#define MSG_UCSETREADER           225  /*����������*/
#define MSG_CLEARCOLLECT           226  /*����ɼ�����*/
#define MSG_DKFROMC           227  /*����Կ������Կ*/
/****************************************��ʾ��ʾ��Ϣend***********************/

#define ERR_MERSET              0x90

#define MSG_DEFAULTMSGEN		"Default\0"
#define MSG_DEFAULTMSGZHCN	"ȱʡ\0"
#define MSG_INVALIDMSGEN		"Invalid Index\0"
#define MSG_INVALIDMSGZHCN	"��Ч\0"

#define MSG_MAXERRMSG			100
#define MSG_MAXNB				255
#define MSG_MAXHOSTMSG		100
#define MSG_MAXCHAR			26
#define MSG_MAXTYPE			2
/* comms.c */
#define COMMS_RECVTIMEOUT		2   //changed 08-09-16 Before 200
/*UTIL.c*/
#define PAPER_KM			0x01
#define PAPER_PE				0x02
#define PAPER_XSBN		0x03

/* pboc.c */
#define PBOC_MAXTERMINALAPP	10
#define PBOC_MAXCARDAPP		5
#define PBOC_MAXAPPIDLEN		16
#define PBOC_ISSUERIDLEN		8
#define PBOC_APPLINOLEN		10

/* admin.c */
#define ADMIN_END						'0'
#define ADMIN_DOWNLOADMSG			'1'
#define ADMIN_DOWNLOADERRMSG			'2'
#define ADMIN_DOWNLOADISO8583ATT	'3'

#define ADMIN_COMMANDOFFSET			  0
#define ADMIN_DATAOFFSET			  	1
#define MAXMENUDISPLINE				10

/* print.c */
#define PRINT_MAXCHAR			50
#define PRINT_MAXRETRYTIME		3
#define RE_PRINT_FLAG			0x01
#define NORMAL_PRINT_FLAG		0x02
#define PRINT_LIST_FLAG			0x04
#define PRINT_TOTAL_FLAG		0x08
#define PRINT_SETTLE_FLAG       	0x10
/* Macro */
#define min(a, b)           (((a) < (b)) ? (a) : (b))

#define EMVAIDLEN              17
#define SCRIPTINFORMMAX       300

#define ONLYMAGTRANS

/* 	Types  */

#define ULONG_C51 unsigned long
#define UINT_C51 unsigned int

#define PRINTER_HEAT_FORMAT_1	0x01	//������ʽһ
#define PRINTER_HEAT_FORMAT_2	0x02	//������ʽ��
#define PRINTER_HEAT_FORMAT_3	0x03	//������ʽ��
#define PRINTER_HEAT_FORMAT_4	0x04	//������ʽ��

#define MobileNUMLen 			12
#define MobileCHKLen 			7

typedef struct
{
	unsigned char ucCla;
	unsigned char ucIns;
	unsigned char ucP1;
	unsigned char ucP2;
	unsigned char ucP3;
	unsigned char aucDataIn[128];
}ISO7816IN;

typedef struct
{
	unsigned short	uiLen;
	unsigned char	ucStatus;
	unsigned char	ucSW1;
	unsigned char	ucSW2;
	unsigned char	aucData[255];
}ISO7816OUT;

typedef struct
{
	unsigned char ucTransType;
	unsigned short uiDispIndex;
	unsigned char (*pfnProc)(void);
}DISPMENU;

typedef struct
{
	DISPMENU DispMenu[MAXMENUDISPLINE];
	unsigned char ucMenuCnt;
}SELMENU;

/* Smart Card struction define *
typedef struct
{
	unsigned short	uiTag;
	unsigned char	ucChildExist;
	unsigned char	ucLen;
}EMVTAG;

typedef struct
{
	unsigned char	ucNumber;
	unsigned short	auiTag[EMV_MAXTAGNB];
	unsigned char	aucLen[EMV_MAXTAGNB];
	unsigned char	*pucOffset[EMV_MAXTAGNB];
}EMVTAGBuf;
*/
typedef struct
{
	unsigned char	ucAppNumber;
	unsigned char	aucAppType[PBOC_MAXTERMINALAPP];
	unsigned char	aucAppID[PBOC_MAXTERMINALAPP][PBOC_MAXAPPIDLEN];
}TERMINALAPP;

typedef struct
{
	unsigned char	ucAppNumber;
	unsigned char	aucAppIDLen[PBOC_MAXCARDAPP];
	unsigned char	aucAppID[PBOC_MAXCARDAPP][PBOC_MAXAPPIDLEN];
	unsigned char	aucAppLabel[PBOC_MAXCARDAPP][PBOC_MAXAPPIDLEN+1];
	unsigned char	ucCurrAppNumber;
	unsigned char	ucTermAppNumber;
}CARDAPP;

typedef struct
{
	unsigned short uiSendIndex;
}SAVESENDINDEX;

typedef struct
{
	unsigned char	aucIssuerID[PBOC_ISSUERIDLEN];
	unsigned char	ucAppliID;
	unsigned char	ucAppliVersion;
	unsigned char	aucAppliNo[PBOC_APPLINOLEN];
	unsigned char	aucAppliOpenDate[4];
	unsigned char	aucAppliExpiredDate[4];
	unsigned char	aucIssuerPrivateDate[2];
}PBOCBANKDATA;
/* Memory Card */
typedef struct
{
	unsigned char	aucMarketNo[8];
	unsigned char	aucSerial[2];
	unsigned char 	aucAccountNo[8];  	/*����*/
	unsigned char   aucBIN;			/*���к�*/
	unsigned char   aucCardMap;
	unsigned char	aucKeyVer;
    unsigned char 	aucRFU[3];
	unsigned char   aucOpenDate[4];		/*�ƿ�����*/
	unsigned char 	aucSaveDate[4];		/*�������ֵ����*/
	unsigned int   Amount;
    unsigned char	aucCRC[4];
}SLE4442DATA;

enum CARDSPECIES {CARDSPECIES_MAG,CARDSPECIES_EMV,CARDSPECIES_UNTOUCH,CARDSPECIES_HB_UNTOUCH};
enum LOADPARAMTYPE {LOADPARAMTYPE_MAG,
						LOADPARAMTYPE_PKQUERY,
						LOADPARAMTYPE_PKLOAD,
						LOADPARAMTYPE_PKLOADEND,
						LOADPARAMTYPE_RID,
						LOADPARAMTYPE_AIDQUERY,
						LOADPARAMTYPE_AIDLOAD,
						LOADPARAMTYPE_AIDLOADEND};
enum ENCRYPTTYPE {ENCRYPTTYPE_SINGLE,ENCRYPTTYPE_DOUBLE};

// ɼ��QtӦ���¼ӽӿ�
typedef struct
{
	//���Դ���: progress ��UI����
	unsigned char ucClsFlag;	// 0xAA �����Ļ
	
	
	unsigned int uiLines;		//��ʾ����
	unsigned char aucLine1[64];
	unsigned char aucLine2[64];
	unsigned char aucLine3[64];
	unsigned char aucLine4[64];	
	unsigned char aucLine5[64];	
	unsigned char aucLine6[64];	
	unsigned char aucLine7[64];	
	unsigned char aucLine8[64];		
	unsigned char aucLine9[64];
	
	unsigned char aucLine10[64];
	unsigned char aucLine11[64];
	unsigned char aucLine12[64];
	unsigned char aucLine13[64];
	unsigned char aucLine14[64];	
	unsigned char aucLine15[64];	
	unsigned char aucLine16[64];	
	unsigned char aucLine17[64];	
	unsigned char aucLine18[64];		
	unsigned char aucLine19[64];
		
	unsigned char aucLine20[64];
	unsigned char aucLine21[64];
	unsigned char aucLine22[64];
	unsigned char aucLine23[64];
	unsigned char aucLine24[64];	
	unsigned char aucLine25[64];	
	unsigned char aucLine26[64];	
	unsigned char aucLine27[64];	
	unsigned char aucLine28[64];		
	unsigned char aucLine29[64];
		
	unsigned char aucLine30[64];
	unsigned char aucLine31[64];
	unsigned char aucLine32[64];
	unsigned char aucLine33[64];
	unsigned char aucLine34[64];	
	unsigned char aucLine35[64];	
	unsigned char aucLine36[64];	
	unsigned char aucLine37[64];	
	unsigned char aucLine38[64];		
	unsigned char aucLine39[64];
	unsigned char aucLine40[64];
	
}FACEPROTOUI;
typedef struct
{
	unsigned char ucOweFlag;	// ˢ���ɹ�

	unsigned char ucInputMode;	//ˢ����ʽ
	unsigned char aucCardAcc[512];

	unsigned int uiLenIso1;
	unsigned char aucIso1[76];		// 76

	unsigned int uiLenIso2;
	unsigned char aucIso2[38];		// 37	

	unsigned int uiLenIso3;
	unsigned char aucIso3[110];	// 104

	unsigned char aucMagInfo[20]; //������ʾ
}OWECARDINFO;
typedef struct
{ 
	//���׽ӿ�: ui���洫�ݵ� progress
	unsigned char ucTransType;	//��������
	unsigned char ucEPTrans;  // ʹ�õ���Ǯ��
	unsigned char ucPrintDetailFlag;// settle print detail

	unsigned char aucOldTrace[7];	// trace Nb

	unsigned char aucTransDate[5];	
	unsigned char aucRefNum[13];	
	unsigned char aucAuthNo[7];	

	
	OWECARDINFO CardInfo;
	
	unsigned long ulAmount;		//���
	unsigned int uiPwLen; 			// ���볤��
	unsigned char aucPasswd[24]; 	// ����
	unsigned int  uiIndex;// ��ѯ��������
}FACEUITOPRO;

typedef struct
{
	unsigned char ucProBar;//�Ƿ��н�����
	unsigned int  uiProgress;//����������

	unsigned int  uiTimes;//����ʱ

	NEW_DRV_TYPE OweDrv;
	FACEPROTOUI ProToUi;
	FACEUITOPRO UiToPro;
}PROUIINTERFACE;

// ɼ��QtӦ���¼ӽӿ� end.
#if 0
typedef struct
{
	UCHAR		 	 ucTermCAPKNum;
	CAPK		 	 TermCAPK;
	UCHAR		 	 ucTermAIDNum;
	TERMSUPPORTAPP	 TermAID;
	UCHAR			 ucIPKRevokeNum;
	IPKREVOKE		 IPKRevoke;
	UCHAR			 ucExceptFileNum;
	EXCEPTPAN		 ExceptFile;
	EMV_CONSTPARAM ConstParam;
	TRANSREQINFO 	 TransReqInfo;

}EMVTRANSINFO;
#endif

/* Normal Transaction Information */
typedef struct
{
	unsigned char		ucCardTableIndex;
	enum CARDSPECIES     euCardSpecies;
	unsigned char   ucPANSeqExist;
	unsigned char   ucPANSeq;
	unsigned char	aucUntouchSeq[20];
	unsigned char   aucAID[EMVAIDLEN];
	unsigned char   ucAIDLen;
	unsigned char   AppCrypt[8];
	unsigned char 	TVR[5];
	unsigned char  TSI[2];
	unsigned char	ATC[2];
	unsigned char 	CVM[3];
	unsigned char   ucAppLabelLen;
	unsigned char   aucAppLabel[16];
	unsigned char   ucAppPreferNameLen;
	unsigned char 	aucAppPreferName[16];
	unsigned char  ucFallBackFlag;
	unsigned char	ucCardType;
	unsigned char	aucOldDate[TRANS_DATELEN];
	unsigned char	aucDate[TRANS_DATELEN];
	unsigned char	aucTime[TRANS_TIMELEN];
	unsigned char	ucSourceAccType;
	unsigned char	aucSourceAcc[TRANS_ACCLEN];
	unsigned char	ucSourceAccLen;
	unsigned char	ucOldAuthType;
	unsigned char	aucAuthGroup[TRANS_GROUPLEN];
	unsigned char	aucCardRemark[TRANS_REMARK];
	unsigned char	aucOldRefNumber[TRANS_REFNUMLEN];
	unsigned char	ucCardRemarkFlag;
	unsigned char	ucTIPAdjustFlag;
	unsigned char	ucTransType;
	unsigned char	ucOldTransType;
	unsigned char	ucOldOldTransType;
	unsigned char ucInputMode;
	unsigned char	ucExpiredDate[TRANS_DATELEN];
	unsigned char ucBatchSendExpiredDate[TRANS_DATELEN];
	unsigned char	aucSettleDate[TRANS_DATELEN];
	unsigned char	aucISO2[20];
	unsigned char	ucISO2Len;
	unsigned char	ucISO3Len;
	unsigned char ucDispSourceAcc[TRANS_ACCLEN*2];
	unsigned char	ucDispSourceAccLen;
	unsigned char	aucISO3[53];
	unsigned char   	aucCardPan[6];
	unsigned char aucCardPan_UnAssign[6];
	unsigned char   	ucNotSendOkFlag;
	ULONG_C51		ulOldAmount;
	ULONG_C51		ulAmount;
	ULONG_C51		ulAuthAddTotalAmount;
	ULONG_C51		ulPrevEDBalance;
	ULONG_C51		ulPrevEPBalance;
	ULONG_C51		ulFeeAmount;
	ULONG_C51		ulTicketNumber;
	ULONG_C51		ulTraceNumber;
	ULONG_C51		ulOldTraceNumber;
	ULONG_C51		ulOldOldTraceNumber; //�����ˮ��
	ULONG_C51		ulTrueAmount;	/*�����*/
	ULONG_C51		ulOldTrueAmount;
	unsigned char   	aucBankID[TRANS_BANKIDLEN*2];
	unsigned char		aucAuthCode[TRANS_AUTHCODELEN+1];
	unsigned char 		aucBatchSendAuthCode[TRANS_AUTHCODELEN];
	unsigned char		aucOldAuthCode[TRANS_AUTHCODELEN];
	unsigned char 		aucAuthCodeHost[TRANS_AUTHCODELEN];
	unsigned char		aucAuthUintCode[TRANS_UintCodeLEN];
	unsigned char		aucCashierNo[TRANS_CASHIERNOLEN];
	unsigned char		aucRefNum[TRANS_REFNUMLEN];
	unsigned char		aucComm[TRANS_COMMLEN];
	unsigned char 	ucICEPKeyVer;
	unsigned char 	ucICEPMatchFlag;
	unsigned char 	ucMac1[4];
	unsigned int 		uiField55Len;
	unsigned char 	ucField55Data[256];
	unsigned short	uiOldTraceIndex;
	unsigned char 	ucICOnlineOrOfflineFlag;
	unsigned char	aucCashTicket[CASHIERTRACE_LEN];
	unsigned char	ucTIPCode;
	unsigned char  ucPrintFee;
	ULONG_C51	ulFeeNumber;
	unsigned char aucOrdersNo[TRANS_ORDERSNO];  //�������
	unsigned char aucOldOrdersNo[TRANS_ORDERSNO]; //����ʱ�õ�ԭ������
	unsigned char ucECTrans;
	ECTRANSINFO PECTransInfo;
	unsigned char aucUnAssignAccountISO2[TRANS_ISO2LEN];
	unsigned char aucUnAssignAccountISO3[TRANS_ISO2LEN];
	unsigned char ucUAAISOLen2;
	unsigned char ucUAAISOLen3;
	unsigned char ucMainAccountLen;
	unsigned char ucPTicket;
	unsigned char	aucTargetAcc[TRANS_ACCLEN];
	unsigned char	ucVoidFlag;
	unsigned char	aucMobileNUM[MobileNUMLen]; 	//�ֻ���
	unsigned char	aucMobileCHK[MobileCHKLen];		//�ֻ�У���
	unsigned char	aucNumStages[2];
	unsigned char	aucProjectNum[TRANS_NumProject];
	UCHAR bInstallments;									//�Ƿ�һ����֧�������֧��
	ULONG_C51	ulRepayAmount;			//���ڻ�����
	ULONG_C51	ulRepayFeeAmt;				//���ڸ���������
	unsigned char aucCBuf[3];						//�������
	ULONG_C51	ulStages;							//��������
	unsigned char	ucStagesType;					//������֧����ʽ
	ULONG_C51	ulFCurrency;					//����������
	ULONG_C51	ulNCurrency;					//ÿ��������
    unsigned char   aucCustName[20];//�ͻ�����
	unsigned char   aucCustNo[2];//�ͻ����
}NORMALTRANS;	//���潻�׽ṹ����ӡʹ�á��ļ�[savetrans]

/* Extra Transaction Information */
typedef struct
{
	unsigned char	 ucISO2Status;
	unsigned short uiISO2Len;
	unsigned char	aucISO2[TRANS_ISO2LEN];
	unsigned char	ucISO3Status;
	unsigned short uiISO3Len;
	unsigned char	aucISO3[TRANS_ISO3LEN];
	unsigned char	ucInputPINLen;
	unsigned char	aucPINData[TRANS_PINDATALEN];
	unsigned char	ucInputPINLen_UnAssign;//��ָ���˻���
	unsigned char	aucPINData_UnAssign[TRANS_PINDATALEN];
	unsigned char	aucHostRespCode[2];
	unsigned char	aucSessionKey[10];
	unsigned char	ucSessionKeyLen;
	unsigned char  aucReadCardTime[10];
	ULONG_C51	ulRefundBatchNumber;

}EXTRATRANS;//��ʱʹ�ã����ṹ��

typedef struct
{
	unsigned char ucInquiry;
	unsigned char	ucAuth;
	unsigned char	ucUnAuth;
	unsigned char	ucAuthFin;
	unsigned char	ucUnAuthFin;
	unsigned char	ucSale;
	unsigned char	ucUnSale;
	unsigned char	ucRefund;
	unsigned char	ucOffline;
	unsigned char	ucTipFee;
	unsigned char ucAuthFinishOff;
	unsigned char ucICScript;
	unsigned char ucOffSale;
	unsigned char ucAddAuth;
	unsigned char ucPurseDeposit;

}ENABLETRANS;

typedef struct
{
	unsigned short	ucErrorExtCode;
	unsigned char	ucTerminalType;
	unsigned char	ucCardType;
	unsigned char	ucDialRetry;
	unsigned char	ucHostConnectFlag;
	unsigned short	uiTransIndex;
	unsigned short	uiOldTransIndex;
	ULONG_C51		ulOldTraceNumber;
	ULONG_C51		ulOldFeeAmount;
	unsigned char	ucPINRetry;
	unsigned char	aucTerminalID[PARAM_TERMINALIDLEN+4];
	unsigned char	aucMerchantID[PARAM_MERCHANTIDLEN];
	unsigned char	aucVoidRefNum[TRANS_REFNUMLEN+1];
    unsigned char   aucVoidAuthCode[TRANS_AUTHCODELEN+1];
    unsigned char   aucOrignalDate[TRANS_DATELEN*2+1];
    unsigned char   aucOriginalBatchNum[TRANS_BATCHNUMLEN+1];
    unsigned char	aucOriginalTraceNum[TRANS_TRACENUMLEN+1];
    unsigned char	aucOriginalTicketNum[TRANS_TICKETNUMLEN+1];
    unsigned char	ucTransType;		/*����������ʹ��������������*/
	PBOCBANKDATA	PBOCBankData;
//	SLE4442DATA		MEMBankData;
    unsigned char	aucNewSuperPass[CASH_MAXSUPERPASSWDLEN+1];
    unsigned char	aucOriginalTerminalId[PARAM_TERMINALIDLEN+1];
    unsigned char   ucReprintFlag;
   	unsigned char ReadCardFlag;
    ENABLETRANS		TransEnable;
    unsigned char	ucTIPSaving;
    unsigned char	ucAdjustAmoutFlag;	/*��Ϊ������С�ѡ���������ԭ������С*/
    unsigned char	ucFiledErrNo;		/*��¼��������¼��*/
    unsigned char    ucProcessCode[6];
	unsigned char ucAmount[12];
	unsigned char ucTransNum[6];
	unsigned char ucTerminerID[8];
	unsigned char ucMarchID[15];
	unsigned char ucDefOperation;
	unsigned char ucEPReadCardFlag;
	unsigned char ucDialFlag;
	unsigned char ucSettleFlag;
	unsigned char ucReadCardFlag;
	unsigned char ucSettleLogonflag;
	unsigned char ucPreDialFalg;
	unsigned char aucOldCardPan[20];
	unsigned char   ucAIDLen;			  //2009-9-4 11:31  cbf
	unsigned char   aucAID[EMVAIDLEN];    //2009-9-4 11:31 cbf
	unsigned char ucQInputPinFlag;		//
	unsigned char aucOperaBuf[17];
	unsigned char aucOperaBufKey[9];
	unsigned short ucErrorFileCode;
	unsigned char aucMBFEBuf[10];
	unsigned char ucPTicketFlag;
	unsigned char ucOrderFlag;
	unsigned char aucMerchantName_cust[9][20+1];//�ͻ�����
    unsigned char ucReversalFlag;
	UCHAR bConectFlag;
}RUNDATA;//��ʱʹ��.

typedef struct
{
	unsigned char	ucDispMsg[17];
	unsigned char	(*pFct)();
}SYSMENU;

typedef struct// add dpnew
{
	unsigned char	dp;
	unsigned char	chdt;
	unsigned char	dt1;
	unsigned char	dt2;
	unsigned char	ht;
	unsigned char	wt;
	unsigned char	ssetup;
	unsigned char	dtimes;
	unsigned char	timeout;
	unsigned char	asmode;
}MODEM_PARAM;

/* Saved Data struct define */
typedef struct
{
	unsigned char	ucSetFlag;
	unsigned char	aucPassword[PARAM_PASSWORDLEN];
	unsigned char	ucTerminalType;
	unsigned char	aucTerminalID[PARAM_TERMINALIDLEN];
	unsigned char	ucMerchantType;
	unsigned char	aucMerchantID[PARAM_MERCHANTIDLEN];
	unsigned char	aucMerchantName[PARAM_MERCHANTNAMELEN];	/*�̻�����*/
	unsigned char	ucCommMode;
	unsigned char	ucDialRetry;					/*�ز�����*/
	unsigned short	uiDialWaitTime;			/*�ȴ���ͨʱ��*/
	unsigned int	uiCommSpeed;
	unsigned int	uiReceiveTimeout;		/*���ճ�ʱʱ��*/
	unsigned char 	ucLoadFlag;				/*�������ر�־*/
	unsigned char 	aucTermianlApp[PARAM_TERMINALAPP];		/*�ն�Ӧ������*/
	unsigned char 	ucXorTipFee;			/*�Ƿ�֧��С��*/
	unsigned int usTipFeePre;				/*С�Ѱٷֱ�*/
	unsigned char	ucInputCardNo;				/*�Ƿ�֧�����俨��*/
	unsigned char	ucSelfLogoff;				/*�Զ�ǩ�˱�־*/
	unsigned char ucOFFLineSendType;			/*�������ͷ�ʽ*/
	unsigned char	ucUseKeyIndex;				/*����Կ����*/
	unsigned char ucTransRetry;				/*�����ط�����*/
	unsigned char ucPrnErrTicket;				/*��ӡ���ϱ��浥��־*/
	unsigned char	ucUnAuthFinSwipe;			/*Ԥ��Ȩ��ɳ����Ƿ�ˢ��*/
	unsigned char	ucUnSaleSwipe;				/*���ѳ����Ƿ�ˢ��*/
	unsigned char ucVoidInputPin;				/*���������Ƿ�������*/
	unsigned char ucUnManagePwd;			/*���ѳ����Ƿ�����������*/
	unsigned char uICCInfo;					/*���������Ƿ�������*/
	unsigned char ucUseLogonDisp;				/*ǩ��״̬��ʾ����*/
	unsigned char ucRePAuthority;				/*�ش�ӡ����Ȩ��*/
	//unsigned char ucSettleAuth;					/*���㹦��Ȩ��*/
	unsigned char ucPREAUTHFINISHInputPin; 	/*Ԥ��Ȩ�������*/
	unsigned long   ulRefundMaxAmount;		/*����˻����*/
	unsigned char	aucPOSDialConnect[25];		/*POS��ͨ��*/
	unsigned char	aucTpdu[2];
	unsigned char	aucSwitchBoard[PARAM_SWITCHLEN];
	unsigned char	aucHostTelNumber1[PARAM_TELNUMBERLEN];/*�ն˽��׵绰*/
	unsigned char	aucHostTelNumber2[PARAM_TELNUMBERLEN];
	unsigned char	aucHostTelNumber3[PARAM_TELNUMBERLEN];
	unsigned char	aucHostTelNumber4[PARAM_TELNUMBERLEN];/*�ն˹���绰*/
	unsigned char	aucGlobalTransEnable[PARAM_GLOBALTRANSENABLELEN];//support transcation
	unsigned char	ucDispMaxLine;
	unsigned char	ucLogonModiBatch;		/*��ǩ���޸�����*/
	unsigned char	aucSettleFlag;			/*�Ƿ��������*/
	unsigned char  ucForgienSettleFlag;	/*�⿨�Ƿ����ɹ���־*/
	unsigned char	ucInternalSettleFlag;		/*�ڿ��Ƿ����ɹ���־*/
	unsigned char ucFunctStep;			/*�����ж�λ��*/
	unsigned char	aucConfig[PARAM_CONFIGLEN];
	unsigned char prnflag;
	unsigned char ucPRNPAPER;
	unsigned char Pinpad_flag;
	unsigned char Pinpadflag;
	unsigned char BackLightFlag;
	unsigned char	prnflagNew;
	unsigned char	prnflagOld;
	unsigned char	prnflagZD;
	unsigned char   linenum;
	unsigned char   font;
	unsigned char ucHeatPrintFormat;
	unsigned char ucPrintPageNum;
	unsigned char   printtimes;
	MODEM_PARAM     ModemParam;
	/* For PBOC Smart Card Process */
	unsigned char	ucPBOCAppSelectFlag;
	unsigned char	ucPBOCPSAMReader;
	unsigned char   ucFirstPKDownLoad;                 //��һ�ι�Կ����
	unsigned char   ucFirstAIDParmaDownLoad;    // ��һ�β�������
	unsigned char   ucDefaultTransParam;             //ȱʡ��������
	enum ENCRYPTTYPE     ENCRYPTTYPEParam;   //���ն˼��ܷ�ʽ����
	unsigned char   ucMagPreauthFinish;               //������Ԥ��Ȩ���ʵ�ֲ���
	unsigned char   aucRecvDate[5];	/*ǩ��ʱ��ͬ����ʱ���*/
	unsigned char   aucTermDate[5];
	unsigned char	APP_Project[30];
	unsigned char	SOFTWARE_Version[17];
	unsigned char	HARDWARE_Version[17];

	unsigned long	ulHostIP;
	unsigned long	ulBackIP;
	unsigned long   ulHostIPAddress1;
	unsigned long	ulHostIPAddress2;
	unsigned long	ulHostIPAddress3;
	unsigned long	ulHostIPGPRS1;
	unsigned long	ulHostIPGPRS2;
	unsigned long	ulHostIPGPRS3;

	unsigned long	ulHostIPBack1;
	unsigned long	ulHostIPBack2;
	unsigned long	ulHostIPBack3;
	unsigned long	ulHostIPMIS1;
	unsigned long	ulHostIPMIS2;
	unsigned long	ulHostIPMIS3;

	unsigned char	ucEncryptDataFlag;
	unsigned char	ucTerIPType;
	unsigned int  	uiHostPort;
	unsigned int  	uiHostBackPort;
	unsigned char	aucAPN[30];
	unsigned char ucPrint;
	unsigned char BankID;
	unsigned char   Top[Display_MAXNB];
	unsigned char ucDisAutoSettleFlag;
	unsigned char ucDisAutoTiming;
	unsigned char ucPrintCardNo;  //�Ƿ�֧�����ؿ��ű�־
	unsigned char ucPBeforeSix;		//���ط�ʽ:ǰ�Ρ��м��
	unsigned char ucCardtype;		//�������ر�־  0:�ڿ���1���⿨��2�����⿨
	unsigned char ucForeignCard;	//�⿨��־
	unsigned char ucDisAuth;        //�Ƿ�֧��Ԥ��Ȩ���ر�־ 0:��֧�֡�1��֧��

	/*���ʵ�һЩ���� ��ʾ��ֱ�*/
	ULONG_C51		ulRMBRate;
	ULONG_C51		ulVISARate;
	ULONG_C51		ulMASTERRate;
	ULONG_C51		ulJCBRate;
	ULONG_C51		ulDINERRate;
	ULONG_C51		ulAMEXRate;
	unsigned char   ucUndoOff;
	unsigned char ucCashFlag;
	unsigned char ucTMSFlag;
	unsigned char aucPrintType[3];
	unsigned char	aucReFuncation[2];
	unsigned char ucENCFlag;
	unsigned char ucKEYMODE;
	unsigned char ucDisplay;
	unsigned char ucList;
	unsigned char ucCNOREN;
	unsigned char ucQuickUndo;//F3�������ѳ���
	unsigned char ucSelIP;
	unsigned char ucSwiptEvent;
	unsigned char ucORDERFLAG;
	unsigned char ucKEYCOMM;
	unsigned char	aucHostTel1[PARAM_TELNUMBERLEN];/*�ն˽��׵绰*/
	unsigned char	aucHostTel2[PARAM_TELNUMBERLEN];/*�ն˽��׵绰*/
	unsigned long	ulHostKEYIP;
	unsigned int  	uiHostKEYPort;
	unsigned char	aucKEYAPN[30];
	unsigned char	aucKEYTpdu[5];
	unsigned char aucKEKRow[3];
	unsigned char aucKEKLine[3];
	unsigned char ucMBFEFlag;
	unsigned char ucSendOrder;
	unsigned char ucLogistics;
	unsigned char ucLoadEMVParam;
	unsigned char ucBatchErrTicket;
	unsigned char ucInforFlag;
	unsigned char ucEmptySettle;

	unsigned char ucCollectFlag;					//���ݲɼ���־
	unsigned char ucSendCollectFlag;				//�ɼ��ɹ���־
	unsigned long ulSendIPAddress;	   			//���ݲɼ�IP
	unsigned int  uiSendPort;				       //���ݲɼ��˿�
	unsigned char aucSendAPN[20];
	unsigned char ucMobileFlag;				//�ֻ�ԤԼ
	unsigned char ucInstallment;    
    unsigned char aucMerchantName_cust[9][20+1];//�ͻ�����
    unsigned char uclujiaomc;
}CONSTANTPARAM;

typedef struct
{
	unsigned char	ucRunStep;
	unsigned char	ucSuperLogonFlag;		/*������Ϊ���Խ���״̬*/
	unsigned char ucPackgeHeadEve;			/*��ͷ����Ҫ��*/
	unsigned char	ucCashierLogonFlag;
	unsigned char	ucCashierLogonIndex;
	unsigned char	aucLogonDate[PARAM_DATELEN];
	ULONG_C51 ulBatchNumber;
	ULONG_C51 ulTraceNumber;
	ULONG_C51 ulTicketNumber;
	UINT_C51 uiTotalNumber;			/*�ۼ������ܱ���*/
	UINT_C51 uiMaxTotalNb;			/*����Ľ����ܱ���*/
	unsigned char	ucSettleTime[TRANS_TIMELEN];
	unsigned char	ucSettleData[TRANS_DATELEN];
	unsigned short uiLastTransIndex;
	unsigned char	ucTicketPrintFlag;
	unsigned char ucTerminalLockFlag;
	unsigned char	ucStoreKeyRight;
	unsigned char ucDownLoadFlag;		/*���ز�����־*/
	unsigned char ucDialStartStamp;		/*�����ۼ�ʱ����*/
	unsigned long	ulDialTimeTotal;		/*�����ۼ�ʱ��*/
	unsigned long	ulDialTotalCount;		/*�����ܴ���*/
	unsigned long	ulDialConnectCnt;		/*��ͨ����*/
	unsigned long	ulDialConnectRate;		/*��ͨ��*/
	unsigned char aucIssuerID[11];
	unsigned char aucAcquereID[11];
	unsigned char ucEMVICTransFailNum;          /*��һ��IC������ʧ��*/
	unsigned short uiEMVICLogNum;                 /*EMV��IC�llog num*/
	unsigned char ucScriptInformValid;
	enum LOADPARAMTYPE euLoadParamType; /*���ز�������*/

	unsigned char aucSettleCycle[5];                  /*�ս���ʱ��*/
	unsigned char aucSettleDate[17];
	unsigned char ucResult;
	unsigned short	ucErrorExtCode;
	unsigned char ucKeyRight;

	unsigned long   ulUnTouchTransLimit;	 /*�ǽӿ����������*/
	UCHAR bReaderSupport;                        /*�Ƿ�֧����Ӷ�����*/
	UCHAR bECSupport;             	               /*�Ƿ�֧�ֵ����ֽ�*/
	unsigned char ucReaderType;              /*����������*/
	unsigned char aucMBFEBuf[TRANS_ORDERSNO];

	UINT_C51		uiSendTotalNb;
	unsigned char	uiSendIndex[TRANS_MAXSENDNB];
	unsigned char	aucSendDate[8];

}CHANGEPARAM;

/* Count information */
typedef struct
{
	UINT_C51		uiTotalNb;      			/*�ܽ��ױ���*/
	UINT_C51		uiCreditNb;					/*�ܴ��Ǳ���*/
	ULONG_C51		ulCreditAmount;				/*�ܴ��ǽ��*/
	UINT_C51		uiDebitNb;					/*�ܽ�Ǳ���*/
	ULONG_C51		ulDebitAmount;				/*�ܽ�ǽ��*/
	UINT_C51		uiAuthNb;					/*����Ȩ����*/
	ULONG_C51		ulAuthAmount;				/*����Ȩ���*/
	UINT_C51		uiPurchaseNb;				/*�����ѱ���*/
	ULONG_C51		ulPurchaseAmount;			/*�����ѽ��*/
	UINT_C51		uiAuthFinishNb;				/*����Ȩ��ɱ���*/
	ULONG_C51		ulAuthFinishAmount;			/*����Ȩ��ɽ��*/
	UINT_C51		uiRefundNb;					/*���˻�����*/
	ULONG_C51		ulRefundAmount;				/*���˻����*/
	UINT_C51		uiOfflineNb;				/*�����ܱ���*/
	ULONG_C51		ulOfflineAmount;			/*�����ܽ��*/
	UINT_C51		uiPreauthSettleNb;				/*Ԥ��Ȩ�����ܱ���*/
	ULONG_C51		ulPreauthSettleAmount;			/*Ԥ��Ȩ�����ܽ��*/
	UINT_C51		uiUndoOffSum;				/*���߳����ܱ���*/
	ULONG_C51		ulUndoOffAmount;			/*���߳����ܽ��*/
	UINT_C51		uiCashLoadNb;      			/*�ֽ�Ȧ���ܱ���*/
	ULONG_C51		uiCashLoadAmount;					/*�ֽ�Ȧ���ܽ��*/
	unsigned char  	ucSettleFlag;	           /*�Ƿ����ɹ���־*/
}TRANSTOTAL;

typedef struct
{
	unsigned int		uiTotalNb;			/*ÿ���⿨���ܱ���*/
	unsigned long		ulAmount;			/* ÿ���⿨���ܽ��*/
	unsigned long		ulTipAmount;		/*ÿ���⿨С���ܽ��*/
	unsigned long		ulTrueAmount;		/*ÿ���⿨�����*/
}TRANSDETAIL;

typedef struct
{
	unsigned short	auiTransIndex[TRANS_SAVEMAXNB];
	TRANSTOTAL		TransTotal;
	TRANSTOTAL		LastTransTotal;
	TRANSTOTAL		ForeignTransTotal;
	TRANSTOTAL		LastForeignTransTotal;
	TRANSDETAIL		TransDetail[6];
	TRANSDETAIL		LastTransDetail[6];
	ULONG_C51		ulLastBatchNumber;

	unsigned char	aucLastCashierNo[TRANS_CASHIERNOLEN];

	unsigned char	ucLasSettleTime[TRANS_TIMELEN];
	unsigned char	ucLasSettleData[TRANS_DATELEN];
	unsigned char ucLastSettleFlag;

}TRANSINFO;



typedef struct
{
	unsigned char aucSuperNo[CASH_MAXSUPERNO];
	unsigned char	aucSuperPassword[CASH_MAXSUPERPASSWDLEN];
	unsigned char ucSYSCashierExitFlag;
	unsigned char	aucSYSCashierNo[CASH_SYSCASHIERNOLEN];
	unsigned char	aucSYSCashierPass[CASH_SYSCASHIERPASSLEN];

	unsigned char	aucCashierNo[CASH_MAXCASHIER][CASH_CASHIERNOLEN];
	unsigned char	aucCashierPass[CASH_MAXCASHIER][CASH_CASHIERPASSLEN];
	unsigned char	aucSuperPassWord[7];

}SYSCASHIER;


typedef struct
{
	unsigned char	aucMSGType;
	unsigned char	aucMSGTab[MSG_MAXCHAR];
}MSG;


typedef struct
{
	unsigned char	ucType;
	unsigned char	ucErrCode;
	unsigned char	aucErrMsg[MSG_MAXCHAR];
}SINGLEERRMSG;

typedef struct
{
	unsigned char	ucType;
	unsigned char	ucErrCode;
	unsigned char	aucErrMsg[MSG_MAXCHAR];
	unsigned char	aucErrMsg_ENG[MSG_MAXCHAR];
}FILEERRMSG;

typedef struct
{
	unsigned char	ucType;
	unsigned char	ucErrCode;
	unsigned char	aucErrMsg[MSG_MAXCHAR];
	unsigned char	aucErrMsgSub[MSG_MAXCHAR];
}HOSTEERRMSG;

typedef struct
{
	unsigned char	ucExistFlag;
	unsigned short uiOffset;
	unsigned short uiLen;
}ISO8583Bit;

typedef struct
{
	ISO8583Bit Bit[ISO8583_MAXBITNUM];
	unsigned short uiBitBufLen;
	unsigned char	aucMsgID[ISO8583_MSGIDLEN];
	unsigned char	aucBitBuf[ISO8583_MAXBITBUFLEN];
	unsigned short uiCommBufLen;
	unsigned char	aucCommBuf[ISO8583_MAXCOMMBUFLEN];
}ISO8583;

typedef struct
{
	ISO8583Bit Bit[ISO8583_MAXBITNUM];
	unsigned short uiBitBufLen;

	unsigned char	aucBitBuf[ISO8583_MAXBITBUFLEN];
	unsigned short uiCommBufLen;
	unsigned char	aucCommBuf[ISO8583_MAXCOMMBUFLEN];
}ISO8583MBFE;

typedef struct
{
	ISO8583Bit		Bit[ISO8583_MAXBITNUM];
	unsigned short	uiBitBufLen;
	unsigned char		aucMsgID[ISO8583_MSGIDLEN];
	unsigned char		aucBitBuf[ISO8583_MAXBITBUFLEN];

}BACKUPISO8583;



typedef struct
{
	unsigned char	ucValid;
	unsigned char   ucRevealEvent[2];
	unsigned char	ucAuthFlag;
	unsigned char	aucAuthData[6];
	BACKUPISO8583	BackupISO8583Data;
	unsigned short  uiICTransInfoLen;
	unsigned char   aucICTransInfo[ISO8583_ICTRANSINFOLEN];

}REVERSALISO8583;

typedef struct
{
	unsigned char	ucAttr;
	unsigned short uiMaxLen;
}ISO8583BitAttr;

typedef struct
{
	unsigned char	ucValid;
	unsigned char	aucMirror[MIRROR_MAXLEN];
}MIRROR;

/* Notice:
	Don't change the order of Data&chk, some function access fixed address!!
*/
typedef struct
{
	NORMALTRANS	ReversalTrans;
	BACKUPISO8583	SaveISO8583Data;
	BACKUPISO8583	SendISO8583Data;
	REVERSALISO8583	ReversalISO8583Data;
	BACKUPISO8583   ScriptInform8583Data;
}TRANS_8583_DATA;

typedef struct
{
    unsigned char aucTempFile[8500];
    NORMALTRANS ucSTRBackupData;
}TEMPFILE_DATA;

typedef struct
{

	ISO8583BitAttr	ISO8583BitAttrTab[ISO8583_MAXBITNUM];
	TERMINALAPP	TerminalAppData;
	CONSTANTPARAM	ConstantParamData;
	CHANGEPARAM	ChangeParamData;
	SYSCASHIER		Cashier_SysCashier_Data;
	TRANSINFO		TransInfoData;
	TRANS_8583_DATA Trans_8583Data;

	NORMALTRANS		SaveTrans1;

}DATASAVEPAGE0;

typedef struct
{
	NORMALTRANS	SaveTrans;
}DATASAVEPAGE1;

#if 0
typedef struct
{

	TERMCONFIG	 		EMVConfig;
	UCHAR		 		ucTermCAPKNum;
	CAPK		 		TermCAPK[MAXTERMCAPKNUMS];
	UCHAR		 		ucTermAIDNum;
	TERMSUPPORTAPP		TermAID[MAXTERMAIDNUMS];
	UCHAR				ucIPKRevokeNum;
	IPKREVOKE			IPKRevoke[MAXIPKREVOKENUMS];
	UCHAR				ucExceptFileNum;
	EXCEPTPAN			ExceptFile[MAXEXCEPTFILENUMS];
	EMV_CONSTPARAM	ConstParam;
	TRANSREQINFO 		TransReqInfo;

}EMVDATASAVE;
#endif
/////////////////////////////////////////////////////////////////
typedef		struct
{
	UCHAR		aucTerminalID[TERMINALIDLEN];
	UCHAR		aucMerchantID[MERCHANTIDLEN];
	UCHAR		aucMerchantName[MERCHANTNAMELEN];
	UCHAR		ucLanguage;
	UCHAR		bForceOnline;
	UCHAR		bBatchCapture;
	UCHAR		bPBOC20;
	UCHAR		bSelectAccount;
	UCHAR		ucAccountType;
	UCHAR		bShowRandNum;
	UCHAR		bDDOL;
	UCHAR		bDTOL;
	UCHAR		ucTransType;
	UINT		uiTotalAmount;
	USHORT		uiTotalNums;
	UINT		uiTraceNumber;
	UCHAR		ucCommID;
	UCHAR		bPrintTicket;
	UINT		uiBatchNumber;
	UCHAR		ucFlag;
	uchar             ucReaderPort;
	UCHAR 		bDigitalDebug;
	UCHAR   		bAutoRemove;
}EMV_CONSTPARAM;
typedef struct
{
	unsigned char aucAppVer[2];
	unsigned char aucTACDefault[5];
	unsigned char aucTACOnline[5];
	unsigned char aucTACDenial[5];
	unsigned long ulFloorLimit;
	unsigned long ulThreshold;
	unsigned char ucMaxTargetPercent;
	unsigned char ucTargetPercent;
	unsigned char ucDDOLLen;
	unsigned char aucDDOL[MAXDEFAULTDOLLEN];
	unsigned char ucOnlinePINCapab;
	unsigned long ulTelECFloorLimit;  /*�ն˵����ֽ�(EC)�����޶� */
	unsigned long ulUnTouchFloorLimit;/*�ǽ��ն��ѻ�����޶�  */
	unsigned long ulUnTouchTransLimit;/*�ǽ��ն˽����޶� */
	unsigned long ulTelCVMLimit;      /*�ǽ��ն�ִ��CVM�޶� 	*/
}EMVAIDPARAM;
#if 1
#define		CONSTPARAMFILE			"ConstPara"
#define		EMVCONFIGFILE				"EMVConfig"
#define		CAPKFILE							"CAPK"
#define		TERMSUPPORTAPPFILE	"TermApps"
#define		TRANSLOG						"TransLog"
#define		EXCEPTFILE						"Except"
#define		BATCHRECORD				"BatchRec"
#define		IPKREVOKEFILE					"IPKRevoke"
#define		EMVAIDPARAMFILE			"AIDParam"
#define		ERRICCTRANS					"ErrICC"
//#define          READERPARAMFILE				"Reader"
#endif
typedef		struct
{
	UCHAR   		ucTransType;
	UCHAR   		aucAuthRespCode[2];
	UCHAR		aucAuthCode[AUTHCODELEN];
	unsigned long ulTraceNum;
	UCHAR		ucPANLen;
	UCHAR		aucPAN[MAXPANDATALEN];
	UCHAR		aucExpiredDate[4];
	unsigned long 	ulAmount;
	UINT      		uiOtherAmount;
	UCHAR   		ucPANSeq;
	UCHAR   		aucCurrencyCode[3];
	USHORT  	uiLogBuffLen;
	UCHAR   		aucLogBuff[256];
	USHORT  	uiSendFlag;
	UCHAR		aucTransDate[TRANSDATELEN];
	UCHAR		aucTransTime[TRANSTIMELEN];
	UCHAR		aucAppCrypt[8];
	UCHAR		ucTransFlag;	//0x00 --- Online ; 0x01 --- Offline ; 0x02 --- ERR
}TRANSRECORD;


typedef enum {LEFT_ALIGN,MIDDLE_ALIGN,RIGHT_ALIGN}ALIGN;
typedef struct
{
	UCHAR		ucOptionValue;
	UCHAR		szOptionMsg[30];

}OPTION,*POPTION;

/////////////////////////////////////////////////////////////////
/* Function Description */
void OnEve_power_on(void);
unsigned char EMV_Check_file(void);


/* trans.c */
unsigned char Trans_TestSingal(void);
unsigned char Trans_EnqBalance(void);
unsigned char Trans_Process(unsigned char Flag);
unsigned char Trans_VoidCUP(void);
unsigned char TRANS_Online(void);
unsigned char Trans_OFFLineTrans(void);
unsigned char TRANS_OnlineProcess(void);
unsigned char  Trans_IssuerScriptInformSend(void);
unsigned char  Trans_SendReversalTrans(void);
unsigned char Trans_MagOffLineSettle(unsigned char ucReTryFlag);
unsigned char EMV_PackgeHeadEveProcess(unsigned char ucPackgeHeadEveID);
unsigned char TRANS_SetField55(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen);
unsigned char Trans_SendICOfflineTrans(void);
unsigned char Serv_OnlineTransSend(unsigned char ucSettleFlag,unsigned int *pucTransNum);
unsigned char Trans_SpecInfoSend(unsigned char ucSettleFlag,unsigned int *pucSpecTransNum);
unsigned char SERV_ICInformTransSend(unsigned char ucSettleFlag,unsigned int *pucTransNum);
unsigned char Trans_EMVProcessScript(void);
unsigned char Trans_SendStats(void);
unsigned char SERV_SettleBatchEnd2(unsigned int ucSentTransNum);



/* mag.c */
unsigned char Mag_Menu(unsigned char ucKey);
unsigned char MAG_InputCashierPass(void);
unsigned char Mag_AuthMenu(void);
unsigned char MAG_GetTrackInfo(unsigned char *pucTrack);
unsigned char MAG_Process(void);
void MAG_Read_LoadTransType(void);
unsigned char Trans_Query(void);
unsigned char MAG_SwipeOrHit(unsigned char *aucBuf);
unsigned char MAG_DisplaySwipeCardMode(uchar ucTransType,uchar ucMOde);
unsigned char MAG_UerICCardFirstProcess(unsigned char ucCardSelect,unsigned char ucFullBackFlag,unsigned char ucLastResult);
unsigned char MAG_ManuSelectCard(unsigned char ucTransType);
unsigned char  MAG_DispCardNo(void);
unsigned char Menu_KEYNINE(void);

/* emv.c */
UCHAR EMV_AccessTag(unsigned short uiTag,unsigned char *pucInBuf,unsigned char ucInLen,
		unsigned char *pucOutBuf,unsigned char *pucOutLen);

/* pboc.c */
unsigned char PBOC_Menu(union seve_in *EveIn);
unsigned char PBOC_EDProcess(void);
unsigned char PBOC_EPProcess(void);
unsigned char PBOC_Process(void);
unsigned char PBOC_AppSelect(void);
unsigned char PBOC_UserAppSelect(void);
unsigned char PBOC_CheckTerminalApp(void);
unsigned char PBOC_VerifyPIN(void);
unsigned char PBOC_PSAMActive(void);
unsigned char PBOC_DebitStep1(void);
unsigned char PBOC_DebitStep2(void);
unsigned char PBOC_ISOSelectFilebyAID(unsigned char ucReader,
			unsigned char *pucAID,unsigned char ucAIDLen);
unsigned char PBOC_ISOSelectFilebyFID(unsigned char ucReader,
			unsigned char *pucFID);
unsigned char PBOC_ISOReadBinarybySFI(unsigned char ucReader,
			unsigned char ucSFI,unsigned char ucOffset,unsigned char ucInLen);
unsigned char PBOC_ISOReadRecordbySFI(unsigned char ucReader,
			unsigned char ucSFI,unsigned char ucRecordNumber);
unsigned char PBOC_ISOGetBalance(unsigned char ucReader,
			unsigned char ucType);
unsigned char PBOC_ISOInitForPurchase(unsigned char ucReader,
			unsigned char ucType,unsigned char *pucInData);
unsigned char PBOC_ISODebit(unsigned char ucReader,
			unsigned char *pucDebitData,unsigned char ucLen);
unsigned char PBOC_ISOVerifyPIN(unsigned char ucReader,
			unsigned char ucInLen,unsigned char *pucPinData);
unsigned char PBOC_ISOGetResponse(unsigned char ucReader,
			unsigned char ucLen);
unsigned char PBOC_ISOGetChallenge(unsigned char ucReader);
unsigned char PBOC_ISOSAMComputeMAC1(unsigned char ucReader,
			unsigned char *pucInData);
unsigned char PBOC_ISOSAMVerifyMAC2(unsigned char ucReader,
			unsigned char *pucInData);
unsigned char PBOC_ISOCheckReturn(void);
void PBOC_SetIso7816Out(void);
unsigned short PBOC_ComputeTerminalDataChk(void);
unsigned char PBOC_CheckTerminalData(void);
void PBOC_WriteTerminalAppDataChk(unsigned char ucClrMirrorFlag);
unsigned char PBOC_PSAM_InitForDescrypt(unsigned char ucReader,unsigned char *pucInData, unsigned char ucInLen);
unsigned char PBOC_PSAM_DESCrypt(unsigned char ucReader,unsigned char *pucInData, unsigned char ucInLen);
unsigned char PBOC_PSAM_ComputeMAC1(void);
unsigned char PBOC_ISOInitForLoad(unsigned char ucReader,
			unsigned char ucType,unsigned char *pucInData);
unsigned char PBOC_TRANSOnline(void);
unsigned char PBOC_TransOnline(void);


/* sel.c */
unsigned char SEL_GetCardMsg(unsigned char ucKey);
unsigned char SEL_LogonMenu(void);
unsigned char SEL_ServiceMenu(void);
unsigned char SEL_ManagementMenu(void);
unsigned char SEL_DisplayEMVInfo(void);
unsigned char SEL_DOWNTTMSPARA(void);
unsigned char SEL_CashierMenu(void);
unsigned char SEL_SLE4442_Menu(union seve_in *EveIn);
unsigned char SEL_UnLockKeyBoard(void);
unsigned char SEL_IncDialTotalCount( void );
unsigned char SEL_IncDialConnectCnt( void );
unsigned char SEL_ClrAccumulatDial( void );
unsigned char SEL_ParamUpdata(void);
unsigned char SEL_SetMasterKey(void);
unsigned char SEL_HostCopyIP(const unsigned char ucIpTY);
unsigned char SEL_HostPorts(void);
unsigned char SEL_HostBackPorts(void);
unsigned char SEL_HostIP(const unsigned char ucTerIp);
unsigned char SEL_SelectIP(const unsigned char ucIpFlag,const unsigned char ucHostIp,unsigned char aucIpBuf[30]);
unsigned char SEL_SelectBackIP(const unsigned char ucIpFlag,const unsigned char ucIp,unsigned char aucIpBuf[30]);
/* cash.c */
unsigned char CASH_AddCashier(void);
unsigned char CASH_DelCashier(void);
unsigned char CASH_ModiCashier(void);
unsigned char CASH_PinReload(void);
unsigned char CASH_CheckCashierNo(unsigned char *pucIndex,unsigned char *pucCashierNo);
void CASH_UpdateCashierData(unsigned char ucNo,
				unsigned char *pucCashierNo,unsigned char *pucCashierPass);
unsigned char CASH_CheckCashierData(void);
void CASH_WriteCashierDataChk(unsigned char ucClrMirrorFlag);
unsigned short CASH_ComputeCashierDataChk(void);
unsigned char CASH_CashierQuery(void);
unsigned char CASH_InputAndChackManagerPass(unsigned char ucFlag);
/* sysinit*/
unsigned char Init_Process(void);
/* serv.c */
unsigned char SERV_Settle(unsigned char ucFlag);
unsigned char SERV_SettleProcess(void);
unsigned char SERV_SettleOnline(void);
unsigned char SERV_SettleBatchSend(unsigned int ucSentTransNum);
unsigned char SERV_SettleOfflineSend(unsigned short  uiSendFlag,unsigned int *pucOfflineNum);
unsigned char SERV_Reprint(void);
unsigned char SERV_ListPrint(unsigned char flag);
unsigned char SERV_TransList(unsigned char flag);

/*e_serv.c*/
unsigned char SettleProcess(void);
unsigned char SettleOnline(void);
unsigned char SettleOfflineBatchSend(void);
unsigned char SERV_SettleBatchEnd(unsigned char ucSettleFlag,unsigned int ucTransNum);
unsigned char SERV_ReprintByTrace(void);
unsigned char SERV_ReprintSettle(void);
unsigned char SERV_DispTotal(void);
unsigned char SERV_BatchSendICOfflineTrans(void);
unsigned char SERV_MAGTransBatchSend(void );

/* logon.c */
unsigned char LOGON_CashierProcess(unsigned char ucFlag);
unsigned char LOGON_Online(void);
void LOGON_WriteSuperLogonFlag(unsigned char ucFlag);
void LOGON_WriteCashierLogonFlag(unsigned char ucFlag);
void LOGON_WriteCashierLogonIndex(unsigned char ucIndex);
unsigned char LOGON_Dispatch_DateTime(unsigned char *pucDate);
//unsigned char LOGON_DispJustLogonFlag(void);


/* sav.c */
unsigned char SAV_TransSave(void);
unsigned char SAV_SavedTransIndex(unsigned char ucFlag);
unsigned short SAV_ComputeTransInfoDataChk(void);
unsigned char SAV_CheckTransInfoData(void);
void SAV_WriteTransInfoDataChk(unsigned char ucClrMirrorFlag);
unsigned char SAV_CheckTransMaxNb(void);
/* comms.c */
unsigned char COMMS_PreComm(void);
unsigned char COMMS_CheckPreComm(void);
unsigned char COMMS_TransOnline(void);
unsigned char COMMS_TransSendReceive(void);
unsigned char COMMS_GenSendReceive(void);
unsigned char COMMS_SendReceive(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned short *puiOutLen);
unsigned char COMMS_KEYDOWNReceiveByte(unsigned char *pucByte,unsigned short *puiTimeout);
unsigned char COMMS_ReceiveByte(unsigned char *pucByte,unsigned short *puiTimeout);
unsigned char COMMS_FinComm(void);
unsigned char KEY_COMMS_FinComm(void);
unsigned char COMMS_PreCommforInit(void);
unsigned char COMMS_MODEM_SendReceive(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned short *puiOutLen);
unsigned char COMMS_TCPIP_SendReceive(unsigned char *pucInData,unsigned short uiInLen,
                    unsigned char *pucOutData,unsigned short *puiOutLen);
unsigned char COMMS_TCPIP_SendReceive_Encrypt(unsigned char *pucInData,unsigned short uiInLen,
                    unsigned char *pucOutData,unsigned short *puiOutLen);
/* ISO8583.c */
void ISO8583_Clear(void);
void ISO8583_ClearBit(void);
void ISO8583_ClearBitBuf(void);
void ISO8583_ClearCommBuf(void);
unsigned char ISO8583_GetMsgID(unsigned short *piMsgID);
unsigned char ISO8583_SetMsgID(unsigned short iMsgID);
unsigned char ISO8583_GetBitValue(short iBitNo, unsigned char *pucOutData, short *piOutDataLen, short iOutBufLen);
unsigned char ISO8583_SetBitValue(short iBitNo,unsigned char *pucInData,ushort uiInLen);
unsigned char ISO8583_SetBitHexValue(short iBitNo,unsigned char *pucInData,ushort uiInLen);
unsigned char ISO8583_CheckBit(short iBitNo);
unsigned char ISO8583_RemoveBit(short iBitNo);
unsigned char ISO8583_PackData(unsigned char *pucOutData, unsigned short *piOutDataLen, short iOutLen);
unsigned char ISO8583_UnpackData(unsigned char *pucInData, short iInDataLen);
void ISO8583_DumpData(void);
unsigned short ISO8583_ComputeBitAttrTabChecksum(void);
//void ISO8583_CheckBitAttrTab(void);
unsigned char ISO8583_CheckResponseValid(void);
unsigned char ISO8583_CompareSentBit(short iBitNo);
void ISO8583_SaveISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
void ISO8583_RestoreISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
unsigned char ISO8583_SaveReversalISO8583Data(void);
unsigned short ISO8583_ComputeReversalISO8583DataChk(void);

/* util.c */
unsigned char UTIL_DisplayMenu(SELMENU *pMenu);
unsigned char UTIL_InputTelNumber(unsigned char ucLine,unsigned char ucClrFlag,
				unsigned char ucMin, unsigned char ucMax,
				unsigned char ucType,
				unsigned char *pucBuf,
				unsigned char *pucMask);
unsigned char UTIL_Input(unsigned char ucLine,unsigned char ucClrFlag,
				unsigned char ucMin, unsigned char ucMax,
				unsigned char ucType,
				unsigned char *pucBuf,
				unsigned char *pucMask);
unsigned char UTIL_InputAmount(unsigned char ucLine,
				unsigned long *pulAmount,
				unsigned long ulMinAmount, unsigned long ucMaxAmount);
unsigned char UTIL_Input_EMV_pp(unsigned char ucLine,unsigned char ucClrFlag,
				unsigned char ucMin, unsigned char ucMax,
				unsigned char ucType,
				unsigned char *pucBuf);
unsigned char UTIL_InputEncryptPIN(unsigned char * pucEnPIN,unsigned char * pucPinLen);
unsigned char UTIL_InputClearPIN(unsigned char ucPinpadFlag,unsigned char ucMax,
						unsigned char *pucOutData);
unsigned char UTIL_CalcGoldenCardSHMAC_Single(
				unsigned char *pucInData,
				unsigned short uiInLen,
				unsigned char *pucOutMAC);
unsigned char UTIL_EncryptCashierPass(
				unsigned char *pucInData,
				unsigned char *pucOutData);
unsigned char UTIL_Single_StoreNewKey(void);
unsigned char UTIL_Double_StoreNewKey(void);
void UTIL_IncreaseTraceNumber(void);
void UTIL_GetTerminalInfo(void);
void UTIL_PrintHexMsg(unsigned char *pt,unsigned short Len);
void UTIL_ClearGlobalData(void);
unsigned char UTIL_SaveDataToMirror(
				unsigned char ucPage,
				unsigned char ucClearFlag,
				unsigned char ucValidMirror,
				unsigned char *pucChkAddr,
				unsigned char *pucDest,
				unsigned short uiLen);
void UTIL_CheckSavedXDataMirror(unsigned char ucPage);
unsigned char UTIL_ClearCollectData(void);
unsigned short UTIL_ComputeMirrorChk(MIRROR *pMirror);
unsigned char UTIL_format_time_bcd_str( unsigned char *Ptd, unsigned char *Pts );
unsigned char UTIL_format_date_bcd_str( unsigned char *Ptd, unsigned char *Pts );
void UTIL_Form_Montant(unsigned char *Mnt_Fmt,unsigned long Montant,unsigned char Pos_Virgule);
unsigned char UTIL_Input_Super_Pass(void);
unsigned char UITL_Input_Merchant_Name(void);
unsigned char UITL_Delete_Reversal_Flag(void);
unsigned char UITL_Delete_Script_Flag(void);
unsigned char UTIL_Is_Trans_Empty(void);
unsigned char UITL_LocateTranceByTraceNumber(unsigned short *index,unsigned long traceNumber);
void UTIL_Beep(void);
unsigned char UTIL_displayCardNum(void);
unsigned char UTIL_Input_EMV_PIN(unsigned char ucLine,unsigned char ucClrFlag,
				unsigned char ucMin, unsigned char ucMax,
				unsigned char ucType,
				unsigned char *pucBuf,
				unsigned char *pucMask);
unsigned char UTIL_Input_pp(unsigned char ucLine,unsigned char ucClrFlag,
				unsigned char ucMin, unsigned char ucMax,
				unsigned char ucType,
				unsigned char *pucBuf);
void UTIL_delay(unsigned int timeOut);
void UTIL_display_amount(unsigned char line,unsigned long amount);
unsigned char UTIL_SetPrinter(void);
unsigned char UTIL_GetKey(unsigned char wait_sec);
unsigned char OSUTIL_Input(unsigned int uiTimeout,unsigned char ucLine,
                unsigned char ucMin,unsigned char ucMax,
                unsigned char ucType,unsigned char *pucBuf,
                char *pcMask);
void UTIL_READ_DateTimeAndFormat(unsigned char * aucOutBuf/*YYMMDDhhmm*/);
unsigned char MSG_DisplyTransType(unsigned char ucInTransType ,unsigned char Line ,unsigned char Col ,unsigned char ucClrFlag ,unsigned char ucClrLine);
unsigned char UTIL_ClearMemery(void);
void UTIL_TestDispStr(unsigned char * pucInBuf ,unsigned int uiLen ,unsigned char * DispInfo, unsigned char flag);
void UTIL_DispTransResult(TRANSRESULT	enTransResult);
void UTIL_SavEMVTransDate(void);
unsigned char UTIL_CheckTransAmount(void);
void UTIL_SavEMVTransData(void);
unsigned char UTIL_DisplyTransType(unsigned char ucInTransType ,unsigned char Line ,unsigned char Col ,unsigned char ucClrFlag ,unsigned char ucClrLine);
unsigned char UTIL_SavPrinterParamToPOS(unsigned char ucFlag);

unsigned char GetINCardName(unsigned char ucFlag ,unsigned char *temp,unsigned char *aucPrintBuf);
void UTIL_ClearEMVInterfaceData(void);
unsigned char UTIL_WaitGetKey(unsigned char wait_sec);
unsigned char UTIL_WaitGetKey_AD(unsigned char wait_sec);
unsigned char KEY_InputKey_Double_TEST(unsigned char ucKeyIndex);
unsigned char EMV_UTIL_Input_PP(void);
unsigned char UTIL_SetPinpad(void);
unsigned char UTIL_GetKey_pp(unsigned char wait_sec);
void UTIL_SetDefault_Clear(void);
unsigned char UTIL_CheckProjAndVer(void);
unsigned char UTIL_ClearTrans(void);
unsigned char UTIL_GetDateTime(unsigned char * pucDateTimeBuf );
unsigned char SERV_Print_Detail(void);
unsigned char UTIL_SetInputCardMode(unsigned char * ucReadCardMode);
void UTIL_WaitPullOutICC(void);
unsigned char UTIL_DispBalance(unsigned char * aucBuf , unsigned char ucFlag);
unsigned char UTIL_DispDateAndTime(unsigned char lin,unsigned col );
unsigned char  UTIL_GetMenu_Value(unsigned char ucTransType,unsigned short uiIndex,
		unsigned char *pucFlag,unsigned char (*pfnProc)(void),SELMENU *Menu);
unsigned char UTIL_Check_BasetSet(void);
unsigned char UTIL_AccumulatRunTime( void );
void UTIL_EMVSetTransAmount(unsigned long ulAmount);
unsigned char EMV_UTIL_Input_PP(void);
unsigned char EMV_UTIL_Input(void);
void gz_xor(unsigned char *a, unsigned char *b, char lg);
unsigned char UTIL_Backlight(void);
unsigned char UTIL_SetPrinter_Heat(void);
unsigned char UTIL_InPutMobileNum(void);

/* cfg.c */
unsigned char CFG_TermIDAndMarchID(void);
unsigned char CFG_ConstantParamBaseData(void);
unsigned char CFG_ConstantParamPBOCData(void);
unsigned char CFG_ConstantParamDateTime(void);
unsigned char CFG_ChangeParamData(void);
unsigned short CFG_ComputeConstantParamDataChecksum(void);
unsigned char CFG_CheckConstantParamData(void);
unsigned short CFG_ComputeChangeParamDataChecksum(void);
unsigned char CFG_CheckChangeParamData(void);
unsigned char CFG_ConstantParamSystInit(void);
unsigned char CFG_Smart_Download_Keys(void);
unsigned char CFG_ConstantParamPhoneNo(void);
unsigned char CFG_ConstantParamTotal(void);
unsigned char CFG_ConstantParamVersion(void);
unsigned char UTIL_AmountFormat(unsigned char * ucOutAmount ,unsigned long  ucInAmount ,unsigned char * ucDispLen);
unsigned char CFG_TermTypeConfig(void);
unsigned char CFG_SetMarchName(void);
unsigned char CFG_WriteDateTimetoPOS(unsigned char * ucInBuf);
unsigned char CFG_SwitchComm(void);
unsigned char CFG_SetCommParam(void);
unsigned char CFG_AllInOneMenu(void);
unsigned char CFG_SetParam_All(void);
/* key.c */
unsigned char KEY_DownKeyFromCard(void);
unsigned char KEY_DownKeyFromPOS(void);
unsigned char KEY_InputKeyIndex(void);
unsigned char KEY_InputMasterKey(void);
unsigned char KEY_InputKey_Single(unsigned char ucKeyIndex);
unsigned char KEY_InputKey_Double(unsigned char ucKeyIndex);
unsigned char KEY_Load_Default_Key(void);
unsigned char KEY_Load_Default_Key1(void);
unsigned char KEY_Load_Default_Key2(void);
unsigned char KEY_InputSYSTrans(void);



/* msg.c */
unsigned short MSG_ComputeMSGDataChecksum(void);
void MSG_CheckMSGData(void);
void MSG_DisplayMsg( unsigned char ucClrDisp,
					unsigned char ucLine,
					unsigned char ucCol,
					unsigned int uiIndex);
void MSG_DisplayMenuMsg( unsigned char ucClrDisp,
					unsigned char ucLine,
					unsigned char ucCol,
					unsigned int uiIndex);
void MSG_PinpadDisplayMsg( unsigned char ucClrDisp,
					unsigned char ucLine,
					unsigned char ucCol,
					unsigned int uiIndex);
unsigned char MSG_DisplayMsgKey(unsigned char ucClrDisp,
					unsigned char ucLine,
					unsigned char ucCol,
					unsigned short uiIndex,
					unsigned short uiTimeout);
unsigned char MSG_WaitKey(unsigned short uiTimeout);
unsigned char MSG_GetMsg(unsigned short uiIndex,unsigned char *pucMsg,unsigned short uiMsgLen);
unsigned char MSG_DisplayErrMsg(unsigned char ucErrorCode);
unsigned char MSG_DisplayErrMsgNotWait(unsigned char ucErrorCode);
unsigned char MSG_DisplayErrMsgNotWait_Dail(unsigned char ucErrorCode);
unsigned short MSG_ComputeErrMSGDataChk(void);
void MSG_CheckErrMSGData(void);
void MSG_WriteErrMSGDataChk(unsigned char ucClrMirrorFlag);
unsigned char MSG_DisplayEMVErrMsg(unsigned char ucErrorCode);

/* pinpad.c */
unsigned char PINPAD_0D_InitScraCode(unsigned char *pucInData);
unsigned char PINPAD_30_BackLight(unsigned char ucBackLight);
unsigned char PINPAD_42_LoadSingleKeyUseSingleKey(
					unsigned char ucKeyArray,
					unsigned char ucDecryptKeyIndex,
					unsigned char ucDestKeyIndex,
					unsigned char *pucInData);
unsigned char PINPAD_47_Encrypt8ByteSingleKey(
					unsigned char ucKeyArray,
					unsigned char ucKeyIndex,
					unsigned char *pucInData,
					unsigned char *pucOutData);
unsigned char PINPAD_47_Crypt8ByteSingleKey(
					unsigned char ucKeyArray,
					unsigned char ucKeyIndex,
					unsigned char *pucInData,
					unsigned char *pucOutData);
unsigned char PINPAD_4E_PINInput(
					unsigned char ucRow,unsigned char ucCol,
					unsigned char ucMin,unsigned char ucMax,
					unsigned char ucDispChar,
					unsigned char ucKeyArray,unsigned char ucKeyIndex,
					unsigned char *pucOutKeyNb,
					unsigned char *pucOutData);
unsigned char PP_In_Store_Key(
					unsigned char ucRow,
					unsigned char ucCol,
					unsigned char ucKeyIndex,
					unsigned char *aucBuf);
unsigned char SendReceivePinpad(void);

/* admin.c */
unsigned char ADMIN_Process(void);
unsigned char ADMIN_SendTerminalData(void);
unsigned char ADMIN_ReceivePackage(void);
unsigned char ADMIN_ProcessPackage(void);
unsigned char ADMIN_DownloadMsg(void);
unsigned char ADMIN_DownloadErrMsg(void);
unsigned char ADMIN_SendPackage(void);

/* print.c */
void PRINT_ConstantParam(void);
void PRINT_OLDData(void);
void PRINT_Ticket(unsigned char flag,unsigned char ucCopy);
void PRINT_Ticket_stylusprinter(unsigned char flag,unsigned char ucSaleCopy);
void PRINT_Ticket_HeatPrinter_1(unsigned char flag,unsigned char ucSaleCopy);
void PRINT_Ticket_HeatPrinter_2(unsigned char flag,unsigned char ucSaleCopy);
void PRINT_Ticket_HeatPrinter_3(unsigned char flag,unsigned char ucSaleCopy);
void PRINT_Ticket_HeatPrinter(unsigned char flag,unsigned char ucSaleCopy);
void PRINT_List(unsigned char flag);
unsigned char PRINT_RePrint_List(void);
unsigned char PRINT_PrintTwoMSG( unsigned char ucFontType,
					unsigned short iIndex1,
					unsigned char *pucBuf1,
					unsigned short uiBufLen1,
//					unsigned short iIndex2,
					short iIndex2,
					unsigned char *pucBuf2,					unsigned short uiBufLen2);
unsigned char PRINT_PrintMsg( unsigned char ucFontType,unsigned short uiIndex);
void PRINT_Print_Local_Date_Time(void);
unsigned char PRINT_Detail_Menu(void);
void PRINT_detail(void);
void PRINT_detail_void(void);
void PRINT_detail_ZD(void);
void PRINT_transType(unsigned char Flag);
void PRINT_SettleTotal(unsigned char flag);
void PRINT_OfflineDetial(void);
unsigned char PRINT_GetReadCardMethord(void);
void PRINT_Ticket_L_Old(unsigned char flag);
void PRINT_Ticket_L_New(unsigned char flag);
void PRINT_Ticket_L_2(unsigned char flag);
void PRINT_Ticket_S(unsigned char flag);
void PRINT_TransType_Stylusprinter(unsigned char Flag);
void PRINT_TransType_L(unsigned char Flag);
void PRINT_version(void);
void PRINT_remark(void);
void PRINT_remark_L(void);
unsigned char PRINT_GB2312_xprint(uchar * pucBuffer, uchar ucFont );
unsigned char PRINT_xprint(uchar * pucBuffer);
unsigned char PRINT_xprint_Big(uchar * pucBuffer, uchar ucFont );
unsigned char PRINT_xlinefeed(uchar ucLineNum);
unsigned char PRINT_XReverseFeed(uchar ucLineNum );
void PRINT_EveryCardDetail(unsigned char printFlag);
unsigned char Print_CheckBase(void);
void PRINT_ext_xlinereverse(uchar nbline);
unsigned char PRINT_ext_GB2312_xprint(unsigned char *pucPtr, unsigned char ucGBFont);
unsigned char PRINT_ext_xlinefeed(uchar nbline);
unsigned char PRINT_ext_xprint(uchar *buffer);
unsigned char PRINT_DFDatalist(void);

/* SEL4442*/
unsigned char SLE4442_ATR(uchar *pBuf);
unsigned char SLE4442_Read_Main_Memory(uchar *pBuf,uchar ucAddr,uchar ucNbr);
unsigned char SLE4442_Read_Security_Memory(uchar *pBuf);
unsigned char SLE4442_Update_Main_Memory(uchar Input_Data,uchar ucAddr);
unsigned char SLE4442_Update_Security_Memory(uchar Input_Data,uchar ucAddr);
unsigned char SLE4442_Compare_Verification_Data(uchar Input_Data,uchar ucAddr);
unsigned char SLE4442_ResetAddress(uchar *code_buff);
unsigned char SLE4442_End_ATR(uchar *code_buff);
unsigned char SLE4442_Enter_CMD_Mode(uchar *code_buff);
unsigned char SLE4442_Leave_CMD_Mode(uchar *code_buff);
unsigned char SLE4442_Enter_Outgoing_Mode(uchar *code_buff);
unsigned char SLE4442_Send_Command(uchar *code_buff,uchar Command_Type,uchar Addr,uchar Input_Data);
unsigned char SLE4442_ReadBits(uchar *code_buff, uchar N_Bits);
unsigned char SLE4442_NReadBits(uchar *code_buff, uchar N_Bits);
unsigned char SLE4442_Erase_And_Write(uchar *code_buff);
unsigned char SLE4442_Compare_Process(uchar *code_buff);
unsigned char SLE4442_FinishCommand(uchar *code_buff);
unsigned char SLE4442_Check_Secret(uchar *pSecret_Code);
unsigned char SLE4442_Modify(void);
unsigned char SLE4442_Write(void);
unsigned char SLE4442_Read(void);
unsigned char SLE4442_Process(void);
unsigned char Check_Card_Valid(unsigned char *key);
unsigned char Smart_Card_Trans(unsigned char *key);

/*e_pub.c*/
unsigned char SEL_SystemMenu(void);
unsigned char Pocket_Read_Binary(unsigned char ucReader,
			unsigned short ucOffset,unsigned char ucInLen);
unsigned char Pocket_Select_File(unsigned char ucReader,
			unsigned short pucAID);
unsigned char Pocket_Input_VerifyPIN(void);
unsigned char Pocket_Verify_PIN(unsigned char ucReader,
			unsigned char ucInLen,unsigned char *pucPinData);
unsigned char Pocket_Update_Binary(unsigned char ucReader,unsigned short ucOffset,
			unsigned char ucInLen,unsigned char *ucBuf);
unsigned char Wait_Read_Card(void);
/*e_logon*/
unsigned char LOGON_Process(void);
unsigned char E_LOGON_Online(void);

/*syscash.c*/
unsigned char SEL_SYSMenu(void);
unsigned char SYSCASH_ChangeSuperPasswd(void);
unsigned char SYSCASH_AddCashier(void);
unsigned char SYSCASH_DelCashier(void);
unsigned char SYSCASH_ModiCashier(void);
unsigned char SYSCASH_SuperKey(void);
unsigned char SYSCASH_SetTransType(void);
unsigned char SYSCASH_CheckCashierData(void);
void SYSCASH_WriteCashierDataChk(unsigned char ucClrMirrorFlag);

/* test.c */
void test_Disp(void);
void Disp_yinglian(void);
void Test_ErrCode(void);

/*appmain.c*/
void DispAD(unsigned char ucFalg);
void DispLogo_yinglian(void);
unsigned char FileSysAPPMain(void);
unsigned char DispDateAndTime(unsigned char lin,unsigned col );
void DispPINPAD_AD(void);
unsigned char OffLineSettle(unsigned char ucReTryFlag);

/*  Menu.c */
unsigned char Menu_EPTrans(void);
unsigned char  MenuSys_TerminalSet(void);
unsigned char  MenuSys_Init(void);
unsigned char OSUTIL_CheckIPAddrField(
                unsigned char *pucIP,
                unsigned char ucIPLen,
                unsigned char *pucIPField);
unsigned char OSUTIL_InputIPV4Addr(
                unsigned int uiTimeout,
                unsigned char ucFont,
                unsigned char ucRow,
                unsigned char *pucIPV4Addr);

unsigned char   Main_MainFunction(unsigned char ucKey);
void Main_PowerOff(void);
unsigned char Main_Logon(void);
unsigned char Main_EMVProcess(unsigned char  ucFlag);
void UTIL_PINPADDispLOGO(void);
unsigned char UITL_Input_Rate(void);
unsigned char UTIL_BANK_Process(void);
unsigned char UTIL_BANK_OTHERS(void);
unsigned char UTIL_OTHERBANK_OTHERS(void);
unsigned char UTIL_Set_BankID(void);
void UTIL_AddTraceNumber(void);

unsigned char UITL_SetAutoSettleTime(void);
void UTIL_AutoSettleOn(void);
void AutoSettle(void);


/*port.c*/
unsigned char Port_Main_Process(RECEIVETRANS *ReceiveData,unsigned char* aucOutData);
unsigned char Port_Card_trans(unsigned char *ucFlag,RECEIVETRANS *ReceiveData/*,SENDTRANS *SendTransData*/);
unsigned char Port_PackResult(RECEIVETRANS *ReceiveData,SENDTRANS *SendTransData,unsigned char ucResult);
unsigned char Port_Card_trans_SecondDeal(unsigned char *ucFlag,RECEIVETRANS *ReceiveData);
unsigned char Port_Query_trans(unsigned char *ucFlag,RECEIVETRANS *ReceiveData);
unsigned char Port_PrintTicket(void);

unsigned char LOADKEY_LoadKeyFromPOS(void);
unsigned char MSG_GetErrMsg(unsigned char ucErrorCode,unsigned char *aucBuf);

#endif
unsigned char TRANS_SetField55_Reversal(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen);
////////////////////////EC///////////
unsigned char Mag_ECMenu(void);
unsigned char MASAPP_SwipeCard(void);
unsigned char MASAPP_GetTrackInfo(unsigned char *pucTrack);
unsigned char UTIL_InputEncryptPIN_EC(unsigned char * pucEnPIN,unsigned char * pucPinLen);
////////////////////////reader hongbao
unsigned char TRANS_SetUntouchField55(unsigned char * SendBuf ,unsigned short * SendBufLen);
unsigned char SEL_DisplayEMVInfo(void);
unsigned char SEL_ReaderMenu_HongBao(void);
unsigned char SEL_ReaderMenu_Sand(void);
unsigned char SEL_TestOnlineTrans(void);
unsigned char CFG_ChangeReaderParamData(void);
UCHAR UTIL_SaveReaderCAPKFile(void);
unsigned char CFG_ChangeReaderParamData_HongBao(void);
unsigned char VerifyReaderCAPK(unsigned char ucFlag);
unsigned char EMV_DisplayReaderPKInfo(void);
UCHAR UTIL_ManualSaveReaderCAPKFile(void);
unsigned char UTIL_SetReader(void);
unsigned char MASAPP_External_Event(DRV_OUT *aucEventOutData,unsigned char InputMode,unsigned char * ucOutKey);
unsigned char MASAPP_UnTouchCard_Event(void);
unsigned char SetHBReaderAIDInfo(void);
unsigned char CFG_ChangeReaderParamData_Sand(void);
unsigned char TRANS_SetUntouchField55_Reversal(unsigned char * SendBuf ,unsigned short * SendBufLen);
/////////////////////////////reader sand///////////////////////////
#if SANDREADER
unsigned char Trans_InputOrder(void);
unsigned char MASAPP_Event_SAND(unsigned char ucDispInfoFlag , unsigned char * ucOutKey);
UCHAR UTIL_WriteReaderParamFile(EMV_CONSTPARAM * pConstParam);
UCHAR	QTransProcess(QTRANSTYPE enTransType,UINT uiAmount,UINT uiOtherAmount);
void MasApp_QSetEMVTransInfo(void);
unsigned char QMasApp_CARDPAN(unsigned char* pstrCardNo,unsigned char *pucCardNoLen);
UCHAR	QTransCapture(TRANSRESULT	enTransResult);
unsigned char QMasAPP_CheckAID(void);
unsigned char QTRANS_SetField55(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen);
unsigned char QTRANS_SetField55_Reversal(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen);
unsigned char  QMasApp_CopytEMVTransInfoToNormalTrans(void);
void SetSandReaderAIDInfo(void);
UCHAR	QCardHolderConfirmApp(PQCANDIDATELIST pCandidateList,PUCHAR pucAppIndex);
unsigned char QEMV_SetEMVConfig(void);
unsigned char EMV_GetTagFromStr(uchar *aucTagStr,uchar *aucTag);
unsigned char DIGITAL_CheckCard(void);
UCHAR WKICC(QICCIN *pQICCIn ,QICCOUT  *pQICCOut);
unsigned char DIGITAL_SendReceive(unsigned int uiTime);
unsigned char DIGITAL_SendReceive_tmp(void);
unsigned char DIGITAL_RemoveCard(uchar ucDisFlag);
void QAID_Dump(void);
#endif
unsigned char CASH_GetLogonCasherNo(unsigned char *pucIndex);
void PRINT_transType_NEW(unsigned char *paucOutData,unsigned char *paucOutDataEN);
UCHAR QEMVICC(QICCIN *pQICCIn ,QICCOUT  *pQICCOut);

unsigned char EXTrans_ProHead_Ic(struct seven *EventInfo);
unsigned char EXTrans_Process_IC(struct seven *EventInfo);

unsigned char EXTrans_ProHead_Mag(struct seven *EventInfo);

unsigned char EXTrans_Process(unsigned char Flag);

unsigned char EX_KEY_InputKey_Double(unsigned char ucKeyIndex);
//int utf8togb2312(char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen);



unsigned int OWE_RstErrInfo(int ret);
unsigned char OWE_GetIsoTrack(unsigned char *pucTrack);
unsigned char OWE_GetCardInfo(void);


void PBOC_CPU_TEST(VOID);


void PRINT_tts(void);
