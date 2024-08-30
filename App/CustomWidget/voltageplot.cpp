#include "voltageplot.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <cmath>
#include <QDebug>
VoltagePlot::VoltagePlot(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupPlot();

    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &VoltagePlot::updateData);

}

void VoltagePlot::startFlucPlot()
{
    qDebug()<<"波形开始";
    dataTimer->start(100); // Update every 100 ms
}

void VoltagePlot::stopFlucPlot()
{
    qDebug()<<"波形停止";
    dataTimer->stop(); // Update every 100 ms
}

void VoltagePlot::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    checkBoxA = new QCheckBox("Show U_a", this);
    checkBoxB = new QCheckBox("Show U_b", this);
    checkBoxC = new QCheckBox("Show U_c", this);

    checkBoxA->setChecked(true);
    checkBoxB->setChecked(true);
    checkBoxC->setChecked(true);

    layout->addWidget(checkBoxA);
    layout->addWidget(checkBoxB);
    layout->addWidget(checkBoxC);

    customPlot = new QCustomPlot(this);
    layout->addWidget(customPlot);

    connect(checkBoxA, &QCheckBox::toggled, this, [this](bool checked) { showCurve(0, checked); });
    connect(checkBoxB, &QCheckBox::toggled, this, [this](bool checked) { showCurve(1, checked); });
    connect(checkBoxC, &QCheckBox::toggled, this, [this](bool checked) { showCurve(2, checked); });
}

void VoltagePlot::setupPlot()
{
    int n = 50; // 数据量
    x.resize(n); // 调整向量 x 的大小为 n
    u_a.resize(n); // 调整向量 u_a 的大小为 n
    u_b.resize(n); // 调整向量 u_b 的大小为 n
    u_c.resize(n); // 调整向量 u_c 的大小为 n

    // 填充 x 的值，从 0 到 2π 均匀分布
    for (int i = 0; i < n; ++i)
    {
        x[i] = i / static_cast<double>(n) * 2 * M_PI;
    }

    customPlot->addGraph(); // 添加第一个图表
    customPlot->graph(0)->setData(x, u_a); // 将 x 和 u_a 数据集分配给第一个图表
    customPlot->graph(0)->setPen(QPen(Qt::red)); // 设置第一个图表的颜色为红色
    customPlot->graph(0)->setName("U_a"); // 设置第一个图表的名称为 U_a

    customPlot->addGraph(); // 添加第二个图表
    customPlot->graph(1)->setData(x, u_b); // 将 x 和 u_b 数据集分配给第二个图表
    customPlot->graph(1)->setPen(QPen(Qt::green)); // 设置第二个图表的颜色为绿色
    customPlot->graph(1)->setName("U_b"); // 设置第二个图表的名称为 U_b

    customPlot->addGraph(); // 添加第三个图表
    customPlot->graph(2)->setData(x, u_c); // 将 x 和 u_c 数据集分配给第三个图表
    customPlot->graph(2)->setPen(QPen(Qt::blue)); // 设置第三个图表的颜色为蓝色
    customPlot->graph(2)->setName("U_c"); // 设置第三个图表的名称为 U_c

    customPlot->xAxis->setLabel("Time [s]"); // 设置 x 轴标签为 "Time [s]"
    customPlot->yAxis->setLabel("Voltage [V]"); // 设置 y 轴标签为 "Voltage [V]"
    customPlot->legend->setVisible(true); // 显示图例
    customPlot->rescaleAxes(); // 自动调整坐标轴的范围
}

void VoltagePlot::showCurve(int index, bool show)
{
    customPlot->graph(index)->setVisible(show);
    customPlot->replot();
}

void VoltagePlot::updateData()
{
    int n = x.size(); // 获取 x 向量的大小，即数据点的数量
    double phase = currentIndex / static_cast<double>(n) * 2 * M_PI; // 计算相位，用于信号的平移

    // 计算每个数据点的 u_a、u_b 和 u_c 值
    for (int i = 0; i < n; ++i)
    {
        u_a[i] = std::sin(x[i] + phase); // 计算 u_a 的值，使用相位平移后的正弦函数
        u_b[i] = std::sin(x[i] + phase - 2 * M_PI / 3); // 计算 u_b 的值，相位平移并减去 2π/3
        u_c[i] = std::sin(x[i] + phase + 2 * M_PI / 3); // 计算 u_c 的值，相位平移并加上 2π/3
    }

    customPlot->graph(0)->setData(x, u_a); // 更新第一个图表的数据
    customPlot->graph(1)->setData(x, u_b); // 更新第二个图表的数据
    customPlot->graph(2)->setData(x, u_c); // 更新第三个图表的数据
    customPlot->replot(); // 重新绘制图表

    currentIndex = (currentIndex + 1) % n; // 更新 currentIndex，确保其在 0 到 n-1 之间循环
}

VoltagePlot::~VoltagePlot()
{

}
