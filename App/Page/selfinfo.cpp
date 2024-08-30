#include "selfinfo.h"
#include "ui_selfinfo.h"

SelfInfo::SelfInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfInfo)
{
    ui->setupUi(this);
    init();
}

SelfInfo::~SelfInfo()
{
    delete ui;
}

SelfInfo::init()
{
    modelPtr->setTable("T_uselog");
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
