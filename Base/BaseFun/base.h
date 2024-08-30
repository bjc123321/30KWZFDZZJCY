#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QLoggingCategory>
#include <QDebug>
#include <QCheckBox>
#include "qcustomplot/qcustomplot.h"
//class 插件------------
//单例模式
#define B_INSTANCE(x)\
    public:\
    static x &  U(){ \
    static x t;\
    return t;\
    }

//权限-------------------
class Base : public QObject
{
    Q_OBJECT
    B_INSTANCE(Base)

    enum WORK_MODE{
        PRODUCE_MODE,
                TEXT_MODE
    };

    enum POWER_MODE{
        ADMIN_MODE,
        USER_MODE,
        VISITOR_MODE
    };
public:
    explicit Base(QObject *parent = 0);
public:
    WORK_MODE curMode = TEXT_MODE;
    void setCurMode(WORK_MODE mode);
    void setCurMode(POWER_MODE mode);
    WORK_MODE getCurMode();
public:
    static void SetCurMode(WORK_MODE mode);
    static void SetCurMode(POWER_MODE mode);
    static WORK_MODE GetCurMode();
signals:

public slots:
};

class Tool{
public:
    template<typename T>
    static T inRange(T min,T max,T value){
        if (value < min){
            return min;
        }else if (value > max){
            return max;
        }else{
            return value;
        }
    }
    static SetCustomPlot(QCustomPlot * customplot,QCheckBox*box0,QCheckBox*box1,QCheckBox*box2);
};

#endif // BASE_H
