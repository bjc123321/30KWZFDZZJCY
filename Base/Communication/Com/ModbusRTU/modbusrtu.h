#ifndef MODBUSRTU_H
#define MODBUSRTU_H
#include "Base/Communication/Com/com.h"
#include <QObject>
#include <QDataStream>
#include <QDebug>

class ModbusRTU :public Com
{
public:
    explicit ModbusRTU(QObject *parent = 0);
public:
    qint64 sendData(QByteArray &data);
public:
    static QString Hex2Str(QByteArray data);
    static QByteArray CRCByCount(QByteArray data);//Modbus Rtu
    static QByteArray CRCByTable(QByteArray data);//Modbus Rtu
    static float floatData(QByteArray orgData);
    static int intData(QByteArray data);
signals:

public slots:

};

#endif // MODBUSRTU_H
