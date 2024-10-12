#include "steadytable.h"
#include "ui_steadytable.h"

SteadyTable::SteadyTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SteadyTable)
{
    ui->setupUi(this);
}

SteadyTable::~SteadyTable()
{
    delete ui;
}
