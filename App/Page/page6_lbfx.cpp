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

    recordWave = new RecordWavePlot(this);
    ui->horizontalLayout_3->addWidget(recordWave);

}
