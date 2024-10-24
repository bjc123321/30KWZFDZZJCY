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

    // 设置列宽自动拉伸以填满表格视图
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 更新表格视图
    ui->tableView->update();





//    initSetFzModel(ui->pushButton);
}

void Data::signalBind()
{

    QTableView *tableView = ui->tableView;
    connect(ui->pushButton,&QPushButton::clicked,this,[this,tableView](){


        int row = tableView->currentIndex().row();
        qDebug()<<"删除第"<<row<<"行";
        if (row >= 0) {
            if (!modelPtr->removeRow(row)) {
                QMessageBox::warning(this, "Error", "Failed to remove row: " + row);
            }
            // 提交修改
            if (!modelPtr->submitAll()) {
                qDebug() << "Submit failed: ";
                return ;
            }
            modelPtr->select();

        }


    });

    connect(ui->pushButton_2,&QPushButton::clicked,this,[this](){

        detailPageView();

    });

    connect(ui->pushButton_4,&QPushButton::clicked,this,[this](){

        qDebug()<<"刷新data数据";

        refreshView();


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

void Data::refreshView()
{


    modelPtr->setTable("T_data");
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    ui->tableView->setModel(modelPtr);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 设置列宽自动拉伸以填满表格视图
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 更新表格视图
    ui->tableView->update();
}


void Data::detailPageView()
{

    DetailPage *detailPage = new DetailPage(this);



     // 获取当前行的索引
    int currentRow = ui->tableView->currentIndex().row();
    qDebug()<<"当前索引行"<<currentRow;

    // 假设 n 是你要获取的列索引
    int columnIndex = 2;


    // 获取模型并直接获取数据
    if (currentRow >= 0 && columnIndex >= 0) {
        QVariant cellData = ui->tableView->model()->data(ui->tableView->model()->index(currentRow, columnIndex));
        // 处理获取到的数据
        QString dataString = cellData.toString();
        qDebug() << "当前第"<<(currentRow + 1)<<"行的第" << (columnIndex + 1)<< "列的数据是:" << dataString;
    } else {
        qDebug() << "无效的行或列索引";
    }

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
