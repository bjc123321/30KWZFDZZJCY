#ifndef DATA_H
#define DATA_H

#include <QWidget>
#include <QSqlTableModel>
#include <QPushButton>
#include <QMessageBox>
#include "Base/BaseFun/Sql/sql.h"


namespace Ui {
class Data;
}

class Data : public QWidget
{
    Q_OBJECT
    QSqlTableModel* modelPtr = new QSqlTableModel(this,Sql::U().db);
public:
    explicit Data(QWidget *parent = 0);
    ~Data();
    void init();
    void signalBind();
    void detailPageView();
    void initSetFzModel(QPushButton *del);


    void refreshView();
signals:



private:
    Ui::Data *ui;
};

#endif // DATA_H
