#include "page5_xbfx.h"
#include "ui_page5_xbfx.h"

Page5_xbfx::Page5_xbfx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page5_xbfx)
{
    ui->setupUi(this);
    init();
    signalBind();
}

Page5_xbfx::~Page5_xbfx()
{
    delete ui;
}

void Page5_xbfx::init()
{

    harmoPlot = new HarmonicPlot(this);
    ui->horizontalLayout_8->addWidget(harmoPlot);
}

void Page5_xbfx::signalBind()
{
    connect(ui->pushButton,&QPushButton::clicked,this,[&](){
        qDebug()<<"进入页面0";
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        qDebug()<<"进入页面1";
        ui->stackedWidget->setCurrentIndex(1);
    });

    // 连接相选择框的信号到槽函数
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            harmoPlot, &HarmonicPlot::onPhaseChanged);

    connect(harmoPlot,&HarmonicPlot::updateVolAndCurValue,this,
            [=](double voltageValue,double currentValue){
        ui->lineEdit_21->setText(QString("%1\%").arg(voltageValue));
        ui->lineEdit_22->setText(QString("%1\%").arg(currentValue));
    });
}
