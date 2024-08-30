#include "page1_fzsz.h"
#include "ui_page1_fzsz.h"

Page1_fzsz::Page1_fzsz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page1_fzsz)
{
    ui->setupUi(this);

    initSetFzModel(model1Ptr,"YSFZ",ui->tableView,ui->pushButton_4,ui->pushButton_12,ui->pushButton_14,ui->pushButton_15,
                   ui->pushButton);
    initAutoFzModel(model2Ptr,"ZDJZ_1",ui->tableView_3);
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

void Page1_fzsz::initSetFzModel(QSqlTableModel *modelPtr, QString sqlData, QTableView *tableView, QPushButton *add, QPushButton *del, QPushButton *up, QPushButton *down,
                                QPushButton *set)
{

    modelPtr->setTable(sqlData);
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    tableView->setModel(modelPtr);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 禁用所有编辑触发器
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QRegExp regexp_NoEdit("");
    QRegExp regexp_0To100("^([0-9]|[1-9]\\d|100)$");
    QRegExp regexp_0_5To1("^0\.[5-9]\d*|1(\.0*)?$");
    QRegExp regexp_OnlyNum("^[0-9]+$");

    static CustomItemDelegate delegate_NoEdit(regexp_NoEdit);
    static CustomItemDelegate delegate_0To100(regexp_0To100);
    static CustomItemDelegate delegate_0_5To1(regexp_0_5To1);
    static CustomItemDelegate delegate_OnlyNum(regexp_OnlyNum);

    tableView->setItemDelegateForColumn(0, &delegate_NoEdit);
    tableView->setItemDelegateForColumn(1, &delegate_0To100); // 第二列只能输入0到100
    tableView->setItemDelegateForColumn(2, &delegate_0_5To1);   // 第三列只能输入0.5到1
    tableView->setItemDelegateForColumn(3, &delegate_OnlyNum);   // 第三列只能输入0到1

    connect(add,QPushButton::clicked,this,[=](){
        if (modelPtr->submitAll()) {
            int row = modelPtr->rowCount();
            modelPtr->insertRow(row);
            tableView->selectRow(row);

            modelPtr->setData(modelPtr->index(row, 0), row);
            // 填充其他列的数据（假设有4列，0为行号，1、2和3为其他数据）
            modelPtr->setData(modelPtr->index(row, 1), ui->lineEdit_2->text());   // 第二列填充文本数据
            modelPtr->setData(modelPtr->index(row, 2), ui->lineEdit->text());   // 第三列填充整数数据
            modelPtr->setData(modelPtr->index(row, 3), ui->lineEdit_4->text());     // 第四列填充整数数据
        }
    });

    connect(del,QPushButton::clicked,this,[=](){
        int row = tableView->currentIndex().row();

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

    connect(up,QPushButton::clicked,this,[=](){
        int row = tableView->currentIndex().row();
        if(row == 0){
            return;
        }else{
            for (int column = 1; column < modelPtr->columnCount(); ++column) {
                QModelIndex index = modelPtr->index(row, column);
                QModelIndex indexUp = modelPtr->index(row-1, column);

                QVariant data = index.data();
                QVariant dataUp = indexUp.data();

                modelPtr->setData(modelPtr->index(row, column), dataUp);
                modelPtr->setData(modelPtr->index(row-1, column), data);
            }
        }

    });

    connect(down,QPushButton::clicked,this,[=](){
        int row = tableView->currentIndex().row();
        if(row == modelPtr->columnCount()){
            return;
        }else{
            for (int column = 1; column < modelPtr->columnCount(); ++column) {
                QModelIndex index = modelPtr->index(row, column);
                QModelIndex indexUp = modelPtr->index(row+1, column);

                QVariant data = index.data();
                QVariant dataDown = indexUp.data();

                modelPtr->setData(modelPtr->index(row, column), dataDown);
                modelPtr->setData(modelPtr->index(row+1, column), data);
            }
        }
    });

    connect(set,QPushButton::clicked,this,[=](){
        int row = tableView->currentIndex().row();
        if(row > -1 && row < modelPtr->columnCount() ){
            for (int column = 1; column < modelPtr->columnCount(); ++column) {
                QModelIndex index = modelPtr->index(row, column);
                QVariant  data = index.data();
                switch (column) {
                case 1:
//                    box1->setValue(ui->doubleSpinBox_3->maximum() * (data.toDouble()/100));
                    break;
                case 2:
//                    box2->setValue(data.toDouble());
                    break;
                case 3:
//                    box3->setValue(data.toInt());
                    break;
                }

            }
        }
    });

    // 设置列宽自动拉伸以填满表格视图
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 更新表格视图
    tableView->update();
}

void Page1_fzsz::initAutoFzModel(QSqlTableModel *modelPtr, QString sqlData, QTableView *tableView)
{
    modelPtr->setTable(sqlData);
    modelPtr->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelPtr->select();
    tableView->setModel(modelPtr);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


    QRegExp regexp_NoEdit("");
    QRegExp regexp_0To100("^([0-9]|[1-9]\\d|100)$");
    QRegExp regexp_0_5To1("^0\.[5-9]\d*|1(\.0*)?$");
    QRegExp regexp_OnlyNum("^[0-9]+$");

    static CustomItemDelegate delegate_NoEdit(regexp_NoEdit);
    static CustomItemDelegate delegate_0To100(regexp_0To100);
    static CustomItemDelegate delegate_0_5To1(regexp_0_5To1);
    static CustomItemDelegate delegate_OnlyNum(regexp_OnlyNum);

    tableView->setItemDelegateForColumn(0, &delegate_NoEdit);
    tableView->setItemDelegateForColumn(1, &delegate_0To100); // 第二列只能输入0到100
    tableView->setItemDelegateForColumn(2, &delegate_0_5To1);   // 第三列只能输入0.5到1
    tableView->setItemDelegateForColumn(3, &delegate_OnlyNum);   // 第三列只能输入0到1

    // 设置列宽自动拉伸以填满表格视图
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



