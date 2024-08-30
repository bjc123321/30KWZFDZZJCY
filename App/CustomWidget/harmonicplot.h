#ifndef HARMONICPLOT_H
#define HARMONICPLOT_H

#include <QWidget>
#include <QSlider>
#include <QList>
#include "qcustomplot/qcustomplot.h"
#include "triangleslider.h"
class HarmonicPlot : public QWidget
{
    Q_OBJECT
public:
    explicit HarmonicPlot(QWidget *parent = 0);
private:
    QCustomPlot *voltagePlot;
    QCustomPlot *currentPlot;
    QCPBars *voltageBars;
    QCPBars *currentBars;
    TriangleSlider *slider;

    QPen defaultVoltagePen;    // 保存默认的电压柱状图颜色
    QBrush defaultVoltageBrush;
    QPen defaultCurrentPen;    // 保存默认的电流柱状图颜色
    QBrush defaultCurrentBrush;

    void setupUI();
    void setupPlots();
    void updateBarsDataAndColor(int phaseIndex);
    void updateTracers(int value);

    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void updateSliderWidth(const QCPRange &range);

signals:
    void updateVolAndCurValue(double voltageValue,double currentValue);

public slots:
    void onPhaseChanged(int index);
    void onSliderValueChanged(int value);
};

#endif // HARMONICPLOT_H
