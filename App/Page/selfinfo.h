#ifndef SELFINFO_H
#define SELFINFO_H

#include <QWidget>
#include <QSqlTableModel>
#include "Base/BaseFun/Sql/sql.h"
namespace Ui {
class SelfInfo;
}

class SelfInfo : public QWidget
{
    Q_OBJECT
    QSqlTableModel* modelPtr = new QSqlTableModel(this,Sql::U().db);
public:
    explicit SelfInfo(QWidget *parent = 0);
    ~SelfInfo();
    init();
private:
    Ui::SelfInfo *ui;
};

#endif // SELFINFO_H
