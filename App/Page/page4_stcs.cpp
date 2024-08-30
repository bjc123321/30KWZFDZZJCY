#include "page4_stcs.h"
#include "ui_page4_stcs.h"
#include "Base/BaseFun/base.h"

Page4_stcs::Page4_stcs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page4_stcs)
{
    ui->setupUi(this);
    init();
}

Page4_stcs::~Page4_stcs()
{
    delete ui;
}

Page4_stcs::init()
{
    Tool::SetCustomPlot(ui->tab,ui->checkBox_22,ui->checkBox_23,ui->checkBox_24);
    Tool::SetCustomPlot(ui->tab_2,ui->checkBox_25,ui->checkBox_26,ui->checkBox_27);
    Tool::SetCustomPlot(ui->tab_3,ui->checkBox_28,ui->checkBox_29,ui->checkBox_30);
    Tool::SetCustomPlot(ui->tab_4,ui->checkBox_31,ui->checkBox_32,ui->checkBox_33);
    Tool::SetCustomPlot(ui->tab_5,ui->checkBox_34,ui->checkBox_35,ui->checkBox_36);
    Tool::SetCustomPlot(ui->tab_6,ui->checkBox_37,ui->checkBox_38,ui->checkBox_39);
}

void Page4_stcs::on_pushButton_clicked()
{
    dealTime(true);
}

void Page4_stcs::on_pushButton_2_clicked()
{
    dealTime(false);
}

void Page4_stcs::dealTime(bool isStart)
{
    static int num=0;

    static int tid = -1;

    if(tid!=-1){
        if(isStart){
            num = 0;
            TimerPool::Start(tid,1000);
            return;
        }else{
            TimerPool::Stop(tid);
            return;
        }
    }

    tid = TimerPool::Start(1000,[&](int tid){
        num++;

        int i1 = num/10/6/10/6/10%2;
        ui->lcdNumber->display(i1);
        int i2 = num/10/6/10/6%10;
        if(i1 == 2 && i2 == 4){
            num = 0;
            ui->lcdNumber->display(0);
            ui->lcdNumber_2->display(0);
            return;
        }else{
            ui->lcdNumber_2->display(i2);
        }
        int i3 = num/10/6/10%6;
        ui->lcdNumber_3->display(i3);
        int i4 = num/10/6%10;
        ui->lcdNumber_4->display(i4);
        int i5 = num/10%6;
        ui->lcdNumber_5->display(i5);
        int i6 = num%10;
        ui->lcdNumber_6->display(i6);
    });
}
