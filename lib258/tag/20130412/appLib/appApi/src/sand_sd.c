#include <stdio.h>

#include <unistd.h>

/*
0�ɹ�����0ʧ��
*/
int Os__check_tf_ready(void)
{
	int iret=-1;
	iret=access("/mnt/mmcblk0p1",F_OK);
	return iret;
}
