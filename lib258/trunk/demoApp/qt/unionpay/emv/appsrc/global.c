
#define	DATAGLO
#include <global.h>
#include <semaphore.h>
unsigned char ProgressFlag;// �߳̽�����־
unsigned char jiaoyiCnt;
unsigned char g_ThreadFlag;// �Ƿ����߳�
unsigned char g_ProgressBarFlag; // ��������ʾ
sem_t binSem1;
sem_t binSem2;
#undef DATAGLO

