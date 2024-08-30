#include "user.h"
#include "ui_user.h"

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

    modelPtr->setTable("T_userInfo");
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    ui->tableView->setModel(modelPtr);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    initSetFzModel(ui->pushButton,ui->pushButton_2);
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
