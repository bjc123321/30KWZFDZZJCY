#ifndef SELFTEST_H
#define SELFTEST_H

#include <QWidget>
#include <QMessageBox>  // QDialog确保包含这个头文件
#include "App/Data/datacom3.h"

namespace Ui {
class SelfTest;
}

class SelfTest : public QWidget
{
    Q_OBJECT

public:
    explicit SelfTest(QWidget *parent = 0);
    ~SelfTest();

    void setFanState(bool isOpen);
    void signalBind();

private slots:
    void on_pushButton_clicked(bool checked);

signals:

private:
    Ui::SelfTest *ui;
};

#endif // SELFTEST_H
