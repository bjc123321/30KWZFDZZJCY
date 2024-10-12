#include "threephaseplot.h"

#include <QDebug>

ThreePhasePlot::ThreePhasePlot(QWidget *parent) : QWidget(parent),
    frequency(1.0)
{

    // 创建QCustomPlot对象
    customPlot = new QCustomPlot(this);

    // 添加三条曲线
    customPlot->addGraph(); // Graph for y1
    customPlot->addGraph(); // Graph for y2
    customPlot->addGraph(); // Graph for y3

    customPlot->graph(0)->setPen(QPen(Qt::red));    // 红色曲线
    customPlot->graph(1)->setPen(QPen(Qt::green));  // 绿色曲线
    customPlot->graph(2)->setPen(QPen(Qt::blue));   // 蓝色曲线

    customPlot->xAxis->setLabel("时间 [s]");
    customPlot->yAxis->setLabel("电压 [V]");
    customPlot->yAxis->setRange(-100, 100);

    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    customPlot->yAxis->grid()->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));

    customPlot->legend->setVisible(true);
    customPlot->graph(0)->setName("电压 1");
    customPlot->graph(1)->setName("电压 2");
    customPlot->graph(2)->setName("电压 3");

    // 添加复选框用于选择显示的曲线
    checkBox1 = new QCheckBox("显示电压1", this);
    checkBox2 = new QCheckBox("显示电压2", this);
    checkBox3 = new QCheckBox("显示电压3", this);

    checkBox1->setChecked(true);
    checkBox2->setChecked(true);
    checkBox3->setChecked(true);

    connect(checkBox1, &QCheckBox::stateChanged, this, [this](bool checked) { showCurve(0, checked); });
    connect(checkBox2, &QCheckBox::stateChanged, this, [this](bool checked) { showCurve(1, checked); });
    connect(checkBox3, &QCheckBox::stateChanged, this, [this](bool checked) { showCurve(2, checked); });

    // 在主窗口布局中添加复选框
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    // 将复选框添加到布局中
    hLayout->addWidget(checkBox1);
    hLayout->addWidget(checkBox2);
    hLayout->addWidget(checkBox3);
    hLayout->addStretch();
    layout->addLayout(hLayout);
    layout->addWidget(customPlot);
    // 初始化数据处理器

    DataProcessor &dtProcessor = DataProcessor::getInstance();
    //后期要区分，updatePlot函数不可以更新同一个波形类
    connect(&dtProcessor, &DataProcessor::drawSignal, this, &ThreePhasePlot::updatePlot);

}

void ThreePhasePlot::startTuningFreqTest()
{
    qDebug()<<"开始整定频率波形";
    DataProcessor::getInstance().startProcessing();
}

void ThreePhasePlot::stopTuningFreqTest()
{
    qDebug()<<"停止整定频率波形";
    DataProcessor::getInstance().stopProcessing();
}

void ThreePhasePlot::updatePlot(double time, double y1, double y2, double y3)
{
    timeData.append(time);
    y1Data.append(y1);
    y2Data.append(y2);
    y3Data.append(y3);

    // 只保留最近100个数据点
    if (timeData.size() > 100) {
        timeData.remove(0);
        y1Data.remove(0);
        y2Data.remove(0);
        y3Data.remove(0);
    }

    // 更新图形数据
    customPlot->graph(0)->setData(timeData, y1Data);
    customPlot->graph(1)->setData(timeData, y2Data);
    customPlot->graph(2)->setData(timeData, y3Data);

    // 设置曲线可见性
    customPlot->graph(0)->setVisible(checkBox1->isChecked());
    customPlot->graph(1)->setVisible(checkBox2->isChecked());
    customPlot->graph(2)->setVisible(checkBox3->isChecked());

    // 设置x轴范围
    customPlot->xAxis->setRange(timeData.first(), timeData.last());
    customPlot->yAxis->setRange(-1.5, 1.5);

    // 重新绘制图形
    customPlot->replot();
}


void ThreePhasePlot::showCurve(int index, bool show)
{
    customPlot->graph(index)->setVisible(show);
    customPlot->replot();
}


