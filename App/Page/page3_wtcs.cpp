    #include "page3_wtcs.h"
#include "ui_page3_wtcs.h"
#include <QDebug>
#include <QDateTime>
#include "App/Data/dataserialcom.h"
#include "App/Page/save.h"
#include "globalsettings.h"

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


    initSteadyUI();
    initTuningUI();




    signalBind();


}

void Page3_wtcs::signalBind()
{

    DataSerialCom& dtSerialCom = DataSerialCom::getInstance();

    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        //切换到电压整定数据页面
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->pushButton_3,&QPushButton::clicked,this,[&](){
        //切换到频率整定数据页面
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->pushButton_6,&QPushButton::clicked,this,[&](){
        //切换到电压曲线页面
        ui->stackedWidget->setCurrentIndex(2);

    });
    connect(ui->pushButton_5,&QPushButton::clicked,this,[&](){
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

    connect(ui->pushButton_8,&QPushButton::clicked,this,[this](){

        saveSteadyData();
    });


    connect(ui->pushButton_9,&QPushButton::clicked,this,[this](){

        qDebug()<<"计算当前负载的数据";
        calculateSteadyData();
        ui->lineEdit_82->setText(QString::number(GlobalSettings::instance().getCurrentLoad()));

    });



    connect(ui->tabWidget, &QTabWidget::currentChanged, &dtSerialCom, &DataSerialCom::onTabChanged);
    connect(ui->pushButton_7,&QPushButton::clicked,&dtSerialCom,&DataSerialCom::StartSteadySlot);
    connect(ui->pushButton_10,&QPushButton::clicked,&dtSerialCom,&DataSerialCom::StopSteadySlot);

    //点击电能数据按钮显示电能数据
    /*
     * display(电能数据)
     * connect(button,clicked,dtSerialCom,requestElecticData)请求电能数据函数作为稳态测试页中的独立的功能
    */
    connect(&dtSerialCom,&DataSerialCom::updateSteadyPageSignal,this,&Page3_wtcs::displaySteadyData);

}

void Page3_wtcs::initSteadyUI()
{
    //将文本框组放在容器里
    lineEdits.append(ui->lineEdit_16);
    lineEdits.append(ui->lineEdit_20);
    lineEdits.append(ui->lineEdit_24);
    lineEdits.append(ui->lineEdit_17);
    lineEdits.append(ui->lineEdit_21);
    lineEdits.append(ui->lineEdit_25);
    lineEdits.append(ui->lineEdit_18);
    lineEdits.append(ui->lineEdit_22);
    lineEdits.append(ui->lineEdit_26);
    lineEdits.append(ui->lineEdit_19);
    lineEdits.append(ui->lineEdit_23);
    lineEdits.append(ui->lineEdit_27);
    //电压平均、电流平均、合计总功率、合计总功率因数
    lineEdits.append(ui->lineEdit_28);
    lineEdits.append(ui->lineEdit_29);
    lineEdits.append(ui->lineEdit_30);
    lineEdits.append(ui->lineEdit_31);
    //u12、u23、u13和频率
    lineEdits.append(ui->lineEdit_32);
    lineEdits.append(ui->lineEdit_33);
    lineEdits.append(ui->lineEdit_34);
    lineEdits.append(ui->lineEdit_35);


    // 获取当前时间
    QDateTime current = QDateTime::currentDateTime();
    // 转换为指定格式的字符串
    QString sequenceNumber = current.toString("yyyyMMddHHmmsszzz");

    ui->lineEdit_14->setText(sequenceNumber);
    ui->lineEdit_14->setToolTip(ui->lineEdit_14->text());


}

void Page3_wtcs::initTuningUI()
{
    //线路电压整定上升范围
    tuningDataEdits.append(ui->lineEdit_53);
    tuningDataEdits.append(ui->lineEdit_57);
    tuningDataEdits.append(ui->lineEdit_61);
    tuningDataEdits.append(ui->lineEdit_65);
    //相对电压整定上升范围
    tuningDataEdits.append(ui->lineEdit_54);
    tuningDataEdits.append(ui->lineEdit_58);
    tuningDataEdits.append(ui->lineEdit_62);
    tuningDataEdits.append(ui->lineEdit_66);
    //整定电压下降范围
    tuningDataEdits.append(ui->lineEdit_55);
    tuningDataEdits.append(ui->lineEdit_59);
    tuningDataEdits.append(ui->lineEdit_63);
    tuningDataEdits.append(ui->lineEdit_67);
    //相对电压整定下降范围
    tuningDataEdits.append(ui->lineEdit_56);
    tuningDataEdits.append(ui->lineEdit_60);
    tuningDataEdits.append(ui->lineEdit_64);
    tuningDataEdits.append(ui->lineEdit_68);
}

// 假设这是你查找 QTabWidget 中 QLineEdit 的函数
void Page3_wtcs::findAllLineEditsInTab(QTabWidget *tabWidget, int tabIndex)
{
    // 获取特定 tab 的 widget
    QWidget *tabPage = tabWidget->widget(tabIndex);

    if (tabPage) {
        // 查找这个 tab 页中的所有 QLineEdit
        QList<QLineEdit*> lineEdits = tabPage->findChildren<QLineEdit*>();

        // 输出找到的所有 QLineEdit
        for (QLineEdit *lineEdit : lineEdits) {
            qDebug() << "Found QLineEdit with text:" << lineEdit->text();
            lineEdits.append(lineEdit);
        }
    } else {
        qDebug() << "Tab index" << tabIndex << "does not exist.";
    }
}

void Page3_wtcs::displaySteadyData(QQueue<QString> strQueue)
{
    qDebug()<<"ssssssssssssssssssssssssssssss文本显示的浮点数ssssssssssssssssssssssssssssssss";

    qDebug()<<"个数：：：："<<strQueue.length()<<"当前tab页索引为"<<ui->tabWidget->currentIndex();

    if(DataSerialCom::getInstance().type == DataSerialCom::STEADY){
        //显示稳态数据页面
        for(int i = 0; i < lineEdits.length(); i++){

            if(!strQueue.isEmpty()){
                lineEdits.at(i)->setText(strQueue.dequeue());
            }else{
                qDebug()<<"稳态数据个数＜文本框个数";
            }


        }

    }else if(DataSerialCom::getInstance().type == DataSerialCom::TUNING){
        //显示整定数据页面
        for(int i = 0; i < tuningDataEdits.length(); i++){

            if(!strQueue.isEmpty()){
                tuningDataEdits.at(i)->setText(strQueue.dequeue());
            }else{
                qDebug()<<"整定数据个数＜文本框个数";
            }


        }
    }else{
        return  ;
    }

    unLoad_VolMax = std::max(unLoad_VolMax, ui->lineEdit_28->text().toFloat());
    unLoad_VolMin = std::min(unLoad_VolMin, ui->lineEdit_28->text().toFloat());
    load_VolMax = std::max(load_VolMax, ui->lineEdit_28->text().toFloat());
    load_VolMin = std::max(load_VolMin, ui->lineEdit_28->text().toFloat());


    diffValue = std::max(diffValue,ui->lineEdit_35->text().toFloat());

    unLoad_FluMax = std::max(unLoad_FluMax,ui->lineEdit_35->text().toFloat());
    unLoad_FluMin = std::max(unLoad_FluMin,ui->lineEdit_35->text().toFloat());
    load_FluMax = std::max(load_FluMax,ui->lineEdit_35->text().toFloat());
    load_FluMin = std::max(load_FluMin,ui->lineEdit_35->text().toFloat());

}


void Page3_wtcs::calculateSteadyData()
{
    //计算稳态电压调整率，若3相电就取平均值

    float steadyVolreg = (ui->lineEdit_28->text().toFloat() - 220.0)/220.0;
    qDebug()<<"稳态电压调整率:"<<steadyVolreg<<"%";
    ui->lineEdit_36->setText(QString::number(steadyVolreg,'f',3));

    //计算稳态电压波动率
    if(load_VolMax + load_VolMin != 0){
        float volatility = (unLoad_VolMax - unLoad_VolMin)/(load_VolMax + load_VolMin);
        qDebug()<<"稳态电压波动率:"<<volatility<<"%";
        ui->lineEdit_37->setText(QString::number(volatility,'f',3));
    }else{
        qDebug()<<"分母为0";
        ui->lineEdit_37->setText(QString::number(0,'f',3));
    }

    //计算稳态频率调整率

    float steadyFluc = (diffValue-50.00)/50.00;
    ui->lineEdit_38->setText(QString::number(steadyFluc,'f',2));

    //计算稳态频率波动率
    if(load_FluMax + load_FluMin != 0){

        float fluctility = (unLoad_FluMax - unLoad_FluMin)/(load_VolMax + load_VolMin);
        qDebug()<<"unLoad_FluMax"<<unLoad_FluMax;
        qDebug()<<"unLoad_FluMin"<<unLoad_FluMin;
        qDebug()<<"load_FluMax"<<load_FluMax;
        qDebug()<<"load_FluMax"<<load_FluMin;
        qDebug()<<"稳态频率波动率:"<<fluctility<<"%";
        ui->lineEdit_39->setText(QString::number(fluctility,'f',2));
    }else{
        qDebug()<<"分母为0";
        ui->lineEdit_39->setText(QString::number(0,'f',2));
    }


}

void Page3_wtcs::saveSteadyData()
{

    qDebug()<<"保存稳态数据";





//    Detection record(uniqueID,"电机","稳态测试","xxx",current.toString(),"Yes");

    Save::Detection record(ui->lineEdit_14->text(),
                           "电机",
                           "稳态测试",
                           ui->lineEdit_15->text(),
                           QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"),
                           "Yes");
    // 打印信息
    qDebug() << "检测编号:" << record.detectionID;
    qDebug() << "检测元件:" << record.detectionComponent;
    qDebug() << "类型:" << record.type;
    qDebug() << "检测人员:" << record.inspector;
    qDebug() << "检测时间:" << record.detectionTime;
    qDebug() << "检测结果:" << record.result;
    Save::U().updateT_dataView(record);

    Save::U().exec();

}



Page3_wtcs::~Page3_wtcs()
{
    delete ui;
}
