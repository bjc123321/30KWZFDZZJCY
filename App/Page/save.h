#ifndef SAVE_H
#define SAVE_H

#include <QDialog>
#include <QSqlTableModel>
#include "Base/BaseFun/base.h"
namespace Ui {
class Save;
}

class Save : public QDialog
{
    Q_OBJECT
    B_INSTANCE(Save)
public:
    explicit Save(QWidget *parent = 0);
    ~Save();

public:

    // 定义结构体
    struct DetectionRecord {
        QString detectionID;    // 检测编号
        QString detectionComponent; // 检测元件
        QString type;           // 类型
        QString inspector;      // 检测人员
        QString detectionTime; // 检测时间
        QString result;         // 检测结果

        // 构造函数
        DetectionRecord(const QString &id, const QString &component, const QString &type,
                        const QString &inspector, const QString &time, const QString &result)
            : detectionID(id), detectionComponent(component), type(type),
              inspector(inspector), detectionTime(time), result(result) {}
    };

    // 使用 typedef 为结构体创建别名
    typedef DetectionRecord Detection;

public:

    QSqlTableModel *model = nullptr;

    void updateT_dataView(Detection record);
    void saveTestRecord();
    void bindDataBaseTable();

private:
    Ui::Save *ui;
};

#endif // SAVE_H
