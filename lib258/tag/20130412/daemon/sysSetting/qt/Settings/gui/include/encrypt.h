#ifndef	ENCRYPT_H_INCLUDED
#define	ENCRYPT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE  0
#endif

#define HASH_INDATA_MAXLEN     1024*8 
#define HASH_SHA1              1
#define HASH_SHA256            2 

#define HASH_SHA1_MAXLEN       20
#define HASH_SHA256_MAXLEN     32
#define ENCRYPT_MAX_KEYDATA    16
#define KEY_MAX_APPNUMBER      10

#define ENCRYPT_SAVE_BYPROTKEY       0x30   // �ñ�����Կ��ѹ���洫����Կ
#define ENCRYPT_SAVE_BYTRANSKEY      0x31   // �ô洢����Կ��ѹ���洫����Կ
#define ENCRYPT_DECRYPT_BYPROTKEY    0x32   // �ñ�����Կ�ӽ���
#define ENCRYPT_DECRYPT_BYTRANSKEY   0x33   // �ô洢����Կ�ӽ���
#define ENCRYPT_LOAD_PROTKEY         0x34   // ���ر�����Կ

#define ENCRYPT_DES_SINGLE                   0x30   //��DES����
#define ENCRYPT_3DES_SINGLE                  0x31   //3DES����
#define ENCRYPT_3DESDOUBLE                   0x32   //3DES˫��������
#define ENCRYPT_DES_SINGLE_DECRYPT           0x33   //��DES����
#define ENCRYPT_3DES_SINGLE_DECRYPT          0x34   //3DES����
#define ENCRYPT_3DESDOUBLE_DECRYPT           0x35   //3DES˫��������

#define KEY_DEL_ALL            0x01    //ȫ��ɾ��
#define KEY_DEL_PART           0x00    //����ɾ��


typedef struct
{
	 unsigned char        hashtype;                                             // HASH_SHA1 ,HASH_SHA256
   unsigned short       hashlen;                                             // ���������ݵĳ���,ͬ��Ҳ�Ǵ���ȥ������
   unsigned char        hashindata[HASH_INDATA_MAXLEN];                      //  ����������
   unsigned char        hashoutdata[HASH_SHA256_MAXLEN];                  // ���������HASHֵ��һ����20��һ����32
}ENCRYPT_HASH_DATA;

typedef struct
{
	 unsigned char        keytype;                                             // ����  0x30(�ñ�����Կ��ѹ���洫����Կ) 0x31�ô洢����Կ��ѹ���洫����Կ) 0x32(�ñ�����Կ�ӽ���)0x33(�ô洢����Կ�ӽ��� ) 0x34 (���ر�����Կ)
   unsigned short       saveappid;                                           //���洫����Կ��Ӧ�ú�
   unsigned short       cryptappid;                                       //  ���ܴ�����Կ��Ӧ�ú�
   unsigned short       cryptindex;                                         // ������Կ����
   unsigned short       saveindex;                                         // ����Կ����
   unsigned char        crypttype;                                         // .���ܷ�ʽ      0x30 ��des���� ������  0x31 3des����  0x32 3des˫��������  0x33 ��DES���� 0x34 3DES����  0x35   3DES˫��������
   unsigned char        protindex;                                          // ������Կ����
   unsigned char        keyvalue[ENCRYPT_MAX_KEYDATA*2];                                         // ��Կ
   unsigned char        outlen;                                            // �����������ݳ���
   unsigned char        outdata[ENCRYPT_MAX_KEYDATA*2];                                         //  ������������
}ENCRYPT_DRV_DATA; 

typedef struct
{
	 unsigned char        delflag;                                             // ȫ��ɾ�������ǲ���ɾ��
   unsigned char        appnumber;                                             // ɾ����Կ��Ӧ�ø���
   unsigned short       appid[KEY_MAX_APPNUMBER];                             //  ����Ӧ�ú�
}KEY_APPID_DATA;



int ENCRYPT_Hash(ENCRYPT_HASH_DATA * hashdata);
int ENCRYPT_Decrypt_Load_Key(ENCRYPT_DRV_DATA *keydrvdata);
int KEY_KeyCheckApp(KEY_APPID_DATA *keyappiddata);

#ifdef __cplusplus
}
#endif

#endif //ENCRYPT_H_INCLUDED
