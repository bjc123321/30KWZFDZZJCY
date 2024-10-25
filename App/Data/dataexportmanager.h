#ifndef DATAEXPORTMANAGER_H
#define DATAEXPORTMANAGER_H

#include <QObject>
#include "ExcelSrc/xlsx/xlsxdocument.h"

class DataExportManager : public QObject
{
    Q_OBJECT
public:
    explicit DataExportManager(QObject *parent = 0);

    // 获取单例实例的静态方法,静态局部变量的初始化是线程安全的
    static DataExportManager& getInstance(){

        static DataExportManager instance; // 静态实例对象
        return instance;
    }

    //确保 DataExportManager 类在整个程序中只有一个实例,符合单例模式的设计意图
    DataExportManager(const DataExportManager&) = delete;//删除拷贝构造函数,防止实例被复制
    DataExportManager& operator=(const DataExportManager&) = delete;//删除赋值操作符,防止实例被复制

    void createStaticTestExcel();  // 创建稳态测试Excel表格的函数

    void generateSteadyDataExcel(QVector<QString > v_Data);//生成填好数据的稳态表格

private:

signals:

public slots:
};

#endif // DATAEXPORTMANAGER_H
