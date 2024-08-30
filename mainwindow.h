#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "App/Data/database.h"
#include "Base/BaseFun/base.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void init();
    void initNavTree();
    void initClose();
private slots:
    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
