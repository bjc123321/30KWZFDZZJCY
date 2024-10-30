#include "save.h"
#include "ui_save.h"

#include "GlobalSettings.h"
#include "Base/BaseFun/Sql/databasemanager.h"
Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,[this](){
        saveSteadyTestRecord();
    });

}

void Save::displayT_dataView(TEST_RECORD record)
{

    ui->lineEdit->setText(record.detectionID);
    ui->lineEdit_2->setText(record.detectionComponent);
    ui->lineEdit_3->setText(record.type);
    ui->lineEdit_4->setText(record.inspector);
    ui->lineEdit_5->setText(record.detectionTime);
    if(record.result == "Yes"){
        ui->radioButton->setChecked(true);
    }else{
        ui->radioButton_2->setChecked(true);
    }

}

void Save::saveSteadyTestRecord()
{

    qDebug()<<"确认保存了***稳态记录***以及***稳态详细数据***";
    QVector<QVariant> vdata;
    vdata.append(ui->lineEdit->text());
    vdata.append(ui->lineEdit_2->text());
    vdata.append(ui->lineEdit_3->text());
    vdata.append(ui->lineEdit_4->text());
    vdata.append(ui->lineEdit_5->text());
    vdata.append("Yes");


    QSqlTableModel *model1 = DatabaseManager::getInstance(GlobalSettings::sqlPath).getModel();
    model1->setTable("T_data");
    bool t_dataSave =  DatabaseManager::getInstance(GlobalSettings::sqlPath).insertData(model1,vdata);

    QSqlTableModel *model2 = DatabaseManager::getInstance(GlobalSettings::sqlPath).getModel();
    model2->setTable("T_static_data");
    bool t_staticSave = DatabaseManager::getInstance(GlobalSettings::sqlPath).insertData(model2,v_Steady_Data);

    if(t_dataSave && t_staticSave){

        QMessageBox::information(
                nullptr,  // 父窗口指针，nullptr表示没有父窗口
                "保存成功",  // 对话框标题
                "您的数据已成功保存。",  // 对话框内容
                QMessageBox::Ok  // 按钮类型
            );

    }

}

void Save::packSteadyDetailRecord(QVector<QVariant> v_Steady_Detail_Data)
{

    qDebug()<<"打包稳态细节记录";
    v_Steady_Data = v_Steady_Detail_Data;


}

Save::~Save()
{
    delete ui;
}
