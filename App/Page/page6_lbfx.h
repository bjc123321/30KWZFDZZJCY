#ifndef PAGE6_LBFX_H
#define PAGE6_LBFX_H

#include <QWidget>
#include "App/CustomWidget/recordwaveplot.h"
namespace Ui {
class Page6_lbfx;
}

class Page6_lbfx : public QWidget
{
    Q_OBJECT

public:
    explicit Page6_lbfx(QWidget *parent = 0);
    ~Page6_lbfx();
    init();

    RecordWavePlot *recordWave = nullptr;

private:
    Ui::Page6_lbfx *ui;
};

#endif // PAGE6_LBFX_H
