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


signals:

    void startRecordWaveSignal();//启动录波测试信号
    void readRecordWaveSignal();//读取录波数据

private:
    Ui::Page6_lbfx *ui;


};

#endif // PAGE6_LBFX_H
