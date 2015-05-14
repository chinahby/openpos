#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include <QString>
#include <QTimer>
#include <QStringList>
#include <QFileInfo>
#include <QListWidget>
#include <QTime>
#include <QProgressBar>
#include <QLabel>
#include <QTableWidget>
#include <QTreeWidget>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include "cappiteminfo.h"
#include "csysiteminfo.h"
#include "qextserialport.h"

#define BUF_MAX_SIZE            10240              //��������ȡbuf

#define DOWNCMD_REQUEST		0x01	//����
#define DOWNCMD_RESPFILE	0x02	//�ļ�
#define DOWNCMD_RESPKEY		0x03	//������ֻ������״̬�²��а���

#define TOOLS_ERROR_INITVAL     1   //�������ʼֵ,��ֵ��ʾû�д���Ҳû�гɹ�
#define TOOLS_ERROR_SUCCESS     0   //�ɹ�
#define TOOLS_ERROR_TIMEOUT     -1  //��ʱ
#define TOOLS_ERROR_SIGNERR     -2  //ǩ������

// ��Э�� 14λ
// 02+downcmd(1)+data+03+lrc
#define SPD_STX 0x02
#define SPD_ETX 0x03

#define REQUEST_FOLDERTYPE_APP			0x31	//Ӧ�ó���
#define REQUEST_FOLDERTYPE_DRIVER		0x32	//����
#define REQUEST_FOLDERTYPE_LIB			0x33	//��
#define REQUEST_FOLDERTYPE_FONT			0x34	//����
#define REQUEST_FOLDERTYPE_APPMANAGER	0x35	//os
#define REQUEST_FOLDERTYPE_UBOOT    	0x36	//uboot
#define REQUEST_FOLDERTYPE_KERNEL   	0x37	//kernel

#define REQUEST_FILETYPE_CONFIG			0x31	//ϵͳ����������ļ�
#define	REQUEST_FILETYPE_SETTING		0x32	//Ӧ�ü���������ļ�
#define REQUEST_FILETYPE_BIN			0x33	//�����ļ�
#define REQUEST_FILETYPE_BINLOGO		0x34	//����ͼ�꼰UI�ű�
#define REQUEST_FILETYPE_BINIMG			0x35	//�������õ���ͼƬ
#define REQUEST_FILETYPE_DAEMON         0x36	//ϵͳ�����ļ�
#define REQUEST_FILETYPE_SCRIPT			0x37	//Ӧ�ó���UI�ű��ļ�

#define PROCESS_ADD 	0x30	//����
#define PROCESS_UPDATE	0x31	//����
#define PROCESS_COVER	0x32	//����
#define PROCESS_DEL		0x33	//ɾ��

#define PREV    0
#define NEXT    1

//��������ѹ��������
#define UPDATE_PACK_NAME        "update.dta"        //���°��ļ���
#define UPDATE_PACK_PATH        "tmp/update.dta"    //���°��ļ�·��
#define UPDATE_PACK_TEMP_PATH   "tmp/update.dta.zip"//���°���ʱ�ļ�·��
#define UPDATE_DATA_NAME        "date.dta"          //���յ����ݰ�����
#define UPDATE_DATA_TEMP_NAME    "~date.dta"        //���ݰ���ʱ�洢����
#define UPDATE_TOOL_NAME        "pack.dta"          //�����������
#define UPDATE_SCHEME_PATH      "SMT_SCHEME/"       //��õİ��Ĵ��·��
#define UPDATE_SCHEME_DATA_PATH "SMT_SCHEME/update.dtas.zip"  //dtas��ʱ��
//����
#define INI_FILE_CONFIG         "conf.ini"          //��׼�����ļ�����
#define SH_FILE_DRIVERS         "drivers.sh"        //����SHELL�ļ�����
#define SH_FILE_DAEMON          "daemon.sh"         //DAEMON��SHELL�ļ�����
#define INI_FILE_DAEMON_SET     "setting.ini"       //DAEMON�������ļ�����
#define INI_FILE_DAEMON_LIST    "list.ini"          //DAEMON�������б��ļ�����
#define INI_FILE_DESC           "desc.ini"          //SYS���ļ��ļ����������

