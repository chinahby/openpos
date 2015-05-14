#if 0
#include <stdio.h>

#include "py_input.h"
#include "asc_table.h"

#include "py_input_config.h"

#ifdef __cplusplus
extern "C"
{
#endif
	unsigned char //Os__GB2312_display
		(
		unsigned char,
		unsigned char,
		unsigned char * pt_msg
		);
	unsigned long asc_long
		(
		unsigned char *Pts,
		unsigned char Ls
		);
	unsigned char *long_tab
		(
		unsigned char *Ptd,
		unsigned char Ld,
		unsigned long *Pts
		);
	unsigned char Os__graph_display
		(
		unsigned char,
		unsigned char,
		unsigned char * pt_msg,
		unsigned char LEN
		)reentrant;
	void Os__display
		(
		unsigned char line,
		unsigned char col,
		unsigned char *text
		);
#ifdef __cplusplus
}
#endif

typedef enum INPUT_TYPE
{
	INPUT_TYPE_PINYIN,
	INPUT_TYPE_NUMBER,
	INPUT_TYPE_UPPER_CASE,
	INPUT_TYPE_LOWER_CASE,
	INPUT_TYPE_QUWEI
}InputType;

typedef enum INPUT_CHECK
{
	INPUT_FOUND,
	INPUT_NOT_FOUND,
	INPUT_INCLUDE
}InputCheck;

typedef struct PY_INDEX
{
	const char *m_py;
	const char *m_pyTable;
}PinYinIndex;

typedef struct PY_CHAR_INDEX
{
	const PinYinIndex *m_pyIndex;
	unsigned int m_tableSize;
}PinYinCharIndex;

/*=================ȫ�ֱ���===========================*/
//��ǰ�����뷨
static InputType sg_inputType;
//���õ����뷨��־
static unsigned char sg_inputFlag;

//��һ�ΰ���
static unsigned char sg_ucPreviousKey;
//��һ��������ַ�
static unsigned char sg_ucPreviousChar;

//����ʱ��(��λ:10ms)
static unsigned int sg_uiTrueTimeOut;
//��һ�ε�ƫ����
static unsigned char sg_ucKeyTabOffset;

//ƴ�����뷨�������б�,��� (mb)
static PinYinIndex sg_indexLetterA[5];
static PinYinIndex sg_indexLetterB[16];
static PinYinIndex sg_indexLetterC[33];
static PinYinIndex sg_indexLetterD[20];
static PinYinIndex sg_indexLetterE[3];
static PinYinIndex sg_indexLetterF[9];
static PinYinIndex sg_indexLetterG[19];
static PinYinIndex sg_indexLetterH[19];
static PinYinIndex sg_indexLetterI[1];
static PinYinIndex sg_indexLetterJ[14];
static PinYinIndex sg_indexLetterK[18];
static PinYinIndex sg_indexLetterL[24];
static PinYinIndex sg_indexLetterM[19];
static PinYinIndex sg_indexLetterN[23];
static PinYinIndex sg_indexLetterO[2];
static PinYinIndex sg_indexLetterP[17];
static PinYinIndex sg_indexLetterQ[14];
static PinYinIndex sg_indexLetterR[14];
static PinYinIndex sg_indexLetterS[34];
static PinYinIndex sg_indexLetterT[19];
static PinYinIndex sg_indexLetterU[1];
static PinYinIndex sg_indexLetterV[1];
static PinYinIndex sg_indexLetterW[9];
static PinYinIndex sg_indexLetterX[14];
static PinYinIndex sg_indexLetterY[15];
static PinYinIndex sg_indexLetterZ[36];

//�������������ٲ����ٶ�
static PinYinCharIndex sg_headLetterIndex[26];

/*===================��������=============================*/

static const unsigned char MAX_SIGNED_CHAR = 0x7F;

//��λ�����뷨����
#define SI_JIAO		1

//ÿһ�������ʾ���ַ���
//�������Զ���Ϊ����!!!
#define MAX_DISPLAY	16
/*=================�ڲ���������===========================*/

//ƴ��ϵͳ��ʼ��
void PY_Initial(void);

//��ȡ��һ�����뷨
InputType PY_GetNextInputType(const InputType a_inputType);

//������뷨�Ƿ����
UCHAR PY_CheckInputType(const InputType a_inputType);

//��ȡ��һ�����õ����뷨
InputType PY_GetFirstInputType(void);

//��������ƴ��
InputCheck PY_Check(const unsigned char *a_input);

//��ȡ�����ƴ����Ӧ�������ֱ�
const unsigned char *PY_Ime(const unsigned char *a_input);

//��ʾ������ַ�
void PY_Display(unsigned char ucLine,	const unsigned char * aucInBuff);

//������ȡ����
unsigned char PY_GetKey(unsigned int a_ucWaitTime);

//��ȡ�û�ѡ���������
unsigned char PY_ChooseBuffer(const unsigned char *pucInData,unsigned char *pucOutData);

//���뵥����ĸ������
unsigned char PY_InputLetter(unsigned char *pucBuf);

//�����û������ƴ���ַ��������
InputCheck PY_InsertPinYin(unsigned char *pucInput,const unsigned char ucInputOffset,const unsigned char *pucKeyTab,unsigned char *pucKeyTabOffset);
//ƴ�����뵥��������
unsigned char PY_InputChinese(unsigned char *pucBuf);


#if SI_JIAO > 0
//��λ�����뵥��������
unsigned char PY_InputSiJiao(unsigned char *pucBuf);
#endif

/*******���뷨������
˵���μ�51py.h
*/
unsigned char PY_Main(unsigned char *aucOut,unsigned char ucLine,const unsigned char ucMin,const unsigned char ucMax,const unsigned char ucInputFlag,const unsigned short usTimeOut)
{
	unsigned char ucResult;
	unsigned int uiLen;
	//һ�������ַ����Ϊ2���ֽ�
	unsigned char aucBuf[3];
	unsigned char aucDisplayBuf[PINYIN_MAX_INPUT_NUMBER + 1];

#if PINYIN_TEST > 0
	util_Printf((char *)"Enter PinYin Main\n");
	util_Printf((char *)"aucOut = %s\n",aucOut);
	util_Printf((char *)"ucLine = %d, ucMin = %d, ucMax = %d\n", ucLine, ucMin ,ucMax);
	util_Printf((char *)"ucInputFlag = %d, usTimeOut = %d\n", ucInputFlag, usTimeOut);
#endif

	//ϵͳ��ʼ��,������,���ñ���
	if((ucInputFlag & (PY_INPUT_CHINESE
					| PY_INPUT_UPPER_CASE
					| PY_INPUT_LOWER_CASE
					| PY_INPUT_NUMBER
					| PY_INPUT_QUWEI) == 0x00)
		|| (ucMax > PINYIN_MAX_INPUT_NUMBER)
		|| (ucMin > PINYIN_MAX_INPUT_NUMBER)
		|| (ucMax < ucMin))
	{
#if PINYIN_TEST > 0
		util_Printf((char *)"PinYin Param Error\n");
#endif
		return ERR_CANCEL;
	}

	PY_Initial();

	sg_uiTrueTimeOut = usTimeOut * ONESECOND;
	sg_inputFlag = ucInputFlag;
	sg_inputType = PY_GetFirstInputType();

	memset(aucDisplayBuf, 0, sizeof(aucDisplayBuf));
	memset(aucBuf, 0, sizeof(aucBuf));

	uiLen = strlen((const char *)aucOut);
	if(uiLen > 0)
	{
		if(uiLen > PINYIN_MAX_INPUT_NUMBER)
		{
#if PINYIN_TEST > 0
			util_Printf((char *)"aucOut is too long\n");
#endif
			uiLen = PINYIN_MAX_INPUT_NUMBER;
		}
		memcpy(aucDisplayBuf, aucOut, uiLen);
	}

#if PINYIN_TEST > 0
	util_Printf((char *)"Length of aucOut = %d\n", uiLen);
#endif

	Os__timer_start(&sg_uiTrueTimeOut);

	//���뷨��ѭ��
	while(1)
	{
		ucResult = SUCCESS;

		PY_Display(ucLine, aucDisplayBuf);

		memset(aucBuf, 0x00, sizeof(aucBuf));

		switch(sg_inputType)
		{
			case INPUT_TYPE_PINYIN:
				//�û�ͨ������ƴ�����ȡһ�������ַ�
				ucResult = PY_InputChinese(aucBuf);
				break;
			case INPUT_TYPE_LOWER_CASE:
			case INPUT_TYPE_UPPER_CASE:
			case INPUT_TYPE_NUMBER:
				//�û�����һ�����ֻ���ĸ
				ucResult = PY_InputLetter(aucBuf);
				util_Printf("*****6***[%02x]*\n",ucResult);
				break;
				#if SI_JIAO > 0
			case INPUT_TYPE_QUWEI:
				ucResult = PY_InputSiJiao(aucBuf);
				break;
				#endif
			default:
				//�߼���Ӧ�ò����ڴ��ж�
				#if PINYIN_TEST > 0
				util_Printf((char *)"/**************************/\n");
				util_Printf((char *)"PinYin InputType Fatal Error\n");
				util_Printf((char *)"/**************************/\n");
				#endif
				sg_inputType = PY_GetFirstInputType();
				break;
		}

		//��ʱ�ж�
		if(sg_uiTrueTimeOut == 0)
		{
			//����Ӧ�ó���֮ǰһ��Ҫֹͣ��ʱ��(��ͬ)!!!

			util_Printf((char *)"\n PinYin Warning: Timeout\n");

			Os__timer_stop(&sg_uiTrueTimeOut);
			return ERR_CANCEL;
		}

		uiLen = strlen((const char  *)aucDisplayBuf);
		//�����ķ����ж�
		switch(ucResult)
		{
			case SUCCESS:
				if((uiLen >= ucMin) && (uiLen <= ucMax))
				{
					strcpy((char *)aucOut,(const char*)aucDisplayBuf);
					aucOut[uiLen]= '\0';
					util_Printf((char *)"\n PinYin Out Success: length of aucOut = %d\n", uiLen);
					util_Printf((char *)"\n aucOut = %s\n", aucOut);

					Os__timer_stop(&sg_uiTrueTimeOut);
					return SUCCESS;
				}
				else
				{
					if(uiLen < ucMin)
					{
						util_Printf((char *)"\n PinYin Warning: Input length short\n");
					}
					else
					{
						util_Printf((char *)"\n PinYin Warning: Input length long\n");
					}
					Os__beep();
				}
				break;
			case KEY_ENTER:
				//�������ַ�������û�������ַ�
				if((uiLen + strlen((char *)aucBuf)) <= ucMax)
				{
					if(strlen((char *)aucBuf) != 0)
					{
						util_Printf((char *)"\n Insert char = %s\n", aucBuf);
					}

					strcat((char *)aucDisplayBuf, (char *)aucBuf);
				}
				else
				{

					util_Printf((char *)"\n PinYin Warning: Input length long\n");
					if((uiLen + strlen((char *)aucBuf)) > ucMax)
						break;

					sg_ucPreviousKey = 0;
					sg_ucPreviousChar = 0;
					sg_ucKeyTabOffset = 0;
					Os__beep();
					return(SUCCESS);
				}
				break;
			case KEY_CLEAR:
				if(uiLen > 0)
				{

					util_Printf((char *)"\n Clean Display\n");

					memset(aucDisplayBuf, 0x00, sizeof(aucDisplayBuf));
				}
				else
				{

					util_Printf((char *)"\n PinYin Cancel Out\n");

					Os__timer_stop(&sg_uiTrueTimeOut);
					return ERR_CANCEL;
				}
				break;
			case KEY_BCKSP:
				if(uiLen > 0)
				{
					--uiLen;
					//�ж�Ϊ������ʱɾ��2���ַ�
					if(aucDisplayBuf[uiLen] > MAX_SIGNED_CHAR)
					{
						util_Printf((char *)"\n Delete a chinese char\n");
						aucDisplayBuf[uiLen] = 0x00;
						aucDisplayBuf[uiLen - 1] = 0x00;
					}
					else
					{
						util_Printf((char *)"\n Delete a english char\n");
						aucDisplayBuf[uiLen] = 0x00;
					}
				}
				break;
			case KEY_PAPER_FEED:
				//�л����뷨ʹ��"��ֽ"��
				sg_ucPreviousKey = 0;
				sg_ucPreviousChar = 0;
				sg_ucKeyTabOffset=0;
				sg_inputType = PY_GetNextInputType(sg_inputType);
				break;
			case KEY_00_PT:
				aucDisplayBuf[uiLen-1]=aucBuf[0];
				break;
			default:
				break;
		}
	}
}

