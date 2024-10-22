#ifndef DEVCENTER_H
#define DEVCENTER_H

#include <QObject>
#include <functional>
#include <tuple>
#include "Base/BaseFun/base.h"
#include "Base/Communication/Com/ModbusRTU/modbusrtu.h"
#include "Base/BaseFun/TimerPool/timerpool.h"

#include "Base/Communication/serialCom/serialportmanager.h"
class DevCenter : public QObject
{
    Q_OBJECT
    B_INSTANCE(DevCenter)
    public:
        explicit DevCenter(QObject *parent = 0);



public:
    ModbusRTU comRtu1;
    ModbusRTU comRtu2;


    // 列出指定类型如:USB串口设备
    void listUsbSerialPorts();
    //判断是不是usb的串口com
    bool isUsbSerialPort(const QSerialPortInfo &portInfo);

    QString loadCom = "";
    QString panelCom = "";


signals:

public slots:
};

#endif // DEVCENTER_H
