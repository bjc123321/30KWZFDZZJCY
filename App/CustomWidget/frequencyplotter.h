#ifndef FREQUENCYPLOTTER_H
#define FREQUENCYPLOTTER_H

#include <QMainWindow>
#include <QTimer>
#include <QCheckBox>
#include "qcustomplot/qcustomplot.h"
#include "App/Data/dataprocessor.h"

class FrequencyPlotter : public QMainWindow
{
    Q_OBJECT
public:
    FrequencyPlotter(QWidget *parent = nullptr);
    ~FrequencyPlotter();

public:

    void startTuningFreqTest();
    void stopTuningFreqTest();

private slots:
    void updatePlot(double time, double y1, double y2, double y3);
    void on_checkBox_stateChanged(int state);

private:
    QCustomPlot *customPlot;
    double frequency;
    QVector<double> timeData, y1Data, y2Data, y3Data;
    QCheckBox *checkBox1, *checkBox2, *checkBox3;
    DataProcessor *dataProcessor;
};

#endif // FREQUENCYPLOTTER_H
