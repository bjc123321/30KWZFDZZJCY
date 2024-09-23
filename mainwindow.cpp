#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "App/Data/database.h"

#include "Base/SaveInfo/SaveText/officeopt.h"
#include <QDebug>
#include "App/Data/datacom1.h"

#include <QMessageBox>
#include "App/Page/save.h"
#include "App/Page/page3_wtcs.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();


//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);//大屏时解开，无边框代码注释
//    this->showFullScreen();//大屏时解开，全屏代码注释
    this->showMaximized();
    ui->treeWidget->setIconSize(QSize(32,32));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    initNavTree();
    initClose();
}

void MainWindow::initNavTree()
{
    ui->stackedWidget->setCurrentIndex(-1);

    static QMap<QString,QString> treeMap = {
        {"负载设置","page1_fzsz"},
        {"加载试验","page2_jzsy"},
        {"稳态测试","page3_wtcs"},
        {"瞬态测试","page4_stcs"},
        {"谐波分析","page5_xbfx"},
        {"录波分析","page6_lbfx"},
        {"数据报表","data"},
        {"用户信息","user"},
        {"自检信息","selfInfo"},
        {"设备自检","selfTest"}
    };

    connect(ui->treeWidget,QTreeWidget::itemClicked,this,[&](QTreeWidgetItem *item, int column){
        QString text = item->text(column);
        if(treeMap.contains(text)){
            for(auto tnode : ui->stackedWidget->children()){
                if(treeMap[text] == tnode->objectName()){
                    qDebug()<<"此节点为:"<<tnode;
                    ui->stackedWidget->setCurrentWidget(qobject_cast<QWidget*>(tnode));
                    return;
                }
            }
        }
    });

    ui->treeWidget->expandAll();
}

void MainWindow::initClose()
{
    connect(ui->toolButton,QToolButton::clicked,this,[&](){
         qApp->quit();
    });
    connect(ui->pushButton_2,&QPushButton::clicked,this,[&]{
        qDebug()<<"点击了保存";
        Save::U().exec();
    });
}


void MainWindow::on_pushButton_clicked()
{
//    QByteArray data;
//    data[0] = 0x85;
//    data[1] = 0x32;
//    data[2] = 0x6B;
//    data[3] = 0x43;

//    qDebug() << ModbusRTU::floatData(data);

    DataCom1::U().startAbruptAdd();
}

