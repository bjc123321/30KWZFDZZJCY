#include "save.h"
#include "ui_save.h"

#include "Base/BaseFun/Sql/databasemanager.h"
Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);


    bindDataBaseTable();
    connect(ui->pushButton,&QPushButton::clicked,this,[this](){
        saveTestRecord();
    });

}

void Save::bindDataBaseTable()
{
    model = DatabaseManager::getInstance("sql.db").getModel();
    model->setTable("T_data");
    qDebug() << "插入" << model->tableName() <<"表的字段个数"<<model->columnCount();
    // 确保数据库连接有效
    if (!model->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return ;
    }else{
        qDebug() << "数据库打开成功,可以查询"<<model->tableName()<<"表";

    }
}

void Save::updateT_dataView(Detection record)
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

void Save::saveTestRecord()
{

    QVector<QVariant> vdata;
    vdata.append(ui->lineEdit->text());
    vdata.append(ui->lineEdit_2->text());
    vdata.append(ui->lineEdit_3->text());
    vdata.append(ui->lineEdit_4->text());
    vdata.append(ui->lineEdit_5->text());
    vdata.append("Yes");

    DatabaseManager::getInstance("sql.db").insertData(model->tableName(),vdata);

}

Save::~Save()
{
    delete ui;
}
