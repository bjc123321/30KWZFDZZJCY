#include "page4_stcs.h"
#include "ui_page4_stcs.h"
#include "Base/BaseFun/base.h"

#include "App/Data/dataserialcom.h"
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

}

void Page4_stcs::signalBind()
{

    connect(ui->pushButton,&QPushButton::clicked,this,[this](){

        //开始突加测试
       emit startSuddIncrease();
    });


    DataSerialCom &dtSerialCom = DataSerialCom::getInstance();

    connect(this,&Page4_stcs::startSuddIncrease,&dtSerialCom,&DataSerialCom::startSuddIncreaseSlot);

}





Page4_stcs::~Page4_stcs()
{
    delete ui;
}




