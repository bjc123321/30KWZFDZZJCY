#include "page4_stcs.h"
#include "ui_page4_stcs.h"
#include "Base/BaseFun/base.h"

#include "App/Data/dataserialcom.h"
#include "App/Data/dataprocessor.h"
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

       DataSerialCom::getInstance().type = DataSerialCom::SUDD_LOAD;
        //开始突加测试
       emit startSuddIncrease();
    });

    connect(ui->pushButton_2,&QPushButton::clicked,this,[this](){

        emit stopSuddLoad();

    });

    DataSerialCom &dtSerialCom = DataSerialCom::getInstance();
    DataProcessor &dtProcessor = DataProcessor::getInstance();

    connect(this,&Page4_stcs::startSuddIncrease,&dtSerialCom,&DataSerialCom::startSuddIncreaseSlot);
    connect(&dtSerialCom,&DataSerialCom::updateSuddLoadPageSignal,this,&Page4_stcs::displaySuddLoadView);


    connect(this,&Page4_stcs::drawSuddLoadPlot,&dtProcessor,&DataProcessor::generateData);



    connect(this,&Page4_stcs::stopSuddLoad,&dtSerialCom,&DataSerialCom::stopSuddLoadSlot);

}

//此函数要包括显示数据和画波形图
void Page4_stcs::displaySuddLoadView(QQueue<QString> dataStrQueue)
{


    qDebug()<<"显示突加数据在View";

    float threePhase[] = {0.00,0.00,0.00};
    for(int i = 0; i<dataStrQueue.length(); i++){

        threePhase[i] = dataStrQueue.dequeue().toFloat() ;

    }

    //计算平均的最大值和最小值



    qDebug()<<"平均突加电压:"<<threePhase[0]
            <<"\n平均突加电流:"<<threePhase[1]
            <<"\n平均突加频率:"<<threePhase[2];

    emit drawSuddLoadPlot(threePhase[0],threePhase[1],threePhase[2]);

}



Page4_stcs::~Page4_stcs()
{
    delete ui;
}




