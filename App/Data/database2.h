#ifndef DataBase2_H
#define DataBase2_H

#include <QObject>
#include "Base/BaseFun/base.h"
#include "Base/Communication/Com/ModbusRTU/modbusrtu.h"
#include "Base/Communication/devcenter.h"

class DataBase2 : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataBase2)
public:
    explicit DataBase2(QObject *parent = 0);
public:
    uint16_t getAddress(QByteArray data);
//发送数据
public:
    void ComRTU_readData(char devId,char addressH,char addressL,char registerCount);

    void ComRTU_writeData(char devId,char addressH,char addressL,QVector<char> vdata);
//接收数据
signals:
    void signal_reRead(QByteArray data);
    void signal_reWrite(QByteArray data);
public slots:
    void slot_dataRTU(QByteArray data);
};

#endif // DataBase2_H
