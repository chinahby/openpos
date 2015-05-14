#ifndef SAND_NETWORK_H
#define SAND_NETWORK_H

/**
 * @defgroup �ṩ����ͨ�ŷ�����շ�����
 * @author 
 *				
 * @brief ����ͨ�Ų�������
 * @{
 */

#define DEV_COMM_GSM 0
#define DEV_COMM_WIFI 1
#define DEV_COMM_ETHERNET 2
#define DEV_COMM_MODEN 3

unsigned char TCP_CheckOSVersion(void);

/**
 * @fn unsigned char TCP_Open(int *piTCPHandle)
 * @brief 	�򿪵�ǰPOS���лỰ,�����ػỰ���
 *
 * @param iHandle		�Ự�����ָ��
 *
 * @return 		�ɹ�����0,����������ʧ��
 *
 */
unsigned char TCP_Open(int *piTCPHandle);
//unsigned char TCP_Connect(int iTCPHandle,unsigned char *ulPeerAddr,unsigned short uiPeerPort);


/**
 * @fn unsigned char TCP_Connect(int iTCPHandle,unsigned long ulPeerAddr,unsigned short uiPeerPort)
 * @brief 	�ô򿪵ĻỰ��������
 *
 * @param iTCPHandle		�Ự���
 * @param ulPeerAddr	����IP��ַ
 * @param uiPeerPort	�����˿ں�
 *
 * @warning IP��ַ�õ��ʮ���Ʊ�ʾ����Ӧ���ĸ�ʮ��������Ӧ��ʮ��������ƴ��һ��ULONG��
 * @return 		�ɹ�����0,����������ʧ��
 *
 */
unsigned char TCP_Connect(int iTCPHandle,unsigned long ulPeerAddr,unsigned short uiPeerPort);

/**
 * @fn unsigned char TCP_Send(int iTCPHandle,unsigned char *pucInBuf,unsigned short uiInLen)
 * @brief 	�ô򿪵ĻỰ��������
 *
 * @param iTCPHandle	�Ự���
 * @param pucInBuf		����BUFF��ָ��
 * @param uiInLen		����BUFF�ĳ���
 * 
 * @warning 			iTCPHandle������ȴ򿪺����ӣ����ܷ��ͽ�������
 * @return 		�ɹ�����0,����������ʧ��
 *
 */
unsigned char TCP_Send(int iTCPHandle,unsigned char *pucInBuf,unsigned short uiInLen);

/**
 * @fn unsigned char TCP_Recv(int iTCPHandle,unsigned char *pucInBuf,unsigned short *puiInLen,unsigned short uiTimeout)
 * @brief 	�ô򿪵ĻỰ��������
 *
 * @param iTCPHandle	�Ự���
 * @param pucInBuf		��������BUFF��ָ��
 * @param puiInLen		��������BUFF��ʵ�ʳ���
 * @param uiTimeout		���ճ�ʱʱ�䣨�룩
 * 
 * @warning 			iTCPHandle������ȴ򿪺�����,���ܷ��ͽ�������,�ڷ������ݺͽ�������֮�価����ʹ�÷�ʱ���(��UART��PRINTF����),����������ʱ���ݽ��ղ���
 * @return 		�ɹ�����0,����������ʧ��
 *
 */
unsigned char TCP_Recv(int iTCPHandle,unsigned char *pucInBuf,unsigned short *puiInLen,unsigned short uiTimeout);

/**
 * @fn unsigned char TCP_Close(int iTCPHandle)
 * @brief 	�رջỰ
 *
 * @param iTCPHandle	�Ự���
 * 
 * @return 		����0
 *
 */
unsigned char TCP_Close(int iTCPHandle);

//if succeed return 0,else return -1;
//only support gsm and wifi right now.
/**
 * @fn int activeDev(int idev)
 * @brief 	��������ͨ��·�ɱ�,Ŀǰֻ֧��GSM��WIFI
 *
 * @param idev	����ͨ����,��Ҫʹ�õ�����ģ��,����GSM,WIFI,ETHERNET��MODEN
 * 
 * @return 		����0
 *
 */
int activeDev(int idev);

/* udp*/
unsigned char UDP_Open(int *piUDPHandle);
unsigned char UDP_Config(int iUDPHandle,unsigned long ulPeerAddr,unsigned short uiPeerPort);
unsigned char UDP_Send(int iUDPHandle,unsigned char *pucInBuf,unsigned short uiInLen);
unsigned char UDP_Recv(int iUDPHandle,unsigned char *pucInBuf,unsigned short *puiInLen,unsigned short uiTimeout);
unsigned char UDP_Close(int iUDPHandle);


#endif

