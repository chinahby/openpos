#ifndef __MODULES_DRVS_H_
#define __MODULES_DRVS_H_

/**
 * @defgroup 蜂鸣器，背光，亮度，状态指示灯接口函数
 * @brief 操作beep,backlight,led的接口
 * @author 刘一峰
 *
 **/

#ifdef __cplusplus
extern "C" {
#endif


#define BEEP_BACKLIGHT_DEVICE "/dev/beep_backlight"

/**
 * @def MODULES_SUCCESS
 * @brief 成功
 **/
#define  MODULES_SUCCESS 0

/**
 * @def MODULES_ERRDEVICE
 * @brief 模块打开失败
 **/
#define  MODULES_ERRDEVICE -1

/**
 * @def MODULES_PARAM
 * @brief 参数错误
 **/
#define  MODULES_PARAM -2




/**
 * @fn int Os__lcd_bright(unsigned char ucBright)
 * @brief 调节屏幕亮度或者对比度
 * @param ucBright    0-255
 *
 * @return 0 成功； 小于0，失败，错误代码标识见上面定义
 */
int Os__lcd_bright(unsigned char ucBright);

/**
 * @fn int Os__light_on(void)
 * @brief 打开屏幕背光或者亮度
 *
 * @return 0 成功； 小于0，失败，错误代码标识见上面定义
 */
int Os__light_on(void);


/**
 * @fn int Os__light_off(void)
 * @brief 关闭屏幕背光或者亮度
 *
 * @return 0 成功； 小于0，失败，错误代码标识见上面定义
 */
int Os__light_off(void);


/**
 * @fn int Os__led_display(int status,int led)
 * @brief 操作led灯
 *
 * @return void
 */
void Os__led_display(int status,int led);


/*
* @fn int Os__beep(void)
* @brief 操作蜂鸣器 响一秒
*
* @return 0 成功； 小于0，失败，错误代码标识见上面定义
*/
int Os__beep(void);

/**
 * @fn void modules_wifi_active(int opt)
 * @brief 打开或关闭wifi
 * @param opt	对wifi要进行的操作,0关闭,1打开
 *
 * @return 无返回值
 */
void modules_wifi_active(int opt);

/**
 * @fn void modules_smt_active(int opt)
 * @brief 打开或关闭SMT下载功能
 * @param opt	对SMT下载功能要进行的操作,0关闭,1打开
 *
 * @return 无返回值
 */
void modules_smt_active(int opt);

/**
 * @fn void modules_gps_active(int opt)
 * @brief 打开或关闭GPS模块
 * @param opt	对GPS模块要进行的操作,0关闭,1打开
 *
 * @return 成功返回0,失败返回-1
 */
int modules_gps_active(int opt);

#ifdef __cplusplus
}
#endif

#endif

