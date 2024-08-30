#include "officeopt.h"

OfficeOpt::OfficeOpt(QString appType, QWidget *parent, QString path):
   basePath(path + "/Depend/Save"),
   QAxWidget(parent)
{
//    QString dirStr = basePath +"/" + "1.xls";
//    this->setControl(dirStr);
//    this->show();
//    QAxWidget * mywidget = new QAxWidget("Excel.Application", ui->标签控件名);
//    mywidget->dynamicCall("SetVisible(bool Visible)", "false");
//    //隐藏不显示Office窗体
//    mywidget->setProperty("DisplayAlerts", false); //屏蔽Office的警告消息框
//    mywidget->setGeometry(ui->标签控件名->geometry().x(), ui->标签控件名->geometry().y(), 宽度, 高
//    度);
//    //设置显示区尺寸
//    mywidget->setControl(Excel文件名); //指定要打开的文件名
//    mywidget->show(); //显示内容AxWidget界面显示
}

bool OfficeOpt::createFile(QString name)
{
    QString dirStr = basePath +"/" + name;
    QFile file(dirStr);
    if(file.exists()){
        return false;
    }
    return file.resize(0);
}

bool OfficeOpt::roomFile(QString name)
{
    QString dirStr = basePath +"/" + name;
    QFile file(dirStr);
    return file.remove();
}

QList<QString> OfficeOpt::getDirAllFileName(QStringList AfterMost)
{
    QDir dir(basePath);
    QList<QFileInfo> infos = dir.entryInfoList(AfterMost);
    QList<QString> fileNames;
    for(QFileInfo& info:infos){
        fileNames.append(info.fileName());
    }
    return fileNames;
}
