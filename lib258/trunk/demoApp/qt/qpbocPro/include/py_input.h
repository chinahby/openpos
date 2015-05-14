#ifndef	YPINPUT_H
#define YPINPUT_H

#ifdef __cplusplus
extern "C" {
#endif

//���뷨��
#define PY_INPUT_CHINESE		0x01
#define PY_INPUT_LOWER_CASE	0x02
#define PY_INPUT_UPPER_CASE	0x04
#define PY_INPUT_NUMBER		0x08
#define PY_INPUT_QUWEI	0x10
/*****************���뷨������********************************
����˵��:
aucOut :����ȷ�Ϻ�,�����û�������ַ�
ucLine :�û������ַ���ʾ������,��0��ʼ,
           ƴ�����뷨����ʱ����С����1,
           ƴ�����뷨������ʱ����С����2,
ucMin :�����û��������С�ַ�,����С����
          ucMax��PINYIN_MAX_INPUT_NUMBER
ucMax :�����û����������ַ�,����С����
           PINYIN_MAX_INPUT_NUMBER
ucInputFlag :ȷ�����õ����뷨,
                   ����PY_INPUT_NUMBER | PY_INPUT_CHINESE��ʾֻ������
                   ƴ�����������뷨֮���л�
usTimeOut :�������ʱ��,��λ��
����ֵ˵��:
ERR_CANCEL :�û���ȡ��������ʱ�䳬���涨ʱ��,
                     aucOut���ݲ��ᱻ�޸�
SUCCESS :�û������ȷ�ϲ��ҷ��Ϲ涨���ַ���,
                aucOut���ݱ��޸�Ϊ�û�������ַ�
*********************************************************************/
unsigned char PY_Main
	(
	unsigned char *aucOut,
	unsigned char ucLine,
	const unsigned char ucMin,
	const unsigned char ucMax,
	const unsigned char ucInputFlag,
	const unsigned short usTimeOut
	);
unsigned char PY_Input
	(
	unsigned char *pucBuf
	);


#ifdef __cplusplus
}
#endif

#endif
