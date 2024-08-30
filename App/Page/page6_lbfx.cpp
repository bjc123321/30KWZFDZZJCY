#include "page6_lbfx.h"
#include "ui_page6_lbfx.h"

Page6_lbfx::Page6_lbfx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page6_lbfx)
{
    ui->setupUi(this);
    init();
}

Page6_lbfx::~Page6_lbfx()
{
    delete ui;
}

Page6_lbfx::init()
{
    Tool::SetCustomPlot(ui->widget,ui->checkBox_40,ui->checkBox_41,ui->checkBox_42);
}
