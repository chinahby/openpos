/*
Linux POS APP for IC Card
*/

#ifndef __SAND_ICC_H__
#define __SAND_ICC_H__

#ifndef __LUXAPP_ICC__
#define ICC_EXTERN extern
#else
#define ICC_EXTERN
#endif

#include "osicc.h"
#include "osdriver.h"

#include <linux/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S3C2410_IC_IOCTL_BASE	'I'
#define DRIVER_VERSION "0.1"
#define DEVICE_NAME	"s3c2410_tda8007"


#define IC_HARDRESET _IOW(S3C2410_IC_IOCTL_BASE, 0, int)//���ڳ���ǿ�Ƹ�λ,2.6�в�ʹ��
#define IC_DETECT _IOWR(S3C2410_IC_IOCTL_BASE, 1, int) //����Ƿ��п�
#define IC_REMOVE _IOW(S3C2410_IC_IOCTL_BASE, 2, int) //�Ƴ������µ�
#define IC_RESET _IOW(S3C2410_IC_IOCTL_BASE, 3, int) //��λ
#define IC_CMD   _IOWR(S3C2410_IC_IOCTL_BASE, 4, int)


ICC_EXTERN ICC_ANSWER * Os__ICC_command (unsigned char ReaderNb, ICC_ORDER * Order);
ICC_EXTERN unsigned char Os__ICC_detect (unsigned char ReaderNb);
ICC_EXTERN DRV_OUT * Os__ICC_insert ();
ICC_EXTERN DRV_OUT * Os__ICC_Async_order (unsigned char *Order, unsigned short Length);
ICC_EXTERN DRV_OUT * Os__ICC_order (DRV_IN *Command);
ICC_EXTERN void Os__ICC_remove ();
ICC_EXTERN void Os__ICC_off ();
ICC_EXTERN unsigned char OSICC_Detect(unsigned char ucReader);

#ifdef __cplusplus
}
#endif
}
#endif
