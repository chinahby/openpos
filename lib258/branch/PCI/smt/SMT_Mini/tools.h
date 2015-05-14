#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include <QString>
#include <QTimer>
#include <QStringList>
#include <QFileInfo>
#include <QTime>
#include <QProgressBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QStringList>
#include "qextserialport.h"

//����
#define DWN_SAVE_PATH       "DWN/"              //δָ���洢Ŀ¼ʱ,DWN�ļ�����Ŀ¼
#define DWN_SAVE_NAME       "~dwnTemp.dwn"      //ֻ����ʱ��dwn��ʱ�ļ���
#define INI_FILE_CONFIG         "conf.ini"          //��׼�����ļ�����
#define DWN_FILE_HEAD       "SAND ARM7&ARM9 DNW"
#define ARM9_CMD            "\x02\x41\x52\x45\x59\x4F\x55\x4F\x4B\x03"
#define INI_FILE_CODE           "UTF-8"             //ini�ļ������ʽ
#define ARM7_CPU_TYPE       1
#define ARM9_CPU_TYPE       2

typedef struct
{
    QString curAppName;
    int curAppID;
    int curAppVersion;
    int curCpuType;//1-ARM7,2-ARM9
    int curMergeType;//1-merge,0-no merge
    int curArm7Len;
    int curArm9Len;
}DWN_FILE_INFO;


typedef struct
{
    QString appName;
    QString appID;
    QString appVersion;
    int cpuType;
}BIN_FILE_INFO;


class Tools
{
public:
    Tools();
    /*
     * @fn int getPrivateProfileInt(QString sessionName, QString keyName, int defValue, QString fileName)
     * @brief ��ȡINI�ļ���sessionΪsessionName,keyֵΪkeyName�ļ�ֵ��ֵ��Ϣ
     *
     * @param sessionName   INI�ļ��е�session
     * @param keyName       INI�ļ��е�key
     * @param defValue      ��ֵ��Ĭ��ֵ
     * @param fileName      INI�ļ�����
     *
     * @return  ������Ҫ��ȡ�ļ�ֵ
     *
     */
    int getPrivateProfileInt(QString sessionName, QString keyName, int defValue, QString fileName);

    /*
     * @fn QString getPrivateProfileString(QString sessionName, QString keyName, QString defValue, QString fileName)
     * @brief ��ȡINI�ļ���sessionΪsessionName,keyֵΪkeyName�ļ�ֵ�ַ�����Ϣ
     *
     * @param sessionName   INI�ļ��е�session
     * @param keyName       INI�ļ��е�key
     * @param defValue      ��ֵ��Ĭ��ֵ
     * @param fileName      INI�ļ�����
     *
     * @return  ������Ҫ��ȡ�ļ�ֵ
     *
     */
    QString getPrivateProfileString(QString sessionName, QString keyName, QString defValue, QString fileName);

    /*
     * @fn bool writePrivateProfileString(QString sessionName, QString keyName, QString value,QString fileName)
     * @brief ����ֵΪvalueд��INI�ļ���,����sessionΪsessionName,keyֵΪkeyName
     *
     * @param sessionName   INI�ļ��е�session
     * @param keyName       INI�ļ��е�key
     * @param value         ��ֵ
     * @param fileName      INI�ļ�����
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool writePrivateProfileString(QString sessionName, QString keyName, QString value,QString fileName);


    void getSerialPort(QextSerialPort *serialPort);
    int mergeDnwFile(QString curArm7Path, QString curArm9Path, QString mergeFilePath);
    void getStatusLabel(QLabel *msgDis);
    void getProgressBar(QProgressBar *proBar);

    void sleep(unsigned int msec);
    bool transmitFileToPos(QString filePath);
    void initMsgPorBar(int maximum);
    void getAppInfo(QString *appName, int *appID, int *appVersion, int *cpuType);
    void getErrMsg(QString *errMsg);

    int checkARM(int timeOut);
    bool convertFileToDwn(QString binFilePath, QString dwnFilePath);
    bool getFileInfo(DWN_FILE_INFO *curFileInfo, QString filePath);
    void clearDwnFileInfo(DWN_FILE_INFO *curFileInfo);
    bool getBinFileInfo(BIN_FILE_INFO *binFileInfo);
    int setBinFileInfo(BIN_FILE_INFO *binFileInfo);
    void getBinFileInfoList(QStringList *binFileNameList);

private:
    //�����������
    QProgressBar *msgProBar;
    //������״̬��
    QLabel *msgDis;
    //����ͨѶQextSerialPort�����ַ
    QextSerialPort *serialPort;
    //��ʾ���ڵ�����Ϣ
    QPlainTextEdit *plnEditSerialPort;

    //Ӧ������
    QString *appName;
    //Ӧ��ID
    int *appID;
    //Ӧ�ð汾
    int *appVersion;
    //Ӧ��CPU����:1 ARM7,2 ARM9
    int *cpuType;


    //����������Ϣ��ʾ
    QString *errMsg;
};

#endif // TOOLS_H
