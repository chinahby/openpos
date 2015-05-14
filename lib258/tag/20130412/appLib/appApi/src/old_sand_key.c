/*
��������˲��ּ��ܺ�������
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termio.h>
#include <termios.h>    /*PPSIX�ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
#include <time.h>       /*ʱ�亯��*/
#include <sys/time.h>
#include <getopt.h>
#include <signal.h>


#define __LUXAPP_KBD__
#include "sand_key.h"

#ifdef EN_DBG
#define dbg Uart_Printf
#else
#define dbg
#endif

char *KEY_DEVICE="/dev/ttyS1";
static int fd_comkey=0;
DRV_OUT *kbd=NULL;


/*
 *�������������������ȴ�Ӧ��ȡ��ֵ
 * ���ض����ĳ��ȣ�
 *
 */
int get_key_from_serial(char *dev, unsigned char *data, unsigned char *status)
{
    int fd, ret, i;
    int count=100;


    return ret;
}



unsigned char scancode_to_asc(unsigned char in)
{

#if 0
    unsigned char out;
    dbg("in=0x%x ",  in);
    switch (in)
    {
    case SKBKEY_ENTER1:
    case SKBKEY_ENTER2:
        out=0x0d;
        break;
    case SKBKEY_N0: 	//����0
        out= '0';
        break;
    case SKBKEY_N1: 	//���� 1
        out= '1';
        break;
    case SKBKEY_N2: 	//���� 2
        out= '2';
        break;
    case SKBKEY_N3: 	//���� 3
        out= '3';
        break;
    case SKBKEY_N4: 	//���� 4
        out= '4';
        break;
    case SKBKEY_N5: 	//���� 5
        out= '5';
        break;
    case SKBKEY_N6: 	//���� 6
        out= '6';
        break;
    case SKBKEY_N7: 	//���� 7
        out= '7';
        break;
    case SKBKEY_N8: 	//���� 8
        out= '8';
        break;
    case SKBKEY_N9: 	//���� 9
        out= '9';
        break;

    case SKBKEY_N00: 	//���� �� .
        out= KEY_00_PT;
        break;


    case SKBKEY_F1:
        out=KEY_F1;
        break;
    case SKBKEY_F2:
        out=KEY_F2;
        break;
    case SKBKEY_F3:
        out=KEY_F3;
        break;
    case SKBKEY_F4:
        out=KEY_F4;
        break;

    case SKBKEY_CANCEL: 	//ȡ����,esc?
        out= KEY_CANCEL;
        break;

    case SKBKEY_CLEAR: 	//�����,backspace?
        out= KEY_BCKSP;
        break;

    case SKBKEY_SUBTOTAL:	// С��
        out=KEY_SUBTOTAL;
        break;

    case SKBKEY_PAPER_FEED: 	//���ڽ�ֽ��
        out= KEY_PAPER_FEED;
        break;

    case SKBKEY_PAPER_REVERSEFEED: 	//���ڽ�ֽ��
        out= KEY_PAPER_REVERSEFEED;
        break;

    case SKBKEY_CASHIER:		//ӪҵԱ
        out=CASHKEY_CASHIER;
        break;

    case SKBKEY_JUMP:			//jump out
        out=KEY_JUMP;
        break;

    case SKBKEY_ADDING:
        out=KEY_ADDING;
        break;
    case SKBKEY_DECREASE:
        out=KEY_DECREASE;
        break;
    case SKBKEY_MUITIPLY:
        out=KEY_MUITIPLY;
        break;
    case SKBKEY_DIVISION:
        out=KEY_DIVISION;
        break;


        //��ĸ��
    case SKBKEY_A: 	//��ĸa
        out= 'A';
        break;

    case SKBKEY_B: 	//��ĸ b
        out= 'B';
        break;

    case SKBKEY_C: 	//��ĸ c
        out= 'C';
        break;

    case SKBKEY_D: 	//��ĸ d
        out= 'D';
        break;

    case SKBKEY_E: 	//��ĸ e
        out= 'E';
        break;

    case SKBKEY_F: 	//��ĸ f
        out= 'F';
        break;

    case SKBKEY_G: 	//��ĸ g
        out= 'G';
        break;

    case SKBKEY_H: 	//��ĸ h
        out= 'H';
        break;

    case SKBKEY_I: 	//��ĸ i
        out= 'I';
        break;

    case SKBKEY_J: 	//��ĸ j
        out= 'J';
        break;

    case SKBKEY_K: 	//��ĸ k
        out= 'K';
        break;

    case SKBKEY_L: 	//��ĸ l
        out= 'L';
        break;

    case SKBKEY_M: 	//��ĸ m
        out= 'M';
        break;

    case SKBKEY_N: 	//��ĸ n
        out= 'N';
        break;

    case SKBKEY_O: 	//��ĸ o
        out= 'O';
        break;

    case SKBKEY_P: 	//��ĸ p		//?
        out= 'P';
        break;

    case SKBKEY_Q: 	//��ĸ q
        out= 'Q';
        break;

    case SKBKEY_R: 	//��ĸ r
        out= 'R';
        break;

    case SKBKEY_S: 	//��ĸ s
        out= 'S';
        break;

    case SKBKEY_T: 	//��ĸ t
        out= 'T';
        break;

    case SKBKEY_U: 	//��ĸ u
        out= 'U';
        break;

    case SKBKEY_V: 	//��ĸ v
        out= 'V';
        break;

    case SKBKEY_W: 	//��ĸ w
        out= 'W';
        break;

    case SKBKEY_X: 	//��ĸ x
        out= 'X';
        break;

    case SKBKEY_Y: 	//��ĸ y
        out= 'Y';
        break;

    case SKBKEY_Z: 	//��ĸ z
        out= 'Z';
        break;

    default:
        out= 0x00;
        break;
    }//switch end
    dbg("out=%c(%02x)\n", out, out);
    return out;
#endif

}


