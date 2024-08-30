#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QSqlTableModel>
#include <QPushButton>
#include "Base/BaseFun/Sql/sql.h"
namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT
    QSqlTableModel* modelPtr = new QSqlTableModel(this,Sql::U().db);
public:
    explicit User(QWidget *parent = 0);
    ~User();
    void init();
    void initSetFzModel(QPushButton *add, QPushButton *del);
private:
    Ui::User *ui;
};

#endif // USER_H