#define INI_FILE_CODE           "UTF-8"             //ini�ļ������ʽ
#define UPDATESERVICE_VERSION   14                  //updateService�汾����Ҫ���ڴ˰汾�Ų���ʹ��

#define INI_TYPE_PLAN           30                  //pln����


class Tools
{
public:
    Tools();

    /*
     * @fn unsigned char OSSPD_tool_lrc(unsigned char *pointChar, int length)
     * @brief �����ַ���pointChar��LRCֵ
     *
     * @param pointChar ��Ҫ����lrc���ַ�����ַ
     * @param length	�ַ����ĳ���
     *
     * @return �����޷����ַ�lrc��ֵ
     *
     */
    unsigned char OSSPD_tool_lrc(unsigned char *pointChar, int length);

    /*
     * @fn int checkPakHeader(char *cmdPonitStr, int *length, QextSerialPort *serialPort, int timeOut)
     * @brief �ж��ַ����Ƿ�ΪͨѶЭ��ͷ
     *
     * @param cmdPonitStr   ��Ҫ�жϵ�Э��ͷ�ַ����ĵ�ַ
     * @param length        �ַ����ĳ��ȵĵ�ַ
     * @Param timeOut       ��ʱ����
     *
     * @return  �����Э��ͷ����0,updateService�汾���ͷ���-1,�������󷵻�-2
     *
     */
    int checkPakHeader(char *cmdPonitStr, int *length, int timeOut);

    /*
     * @fn int processRecvStream(unsigned char *recvBuf, int recvLength, QextSerialPort *serialPort)
     * @brief ���ղ�����Э����
     *
     * @param recvBuf       �Ѿ����յ�Э���ַ����ĵ�ַ,���ַ����Ѿ�ͨ����isCmdHead()�������ж�
     * @param recvLength    �ַ����ĳ���
     * @param serialPort    ���ڲ������ʵ����ַ��ͨ����ʵ���ɶ�д�������ݲ�������
     *
     * @return  ����ɹ����շ���ʵ�����ݵĳ���(������Э��ͷ��У��λ,�����ļ���ʵ�ʴ�С),���򷵻�-1
     *
     */
    int processRecvStream(unsigned char *recvBuf, int recvLength);

    /*
     * @fn int packgeCommand(int cmdType, QString name, unsigned char cmdArray[][2], int nCmd, QextSerialPort *serialPort)
     * @brief ����POS���������ͷ��͸�POS�������ļ����д������
     *
     * @param cmdType       �������������,��Ϊ����(0x01),�ļ�(0x02),�����µİ���(0x03)
     * @param name          ������������,�������APP��ϸ��Ϣ,nameΪAPPID,�������ΪNULL;�����������ļ�,nameΪ�ļ���·����
     * @param cmdArray      ����Ķ�ά����,�����ϸ�������
     * @param nCmd          ����ĸ���
     * @param serialPort    ���ڲ������ʵ����ַ��ͨ����ʵ���ɶ�д�������ݲ�������
     *
     * @return  ����ɹ����ͷ���ʵ�����ݵĳ���(������Э��ͷ��У��λ,�����ļ���ʵ�ʴ�С),���򷵻�-1
     *
     */
    int packgeCommand(int cmdType, QString name, unsigned char cmdArray[][2], int nCmd);

    /*
     * @fn void deleteDirectory(QFileInfo fileList)
     * @brief �ݹ�ɾ���ļ��м��ļ����µ��ļ�
     *
     * @param fileList  Ҫɾ���ļ��е�QFileInfo����ʵ��
     *
     * @return  �޷�������
     *
     */
    void deleteDirectory(QFileInfo fileList);

    /*
     * @fn bool addItemToListWidget(void *item, int type)
     * @brief ���item��listWidget��
     *
     * @param item    ��Ҫ��ӵ�item�ĵ�ַ,APPΪCAPPItemInfo����,����ΪCSYSItemInfo
     * @param type    item������,���Ϳ���ΪAPPLICATION,DAEMON,LIB,DRIVER,FONT,DATA,SHELL,UBOOT,KERNEL,CRT,CRAMFS,OTHER_BOOT
     *
     * @return  �ɹ���ӷ�����,���򷵻ؼ�
     *
     */
    bool addItemToListWidget(void *item, int type);

