#include "databasemanager.h"
#include <QDebug>
#include <QApplication>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

DatabaseManager::DatabaseManager(const QString &dbName)
{
    // 设置SQLite数据库文件连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    // 打开数据库
    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
    }

}

DatabaseManager::~DatabaseManager()
{
    db.close();
}

bool DatabaseManager::initialize()
{
    // 如果数据库打开失败，返回false
    if (!db.isOpen()) {
        return false;
    }

    // 检查数据库中是否存在表，如果不存在则创建
    if (db.tables().isEmpty()) {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE data ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "percentage INTEGER, "
                        "power_factor REAL, "
                        "duration INTEGER)")) {
            qDebug() << "Error creating table: " << query.lastError().text();
            return false;
        }
    }

    // 初始化模型
    model = new QSqlTableModel(nullptr, db);
    model->setTable("data");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();  // 重新从数据库中获取"所有"数据，并在视图中更新显示

    // 初始化时只加载第一页数据，不加载所有数据。注意：model->select();和 loadMoreData_2(20);只能存在一个
//    loadMoreData_2(20);  // 例如界面刚打开只加载20行数据，仅限显示不可编辑


    return true;
}

void DatabaseManager::setModel(QSqlTableModel *sqlModel)
{
    model = sqlModel;
    model->setTable(sqlModel->tableName());
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();  // 重新从数据库中获取"所有"数据，并在视图中更新显示
}

QSqlTableModel* DatabaseManager::getModel()
{
    return model;
}


void DatabaseManager::loadMoreData_2(int batchSize)
{
    QString queryStr = QString("SELECT * FROM data LIMIT %1 OFFSET %2")
                           .arg(batchSize)
                           .arg(currentRowIndex);

    QSqlQuery query(queryStr, model->database());

    int loadedRows = 0;  // 记录加载了多少行数据

    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            int newRow = model->rowCount();
            model->insertRow(newRow);

            for (int col = 0; col < record.count(); ++col) {
                model->setData(model->index(newRow, col), record.value(col));
            }

            loadedRows++;
        }

        qDebug() << "加载了" << loadedRows << "行";
        model->submitAll();
        currentRowIndex += loadedRows;  // 更新当前加载的行索引
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
}


// 分页加载数据
void DatabaseManager::loadMoreData(int rowsPerPage, int currentPage)
{
    int startRow = currentPage * rowsPerPage;

    //查询从 data 表的第 startRow 行开始，返回 rowsPerPage 行数据。
    QString queryStr = QString("SELECT * FROM data LIMIT %1 OFFSET %2").arg(rowsPerPage).arg(startRow);
    QSqlQuery query(queryStr, model->database());

    int loadedRows = 0;

    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            int newRow = model->rowCount();
            model->insertRow(newRow);

            for (int col = 0; col < record.count(); ++col) {
                model->setData(model->index(newRow, col), record.value(col));
            }

            loadedRows ++;
        }

        qDebug() << "加载了" << loadedRows << "行";
        model->submitAll();

        // 如果没有更多数据，停止加载
        if (loadedRows < rowsPerPage) {
            qDebug() << "No more data to load";
        }
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
}

// 一键批量增加数据,插入一段数据之后刷新模型，确保数据被逐步更新，而不是在插入完 x条数据之后再一次性刷新。
void DatabaseManager::addMultipleRows(int numRows)
{
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    const int batchSize = 10; // 每10条数据刷新一次
    int insertedCount = 0;

    for (int i = 0; i < numRows; ++i) {
        int percentage = rand() % 101;  // 随机生成百分比
        double powerFactor = static_cast<double>(rand() % 101) / 100.0;  // 随机生成功率因数
        int duration = rand() % 100 + 1;  // 随机生成持续时间

        if (!addRow(percentage, powerFactor, duration)) {
            qDebug() << "Failed to add row!";
            return;
        }

        insertedCount++;

        // 每插入 batchSize 条数据，刷新模型
        if (insertedCount % batchSize == 0) {
            model->select();  // 刷新模型以更新界面
            QCoreApplication::processEvents();  // 处理当前的UI事件，保持界面响应
        }
    }

    // 最后一次刷新所有数据
    model->select();
}


bool DatabaseManager::addRow(int percentage, double powerFactor, int duration)
{
    if (!model) return false;

    qDebug()<<"执行了";
    int row = model->rowCount();
    model->insertRow(row);
    model->setData(model->index(row, 1), percentage);
    model->setData(model->index(row, 2), powerFactor);
    model->setData(model->index(row, 3), duration);

    return model->submitAll();
}

bool DatabaseManager::removeRow(int row)
{
    if (!model || row < 0 || row >= model->rowCount()) return false;

    model->removeRow(row);
    return model->submitAll();
}

bool DatabaseManager::clearRows()
{

    if (!model) return false;

       // 执行SQL删除操作，清除所有数据
       QSqlQuery query(model->database());
       if (!query.exec("DELETE FROM "+ model->tableName())) {  // 根据表名清空数据
           qDebug() << "Failed to clear rows: " << query.lastError().text();
           return false;
       }

       // 提交更改并刷新模型
       model->select();  // 重新从数据库中获取所有数据，并在视图中更新显示
       return true;
}

bool DatabaseManager::moveRowUp(int row)
{
    if (!model || row <= 0 || row >= model->rowCount()) {
        return false;
    }

    // 获取当前行和上一行的记录
    QSqlRecord currentRecord = model->record(row);
    QSqlRecord previousRecord = model->record(row - 1);

    // 只交换非主键字段（假设第0列是id）
    for (int i = 1; i < model->columnCount(); ++i) {  // 从1开始，跳过id列
        QVariant currentValue = currentRecord.value(i);
        QVariant previousValue = previousRecord.value(i);

        // 交换数据
        model->setData(model->index(row, i), previousValue);
        model->setData(model->index(row - 1, i), currentValue);
    }

    // 提交修改到数据库
    if (!model->submitAll()) {
        qDebug() << "Submit failed: " << model->lastError().text();
        return false;
    }

    return true;
}


bool DatabaseManager::moveRowDown(int row)
{
    if (!model || row < 0 || row >= model->rowCount() - 1) {
            return false;
    }

    // 获取当前行和下一行的记录
    QSqlRecord currentRecord = model->record(row);
    QSqlRecord nextRecord = model->record(row + 1);

    // 只交换非主键字段，比如 id 是主键，不进行交换
    for (int i = 1; i < model->columnCount(); ++i) {  // 从1开始，跳过第0列（假设第0列是id）
        QVariant currentValue = currentRecord.value(i);
        QVariant nextValue = nextRecord.value(i);

        // 交换数据
        model->setData(model->index(row, i), nextValue);
        model->setData(model->index(row + 1, i), currentValue);
    }

    // 提交修改
    if (!model->submitAll()) {
        qDebug() << "Submit failed: " << model->lastError().text();
        return false;
    }

    return true;
}


QString DatabaseManager::lastError() const
{
    return db.lastError().text();
}
