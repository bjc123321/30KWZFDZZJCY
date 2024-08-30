#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QTimer>
#include "Base/BaseFun/base.h"


class DataProcessor : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataProcessor)
    public:
        explicit DataProcessor(QObject *parent = nullptr);

    signals:
        void newData(double time, double y1, double y2, double y3);

    public slots:
        void startProcessing();
        void stopProcessing();

    private slots:
        void generateData();

    private:
        QTimer *dataTimer;
        double currentTime = 0.0;
        double frequency = 1.0;
};

#endif // DATAPROCESSOR_H
