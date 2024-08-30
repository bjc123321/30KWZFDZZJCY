#ifndef TIMERPOOL_H
#define TIMERPOOL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QQueue>
#include <functional>

#include "Base/BaseFun/base.h"

class TimerPool : public QObject
{
    Q_OBJECT
    B_INSTANCE(TimerPool)
private:
    explicit TimerPool(int size = 5,QObject *parent = 0);
    ~TimerPool();
    QVector<QTimer*> vtimer;
    QQueue<int> quei;
private:
    int getTimer();
    void reTimer(int i);
public:
    void start(int i,int ms = -1);
    int start(int ms,std::function<void(int)> fun,bool isOne = false,bool isRun = false);
    int startOne(int ms, std::function<void (int)> fun);
    void stop(int i);
    void close(int i);
    void closeAll();
public:
    static void Start(int i,int ms = -1);
    static int Start(int ms,std::function<void(int)> fun,bool isOne = false,bool isRun = false);
    static int StartOne(int ms, std::function<void (int)> fun);
    static void Stop(int i);
    static void Close(int i);
    static void CloseAll();
signals:
public slots:
};

#endif // TIMERPOOL_H
