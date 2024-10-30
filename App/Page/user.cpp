#include "user.h"
#include "ui_user.h"

#include "GlobalSettings.h"
#include "Base/BaseFun/Sql/databasemanager.h"
#include "Dialog/login.h"

User::User(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);



    init();
}

User::~User()
{
    delete ui;
}

void User::init()
{

    modelPtr = new QSqlTableModel(this,DatabaseManager::getInstance(GlobalSettings::sqlPath).getDataBase());
    modelPtr->setTable("login");
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    ui->tableView->setModel(modelPtr);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


    if(GlobalSettings::instance().getLoginMode() == "2"){

        //管理员模式切换到0页
        ui->stackedWidget->setCurrentIndex(0);
    }else{

        //普通用户模式切换到1页
        ui->stackedWidget->setCurrentIndex(1);
    }

    initSetFzModel(ui->pushButton,ui->pushButton_2);


    connect(ui->pushButton_6,&QPushButton::clicked,this,[this](){

        qDebug()<<"进入管理员模式";

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示！！!", "如进入管理员模式，需要重新登录，确定要退出吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::Yes){
            Login& l = Login::U();
            l.exec();
            ui->stackedWidget->setCurrentIndex(0);
        }

    });

    connect(ui->pushButton_7,&QPushButton::clicked,this,[this](){

        qDebug()<<"退出管理员模式";

        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示！！!", "如退出管理员模式，需要重新登录，确定要退出吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::Yes){
            Login& l = Login::U();
            l.exec();
            ui->stackedWidget->setCurrentIndex(1);
        }

    });



}

void User::initSetFzModel(QPushButton *add, QPushButton *del){
    connect(add,QPushButton::clicked,this,[=](){
        if (modelPtr->submitAll()) {
            int row = modelPtr->rowCount();
            modelPtr->insertRow(row);
            ui->tableView->selectRow(row);

            modelPtr->setData(modelPtr->index(row, 0), row);
        }
    });

    connect(del,QPushButton::clicked,this,[=](){
        int row = ui->tableView->currentIndex().row();

        modelPtr->select();

        if(modelPtr->rowCount() > 0 && row  < modelPtr->rowCount() ){
            modelPtr->removeRow(row);
            if(modelPtr->submitAll()){
                modelPtr->select();
                for(int i=row;i<modelPtr->rowCount();i++){
                    modelPtr->setData(modelPtr->index(i, 0), i);
                }
            }
        }
    });

    // 设置列宽自动拉伸以填满表格视图
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 更新表格视图
    ui->tableView->update();
}
