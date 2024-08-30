#ifndef PAGE5_XBFX_H
#define PAGE5_XBFX_H

#include <QWidget>
#include "App/CustomWidget/harmonicplot.h"

namespace Ui {
class Page5_xbfx;
}

class Page5_xbfx : public QWidget
{
    Q_OBJECT

public:
    explicit Page5_xbfx(QWidget *parent = 0);
    ~Page5_xbfx();
    void init();
    void signalBind();
private:
    Ui::Page5_xbfx *ui;

    HarmonicPlot *harmoPlot = nullptr;
};

#endif // PAGE5_XBFX_H