    /*
     * @fn void sortingFileDir(CAPPItemInfo *item, QString filePath)
     * @brief ���������POS�����յ���APP��Ϣ�������д���
     *
     * @param item      CAPPItemInfo����Ӧ�õ�ַ
     * @param filePath  �������ļ�·��
     *
     * @return  �޷���ֵ
     *
     */
    void sortingFileDir(void *item, QString filePath, int type=0);

    /*
     * @fn void sleep(unsigned int msec)
     * @brief ����msec��,�����߹����д����ܹ���Ӧ�ⲿ�¼�
     *
     * @param msec  ����msec��
     *
     * @return  �޷���ֵ
     *
     */
    void sleep(unsigned int msec);

    /*
     * @fn bool updateItemOnListWidget(int index)
     * @brief ����QListWidget�ϵ�APPӦ����Ϣ,��Ҫ��������ͼƬ��Ϣ
     *
     * @param index     ��Ҫ����APPӦ����Ϣ������ֵ
     *
     * @return  �ɹ����·�����,���򷵻ؼ�
     *
     */
    bool updateItemOnListWidget(int index);

    /*
     * @fn void getProgressBar(QProgressBar *proBar)
     * @brief ��ȡ������Ľ�������ַ
     *
     * @param proBar    �������ؼ���ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getProgressBar(QProgressBar *proBar);

    /*
     * @fn void getStatusLabel(QLabel *msgDis, QLabel *msgLable)
     * @brief ��ȡ�������״̬��ʾLabel��ַ
     *
     * @param msgDis    ״̬��ʾLabel��ַ
     * @param msgLable  ͨѶ״̬��ʾLabel��ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getStatusLabel(QLabel *msgDis, QLabel *msgLable);

    /*
     * @fn void getTimer(QTimer *readTimer)
     * @brief ��ȡ������Ķ��Ĵ���
     *
     * @param readTimer    ������Ķ��Ĵ�����ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getTimer(QTimer *readTimer);

    /*
     * @fn void getTransType(int *transType)
     * @brief ��ȡ�������ͨѶ��ʽ
     *
     * @param transType    �������ͨѶ��ʽ,0��ʾ����,1��ʾ����
     *
     * @return  �޷���ֵ
     *
     */
    void getTransType(int *transType);

