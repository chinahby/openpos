/* skb25_communication.h */
#ifndef __SAND_COM__
#define COM_EXTERN extern
#else
#define COM_EXTERN
#endif

#include "osmodem.h"

/**
 * @defgroup �ṩ����ͨ�ŵ���غ���
 * @author
 *				
 * @brief �򿪴���,���ͽ��մ��������Լ��رմ��ڵȲ���
 * @{
 */

extern int _g_com2_openned;
extern unsigned char Os__init_com(COM_PARAM *ComParameter);
extern unsigned char Os__txcar(unsigned char Charac);
extern unsigned short Os__rxcar(unsigned short TimeOut);

/**
 * @fn unsigned char OSUART_Init1(COM_PARAM *pa)
 * @brief 	��ʼ������һ,�˴���Ϊ�����������
 *
 * @param pa	����������Ϣ,�˴��ڰ����ĳ�Ա������
 * 				    unsigned char stop_bits;        // nbr of bit stop 1 or 2 
 * 				    unsigned int  speed;            // speed from 75 to 19200bds 
 * 				    unsigned char com_nr;           // 0 : modem, 1 : rs232 
 * 				    unsigned char data_bits;        // nbr of data bits 7 or 8 
 * 				    unsigned char parity;           // parity E ou O ou N 
 *
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 * @code
 * 	int main(int argv, char *argc[])
 * 	{
 * 		unsigned int rel; 
 * 		COM_PARAM param;
 * 		memset(&param,0,sizeof(param));
 * 		param.stop_bits = 1;
 * 		param.speed = 115200;
 * 		param.com_nr = 1;
 * 		param.data_bits = 8;
 * 		param.parity = 'N';
 * 		if(OSUART_Init1(&param) == 0)
 * 		{
 * 			printf("open uart OK!\n");
 * 			
 * 			if(argv == 1)
 * 			{
 * 				printf("recv data!\n");
 * 				while(1)
 * 				{
 * 					rel = OSUART_RxChar1(500);
 * 					if(rel/256 == 0)
 * 					{
 * 						//�ɹ��������ݺ󷵻��ַ�9
 * 						OSUART_TxChar1(0x39);
 * 						printf("read date OK is %c\n",rel);
 * 					}
 * 					else
 * 						printf("read date error!\n");
 * 					sleep(2);	
 * 				}
 * 			}
 * 			else
 * 			{
 * 				printf("send data!\n");
 * 				while(1)
 * 				{
 * 					OSUART_TxChar1(0x31);
 * 					printf("send date ok!\n");	
 * 				}
 * 			}
 * 		}
 * 		else
 * 			printf("open uart error!\n");
 * 		OSUART_Close1();
 * 		return 0;
 * 	}
 * @endcode
 */
unsigned char OSUART_Init1(COM_PARAM *pa);

/**
 * @fn unsigned short OSUART_RxChar1(unsigned short uiTimeout)
 * @brief 	�������Դ���1������
 *
 * @param uiTimeout		��ʱʱ��,��λ1ms
 *
 * @warning	����ֵ��λΪ״̬λ,��λΪ����λ,���ɹ�ʱ,״̬λΪ0,��λΪ����,ʧ��ʱ��λΪ0,��λΪ������
 * @return 		�ɹ����ض������ַ�,ʧ�ܷ��ص�״ֵ̬�ڸ�λ,Ϊ��0ֵ
 *
 */
unsigned short OSUART_RxChar1(unsigned short uiTimeout);

/**
 * @fn void OSUART_TxChar1(unsigned char ucCh)
 * @brief 	�Ӵ���1��������
 *
 * @param ucCh		Ҫ���͵�����
 *
 * @return 		�޷���ֵ
 *
 */
void OSUART_TxChar1(unsigned char ucCh);

/**
 * @fn void OSUART_Close1(void)
 * @brief 	�رմ���1
 *
 * @return 		�޷���ֵ
 *
 */
void OSUART_Close1(void);

/** @} end of etc_fns */
