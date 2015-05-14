#ifndef CAPPITEMINFO_H
#define CAPPITEMINFO_H
#include <QList>
#include <QString>
#include <QStringList>

#define APPLICATION     0

class CAPPItemInfo
{
public:
    CAPPItemInfo();
    void resetItem(void);
    int isItemExist(QList<CAPPItemInfo> *itemList);
    bool addItem(QList<CAPPItemInfo> *itemList);

public:
    //Դ�ļ�����
    QString srcName;
    //Ӧ����POS�ն�������ʾ������
    QString appDesc;
    //Ӧ�ú�
    int appNumber;
    //Դ�ļ��Ĵ洢·��,������POS�˵��ļ�·��������
    QString srcPath;

    /*
    **  Ӧ�õ�״̬,4���ֽ�int���ͱ���,���Ϊ��λ�ұ�Ϊ��λ
    **
    **  8 7 6 5   4 3 2 1
    **  | | | |   | | | |
    **  x x x x   x x x x
    **
    ** ÿһλ����������:
    **  1)Ӧ��ICOͼ���޸ı�־,0δ�޸�,1�Ѿ��޸Ļ���
    **  2)Ӧ�ô�ӡ������ͼƬ�޸ı�־,0δ�޸�,1�Ѿ��޸Ļ���
    **  3)Ӧ�������ļ��޸ı�־,0δ�޸�,1�Ѿ��޸Ļ���
    **  4)UI�ű��޸ı�־,0δ�޸�,1�Ѿ��޸Ļ���
    **  5)Ӧ��������POS����־Ϊ0,�����־Ϊ1
    **  6)�Ѿ����޸��ļ��ɹ����±�־Ϊ0,����ʧ�ܻ�δ����Ϊ1
    **  7)Ӧ������POS��ʱ,�ɹ���POS�л�ȡ��ϸ��Ϣ��־Ϊ0,����Ϊ1
    **  8)Ӧ������DTA�ļ���־Ϊ1,����Ϊ0
    */
    int appStatus;
    //Ӧ�����ر�־��Ϊtrueʱ����Ӧ��,��config.ini�б���Ϊ1����Ӧ��
    bool hideFlag;
    //Ӧ��ICOͼƬ·��
    QString ICOPicPath;
    //Ӧ��Logo/��ӡͼƬ·��
    QStringList logoPicPath;
    //Ӧ��Logo/��ӡͼƬ���Ѿ�ɾ����ͼƬ
    QStringList delPicture;
    //Ӧ��������Ϣ�������̻��ţ��ն˺ţ�ͨѶ��ʽ�ȵ�
    QString appSettings;
    //Ӧ������޸�����
    QString editDate;
    //��Ŀ���
    QString itemCode;
    //Ӧ�ð汾��
    QString version;
    //���»�ɾ����־,1��ʾ���������,2��ʾ����
    int editType;
    //����������
    QString owner;
    //��֤��
    QString checkCode;
    //�����dta��������dta����·��
    QString dtaPath;
    //UI �ű�·��
    QString uiScriptPath;
    //ɾ����UI�ű���
    QString delUiScriptName;
};

#endif // CAPPITEMINFO_H
