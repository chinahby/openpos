#ifndef SMTMAINWINDOW_H
#define SMTMAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include "cappiteminfo.h"
#include "csysiteminfo.h"
#include "qextserialport.h"
#include "sessionoptions.h"
#include "codegenerator.h"
#include "tools.h"

#define SMT_VERSION     "SMT_1.5.8"

/**************************************
    SMT 1.5.8 update at 2013.03.21 by garvey
    1.�޸�write��read����,����SMT�������������������
    2.�޸�ĳһ����ƻ��б��ļ�������ʱɾ����,�ƻ������б���������10����ini�ļ���������
    3.��ӱ�����ʾ��Ϣ,���̧ͷ����״̬����Ϣ
    4.����������ӶϿ������ͨ��SMT��������

    SMT 1.5.7 update at 2013.02.02 by garvey
    1.�޸�daemon.sh�ļ��н��̵��Ա�־
    2.��Ӹ���ͬ�����ʱ��ʾ����
    3.����updateService��֤�汾��1.4

    SMT 1.5.6 update at 2013.01.11 by garvey
    1.��pln�ļ��е�conf.ini���UPDATE��־

    SMT 1.5.5 update at 2012.12.26 by Garvey
    1.�������USB���°�".pln"�ļ��Ĺ���
    2.�Խ��������ЩСbug�����޸�

    SMT 1.5.4 update at 2012.12.18 by Garvey
    1.������ͼƬ��Ӧ�����ô���
    2.���UI�ű�ȡ���ٴε�ѡ��ű�·��û�б����bug

    SMT 1.5.3 update at 2012.12.13 by Garvey
    1.�޸�ͨ��SCHEME�����ɾ����Ӧ�ú�,POS����Ӧ�ñ���ɾ����bug
    2.���������������ʱ,SMT��Ȼ��ʾδ���ӵ�bug
    3.���SMT.exe�ļ���icoͼ��

    SMT_1.5.2 update at 2012.12.05 by Garvey
    1.�޸����Ľ���ʱ������ȷ��ʾ������������
    2.�޸�desc.ini�ļ�

    SMT_1.5.1 update at 2012.12.03 by Garvey
    1.�޸�SCHEME��·����������ʱ������ȷ��ӵ�����

    SMT 1.5.0 update at 2012.11.12 by Garvey
    1.���SMT�������ع���
    2.�Ż��жϰ�ͷ�ļ�����,��߲��񱨵�����
    3.��������л���,֧����Ӣ��

    SMT 1.4.6 update at 2012.11.09 by Garvey
    1.�޸����ô��ڵ��ȡ���ر�����SMT��bug
    2.�޸��˻�ȡDAEMON��Ϣ��ȫ��bug
    3.��Ӷ�setting.ini�ļ��г�������ѡ��ʱ�Ĵ���

    SMT 1.4.5 update at 2012.11.07 by Garvey
    1.�޸ĳ�������ʱEDIT��־,����Ϊ0,����Ϊ1,����Ϊ2,ɾ��Ϊ3
    2.�޸�imageȫ��ɾ��ʱPOS��ͼƬ����ɾ����BUG

    SMT_1.4.4 update at 2012.10.30 by Garvey
    1.�������ʱУ��updateService�汾
    2.��UPDATE_SCHEME_PATH�궨���Ϊ"SMT_SCHEME/",Ϊ�˽����ѹ��·���к���sc��������

    SMT_1.4.3 update at 2012.10.30 by Garvey
    1.��SYSѡ������SHELL�ļ�ʱ���طָ��ߺ�apply��ť;
    2.����tab����
    3.�޸�daemon.sh��drivers.sh�е��ļ�����

    SMT1.4.2 update at 2012.10.26 by Garvey
    1.����APP����ȡӦ����ϸ��Ϣʱͬ�����Ը���Ӧ�ò�����Ϣ
    2.��ȡAPP��ϸ��Ϣ���ٻ�ȡӦ��.bin�ļ�
    3.UI�ű���ͼƬɾ��ʱ��¼ɾ������Ϣ,ͬ��ʱ��ɾ���ļ�ͷ��~

    SMT1.4.1 update at 2012.10.09 by Garvey
    1.����ʱ�����һ��˵�

    SMT1.4 update log at 2012.9.26
    1.�Ż��˴�����պ���
    2.����˲�ͬ����ϵͳ������������
    3.����INI�������ļ�ȫ��ͳһʹ��UTF-8����
**************************************/

namespace Ui {
class SMTMainWindow;
}

class SMTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SMTMainWindow(QWidget *parent = 0);
    ~SMTMainWindow();

