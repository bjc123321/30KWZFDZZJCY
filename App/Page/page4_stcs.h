#ifndef PAGE4_STCS_H
#define PAGE4_STCS_H

#include <QWidget>
#include "Base/BaseFun/TimerPool/timerpool.h"

#include "App/CustomWidget/frequencyplotter.h"
namespace Ui {
class Page4_stcs;
}

class Page4_stcs : public QWidget
{
    Q_OBJECT

public:
    explicit Page4_stcs(QWidget *parent = 0);
    ~Page4_stcs();

    void init();
    void signalBind();


signals:
    void startSuddIncrease();
    void stopSuddLoad();

    void drawSuddLoadPlot(double U,double I,double F);


private slots:


private:
    Ui::Page4_stcs *ui;

    FrequencyPlotter *frePlot = nullptr;

    //此函数要包括显示数据和画波形图
    void displaySuddLoadView(QQueue<QString> dataStrQueue);

    //计算瞬态调整率和波动率相关数据
    void calculateTransientData();

};

#endif // PAGE4_STCS_H
