#include "page1_fzsz.h"
#include "ui_page1_fzsz.h"


#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

#include "globalsettings.h"
#include "App/Data/dataserialcom.h"

Page1_fzsz::Page1_fzsz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page1_fzsz)
{
    ui->setupUi(this);

    initSetFzModel(model1Ptr,"YSFZ",ui->tableView,ui->pushButton_4,ui->pushButton,ui->pushButton_12,ui->pushButton_14,ui->pushButton_15,ui->pushButton_2);

//    initAutoFzModel(model2Ptr,"ZDJZ_1",ui->tableView_3);
    // 连接滑动条的 valueChanged 信号到文本框的槽函数
    connect(ui->horizontalSlider, &QSlider::valueChanged,this,[this](int value) {
       GlobalSettings::instance().setCurrentLoad(value);
       qDebug()<<"负载"<<GlobalSettings::instance().getCurrentLoad();
       ui->lineEdit_2->setText(QString::number(value)); // 将滑动条的值转换为字符串并显示在文本框中
       ui->lineEdit_3->setText(QString::number(value*1.0/100,'f',1)); // 将滑动条的值转换为字符串并显示在文本框中,最大1KW
    });
    connect(ui->horizontalSlider_2, &QSlider::valueChanged,this,[this](int value) {
       ui->lineEdit->setText(QString::number(value*0.1)); // 将滑动条的值转换为字符串并显示在文本框中
    });

    // 正则表达式匹配1到15之间的数字设置时最少1min最多15分钟.初始默认1min即60s
    QRegularExpression regex("^(1[0-5]|[1-9])$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

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
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);



    // 设置单行选中和不可编辑
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列宽自动拉伸以填满表格视图
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    /*
     * QT5.7不支持TableView设置表头字体，所以需要手动设置
    */
    QHeaderView *header = tableView->horizontalHeader();
    // 创建字体
    QFont font("Arial",20); // 设置字体为 Arial，大小为 12
    header->setFont(font);    // 设置表头字体



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
    connect(load,&QPushButton::clicked,this,[this,tableView,modelPtr](){

        selectRowsWithTimer(tableView,modelPtr);

    });


    connect(ui->pushButton_5,&QPushButton::clicked,this,[this](){

        float value = ui->lineEdit_3->text().toFloat();
        int tens_value = value*10;
        if(tens_value <= 5){
            QString hexStr = QString::number(tens_value, 16).toUpper().rightJustified(4, '0');

            QString dataStr = "01060032";
            dataStr.append(hexStr);

            QByteArray byteArray = dataStr.toUtf8();
            qDebug()<<"测试文本显示,远程一键加载负载(10倍输入)"<<tens_value<<"转为16进制字符串为:"<<dataStr;
            QByteArray dataToSend = QByteArray::fromHex(byteArray);
            qDebug()<<"发送的请求帧数据(无CRC)为:"<<dataToSend.toHex();

            SerialPortManager::getInstance().writeData("COM5", dataToSend);
        }else{

            qDebug()<<"暂时不可设置负载值大于500W的";

        }



    });

    connect(ui->pushButton_6,&QPushButton::clicked,this,[this](){

        qDebug()<<"远程一键卸载负载";

        QByteArray dataToSend = QByteArray::fromHex("010600230001");
        SerialPortManager::getInstance().writeData("COM5", dataToSend);

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
    if(!isLoading){
        static bool seeded = false;
        if (!seeded) {
            srand(static_cast<unsigned int>(time(nullptr)));
            seeded = true;
        }

        // 获取文本框中的负载百分比
        int percentage = ui->lineEdit_2->text().toInt();
        // 获取文本框中的功率因数
        double powerFactor = ui->lineEdit->text().toDouble();
        // 获取文本框中的持续时间
        int duration = ui->lineEdit_4->text().toInt();

        // 调用DatabaseManager::getInstance("sql.db")的addRow方法插入新行
        if (!DatabaseManager::getInstance("sql.db").addRow(percentage, powerFactor, duration)) {
            QMessageBox::warning(this, "Error", "Failed to add row: " + DatabaseManager::getInstance("sql.db").lastError());
        }
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }

}

void Page1_fzsz::removeRow(int row)
{

    if(!isLoading){
        qDebug()<<"RemoveRow:"<<row;
        if (row >= 0) {
            if (!DatabaseManager::getInstance("sql.db").removeRow(row)) {
                QMessageBox::warning(this, "Error", "Failed to remove row: " + DatabaseManager::getInstance("sql.db").lastError());
            }

        }
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }


}

void Page1_fzsz::clearRows()
{
    if(!isLoading){
        if (!DatabaseManager::getInstance("sql.db").clearRows()) {
            QMessageBox::warning(this, "Error", "Failed to clear rows: " + DatabaseManager::getInstance("sql.db").lastError());
        }
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }

}


void Page1_fzsz::moveRowUp(int row)
{
    if(!isLoading){
        qDebug()<<"RowUp:"<<row;
        if (row > 0) {
            if (!DatabaseManager::getInstance("sql.db").moveRowUp(row)) {
                QMessageBox::warning(this, "Error", "Failed to move row up: " + DatabaseManager::getInstance("sql.db").lastError());
            }
        }
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }

}

void Page1_fzsz::moveRowDown(int row)
{

    if(!isLoading){
        qDebug()<<"RowDown:"<<row;
        if (row < DatabaseManager::getInstance("sql.db").getModel()->rowCount() - 1) {
            if (!DatabaseManager::getInstance("sql.db").moveRowDown(row)) {
                QMessageBox::warning(this, "Error", "Failed to move row down: " + DatabaseManager::getInstance("sql.db").lastError());
            }
        }
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }


}

// 封装的函数，用于逐行选中
void Page1_fzsz::selectRowsWithTimer(QTableView *tableView,QSqlTableModel *model) {

    if(!isLoading){
        QTimer *timer = new QTimer(tableView);  // 定时器
        int *currentRow = new int(-1);  // 当前行的索引，使用指针来在 lambda 中引用修改

        // 设置焦点到表格，确保可以看到蓝色背景的选中效果
        tableView->setFocus();

        // 禁用 tableView 的点击操作
        tableView->setEnabled(false);
        isLoading = true;

        // 连接定时器的 timeout 信号到一个 lambda 表达式
        QObject::connect(timer, &QTimer::timeout, [=]() mutable {
            (*currentRow)++;
            if (*currentRow < model->rowCount()) {
                tableView->selectRow(*currentRow);  // 选中行
            } else {
                timer->stop();  // 如果所有行都已选中，则停止定时器
                // 所有行选中完成后，恢复 tableView 的点击操作
                tableView->setEnabled(true);
                isLoading = false;
            }
        });

        // 启动定时器，每秒执行一次
        timer->start(1000);
    }else{
        QMessageBox::warning(this, "无效操作", "请等待负载加载完成后，或者强制停止加载后进行其他操作!");
    }

}
