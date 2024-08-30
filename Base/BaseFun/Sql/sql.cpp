#include "sql.h"

Sql::Sql(QString basePath, QObject *parent) :
    QObject(parent),
    strPath(basePath + "/Depend/Base/sql.db"),
    db(QSqlDatabase::addDatabase("QSQLITE","sqlite")),
    query(QSqlQuery(db))
{

    //注意检查如果数据库被移动则要做好判断
    if (!QFile::exists(strPath)) {
        QMessageBox::critical(nullptr, "Database Error",strPath + ".The database file does not exist.");
        return;
    }else{

        db.setDatabaseName(strPath);
        openDB();
    }

}

Sql::~Sql()
{
    closeDB();
}

bool Sql::openDB()
{
    return db.open();
}

void Sql::closeDB()
{
    db.close();
}
//使用QSqlTableModel绑定QTableView后无需再使用传统的sql语句进行增删查改,即无需调用下面一些函数了


QVector<QString> Sql::getMapColsName(QString table, bool isNeedId)
{
    QVector<QString> vstr;

    if(!mapColsName.contains(table)){

        query.exec("SELECT * FROM " + table);

        QSqlRecord record = query.record();
        for(int i=0;i<record.count();i++){
            vstr << record.fieldName(i);
        }
        mapColsName[table] = vstr;
    }else{
        vstr = mapColsName[table];
    }

    if(!isNeedId){
        vstr.remove(0);
    }
    return vstr;
}

bool Sql::createTable(QString table, QVector<QString> &vstr)
{
    QString sqlStr ="CREATE TABLE "+table+" ( id INTEGER PRIMARY KEY AUTOINCREMENT,";
    for(int i=0;i<vstr.size();i++){
        sqlStr += vstr[i] + " TEXT ";
        if(i!=vstr.size()-1){
            sqlStr +=",";
        }
    }
    sqlStr+=" ) ";
    return query.exec(sqlStr);
}

bool Sql::insertData(QString table, QVector<QVariant> vdata)
{
    QVector<QString> vstr = getMapColsName(table);

    if(vdata.size() != vstr.size()){
        return 0;
    }

    QString sqlStr = "INSERT INTO "+table+" ( ";
    for(int i=0;i<vstr.size();i++){
        sqlStr += vstr[i];
        if(i!=vstr.size()-1){
            sqlStr +=",";
        }
    }
    sqlStr+=" ) VALUES ( ";
    for(int i=0;i<vdata.size();i++){
        sqlStr += '\'' + vdata[i].toString()+'\'';
        if(i!=vstr.size()-1){
            sqlStr +=",";
        }
    }
    sqlStr+=" ) ";
    bool ret = query.exec(sqlStr);

    if(ret && mapCurSelect[table] != ""){
        ret = query.exec(mapCurSelect[table]);
        query.last();
        if((query.at()+1) > mapSqlData[table].size()){
            mapSqlData[table] << vdata;
        }
    }

    return ret;
}

bool Sql::deleteData(QString table, int row)
{
    int id = mapSqlData[table][row][0].toInt();

    QString sqlStr = "DELETE FROM "+table+" WHERE id = "+ QString::number(id);

    bool ret = query.exec(sqlStr);
    if(ret){
        mapSqlData[table].removeAt(row);
    }
    return ret;
}

bool Sql::clearData(QString table)
{
    QVector<QString> vstr = getMapColsName(table);

    QString sqlStr = "DROP TABLE " + table;
    bool ret = query.exec(sqlStr);
    if(ret){
        ret = createTable(table,vstr);
    }
    return ret;
}

bool Sql::reviseData(QString table, int row, int col, QVariant value)
{
    QList<QVector<QVariant>> lvdata = mapSqlData[table];
    QVector<QString> headers = getMapColsName(table);
    int id = lvdata[row][0].toInt();


    QString sqlStr = "UPDATE "+table+" SET "+
            headers[col]+" = "+value.toString()+" WHERE "+
            " id = "+QString::number(id);

    bool ret = query.exec(sqlStr);
    if(ret){
        mapSqlData[table][row][col] = value;
    }

    return ret;
}

QList<QVector<QVariant>> &Sql::selectData(QString table, QString select)
{
    QString sqlStr = "SELECT * FROM "+table;
    if(select != ""){
        sqlStr+= " WHERE "+ select;
    }

    if(sqlStr != mapCurSelect[table]){
        mapCurSelect[table] = sqlStr;

        bool ret = query.exec(sqlStr);
        if(ret){
            mapSqlData[table].clear();

            QVector<QString> vstr = getMapColsName(table,true);
            while (query.next()) {
                QVector<QVariant> vdata;
                for(int i=0;i<vstr.size();i++){
                    vdata <<  query.value(i);
                }
                mapSqlData[table] << vdata;
            }
        }
    }
    return mapSqlData[table];
}

bool Sql::InsertData(QString table, QVector<QVariant> vdata)
{
    return Sql::U().insertData(table,vdata);
}

bool Sql::DeleteData(QString table, int row)
{
    return Sql::U().deleteData(table,row);
}

bool Sql::ClearData(QString table)
{
    return Sql::U().clearData(table);
}

bool Sql::ReviseData(QString table, int row, int col, QVariant value)
{
    return Sql::U().reviseData(table,row,col,value);
}

QList<QVector<QVariant> > &Sql::SelectData(QString table, QString select)
{
    return Sql::U().selectData(table,select);
}
