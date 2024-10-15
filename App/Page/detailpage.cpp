#include "detailpage.h"
#include "ui_detailpage.h"


DetailPage::DetailPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailPage)
{
    ui->setupUi(this);
    setModal(true);

}


void DetailPage::displaySteadyDetail(QSqlQueryModel *model)
{
    // 将模型设置给 QTableView
    ui->tableView->setModel(model);
//    ui->tableView->resizeColumnsToContents(); // 自动调整列宽
    // 设置列宽自动拉伸以填满表格视图
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 更新表格视图
    ui->tableView->update();
}


DetailPage::~DetailPage()
{
    delete ui;
}
