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

    // 获取表模型
    QSqlTableModel* getModel();

    // 增加1行
    bool addRow(int percentage, double powerFactor, int duration);

    // 批量插入多行数据
    void addMultipleRows(int numRows);

    // 分页加载数据
    void loadMoreData(int rowsPerPage, int currentPage);

    // 删除1行
    bool removeRow(int row);

    // 清空所有行
    bool clearRows();

    // 上移选中的行
    bool moveRowUp(int row);

    // 下移选中的行
    bool moveRowDown(int row);

    //根据数据信息查询
    QSqlQueryModel* queryRecordNum(QString id);

    //向指定表中插入一条数据(通用数据:数组和结构体等)
    bool insertData(QString table, QVector<QVariant> vdata);

    // 获取数据库错误信息
    QString lastError() const;

    void loadMoreData_2(int batchSize);
    int currentRowIndex = 0;  // 初始化加载起点


    bool isExecSucceed = false;


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
