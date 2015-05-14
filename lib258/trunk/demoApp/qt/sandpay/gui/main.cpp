#include <QtGui>
//#include "widget.h"
#include <QtGui/QApplication>
#include "mainwindow.h"

#include <include.h>
#include <global.h>
#include <xdata.h>
#include "sand_main.h"


void manager(struct seven *EventInfo)
{
    return;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("./gui.qss");//gui.qss is a file writen by ourself,it can set the gui style .
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line = in.readAll();

        qApp->setStyleSheet(line);
    }

    file.close();

    MainWindow w(argc, argv);//main window
    w.hide();


	QTextCodec :: setCodecForTr( QTextCodec :: codecForName( "utf8" ));  //ΪQObject::tr�����ַ�����
	QTextCodec :: setCodecForLocale( QTextCodec :: codecForName( "utf8" ));  //�����linux�ϣ�����localΪgb18030���ܻᵼ�¶�ȡ	                                                                        //�������unicode�����ַ�������ȷ��ʾ
	QTextCodec ::setCodecForCStrings(QTextCodec :: codecForName( "utf8" ) );  //ΪQString�����ַ����룬Ĭ�������ΪLatin1

    main_entry_gui(argc, argv);
    OnEve_power_on();


    return a.exec();
}
}
