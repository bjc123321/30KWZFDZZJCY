#ifndef RECORDWAVEPLOT_H
#define RECORDWAVEPLOT_H

#include <QWidget>
#include <QSlider>
#include "qcustomplot/qcustomplot.h"

class RecordWavePlot : public QWidget
{
    Q_OBJECT

public:
    RecordWavePlot(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void togglePhaseA(int state);
    void togglePhaseB(int state);
    void togglePhaseC(int state);
    void handleSliderValueChanged(int value);  // 滑块值改变的槽函数

private:
    QCustomPlot *customPlot;
    QCPItemLine *verticalLine;
    QSlider *slider;
    QCPItemText *labelPhaseA, *labelPhaseB, *labelPhaseC;
    double getYValueAtX(QCPGraph *graph, double x);  //获取指定x处曲线的y值
    void updateIntersectionPoints(double x); // 更新交点

    void generateThreePhaseVoltage(); // 生成三相电压数据
    void beautifyPlot();              // 美化图表

};

#endif // RECORDWAVEPLOT_H
