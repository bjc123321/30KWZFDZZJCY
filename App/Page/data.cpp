#include "data.h"
#include "ui_data.h"

#include "Base/BaseFun/Sql/databasemanager.h"
#include "GlobalSettings.h"
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

    modelPtr = DatabaseManager::getInstance(GlobalSettings::sqlPath).getModel();

    modelPtr->setTable("T_data");
//    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();


    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->resizeRowsToContents(); // 自动调整行宽
    ui->tableView->setModel(modelPtr);

    // 设置列宽自动拉伸以填满表格视图
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 更新表格视图
    ui->tableView->update();

}

void Data::signalBind()
{

    connect(ui->pushButton,&QPushButton::clicked,this,[this](){


        int row = ui->tableView->currentIndex().row();

        if (row >= 0) {

            QString testId = modelPtr->data(modelPtr->index(row, 0)).toString(); // 假设 测试编号 在第一列
            qDebug()<<"删除第"<<row<<"行"<<"测试编号为:"<<testId;

            QString columnName = modelPtr->headerData(0, Qt::Horizontal).toString();
            qDebug() << "列名:" << columnName;

            //先删除次表的数据
            QSqlTableModel *model2 = DatabaseManager::getInstance(GlobalSettings::sqlPath).getModel();
            model2->setTable("T_static_data");
            bool isExe_StaticDel = DatabaseManager::getInstance(GlobalSettings::sqlPath).delRecord(model2,columnName,testId);


            if (!modelPtr->removeRow(row)) {
                QMessageBox::warning(this, "Error", "Failed to remove row: " + row);
                return ;
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
        QString testTypeString = cellData.toString();
        qDebug() << "当前第"<<(currentRow + 1)<<"行的第" << (columnIndex + 1)<< "列的数据是:" << testTypeString;

        if(testTypeString == "稳态测试"){
            QString id = modelPtr->data(modelPtr->index(currentRow, 0)).toString(); // 假设 id 在第一列
            qDebug()<<"编号:"<<id;

            detailPage->displaySteadyDetail(DatabaseManager::getInstance(GlobalSettings::sqlPath).queryRecordNum(id));
            detailPage->exec();


        }else if(testTypeString == "突加测试"){

            QString id = modelPtr->data(modelPtr->index(currentRow, 0)).toString(); // 假设 id 在第一列
            qDebug()<<"编号:"<<id;

            //后面往数据库中增加突加数据表
            detailPage->displaySuddLoadDetail(DatabaseManager::getInstance(GlobalSettings::sqlPath).queryRecordNum(id));
            detailPage->exec();


        }else if(testTypeString == "突卸测试"){

        }else if(testTypeString == "录波测试"){

        }

    } else {

        QMessageBox::warning(this, "Selection Error", "Please select a row first.");
    }



}