void PY_Initial(void)
{
	unsigned char index = 0;
	sg_ucPreviousKey = 0;
	sg_ucPreviousChar = 0;
	sg_ucKeyTabOffset=0;
	//��ʼ��ƴ���б�
	/*a*/
	sg_indexLetterA[0].m_py = "";		sg_indexLetterA[0].m_pyTable = "����";
	sg_indexLetterA[1].m_py = "i";		sg_indexLetterA[1].m_pyTable = "��������������������������";
	sg_indexLetterA[2].m_py = "n";		sg_indexLetterA[2].m_pyTable = "������������������";
	sg_indexLetterA[3].m_py = "ng";		sg_indexLetterA[3].m_pyTable = "������";
	sg_indexLetterA[4].m_py = "o";		sg_indexLetterA[4].m_pyTable = "�������������°İ�";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterA;
	sg_headLetterIndex[index].m_tableSize = 5;
	++index;
	/*b*/
	sg_indexLetterB[0].m_py = "a";		sg_indexLetterB[0].m_pyTable = "�˰ͰȰǰɰṴ̋ưʰΰϰѰаӰְհ�";
	sg_indexLetterB[1].m_py = "ai";		sg_indexLetterB[1].m_pyTable = "�װٰ۰ذڰܰݰ�";
	sg_indexLetterB[2].m_py = "an";		sg_indexLetterB[2].m_pyTable = "�����߰����������";
	sg_indexLetterB[3].m_py = "ang";	sg_indexLetterB[3].m_pyTable = "������������������";
	sg_indexLetterB[4].m_py = "ao";		sg_indexLetterB[4].m_pyTable = "������������������������������������";
	sg_indexLetterB[5].m_py = "ei";		sg_indexLetterB[5].m_pyTable = "������������������������������";
	sg_indexLetterB[6].m_py = "en";		sg_indexLetterB[6].m_pyTable = "����������";
	sg_indexLetterB[7].m_py = "eng";	sg_indexLetterB[7].m_pyTable = "�����±ñű�";
	sg_indexLetterB[8].m_py = "i";		sg_indexLetterB[8].m_pyTable = "�ƱǱȱ˱ʱɱұرϱձӱѱݱбֱԱͱױ̱αڱܱ�";
	sg_indexLetterB[9].m_py = "ian";	sg_indexLetterB[9].m_pyTable = "�߱�ޱ���������";
	sg_indexLetterB[10].m_py = "iao";	sg_indexLetterB[10].m_pyTable = "�����";
	sg_indexLetterB[11].m_py = "ie";	sg_indexLetterB[11].m_pyTable = "�����";
	sg_indexLetterB[12].m_py = "in";	sg_indexLetterB[12].m_pyTable = "����������";
	sg_indexLetterB[13].m_py = "ing";	sg_indexLetterB[13].m_pyTable = "������������������";
	sg_indexLetterB[14].m_py = "o";		sg_indexLetterB[14].m_pyTable = "����������������������������������������";
	sg_indexLetterB[15].m_py = "u";		sg_indexLetterB[15].m_pyTable = "��������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterB;
	sg_headLetterIndex[index].m_tableSize = 16;
	++index;
	/*c*/
	sg_indexLetterC[0].m_py = "a";		sg_indexLetterC[0].m_pyTable = "��";
	sg_indexLetterC[1].m_py = "ai";		sg_indexLetterC[1].m_pyTable = "�²ŲĲƲòɲʲǲȲ˲�";
	sg_indexLetterC[2].m_py = "an";		sg_indexLetterC[2].m_pyTable = "�βͲвϲѲҲ�";
	sg_indexLetterC[3].m_py = "ang";	sg_indexLetterC[3].m_pyTable = "�ֲײԲղ�";
	sg_indexLetterC[4].m_py = "ao";		sg_indexLetterC[4].m_pyTable = "�ٲڲܲ۲�";
	sg_indexLetterC[5].m_py = "e";		sg_indexLetterC[5].m_pyTable = "���޲��";
	sg_indexLetterC[6].m_py = "eng";	sg_indexLetterC[6].m_pyTable = "�����";
	sg_indexLetterC[7].m_py = "ha";		sg_indexLetterC[7].m_pyTable = "������������ɲ";
	sg_indexLetterC[8].m_py = "hai";	sg_indexLetterC[8].m_pyTable = "����";
	sg_indexLetterC[9].m_py = "han";	sg_indexLetterC[9].m_pyTable = "�������������������";
	sg_indexLetterC[10].m_py = "hang";	sg_indexLetterC[10].m_pyTable = "������������������������";
	sg_indexLetterC[11].m_py = "hao";	sg_indexLetterC[11].m_pyTable = "��������������������";
	sg_indexLetterC[12].m_py = "he";	sg_indexLetterC[12].m_pyTable = "������������";
	sg_indexLetterC[13].m_py = "hen";	sg_indexLetterC[13].m_pyTable = "�������������³��ĳ�";
	sg_indexLetterC[14].m_py = "heng";	sg_indexLetterC[14].m_pyTable = "�Ƴųɳʳгϳǳ˳ͳ̳γȳѳҳ�";
	sg_indexLetterC[15].m_py = "hi";	sg_indexLetterC[15].m_pyTable = "�Գճڳس۳ٳֳ߳޳ݳܳ����";
	sg_indexLetterC[16].m_py = "hong";	sg_indexLetterC[16].m_pyTable = "������";
	sg_indexLetterC[17].m_py = "hou";	sg_indexLetterC[17].m_pyTable = "�������������";
	sg_indexLetterC[18].m_py = "hu";	sg_indexLetterC[18].m_pyTable = "����������������������������������";
	sg_indexLetterC[19].m_py = "huai";	sg_indexLetterC[19].m_pyTable = "��";
	sg_indexLetterC[20].m_py = "huan";	sg_indexLetterC[20].m_pyTable = "��������������";
	sg_indexLetterC[21].m_py = "huang";	sg_indexLetterC[21].m_pyTable = "����������";
	sg_indexLetterC[22].m_py = "hui";	sg_indexLetterC[22].m_pyTable = "����������";
	sg_indexLetterC[23].m_py = "hun";	sg_indexLetterC[23].m_pyTable = "��������������";
	sg_indexLetterC[24].m_py = "huo";	sg_indexLetterC[24].m_pyTable = "��";
	sg_indexLetterC[25].m_py = "i";		sg_indexLetterC[25].m_pyTable = "�ôʴĴɴȴǴŴƴ˴δ̴�";
	sg_indexLetterC[26].m_py = "ong";	sg_indexLetterC[26].m_pyTable = "�ѴӴҴдϴ�";
	sg_indexLetterC[27].m_py = "ou";	sg_indexLetterC[27].m_pyTable = "��";
	sg_indexLetterC[28].m_py = "u";		sg_indexLetterC[28].m_pyTable = "�ִٴ״�";
	sg_indexLetterC[29].m_py = "uan";	sg_indexLetterC[29].m_pyTable = "�ڴܴ�";
	sg_indexLetterC[30].m_py = "ui";	sg_indexLetterC[30].m_pyTable = "�޴ߴݴ�����";
	sg_indexLetterC[31].m_py = "un";	sg_indexLetterC[31].m_pyTable = "����";
	sg_indexLetterC[32].m_py = "uo";	sg_indexLetterC[32].m_pyTable = "�������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterC;
	sg_headLetterIndex[index].m_tableSize = 33;
	++index;
	/*d*/
	sg_indexLetterD[0].m_py = "a";		sg_indexLetterD[0].m_pyTable = "�������";
	sg_indexLetterD[1].m_py = "ai";		sg_indexLetterD[1].m_pyTable = "������������������������";
	sg_indexLetterD[2].m_py = "an";		sg_indexLetterD[2].m_pyTable = "������������������������������";
	sg_indexLetterD[3].m_py = "ang";	sg_indexLetterD[3].m_pyTable = "����������";
	sg_indexLetterD[4].m_py = "ao";		sg_indexLetterD[4].m_pyTable = "������������������������";
	sg_indexLetterD[5].m_py = "e";		sg_indexLetterD[5].m_pyTable = "�õµ�";
	sg_indexLetterD[6].m_py = "eng";	sg_indexLetterD[6].m_pyTable = "�Ƶǵŵȵ˵ʵ�";
	sg_indexLetterD[7].m_py = "i";		sg_indexLetterD[7].m_pyTable = "�͵̵εҵϵеӵѵյ׵ֵصܵ۵ݵڵ޵�";
	sg_indexLetterD[8].m_py = "ian";	sg_indexLetterD[8].m_pyTable = "���ߵ�������������";
	sg_indexLetterD[9].m_py = "iao";	sg_indexLetterD[9].m_pyTable = "�����������";
	sg_indexLetterD[10].m_py = "ie";	sg_indexLetterD[10].m_pyTable = "��������������";
	sg_indexLetterD[11].m_py = "ing";	sg_indexLetterD[11].m_pyTable = "������������������";
	sg_indexLetterD[12].m_py = "iu";	sg_indexLetterD[12].m_pyTable = "��";
	sg_indexLetterD[13].m_py = "ong";	sg_indexLetterD[13].m_pyTable = "��������������������";
	sg_indexLetterD[14].m_py = "ou";	sg_indexLetterD[14].m_pyTable = "����������������";
	sg_indexLetterD[15].m_py = "u";		sg_indexLetterD[15].m_pyTable = "�����������¶ĶöʶŶǶȶɶ�";
	sg_indexLetterD[16].m_py = "uan";	sg_indexLetterD[16].m_pyTable = "�˶̶ζ϶ж�";
	sg_indexLetterD[17].m_py = "ui";	sg_indexLetterD[17].m_pyTable = "�ѶӶԶ�";
	sg_indexLetterD[18].m_py = "un";	sg_indexLetterD[18].m_pyTable = "�ֶضն׶ܶ۶ٶ�";
	sg_indexLetterD[19].m_py = "uo";	sg_indexLetterD[19].m_pyTable = "��߶�޶��������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterD;
	sg_headLetterIndex[index].m_tableSize = 20;
	++index;
	/*e*/
	sg_indexLetterE[0].m_py = "";		sg_indexLetterE[0].m_pyTable = "����������������";
	sg_indexLetterE[1].m_py = "n";		sg_indexLetterE[1].m_pyTable = "��";
	sg_indexLetterE[2].m_py = "r";		sg_indexLetterE[2].m_pyTable = "����������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterE;
	sg_headLetterIndex[index].m_tableSize = 3;
	++index;
	/*f*/
	sg_indexLetterF[0].m_py = "a";		sg_indexLetterF[0].m_pyTable = "����������������";
	sg_indexLetterF[1].m_py = "an";		sg_indexLetterF[1].m_pyTable = "����������������������������������";
	sg_indexLetterF[2].m_py = "ang";	sg_indexLetterF[2].m_pyTable = "���������������·÷ķ�";
	sg_indexLetterF[3].m_py = "ei";		sg_indexLetterF[3].m_pyTable = "�ɷǷȷƷʷ˷̷ͷϷзη�";
	sg_indexLetterF[4].m_py = "en";		sg_indexLetterF[4].m_pyTable = "�ַԷ׷ҷշӷطڷٷ۷ݷܷ޷߷�";
	sg_indexLetterF[5].m_py = "eng";	sg_indexLetterF[5].m_pyTable = "����������������";
	sg_indexLetterF[6].m_py = "o";		sg_indexLetterF[6].m_pyTable = "��";
	sg_indexLetterF[7].m_py = "ou";		sg_indexLetterF[7].m_pyTable = "��";
	sg_indexLetterF[8].m_py = "u";		sg_indexLetterF[8].m_pyTable = "������󸥷�����������������������������������������������������������������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterF;
	sg_headLetterIndex[index].m_tableSize = 9;
	++index;
	/*g*/
	sg_indexLetterG[0].m_py = "a";		sg_indexLetterG[0].m_pyTable = "�¸�";
	sg_indexLetterG[1].m_py = "ai";		sg_indexLetterG[1].m_pyTable = "�øĸƸǸȸ�";
	sg_indexLetterG[2].m_py = "an";		sg_indexLetterG[2].m_pyTable = "�ɸʸ˸θ̸͸ѸϸҸи�";
	sg_indexLetterG[3].m_py = "ang";	sg_indexLetterG[3].m_pyTable = "�Ըոڸٸظ׸ָ۸�";
	sg_indexLetterG[4].m_py = "ao";		sg_indexLetterG[4].m_pyTable = "�޸�߸�ݸ�����";
	sg_indexLetterG[5].m_py = "e";		sg_indexLetterG[5].m_pyTable = "����������������������";
	sg_indexLetterG[6].m_py = "ei";		sg_indexLetterG[6].m_pyTable = "��";
	sg_indexLetterG[7].m_py = "en";		sg_indexLetterG[7].m_pyTable = "����";
	sg_indexLetterG[8].m_py = "eng";	sg_indexLetterG[8].m_pyTable = "��������������";
	sg_indexLetterG[9].m_py = "ong";	sg_indexLetterG[9].m_pyTable = "������������������������������";
	sg_indexLetterG[10].m_py = "ou";	sg_indexLetterG[10].m_pyTable = "������������������";
	sg_indexLetterG[11].m_py = "u";		sg_indexLetterG[11].m_pyTable = "�����ù¹��������ŹȹɹǹƹĹ̹ʹ˹�";
	sg_indexLetterG[12].m_py = "ua";	sg_indexLetterG[12].m_pyTable = "�Ϲιйѹҹ�";
	sg_indexLetterG[13].m_py = "uai";	sg_indexLetterG[13].m_pyTable = "�Թչ�";
	sg_indexLetterG[14].m_py = "uan";	sg_indexLetterG[14].m_pyTable = "�ع۹ٹڹ׹ݹܹ�߹��";
	sg_indexLetterG[15].m_py = "uang";	sg_indexLetterG[15].m_pyTable = "����";
	sg_indexLetterG[16].m_py = "ui";	sg_indexLetterG[16].m_pyTable = "������������������";
	sg_indexLetterG[17].m_py = "un";	sg_indexLetterG[17].m_pyTable = "������";
	sg_indexLetterG[18].m_py = "uo";	sg_indexLetterG[18].m_pyTable = "������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterG;
	sg_headLetterIndex[index].m_tableSize = 19;
	++index;
	/*h*/
	sg_indexLetterH[0].m_py = "a";		sg_indexLetterH[0].m_pyTable = "���";
	sg_indexLetterH[1].m_py = "ai";		sg_indexLetterH[1].m_pyTable = "��������������";
	sg_indexLetterH[2].m_py = "an";		sg_indexLetterH[2].m_pyTable = "��������������������������������������";
	sg_indexLetterH[3].m_py = "ang";	sg_indexLetterH[3].m_pyTable = "������";
	sg_indexLetterH[4].m_py = "ao";		sg_indexLetterH[4].m_pyTable = "���������úºźƺ�";
	sg_indexLetterH[5].m_py = "e";		sg_indexLetterH[5].m_pyTable = "�ǺȺ̺ϺκͺӺҺ˺ɺԺкʺغֺպ�";
	sg_indexLetterH[6].m_py = "ei";		sg_indexLetterH[6].m_pyTable = "�ں�";
	sg_indexLetterH[7].m_py = "en";		sg_indexLetterH[7].m_pyTable = "�ۺܺݺ�";
	sg_indexLetterH[8].m_py = "eng";	sg_indexLetterH[8].m_pyTable = "��ߺ���";
	sg_indexLetterH[9].m_py = "ong";	sg_indexLetterH[9].m_pyTable = "����������";
	sg_indexLetterH[10].m_py = "ou";	sg_indexLetterH[10].m_pyTable = "��������";
	sg_indexLetterH[11].m_py = "u";		sg_indexLetterH[11].m_pyTable = "������������������������������������";
	sg_indexLetterH[12].m_py = "ua";	sg_indexLetterH[12].m_pyTable = "������������������";
	sg_indexLetterH[13].m_py = "uai";	sg_indexLetterH[13].m_pyTable = "����������";
	sg_indexLetterH[14].m_py = "uan";	sg_indexLetterH[14].m_pyTable = "�����������û»�������������";
	sg_indexLetterH[15].m_py = "uang";	sg_indexLetterH[15].m_pyTable = "�ĻŻʻ˻ƻ̻ͻȻǻɻлλѻ�";
	sg_indexLetterH[16].m_py = "ui";	sg_indexLetterH[16].m_pyTable = "�һֻӻԻջػ׻ڻܻ������߻޻�ݻٻ�";
	sg_indexLetterH[17].m_py = "un";	sg_indexLetterH[17].m_pyTable = "�������";
	sg_indexLetterH[18].m_py = "uo";	sg_indexLetterH[18].m_pyTable = "�������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterH;
	sg_headLetterIndex[index].m_tableSize = 19;
	++index;
	/*i*/
	sg_indexLetterI[0].m_py = "";		sg_indexLetterI[0].m_pyTable = "";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterI;
	sg_headLetterIndex[index].m_tableSize = 0;
	++index;
	/*j*/
	sg_indexLetterJ[0].m_py = "i";		sg_indexLetterJ[0].m_pyTable = "���������������������������������������������������������������������ƼǼ��ͼ˼ɼ��ʼ����ȼü̼żļ¼�������";
	sg_indexLetterJ[1].m_py = "ia";		sg_indexLetterJ[1].m_pyTable = "�ӼмѼϼҼμԼռ׼ּؼۼݼܼټ޼�Ю";
	sg_indexLetterJ[2].m_py = "ian";	sg_indexLetterJ[2].m_pyTable = "����߼����������������������������������������������������";
	sg_indexLetterJ[3].m_py = "iang";	sg_indexLetterJ[3].m_pyTable = "��������������������������";
	sg_indexLetterJ[4].m_py = "iao";	sg_indexLetterJ[4].m_pyTable = "���������������������ǽƽʽȽýŽ½��˽ɽнνϽ̽ѽ;���";
	sg_indexLetterJ[5].m_py = "ie";		sg_indexLetterJ[5].m_pyTable = "�׽Խӽսҽֽڽٽܽ��ݽ޽ؽ߽����������";
	sg_indexLetterJ[6].m_py = "in";		sg_indexLetterJ[6].m_pyTable = "���������������������������������";
	sg_indexLetterJ[7].m_py = "ing";	sg_indexLetterJ[7].m_pyTable = "��������������������������������������������������";
	sg_indexLetterJ[8].m_py = "iong";	sg_indexLetterJ[8].m_pyTable = "����";
	sg_indexLetterJ[9].m_py = "iu";		sg_indexLetterJ[9].m_pyTable = "�������žþľ��¾ƾɾʾ̾ξǾȾ;�";
	sg_indexLetterJ[10].m_py = "u";		sg_indexLetterJ[10].m_pyTable = "�ӾоѾԾҾϾֽ۾վ׾ھپؾ�޾ܾ߾����ݾ��۾�";
	sg_indexLetterJ[11].m_py = "uan";	sg_indexLetterJ[11].m_pyTable = "��������";
	sg_indexLetterJ[12].m_py = "ue";	sg_indexLetterJ[12].m_pyTable = "��������������";
	sg_indexLetterJ[13].m_py = "un";	sg_indexLetterJ[13].m_pyTable = "����������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterJ;
	sg_headLetterIndex[index].m_tableSize = 14;
	++index;
	/*k*/
	sg_indexLetterK[0].m_py = "a";		sg_indexLetterK[0].m_pyTable = "������";
	sg_indexLetterK[1].m_py = "ai";		sg_indexLetterK[1].m_pyTable = "����������";
	sg_indexLetterK[2].m_py = "an";		sg_indexLetterK[2].m_pyTable = "��������������";
	sg_indexLetterK[3].m_py = "ang";	sg_indexLetterK[3].m_pyTable = "��������������";
	sg_indexLetterK[4].m_py = "ao";		sg_indexLetterK[4].m_pyTable = "��������";
	sg_indexLetterK[5].m_py = "e";		sg_indexLetterK[5].m_pyTable = "�����¿ƿÿſĿǿȿɿʿ˿̿Ϳ�";
	sg_indexLetterK[6].m_py = "en";		sg_indexLetterK[6].m_pyTable = "�Ͽѿҿ�";
	sg_indexLetterK[7].m_py = "eng";	sg_indexLetterK[7].m_pyTable = "�Կ�";
	sg_indexLetterK[8].m_py = "ong";	sg_indexLetterK[8].m_pyTable = "�տ׿ֿ�";
	sg_indexLetterK[9].m_py = "ou";		sg_indexLetterK[9].m_pyTable = "�ٿڿۿ�";
	sg_indexLetterK[10].m_py = "u";		sg_indexLetterK[10].m_pyTable = "�ݿ޿߿����";
	sg_indexLetterK[11].m_py = "ua";	sg_indexLetterK[11].m_pyTable = "������";
	sg_indexLetterK[12].m_py = "uai";	sg_indexLetterK[12].m_pyTable = "���������";
	sg_indexLetterK[13].m_py = "uan";	sg_indexLetterK[13].m_pyTable = "���";
	sg_indexLetterK[14].m_py = "uang";	sg_indexLetterK[14].m_pyTable = "�����������";
	sg_indexLetterK[15].m_py = "ui";	sg_indexLetterK[15].m_pyTable = "����������������������";
	sg_indexLetterK[16].m_py = "un";	sg_indexLetterK[16].m_pyTable = "��������";
	sg_indexLetterK[17].m_py = "uo";	sg_indexLetterK[17].m_pyTable = "��������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterK;
	sg_headLetterIndex[index].m_tableSize = 18;
	++index;
	/*l*/
	sg_indexLetterL[0].m_py = "a";		sg_indexLetterL[0].m_pyTable = "��������������";
	sg_indexLetterL[1].m_py = "ai";		sg_indexLetterL[1].m_pyTable = "������";
	sg_indexLetterL[2].m_py = "an";		sg_indexLetterL[2].m_pyTable = "������������������������������";
	sg_indexLetterL[3].m_py = "ang";	sg_indexLetterL[3].m_pyTable = "��������������";
	sg_indexLetterL[4].m_py = "ao";		sg_indexLetterL[4].m_pyTable = "������������������";
	sg_indexLetterL[5].m_py = "e";		sg_indexLetterL[5].m_pyTable = "������";
	sg_indexLetterL[6].m_py = "ei";		sg_indexLetterL[6].m_pyTable = "����������������������";
	sg_indexLetterL[7].m_py = "eng";	sg_indexLetterL[7].m_pyTable = "������";
	sg_indexLetterL[8].m_py = "i";		sg_indexLetterL[8].m_pyTable = "��������������������������������������������������������������������";
	sg_indexLetterL[9].m_py = "ian";	sg_indexLetterL[9].m_pyTable = "����������������������������";
	sg_indexLetterL[10].m_py = "iang";	sg_indexLetterL[10].m_pyTable = "������������������������";
	sg_indexLetterL[11].m_py = "iao";	sg_indexLetterL[11].m_pyTable = "������������������������";
	sg_indexLetterL[12].m_py = "ie";	sg_indexLetterL[12].m_pyTable = "����������";
	sg_indexLetterL[13].m_py = "in";	sg_indexLetterL[13].m_pyTable = "������������������������";
	sg_indexLetterL[14].m_py = "ing";	sg_indexLetterL[14].m_pyTable = "����������������������������";
	sg_indexLetterL[15].m_py = "iu";	sg_indexLetterL[15].m_pyTable = "����������������������";
	sg_indexLetterL[16].m_py = "ong";	sg_indexLetterL[16].m_pyTable = "��������¡��¤¢£";
	sg_indexLetterL[17].m_py = "ou";	sg_indexLetterL[17].m_pyTable = "¦¥§¨ª©";
	sg_indexLetterL[18].m_py = "u";		sg_indexLetterL[18].m_pyTable = "¶¬®«¯­±²°³½¼¸¹»µ·¾º´";
	sg_indexLetterL[19].m_py = "uan";	sg_indexLetterL[19].m_pyTable = "������������";
	sg_indexLetterL[20].m_py = "ue";	sg_indexLetterL[20].m_pyTable = "����";
	sg_indexLetterL[21].m_py = "un";	sg_indexLetterL[21].m_pyTable = "��������������";
	sg_indexLetterL[22].m_py = "uo";	sg_indexLetterL[22].m_pyTable = "������������������������";
	sg_indexLetterL[23].m_py = "v";		sg_indexLetterL[23].m_pyTable = "��¿������������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterL;
	sg_headLetterIndex[index].m_tableSize = 24;
	++index;
	/*m*/
	sg_indexLetterM[0].m_py = "a";		sg_indexLetterM[0].m_pyTable = "������������������";
	sg_indexLetterM[1].m_py = "ai";		sg_indexLetterM[1].m_pyTable = "������������";
	sg_indexLetterM[2].m_py = "an";		sg_indexLetterM[2].m_pyTable = "����������á������";
	sg_indexLetterM[3].m_py = "ang";	sg_indexLetterM[3].m_pyTable = "æâäãçå";
	sg_indexLetterM[4].m_py = "ao";		sg_indexLetterM[4].m_pyTable = "èëìéêîíïðóñò";
	sg_indexLetterM[5].m_py = "e";		sg_indexLetterM[5].m_pyTable = "ô";
	sg_indexLetterM[6].m_py = "ei";		sg_indexLetterM[6].m_pyTable = "ûöõü÷ýúøùÿ��þ��������";
	sg_indexLetterM[7].m_py = "en";		sg_indexLetterM[7].m_pyTable = "������";
	sg_indexLetterM[8].m_py = "eng";	sg_indexLetterM[8].m_pyTable = "����������������";
	sg_indexLetterM[9].m_py = "i";		sg_indexLetterM[9].m_pyTable = "����������������������������";
	sg_indexLetterM[10].m_py = "ian";	sg_indexLetterM[10].m_pyTable = "������������������";
	sg_indexLetterM[11].m_py = "iao";	sg_indexLetterM[11].m_pyTable = "����������������";
	sg_indexLetterM[12].m_py = "ie";	sg_indexLetterM[12].m_pyTable = "����";
	sg_indexLetterM[13].m_py = "in";	sg_indexLetterM[13].m_pyTable = "������������";
	sg_indexLetterM[14].m_py = "ing";	sg_indexLetterM[14].m_pyTable = "������������";
	sg_indexLetterM[15].m_py = "iu";	sg_indexLetterM[15].m_pyTable = "��";
	sg_indexLetterM[16].m_py = "o";		sg_indexLetterM[16].m_pyTable = "����ġģĤĦĥĢħĨĩĭİĪįĮīĬ";
	sg_indexLetterM[17].m_py = "ou";	sg_indexLetterM[17].m_pyTable = "Ĳıĳ";
	sg_indexLetterM[18].m_py = "u";		sg_indexLetterM[18].m_pyTable = "ĸĶĵķĴľĿ��ļĹĻ��Ľĺ��";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterM;
	sg_headLetterIndex[index].m_tableSize = 19;
	++index;
	/*n*/
	sg_indexLetterN[0].m_py = "a";		sg_indexLetterN[0].m_pyTable = "��������������";
	sg_indexLetterN[1].m_py = "ai";		sg_indexLetterN[1].m_pyTable = "����������";
	sg_indexLetterN[2].m_py = "an";		sg_indexLetterN[2].m_pyTable = "������";
	sg_indexLetterN[3].m_py = "ang";	sg_indexLetterN[3].m_pyTable = "��";
	sg_indexLetterN[4].m_py = "ao";		sg_indexLetterN[4].m_pyTable = "����������";
	sg_indexLetterN[5].m_py = "e";		sg_indexLetterN[5].m_pyTable = "��";
	sg_indexLetterN[6].m_py = "ei";		sg_indexLetterN[6].m_pyTable = "����";
	sg_indexLetterN[7].m_py = "en";		sg_indexLetterN[7].m_pyTable = "��";
	sg_indexLetterN[8].m_py = "eng";	sg_indexLetterN[8].m_pyTable = "��";
	sg_indexLetterN[9].m_py = "i";		sg_indexLetterN[9].m_pyTable = "����������������������";
	sg_indexLetterN[10].m_py = "ian";	sg_indexLetterN[10].m_pyTable = "��������������";
	sg_indexLetterN[11].m_py = "iang";	sg_indexLetterN[11].m_pyTable = "����";
	sg_indexLetterN[12].m_py = "iao";	sg_indexLetterN[12].m_pyTable = "����";
	sg_indexLetterN[13].m_py = "ie";	sg_indexLetterN[13].m_pyTable = "��������������";
	sg_indexLetterN[14].m_py = "in";	sg_indexLetterN[14].m_pyTable = "��";
	sg_indexLetterN[15].m_py = "ing";	sg_indexLetterN[15].m_pyTable = "��š������Ţ";
	sg_indexLetterN[16].m_py = "iu";	sg_indexLetterN[16].m_pyTable = "ţŤŦť";
	sg_indexLetterN[17].m_py = "ong";	sg_indexLetterN[17].m_pyTable = "ũŨŧŪ";
	sg_indexLetterN[18].m_py = "u";		sg_indexLetterN[18].m_pyTable = "ūŬŭ";
	sg_indexLetterN[19].m_py = "uan";	sg_indexLetterN[19].m_pyTable = "ů";
	sg_indexLetterN[20].m_py = "ue";	sg_indexLetterN[20].m_pyTable = "űŰ";
	sg_indexLetterN[21].m_py = "uo";	sg_indexLetterN[21].m_pyTable = "ŲŵųŴ";
	sg_indexLetterN[22].m_py = "v";		sg_indexLetterN[22].m_pyTable = "Ů";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterN;
	sg_headLetterIndex[index].m_tableSize = 23;
	++index;
	/*o*/
	sg_indexLetterO[0].m_py = "";		sg_indexLetterO[0].m_pyTable = "Ŷ";
	sg_indexLetterO[1].m_py = "u";		sg_indexLetterO[1].m_pyTable = "ŷŹŸŻżźŽ";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterO;
	sg_headLetterIndex[index].m_tableSize = 2;
	++index;
	/*p*/
	sg_indexLetterP[0].m_py = "a";		sg_indexLetterP[0].m_pyTable = "ſž����������";
	sg_indexLetterP[1].m_py = "ai";		sg_indexLetterP[1].m_pyTable = "������������";
	sg_indexLetterP[2].m_py = "an";		sg_indexLetterP[2].m_pyTable = "����������������";
	sg_indexLetterP[3].m_py = "ang";	sg_indexLetterP[3].m_pyTable = "����������";
	sg_indexLetterP[4].m_py = "ao";		sg_indexLetterP[4].m_pyTable = "��������������";
	sg_indexLetterP[5].m_py = "ei";		sg_indexLetterP[5].m_pyTable = "������������������";
	sg_indexLetterP[6].m_py = "en";		sg_indexLetterP[6].m_pyTable = "����";
	sg_indexLetterP[7].m_py = "eng";	sg_indexLetterP[7].m_pyTable = "����������������������������";
	sg_indexLetterP[8].m_py = "i";		sg_indexLetterP[8].m_pyTable = "��������������Ƥ��ƣơ��ƢƥƦƨƧƩ";
	sg_indexLetterP[9].m_py = "ian";	sg_indexLetterP[9].m_pyTable = "Ƭƫƪƭ";
	sg_indexLetterP[10].m_py = "iao";	sg_indexLetterP[10].m_pyTable = "ƯƮưƱ";
	sg_indexLetterP[11].m_py = "ie";	sg_indexLetterP[11].m_pyTable = "ƲƳ";
	sg_indexLetterP[12].m_py = "in";	sg_indexLetterP[12].m_pyTable = "ƴƶƵƷƸ";
	sg_indexLetterP[13].m_py = "ing";	sg_indexLetterP[13].m_pyTable = "ƹƽ��ƾƺƻ��ƿƼ";
	sg_indexLetterP[14].m_py = "o";		sg_indexLetterP[14].m_pyTable = "����������������";
	sg_indexLetterP[15].m_py = "ou";	sg_indexLetterP[15].m_pyTable = "��";
	sg_indexLetterP[16].m_py = "u";		sg_indexLetterP[16].m_pyTable = "������������������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterP;
	sg_headLetterIndex[index].m_tableSize = 17;
	++index;
	/*q*/
	sg_indexLetterQ[0].m_py = "i";		sg_indexLetterQ[0].m_pyTable = "������������������������������������������������������������������������";
	sg_indexLetterQ[1].m_py = "ia";		sg_indexLetterQ[1].m_pyTable = "��ǡǢ";
	sg_indexLetterQ[2].m_py = "ian";	sg_indexLetterQ[2].m_pyTable = "ǧǪǤǨǥǣǦǫǩǰǮǯǬǱǭǳǲǴǷǵǶǸ";
	sg_indexLetterQ[3].m_py = "iang";	sg_indexLetterQ[3].m_pyTable = "ǺǼǹǻǿǽǾ��";
	sg_indexLetterQ[4].m_py = "iao";	sg_indexLetterQ[4].m_pyTable = "������������������������������";
	sg_indexLetterQ[5].m_py = "ie";		sg_indexLetterQ[5].m_pyTable = "����������";
	sg_indexLetterQ[6].m_py = "in";		sg_indexLetterQ[6].m_pyTable = "����������������������";
	sg_indexLetterQ[7].m_py = "ing";	sg_indexLetterQ[7].m_pyTable = "��������������������������";
	sg_indexLetterQ[8].m_py = "iong";	sg_indexLetterQ[8].m_pyTable = "����";
	sg_indexLetterQ[9].m_py = "iu";		sg_indexLetterQ[9].m_pyTable = "����������������";
	sg_indexLetterQ[10].m_py = "u";		sg_indexLetterQ[10].m_pyTable = "����������������ȡȢȣȥȤ";
	sg_indexLetterQ[11].m_py = "uan";	sg_indexLetterQ[11].m_pyTable = "ȦȫȨȪȭȬȩȧȮȰȯ";
	sg_indexLetterQ[12].m_py = "ue";	sg_indexLetterQ[12].m_pyTable = "Ȳȱȳȴȸȷȵȶ";
	sg_indexLetterQ[13].m_py = "un";	sg_indexLetterQ[13].m_pyTable = "ȹȺ";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterQ;
	sg_headLetterIndex[index].m_tableSize = 14;
	++index;
	/*r*/
	sg_indexLetterR[0].m_py = "an";		sg_indexLetterR[0].m_pyTable = "ȻȼȽȾ";
	sg_indexLetterR[1].m_py = "ang";	sg_indexLetterR[1].m_pyTable = "ȿ��������";
	sg_indexLetterR[2].m_py = "ao";		sg_indexLetterR[2].m_pyTable = "������";
	sg_indexLetterR[3].m_py = "e";		sg_indexLetterR[3].m_pyTable = "����";
	sg_indexLetterR[4].m_py = "en";		sg_indexLetterR[4].m_pyTable = "��������������������";
	sg_indexLetterR[5].m_py = "eng";	sg_indexLetterR[5].m_pyTable = "����";
	sg_indexLetterR[6].m_py = "i";		sg_indexLetterR[6].m_pyTable = "��";
	sg_indexLetterR[7].m_py = "ong";	sg_indexLetterR[7].m_pyTable = "��������������������";
	sg_indexLetterR[8].m_py = "ou";		sg_indexLetterR[8].m_pyTable = "������";
	sg_indexLetterR[9].m_py = "u";		sg_indexLetterR[9].m_pyTable = "��������������������";
	sg_indexLetterR[10].m_py = "uan";	sg_indexLetterR[10].m_pyTable = "����";
	sg_indexLetterR[11].m_py = "ui";	sg_indexLetterR[11].m_pyTable = "������";
	sg_indexLetterR[12].m_py = "un";	sg_indexLetterR[12].m_pyTable = "����";
	sg_indexLetterR[13].m_py = "uo";	sg_indexLetterR[13].m_pyTable = "����";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterR;
	sg_headLetterIndex[index].m_tableSize = 14;
	++index;
	/*s*/
	sg_indexLetterS[0].m_py = "a";		sg_indexLetterS[0].m_pyTable = "������";
	sg_indexLetterS[1].m_py = "ai";		sg_indexLetterS[1].m_pyTable = "��������";
	sg_indexLetterS[2].m_py = "an";		sg_indexLetterS[2].m_pyTable = "����ɡɢ";
	sg_indexLetterS[3].m_py = "ang";	sg_indexLetterS[3].m_pyTable = "ɣɤɥ";
	sg_indexLetterS[4].m_py = "ao";		sg_indexLetterS[4].m_pyTable = "ɦɧɨɩ";
	sg_indexLetterS[5].m_py = "e";		sg_indexLetterS[5].m_pyTable = "ɫɬɪ";
	sg_indexLetterS[6].m_py = "en";		sg_indexLetterS[6].m_pyTable = "ɭ";
	sg_indexLetterS[7].m_py = "eng";	sg_indexLetterS[7].m_pyTable = "ɮ";
	sg_indexLetterS[8].m_py = "ha";		sg_indexLetterS[8].m_pyTable = "ɱɳɴɰɯɵɶɷ��";
	sg_indexLetterS[9].m_py = "hai";	sg_indexLetterS[9].m_pyTable = "ɸɹ";
	sg_indexLetterS[10].m_py = "han";	sg_indexLetterS[10].m_pyTable = "ɽɾɼ��ɺɿ������ɻ������������դ";
	sg_indexLetterS[11].m_py = "hang";	sg_indexLetterS[11].m_pyTable = "����������������";
	sg_indexLetterS[12].m_py = "hao";	sg_indexLetterS[12].m_pyTable = "����������������������";
	sg_indexLetterS[13].m_py = "he";	sg_indexLetterS[13].m_pyTable = "������������������������";
	sg_indexLetterS[14].m_py = "hen";	sg_indexLetterS[14].m_pyTable = "��������������������������������ʲ";
	sg_indexLetterS[15].m_py = "heng";	sg_indexLetterS[15].m_pyTable = "��������ʤ����ʡʥʢʣ";
	sg_indexLetterS[16].m_py = "hi";	sg_indexLetterS[16].m_pyTable = "��ʬʧʦʭʫʩʨʪʮʯʱʶʵʰʴʳʷʸʹʼʻʺʿ��������ʾʽ������������������������������������";
	sg_indexLetterS[17].m_py = "hou";	sg_indexLetterS[17].m_pyTable = "��������������������";
	sg_indexLetterS[18].m_py = "hu";	sg_indexLetterS[18].m_pyTable = "������������������������������������������������������ˡ����������";
	sg_indexLetterS[19].m_py = "hua";	sg_indexLetterS[19].m_pyTable = "ˢˣ";
	sg_indexLetterS[20].m_py = "huai";	sg_indexLetterS[20].m_pyTable = "˥ˤ˦˧";
	sg_indexLetterS[21].m_py = "huan";	sg_indexLetterS[21].m_pyTable = "˩˨";
	sg_indexLetterS[22].m_py = "huang";	sg_indexLetterS[22].m_pyTable = "˫˪ˬ";
	sg_indexLetterS[23].m_py = "hui";	sg_indexLetterS[23].m_pyTable = "˭ˮ˰˯";
	sg_indexLetterS[24].m_py = "hun";	sg_indexLetterS[24].m_pyTable = "˱˳˴˲";
	sg_indexLetterS[25].m_py = "huo";	sg_indexLetterS[25].m_pyTable = "˵˸˷˶";
	sg_indexLetterS[26].m_py = "i";		sg_indexLetterS[26].m_pyTable = "˿˾˽˼˹˻˺����������������";
	sg_indexLetterS[27].m_py = "ong";	sg_indexLetterS[27].m_pyTable = "����������������";
	sg_indexLetterS[28].m_py = "ou";	sg_indexLetterS[28].m_pyTable = "��������";
	sg_indexLetterS[29].m_py = "u";		sg_indexLetterS[29].m_pyTable = "����������������������";
	sg_indexLetterS[30].m_py = "uan";	sg_indexLetterS[30].m_pyTable = "������";
	sg_indexLetterS[31].m_py = "ui";	sg_indexLetterS[31].m_pyTable = "����������������������";
	sg_indexLetterS[32].m_py = "un";	sg_indexLetterS[32].m_pyTable = "������";
	sg_indexLetterS[33].m_py = "uo";	sg_indexLetterS[33].m_pyTable = "����������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterS;
	sg_headLetterIndex[index].m_tableSize = 34;
	++index;
	/*t*/
	sg_indexLetterT[0].m_py = "a";		sg_indexLetterT[0].m_pyTable = "����������̡̢̤̣";
	sg_indexLetterT[1].m_py = "ai";		sg_indexLetterT[1].m_pyTable = "̨̧̥̦̫̭̬̩̪";
	sg_indexLetterT[2].m_py = "an";		sg_indexLetterT[2].m_pyTable = "̸̵̷̶̴̮̰̯̲̱̳̹̻̺̼̾̿̽";
	sg_indexLetterT[3].m_py = "ang";	sg_indexLetterT[3].m_pyTable = "��������������������������";
	sg_indexLetterT[4].m_py = "ao";		sg_indexLetterT[4].m_pyTable = "����������������������";
	sg_indexLetterT[5].m_py = "e";		sg_indexLetterT[5].m_pyTable = "��";
	sg_indexLetterT[6].m_py = "eng";	sg_indexLetterT[6].m_pyTable = "��������";
	sg_indexLetterT[7].m_py = "i";		sg_indexLetterT[7].m_pyTable = "������������������������������";
	sg_indexLetterT[8].m_py = "ian";	sg_indexLetterT[8].m_pyTable = "����������������";
	sg_indexLetterT[9].m_py = "iao";	sg_indexLetterT[9].m_pyTable = "������������";
	sg_indexLetterT[10].m_py = "ie";	sg_indexLetterT[10].m_pyTable = "������";
	sg_indexLetterT[11].m_py = "ing";	sg_indexLetterT[11].m_pyTable = "��͡����ͤͥͣͦͧ͢";
	sg_indexLetterT[12].m_py = "ong";	sg_indexLetterT[12].m_pyTable = "ͨͬͮͩͭͯͪͫͳͱͰͲʹ";
	sg_indexLetterT[13].m_py = "ou";	sg_indexLetterT[13].m_pyTable = "͵ͷͶ͸";
	sg_indexLetterT[14].m_py = "u";		sg_indexLetterT[14].m_pyTable = "͹ͺͻͼͽͿ;��������";
	sg_indexLetterT[15].m_py = "uan";	sg_indexLetterT[15].m_pyTable = "����";
	sg_indexLetterT[16].m_py = "ui";	sg_indexLetterT[16].m_pyTable = "������������";
	sg_indexLetterT[17].m_py = "un";	sg_indexLetterT[17].m_pyTable = "��������";
	sg_indexLetterT[18].m_py = "uo";	sg_indexLetterT[18].m_pyTable = "����������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterT;
	sg_headLetterIndex[index].m_tableSize = 19;
	++index;
	/*u*/
	sg_indexLetterU[0].m_py = "";		sg_indexLetterU[0].m_pyTable = "";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterU;
	sg_headLetterIndex[index].m_tableSize = 0;
	++index;
	/*v*/
	sg_indexLetterV[0].m_py = "";		sg_indexLetterV[0].m_pyTable = "";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterV;
	sg_headLetterIndex[index].m_tableSize = 0;
	++index;
	/*w*/
	sg_indexLetterW[0].m_py = "a";		sg_indexLetterW[0].m_pyTable = "��������������";
	sg_indexLetterW[1].m_py = "ai";		sg_indexLetterW[1].m_pyTable = "����";
	sg_indexLetterW[2].m_py = "an";		sg_indexLetterW[2].m_pyTable = "����������������������������������";
	sg_indexLetterW[3].m_py = "ang";	sg_indexLetterW[3].m_pyTable = "��������������������";
	sg_indexLetterW[4].m_py = "ei";		sg_indexLetterW[4].m_pyTable = "Σ��΢ΡΪΤΧΥΦΨΩάΫΰαβγέίή��δλζηθξνιμεοκ";
	sg_indexLetterW[5].m_py = "en";		sg_indexLetterW[5].m_pyTable = "��������������������";
	sg_indexLetterW[6].m_py = "eng";	sg_indexLetterW[6].m_pyTable = "������";
	sg_indexLetterW[7].m_py = "o";		sg_indexLetterW[7].m_pyTable = "������������������";
	sg_indexLetterW[8].m_py = "u";		sg_indexLetterW[8].m_pyTable = "����������������������������������������������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterW;
	sg_headLetterIndex[index].m_tableSize = 9;
	++index;
	/*x*/
	sg_indexLetterX[0].m_py = "i";		sg_indexLetterX[0].m_pyTable = "Ϧϫ����ϣ������Ϣ��Ϥϧϩ����ϬϡϪ��Ϩ����ϥϰϯϮϱϭϴϲϷϵϸ϶";
	sg_indexLetterX[1].m_py = "ia";		sg_indexLetterX[1].m_pyTable = "ϺϹϻ��Ͽ��ϾϽϼ������";
	sg_indexLetterX[2].m_py = "ian";	sg_indexLetterX[2].m_pyTable = "ϳ����������������������������������������������������";
	sg_indexLetterX[3].m_py = "iang";	sg_indexLetterX[3].m_pyTable = "����������������������������������������";
	sg_indexLetterX[4].m_py = "iao";	sg_indexLetterX[4].m_pyTable = "����������������С��ТФ��ЧУЦХ";
	sg_indexLetterX[5].m_py = "ie";		sg_indexLetterX[5].m_pyTable = "ЩШЪЫЭавбгЯЬдйкжмелиз";
	sg_indexLetterX[6].m_py = "in";		sg_indexLetterX[6].m_pyTable = "����о����п��н����";
	sg_indexLetterX[7].m_py = "ing";	sg_indexLetterX[7].m_pyTable = "����������������������������";
	sg_indexLetterX[8].m_py = "iong";	sg_indexLetterX[8].m_pyTable = "��������������";
	sg_indexLetterX[9].m_py = "iu";		sg_indexLetterX[9].m_pyTable = "��������������������";
	sg_indexLetterX[10].m_py = "u";		sg_indexLetterX[10].m_pyTable = "��������������������������������������";
	sg_indexLetterX[11].m_py = "uan";	sg_indexLetterX[11].m_pyTable = "������������ѡѢѤѣ";
	sg_indexLetterX[12].m_py = "ue";	sg_indexLetterX[12].m_pyTable = "��ѥѦѨѧѩѪ";
	sg_indexLetterX[13].m_py = "un";	sg_indexLetterX[13].m_pyTable = "ѫѬѰѲѮѱѯѭѵѶѴѸѷѳ";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterX;
	sg_headLetterIndex[index].m_tableSize = 14;
	++index;
	/*y*/
	sg_indexLetterY[0].m_py = "a";		sg_indexLetterY[0].m_pyTable = "ѾѹѽѺѻѼ��ѿ����������������";
	sg_indexLetterY[1].m_py = "an";		sg_indexLetterY[1].m_pyTable = "������������������������������������������������������������������";
	sg_indexLetterY[2].m_py = "ang";	sg_indexLetterY[2].m_pyTable = "����������������������������������";
	sg_indexLetterY[3].m_py = "ao";		sg_indexLetterY[3].m_pyTable = "��������ҢҦҤҥҡң��ҧҨҩҪҫԿ";
	sg_indexLetterY[4].m_py = "e";		sg_indexLetterY[4].m_pyTable = "ҬҭүҮҲұҰҵҶҷҳҹҴҺҸ";
	sg_indexLetterY[5].m_py = "i";		sg_indexLetterY[5].m_pyTable = "һ����ҽ��ҿҼҾ������������������������������������������������������������������������������������������";
	sg_indexLetterY[6].m_py = "in";		sg_indexLetterY[6].m_pyTable = "������������������������������ӡ";
	sg_indexLetterY[7].m_py = "ing";	sg_indexLetterY[7].m_pyTable = "ӦӢӤӧӣӥӭӯӫӨөӪӬӮӱӰӳӲ";
	sg_indexLetterY[8].m_py = "o";		sg_indexLetterY[8].m_pyTable = "Ӵ";
	sg_indexLetterY[9].m_py = "ong";	sg_indexLetterY[9].m_pyTable = "ӶӵӸӹӺӷ��ӽӾ��ӿ��Ӽӻ��";
	sg_indexLetterY[10].m_py = "ou";	sg_indexLetterY[10].m_pyTable = "����������������������������������������";
	sg_indexLetterY[11].m_py = "u";		sg_indexLetterY[11].m_pyTable = "����������������������������������������������������Ԧ����������ԡԤ������Ԣ��ԣ������ԥ";
	sg_indexLetterY[12].m_py = "uan";	sg_indexLetterY[12].m_pyTable = "ԩԧԨԪԱ԰ԫԭԲԬԮԵԴԳԯԶԷԹԺԸ";
	sg_indexLetterY[13].m_py = "ue";	sg_indexLetterY[13].m_pyTable = "ԻԼ��������Ծ��Խ";
	sg_indexLetterY[14].m_py = "un";	sg_indexLetterY[14].m_pyTable = "������������������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterY;
	sg_headLetterIndex[index].m_tableSize = 15;
	++index;
	/*z*/
	sg_indexLetterZ[0].m_py = "a";		sg_indexLetterZ[0].m_pyTable = "������զ";
	sg_indexLetterZ[1].m_py = "ai";		sg_indexLetterZ[1].m_pyTable = "����������������";
	sg_indexLetterZ[2].m_py = "an";		sg_indexLetterZ[2].m_pyTable = "��������";
	sg_indexLetterZ[3].m_py = "ang";	sg_indexLetterZ[3].m_pyTable = "������";
	sg_indexLetterZ[4].m_py = "ao";		sg_indexLetterZ[4].m_pyTable = "����������������������������";
	sg_indexLetterZ[5].m_py = "e";		sg_indexLetterZ[5].m_pyTable = "��������";
	sg_indexLetterZ[6].m_py = "ei";		sg_indexLetterZ[6].m_pyTable = "��";
	sg_indexLetterZ[7].m_py = "en";		sg_indexLetterZ[7].m_pyTable = "��";
	sg_indexLetterZ[8].m_py = "eng";	sg_indexLetterZ[8].m_pyTable = "������";
	sg_indexLetterZ[9].m_py = "ha";		sg_indexLetterZ[9].m_pyTable = "����������բագէթըե��";
	sg_indexLetterZ[10].m_py = "hai";	sg_indexLetterZ[10].m_pyTable = "իժլ��խծկ";
	sg_indexLetterZ[11].m_py = "han";	sg_indexLetterZ[11].m_pyTable = "մձճղհնչյոշռսջվ��տպ";
	sg_indexLetterZ[12].m_py = "hang";	sg_indexLetterZ[12].m_pyTable = "��������������������������������";
	sg_indexLetterZ[13].m_py = "hao";	sg_indexLetterZ[13].m_pyTable = "��������������������צ";
	sg_indexLetterZ[14].m_py = "he";	sg_indexLetterZ[14].m_pyTable = "����������������������";
	sg_indexLetterZ[15].m_py = "hen";	sg_indexLetterZ[15].m_pyTable = "��������������������������������֡";
	sg_indexLetterZ[16].m_py = "heng";	sg_indexLetterZ[16].m_pyTable = "��������������������֤֣��֢";
	sg_indexLetterZ[17].m_py = "hi";	sg_indexLetterZ[17].m_pyTable = "ְֱֲֳִֵֶַָֹֺֻּֽ֧֥֦֪֭֮֨֯֫֬֩��־������������ֿ������������������";
	sg_indexLetterZ[18].m_py = "hong";	sg_indexLetterZ[18].m_pyTable = "����������������������";
	sg_indexLetterZ[19].m_py = "hou";	sg_indexLetterZ[19].m_pyTable = "����������������������������";
	sg_indexLetterZ[20].m_py = "hu";	sg_indexLetterZ[20].m_pyTable = "������������������������������ס��ע��פ��ף��������";
	sg_indexLetterZ[21].m_py = "hua";	sg_indexLetterZ[21].m_pyTable = "ץ";
	sg_indexLetterZ[22].m_py = "huai";	sg_indexLetterZ[22].m_pyTable = "ק";
	sg_indexLetterZ[23].m_py = "huan";	sg_indexLetterZ[23].m_pyTable = "רשת׫׭";
	sg_indexLetterZ[24].m_py = "huang";	sg_indexLetterZ[24].m_pyTable = "ױׯ׮װ׳״��ײ";
	sg_indexLetterZ[25].m_py = "hui";	sg_indexLetterZ[25].m_pyTable = "׷׵׶׹׺׸";
	sg_indexLetterZ[26].m_py = "hun";	sg_indexLetterZ[26].m_pyTable = "׻׼";
	sg_indexLetterZ[27].m_py = "huo";	sg_indexLetterZ[27].m_pyTable = "׿׾׽��������������";
	sg_indexLetterZ[28].m_py = "i";		sg_indexLetterZ[28].m_pyTable = "����������������������������";
	sg_indexLetterZ[29].m_py = "ong";	sg_indexLetterZ[29].m_pyTable = "��������������";
	sg_indexLetterZ[30].m_py = "ou";	sg_indexLetterZ[30].m_pyTable = "��������";
	sg_indexLetterZ[31].m_py = "u";		sg_indexLetterZ[31].m_pyTable = "����������������";
	sg_indexLetterZ[32].m_py = "uan";	sg_indexLetterZ[32].m_pyTable = "׬����";
	sg_indexLetterZ[33].m_py = "ui";	sg_indexLetterZ[33].m_pyTable = "��������";
	sg_indexLetterZ[34].m_py = "un";	sg_indexLetterZ[34].m_pyTable = "����";
	sg_indexLetterZ[35].m_py = "uo";	sg_indexLetterZ[35].m_pyTable = "��������������";
	sg_headLetterIndex[index].m_pyIndex = sg_indexLetterZ;
	sg_headLetterIndex[index].m_tableSize = 36;
	++index;
#if PINYIN_TEST > 0
	util_Printf((char *)"PinYin Initial Success\n");
#endif
}

