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

    void signalBind();
    void displayData(QQueue<QString> dataStrQueue);//界面上的显示
    QButtonGroup *buttonGroup = nullptr;

    QVector<QLineEdit *> lineEdits;

    QVector<QLineEdit *> tuningDataEdits;

signals:


private:

    Ui::Page3_wtcs *ui;
    FrequencyPlotter *freqPlot = nullptr;
    VoltagePlot *voltageWidget = nullptr;

    void initSteadyUI();
    //初始化整定页面UI
    void initTuningUI();
    // 假设这是你查找 QTabWidget 中 QLineEdit 的函数
    void findAllLineEditsInTab(QTabWidget *tabWidget, int tabIndex);

public slots:




};

#endif // PAGE3_WTCS_H