    /*
     * @fn void getSocket(QTcpSocket *tcpSocket)
     * @brief ��ȡ�������QTcpSocket�����ַ
     *
     * @param tcpSocket    �������QTcpSocket�����ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getSocket(QTcpSocket *tcpSocket);

    /*
     * @fn void getItemAppList(QList<CAPPItemInfo> *itemAppList, QList<CAPPItemInfo> *itemAppDelList)
     * @brief ��ȡ�������APPӦ�õ���Ϣ�洢�б��ɾ����Ϣ�洢�б��ַ
     *
     * @param itemAppList       APPӦ�õ���Ϣ�洢�б��ַ
     * @param itemAppDelList    APPӦ�õ�ɾ����Ϣ�洢�б��ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getItemAppList(QList<CAPPItemInfo> *itemAppList, QList<CAPPItemInfo> *itemAppDelList);

    /*
     * @fn void getItemSysList(QList<CSYSItemInfo> *itemSysList, QList<CSYSItemInfo> *itemSysDelList)
     * @brief ��ȡ�������SYSӦ�õ���Ϣ�洢�б��ɾ����Ϣ�洢�б��ַ
     *
     * @param itemSysList       SYSӦ�õ���Ϣ�洢�б��ַ
     * @param itemSysDelList    SYSӦ�õ�ɾ����Ϣ�洢�б��ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getItemSysList(QList<CSYSItemInfo> *itemSysList, QList<CSYSItemInfo> *itemSysDelList);

    /*
     * @fn void getListWidget(QListWidget *appListWidget, QListWidget *sysListWidget, QListWidget *imageListWidget)
     * @brief ��ȡ�������appListWidget,sysListWidget,imageListWidget�ؼ��ĵ�ַ
     *
     * @param appListWidget     APPӦ�õĴ���洢������ַ
     * @param sysListWidget     SYSӦ�õĴ���洢������ַ
     * @param imageListWidget   APPͼƬ�Ĵ���洢������ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getListWidget(QListWidget *appListWidget, QListWidget *sysListWidget, QListWidget *imageListWidget);

    /*
     * @fn void getTableWidget(QTableWidget *tableWidget)
     * @brief ��ȡ�������tableWidget�ؼ��ĵ�ַ
     *
     * @param tableWidget   ��ʾSYS�б����ݵĿؼ�������ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getTableWidget(QTableWidget *tableWidget);

    /*
     * @fn void getSerialPort(QextSerialPort *serialPort)
     * @brief ��ȡ�������QextSerialPort����ĵ�ַ,���ڶԴ��ڽ��ж�д����
     *
     * @param serialPort    ������QextSerialPort����ĵ�ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getSerialPort(QextSerialPort *serialPort);

    /*
     * @fn void getDaemonSet(QList<DAEMON_SET> *daemonSet)
     * @brief ��ȡ�������DAEMON_SET���ͱ����ĵ�ַ,�˱������ڼ�¼setting.ini�ļ���DAEMON��������Ϣ
     *
     * @param daemonSet DAEMON_SET���ͱ����ĵ�ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getDaemonSet(QList<DAEMON_SET> *daemonSet);

    /*
     * @fn void getTreeWidget(QTreeWidget *treeWidget)
     * @brief ��ȡ�������QTreeWidget��Ķ����ַ,�˶����ַ���ڼ�¼�û���ӵ�Ӧ�õ�scheme
     *
     * @param treeWidget    QTreeWidget��Ķ����ַ
     *
     * @return  �޷���ֵ
     *
     */
    void getTreeWidget(QTreeWidget *treeWidget);

    /*
     * @fn bool drawTable(int type)
     * @brief ����Ӧ�����ͻ��Ʋ�ͬ�ı����ʾItem����Ϣ
     *
     * @param type  Ӧ�õ�����
     *
     * @return  �ɹ����Ʒ�����,���򷵻ؼ�
     *
     */
    bool drawTable(int type);

    /*
     * @fn bool creatIni(QString iniName, int itemType, void *item = NULL)
     * @brief ���������ļ�,�����ļ����ݸ���item��Ϣ����
     *

     * @param itemType      Ӧ������
     * @param iniName       �����ļ�·��������
     * @param item          Ӧ����Ϣ������ַ
     *
     * @return  �ɹ�������,���򷵻ؼ�
     *
     */
    bool creatIni(QString iniName, int itemType, void *item = NULL);

    /*
     * @fn bool creatShell(QString shellName, int itemType)
     * @brief ����SHELL�ļ�,����Item���͵Ĳ�ͬ������Ӧ��SHELL�ļ�
     *
     * @param shellName     SHELL�ļ�����
     * @param itemType      Ӧ������
     *
     * @return  �ɹ�������,���򷵻ؼ�
     *
     */
    bool creatShell(QString shellName, int itemType);

    /*
     * @fn bool updateAllItem(void)
     * @brief ��������Ӧ�õ�POS
     *
     * @param �޲���
     *
     * @return  �ɹ�������,���򷵻ؼ�
     *
     */
    bool updateAllItem(void);

    /*
     * @fn bool updateItemSortInfo(void)
     * @brief ��������APPӦ�õ�������Ϣ�����䷢�͵�POS
     *
     * @param �޲���
     *
     * @return  �ɹ�������,���򷵻ؼ�
     *
     */
    bool updateItemSortInfo(void);

    /*
     * @fn void updateInsmodList(int itemType)
     * @brief ����insmod�б���ӦӦ�õ���Ϣ�б���
     *
     * @param itemType  Ӧ������
     *
     * @return  �޷���ֵ
     *
     */
    void updateInsmodList(int itemType);

