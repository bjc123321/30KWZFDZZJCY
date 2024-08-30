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
    void initSetFzModel(QSqlTableModel* modelPtr,QString sqlData,QTableView*tableView,QPushButton*add,QPushButton*del,QPushButton*up,QPushButton*down,
                        QPushButton*set);
    void initAutoFzModel(QSqlTableModel* modelPtr,QString sqlData,QTableView*tableView);

private slots:


private:
    Ui::Page1_fzsz *ui;
};

class CustomItemDelegate : public QItemDelegate {
public:
    CustomItemDelegate(const QRegExp& regExp, QObject* parent = nullptr)
        : QItemDelegate(parent), m_regExp(regExp) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (index.column() == 0) // 第一列禁止输入
            return nullptr;
        else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setValidator(new QRegExpValidator(m_regExp, editor));
            return editor;
        }
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
            lineEdit->setText(index.data(Qt::EditRole).toString());
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor))
            model->setData(index, lineEdit->text(), Qt::EditRole);
    }

private:
    QRegExp m_regExp;
};

#endif // PAGE1_FZSZ_H
