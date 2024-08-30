#ifndef VOLTAGEPLOT_H
#define VOLTAGEPLOT_H

#include <QWidget>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"

class VoltagePlot : public QWidget
{
    Q_OBJECT
public:
    explicit VoltagePlot(QWidget *parent = 0);
    ~VoltagePlot();

    void startFlucPlot();//波形开始
    void stopFlucPlot();//波形停止

private slots:
    void showCurve(int index, bool show);
    void updateData();



private:
    QCustomPlot *customPlot;
    QCheckBox *checkBoxA;
    QCheckBox *checkBoxB;
    QCheckBox *checkBoxC;
    QTimer *dataTimer;
    QVector<double> x;
    QVector<double> u_a;
    QVector<double> u_b;
    QVector<double> u_c;
    int currentIndex;

    void setupPlot();
    void setupUI();

signals:

public slots:
};

#endif // VOLTAGEPLOT_H
