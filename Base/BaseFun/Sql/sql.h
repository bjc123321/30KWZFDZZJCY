#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QVector>
#include <QMap>
#include <QDir>
#include "Base/BaseFun/base.h"

class Sql : public QObject
{
    Q_OBJECT
    B_INSTANCE(Sql)
public:
    explicit Sql(QString basePath = QDir::currentPath(),QObject *parent = 0);
    ~Sql();
    QString strPath;
    QSqlDatabase db;
    QMap<QString,QVector<QString>> mapColsName;//表头
    QMap<QString,QString> mapCurSelect;//表当前显示信息的查询语句
    QMap<QString,QList<QVector<QVariant>>> mapSqlData;//表当前显示信息

    QSqlQuery query;
public:
    bool openDB();
    void closeDB();

    QVector<QString> getMapColsName(QString table,bool isNeedId = false); //获取标头(需要主键id)
    bool createTable(QString table,QVector<QString> &vstr);
public:
    bool insertData(QString table,QVector<QVariant> vdata);
    bool deleteData(QString table,int row);
    bool clearData(QString table);//清空表 (刷新id行号计数器)
    bool reviseData(QString table,int row,int col,QVariant value);

    QList<QVector<QVariant> > & selectData(QString table,QString select = "");//查询表 select 格式 为 $='$' $>0 等查询语句
public:
    static bool InsertData(QString table,QVector<QVariant> vdata);
    static bool DeleteData(QString table,int row);
    static bool ClearData(QString table);
    static bool ReviseData(QString table,int row,int col,QVariant value);

    static QList<QVector<QVariant> > & SelectData(QString table,QString select = "");
signals:

public slots:
};

#endif // SQL_H
