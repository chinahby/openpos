#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
0�ɹ�����0ʧ��
*/
int Os__check_usb_ready(void)
{
	int iret=-1;
	iret=access("/mnt/sda1",F_OK);
	return iret;
}



	void usb_test(void)
	{
		int ret=0;
		ret=Os__check_usb_ready();
		if(ret!=0)
			Uart_Printf("USB is NOT ready\n");
		else
			Uart_Printf("USB is ready!\n");//����·��Ϊ/mnt/usb
	}

