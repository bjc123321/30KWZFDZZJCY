#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


class DatabaseManager
{

public:

    static DatabaseManager& getInstance(const QString &dbName) {
        static DatabaseManager instance(dbName);
        return instance;
    }

    // 初始化数据库并创建表
    bool initialize();

    // 设置表模型
    void setModel(QSqlTableModel* model);


    //获取数据库
    QSqlDatabase getDataBase(){ return db;}

    // 获取表模型
    QSqlTableModel* getModel();

    /*下面是直接在model上进行增、删操作*/

    // 增加1行
    bool addRow(QSqlTableModel *model,int percentage, double powerFactor, int duration);

    // 批量插入多行数据
    void addMultipleRows(QSqlTableModel *model,int numRows);

    // 分页加载数据,待完善：参数增加QSqlTableModel *model
    void loadMoreData(int rowsPerPage, int currentPage);

    // 删除1行
    bool removeRow(QSqlTableModel *model,int row);

    // 清空所有行
    bool clearRows(QSqlTableModel *model);

    // 上移选中的行
    bool moveRowUp(QSqlTableModel *model,int row);

    // 下移选中的行
    bool moveRowDown(QSqlTableModel *model,int row);


    /*下面是使用sql语句对model进行增、删、查、改*/
    //根据数据信息查询
    QSqlQueryModel* queryRecordNum(QString id);

    //根据表名和条件查询相关数据,查询结果为视图
    QSqlQueryModel* queryRecord(QSqlTableModel *model ,QString queryCondition = "");

    //删除记录,先查询出来，按照删除条件对记录删除
    bool delRecord(QSqlTableModel *model ,QString columnName, QString delCondition = "");

    //查询T_data(显示主要部分),后续可根据需要进行查询限制主要的字段
    QSqlQueryModel* queryT_dataPart();

    //向指定表中插入一条数据(通用数据:数组和结构体等)
    bool insertData(QSqlTableModel *model, QVector<QVariant> vdata);

    // 获取数据库错误信息
    QString lastError() const;

    void loadMoreData_2(int batchSize);
    int currentRowIndex = 0;  // 初始化加载起点

private:

    // 构造函数，传入数据库文件名
    explicit DatabaseManager(const QString &dbName);// 私有化构造函数
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;  // 禁止拷贝构造
    DatabaseManager& operator=(const DatabaseManager&) = delete;  // 禁止赋值操作

    QSqlDatabase db;
    QSqlTableModel* model = nullptr;





};

#endif // DATABASEMANAGER_H
