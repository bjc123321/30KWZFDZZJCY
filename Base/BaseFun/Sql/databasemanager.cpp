#include "databasemanager.h"
#include <QDebug>
#include <QApplication>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

#include "GlobalSettings.h"

QString GlobalSettings::sqlPath = QDir::currentPath() + "/Depend/Base/sql.db";

DatabaseManager::DatabaseManager(const QString &dbName)
{
    // 设置SQLite数据库文件连接
    db = QSqlDatabase::addDatabase("QSQLITE","sqlite");
    db.setDatabaseName(dbName);

    // 先打开数据库
    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
    }else{
        qDebug() << "数据库"<<db.databaseName()<<"成功打开";
    }

    //初始化
    initialize();
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
//    if (db.tables().isEmpty()) {
//        QSqlQuery query;
//        if (!query.exec("CREATE TABLE YSFZ ("
//                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                        "percentage INTEGER, "
//                        "power_factor REAL, "
//                        "duration INTEGER)")) {
//            qDebug() << "Error creating table: " << query.lastError().text();
//            return false;
//        }
//    }

    // 初始化模型
    model = getModel();


    // 初始化时只加载第一页数据，不加载所有数据。注意：model->select();和 loadMoreData_2(20);只能存在一个
//    loadMoreData_2(20);  // 例如界面刚打开只加载20行数据，仅限显示不可编辑


    return true;
}

void DatabaseManager::setModel(QSqlTableModel *sqlModel)
{
    model = sqlModel;
    qDebug()<<"模型的表名："<<model->tableName();
}

QSqlTableModel* DatabaseManager::getModel()
{

    return new QSqlTableModel(nullptr, db);
}


