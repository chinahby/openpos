/*
	timer_driver.h
*/
/*  */
#ifndef __S3C2410_TIMER_H__
#define __S3C2410_TIMER_H__

#ifdef __cplusplus 
extern "C" { 
#endif


#define PIT_IO_MAGIC	't'

#define DRIVER_VERSION		"0.1"

#define TIMER_DEV_NAME "/dev/s3c2410_timer"

unsigned int pit_set_second;
unsigned int pit_get_second;

#define CMD_PIT_ADD	_IOW(PIT_IO_MAGIC, 0, int)		/*���Ӳ���ʼ����ʱ��*/
#define CMD_PIT_FOO	_IOW(PIT_IO_MAGIC, 1, int)	/*���ö�ʱ����ʱֵ*/
#define CMD_PIT_GET	_IOR(PIT_IO_MAGIC, 2, int)	/*��ȡ��ǰʣ��ֵ*/
#define CMD_PIT_STOP	_IO(PIT_IO_MAGIC, 3)		/*ɾ����ʱ��*/

#ifdef __cplusplus 
}
#endif

#endif
