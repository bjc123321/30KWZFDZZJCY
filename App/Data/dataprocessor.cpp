#include "dataprocessor.h"
#include <QtMath>

DataProcessor::DataProcessor(QObject *parent)
    : QObject(parent)

{
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &DataProcessor::generateData2);
}

void DataProcessor::startProcessing()
{
    dataTimer->start(100); // 每100ms生成一次数据
}

void DataProcessor::stopProcessing()
{
    dataTimer->stop();
}

void DataProcessor::generateData(double U,double I,double F)
{

    currentTime += 0.1;
    double y1 = qSin(U * currentTime);
    double y2 = qSin(2 * I * currentTime);
    double y3 = qSin(3 * F * currentTime);
    qDebug()<<"currentTime:"<<currentTime
           <<"y1:"<<y1<<"y2:"<<y2<<"y3:"<<y3;

    emit drawSignal(currentTime, y1, y2, y3);
}

void DataProcessor::generateData2()
{

    currentTime += 0.1;
    double y1 = qSin(0.2 * currentTime);
    double y2 = qSin(2 * 0.5 * currentTime);
    double y3 = qSin(3 * 0.8 * currentTime);
    qDebug()<<"currentTime2:"<<currentTime
           <<"y1:"<<y1<<"y2:"<<y2<<"y3:"<<y3;

    emit newData(currentTime, y1, y2, y3);
}
