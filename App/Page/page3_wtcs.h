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

    void signalBind();
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