    /*
     * @fn void creatDelFile(QString path, int itemType)
     * @brief ������Ҫɾ����SYSӦ�õ��ļ�
     *
     * @param path      ɾ�����ļ��ı���·��
     * @param itemType  Ӧ������
     *
     * @return  �޷���ֵ
     *
     */
    void creatDelFile(QString path, int itemType);

    /*
     * @fn bool creatRSAFile(CSYSItemInfo *item)
     * @brief ����RSA�ļ�
     *
     * @param item      ��Ҫ����RSA�ļ���Ӧ����Ϣ�����ַ
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool creatRSAFile(CSYSItemInfo *item);

    /*
     * @fn void rsaSignData(unsigned char *srcData, int length, unsigned char *rsa_signdata)
     * @brief ����Ҫǩ���������ļ���������ǩ��
     *
     * @param srcData       ��Ҫǩ����Դ���ݵ�ַ
     * @param length        Դ���ݳ���
     * @param rsa_signdata  ǩ����ɺ����ݴ洢��ַ
     *
     * @return  �޷���ֵ
     *
     */
    void rsaSignData(unsigned char *srcData, int length, unsigned char *rsa_signdata);

    /*
     * @fn void initMsgPorBar(int maximum)
     * @brief ��ʼ��������,���������������ֵ����Ϊmaximum
     *
     * @param maximum   �����������ֵ
     *
     * @return  �޷���ֵ
     *
     */
    void initMsgPorBar(int maximum);

    /*
     * @fn bool updateOtherBoot(void)
     * @brief ����s19�ļ����ն�
     *
     * @param �޲���
     *
     * @return  �ɹ�������,���򷵻ؼ�
     *
     */
    bool updateOtherBoot(void);

    /*
     * @fn int packData(unsigned char sendBuf[], int sendBufLen, int packNum, unsigned char dataBuf[], int dataLen, int packType)
     * @brief ���������ͷ��Ϣ�����ݳ��ȣ������ݺ�У��λ������װ��������װ������ݵĳ���
     *
     * @param sendBuf       ����ú�Ĵ�ŵ�ַ
     * @param sendBufLen    ���Ͱ�����󳤶�
     * @param packNum       �����
     * @param dataBuf       Դ���ݵ�ַ
     * @param dataLen       Դ���ݵĳ���
     * @param packType      ������,0x01��ʾ������������,0x02��ʾ�����ļ�����
     *
     * @return  ������װ������ݵĳ���
     *
     */
    int packData(unsigned char sendBuf[], int sendBufLen, int packNum, unsigned char dataBuf[], int dataLen, int packType);

    /*
     * @fn bool waitReply(int timeOut, int *packNum, bool *replay)
     * @brief ���ذ����ͺ�Է��Ļظ����
     *
     * @param timeOut   ����Ӧ��ĳ�ʱ����
     * @param packNum   �Է����հ��İ���
     * @param replay    �Է����յİ�����ΪpackNum�Ľ��ս��,����Է�����okΪ��,����ngΪ��
     *
     * @return  ������յ��ظ���Ϣ������,���򷵻ؼ�
     *
     */
    bool waitReply(int timeOut, int *packNum, bool *replay);

    /*
     * @fn int getItemCount(int itemType, int listType = 0)
     * @brief ��ȡ�б���itemType����Ӧ�õĸ���
     *
     * @param itemType  Ӧ������
     * @param listType  Ӧ���б�����,Ĭ��Ϊ0,Ϊ1ʱ��ʾɾ���б�
     *
     * @return  �б���itemType����Ӧ�õĸ���
     *
     */
    int getItemCount(int itemType, int listType = 0);

    /*
     * @fn int getUpdateItemCount(int itemType)
     * @brief ��ȡ�б���itemType������Ҫ���ص�Ӧ�ø���
     *
     * @param itemType  Ӧ�õ�����
     *
     * @return  �б���itemType������Ҫ���ص�Ӧ�ø���
     *
     */
    int getUpdateItemCount(int itemType);

