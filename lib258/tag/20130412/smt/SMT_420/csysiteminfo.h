#ifndef CSYSITEMINFO_H
#define CSYSITEMINFO_H
#include <QString>
#include <QList>

#define DAEMON      1
#define LIB         2
#define DRIVER      3
#define FONT        4
#define DATA        5
#define SHELL       6
#define UBOOT       7
#define KERNEL      8
#define CRT         9
#define CRAMFS      10
#define OTHER_BOOT  11


typedef struct{
    QString session;
    QString key;
    QString value;
    int type;
    QString brief;
    QString valueList;
    QString parent;
} DAEMON_SET;

class CSYSItemInfo
{
public:
    CSYSItemInfo();
    void resetItem();
    int getItemCount(QList<CSYSItemInfo> *itemList);
    bool removeItem(int index, QList<CSYSItemInfo> *itemList);
    bool replaceItem(int index, QList<CSYSItemInfo> *itemList);
    int isItemExist(QList<CSYSItemInfo> *itemList);
    bool addItem(QList<CSYSItemInfo> *itemList);
public:
    //����
    int type;
    //Դ�ļ�����
    QString srcName;
    //Դ�ļ�·��
    QString srcPath;
    //Դ�ļ�״̬
    int status;
    //Ӧ�ð汾��
    QString version;
    //�����DTA��������DTA��·��
    QString dtaPath;
    //���Item��KO����Daemon��KO���ر�־����,0�����أ�1����
    int insmod;
    //KO debug ��־
    int dbg;
};

#endif // CSYSITEMINFO_H
