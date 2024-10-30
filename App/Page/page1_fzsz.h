#ifndef PAGE1_FZSZ_H
#define PAGE1_FZSZ_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidget>
#include <QSqlTableModel>
#include "App/Data/datacom3.h"
#include "Base/BaseFun/Sql/sql.h"
#include <QSqlError>
#include <QTimer>
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

//    QSqlTableModel* model1Ptr = new QSqlTableModel(this,Sql::U().db);//手动设置

public:
    void initSetFzModel(QTableView*tableView,QPushButton*add,QPushButton*clear,QPushButton*del,QPushButton*up,QPushButton*down,
                        QPushButton*load);

    void addRow(QSqlTableModel *model);

    void removeRow(QSqlTableModel *model,int row);

    void clearRows(QSqlTableModel *model);

    void moveRowUp(QSqlTableModel *model,int row);

    void moveRowDown(QSqlTableModel *model,int row);

    void selectRowsWithTimer(QTableView *tableView,QSqlTableModel *modelPtr);

    //默认没有开始加载负载,当isLoading值为true时，则禁止其他对表格的操作,包括增删清空和多次点击加载的场景！
    bool isLoading = false;


private slots:


private:
    Ui::Page1_fzsz *ui;
};



#endif // PAGE1_FZSZ_H
