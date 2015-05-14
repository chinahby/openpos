#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED


/**
 * @defgroup wireless��Ϣ��ȡ�����ú���
 * @author �ܽ�
 * @brief �ṩ��wifi���������������úͻ�ȡ��Ϣ
 * @{
 */

typedef struct
{
	char addressMAC[50];		//MAC��ַ
	char essid[50];				//wifi��ID
	char frequency[50];			//Ƶ��
	char bitRates[50];			//������
	char quality[50];			//�ź�����
	char sigLev[50];			//�ź�ˮƽ
}WIFISCAN;

/**
 * @fn int wifi_scan(char *ifname, WIFISCAN wifiscan[], int maxSize)
 * @brief 	��ȡwifiɨ��Ľ����Ϣ,������ǰ����wifi��ssid,�ź�ǿ�ȼ��ܷ�ʽ����Ϣ
 *
 * @param ifname		��Ҫ��ȡ��������Ϣ,һ��Ϊwlan0
 * @param wifiscan  	����wifiɨ����Ϣ�б�Ľṹ�������ַ
 * @param maxSize		����ܱ���wifiɨ����Ϣ�ĸ���
 *
 * @return 		�ɹ�����0,ʧ�ܷ���-1
 *
 */
int wifi_scan(char *ifname, WIFISCAN wifiscan[], int maxSize);

#endif	/* WIFI_H_INCLUDED */
/** @} end of etc_fns */
