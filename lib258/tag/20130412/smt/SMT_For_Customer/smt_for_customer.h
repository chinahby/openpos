#ifndef SMT_FOR_CUSTOMER_H
#define SMT_FOR_CUSTOMER_H

#include <QMainWindow>
#include <QTimer>
#include "tools.h"

#define     SMT_VERSION     "SMT For Customer 1.0.2"
/*****************************************************************
  SMT For Customer 1.0.2 update at 2013.03.18 by garvey
  1.�޸Ļ�ȡdwn�ļ�cpu����ʱ���cpu����Ϊ��,��Ĭ��Ϊ1

  SMT For Customer 1.0.1 update at 2013.03.01 by garvey
  1.�޸���������
  2.Ĭ�������,dwn�ļ�Ϊcpu����ΪARM7��ʽ

  SMT For Customer 1.0.0 update at 2013.02.28 by garvey
  1.Ϊ�ͻ��ṩӦ�����ع���

*****************************************************************/

namespace Ui {
class SMT_For_Customer;
}

class SMT_For_Customer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SMT_For_Customer(QWidget *parent = 0);
    ~SMT_For_Customer();

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


    //ͬ������
    void on_actionSync_triggered();


    void on_pushButtonAppPath_clicked();

    void on_comboBoxCom_currentIndexChanged(const QString &arg1);

private:
    void clear(void);


private:
    Ui::SMT_For_Customer *ui;
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
    QextSerialPort *serialPort;

    int appID;
    QString appName;
    QString appVersion;
    int appSize;
    QString appPath;
    QString errMsg;
    bool lock;
};

#endif // SMT_FOR_CUSTOMER_H