/***************��ȡ��һ�����뷨*******************
����˵��:
a_inputType :��ǰ�����뷨
����ֵ˵��:��һ�����õ����뷨
***********************************************************/
InputType PY_GetNextInputType(const InputType a_inputType)
{
	InputType newInputType = a_inputType;
	do
	{
		if(newInputType == INPUT_TYPE_QUWEI)
		{
			newInputType = INPUT_TYPE_PINYIN;
		}
		else
		{
			newInputType = (InputType)(newInputType + 1);
		}
	}
	while(PY_CheckInputType(newInputType) == false);
#if PINYIN_TEST > 0
	util_Printf((char *)"Old InputType = %d\n", a_inputType);
	util_Printf((char *)"New InputType = %d\n", newInputType);
#endif
	return newInputType;
}

/*************������뷨�Ƿ����*****************
����˵��:
a_inputType :��Ҫ�������뷨
����ֵ˵��:
true :���뷨����
false :���뷨������
***********************************************************/
UCHAR PY_CheckInputType(const InputType a_inputType)
{
	UCHAR check = false;
	if(((sg_inputFlag & PY_INPUT_CHINESE) != 0)
		&& (a_inputType == INPUT_TYPE_PINYIN))
	{
		check = true;
	}
	else if(((sg_inputFlag & PY_INPUT_LOWER_CASE) != 0)
		&& (a_inputType == INPUT_TYPE_LOWER_CASE))
	{
		check = true;
	}
	else if(((sg_inputFlag & PY_INPUT_UPPER_CASE) != 0)
		&& (a_inputType == INPUT_TYPE_UPPER_CASE))
	{
		check = true;
	}
	else if(((sg_inputFlag & PY_INPUT_NUMBER) != 0)
		&& (a_inputType == INPUT_TYPE_NUMBER))
	{
		check = true;
	}
	else if(((sg_inputFlag & PY_INPUT_QUWEI) != 0)
		&& (a_inputType == INPUT_TYPE_QUWEI))
	{
		check = true;
	}
#if PINYIN_TEST > 0
	util_Printf((char *)"InputType = %d ", a_inputType);
	if(check != false)
	{
		util_Printf((char *)"can use\n", a_inputType);
	}
	else
	{
		util_Printf((char *)"can't use\n", a_inputType);
	}
#endif
	return check;
}

