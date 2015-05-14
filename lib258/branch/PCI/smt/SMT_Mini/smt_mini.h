#ifndef SMT_MINI_H
#define SMT_MINI_H

#include <QMainWindow>
#include "tools.h"

#define     SMT_VERSION     "SMT_Mini 2.0.0"
/*****************************************************************
  SMT_Mini 2.0.0 update at 2013.03.15 by garvey
  1.�������ý���
  2.�������±�д
  3.�������¶���

  SMT_Mini 1.2.3 update at 2012.12.19 by garvey
  1.�޸ĺϲ�����

  SMT_Mini 1.2.2 update at 2012.12.19 by garvey
  1.�޸Ľ��沼��
  2.�޸Ŀ��ļ��ǵ���ϲ���ť��������

  SMT_Mini 1.2.1 update at 2012.12.17 by garvey
  1.���Ӷ�ɼ��DNW���ֱ��ѹ�����ɵ�dwn�ļ���֧��
  2.��Ӷ�bin�ļ����д��ѹ�������dwn�ļ��Ĺ���

  SMT_Mini 1.2 update at 2012.11.14 by garvey
  1.�Դ��䱨�İ�ͷ�Ͱ�β�������
  2.�޸��ܹ����ر�SMT���ܴ���ARM7��ARM9��APP
  3.�������汾���˳��˵�

  SMT_Mini 1.1 update at 2012.11.12 by garvey
  1.�����ȡdwn�ļ���ID�����Ʋ�׼ȷ������
  SMT_Mini 1.0 create at 2012.11.12 by Garvey


*****************************************************************/
namespace Ui {
class SMT_Mini;
}

class SMT_Mini : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SMT_Mini(QWidget *parent = 0);
    ~SMT_Mini();

private slots:
    //��ȡ�����е���Ϣ
    void onReadyRead();
    //�򿪴��ڻ�����
    void on_actionConnect_triggered();
    //�Ͽ����ڻ�����
    void on_actionDisconnect_triggered();
    //����汾
    void on_actionVersion_triggered();
    //�˳����
    void on_actionExit_triggered();
    //����������
    bool eventFilter(QObject *,QEvent *);
    //����
    void on_actionWipe_triggered();
    //����������Ϣ��HTML���Ը�ʽ������ʾ
    void insertHtmlToWidget();

    //ͬ������
    void on_actionSync_triggered();

    void on_actionCOM1_triggered();
    void on_actionCOM2_triggered();
    void on_actionCOM3_triggered();
    void on_actionCOM4_triggered();
    void on_actionCOM5_triggered();


    //ȷ��APPID����ֵ��ȷ
    void on_lineEditAppID_textEdited(const QString &arg1);

    //ȷ��APP Version����ֵ��ȷ
    void on_lineEditVersion_textEdited(const QString &arg1);

    void on_pushButtonArm7FileSelect_clicked();

    void on_pushButtonArm9FileSelect_clicked();

    void on_pushButtonMergeFileSavePath_clicked();

    void on_pushButtonDwnFileSavePath_clicked();

    void on_pushButtonBinFileSelect_clicked();

    void on_radioButtonTransmit_clicked();

    void on_radioButtonConvert_clicked();

    void on_radioButtonConvertAndTransmit_clicked();

    void on_tabMain_currentChanged(int index);

    void on_comboBoxCPUType_currentIndexChanged(int index);

    void on_radioButtonMerge_clicked();

    void on_radioButtonMergeTransmit_clicked();

    void on_pushButtonDwnSelectFilePath_clicked();

    void on_comboBoxAppName_currentIndexChanged(const QString &arg1);

private:
    void writeCommand(int key);
    void insertCmdChar(int key);
    //void resizeEvent(QResizeEvent *e);
    void clearAppInfo(int type);
    void setAppInfo(void);
    void resetActionComPortIco(void);
    void updateAppNameList(void);

private:
    Ui::SMT_Mini *ui;
    Tools tools;
    QLabel *msgPartL;
    QLabel *msgTty;
    QLabel *msgLable;
    QLabel *msgSpace;
    QLabel *msgPos;
    QLabel *msgPosType;
    QLabel *msgDis;
    QPalette pal;
    QProgressBar *msgProBar;
    QTimer *readTimer;
    QTimer *writeTimer;
    QextSerialPort *serialPort;
    QStringList strCmdList;
    QString strCmd;
    bool lock;

    //Ӧ������
    QString appName;
    //Ӧ��ID
    int appID;
    //Ӧ�ð汾
    int appVersion;
    //Ӧ��CPU����:1 ARM7,2 ARM9
    int cpuType;
    //bin�ļ�·��
    QString binPath;
    //ת����dwn�ļ�����
    QString dwnFileName;
    //ת����dwn�ļ�����·��
    QString dwnFilePath;
    //�ļ�ת�����������:0ֻת��,1ֻ����,2ת��������ٴ���
    int convertOption;

    //Arm7�ļ�·��
    QString arm7FilePath;
    //Arm7Ӧ������
    QString arm7AppName;
    //Arm7Ӧ��ID
    int arm7AppID;

    //Arm9�ļ�·��
    QString arm9FilePath;
    //Arm9Ӧ������
    QString arm9AppName;
    //Arm7Ӧ��ID
    int arm9AppID;

    //�ϲ����dwn�ļ�����·��
    QString mergeFilePath;
    //�ϲ����dwn�ļ�����
    QString mergeFileName;
    //�ϲ����ļ�����:0ֻ�ϲ��ļ�,1�ϲ������ļ����ٴ���
    int mergeOption;

    QString dwnTransmitPath;//dwn�ļ����ؽ������ļ�·��

    //����������Ϣ��ʾ
    QString errMsg;
};

#endif // SMT_MINI_H
