#ifndef MODBUSPROTOCOLPARSER_H
#define MODBUSPROTOCOLPARSER_H

#include <QObject>
#include <QByteArray>
#include <cstdint>
#include <QDebug>
#include <QDataStream>

class ModbusProtocolParser : public QObject
{
    Q_OBJECT
public:
    explicit ModbusProtocolParser(QObject *parent = 0);

public:
    // 解析 Modbus 响应帧
    bool parseReponse(const QByteArray &reponse);

    // 生成 Modbus 响应帧
    QByteArray generateResponse(uint8_t slaveAddress, uint8_t functionCode, const QByteArray &data);

    // 通用数据域提取函数,可处理"请求帧"和"响应帧"
    QByteArray extractDataField(const QByteArray& frame, bool isRequest);


    //方法1: 逐位计算法, CRC16 校验效率较低，不适用于高性能场景
    static uint16_t calculateCRC16(const QByteArray &data);
    //方法2：查表法，高性能的场景（如网络协议实现、文件校验等）通常使用查表法.查表法的速度明显快于逐位计算法
    static quint16 crc16UsingTable(const QByteArray &hexText);

    // 验证 CRC16 校验
    bool verifyCRC(const QByteArray &data, uint16_t receivedCRC);

    uint8_t getSlaveAddress() const { return slaveAddress; }

    // 获取解析后的功能码
    uint8_t getFunctionCode() const { return functionCode; }

    // 获取解析后的数据内容
    QByteArray getDataField() const { return dataField; }

    //解析16进制为浮点数
    float floatData(QByteArray orgData);

private:

    uint8_t slaveAddress = 0;   // 从机地址
    uint8_t functionCode = 0;   // 功能码
    uint8_t byteCount = 0;      // 字节计数
    QByteArray dataField;       // 数据域



signals:

public slots:
};

#endif // MODBUSPROTOCOLPARSER_H
