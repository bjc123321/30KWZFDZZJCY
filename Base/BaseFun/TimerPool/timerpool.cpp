#include "timerpool.h"

TimerPool::TimerPool(int size, QObject *parent) : QObject(parent)
{
    for(int i=0;i<size;i++){
        vtimer << new QTimer(this);
        quei << i;
    }
}

TimerPool::~TimerPool()
{
    for(int i=0;i<vtimer.size();i++){
        close(i);
        delete vtimer[i];
    }
}

int TimerPool::getTimer()
{
    if(quei.isEmpty()){
        vtimer << new QTimer(this);
        return vtimer.size() - 1;
    }else{
        return quei.dequeue();
    }
}

void TimerPool::reTimer(int i)
{
    quei << i;
}

void TimerPool::start(int i, int ms)
{
    QTimer * tptr = vtimer[i];
    if(ms>=0){
        tptr->start(ms);
    }else{
        tptr->start();
    }
}

int TimerPool::start(int ms, std::function<void (int)> fun, bool isOne, bool isInstanlyRun)
{
    if(isOne && isInstanlyRun){
        fun(-1);
        return -1;
    }

    int id = getTimer();
    QTimer * tptr = vtimer[id];

    if(isInstanlyRun){
        fun(id);
    }

    if(!tptr->isActive()){
        tptr->disconnect();
        connect(tptr,QTimer::timeout,this,[=](){
            fun(id);
            if(isOne){
                tptr->disconnect();
                tptr->stop();
            }
        });
    }

    tptr->start(ms);

    return id;
}

int TimerPool::startOne(int ms, std::function<void (int)> fun)
{
    return TimerPool::start(ms,fun,true);
}

void TimerPool::stop(int i)
{
    if(i >= 0 && i <vtimer.size()){
        QTimer * tptr = vtimer[i];
        tptr->stop();
    }
}

void TimerPool::close(int i)
{
    QTimer * tptr = vtimer[i];
    tptr->disconnect();
    tptr->stop();
    reTimer(i);
}

void TimerPool::closeAll()
{
    for(int i=0;i<vtimer.size();i++){
        close(i);
    }
}

void TimerPool::Start(int i, int ms)
{
    return TimerPool::U().start(i,ms);
}

int TimerPool::Start(int ms, std::function<void (int)> fun, bool isOne,bool isRun)
{
    return TimerPool::U().start(ms,fun,isOne,isRun);
}

int TimerPool::StartOne(int ms, std::function<void (int)> fun)
{
    return TimerPool::U().startOne(ms,fun);
}

void TimerPool::Stop(int i)
{
    TimerPool::U().stop(i);
}

void TimerPool::Close(int i)
{
    TimerPool::U().close(i);
}

void TimerPool::CloseAll()
{
    TimerPool::U().closeAll();
}

