#include "detailpage.h"
#include "ui_detailpage.h"

#include "GlobalSettings.h"
#include "App/Data/dataexportmanager.h"

DetailPage::DetailPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailPage)
{
    ui->setupUi(this);
    setModal(true);//设置模态窗口
    displayBasicText();
    signalBind();

}

void DetailPage::signalBind()
{

    connect(ui->pushButton_6,&QPushButton::clicked,this,[this](){

        QVector<QString> v_data;
        v_data.append(GlobalSettings::BASIC_INFO.testingCompany);
        v_data.append(GlobalSettings::BASIC_INFO.ratedPower);
        v_data.append(GlobalSettings::BASIC_INFO.productName);
        v_data.append(GlobalSettings::BASIC_INFO.productModel);
        v_data.append(GlobalSettings::BASIC_INFO.temperature);
        v_data.append(GlobalSettings::BASIC_INFO.execStandard);
        v_data.append(GlobalSettings::BASIC_INFO.ratedVoltage);

        DataExportManager::getInstance().generateSteadyDataExcel(v_data);

    });

}


void DetailPage::displaySteadyDetail(QSqlQueryModel *model)
{
    ui->label->setText("稳态测试");
    // 将模型设置给 QTableView
    ui->tableView->setModel(model);
//    ui->tableView->resizeColumnsToContents(); // 自动调整列宽
    ui->tableView->resizeRowsToContents(); // 自动调整行宽
    // 设置列宽自动拉伸以填满表格视图
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 更新表格视图
    ui->tableView->update();
}

void DetailPage::displaySuddLoadDetail(QSqlQueryModel *model)
{
    ui->label->setText("突加测试");
    ui->tableView->setModel(model);
//    ui->tableView->resizeColumnsToContents(); // 自动调整列宽
    ui->tableView->resizeRowsToContents(); // 自动调整行宽
    // 设置列宽自动拉伸以填满表格视图
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 更新表格视图
    ui->tableView->update();
}

void DetailPage::displayBasicText()
{

    ui->label_3->setText(GlobalSettings::BASIC_INFO.testingCompany);
    ui->label_5->setText(GlobalSettings::BASIC_INFO.execStandard);
    ui->label_18->setText(GlobalSettings::BASIC_INFO.testDate);
    ui->label_17->setText(GlobalSettings::BASIC_INFO.ratedPower);
    ui->label_22->setText(GlobalSettings::BASIC_INFO.ratedVoltage);
    ui->label_23->setText(GlobalSettings::BASIC_INFO.ratedFreq);
    ui->label_21->setText(GlobalSettings::BASIC_INFO.productState);
    ui->label_25->setText(GlobalSettings::BASIC_INFO.productName);
    ui->label_27->setText(GlobalSettings::BASIC_INFO.productModel);
    ui->label_29->setText(GlobalSettings::BASIC_INFO.productNum);
    ui->label_31->setText(GlobalSettings::BASIC_INFO.phaseOrLine);
    ui->label_34->setText(GlobalSettings::BASIC_INFO.temperature);
    ui->label_35->setText(GlobalSettings::BASIC_INFO.relativeHumidity);
    ui->label_37->setText(GlobalSettings::BASIC_INFO.atmospherePressure);
    ui->label_39->setText(GlobalSettings::BASIC_INFO.testPerson);



}


DetailPage::~DetailPage()
{
    delete ui;
}