private slots:
    //��ȡ�����е���Ϣ
    void onReadyRead();
    //�򿪴��ڻ�����
    void on_actionConnect_triggered();
    //�Ͽ����ڻ�����
    void on_actionDisconnect_triggered();
    //����������
    bool eventFilter(QObject *,QEvent *);
    //�����ô���
    void on_actionConfig_triggered();
    //����Ӧ�õ�POS
    void on_actionUpdate_triggered();
    //SMT��POS��������,��ȡPOS����Ϣ
    void on_actionShake_triggered();
    //����
    void on_actionWip_triggered();
    //�л�������ģʽ
    void on_actionNetWork_Server_triggered();
    //�л��ɴ���ģʽ
    void on_actionSerial_Port_triggered();
    //����
    void on_actionArm_GCC_triggered();
    //��ʾ����汾
    void on_actionVersion_triggered();
    //�˳����
    void on_actionExit_triggered();
    //����������Ϣ��HTML���Ը�ʽ������ʾ
    void insertHtmlToWidget();
    //��Դ�б��е���������һ��Ŀ¼
    void on_treeResource_itemClicked(QTreeWidgetItem *item, int column);
    //��Դ�б���˫�����Ӧ��
    void on_treeResource_itemDoubleClicked(QTreeWidgetItem *item, int column);
    //����APP��ʾ�ؼ��е�item��ʾitem����ϸ��Ϣ
    void on_listWgtApplication_itemClicked(QListWidgetItem *item);
    //����SYS��ʾ�ؼ��е�item��ʾitem����ϸ��Ϣ
    void on_listWgtSystem_itemClicked(QListWidgetItem *item);
    //APPǰ��
    void prevMoveItem();
    //APP����
    void nextMoveItem();
    //�Ƴ�item
    void removeItem();
    //�Ƴ���Դ�е�item
    void removeTreeItem();
    //ˢ����Դ�е�item
    void refreshTreeItem();
    //����APP�е�ָ��item,���DTA���洢��schemĿ¼��
    void releaseItem();
    //����SCHEME�е�APP����USB����,�����DTAS��
    void releasePln();
    //����item���޸�
    void on_btnApply_clicked();
    //ȷ��APPID����ֵ��ȷ
    void on_editAppID_textEdited(const QString &arg1);
    //˫��item��ȡPOS���еĸ���ϸ����Ϣ
    void on_listWgtApplication_itemDoubleClicked(QListWidgetItem *item);
    //˫��ͼƬɾ��
    void on_listWgtImage_itemDoubleClicked(QListWidgetItem *item);
    //����scheme�����üƻ�
    void on_btnCreateScheme_clicked();
    //˫��scheme�е�item��ӵ����пؼ���
    void on_treeScheme_itemDoubleClicked(QTreeWidgetItem *item, int column);

    //UI�ű�״̬�����ı�
    void on_chkBoxUIScript_stateChanged(int arg1);

    //����һ��QTcpSocket������
    void creatNewConnect(void);
    //����Ͽ�
    void disconnectNetwork(void);
    //ɾ����ѡ��Ӧ�� δ���
    //void removeMultipleItems(void);

private:
    void writeCommand(int key);
    void insertCmdChar(int key);
    void resizeEvent(QResizeEvent *e);
    void showItemInfo(int type);
    void clear(void);
    void clearAll(void);
    void contextMenuEvent(QContextMenuEvent * event);
    
private:
    Ui::SMTMainWindow *ui;
    QTimer *readTimer;
    QTimer *writeTimer;
    QextSerialPort *serialPort;
    QStringList strCmdList;
    QString strCmd;
    Tools tools;
    char tempstr[20];
    QLabel *msgPartL;
    QLabel *msgTty;
    QLabel *msgLable;
    QLabel *msgSpace;
    QLabel *msgPos;
    QLabel *msgPosType;
    QLabel *msgDis;
    QPalette pal;
    QProgressBar *msgProBar;
    QList<CAPPItemInfo> itemAPPInfo;
    QList<CSYSItemInfo> itemSYSInfo;
    QList<CAPPItemInfo> itemDelList;
    QList<CSYSItemInfo> itemSysDelList;
    QList<DAEMON_SET>   daemonSet;
    QString curIcoPath;
    QStringList curLogoPath;
    QStringList curDelPicture;
    QString curUIScriptPath;
    QString curDelUIScriptName;
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    int appIndex;
    int sysIndex;
    bool lock;
    int transType;
    bool shakeFlag;
};

#endif // SMTMAINWINDOW_H
