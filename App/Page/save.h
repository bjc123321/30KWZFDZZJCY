#ifndef SAVE_H
#define SAVE_H

#include <QDialog>
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

private:
    Ui::Save *ui;
};

#endif // SAVE_H
