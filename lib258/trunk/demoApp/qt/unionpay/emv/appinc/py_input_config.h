#ifndef PY51_CONFIG_H
#define PY51_CONFIG_H
#ifdef __cplusplus
extern "C"
{
#endif
	#include "include.h"
#ifdef __cplusplus
}
#endif

//������������ַ�
#define PINYIN_MAX_INPUT_NUMBER 100

static const unsigned int PINYIN_INPUT_INTERVAL = ONESECOND;
static const unsigned int LETTER_INPUT_INTERVAL = ONESECOND;

/*ģ���Ƿ��������
0 :�����
1 :���*/
#define PINYIN_TEST 0

#endif