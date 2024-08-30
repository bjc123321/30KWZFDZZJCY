 #include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    ModbusRTU &com = DevCenter::U().comRtu1;
    connect(&com,ModbusRTU::sigal_readData,this,slot_dataRTU);
}

uint16_t DataBase::getAddress(QByteArray data)
{
    uint16_t d = data[2];
    d = d<<8 + data[3];
    return  d;
}

void DataBase::ComRTU_readData(char devId, char addressH, char addressL, char registerCount)
{
    char d[] = {devId,0x03,addressH,addressL,0x00,registerCount};
    QByteArray data(d,6);
    data += ModbusRTU::CRCByTable(data);

    ModbusRTU &com = DevCenter::U().comRtu1;

    sendReadData = data;
    com.writeData(data);
}

void DataBase::ComRTU_writeData(char devId, char addressH, char addressL, char registerCount, QVector<char> vdata)
{
    char d[] = {devId,0x10,addressH,addressL,0x00,registerCount};
    QByteArray data(d,6);
    data.append(vdata.size());
    for(char &ud: vdata){
        data.append(ud);
    }
    data += ModbusRTU::CRCByTable(data);

    ModbusRTU &com = DevCenter::U().comRtu1;

    sendWriteData = data;
    com.writeData(data);
}

void DataBase::ComRTU_wlockPage(char devId, char index)
{
    char d = index;
    ComRTU_writeData(devId,0x00,0x00,0x01,QVector<char>{0x00,d});
}

void DataBase::slot_dataRTU(QByteArray data)
{

    switch (data[1]) {
    case 0x10:
        emit(signal_reWrite(data));
        break;
     default:
        emit(signal_reRead(data));
        break;
    }
}