/**********��ȡ��һ�����õ����뷨**************
����˵��:

����ֵ˵��:��һ�����õ����뷨
***********************************************************/
InputType PY_GetFirstInputType(void)
{
	InputType firstInputType = INPUT_TYPE_PINYIN;

	for(firstInputType = INPUT_TYPE_PINYIN; PY_CheckInputType(firstInputType) == false; )
	{
		if(firstInputType == INPUT_TYPE_QUWEI)
		{
			firstInputType = INPUT_TYPE_PINYIN;
		}
		else
		{
			firstInputType = (InputType)(firstInputType + 1);
		}
	}
#if PINYIN_TEST > 0
	util_Printf((char *)"FirstInputType = %d\n", firstInputType);
#endif
	return firstInputType;
}

/***************��������ƴ��*********************
����˵��:
a_input :������ƴ���ַ���
����ֵ˵��:
INPUT_NOT_FOUND :û��ȫƥ��򲿷�ƥ���ƴ��
INPUT_FOUND :ȫƥ���ƴ��
INPUT_INCLUDE :����ƥ���ƴ��
***********************************************************/
InputCheck PY_Check(const unsigned char *a_input)
{
	const PinYinIndex *cpHZ;
	unsigned int index;
	unsigned int maxIndex;
	unsigned int inputLen = strlen((const char *)a_input);
	InputCheck result = INPUT_NOT_FOUND;

#if PINYIN_TEST > 0
	util_Printf((char *)"Check PinYin = %s\n", a_input);
#endif

    if(inputLen != 0
	&& a_input[0] >= 'a'
	&& a_input[0] <= 'z')
    {
	    cpHZ = sg_headLetterIndex[a_input[0] - 'a'].m_pyIndex;/*������ĸ����*/
		maxIndex = sg_headLetterIndex[a_input[0] - 'a'].m_tableSize;/*����ָ�����*/

		for(index = 0; index < maxIndex; ++index, ++cpHZ)/*����������*/
	 	{
	 		//��ĸ������ƥ��
			if(strncmp(cpHZ->m_py, (const char*)&a_input[1], inputLen - 1) == 0)
			{
				//��ĸ��ȫƥ��
				if(strcmp(cpHZ->m_py, (const char*)&a_input[1]) == 0)
				{
					result = INPUT_FOUND;
				}
				else
				{
					result = INPUT_INCLUDE;
				}
				break;
			}
	    }
    }
#if PINYIN_TEST > 0
	util_Printf((char *)"Check PinYin result = %d\n", result);
#endif
    return result;/*�޹�����*/
}

