#ifndef GSM_H_INCLUDED
#define GSM_H_INCLUDED

/**
 * @defgroup �ṩGSM���õ���غ���
 * @author ��һ��
 *				
 * @brief ����GSMƵ����APN��
 * @{
 */

/**
 * @fn int GSM__dial_check(unsigned short uiTimeout)
 * @brief 	���GSM�����Ƿ�ɹ�
 *
 * @param uiTimeout	��ʱ�˳�����
 *
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int GSM__dial_check(unsigned short uiTimeout);

/**
 * @fn void GSM__setParam_gprs(int _bands)
 * @brief 	����GSM�Ĳ���
 *
 * @param _bands	GSM����
 *
 * @return 		�޷���ֵ
 *
 */
void GSM__setParam_gprs(int _bands);

/**
 * @fn int GSM_set_APN(unsigned char* ucAPN)
 * @brief 	����GSM��APN
 *
 * @param ucAPN	GSM��APN
 *
 * @warning		�˺���ֻ�ı�GSM��APN,�������÷�����޸�APN����,ֻ���ڲ�ʹ��
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int GSM_set_APN(unsigned char* ucAPN);

void GSM_config_pap_chap(char *username, char *password);

/**
 * @fn int Os__change_APN(unsigned char* ucAPN)
 * @brief 	���GSM��APN
 *
 * @param ucAPN	GSM��APN
 *
 * @warning		�˺�����ı�GSM��APN,���������ļ������޸ĺ�����GSM���ŷ���
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int Os__change_APN(unsigned char* ucAPN);

/**
 * @fn int GSM_AT_cmd(char *cmd,unsigned char *rbuf,int rlen,int timeout)
 * @brief 	��GSMģ�鷢��AT����
 *
 * @param cmd		AT����
 * @param rbuf		����������յ�����Ϣ�洢��ַ
 * @param rlen		����Ϣbuf�ĳ���
 * @param timeout	��ʱʱ��,��λ��
 *
 * @warning		�˺�����ı�GSM��APN,���������ļ������޸ĺ�����GSM���ŷ���
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int GSM_AT_cmd(char *cmd,unsigned char *rbuf,int rlen,int timeout);

#endif //GSM_H_INCLUDED
/** @} end of etc_fns */
