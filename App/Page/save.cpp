#include "save.h"
#include "ui_save.h"

#include "Base/BaseFun/Sql/databasemanager.h"
Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);


    bindDataBaseModel();
    connect(ui->pushButton,&QPushButton::clicked,this,[this](){
        saveSteadyTestRecord();
    });

}

void Save::bindDataBaseModel()
{
    model1 = DatabaseManager::getInstance("sql.db").getModel();

    // 确保数据库连接有效
    if (!model1->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return ;
    }else{
        qDebug() << "数据库打开成功,可以查询";

    }

    model2 = DatabaseManager::getInstance("sql.db").getModel();

    // 确保数据库连接有效
    if (!model2->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return ;
    }else{
        qDebug() << "数据库打开成功,可以查询";

    }

    model3 = DatabaseManager::getInstance("sql.db").getModel();

    // 确保数据库连接有效
    if (!model3->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return ;
    }else{
        qDebug() << "数据库打开成功,可以查询";

    }


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

    model1->setTable("T_data");
    DatabaseManager::getInstance("sql.db").insertData(model1->tableName(),vdata);
    model2->setTable("T_static_data");
    DatabaseManager::getInstance("sql.db").insertData(model2->tableName(),v_Steady_Data);


    model3->setTable("YSFZ");
    model3->setEditStrategy(QSqlTableModel::OnFieldChange);
    model3->select();




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
