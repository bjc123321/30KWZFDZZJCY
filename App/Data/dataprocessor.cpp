#include "dataprocessor.h"
#include <QtMath>

DataProcessor::DataProcessor(QObject *parent)
    : QObject(parent)

{
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &DataProcessor::generateData);
}

void DataProcessor::startProcessing()
{
    dataTimer->start(200); // 每100ms生成一次数据
}

void DataProcessor::stopProcessing()
{
    dataTimer->stop();
}

void DataProcessor::generateData()
{

    currentTime += 0.1;
    double y1 = qSin(frequency * currentTime);
    double y2 = qSin(2 * frequency * currentTime);
    double y3 = qSin(3 * frequency * currentTime);
    qDebug()<<"frequency:"<<frequency<<"  currentTime:"<<currentTime
           <<"y1:"<<y1<<"y2:"<<y2<<"y3:"<<y3;

    emit newData(currentTime, y1, y2, y3);
}
