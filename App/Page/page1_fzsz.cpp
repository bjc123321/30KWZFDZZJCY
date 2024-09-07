#include "page1_fzsz.h"
#include "ui_page1_fzsz.h"

#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

Page1_fzsz::Page1_fzsz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page1_fzsz)
{
    ui->setupUi(this);

    initSetFzModel(model1Ptr,"YSFZ",ui->tableView,ui->pushButton_4,ui->pushButton,ui->pushButton_12,ui->pushButton_14,ui->pushButton_15,ui->pushButton_2);

//    initAutoFzModel(model2Ptr,"ZDJZ_1",ui->tableView_3);
    // 连接滑动条的 valueChanged 信号到文本框的槽函数

    int maxKw = 120;
    connect(ui->horizontalSlider, &QSlider::valueChanged,this,[this](int value) {
       ui->lineEdit_2->setText(QString::number(value)); // 将滑动条的值转换为字符串并显示在文本框中
       ui->lineEdit_3->setText(QString::number(value*1.0/100*120)); // 将滑动条的值转换为字符串并显示在文本框中
    });
    connect(ui->horizontalSlider_2, &QSlider::valueChanged,this,[this](int value) {
       ui->lineEdit->setText(QString::number(value*0.1)); // 将滑动条的值转换为字符串并显示在文本框中

    });

    // 创建正则表达式，匹配1到3000之间的数字
    QRegularExpression regExp("^(?:[1-9][0-9]{0,2}|[1-2][0-9]{3}|3000)$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);

    // 将验证器应用到 QLineEdit
    ui->lineEdit_4->setValidator(validator);

}

Page1_fzsz::~Page1_fzsz()
{
    delete ui;
}

void Page1_fzsz::initSetFzModel(QSqlTableModel* modelPtr,QString tableName,QTableView*tableView,QPushButton*add,QPushButton*clear,QPushButton*del,QPushButton*up,QPushButton*down,
                                QPushButton*load)
{


    modelPtr->setTable(tableName);
    modelPtr->select();  // 重新从数据库中获取"所有"数据，并在视图中更新显示
    // 设置QTableView
    tableView->setModel(modelPtr);
    //数据库管理类设置model
    DatabaseManager::getInstance("sql.db").setModel(modelPtr);
    // 设置表头
    modelPtr->setHeaderData(0, Qt::Horizontal, tr("编号"));
    modelPtr->setHeaderData(1, Qt::Horizontal, tr("总功率百分比(%)"));
    modelPtr->setHeaderData(2, Qt::Horizontal, tr("功率因数"));
    modelPtr->setHeaderData(3, Qt::Horizontal, tr("持续时间"));


    // 设置单行选中和不可编辑
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列宽自动拉伸以填满表格视图
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(add,&QPushButton::clicked,this,[this](){

        addRow();

    });

    connect(del,&QPushButton::clicked,this,[this,tableView](){

        removeRow(tableView->currentIndex().row());

    });

    connect(clear,&QPushButton::clicked,this,[this](){

        clearRows();

    });

    connect(up,&QPushButton::clicked,this,[this,tableView](){

        moveRowUp(tableView->currentIndex().row());

    });

    connect(down,&QPushButton::clicked,this,[this,tableView](){

        moveRowDown(tableView->currentIndex().row());

    });

}

void Page1_fzsz::initAutoFzModel(QSqlTableModel *modelPtr, QString tableName, QTableView *tableView)
{
    tableView->setModel(modelPtr);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 设置列宽自动拉伸以填满表格视图
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void Page1_fzsz::addRow()
{
    // 初始化随机数生成器（只需要一次）
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    // 生成模拟数据
    int percentage = rand() % 101;  // 生成0到100之间的随机整数（模拟百分比）
    double powerFactor = static_cast<double>(rand() % 101) / 100.0;  // 生成0.0到1.0之间的随机小数（模拟功率因数）
    int duration = rand() % 100 + 1;  // 生成1到100之间的随机整数（模拟持续时间）


    // 调用DatabaseManager::getInstance("sql.db")的addRow方法插入新行
    if (!DatabaseManager::getInstance("sql.db").addRow(percentage, powerFactor, duration)) {
        QMessageBox::warning(this, "Error", "Failed to add row: " + DatabaseManager::getInstance("sql.db").lastError());
    }
}

void Page1_fzsz::removeRow(int row)
{

    qDebug()<<"RemoveRow:"<<row;
    if (row >= 0) {
        if (!DatabaseManager::getInstance("sql.db").removeRow(row)) {
            QMessageBox::warning(this, "Error", "Failed to remove row: " + DatabaseManager::getInstance("sql.db").lastError());
        }
    }

}


void Page1_fzsz::clearRows()
{
    if (!DatabaseManager::getInstance("sql.db").clearRows()) {
        QMessageBox::warning(this, "Error", "Failed to clear rows: " + DatabaseManager::getInstance("sql.db").lastError());
    }
}


void Page1_fzsz::moveRowUp(int row)
{
    qDebug()<<"RowUp:"<<row;
    if (row > 0) {
        if (!DatabaseManager::getInstance("sql.db").moveRowUp(row)) {
            QMessageBox::warning(this, "Error", "Failed to move row up: " + DatabaseManager::getInstance("sql.db").lastError());
        }
    }
}

void Page1_fzsz::moveRowDown(int row)
{

    qDebug()<<"RowDown:"<<row;
    if (row < DatabaseManager::getInstance("sql.db").getModel()->rowCount() - 1) {
        if (!DatabaseManager::getInstance("sql.db").moveRowDown(row)) {
            QMessageBox::warning(this, "Error", "Failed to move row down: " + DatabaseManager::getInstance("sql.db").lastError());
        }
    }

}