    /*
     * @fn bool modifyItemOnListWidget(void *item, int itemType)
     * @brief �޸�item������Ϣ���浽�б���
     *
     * @param item    ��Ҫ�޸ĵ�item�ĵ�ַ,APPΪCAPPItemInfo����,����ΪCSYSItemInfo
     * @param type    item������,���Ϳ���ΪAPPLICATION,DAEMON,LIB,DRIVER,FONT,DATA,SHELL,UBOOT,KERNEL,CRT,CRAMFS,OTHER_BOOT
     *
     * @return  �޸ĳɹ�������,���򷵻ؼ�
     *
     */
    bool modifyItemOnListWidget(void *item, int itemType);

    /*
     * @fn bool swapItemOnListWidget(int moveType)
     * @brief �ƶ�ѡ�е�Ӧ�ã��ṩ��ǰ����������ƶ����˺���ֻ�ʺ�APP���ƶ�
     *
     * @param moveType  Ӧ���ƶ����ͣ�PRVE��ǰ�ƶ���NEXT����ƶ�
     *
     * @return  �ɹ��ƶ�Ӧ�÷����棬���򷵻ؼ�
     *
     */
    bool swapItemOnListWidget(int moveType);

    /*
     * @fn bool removeItemOnListWidget(int itemType)
     * @brief ��listWidget���Ƴ�Ӧ�ã����Ӧ������POS����ɾ����Ӧ����Ϣ������ɾ���б���
     *
     * @param itemType  ��Ҫ�Ƴ���Ӧ�õ�����
     *
     * @return  �ɹ�ɾ��Ӧ�÷����棬���򷵻ؼ�
     *
     */
    bool removeItemOnListWidget(int itemType);

    /*
     * @fn bool getItemInfoFromPos(unsigned char cmdArray[][2], int nCmd)
     * @brief ��POS����ȡAPP�б���Ϣ��������Ϣ��ӵ�listWidget��
     *
     * @param cmdArray      ��POS�����͵������б���Ϣ
     * @param nCmd          ���������б�ĸ���
     *
     * @return  �ɹ���ȡӦ�÷����棬���򷵻ؼ�
     *
     */
    bool getItemInfoFromPos(unsigned char cmdArray[][2], int nCmd);

    /*
     * @fn bool unpack(unsigned char cmdArray[][2])
     * @brief �Խ��յ��İ����н�ѹ����������Ӧ��Ϣ��ӵ�QListWidget��
     *
     * @param cmdArray  �����б�
     *
     * @return  �ɹ������棬ʧ�ܷ��ؼ�
     *
     */
    bool unpack(unsigned char cmdArray[][2]);

    /*
     * @fn void readIni(int itemType, QString iniPath)
     * @brief ��ȡINI�ļ��е�������Ϣ
     *
     * @param itemType  Ӧ������
     * @param iniPath   INI�ļ�·��
     *
     * @return  �ɹ������棬ʧ�ܷ��ؼ�
     *
     */
    void readIni(int itemType, QString iniPath);

    /*
     * @fn bool updateItemToPos(int itemType, int index = -1, int updateType = 0)
     * @brief ����itemType���͵�Ӧ�õ�POS��
     *
     * @param itemType      Ӧ������
     * @param index         APPӦ��������,ֻ�е�itemType=0ʱ��������
     * @param updateType    ��������,Ĭ��0��ʾ���µ�POS��,����ֵ��ʾֻ�Ǵ������,ֻ�е�itemType=0ʱ��������
     *
     * @return  �޷���ֵ
     *
     */
    bool updateItemToPos(int itemType, int index = -1, int updateType = 0);

    /*
     * @fn bool addDtaToListWidget(QString dtaPath)
     * @brief ��DTA���е����ݽ�ѹ������ӵ�QListWidget��
     *
     * @param dtaPath   DTA�ļ�·��
     *
     * @return  �ɹ������棬ʧ�ܷ��ؼ�
     *
     */
    bool addDtaToListWidget(QString dtaPath);

