#ifndef DEVCENTER_H
#define DEVCENTER_H

#include <QObject>
#include <functional>
#include <tuple>
#include "Base/BaseFun/base.h"
#include "Base/Communication/Com/ModbusRTU/modbusrtu.h"
#include "Base/BaseFun/TimerPool/timerpool.h"
class DevCenter : public QObject
{
    Q_OBJECT
    B_INSTANCE(DevCenter)
    public:
        explicit DevCenter(QObject *parent = 0);
public:
    ModbusRTU comRtu1;
    ModbusRTU comRtu2;
signals:

public slots:
};

#endif // DEVCENTER_H
