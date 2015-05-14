#include "screensetting.h"
#include "ui_screensetting.h"

#include "modules.h"

ScreenSetting::ScreenSetting(QWidget *parent) :
    QWidget(parent,Qt::FramelessWindowHint),
    ui(new Ui::ScreenSetting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("./image/thirdback.png")));
    setPalette(palette);
    setAutoFillBackground(true);

    ui->pushButton->setFocus();

    getValue();
}

void ScreenSetting::getValue()
{
     QSettings set("/daemon/setting.ini",QSettings::IniFormat);
    set.beginGroup("SCREEN_SETTING");

    keyList = set.allKeys();
    qDebug()<<"keyList"<<keyList;

    {//1111111111
    if(keyList.count()==0)
        return;
    QString temp1 = keyList.at(0);
    temp1.remove(0,3);
    ui->label_3->setText(temp1);

    qDebug()<<"set.value(keyList.at(0)).toString()::"<<set.value(keyList.at(0)).toString();
    QString value1 = set.value(keyList.at(0)).toString();
    int idx1 = value1.indexOf("#");
    QString str1 = value1.right(value1.size()-value1.indexOf("#"));
    qDebug()<<"STR1 "<<str1<<value1.size()-value1.indexOf("#");
    QString brief;
    QString valueList;
    analysisDesc(str1,&brief,&valueList);

    QStringList vlist = valueList.split(";");
    qDebug()<<"000vlist"<<vlist;
    for(int i=0;i<vlist.count();i++)
    {
        QString text = vlist.at(i);
        qDebug()<<"333text"<<text;
        ui->comboBox->addItem(text.right(text.size()-2));
    }

    QString simValue = value1.left(idx1).simplified();
    ui->comboBox->setCurrentIndex(simValue.toInt()-1);

    noteList.append(str1);
    }

    {//2222222
    if(keyList.count()<2)
        return;
    QString temp2 = keyList.at(1);
    temp2.remove(0,3);
    ui->label->setText(temp2);

    QString value2 = set.value(keyList.at(1)).toString();
    int idx2 = value2.indexOf("#");
    QString str2 = value2.left(idx2).simplified();
    ui->lineEdit->setText(str2);

    qDebug()<<"value2 "<<value2<<value2.size()-idx2;
    noteList.append(value2.right(value2.size()-idx2));

    }

    {//two
     if(keyList.count()<3)
         return;
    QString temp3 = keyList.at(2);
    temp3.remove(0,3);
    ui->label_2->setText(temp3);

    QString value3 = set.value(keyList.at(2)).toString();
    int idx3 = value3.indexOf("#");
    QString str3 = value3.left(idx3).simplified();
    ui->lineEdit_2->setText(str3);

    noteList.append(value3.right(value3.size()-idx3));

    }
    set.endGroup();

}

void ScreenSetting::setValue()
{
     QSettings set("/daemon/setting.ini",QSettings::IniFormat);
     set.beginGroup("SCREEN_SETTING");

      if((!keyList.count())||(!noteList.count()))
          return;
      set.setValue(keyList.at(0),QString::number(ui->comboBox->currentIndex()+1)+ " "+noteList.at(0) );

      if((keyList.count()<2)||(noteList.count()<2))
          return;
      set.setValue(keyList.at(1),ui->lineEdit->text()+" "+noteList.at(1));

      if((keyList.count()<3)||(noteList.count()<3))
          return;
      set.setValue(keyList.at(2),ui->lineEdit_2->text()+" "+noteList.at(2));

      set.endGroup();

      if(ui->comboBox->currentIndex()==0)
          Os__lcd_bright(0x33);
      else if(ui->comboBox->currentIndex()==1)
          Os__lcd_bright(0x66);
      else if(ui->comboBox->currentIndex()==2)
          Os__lcd_bright(0x99);
      else if(ui->comboBox->currentIndex()==3)
          Os__lcd_bright(0xcc);
      else
          Os__lcd_bright(0xff);


}

int ScreenSetting::analysisDesc(QString desc, QString *brief, QString *valueList)
{
    int type = 0;
    //ȥ�����˿ո�
    QString strTemp = desc;
    QString valueTemp;
    QRegExp regExp("[0-9]{1,2}-");
    //�жϵ�һ���ַ��Ƿ�Ϊ#��
    if(strTemp.left(1).compare("#") != 0)
        return -1;
    //��#��ȥ��
    strTemp = strTemp.right(strTemp.size() - 1);
    //��ȡ���ͣ���#�ź����ֵ
    type = strTemp.left(strTemp.indexOf(" ")).toInt();
    //���Ѿ���ȡ���ַ���ȥ��
    if(strTemp.contains(" "))
        strTemp = strTemp.right(strTemp.size() - strTemp.indexOf(" ") -1);
    else
        strTemp = "";
    //�ж��Ƿ���|
    if(strTemp.contains("|"))
    {
        //|ǰ�����ݶ��Ǽ��
        *brief = strTemp.left(strTemp.indexOf("|") - 1);
        //ȥ��|ǰ������
        strTemp = strTemp.right(strTemp.size() - strTemp.indexOf("|") - 1);
        //ѭ���ж��Ƿ���������ʽ"[0-9]{1,2}-"
        while(strTemp.contains(regExp))
        {
            //ȥ�����˵Ŀո�
            strTemp = strTemp.simplified();
            //�ж��м��Ƿ��пո񣬶����ͬ��ֵ��Ҫ�ÿո����
            if(strTemp.count(regExp) > 1)
            {
                //��ȡ-��Ϳո�ǰ������
                valueTemp = strTemp.left(strTemp.indexOf(regExp,3) - 1);
                *valueList += valueTemp + ";";
                //ȥ���Ѿ���ȡ����Ϣ
                strTemp = strTemp.right(strTemp.size() - strTemp.indexOf(regExp,3));
            }
            else
            {
                //û�пո�˵�������һ��ֵ��ȡֵ���˳�
                valueTemp = strTemp;
                *valueList += valueTemp;
                break;
            }
        }
    }
    //û��|ʱ�������ݶ��Ǽ��
    else
        *brief = strTemp;

    return type;

}

ScreenSetting::~ScreenSetting()
{

    keyList.clear();
    noteList.clear();

    delete ui;
}

void ScreenSetting::on_pushButton_clicked()
{
    close();
}

void ScreenSetting::on_pushButton_2_clicked()
{
    setValue();
    system("sync");
    close();
}

//void ScreenSetting::on_pushButton_3_clicked()
//{
//    Os__lcd_bright(0);
//    sleep(1);
//    Os__lcd_bright(0x33);
//    sleep(1);

//      Os__lcd_bright(0x66);
//      sleep(1);
//      Os__lcd_bright(0x99);
//      sleep(1);

//}

void ScreenSetting::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    case Qt::Key_Escape:
    case Qt::Key_Cancel:
        close();
        break;
    }
}
