#ifndef PAGE4_STCS_H
#define PAGE4_STCS_H

#include <QWidget>
#include "Base/BaseFun/TimerPool/timerpool.h"
namespace Ui {
class Page4_stcs;
}

class Page4_stcs : public QWidget
{
    Q_OBJECT

public:
    explicit Page4_stcs(QWidget *parent = 0);
    ~Page4_stcs();
    init();
    void dealTime(bool isStart);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Page4_stcs *ui;
};

#endif // PAGE4_STCS_H
