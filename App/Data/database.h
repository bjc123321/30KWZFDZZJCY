#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "Base/BaseFun/base.h"
#include "Base/Communication/Com/ModbusRTU/modbusrtu.h"
#include "Base/Communication/devcenter.h"
class DataBase : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataBase)
public:
    explicit DataBase(QObject *parent = 0);
public:
    uint16_t getAddress(QByteArray data);
//发送数据
public:
    void ComRTU_readData(char devId,char addressH,char addressL,char registerCount);

    void ComRTU_writeData(char devId,char addressH,char addressL,char registerCount,QVector<char> vdata);

    QByteArray sendReadData;
    QByteArray sendWriteData;
//接收数据
public:
    void ComRTU_wlockPage(char devId,char index);
signals:
    void signal_reRead(QByteArray data);
    void signal_reWrite(QByteArray data);
public slots:
    void slot_dataRTU(QByteArray data);
};

#endif // DATABASE_H
