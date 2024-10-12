#ifndef STEADYTABLE_H
#define STEADYTABLE_H

#include <QMainWindow>

namespace Ui {
class SteadyTable;
}

class SteadyTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit SteadyTable(QWidget *parent = nullptr);
    ~SteadyTable();

private:
    Ui::SteadyTable *ui;
};

#endif // STEADYTABLE_H