void DatabaseManager::loadMoreData_2(int batchSize)
{
    QString queryStr = QString("SELECT * FROM YSFZ LIMIT %1 OFFSET %2")
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
void DatabaseManager::addMultipleRows(QSqlTableModel *model,int numRows)
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

        if (!addRow(model,percentage, powerFactor, duration)) {
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


bool DatabaseManager::addRow(QSqlTableModel *model,int percentage, double powerFactor, int duration)
{
    if (!model) return false;

    qDebug() << "插入" << model->tableName()<<"表字段个数"<<model->columnCount();

    qDebug()<<"执行了";
    int row = model->rowCount();
    model->insertRow(row);
    model->setData(model->index(row, 1), percentage);
    model->setData(model->index(row, 2), powerFactor);
    model->setData(model->index(row, 3), duration);

    return model->submitAll();
}

bool DatabaseManager::removeRow(QSqlTableModel *model,int row)
{
    if (!model || row < 0 || row >= model->rowCount()) {

        if(!model){
            qDebug()<<"model为空";
        }else{
            qDebug()<<"行数不合法";
        }

        return false;
    }

    model->removeRow(row);
    if (!model->submitAll()) {
        qDebug() << "Error submitting changes:" << model->lastError().text();
        model->revertAll();
        return false;
    }
    model->select();  // 强制刷新模型，确保视图更新
    return true;
}

bool DatabaseManager::clearRows(QSqlTableModel *model)
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

bool DatabaseManager::moveRowUp(QSqlTableModel *model,int row)
{
    if (!model || row <= 0 || row >= model->rowCount()) {
        qDebug()<<"model为空";
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


bool DatabaseManager::moveRowDown(QSqlTableModel *model,int row)
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

QSqlQueryModel* DatabaseManager::queryRecordNum(QString id)
{

    QSqlQueryModel *queryModel = new QSqlQueryModel;
    qDebug() << "查询记录编号为" << id << "的数据";

    // 确保数据库连接有效
    if (!model->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return nullptr;
    }

    //以下4条缺一不可
    QString queryStr = QString("SELECT * FROM T_static_data WHERE 测试编号 = :id");
    QSqlQuery query(queryStr,model->database()); // 直接使用数据库连接
    query.prepare(queryStr); // 使用 prepare 方法准备查询
    query.bindValue(":id", id); // 绑定参数，防止sql注入

    if (query.exec()) {
        qDebug() << "执行成功";
        while (query.next()) { // 如果有记录返回 true
            QString date = query.value("测试日期").toString();
            QString time = query.value("测试时长").toString();
            // 其他字段...
            qDebug() << "测试日期:" << date << "测试时长:" << time;

        }

        queryModel->setQuery(query); // 设置查询结果到模型

    } else {
        qDebug() << "查询执行失败: " << query.lastError().text();
        return nullptr;
    }

    return queryModel;

}

QSqlQueryModel* DatabaseManager::queryRecord(QSqlTableModel *model,QString queryCondition)
{

    QSqlQueryModel *queryModel = new QSqlQueryModel;
    // 确保在使用之前 model 已被初始化
    if (!model || !model->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return nullptr;
    }

    // 构建查询字符串
    QString queryStr = QString("SELECT * FROM %1").arg(model->tableName());
    if (!queryCondition.isEmpty()) {
        queryStr += " WHERE " + queryCondition;
    }

    // 使用 QSqlQuery 执行查询
    QSqlQuery query(queryStr,model->database());
    if (!query.exec()) {
        qDebug() << "查询执行失败: " << query.lastError().text();
        return nullptr;
    }

    // 将查询结果设置到模型
    queryModel->setQuery(query);

    qDebug() << "执行成功,查到" << queryModel->rowCount() << "条数据";

    return queryModel;
}

bool DatabaseManager::delRecord(QSqlTableModel *model ,QString columnName,QString delCondition)
{

    qDebug()<<"操作的表名:"<<model->tableName()<<"删除的条件是测试编号为:"<<delCondition;

    QString sqlStr = "DELETE FROM "+model->tableName()+" WHERE " +columnName+" = "+delCondition;

    QSqlQuery query(sqlStr,model->database());

    bool ret = query.exec(sqlStr);
    if(ret){
        qDebug() << "执行成功,删除数据";
    }else{

        qDebug() << "删除操作失败";
    }
    return ret;
}


QSqlQueryModel* DatabaseManager::queryT_dataPart()
{

    QSqlQueryModel *queryModel = new QSqlQueryModel;

    // 确保数据库连接有效
    if (!model->database().isOpen()) {
        qDebug() << "数据库未打开，无法查询。";
        return nullptr;
    }

    // 假设你已经有一个 QSqlDatabase 连接
    QStringList fields; // 用于存储要查询的字段
    fields << "测试编号" << "产品名称" << "测试类型"<< "检测人员"<< "检测时间"<< "检测结果"; // 替换为你需要查询的字段名称

    // 创建 SQL 查询语句
    QString queryString = QString("SELECT  %1 FROM T_data").arg(fields.join(", ")); // 将字段名称以逗号分隔连接

    QSqlQuery query(queryString,model->database()); // 直接使用数据库连接
    query.prepare(queryString); // 使用 prepare 方法准备查询

    if (query.exec()) {
        qDebug() << "执行成功";
        while (query.next()) { // 如果有记录返回 true
            QString testNum = query.value("测试编号").toString();
//            QString testPerson = query.value("检测人员").toString();
            // 其他字段...
            qDebug() << "测试编号:" << testNum ;

        }

        queryModel->setQuery(query); // 设置查询结果到模型

    } else {
        qDebug() << "查询执行失败: " << query.lastError().text();
        return nullptr;
    }

    return queryModel;

}

bool DatabaseManager::insertData(QSqlTableModel *model, QVector<QVariant> vdata)
{
    if (!model) {
        qDebug() << "insertData错误: model is null";
        return false;
    }

    qDebug() << "插入" << model->tableName() << "字段个数:" << vdata.size()<<"表的字段个数"<<model->columnCount();
    // 检查 vdata 的大小
    if (vdata.size() != model->columnCount()) { // 看数据字段是否与表字段数量一致
        qDebug() << "插入失败: 数据字段数量与表字段数量不匹配";
        return false;
    }

    int row = model->rowCount();
    model->insertRow(row);

    // 设置数据
    for (int i = 0; i < vdata.size(); ++i) {
        model->setData(model->index(row, i), vdata.at(i));
    }

    // 提交修改
    if (!model->submitAll()) {
        qDebug() << "Submit failed: " << model->lastError().text();
        return false;
    }

    qDebug() << "插入" << vdata.at(0) << "数据到" << model->tableName();

    return true;

}


QString DatabaseManager::lastError() const
{
    return db.lastError().text();
}
