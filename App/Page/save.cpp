#include "save.h"
#include "ui_save.h"

Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);
}

void Save::updateSteadyData(Detection record)
{



    ui->lineEdit->setText(record.detectionID);
    ui->lineEdit_2->setText(record.detectionComponent);
    ui->lineEdit_3->setText(record.type);
    ui->lineEdit_4->setText(record.inspector);
    ui->lineEdit_5->setText(record.detectionTime);
    if(record.result == "NO"){
        ui->radioButton->setChecked(true);
    }else{
        ui->radioButton_2->setChecked(true);
    }


}

Save::~Save()
{
    delete ui;
}