    /*
     * @fn void addImageToListWidget(QString imagePath, QString *curIcoPath, QStringList *curLogoPath, QStringList *curDelPicture)
     * @brief ��ͼƬ��ӵ�ͼƬ��ʾ�����У���Ҫʹ����APP��ICO��Logo�ʹ�ӡͼƬ
     *
     * @param imagePath     ��Ҫ��ӵ�ͼƬ�ĵ�ַ
     * @param curIcoPath    ��ǰItem��ICOͼ���ַ
     * @param curLogoPath   ��ǰItem��LogoͼƬ��ַ�б�
     * @param curDelPicture ��ǰItem�Ѿ�ɾ����ͼƬ�����б�
     *
     * @return  �޷���ֵ
     *
     */
    void addImageToListWidget(QString imagePath, QString *curIcoPath, QStringList *curLogoPath, QStringList *curDelPicture);

    /*
     * @fn void showImage(QString imagePath)
     * @brief ����ͼƬ����,��������ӵ���Ӧ��λ�ò���ʾ
     *
     * @param imagePath     ��Ҫ��ӵ�ͼƬ�ĵ�ַ
     *
     * @return  �޷���ֵ
     *
     */
    void showImage(QString imagePath);

    /*
     * @fn void initTreeWidget(QString iniPath)
     * @brief ��ʼ��Scheme Tree Widget�ռ�
     *
     * @param iniPath   ������Scheme��Ϣ�������ļ�
     *
     * @return  �޷���ֵ
     *
     */
    void initTreeWidget(QString iniPath);

    /*
     * @fn bool saveScheme(char *iniName)
     * @brief ����Scheme��Ϣ
     *
     * @param iniName   schemeINI�ļ�����
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool saveScheme(char *iniName);

    /*
     * @fn bool addSchemeToWidget(QString iniPath)
     * @brief ���Scheme��Ϣ��QtreeWidget
     *
     * @param iniName   schemeINI�ļ�����
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool addSchemeToWidget(QString iniPath);

    /*
     * @fn bool removeScheme(QTreeWidgetItem *curTreeWdtItem)
     * @brief �Ƴ�Scheme��Ϣ��QtreeWidget
     *
     * @param iniName   schemeINI�ļ�����
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool removeScheme(QTreeWidgetItem *curTreeWdtItem);

    /*
     * @fn bool addItemFromScheme(QTreeWidgetItem *curTreeWdtItem)
     * @brief ��ָ��Scheme��Ϣ��ӵ�QListWidget
     *
     * @param curTreeWdtItem    Scheme��Ϣ����Ӧ��QTreeWidgetItem�����ַ
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool addItemFromScheme(QTreeWidgetItem *curTreeWdtItem);

    /*
     * @fn int analysisDesc(QString desc, QString *brief, QString *valueList)
     * @brief ����DAEMON��setting.ini�ļ��е���Ϣ,��������Ϣ����
     *
     * @param desc      �������������ַ���
     * @param brief     ѡ��ļ���
     * @param valueList ����ѡ���ֵ���б���Ϣ
     *
     * @return  0��ʾscan by carcode,1��ʾtext,2��ʾradio,3��ʾcheckbox,4��ʾjust for display,5��ʾpasswd
     *
     */
    int analysisDesc(QString desc, QString *brief, QString *valueList);

    /*
     * @fn int getSessionCount(void)
     * @brief ��ȡdaemonSet�еĲ�ͬ��session�ĸ���
     *
     * @param �޲���
     *
     * @return  ����daemonSet�еĲ�ͬ��session�ĸ���
     *
     */
    int getSessionCount(void);

    /*
     * @fn bool updateDaemonSet(void)
     * @brief ����daemonSet
     *
     * @param �޲���
     *
     * @return  �ɹ�������,ʧ�ܷ��ؼ�
     *
     */
    bool updateDaemonSet(void);

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

    /*
     * @fn QStringList getChildValues(QString sessionName, QString fileName)
     * @brief ��ȡchildSession�µ�valueֵ�б�
     *
     * @param sessionName   session����
     * @param fileName      �����ļ�����
     *
     * @return  ����childSession�µ�valueֵ�б�
     *
     */
    QStringList getChildValues(QString sessionName, QString fileName);

    /*
     * @fn int read(char *readBuf, int bufSize)
     * @brief ��ȡ���ڻ����������,���ض������ֽ���
     *
     * @param readBuf   ��ȡ�ַ�����buf
     * @param bufSize   buf��С
     *
     * @return  ���ض�ȡ���ֽ���
     *
     */
    int read(char *readBuf, int bufSize);

