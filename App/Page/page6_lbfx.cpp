#include "page6_lbfx.h"
#include "ui_page6_lbfx.h"

#include "App/Data/dataserialcom.h"

Page6_lbfx::Page6_lbfx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page6_lbfx)
{
    ui->setupUi(this);
    init();
}

Page6_lbfx::~Page6_lbfx()
{
    delete ui;
}

Page6_lbfx::init()
{

    recordWave = new RecordWavePlot(this);
    ui->horizontalLayout_3->addWidget(recordWave);

    DataSerialCom &dtmanager = DataSerialCom::getInstance();

    connect(ui->pushButton,&QPushButton::clicked,this,[this](){

        emit startRecordWaveSignal();

    });
    connect(this,&Page6_lbfx::startRecordWaveSignal,&dtmanager,&DataSerialCom::startRecordWaveSlot);

    connect(ui->pushButton_5,&QPushButton::clicked,this,[this](){

        emit readRecordWaveSignal();

    });
    connect(this,&Page6_lbfx::readRecordWaveSignal,&dtmanager,&DataSerialCom::readRecordWaveSlot);

}
