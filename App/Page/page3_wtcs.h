#ifndef PAGE3_WTCS_H
#define PAGE3_WTCS_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QButtonGroup>


#include "App/Data/datatest.h"
#include "App/CustomWidget/voltageplot.h"
#include "App/CustomWidget/frequencyplotter.h"
#include "App/Data/dataprocessor.h"
#include "App/Data/dataexportmanager.h"



namespace Ui {
class Page3_wtcs;
}

class Page3_wtcs : public QWidget
{
    Q_OBJECT

public:
    explicit Page3_wtcs(QWidget *parent = 0);
    ~Page3_wtcs();

public:

    // 定义结构体
    struct DetailRecord {

        QString t_id;
        QString t_date;
        QString t_last;
        QString t_load;
        QString t_power;
        QString t_power_factor;
        QString t_I1;
        QString t_I2;
        QString t_I3;
        QString t_U1;
        QString t_U2;
        QString t_U3;
        QString t_aveU;
        QString t_maxU;
        QString t_minU;
        QString t_aveF;
        QString t_maxF;
        QString t_minF;
        QString t_steadyVolReg;
        QString t_volWaveReg;
        QString t_steadyFreReg;
        QString t_freWaveReg;

        // 构造函数
        DetailRecord(const QString &t_id, const QString &t_date, const QString &t_last,
                     const QString &t_load, const QString &t_power, const QString &t_power_factor,
                     const QString &t_I1, const QString &t_I2, const QString &t_I3,
                     const QString &t_U1, const QString &t_U2, const QString &t_U3,
                     const QString &t_aveU, const QString &t_maxU, const QString &t_minU,
                     const QString &t_aveF, const QString &t_maxF, const QString &t_minF,
                     const QString &t_steadyVolReg, const QString &t_volWaveReg,
                     const QString &t_steadyFreReg, const QString &t_freWaveReg)
            : t_id(t_id), t_date(t_date), t_last(t_last), t_load(t_load), t_power(t_power),
              t_power_factor(t_power_factor), t_I1(t_I1), t_I2(t_I2), t_I3(t_I3),
              t_U1(t_U1), t_U2(t_U2), t_U3(t_U3), t_aveU(t_aveU), t_maxU(t_maxU),
              t_minU(t_minU), t_aveF(t_aveF), t_maxF(t_maxF), t_minF(t_minF),
              t_steadyVolReg(t_steadyVolReg), t_volWaveReg(t_volWaveReg),
              t_steadyFreReg(t_steadyFreReg), t_freWaveReg(t_freWaveReg){}

    };


    // 使用 typedef 为结构体创建别名
    typedef DetailRecord DETAIL_RECORD;

public:

    //其他信号绑定
    void signalBind();
    //文本框被改变时数据信号槽绑定
    void lineEditsChangedBind();

    void displaySteadyData(QQueue<QString> dataStrQueuex);//显示某页数据
    QButtonGroup *buttonGroup = nullptr;

    QVector<QLineEdit *> lineEdits;

    QVector<QLineEdit *> tuningDataEdits;

    float unLoad_VolMax = 0.000;
    float unLoad_VolMin = 230.000;
    float load_VolMax = 0.000;
    float load_VolMin = 230.000;

    float diffValue = 0.0;

    float unLoad_FluMax = 0.00;
    float unLoad_FluMin = 50.00;
    float load_FluMax = 0.00;
    float load_FluMin = 50.00;

private:

    Ui::Page3_wtcs *ui;
    FrequencyPlotter *freqPlot = nullptr;
    VoltagePlot *voltageWidget = nullptr;

    void initSteadyUI();
    //初始化整定页面UI
    void initTuningUI();
    // 假设这是你查找 QTabWidget 中 QLineEdit 的函数
    void findAllLineEditsInTab(QTabWidget *tabWidget, int tabIndex);

    //计算稳态调整率和波动率相关数据
    void calculateSteadyData();


    void saveSteadyData();


public slots:



};

#endif // PAGE3_WTCS_H