/*****��ȡ�����ƴ����Ӧ�������ֱ�*********
����˵��:
a_input :������ƴ���ַ���
����ֵ˵��:
NULL :û��ȫƥ���ƴ��
����:��Ӧ�������ֱ�
***********************************************************/
const unsigned char *PY_Ime(const unsigned char *a_input)
{
	const PinYinIndex *cpHZ;
	unsigned int index;
	unsigned int maxIndex;
	const unsigned char *result = NULL;

#if PINYIN_TEST > 0
	util_Printf((char *)"IME PinYin = %s\n", a_input);
#endif

	//���������ַ�����0
    if(strlen((const char *)a_input) != 0
	&& a_input[0] >= 'a'
	&& a_input[0] <= 'z')
	{
	    cpHZ = sg_headLetterIndex[a_input[0] - 'a'].m_pyIndex;/*������ĸ����*/
		maxIndex = sg_headLetterIndex[a_input[0] - 'a'].m_tableSize;/*����ָ�����*/

		for(index = 0; index < maxIndex; ++index, ++cpHZ)/*����������*/
		{
	    	if(strcmp(cpHZ->m_py,(const char*)&a_input[1]) == 0)/*��ĸ��ȫƥ��*/
	    	{
				result = (const unsigned char *)(cpHZ->m_pyTable);
	    	}
		}
	}
#if PINYIN_TEST > 0
	if(result == NULL)
	{
		util_Printf((char *)"No such pinyin\n");
	}
	else
	{
		util_Printf((char *)"PinYin Chinese Table = %s\n", result);
	}
#endif
    return result;/*�޹�����*/
}

