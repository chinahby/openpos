#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include <QString>
#include <QProgressBar>
#include <QLabel>

#include "qextserialport.h"

//����
#define INI_FILE_CONFIG         "conf.ini"          //��׼�����ļ�����

#define DWN_FILE_HEAD       "SAND ARM7&ARM9 DNW"
#define ARM9_CMD            "\x02\x41\x52\x45\x59\x4F\x55\x4F\x4B\x03"
#define ARM7_CPU_TYPE       1
#define ARM9_CPU_TYPE       2

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
    bool getFileInfo(QString filePath);
    void getStatusLabel(QLabel *msgDis);
    void getProgressBar(QProgressBar *proBar);
    void getAppParam(int *appID, QString *appName, QString *appVersion, int *appSize);
    void getErrMsg(QString *errMsg);
    void sleep(unsigned int msec);
    bool sync(QString filePath);
    void initMsgPorBar(int maximum);
    int checkARM(int timeOut);

private:
    //�����������
    QProgressBar *msgProBar;
    //������״̬��
    QLabel *msgDis;
    //����ͨѶQextSerialPort�����ַ
    QextSerialPort *serialPort;

    int *appID;
    QString *appName;
    QString *appVersion;
    int *appSize;
    QString *errMsg;
    int cpuType;//1��ARM7��2��ARM9
    int mergeType;//1:merge file,0:no merge file
    int arm7Len;
    int arm9Len;
};

#endif // TOOLS_H
