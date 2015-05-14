#ifndef NETWORK_H_INCLUDE
#define NETWORK_H_INCLUDE

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ���纯����
 * @author �ܽ�
 * @brief �ṩ��������в�������غ���
 * @{
 */


typedef struct
{
	char ssid[100];
	char psk[50];
}WifiNetwork;


/**
 * @fn int Os__setSetting(WifiNetwork wifiInfo)
 * @brief ��wifi��Ϣ���浽ϵͳ�����ļ���
 * @param wifiInfo wifi������Ϣ�ṹ��
 *
 * @return 0 �ɹ�, < 0 ����.
 * @code
 *	int main(void)
 *	{
 *		WifiNetwork wifi;
 *		memset(&wifi,0,sizeof(WifiNetwork));
 *		sprintf(wifi.ssid,"YF_II");
 *		sprintf(wifi.psk,"SAND");
 *		return Os__setSetting(wifi);
 *	}
 * @endcode
 */
int Os__setWifi(WifiNetwork wifiInfo);

#ifdef __cplusplus
}
#endif


#endif // NETWORK_H_INCLUDE
/** @} end of etc_fns */
