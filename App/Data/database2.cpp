#include "DataBase2.h"

DataBase2::DataBase2(QObject *parent) : QObject(parent)
{
    ModbusRTU &com = DevCenter::U().comRtu2;
    connect(&com,ModbusRTU::sigal_readData,this,slot_dataRTU);
}

uint16_t DataBase2::getAddress(QByteArray data)
{
    uint16_t d = data[2];
    d = d<<8 + data[3];
    return  d;
}

void DataBase2::ComRTU_readData(char devId, char addressH, char addressL, char registerCount)
{
    char d[] = {devId,0x03,addressH,addressL,0x00,registerCount};
    QByteArray data(d,6);
    data += ModbusRTU::CRCByTable(data);

    ModbusRTU &com = DevCenter::U().comRtu2;
    com.writeData(data);
}

void DataBase2::ComRTU_writeData(char devId, char addressH, char addressL, QVector<char> vdata)
{
    char d[] = {devId,0x06,addressH,addressL,0x00};
    QByteArray data(d,4);
    for(char &ud: vdata){
        data.append(ud);
    }
    data += ModbusRTU::CRCByTable(data);

    ModbusRTU &com = DevCenter::U().comRtu2;
    com.writeData(data);
}

void DataBase2::slot_dataRTU(QByteArray data)
{
    switch (data[1]) {
    case 0x03:
        emit(signal_reRead(data));
        break;
    case 0x06:
        emit(signal_reWrite(data));
        break;
    }
}

