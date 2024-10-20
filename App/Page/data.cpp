#include "data.h"
#include "ui_data.h"

#include "Base/BaseFun/Sql/databasemanager.h"

#include "App/Page/detailpage.h"

Data::Data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Data)
{
    ui->setupUi(this);

    init();
    signalBind();
}

Data::~Data()
{
    delete ui;
}

void Data::init()
{
    modelPtr->setTable("T_data");
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    ui->tableView->setModel(modelPtr);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    initSetFzModel(ui->pushButton);
}

void Data::signalBind()
{
    connect(ui->pushButton_2,&QPushButton::clicked,this,[this](){

        detailPageView();

    });
}

void Data::initSetFzModel(QPushButton *del){

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

void Data::detailPageView()
{

    DetailPage *detailPage = new DetailPage(this);



    int currentRow = ui->tableView->currentIndex().row();
    qDebug()<<"当前索引"<<currentRow;

    //后期增加个详情类型判断：如稳态详情、瞬态详情、录波分析详情等
    //    if(稳态){

    //    }else if(瞬态){

    //    }else if(录波){

    //    }


    if (currentRow >= 0) {
        QString id = modelPtr->data(modelPtr->index(currentRow, 0)).toString(); // 假设 id 在第一列
        qDebug()<<"编号:"<<id;
        detailPage->displaySteadyDetail(DatabaseManager::getInstance("sql.db").queryRecordNum(id));
        detailPage->exec();

    } else {
        QMessageBox::warning(this, "Selection Error", "Please select a row first.");
    }

}