/*****************��ʾ������ַ�*******************
����˵��:
ucLine :�ַ�����ʾ������
a_input :����ʾ�ַ���
����ֵ˵��:

***********************************************************/
void PY_Display(unsigned char ucLine,	const unsigned char * aucInBuff)
{
	unsigned int uiLen;
	unsigned char aucBuf[MAX_DISPLAY + 1];
	unsigned char ucIndex;
	unsigned char ucDisplayIndex;
	unsigned char ucGb2312Count;

	uiLen = strlen((char  *)aucInBuff);
	memset(aucBuf, 0x00, sizeof(aucBuf));

#if PINYIN_TEST > 0
	util_Printf((char *)"Input Buffer = %s\n", aucInBuff);
#endif

	if(uiLen < MAX_DISPLAY)
	{
		strcpy((char *)aucBuf, (const char *)aucInBuff);
	}
	else
	{
		//��Ӣ�Ļ����ʾ,����Ƿ��а�������ַ�
		ucGb2312Count = 0;
		for(ucIndex = 1; ucIndex < MAX_DISPLAY; ++ucIndex)
		{
			if(aucInBuff[uiLen - ucIndex] > MAX_SIGNED_CHAR)
			{
				++ucGb2312Count;
			}
		}
#if PINYIN_TEST > 0
		util_Printf((char *)"GB2312 Count = %d\n", ucGb2312Count);
#endif
		if((ucGb2312Count & 0x01) == 0x01)
		{
			ucDisplayIndex = MAX_DISPLAY - 2;
		}
		else
		{
			ucDisplayIndex = MAX_DISPLAY - 1;
		}
		strncpy((char *)aucBuf, (const char *)&aucInBuff[uiLen - ucDisplayIndex], ucDisplayIndex);
	}
	//���������ֿ���»��������⣬��ʹ�ã���Ӣ�ĵ��»��ߴ���
#if 0
	strcat((char *)aucBuf, "_");
#endif

	//Os__clr_display(ucLine);
	//Os__GB2312_display(ucLine, 0, aucBuf);
	Os__display(ucLine, strlen((const char*)aucBuf), (unsigned char *)"_");

#if PINYIN_TEST > 0
	util_Printf((char *)"Display In Screen = %s_\n", aucBuf);
#endif
}

/*****************������ȡ����*******************
����˵��:
a_ucWaitTime :�������ʱ��,��λ10����
����ֵ˵��:
NULL :�ȴ�������ʱ
����:�û�����
��ע:����ͨUTIL_GetKey������������,
            ���ڳ�ʱ�ķ���ֵ�Ͳ����ĺ���������
***********************************************************/
unsigned char PY_GetKey(unsigned int a_ucWaitTime)
{
	DRV_OUT *pdKey;

	pdKey = Os__get_key();
	do
	{
		--a_ucWaitTime;
		Os__xdelay(1);
	}
	while((pdKey->gen_status == DRV_RUNNING) && (a_ucWaitTime != 0));

	if(a_ucWaitTime == 0)
	{
		Os__abort_drv(drv_mmi);
		return NULL;
	}

#if PINYIN_TEST > 0
	util_Printf((char *)"PinYin get key = %02x\n", pdKey->xxdata[1]);
#endif

	return pdKey->xxdata[1];
}

/*****************��ʾ������ַ�*******************
����˵��:
pucInData :����������ֱ�
pucOutData :�û�ѡ���������
uiTimeout :�������ʱ��,��λ10����
����ֵ˵��:
ERR_CANCEL :�û�����ȡ��
ERR_END :���볬ʱ
SUCCESS :ѡ��ɹ�
��ע:�����ֱ���ʾ�ڵ�3��,
            ѡ��������ʾ�ڵ�4�е��ϰ���
***********************************************************/
unsigned char PY_ChooseBuffer	(const unsigned char *pucInData,unsigned char *pucOutData)
{
	unsigned char    ucBuffIndex;
	unsigned char    ucChooseIndex;
	unsigned char    ucIndex;
	unsigned char    ucKey;
	unsigned int    uiChineseSize;
	unsigned char    aucDisplayBuf[MAX_DISPLAY + 1];
	UCHAR choosed = false;
	unsigned char aucDisplayNumber[MAX_DISPLAY / 2 + 1];
	const unsigned int uiInLen = strlen((const char*)pucInData);

	//ʹ�û�ͼ���������4�е��ϰ���
	memset(aucDisplayNumber, 0, sizeof(aucDisplayNumber));
	ASC_DrawAscString(48, 0, 8, aucDisplayNumber);

	ucBuffIndex = 0;
	while(choosed == false)
	{
		//��ʾ�����ֱ�
		memset(aucDisplayBuf, 0x00, sizeof(aucDisplayBuf));
		strncpy((char *)aucDisplayBuf, (const char *)(&pucInData[ucBuffIndex]), MAX_DISPLAY);
		//Os__clr_display(2);
#if PINYIN_TEST > 0
		util_Printf((char *)"Chinese table can choose = %s\n", aucDisplayBuf);
		util_Printf((char *)"Chinese table index = %d\n", ucBuffIndex);
#endif
		//Os__GB2312_display(2, 0, aucDisplayBuf);

		//��ʾ��ѡ������
		uiChineseSize = strlen((const char *)aucDisplayBuf) / 2;
		for(ucIndex = 0; ucIndex < uiChineseSize; ++ucIndex)
		{
			aucDisplayNumber[ucIndex] = ucIndex + '1';
		}
		aucDisplayNumber[ucIndex] = 0;
		ASC_DrawAscString(48, 5, 16, aucDisplayNumber);

		//�ȴ�����
		ucKey = PY_GetKey(sg_uiTrueTimeOut);
		if(ucKey == NULL)
		{
			ucKey = KEY_CLEAR;
		}

		switch(ucKey)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			ucChooseIndex = ucKey - '1';
			if(ucChooseIndex < uiChineseSize)
			{
				memcpy(pucOutData, &aucDisplayBuf[ucChooseIndex  * 2], 2);
				pucOutData[2] = 0;
#if PINYIN_TEST > 0
				util_Printf((char *)"Choose a chinese char = %s\n", pucOutData);
				util_Printf((char *)"Chinese char index = %d\n", ucChooseIndex * 2);
#endif
				choosed = true;
			}
			break;
		case KEY_F4:
			//���·�ҳ
			if(uiInLen > ucBuffIndex + MAX_DISPLAY)
			{
				ucBuffIndex += MAX_DISPLAY;
			}
			break ;
		case KEY_F3:
			//���Ϸ�ҳ
			if(ucBuffIndex >= MAX_DISPLAY)
			{
				ucBuffIndex -= MAX_DISPLAY;
			}
			break ;
		case KEY_CLEAR:
			choosed = true;
			break;
		default:
			break;
		}
	}
	memset(aucDisplayNumber, 0, sizeof(aucDisplayNumber));
	ASC_DrawAscString(48, 0, 8, aucDisplayNumber);
	if(ucKey == KEY_CLEAR)
	{
		return ERR_CANCEL;
	}
	else
	{
		return SUCCESS;
	}
}

