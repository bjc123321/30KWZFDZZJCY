#include "page3_wtcs.h"
#include "ui_page3_wtcs.h"
#include <QDebug>

Page3_wtcs::Page3_wtcs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page3_wtcs)
{
    ui->setupUi(this);

    freqPlot = new FrequencyPlotter(this);
    voltageWidget = new VoltagePlot(this);
    ui->horizontalLayout_9->addWidget(freqPlot);
    ui->horizontalLayout_11 ->addWidget(voltageWidget);
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true); // 保证一时间只有一个按钮处于选中状态
    buttonGroup->addButton(ui->pushButton_2);
    buttonGroup->addButton(ui->pushButton_3);
    buttonGroup->addButton(ui->pushButton_5);
    buttonGroup->addButton(ui->pushButton_6);
    signalBind();


}

void Page3_wtcs::signalBind()
{

    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        //切换到电压整定页面
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->pushButton_3,&QPushButton::clicked,this,[&](){
        //切换到频率整定页面
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->pushButton_5,&QPushButton::clicked,this,[&](){
        //切换到电压曲线页面
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->pushButton_6,&QPushButton::clicked,this,[&](){
        //切换到频率曲线页面
        ui->stackedWidget->setCurrentIndex(3);
    });

    // 信号槽连接
    connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this, [=](QAbstractButton *button) {
        for (auto btn : buttonGroup->buttons()) {
            if (btn == button) {
                btn->setStyleSheet("background-color: #41CD52;"); // 被点击的按钮变浅绿色
            } else {
                btn->setStyleSheet(""); // 其他按钮恢复默认颜色
            }
        }
    });
    connect(ui->pushButton,&QPushButton::clicked,this,[&](){

        qDebug()<<"导出稳态等数据,目前是空表";
        DataExportManager::getInstance().createStaticTestExcel();


    });

    connect(ui->stopButton,&QPushButton::clicked,this,[&](){

        qDebug()<<"整定波形停止";//停止电压和频率等波形
        freqPlot->stopTuningFreqTest();
        voltageWidget->stopFlucPlot();


    });
    connect(ui->startVolTageTuningBtn,&QPushButton::clicked,this,[](){
        qDebug()<<"启动整定电压";
    });
    connect(ui->startFrequencyTuningBtn,&QPushButton::clicked,this,[&](){
        qDebug()<<"启动频率整定";
        freqPlot->startTuningFreqTest();
    });
    connect(ui->pushButton_4,&QPushButton::clicked,this,[&](){

        qDebug()<<"启动波动实验";
        DataTest::U().Fluctuation_Test();
        voltageWidget->startFlucPlot();

    });
    connect(&DataTest::U(),&DataTest::signal_setValue,this,
            [&](ResponseData reponseData){

        //设置界面上控件的显示
        QString floatStr = QString::number(reponseData.value, 'f', 2);
        qDebug()<<"返回的数据:"<<reponseData.data.toHex()<<"文本显示的浮点数为："<<floatStr;
    });



}

void Page3_wtcs::displayData()
{
    qDebug()<<"文本显示的浮点数为：";
}



Page3_wtcs::~Page3_wtcs()
{
    delete ui;
}
