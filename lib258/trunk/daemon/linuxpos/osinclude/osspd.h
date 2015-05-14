/*********************
create by liuyifeng	2010/5/14 11:50:55
**********************/
#ifndef __OS_SPD_H
#define __OS_SPD_H

#define SPD_FLASHSIZE_MAX		10485760		//��װ�ļ��ܴ�С1M,����down��app

#define SPD_TLV		          20		//TLV�����Ը���

#define SPD_SUCCESS		   		 0		
#define SPD_OPENFAILED   		-1		//���ڴ�ʧ��
#define SPD_CANCEL		   		-2		//ȡ��
#define SPD_CONTINUE	  		-3		//����
#define SPD_FILEERR		  		-5		//�ļ������ϴ�С
#define SPD_LOADCONFIG		  -6		//�����ļ���ȡʧ��
#define SPD_ERRFILE				  -7		//�ļ�����
#define SPD_BADCRC				  -8		//crcУ���
#define SPD_OPENFILE			  -9		//�����ļ���ʧ��
#define SPD_WRITEFILE			  -10		//�����ļ�дʧ��
#define SPD_ZIP						  -11		//��ѹʧ��
#define SPD_RELIST					-12		//�������

//�ļ�·��
#define SPD_LIB_PATH					"/lib/"
#define SPD_DRIVER_PATH				"/driver/"
#define SPD_FONT_PATH					"/fonts/"


#define SPD_LINUXPOS_INFO_FILE	SPD_CONFIG_PATH"linuxpos.dat"
#define SPD_APP_INFO_FILE     	SPD_CONFIG_PATH"appinfo.dat"
#define SPD_FONT_INFO_FILE			SPD_CONFIG_PATH"font.dat"
#define SPD_LIB_INFO_FILE				SPD_CONFIG_PATH"lib.dat"
#define SPD_DRIVER_INFO_FILE		SPD_CONFIG_PATH"driver.dat"
#define SPD_CONFIG							SPD_CONFIG_PATH"spd.dat"

void OSSPD_tool_reboot(void);
void OSSPD_tool_disBufSize(void);
int OSSPD_transmit(void);
void OSSPD_file_delete(void);
void OSSPD_file_formatFileSys(void);
void OSSPD_setting(void);
int OSSPD_file_readDWNDirect(void);
unsigned char OSSPD_file_set(void);

#endif

