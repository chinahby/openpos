/*******************************************************************************
* PS4000 ���ò�������
*
* Note:
*			��Ҫ�ο�PS400��oscfg.h
********************************************************************************/

#ifndef __OSCFG_H_
#define __OSCFG_H_

#ifndef OSCFGDATA
#define OSCFGDATA extern
#endif



#ifdef __cplusplus
extern "C" {
#endif




//�ſ����ò���
typedef struct
{
	unsigned char ucMagDebug;
	unsigned char ucMagVer[5+1]; //��ʽ��mm.nn ���5���ֽ�
}OSMAGCFG;
OSCFGDATA OSMAGCFG OsMAGCfg;


//IC�����ò���
typedef struct
{
	unsigned char ucICDebug;
	unsigned char ucICVer[5+1]; //��ʽ��mm.nn ���5���ֽ�
}OSICCFG;
OSCFGDATA OSICCFG OsICCfg;


#ifdef __cplusplus
}
#endif

#endif
