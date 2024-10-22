#ifndef PAGE4_STCS_H
#define PAGE4_STCS_H

#include <QWidget>
#include "Base/BaseFun/TimerPool/timerpool.h"

#include "App/CustomWidget/frequencyplotter.h"
#include "App/CustomWidget/threephaseplot.h"

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

    void readSuddLoad800YSignal();


private slots:


private:
    Ui::Page4_stcs *ui;



    FrequencyPlotter *frePlot = nullptr;
    ThreePhasePlot *threePlot = nullptr;

    //显示时钟标签
    void displayClockSlot(int t_elapsedTime);
    //此函数要包括显示数据和画波形图
    void displaySuddLoadWaveSlot(QQueue<QString> dataStrQueue);

    //保存数据，根据当前tab页显示，默认保存一份数据。先弹出保存页面，在保存页面编辑好再点保存按钮
    void saveCurrentData(QString dataType);



    void saveSuddLoadData();
    void saveSuddUnLoadData();



    //计算瞬态调整率和波动率相关数据
    void calculateTransientData();

};

#endif // PAGE4_STCS_H
