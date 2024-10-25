#include "page4_stcs.h"
#include "ui_page4_stcs.h"
#include "Base/BaseFun/base.h"

#include "App/Data/dataserialcom.h"
#include "App/Data/dataprocessor.h"
#include "App/Page/save.h"
#include "GlobalSettings.h"


Page4_stcs::Page4_stcs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page4_stcs)
{
    ui->setupUi(this);
    init();
    signalBind();

}

void Page4_stcs::init()
{
//    frePlot = new FrequencyPlotter(this);
//    ui->horizontalLayout_2->addWidget(frePlot);

    threePlot = new ThreePhasePlot(this);
    ui->horizontalLayout_2->addWidget(threePlot);


}

void Page4_stcs::signalBind()
{

    connect(ui->pushButton,&QPushButton::clicked,this,[this](){

        //开始突加测试
       emit startSuddIncrease();
    });

    connect(ui->pushButton_2,&QPushButton::clicked,this,[this](){


        GlobalSettings::instance().setCurrentTestType(GlobalSettings::OTHERS);

        emit stopSuddLoad();

    });


    connect(ui->pushButton_5, &QPushButton::clicked, this, [&]() {
        QString tab_Name = ui->tabWidget->currentWidget()->objectName(); // 获取当前标签页名称
        qDebug() << "保存当前" << tab_Name << "页数据"; // 输出调试信息
        saveCurrentUIData(tab_Name); // 调用保存函数
    });


    DataSerialCom &dtSerialCom = DataSerialCom::getInstance();
    DataProcessor &dtProcessor = DataProcessor::getInstance();

    connect(this,&Page4_stcs::startSuddIncrease,&dtSerialCom,&DataSerialCom::startSuddIncreaseSlot);
    connect(&dtSerialCom,&DataSerialCom::updateSuddLoadPageSignal,this,&Page4_stcs::displaySuddLoadWaveSlot);
    connect(&dtSerialCom,&DataSerialCom::displayLCDNumberSignal,this,&Page4_stcs::displayClockSlot);


    connect(this,&Page4_stcs::drawSuddLoadPlot,&dtProcessor,&DataProcessor::generateData);

    connect(this,&Page4_stcs::stopSuddLoad,&dtSerialCom,&DataSerialCom::stopSuddLoadSlot);

    connect(this,&Page4_stcs::readSuddLoad800YSignal,&dtSerialCom,&DataSerialCom::readSuddLoad800YSlot);

}

void Page4_stcs::displayClockSlot(int t_elapsedTime)
{

    int tens = t_elapsedTime / 10;
    int ones = t_elapsedTime % 10;

    ui->lcdNumber->display(tens);
    ui->lcdNumber_2->display(ones);

}

//此函数要包括显示数据和画波形图
void Page4_stcs::displaySuddLoadWaveSlot(QQueue<QString> dataStrQueue)
{


    qDebug()<<"显示突加数据在View,收到的数据个数"<<dataStrQueue.length();

    float threePhase[] = {0.00,0.00,0.00};

    int i = 0;

    while(!dataStrQueue.isEmpty()){


            threePhase[i] = dataStrQueue.dequeue().toFloat() ;
            qDebug()<<"电压"<<(i+1)<<threePhase[i];

            i++;

    }

    //计算平均的最大值和最小

    emit drawSuddLoadPlot(threePhase[0],threePhase[1],threePhase[2]);

}

void Page4_stcs::saveCurrentUIData(QString dataType)
{

    if(dataType == "tab"){

        qDebug()<<"突加数据保存";

    }else if(dataType == "tab_2"){

        qDebug()<<"*****突加曲线数据保存*****";
        //1.先弹出save界面
        //2.发送查询800个突加数据信号(可以先查100个每隔8个查1次)
        //3.返回的响应帧解析后放队列中，再发给界面画出这些点形成的曲线
        saveSuddLoadData();


    }else if(dataType == "tab_3"){

        qDebug()<<"突卸数据保存";

    }else if(dataType == "tab_4"){

        qDebug()<<"*****突卸曲线数据保存*****";

    }

}

void Page4_stcs::saveSuddLoadData()
{
    Save::TEST_RECORD record(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz"),
                           "电机",
                           "突加测试",
                           "小王",
                           QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"),
                           "Yes");
    // 打印信息
    qDebug() << "检测编号:" << record.detectionID;
    qDebug() << "检测元件:" << record.detectionComponent;
    qDebug() << "类型:" << record.type;
    qDebug() << "检测人员:" << record.inspector;
    qDebug() << "检测时间:" << record.detectionTime;
    qDebug() << "检测结果:" << record.result;
    Save::U().displayT_dataView(record);
    Save::U().exec();


    emit readSuddLoad800YSignal();

}



Page4_stcs::~Page4_stcs()
{
    delete ui;
}