void EatAnyKey()
{

}



DRV_OUT * OSMMI_GetKey()
{
    DRV_OUT *tmp;
    tmp=Os__get_key();
    return tmp;
}


/*
���ã�����һ�������¼�, �����ؼ�ֵ��
������ DRV_OUT * Os__get_key () ;
�ȴ�����
˵����Os__get_key�������ն˼���, ͬʱ���ȴ���������, ��Ӧ�ÿ����ն˰������ء�
����ֵ�� ָ�� DRV_OUT �����ݽṹָ�롣
         �ṹ�������£�
		reqnr�� �ݲ�ʹ��.
		gen_status�� ϵͳģ��״̬��
		drv_status�� ϵͳִ��״̬
		xxdata[0]        :���������ݳ���, ����λһ�ֽڡ�
		xxdata[1]        �����ؼ�ֵ, ��Ϊasc�롣
*/
DRV_OUT * Os__get_key ()
{
    unsigned char data, status;
    char *dev_path=KEY_DEVICE;
    int ret;

    kbd=(DRV_OUT *)malloc(sizeof(DRV_OUT));
    memset(kbd, 0, sizeof(kbd));

    ret=get_key_from_serial(dev_path, &data, &status);
    if (ret<0) return NULL;

    kbd->xxdata[0]=1;
    kbd->xxdata[1]=data;
    kbd->gen_status=status;
    return kbd;
}

DRV_OUT * Os__get_key_pp(void)
{
}

unsigned char   Os__xget_key_pp(void)
{
}




/**
 *��ȡ����״̬����ֵ OK
 */
unsigned char  Os__get_key_status()
{
    unsigned int tmp;
    DRV_OUT tmp_drv;
    unsigned char data=0, status;
    char *dev_path=KEY_DEVICE;
    int ret;

    ret=get_key_from_serial(dev_path, &data, &status);

    if (status==DRV_ENDED)
    {
        kbd->xxdata[0]=0;
        kbd->xxdata[1]=scancode_to_asc(data);	/*asc��*/
        dbg("xxdata1=%c tmp_data=%x\n", kbd->xxdata[1], data);
    }
    kbd->gen_status=status;

    return status;
}


/*
 *�رմ��ڣ��ͷ�ָ��
 */
/*
void Os__stop_key()
{
	dbg("fd_comkey=%d\n", fd_comkey);

	if (fd_comkey>0)
	{
		ioctl(fd_comkey, CMD_TIMER_KBD_STOP);
		close(fd_comkey);
	}
	if (kbd!=NULL) {
		dbg("kbd is not NULL\n");
//		kbd=NULL;
		free(kbd);
	}
	else
		dbg("kbd is NULL\n");
}
*/


/*
����	�� �õ�һ������Ϣ��
����	�� unsigned char Os__xget_key () ;
�ȴ�	����
˵��	���ȴ��ն˰���, ���ն��ް�������, ϵͳ��һֱ�ȴ�, ����ͬ�ں���Os__get_key()��
����ֵ	����ֵ��
ok
*/
unsigned char Os__xget_key ()
{
    unsigned char data, status;
    char *dev_path=KEY_DEVICE;
    int ret;

    while (1)
    {
        ret=get_key_from_serial(dev_path, &data, &status);
        if (ret<0)
        {
            Uart_Printf("%s get key failed:%d \n", ret);
            return -1;
        }

        if (status==KEY_REP_OK)
        {
            return data;
        }
    }
}

unsigned char Os__read_eeprom(unsigned short uiStartAddr, unsigned char ucLen, unsigned char *pucPtr)
{
}

void Os__crypt(unsigned char *pucCryptKey)
{

}
unsigned char *Os__xget_pin(unsigned char ucRow,unsigned char ucCol, unsigned char *pucPtr, unsigned char ucCharNB)
{

}

DRV_OUT *Os__get_fixkey(unsigned char ucRow,unsigned char ucCol,unsigned char *pucPtr, unsigned char ucCharNB)
{

}


DRV_OUT *Os__get_pin(unsigned char ucRow,unsigned char ucCol, unsigned char *pucPtr, unsigned char ucCharNB)
{
}
int key_main(char argc, char *argv[])
{
    int ret;
    char *dev_path=KEY_DEVICE;
    unsigned char status, data;



    return 0;
}

