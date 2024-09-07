#ifndef PAGE1_FZSZ_H
#define PAGE1_FZSZ_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidget>
#include <QSqlTableModel>
#include "App/Data/datacom3.h"
#include "Base/BaseFun/Sql/sql.h"
#include <QSqlError>
#include <QItemDelegate>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "Base/BaseFun/Sql/databasemanager.h"

namespace Ui {
class Page1_fzsz;
}

class Page1_fzsz : public QWidget
{
    Q_OBJECT

public:
    explicit Page1_fzsz(QWidget *parent = 0);

    ~Page1_fzsz();

    QSqlTableModel* model1Ptr = new QSqlTableModel(this,Sql::U().db);//手动设置
    QSqlTableModel* model2Ptr = new QSqlTableModel(this,Sql::U().db);//自动设置

public:
    void initSetFzModel(QSqlTableModel* modelPtr,QString tableName,QTableView*tableView,QPushButton*add,QPushButton*clear,QPushButton*del,QPushButton*up,QPushButton*down,
                        QPushButton*load);
    void initAutoFzModel(QSqlTableModel* modelPtr,QString tableName,QTableView*tableView);

    void addRow();

    void removeRow(int row);

    void clearRows();

    void moveRowUp(int row);

    void moveRowDown(int row);

private slots:


private:
    Ui::Page1_fzsz *ui;
};



#endif // PAGE1_FZSZ_H
