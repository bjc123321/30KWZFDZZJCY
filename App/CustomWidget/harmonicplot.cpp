#include "harmonicplot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

HarmonicPlot::HarmonicPlot(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupPlots();
    updateBarsDataAndColor(0);  // 初始化为A相
}


void HarmonicPlot::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建电压和电流的QCustomPlot对象
    voltagePlot = new QCustomPlot(this);
    currentPlot = new QCustomPlot(this);
    mainLayout->addWidget(voltagePlot);
    mainLayout->addWidget(currentPlot);

    // 创建三角形滑块的自定义滑动条
    slider = new TriangleSlider(Qt::Horizontal, this);
    slider->setRange(1, 23);
    slider->setSingleStep(1);


    // 将滑动条放在图表下方，并使其与X轴对齐
    mainLayout->addWidget(slider, 0, Qt::AlignHCenter);
    mainLayout->setContentsMargins(5, 0, 5, 0);
    mainLayout->setSpacing(5);
    setLayout(mainLayout);

    slider->setStyleSheet(R"(
      QSlider::groove:horizontal {
             border: 1px solid #999999;
             background: #cccccc;
             height: 10px;
             margin: 0px 0;
         }
         QSlider::handle:horizontal {
             width: 0px;
         }
    )");

    // 连接滑动条值变化的信号到槽函数
    connect(slider, &QSlider::valueChanged, this, &HarmonicPlot::onSliderValueChanged);

    connect(currentPlot->xAxis, static_cast<void(QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
            this, &HarmonicPlot::updateSliderWidth);
}

void HarmonicPlot::setupPlots()
{
    voltageBars = new QCPBars(voltagePlot->xAxis, voltagePlot->yAxis);
    voltagePlot->xAxis->setRange(0, 24);
    voltagePlot->yAxis->setRange(0, 2);
    voltagePlot->yAxis->setLabel("U [%]"); // 设置 y 轴标签为 "U [%]相对值"

    currentBars = new QCPBars(currentPlot->xAxis, currentPlot->yAxis);
    currentPlot->xAxis->setRange(0, 24);
    currentPlot->yAxis->setRange(0, 0.02);
    currentPlot->yAxis->setLabel("I [%]"); // 设置 y 轴标签为 "I [%]相对值"

    voltagePlot->yAxis->setTickLabels(false);
    currentPlot->yAxis->setTickLabels(false);



    // 保存默认颜色
    defaultVoltagePen = QPen(Qt::darkRed);
    defaultVoltageBrush = QBrush(Qt::darkRed);
    defaultCurrentPen = QPen(Qt::darkGreen);
    defaultCurrentBrush = QBrush(Qt::darkGreen);

    voltageBars->setPen(defaultVoltagePen);
    voltageBars->setBrush(defaultVoltageBrush);
    currentBars->setPen(defaultCurrentPen);
    currentBars->setBrush(defaultCurrentBrush);

    connect(voltagePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), currentPlot->xAxis, SLOT(setRange(QCPRange)));
    connect(currentPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), voltagePlot->xAxis, SLOT(setRange(QCPRange)));

    voltagePlot->replot();
    currentPlot->replot();
}


void HarmonicPlot::updateBarsDataAndColor(int phaseIndex)
{
    QVector<double> voltageData(23), currentData(23);
    QVector<double> ticks(23);
        // 清空现有数据并添加新数据
        voltageBars->data()->clear();
        currentBars->data()->clear();

    for (int i = 0; i < 23; ++i) {
        ticks[i] = i + 1;
        if (phaseIndex == 0) {  // A相
            voltageData[i] = 1.0 + 0.05 * (i % 3);
            currentData[i] = 0.01 + 0.001 * (i % 3);
        } else if (phaseIndex == 1) {  // B相
            voltageData[i] = 1.2 + 0.05 * (i % 3);
            currentData[i] = 0.013 + 0.001 * (i % 3);
        } else if (phaseIndex == 2) {  // C相
            voltageData[i] = 1.3 + 0.05 * (i % 3);
            currentData[i] = 0.014 + 0.001 * (i % 3);
        }
        voltageBars->addData(ticks[i], voltageData[i]);
        currentBars->addData(ticks[i], currentData[i]);
    }


    voltagePlot->replot();
    currentPlot->replot();
}

void HarmonicPlot::updateTracers(int value)
{
    // 清除现有的游标
    voltagePlot->clearItems();
    currentPlot->clearItems();


    voltagePlot->replot();
    currentPlot->replot();

    // 更新显示的电压和电流值
    double voltageValue = voltageBars->data()->at(value - 1)->value;
    double currentValue = currentBars->data()->at(value - 1)->value;
    emit updateVolAndCurValue(voltageValue,currentValue);
}

void HarmonicPlot::onPhaseChanged(int index)
{
    updateBarsDataAndColor(index);
}

void HarmonicPlot::onSliderValueChanged(int value)
{
    updateTracers(value);
}

void HarmonicPlot::updateSliderWidth(const QCPRange &range)
{
    int plotWidth = voltagePlot->axisRect()->width();
    slider->setFixedWidth(plotWidth);
}

void HarmonicPlot::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateSliderWidth(voltagePlot->xAxis->range());
}

void HarmonicPlot::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSliderWidth(voltagePlot->xAxis->range());
}
