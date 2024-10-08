#include "recordwaveplot.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QPen>
#include <cmath>

RecordWavePlot::RecordWavePlot(QWidget *parent) : QWidget(parent)
{
    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建复选框
    QCheckBox *checkBoxA = new QCheckBox("Show Phase A", this);
    QCheckBox *checkBoxB = new QCheckBox("Show Phase B", this);
    QCheckBox *checkBoxC = new QCheckBox("Show Phase C", this);

    QHBoxLayout *hLayout = new QHBoxLayout(this);

    // 将复选框添加到布局中
    hLayout->addWidget(checkBoxA);
    hLayout->addWidget(checkBoxB);
    hLayout->addWidget(checkBoxC);
    hLayout->addStretch();
    layout->addLayout(hLayout);


    // 创建QCustomPlot实例
    customPlot = new QCustomPlot(this);
    layout->addWidget(customPlot);

    // 创建滑块
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);  // 设置滑块的范围为0到100
    layout->addWidget(slider);


    // 初始化复选框状态为勾选
    checkBoxA->setChecked(true);
    checkBoxB->setChecked(true);
    checkBoxC->setChecked(true);

    // 生成三相电压数据并美化图表
    generateThreePhaseVoltage();
    beautifyPlot();

    // 创建竖直线
        verticalLine = new QCPItemLine(customPlot);
        verticalLine->setVisible(false);  // 初始不可见
        verticalLine->start->setCoords(0, -320);
        verticalLine->end->setCoords(0, 320);
        verticalLine->setPen(QPen(Qt::DashLine));

        // 创建交点标签
        labelPhaseA = new QCPItemText(customPlot);
        labelPhaseB = new QCPItemText(customPlot);
        labelPhaseC = new QCPItemText(customPlot);

        labelPhaseA->setVisible(false);
        labelPhaseB->setVisible(false);
        labelPhaseC->setVisible(false);

    // 连接复选框的状态改变信号到槽函数
    connect(checkBoxA, &QCheckBox::stateChanged, this, &RecordWavePlot::togglePhaseA);
    connect(checkBoxB, &QCheckBox::stateChanged, this, &RecordWavePlot::togglePhaseB);
    connect(checkBoxC, &QCheckBox::stateChanged, this, &RecordWavePlot::togglePhaseC);
    // 连接滑块的值改变信号到槽函数
       connect(slider, &QSlider::valueChanged, this, &RecordWavePlot::handleSliderValueChanged);

       // 获取图表的宽度
       double plotWidth = customPlot->width(); // 获取绘图区域的宽度

       // 打印 X 轴的实际宽度
       qDebug() << "X 轴实际宽度：" << plotWidth;
       qDebug() << "slider宽度：" << slider->width();

}


void RecordWavePlot::mousePressEvent(QMouseEvent *event)
{
    // 获取点击的x坐标在plot中的实际值
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());

    // 更新竖直线的位置
    verticalLine->start->setCoords(x, -320);
    verticalLine->end->setCoords(x, 320);
    verticalLine->setVisible(true);

    // 更新交点
    updateIntersectionPoints(x);

    // 更新滑块的值（根据x的范围0到1转换为0到100）
    slider->blockSignals(true);  // 防止滑块更新时发出信号
    slider->setValue(static_cast<int>(x * 100));
    slider->blockSignals(false);

    customPlot->replot();
}

void RecordWavePlot::mouseMoveEvent(QMouseEvent *event)
{
    if (verticalLine->visible()) // 当线段可见时允许拖动
    {
        // 获取当前鼠标位置的x坐标
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());

        // 更新竖直线的位置
        verticalLine->start->setCoords(x, -320);
        verticalLine->end->setCoords(x, 320);

        // 更新交点
        updateIntersectionPoints(x);

        // 更新滑块的值
        slider->blockSignals(true);
        slider->setValue(static_cast<int>(x * 100));
        slider->blockSignals(false);

        customPlot->replot();
    }
}

void RecordWavePlot::handleSliderValueChanged(int value)
{
    // 将滑块的值（0到100）映射为图表中的x坐标（0到1）
    double x = value / 100.0;

    // 更新竖直线的位置
    verticalLine->start->setCoords(x, -320);
    verticalLine->end->setCoords(x, 320);
    verticalLine->setVisible(true);

    // 更新交点
    updateIntersectionPoints(x);

    customPlot->replot();
}
void RecordWavePlot::togglePhaseA(int state)
{
    customPlot->graph(0)->setVisible(state == Qt::Checked);
    customPlot->replot();
}

void RecordWavePlot::togglePhaseB(int state)
{
    customPlot->graph(1)->setVisible(state == Qt::Checked);
    customPlot->replot();
}

void RecordWavePlot::togglePhaseC(int state)
{
    customPlot->graph(2)->setVisible(state == Qt::Checked);
    customPlot->replot();
}

