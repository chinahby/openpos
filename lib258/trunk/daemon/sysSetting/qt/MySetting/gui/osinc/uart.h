#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/**
*@def UART_OK
*@brief ���ڲ����ɹ�
**/
#define UART_OK 0

/**
*@def UART_NOK
*@brief ���ڲ���ʧ��
**/
#define UART_NOK -1

/**
*@def UART_SENDERROR
*@brief ���ڷ���ʧ��
**/
#define UART_SENDERROR -2

/**
*@def UART_RECVERROR
*@brief ���ڽ���ʧ��
**/
#define UART_RECVERROR -3

/**
*@def UART_SELECTERROR
*@brief ����Select����
**/
#define UART_SELECTERROR -4

/**
*@def UART_TIMEOUT
*@brief ���ڽ��ճ�ʱ
**/
#define UART_TIMEOUT -5

/**
 * @fn int UART_Init(char *dev, COM_PARAM *pa)
 * @brief ��ʼ�������豸
 *
 * @return >0 ��ʼ���ɹ������ļ�������, UART_NOK ��ʼ��ʧ��.
 */
int UART_Init(char *dev, COM_PARAM *pa);

/**
 * @fn void UartClose(int fd)
 * @brief �رմ����豸
 *
 * @return void.
 */
void UartClose(int fd);

/**
 * @fn int UartSend(int fd, unsigned char* inBuf, long inLen)
 * @brief ָ�����ڷ���һ���ַ�
 *
 * @return UART_OK ���ͳɹ���UART_SENDERROR ����ʧ��.
 */
int UartSend(int fd, unsigned char* inBuf, long inLen);

/**
 * @fn int UartRecv(int fd,unsigned short timeout, unsigned char *recvBuf, size_t recvLen)
 * @brief ָ�����ڽ���һ���ַ�
 *
 * @return UART_OK ���ճɹ�, UART_RECVERROR ����ʧ�ܣ�UART_TIMEOUT ���ճ�ʱ��UART_SELECTERROR ����Select����.
 */
int UartRecv(int fd,unsigned short timeout, unsigned char *recvBuf, size_t recvLen);