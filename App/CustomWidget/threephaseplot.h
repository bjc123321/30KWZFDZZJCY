#ifndef THREEPHASEPLOT_H
#define THREEPHASEPLOT_H

#include <QWidget>
#include <QTimer>
#include <QCheckBox>
#include "qcustomplot/qcustomplot.h"
#include "App/Data/dataprocessor.h"

class ThreePhasePlot : public QWidget
{
    Q_OBJECT
public:
    explicit ThreePhasePlot(QWidget *parent = nullptr);

public:

    void startTuningFreqTest();
    void stopTuningFreqTest();

signals:

private slots:
    void updatePlot(double time, double y1, double y2, double y3);

    void showCurve(int index, bool show);

private:
    QCustomPlot *customPlot;
    double frequency;
    QVector<double> timeData, y1Data, y2Data, y3Data;
    QCheckBox *checkBox1, *checkBox2, *checkBox3;



};

#endif // THREEPHASEPLOT_H
