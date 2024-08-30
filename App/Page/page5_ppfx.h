#ifndef PAGE5_PPFX_H
#define PAGE5_PPFX_H

#include <QWidget>

namespace Ui {
class page5_ppfx;
}

class page5_ppfx : public QWidget
{
    Q_OBJECT

public:
    explicit page5_ppfx(QWidget *parent = 0);
    ~page5_ppfx();

private:
    Ui::page5_ppfx *ui;
};

#endif // PAGE5_PPFX_H