/*************���뵥����ĸ������*****************
����˵��:
pucBuf :�û�����ĵ����ַ�
����ֵ˵��:
KEY_ENTER :ȷ�ϲ����ַ�
KEY_BCKSP :ɾ���û���������һ���ַ�
SUCCESS :�û�ϣ���˳���������������ַ�
KEY_CLEAR :�û���ȡ����,
                     ɾ���������ַ�,����Ϊ0���˳����Ҳ�����
KEY_PAPER_FEED :�л����뷨
��ע:��ΪӢ������,���û���ѡ���ַ�
            ��ʾ�ڵ�4�е��°���
***********************************************************/
unsigned char PY_InputLetter(unsigned char *pucBuf)
{
	static const unsigned char aucUpperCaseLeterTab[][21] =
	{{"0,.-!\"%\'()/:;<=>?*"},
	{"1QZ# +-"},
	{"2ABC"},
	{"3DEF"},
	{"4GHI"},
	{"5JKL"},
	{"6MNO"},
	{"7PRS"},
	{"8TUV"},
	{"9WXY"}};

	static const unsigned char aucLowerCaseLeterTab[][21] =
	{{"0,.-!\"%\'()/:;<=>?*"},
	{"1qz# +-"},
	{"2abc"},
	{"3def"},
	{"4ghi"},
	{"5jkl"},
	{"6mno"},
	{"7prs"},
	{"8tuv"},
	{"9wxy"}};

	static const unsigned char aucNumberTab[][2] =
	{{"0"},
	{"1"},
	{"2"},
	{"3"},
	{"4"},
	{"5"},
	{"6"},
	{"7"},
	{"8"},
	{"9"},
	{"0"}};
	static const unsigned char aucSymbolTab[] =
	{"  "};//{",.-!\"%\'()+/:;<=>?"};
	const unsigned char *(aucKeyTab[11]);

	//��ʾ���뷨����
	unsigned char aucDisp[5];
	//��ǰ������ַ�
	unsigned char ucCurrentInput;
	unsigned char ucKey;
	unsigned char ucLastKey;
	unsigned char ucEnd;
	unsigned int uiTimeout;
	unsigned char index;
	unsigned char indexFont;
	unsigned char indexChar;
	unsigned char aucAscFont[ASC_SIZE + 1];
	//��ʼ������
	memset(&aucDisp[0],0,sizeof(aucDisp));
	pucBuf[0] = 0;
	pucBuf[1] = 0;
	ucCurrentInput = 0;
	ucLastKey = 0;
	ucEnd = FALSE;
	//panmin
	#if 1
	sg_ucPreviousKey = 0;
	sg_ucPreviousChar = 0;
	sg_ucKeyTabOffset=0;
	#endif

	util_Printf("��ǰֵ:%02x\n",sg_ucPreviousKey);
	//�������뷨ѡ���ַ�����
	switch(sg_inputType)
	{
		case INPUT_TYPE_UPPER_CASE:
			for(index = 0; index < 10; ++index)
			{
				aucKeyTab[index] = aucUpperCaseLeterTab[index];
			}
			aucKeyTab[index] = aucSymbolTab;
			strcpy((char *)aucDisp,"ABC:");
			break;
		case INPUT_TYPE_LOWER_CASE:
			for(index = 0; index < 10; ++index)
			{
				aucKeyTab[index] = aucLowerCaseLeterTab[index];
			}
			aucKeyTab[index] = aucSymbolTab;
			strcpy((char *)aucDisp,"abc:");
			break;
		case INPUT_TYPE_NUMBER:
			for(index = 0; index < 11; ++index)
			{
				aucKeyTab[index] = aucNumberTab[index];
			}
			strcpy((char *)aucDisp,"123:");
			break;
		default:
			return KEY_CLEAR;
			break;
	}
	//��ʾ��ǰ���뷨
	ASC_DrawAscString(56, 0, 6, aucDisp);

	//�ָ��û���һ�εİ���
	if(sg_ucPreviousChar != 0 && sg_inputType != INPUT_TYPE_NUMBER)
	{
		ucCurrentInput = sg_ucPreviousChar;
		ucLastKey = sg_ucPreviousKey;

		util_Printf((char *)"In sg_ucPreviousChar = %c\n", sg_ucPreviousChar);
		util_Printf((char *)"In sg_ucPreviousKey = %c\n", sg_ucPreviousKey);

	}

	do
	{
		//��ʾ�û���ѡ����ַ���
		//��ǰ�û�ͣ�����ַ��úڵװ�����ʾ
		//util_Printf((char *)"ucflag3 = %02x\n", ucflag);

		//�ȴ�1�밴��(���޸�ͣ��ʱ��)
		uiTimeout = LETTER_INPUT_INTERVAL;
		ucKey = PY_GetKey(sg_uiTrueTimeOut);
		//������ʱ�䳬ʱ
		if(sg_uiTrueTimeOut == 0)
		{
			sg_ucPreviousKey = 0;
			sg_ucPreviousChar = 0;
			sg_ucKeyTabOffset=0;

			pucBuf[0] = 0;
			ucKey = KEY_CLEAR;
			break;
		}

		if(ucKey == NULL)
		{
			//��ʱ��ѵ�ǰ��ѡ�������沢�˳�
			sg_ucPreviousKey = 0;
			sg_ucPreviousChar = 0;
			sg_ucKeyTabOffset=0;

			pucBuf[0] = ucCurrentInput;
			ucKey = KEY_ENTER;
			break;
		}

		if((ucKey >= '0') && (ucKey <= '9'))
		{
			if(sg_inputType != INPUT_TYPE_NUMBER)
			{
#if PINYIN_TEST > 0
				util_Printf((char *)"Different key in letter\n");
#endif
				sg_ucKeyTabOffset = 0;
				ucLastKey = ucKey;
				//��ͬ����,����������ַ�,
				//���汾���ַ�,�����ϴ��ַ�
				sg_ucPreviousKey = ucKey;

				ASC_DrawAscString(56,24,6,(const unsigned char*)"");
				if(sg_ucPreviousKey != 0)
				{
					indexChar = sg_ucPreviousKey - '0';
#if PINYIN_TEST > 0
					util_Printf((char *)"Char table = %s\n", aucKeyTab[indexChar]);
					if(ucCurrentInput != 0)
					{
						util_Printf((char *)"Current char = %c\n", ucCurrentInput);
					}
#endif
					for(index = 0; aucKeyTab[indexChar][index] != 0; ++index)
					{
						 ASC_GetAscFont(aucKeyTab[indexChar][index], aucAscFont);
						if(sg_ucKeyTabOffset == index)
						{
							for(indexFont = 0; indexFont < ASC_SIZE; indexFont += 2)
							{
								aucAscFont[indexFont] = (~aucAscFont[indexFont]);
							}
						}
						Os__graph_display
							(
							56,
							index * 6 + 24,
							aucAscFont,
							ASC_SIZE
							);
					}
				}

				util_Printf("��888ֵ:%02x\n",ucKey);
				if((ucKey >= '0') && (ucKey <= '9'))
				{
					sg_ucPreviousChar = aucKeyTab[sg_ucPreviousKey - '0'][0];
					//pucBuf[0] = sg_ucPreviousChar;
					// sg_ucKeyTabOffset++;
				}
				util_Printf("��ֵ:%02x,%d\n",sg_ucPreviousChar,sg_ucKeyTabOffset);
				//ucKey = KEY_ENTER;
				//ucEnd = TRUE;
				ucEnd = false;
			}
			else
			{
				//�������뷨�µ����ְ��������˳�(Ϊ��ֱ����ʾ)
				pucBuf[0] = aucKeyTab[ucKey - '0'][0];
				ucKey = KEY_ENTER;
				ucEnd = TRUE;
			}
		}
		else if(ucKey == KEY_00_PT)
		{
			if(sg_inputType != INPUT_TYPE_NUMBER)
			{
				ASC_DrawAscString(56,24,6,(const unsigned char*)"");
				
				util_Printf("��ǰֵ:%02x,%02x,%02x\n",sg_ucPreviousChar,sg_ucKeyTabOffset,sg_ucPreviousKey);
				if(sg_ucPreviousChar != 0)
				{
					//00����,�л����¸��Ϳ�ѡ�ַ�
					util_Printf("��ѡ:%d\n",strlen((char *)aucKeyTab[sg_ucPreviousKey-'0']));
					if(sg_ucKeyTabOffset < strlen((char *)aucKeyTab[sg_ucPreviousKey-'0']))
					{
						sg_ucKeyTabOffset++;
					}
					else
					{
						sg_ucKeyTabOffset = 0;
					}

					util_Printf((char *)"Same key in letter, offset = %d\n", sg_ucKeyTabOffset);
				}

				util_Printf("��ǰ����:%02x\n",sg_ucPreviousKey);
				if(sg_ucPreviousKey != 0)
				{
					indexChar = sg_ucPreviousKey-'0';
#if PINYIN_TEST > 0
					util_Printf((char *)"9999Char table = %s\n", aucKeyTab[indexChar]);
					if(ucCurrentInput != 0)
					{
						util_Printf((char *)"888Current char = %c\n", ucCurrentInput);
					}
#endif

					for(index = 0; aucKeyTab[indexChar][index] != 0; ++index)
					{
						 ASC_GetAscFont(aucKeyTab[indexChar][index], aucAscFont);
						if(sg_ucKeyTabOffset == index)
						{
							for(indexFont = 0; indexFont < ASC_SIZE; indexFont += 2)
							{
								aucAscFont[indexFont] = (~aucAscFont[indexFont]);
							}
						}
						Os__graph_display
							(
							56,
							index * 6 + 24,
							aucAscFont,
							ASC_SIZE
							);
					}
				}
				
			}
			else
			{
				strcpy((char *)pucBuf, "00");
				ucKey = KEY_ENTER;
				ucEnd = TRUE;
			}
		}
		else
		{
			//������ֹ��ܼ�
			ucLastKey = ucKey;
			switch(ucKey)
			{
				case KEY_ENTER:
					util_Printf((char *)"ENTER, offset = %d\n", sg_ucKeyTabOffset);
					util_Printf("LstK=%02x\n",ucLastKey);
					if (ucLastKey==KEY_ENTER && (!sg_ucPreviousKey))
					{
						sg_ucPreviousKey = 0;
						sg_ucPreviousChar = 0;
						sg_ucKeyTabOffset=0;
						return(SUCCESS);
					}
					// if (sg_ucKeyTabOffset)
						// --sg_ucKeyTabOffset;
					ucCurrentInput = aucKeyTab[sg_ucPreviousKey - '0'][sg_ucKeyTabOffset];
					util_Printf("����:%02x\n",ucCurrentInput);
					if (ucCurrentInput !=0)
					{
						pucBuf[0] = ucCurrentInput;
					}
					else
						ucKey = SUCCESS;
					sg_ucPreviousKey = 0;
					sg_ucPreviousChar = 0;
					sg_ucKeyTabOffset=0;
					memset(aucKeyTab,0,sizeof(aucKeyTab));
					ucEnd = TRUE;
					break;
				case KEY_CLEAR:
				case KEY_BCKSP:
					sg_ucPreviousKey = 0;
					sg_ucPreviousChar = 0;
					sg_ucKeyTabOffset=0;
					ucEnd = TRUE;
					break;
				case KEY_PAPER_FEED:
					ucEnd = TRUE;
					break;
				default :
					break;
			}
		}
	}while(ucEnd == FALSE);

	return(ucKey);
}

/*****�����û������ƴ���ַ��������******
����˵��:
pucInput :�������ƴ��
ucInputOffset :�����ƫ����
pucKeyTab :�ɲ�����ַ���
pucKeyTabOffset :�ַ������ʼƫ����,
                           �ı���ƫ������Ҫ��ʹ��,��ָ�봫��
����ֵ˵��:ͬPY_Check�ķ���ֵ˵��
***********************************************************/
InputCheck PY_InsertPinYin(	unsigned char *pucInput,	const unsigned char ucInputOffset,	const unsigned char *pucKeyTab,	unsigned char *pucKeyTabOffset)
{
	unsigned char index;
	InputCheck inputCheck;
	int maxKeyLen = strlen((char *)pucKeyTab);
	//���ַ������ʼƫ������ʼ���ַ����������
	for(index = 0; index < maxKeyLen; ++index)
	{
#if PINYIN_TEST > 0
		util_Printf((char *)"KeyTabOffset = %d\n", *pucKeyTabOffset);
#endif
		pucInput[ucInputOffset] = pucKeyTab[*pucKeyTabOffset];
		inputCheck = PY_Check(pucInput);
		if(inputCheck == INPUT_NOT_FOUND)
		{
			if(((*pucKeyTabOffset) + 1) < maxKeyLen)
			{
				++(*pucKeyTabOffset);
			}
			else
			{
				(*pucKeyTabOffset) = 0;
			}
		}
		else
		{
			break;
		}
	}
	return inputCheck;
}

/***************ƴ�����뵥��������*******************
����˵��:
pucBuf :�û�����ĵ���������
����ֵ˵��:
KEY_ENTER :ȷ�ϲ����ַ�
KEY_BCKSP :ɾ���û���������һ���ַ�
SUCCESS :�û�ϣ���˳���������������ַ�
KEY_CLEAR :�û���ȡ����,
                     ɾ���������ַ�,����Ϊ0���˳����Ҳ�����
KEY_PAPER_FEED :�л����뷨
��ע:�û������ƴ����ʾ�ڵ�4�е��°���
***********************************************************/
unsigned char PY_InputChinese(unsigned char *pucBuf)
{
	const unsigned char aucKeyTab[][7] =
	{
		{"qz"},
		{"abc"},
		{"def"},
		{"ghi"},
		{"jkl"},
		{"mno"},
		{"prs"},
		{"tuv"},
		{"wxy"},
	};
	unsigned char   aucDisp[24];
	unsigned char   aucInput[20];
	unsigned char   ucKey;
	unsigned char   ucLastKey;
	unsigned char   ucKeyTabOffset;
	unsigned char   ucEnd;
	unsigned char   ucChooseResult;
	unsigned char   ucRedraw;
	unsigned char   ucCount;
	unsigned char   ucOffset;
	unsigned int uiLastTime;
	InputCheck inputCheck;
	const unsigned char *pucChinese;
	unsigned int maxKeyLen;
	unsigned char ucLastChar;

	memset(&aucDisp[0],0,sizeof(aucDisp));
	memset(&aucInput[0],0,sizeof(aucInput));
	ucLastKey = 0;
	ucEnd = FALSE;
	ucRedraw = TRUE;
	uiLastTime = sg_uiTrueTimeOut;
	ucCount = 0;

	do
	{
		if(ucRedraw == TRUE)
		{
			//��ʾƴ��
			//Os__clr_display(2);
			memset(&aucDisp[0],0,sizeof(aucDisp));

			strcpy((char *)aucDisp, "PinYin:");
			strcat((char *)aucDisp, (const char *)aucInput);
#if PINYIN_TEST > 0
			util_Printf((char *)"Input PinYin = %s\n", aucDisp);
#endif
			strcat((char *)aucDisp, "_");
			ASC_DrawAscString(56, 0, 6, aucDisp);
			pucChinese = PY_Ime(aucInput);
			if(pucChinese != NULL)
			{
				//����ƥ���ƴ������ʾ����
				memset(&aucDisp[0],0,sizeof(aucDisp));
				strncpy((char *)aucDisp, (const char*)pucChinese, 16);
				//Os__GB2312_display(2,0,aucDisp);
			}
			ucRedraw = FALSE;
		}

		//�ȴ�����
		uiLastTime = sg_uiTrueTimeOut;
		ucKey = PY_GetKey(sg_uiTrueTimeOut);
		if(ucKey == NULL)
		{
			ucKey = KEY_CLEAR;
			break;
		}

		if((ucKey >= '1') && (ucKey <= '9'))
		{
			if(ucLastKey != ucKey)
			{
				//��ͬ�������Բ������ַ�
				ucKeyTabOffset = 0;
				inputCheck = PY_InsertPinYin(aucInput, ucCount, aucKeyTab[ucKey - '1'], &ucKeyTabOffset);
#if PINYIN_TEST > 0
				util_Printf((char *)"Different key = %c\n", ucKey);
#endif
				if(inputCheck == INPUT_NOT_FOUND)
				{
					//���е��ַ������ɲ���
					ucLastKey = 0;
					ucRedraw = FALSE;
					ucKeyTabOffset = 0;
				}
				else
				{
					//�ҵ���һ���ɲ�����ַ�
					ucLastKey = ucKey;
					ucRedraw = TRUE;
					++ucCount;
				}
				aucInput[ucCount] = 0;
			}
			else
			{
				if((uiLastTime - sg_uiTrueTimeOut) < PINYIN_INPUT_INTERVAL)
				{
					if(ucCount)
					{
						ucOffset = ucCount - 1;
					}
					else
					{
						ucOffset = 0;
					}
					maxKeyLen = strlen((char *)aucKeyTab[ucKey - '1']) - 1;
					if(ucKeyTabOffset < maxKeyLen)
					{
						++ucKeyTabOffset;
					}
					else
					{
						ucKeyTabOffset = 0;
					}
					ucLastChar = aucInput[ucOffset];
					//��������������Ǵ���һ���ɲ�����ַ�
					//���Բ���inputCheck���ж�
					inputCheck = PY_InsertPinYin(aucInput, ucOffset, aucKeyTab[ucKey - '1'], &ucKeyTabOffset);
#if PINYIN_TEST > 0
					util_Printf((char *)"Same key = %c\n",ucKey);
#endif
					//ֻ��һ�����ʱ��ʱ������,
					//Ŀ���ǽ���ͬ����ת����Ϊ��һ������
					if(ucLastChar == aucInput[ucOffset])
					{
						uiLastTime = sg_uiTrueTimeOut + PINYIN_INPUT_INTERVAL;
					}
					else
					{
						ucRedraw = TRUE;
					}
				}
				if((uiLastTime - sg_uiTrueTimeOut) >= PINYIN_INPUT_INTERVAL)
				{
					//������ͬ���β�ͬ����
					ucKeyTabOffset = 0;
					inputCheck = PY_InsertPinYin(aucInput, ucCount, aucKeyTab[ucKey - '1'], &ucKeyTabOffset);
#if PINYIN_TEST > 0
					util_Printf((char *)"Same key = %c, timeout\n",ucKey);
#endif
					if(inputCheck == INPUT_NOT_FOUND)
					{
						ucLastKey = 0;
						ucRedraw = FALSE;
						ucKeyTabOffset = 0;
					}
					else
					{
						ucLastKey = ucKey;
						ucRedraw = TRUE;
						++ucCount;
					}
					aucInput[ucCount] = 0;
				}
			}
		}
		else
		{
			//������ֹ��ܼ�
			ucLastKey = ucKey;
			switch(ucKey)
			{
			case KEY_ENTER:
				if(ucCount == 0)
				{
					ucKey = SUCCESS;
					ucEnd = TRUE;
				}
				else if((pucChinese = PY_Ime(aucInput)) != NULL)
				{
					ucChooseResult = PY_ChooseBuffer(pucChinese,pucBuf);
					if(sg_uiTrueTimeOut == 0)
					{
						ucEnd = TRUE;
						ucKey = KEY_CLEAR;
					}
					else if(ucChooseResult == SUCCESS)
					{
						ucEnd = TRUE;
					}
				}
				else
				{
					Os__beep();
				}
				break;
			case KEY_CLEAR:

				if(ucCount)
				{
					ucCount = 0;
					ucLastKey = 0;
					memset(&aucInput[0],0,sizeof(aucInput));
					memset(&aucDisp[0],0,sizeof(aucDisp));
					ucRedraw = TRUE;
				}
				else
				{
					ucEnd = TRUE;
				}
				break;
			case KEY_BCKSP:
				if(ucCount)
				{
					ucCount--;
					aucInput[ucCount] = 0;
					ucRedraw = TRUE;
				}
				else
				{
					ucEnd = TRUE;
				}

				break;
			case KEY_PAPER_FEED:
				ucEnd = TRUE;
				break;
			default :
				break;
			}
		}
	}
	while(ucEnd == FALSE);

	return(ucKey);
}

