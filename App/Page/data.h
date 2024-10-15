#ifndef DATA_H
#define DATA_H

#include <QWidget>
#include <QSqlTableModel>
#include <QPushButton>
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

signals:



private:
    Ui::Data *ui;
};

#endif // DATA_H