    /*
     * @fn int write(char *writeBuf, int bufSize)
     * @brief ������д�봮�ڻ�����,����ʵ��д����ֽ���
     *
     * @param writeBuf   Ҫд����ַ���
     * @param bufSize   buf��С
     *
     * @return  ����ʵ��д����ֽ���
     *
     */
    int write(char *writeBuf, int bufSize);

    /*
     * @fn bool checkTransPort(void)
     * @brief ��鴫��˿��Ƿ����,�˿ڷ�Ϊ���ں���������
     *
     * @return  �˿ڿ��÷�����,���򷵻ؼ�
     *
     */
    bool checkTransPort(void);

    /*
     * @fn bool openSerialPort(void)
     * @brief �򿪴���
     *
     * @return  �ɹ��򿪴��ڷ�����,���򷵻ؼ�
     *
     */
    bool openSerialPort(void);

    /*
     * @fn bool openSerialPort(void)
     * @brief �رմ���
     *
     * @return  �ɹ��رմ��ڷ�����,���򷵻ؼ�
     *
     */
    void closeSerialPort(void);
    //bool connectNetWork(void);
    //void disconnectNetWork(void);

    void formatKey(QString *keyBuf);
    bool releasePlnToUSB(QTreeWidgetItem *treeWidgetItem);
    bool packPln(QList<CAPPItemInfo> *itemList, QString schemeName);
    bool unpackPln(QString dtasPath);
    void repeatRsaSignData(unsigned char *srcData, int length, unsigned char offsetData[128], unsigned char *rsa_signdata);
    bool signApp(QString srcPath, QString signPath);

private:
    //�����������
    QProgressBar *msgProBar;
    //������״̬��
    QLabel *msgDis;
    //��ǰͨѶ״̬
    QLabel *msgLable;
    //����Ϣ��ʱ��
    QTimer *readTimer;
    //APPӦ����Ϣ�洢�б�
    QList<CAPPItemInfo> *itemAppList;
    //SYSӦ����Ϣ�洢�б�
    QList<CSYSItemInfo> *itemSysList;
    //APP�Ѿ�ɾ��Ӧ�õĴ洢�б�
    QList<CAPPItemInfo> *itemAppDelList;
    //SYS�Ѿ�ɾ��Ӧ�õĴ洢�б�
    QList<CSYSItemInfo> *itemSysDelList;
    //DAEMON��������Ϣ�洢�б�
    QList<DAEMON_SET>   *daemonSet;
    //������APPӦ����ʾ�ؼ���ַ
    QListWidget *appListWidget;
    //������SYSӦ����ʾ�ؼ���ַ
    QListWidget *sysListWidget;
    //������ͼƬ��ʾ�ؼ���ַ
    QListWidget *imageListWidget;
    //�ڲ�ICOͼ��·���б���Ϣ
    QStringList resourceIcoPath;
    //������SYSӦ����Ϣ�б���ʾ�ռ��ַ
    QTableWidget *tableWidget;
    //������SYSӦ����Ϣ�б���ʾ�ı���ͷ
    QStringList tableHeader;
    //���ڶ����Խ���ķ���
    QStringList tableDesc;
    //����ͨѶQextSerialPort�����ַ
    QextSerialPort *serialPort;
    //������scheme��Ϣ��ʾ�ؼ���ַ
    QTreeWidget *treeWidget;
    //scheme���Ѵ��ڵ���Ϣ�б�
    QStringList saveCfgList;
    //ͨѶ���ͱ�ʶ,0��ʾ����,1��ʾ����
    int *transType;
    //TcpSocket�����ַ
    QTcpSocket *tcpSocket;
    //scheme�ж���Ŀ¼QTreeWidgetItem
    QTreeWidgetItem *rootTreeWdtItem;
    //setting.ini�ļ��޸ı�־,δ�޸�Ϊ0���޸�Ϊ1
    int settingMoidfy;
    //����dta�ļ��ɹ���־
    int sendDtaSuccess;

};

#endif // TOOLS_H