#if SI_JIAO > 0
/***************��λ�����뵥��������*******************
����˵��:
pucBuf :�û�����ĵ���������
����ֵ˵��:
KEY_ENTER :ȷ�ϲ����ַ�
KEY_BCKSP :ɾ���û���������һ���ַ�
SUCCESS :�û�ϣ���˳���������������ַ�
KEY_CLEAR :�û���ȡ����,
                     ɾ���������ַ�,����Ϊ0���˳����Ҳ�����
KEY_PAPER_FEED :�л����뷨
��ע:�û��������λ����ʾ�ڵ�4�е��°���
***********************************************************/
unsigned char PY_InputSiJiao(unsigned char *pucBuf)
{
	unsigned char aucDispBuf[5];
	unsigned char ucKey;
	unsigned char ucEnd;
	unsigned char ucCount;
	unsigned long qm;
	unsigned long wm;

	ucEnd = FALSE;
	ucCount = 0;
	memset(aucDispBuf, 0, sizeof(aucDispBuf));

	do
	{
		ASC_DrawAscString(56, 0, 6, (const unsigned char*)"QuWei:");
		ASC_DrawAscString(56, 36, 6, aucDispBuf);
		ASC_DrawAscString(56, 36 + ucCount * 6, 6, (const unsigned char*)"_");
		ucKey = PY_GetKey(sg_uiTrueTimeOut);
		switch(ucKey)
		{
		case KEY_ENTER:
			if(ucCount >= 4)
			{
				qm = asc_long(&aucDispBuf[0], 2);
				wm = asc_long(&aucDispBuf[2], 2);
				qm = qm + 0xA0;
				wm = wm + 0xA0;
				long_tab(&pucBuf[0], 1, &qm);
				long_tab(&pucBuf[1], 1, &wm);
				pucBuf[2] = 0;
				ucCount = 0;
				ucEnd = TRUE;
			}
			else if(ucCount == 0)
			{
				ucKey = SUCCESS;
				ucEnd = TRUE;
			}
			break;
		case KEY_BCKSP:
			if(ucCount != 0)
			{
				--ucCount;
				aucDispBuf[ucCount] = 0x00;
			}
			else
			{
				ucEnd = TRUE;
			}
			break;
		case KEY_CLEAR:
			if(ucCount != 0)
			{
				ucCount = 0;
				memset(aucDispBuf, 0, sizeof(aucDispBuf));
			}
			else
			{
				ucEnd = TRUE;
			}
			break;
		case KEY_PAPER_FEED:
			ucEnd = TRUE;
			break;
		default:
			if((ucKey >= '0')
			&& (ucKey <= '9'))
			{
				if(//������Χ
				(ucCount >= 4)
				//ǰ��λ������ͬʱΪ0
				|| ((ucCount == 1)
					&& (aucDispBuf[ucCount - 1] == '0')
					&& (ucKey == '0'))
				//����λ������ͬʱΪ0
				|| ((ucCount == 3)
					&& (aucDispBuf[ucCount - 1] == '0')
					&& (ucKey == '0')))
				{
					Os__beep();
				}
				else
				{
					aucDispBuf[ucCount] = ucKey;
					ucCount++;
				}
			}
			break;
		}
	}
	while(ucEnd == FALSE);

	return(ucKey);
}
#endif

unsigned char PY_Input(unsigned char *pucBuf)
{
	static const unsigned char aucUpperCaseLeterTab[][21] =
	{{"0!\"%\'()./:;<=>?*,"},
	{"1QZ# +-"},
	{"2ABC"},
	{"3DEF"},
	{"4GHI"},
	{"5JKL"},
	{"6MNO"},
	{"7PRS"},
	{"8TUV"},
	{"9WXY"}};

	static const unsigned char aucLowerCaseLeterTab[][21] =
	{{"0!\"%\'()./:;<=>?*,"},
	{"1qz# +-"},
	{"2abc"},
	{"3def"},
	{"4ghi"},
	{"5jkl"},
	{"6mno"},
	{"7prs"},
	{"8tuv"},
	{"9wxy"}};

	static const unsigned char aucNumberTab[][2] =
	{{"0"},
	{"1"},
	{"2"},
	{"3"},
	{"4"},
	{"5"},
	{"6"},
	{"7"},
	{"8"},
	{"9"},
	{"0"}};
	static const unsigned char aucSymbolTab[] =
	{"!\"%\'()+-./:;<=>?"};
	const unsigned char *(aucKeyTab[11]);

	//��ʾ���뷨����
	unsigned char aucDisp[5];
	//��ǰ������ַ�
	unsigned char ucCurrentInput;
	unsigned char ucKey;
	unsigned char ucLastKey;
	unsigned char ucEnd;
	unsigned int uiTimeout;
	unsigned char index;
	unsigned char indexFont;
	unsigned char indexChar;
	unsigned char aucAscFont[ASC_SIZE + 1];
	unsigned char ucflag=0;
	//��ʼ������
	memset(&aucDisp[0],0,sizeof(aucDisp));
	pucBuf[0] = 0;
	pucBuf[1] = 0;
	ucCurrentInput = 0;
	ucLastKey = 0;
	ucEnd = FALSE;
	//panmin
	#if 1
	sg_ucPreviousKey = 0;
	sg_ucPreviousChar = 0;
	sg_ucKeyTabOffset=0;
	#endif
	//�������뷨ѡ���ַ�����
	switch(sg_inputType)
	{
	case INPUT_TYPE_UPPER_CASE:
		for(index = 0; index < 10; ++index)
		{
			aucKeyTab[index] = aucUpperCaseLeterTab[index];
		}
		aucKeyTab[index] = aucSymbolTab;
		strcpy((char *)aucDisp,"ABC:");
		break;
	case INPUT_TYPE_LOWER_CASE:
		for(index = 0; index < 10; ++index)
		{
			aucKeyTab[index] = aucLowerCaseLeterTab[index];
		}
		aucKeyTab[index] = aucSymbolTab;
		strcpy((char *)aucDisp,"abc:");
		break;
	case INPUT_TYPE_NUMBER:
		for(index = 0; index < 11; ++index)
		{
			aucKeyTab[index] = aucNumberTab[index];
		}
		strcpy((char *)aucDisp,"123:");
		break;
	default:
		return KEY_CLEAR;
		break;
	}
	//��ʾ��ǰ���뷨
	ASC_DrawAscString(56, 0, 6, aucDisp);

	//�ָ��û���һ�εİ���
	if(sg_ucPreviousChar != 0 && sg_inputType != INPUT_TYPE_NUMBER)
	{
		ucCurrentInput = sg_ucPreviousChar;
		ucLastKey = sg_ucPreviousKey;
#if PINYIN_TEST > 0
		util_Printf((char *)"In sg_ucPreviousChar = %c\n", sg_ucPreviousChar);
		util_Printf((char *)"In sg_ucPreviousKey = %c\n", sg_ucPreviousKey);
#endif
	}

	do
	{
		//��ʾ�û���ѡ����ַ���
		//��ǰ�û�ͣ�����ַ��úڵװ�����ʾ
		util_Printf((char *)"ucflag3 = %02x\n", ucflag);



		//�ȴ�1�밴��(���޸�ͣ��ʱ��)
		uiTimeout = LETTER_INPUT_INTERVAL;
		ucKey = PY_GetKey(sg_uiTrueTimeOut);
		//������ʱ�䳬ʱ
		if(sg_uiTrueTimeOut == 0)
		{
			sg_ucPreviousKey = 0;
			sg_ucPreviousChar = 0;
				sg_ucKeyTabOffset=0;

			pucBuf[0] = 0;
			ucKey = KEY_CLEAR;
			break;
		}

		if(ucKey == NULL)
		{
			//��ʱ��ѵ�ǰ��ѡ�������沢�˳�
			sg_ucPreviousKey = 0;
			sg_ucPreviousChar = 0;
				sg_ucKeyTabOffset=0;

			pucBuf[0] = ucCurrentInput;
			ucKey = KEY_ENTER;
			break;
		}

		if((ucKey >= '0') && (ucKey <= '9'))
		{
			if(sg_inputType != INPUT_TYPE_NUMBER)
			{
				{
#if PINYIN_TEST > 0
					util_Printf((char *)"Different key in letter\n");
#endif
#if 0
					sg_ucKeyTabOffset = 0;
					ucLastKey = ucKey;
					{
						//��ͬ����,����������ַ�,
						//���汾���ַ�,�����ϴ��ַ�
						sg_ucPreviousKey = ucKey;

						sg_ucPreviousChar = aucKeyTab[ucKey - '0'][sg_ucKeyTabOffset];
						pucBuf[0] = sg_ucPreviousChar;
						ucKey = KEY_ENTER;
						ucEnd = TRUE;
					}
#else
						sg_ucKeyTabOffset = 0;
					ucLastKey = ucKey;
					{
						//��ͬ����,����������ַ�,
						//���汾���ַ�,�����ϴ��ַ�
						sg_ucPreviousKey = ucKey;
						util_Printf((char *)"ucKey = %02x\n", ucKey);
						ASC_DrawAscString(56,24,6,(const unsigned char*)"");
						if(sg_ucPreviousKey != 0)
						{
							indexChar = sg_ucPreviousKey - '0';
#if PINYIN_TEST > 0
							util_Printf((char *)"Char table = %s\n", aucKeyTab[indexChar]);
							if(ucCurrentInput != 0)
							{
								util_Printf((char *)"Current char = %c\n", ucCurrentInput);
							}
#endif
							for(index = 0; aucKeyTab[indexChar][index] != 0; ++index)
							{
								 ASC_GetAscFont(aucKeyTab[indexChar][index], aucAscFont);
								if(sg_ucKeyTabOffset == index)
								{
									for(indexFont = 0; indexFont < ASC_SIZE; indexFont += 2)
									{
										aucAscFont[indexFont] = (~aucAscFont[indexFont]);
									}
								}
								Os__graph_display
									(
									56,
									index * 6 + 24,
									aucAscFont,
									ASC_SIZE
									);
							}
						}
/*
						switch(ucflag)
						{
						case 0:
							ucflag=1;
						break;
						case 1:
							ucflag=2;
						break;
						case 2:
							ucflag=1;
						break;
						default :
						break;
						}
						util_Printf((char *)"ucflag = %02x\n", ucflag);
						if(ucflag==1)
							{
							continue;
							}
							*/
						while (1)
						{
							ucKey = PY_GetKey(sg_uiTrueTimeOut);
							if((ucKey >= '0') && (ucKey <= '9')||(ucKey==KEY_CLEAR))
							{
								break;
							}
						}
						if(ucKey==KEY_CLEAR)
							{
							return ERR_CANCEL;
							}
						{
						sg_ucPreviousChar = aucKeyTab[sg_ucPreviousKey - '0'][ucKey-'1'];

						pucBuf[0] = sg_ucPreviousChar;
						}
						ucKey = KEY_ENTER;
						ucEnd = TRUE;
					}
#endif

				}

			}
			else
			{
				//�������뷨�µ����ְ��������˳�(Ϊ��ֱ����ʾ)
				pucBuf[0] = aucKeyTab[ucKey - '0'][0];
				ucKey = KEY_ENTER;
				ucEnd = TRUE;
			}
		}
		else if(ucKey == KEY_00_PT)
		{
			if(sg_inputType != INPUT_TYPE_NUMBER)
			{
			#if 1
		#if PINYIN_TEST > 0
					util_Printf((char *)"sg_ucPreviousChar= %d\n", sg_ucPreviousChar);
		#endif
				if(sg_ucPreviousChar != 0)
				{
					//00����,�л����¸��Ϳ�ѡ�ַ�
					if(sg_ucKeyTabOffset < strlen((char *)aucKeyTab[sg_ucPreviousKey-'0']) - 1)
					{
						sg_ucKeyTabOffset++;
					}
					else
					{
						sg_ucKeyTabOffset = 0;
					}
#if PINYIN_TEST > 0
					util_Printf((char *)"Same key in letter, offset = %d\n", sg_ucKeyTabOffset);
#endif
					ucCurrentInput = aucKeyTab[sg_ucPreviousKey - '0'][sg_ucKeyTabOffset];
					pucBuf[0] = ucCurrentInput;
					ucEnd = TRUE;
				}
			#endif
			}
			else
			{
				strcpy((char *)pucBuf, "00");
				ucKey = KEY_ENTER;
				ucEnd = TRUE;
			}
		}
		else
		{
			//������ֹ��ܼ�
			ucLastKey = ucKey;
			switch(ucKey)
			{
			case KEY_ENTER:
				ucKey = SUCCESS;
				sg_ucPreviousKey = 0;
				sg_ucPreviousChar = 0;
				sg_ucKeyTabOffset=0;
				ucEnd = TRUE;
				break;
			case KEY_CLEAR:
			case KEY_BCKSP:
				sg_ucPreviousKey = 0;
				sg_ucPreviousChar = 0;
				sg_ucKeyTabOffset=0;
				ucEnd = TRUE;
				break;
			case KEY_PAPER_FEED:
				ucEnd = TRUE;
				break;
			default :
				break;
			}
		}
	util_Printf((char *)"ucflag2 = %02x\n", ucflag);

	}
	while(ucEnd == FALSE);
#if PINYIN_TEST > 0
	if(sg_ucPreviousChar != 0)
	{
		util_Printf((char *)"Out sg_ucPreviousChar = %c\n", sg_ucPreviousChar);
		util_Printf((char *)"Out sg_ucPreviousKey = %c\n", sg_ucPreviousKey);
		util_Printf((char *)"Out sg_ucKeyTabOffset = %d\n", sg_ucKeyTabOffset);
	}
#endif
	return(ucKey);
}
#endif
