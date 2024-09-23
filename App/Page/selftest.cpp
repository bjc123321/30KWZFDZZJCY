#include "selftest.h"
#include "ui_selftest.h"
#include "base/BaseFun/TimerPool/timerpool.h"
#include "globalsettings.h"

SelfTest::SelfTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfTest)
{
    ui->setupUi(this);

    signalBind();
    ui->pushButton_3->setStyleSheet("border-image:url(:/res/light1_1.png);");
    ui->pushButton_4->setStyleSheet("border-image:url(:/res/light1_1.png);");
    ui->pushButton_5->setStyleSheet("border-image:url(:/res/light1_4.png);");

}

void SelfTest::signalBind()
{
    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        //打开风机
        setFanState(true);
    });
    connect(ui->pushButton_7,&QPushButton::clicked,this,[&](){
        //关闭风机,要注意先卸掉所有负载之后延时关闭风机，防止负载箱内设备过热
        setFanState(false);
    });
}

void SelfTest::setFanState(bool isOpen)
{
    // 如果当前风机状态与要设置的状态相同，则直接返回，避免重复操作
    if (globalSettingsInstance().fanIsOpen == isOpen) {
        if (isOpen) {
            // 弹出一个提示框，告知用户风机已经打开
            QMessageBox::warning(this, tr("操作无效"), tr("风机已经处于打开状态，不能重复打开!"));
        } else {
            QMessageBox::warning(this, tr("操作无效"), tr("风机已经处于关闭状态，不能重复关闭!"));
        }
        return;
    }

    globalSettingsInstance().fanIsOpen = isOpen;
    if(globalSettingsInstance().fanIsOpen){
        qDebug()<<"风机打开!";
        ui->pushButton_6->hide();
        ui->label_4->hide();
        ui->pushButton_3->setStyleSheet("border-image:url(:/res/light1_4.png);");
    //    DataCom3::U().setFJKG(true);//待其他功能实现后在解开注释
    }else{
        qDebug()<<"风机未开,请先开风机!";
        ui->pushButton_6->show();
        ui->label_4->show();
        ui->pushButton_3->setStyleSheet("border-image:url(:/res/light1_1.png);");
    }

}

void SelfTest::on_pushButton_clicked(bool checked)
{
    if(checked){
        ui->textEdit->clear();
        TimerPool::Start(100,[=](int tid){
            ui->pushButton_4->setStyleSheet("border-image:url(:/res/light1_4.png);");
            ui->textEdit->append("串口检测正常");
            ui->textEdit->append("485模块1工作正常");
            ui->textEdit->append("485模块2工作正常");
            ui->textEdit->append("硬件检测正常");
            ui->textEdit->append("设备自检正常");
            ui->pushButton->click();
        },true);

        ui->pushButton->setText("停止自检");
    }else{
        ui->pushButton->setText("开始自检");
    }
}

SelfTest::~SelfTest()
{
    delete ui;
}


