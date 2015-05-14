#ifndef __MODULES_DRVS_H_
#define __MODULES_DRVS_H_

/**
 * @defgroup �����������⣬���ȣ�״ָ̬ʾ�ƽӿں���
 * @brief ����beep,backlight,led�Ľӿ�
 * @author ��һ��
 *
 **/

#ifdef __cplusplus
extern "C" {
#endif


#define BEEP_BACKLIGHT_DEVICE "/dev/beep_backlight"

/**
 * @def MODULES_SUCCESS
 * @brief �ɹ�
 **/
#define  MODULES_SUCCESS 0

/**
 * @def MODULES_ERRDEVICE
 * @brief ģ���ʧ��
 **/
#define  MODULES_ERRDEVICE -1

/**
 * @def MODULES_PARAM
 * @brief ��������
 **/
#define  MODULES_PARAM -2




/**
 * @fn int Os__lcd_bright(unsigned char ucBright)
 * @brief ������Ļ���Ȼ��߶Աȶ�
 * @param ucBright    0-255
 *
 * @return 0 �ɹ��� С��0��ʧ�ܣ���������ʶ�����涨��
 */
int Os__lcd_bright(unsigned char ucBright);

/**
 * @fn int Os__light_on(void)
 * @brief ����Ļ�����������
 *
 * @return 0 �ɹ��� С��0��ʧ�ܣ���������ʶ�����涨��
 */
int Os__light_on(void);


/**
 * @fn int Os__light_off(void)
 * @brief �ر���Ļ�����������
 *
 * @return 0 �ɹ��� С��0��ʧ�ܣ���������ʶ�����涨��
 */
int Os__light_off(void);


/**
 * @fn int Os__led_display(int status,int led)
 * @brief ����led��
 *
 * @return void
 */
void Os__led_display(int status,int led);


/*
* @fn int Os__beep(void)
* @brief ���������� ��һ��
*
* @return 0 �ɹ��� С��0��ʧ�ܣ���������ʶ�����涨��
*/
int Os__beep(void);

#ifdef __cplusplus
}
#endif

#endif