double RecordWavePlot::getYValueAtX(QCPGraph *graph, double x)
{
    // 查找与指定x最接近的y值
    int n = graph->dataCount();
    if (n == 0) return 0;

    double y = 0;
    for (int i = 0; i < n - 1; ++i)
    {
        double x1 = graph->data()->at(i)->key;
        double x2 = graph->data()->at(i + 1)->key;
        if (x >= x1 && x <= x2)
        {
            double y1 = graph->data()->at(i)->value;
            double y2 = graph->data()->at(i + 1)->value;
            y = y1 + (y2 - y1) * (x - x1) / (x2 - x1); // 线性插值
            break;
        }
    }
    return y;
}

void RecordWavePlot::updateIntersectionPoints(double x)
{
    // 获取每个曲线在x位置的y值
    double yA = getYValueAtX(customPlot->graph(0), x);
    double yB = getYValueAtX(customPlot->graph(1), x);
    double yC = getYValueAtX(customPlot->graph(2), x);

    // 更新文本标签
    labelPhaseA->setText(QString("Phase A: %1 V").arg(yA, 0, 'f', 2));
    labelPhaseB->setText(QString("Phase B: %1 V").arg(yB, 0, 'f', 2));
    labelPhaseC->setText(QString("Phase C: %1 V").arg(yC, 0, 'f', 2));

    // 设置文本标签的位置
    labelPhaseA->position->setCoords(x, yA);
    labelPhaseB->position->setCoords(x, yB);
    labelPhaseC->position->setCoords(x, yC);

    labelPhaseA->setVisible(true);
    labelPhaseB->setVisible(true);
    labelPhaseC->setVisible(true);
}

void RecordWavePlot::generateThreePhaseVoltage()
{
    // 设置x轴和y轴的标签
    customPlot->xAxis->setLabel("Time [s]");
    customPlot->yAxis->setLabel("Voltage [V]");

    // 生成数据
    int numPoints = 50; // 每个相有50个点
    QVector<double> time(numPoints);
    QVector<double> phaseA(numPoints), phaseB(numPoints), phaseC(numPoints);

    double frequency = 50.0;  // 50Hz
    double amplitude = 311.0; // 311V，峰值
    double timeInterval = 0.02;  // 20ms

    for (int i = 0; i < numPoints; ++i)
    {
        time[i] = i * timeInterval;
        phaseA[i] = amplitude * qSin(2 * 3.0578 * frequency * time[i]);               // A相
        phaseB[i] = amplitude * qSin(2 * 3.1415 * frequency * time[i] - 2 * M_PI / 3); // B相，滞后120度
        phaseC[i] = amplitude * qSin(2 * 3.2568 * frequency * time[i] + 2 * M_PI / 3); // C相，超前120度
    }

    // 添加A相数据到QCustomPlot
    customPlot->addGraph();
    customPlot->graph(0)->setData(time, phaseA);
    customPlot->graph(0)->setPen(QPen(Qt::red));  // A相用红色表示
    customPlot->graph(0)->setName("Phase A");

    // 添加B相数据到QCustomPlot
    customPlot->addGraph();
    customPlot->graph(1)->setData(time, phaseB);
    customPlot->graph(1)->setPen(QPen(Qt::green));  // B相用绿色表示
    customPlot->graph(1)->setName("Phase B");

    // 添加C相数据到QCustomPlot
    customPlot->addGraph();
    customPlot->graph(2)->setData(time, phaseC);
    customPlot->graph(2)->setPen(QPen(Qt::blue));  // C相用蓝色表示
    customPlot->graph(2)->setName("Phase C");

    // 设置x轴范围，显示1秒的数据
    customPlot->xAxis->setRange(0, 1);

    // 设置y轴范围，电压范围为-320到320V
    customPlot->yAxis->setRange(-320, 320);

    customPlot->replot();  // 重新绘制图表
}

void RecordWavePlot::beautifyPlot()
{
    // 设置背景颜色
    customPlot->setBackground(QBrush(QColor(240, 240, 240)));

    // 设置网格线
    customPlot->xAxis->grid()->setVisible(true);      // 显示x轴网格线
    customPlot->xAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DotLine));  // 使用灰色虚线
    customPlot->yAxis->grid()->setVisible(true);      // 显示y轴网格线
    customPlot->yAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DotLine));  // 使用灰色虚线

    // 设置坐标轴的刻度样式
    customPlot->xAxis->setTickLabelFont(QFont("Arial", 10));
    customPlot->yAxis->setTickLabelFont(QFont("Arial", 10));
    customPlot->xAxis->setLabelFont(QFont("Arial", 12));
    customPlot->yAxis->setLabelFont(QFont("Arial", 12));

    // 设置轴的线条颜色和宽度
    customPlot->xAxis->setBasePen(QPen(Qt::black, 2));
    customPlot->yAxis->setBasePen(QPen(Qt::black, 2));
    customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::black, 1));

    // 调整图例样式
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 150))); // 半透明背景
    customPlot->legend->setFont(QFont("Arial", 9));
    customPlot->legend->setBorderPen(QPen(Qt::black));  // 图例边框
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);  // 图例位置

    // 重新绘制图表
    customPlot->replot();
}
