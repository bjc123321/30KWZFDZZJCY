#include "modbusprotocolparser.h"

ModbusProtocolParser::ModbusProtocolParser(QObject *parent) : QObject(parent)
{

}

//初始化表256个字节，注意不可更改
static const quint16 crc16Table[] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

bool ModbusProtocolParser::parseReponse(const QByteArray &reponse)
{
    if (reponse.size() < 4) {  // Modbus 最小响应帧长度
            qDebug() << "响应帧长度不足，无法解析";
            return false;
    }



    // 提取地址域（从机地址）
    slaveAddress = static_cast<uint8_t>(reponse.at(0));
    qDebug()<<"从机地址(10十进制)"<<slaveAddress;

    // 提取功能码(值得注意的是：异常响应中"功能码"的最高位会设置为 1，表示错误响应,)
    functionCode = static_cast<uint8_t>(reponse.at(1));
    qDebug()<<"功能码(10十进制)"<<functionCode;

    // 获取字节数（3字节之后的位置）
    byteCount = static_cast<uint8_t>(reponse.at(2));
    uint8_t expectedFrameLength = 5 + byteCount; // 地址(1字节) + 功能码(1字节) + + 字节数(1字节) + 数据(n字节) + CRC（2字节）

    // 检查数据长度是否足够
    if (reponse.size() < expectedFrameLength) {
        qDebug()<<"reponse.size()"<<reponse.size()<<"期望长度:"<<expectedFrameLength;
        return false; // 数据不完整
    }

    /*
     * 提取数据部分(此方法普遍适用与Modbus Rtu协议的"变长数据域"的提取,因为不同功能码的消息，数据域的长度可能不同)
     * 注：有些数据帧可能包含字节计数，也要把字节计数部分去除
     * (具体协议具体分析,某些协议可能数据域格式有变化,但此顺序是保持不变的：[地址] [功能码] [字节计数] [数据域] [CRC])
     * "功能码"是判断是否包含字节计数的,如:0x01,0x02,0x03,0x04都是包含字节计数的功能码;
     * data = request.mid(3, request.size() - 5);// 数据域大小 = 总大小 - 1字节地址 - 1字节功能码 -1字节计数 - 2字节CRC
     */

    if (functionCode >= 0x01 && functionCode <= 0x04) {
        // 功能码 0x01 到 0x04 的响应包含字节计数
        dataField = reponse.mid(3, reponse.size() - 5);// 数据域大小 = 总大小 - 1字节地址 - 1字节功能码 -1字节计数 - 2字节CRC
    }else{
        // 其他功能码的响应通常不包含字节计数
        dataField = reponse.mid(2, reponse.size() - 4);  // 数据域大小 = 总大小 - 1字节地址 - 1字节功能码 - 2字节CRC
    }

    qDebug()<<"数据域"<<dataField.toHex();

    // 提取接收到的 CRC16 校验
    /*
     * 例：这时串口缓存读到的CRC为0xCDAB。
     * 则crc的倒数第一个元素0xAB左移 8 位后得到 0xAB00;
     * crc的倒数第二个元素0xCD，保持不变为 0x00CD
     * 按位或操作后，结果为 0xAB00 | 0x00CD = 0xABCD，即receivedCRC = ABCD
     * 通过 static_cast<uint8_t> 明确指定类型为"无符号" 8 位整数，这可以避免符号扩展问题。
    */
    uint16_t receivedCRC = (static_cast<uint8_t>(reponse.at(reponse.size() - 1)) << 8) |
                           static_cast<uint8_t>(reponse.at(reponse.size() - 2));

    // 校验 CRC
    return verifyCRC(reponse.left(reponse.size() - 2), receivedCRC);
}

QByteArray ModbusProtocolParser::generateResponse(uint8_t slaveAddress, uint8_t functionCode, const QByteArray &data)
{
    QByteArray response;
    response.append(static_cast<char>(slaveAddress));  // 从机地址
    response.append(static_cast<char>(functionCode));  // 功能码
    response.append(data);  // 数据内容

    uint16_t crc = crc16UsingTable(response);
    response.append(static_cast<char>(crc & 0xFF));        // CRC 低字节
    response.append(static_cast<char>((crc >> 8) & 0xFF)); // CRC 高字节

    qDebug() << "生成的response：" << response.toHex();
    return response;
}

QByteArray ModbusProtocolParser::extractDataField(const QByteArray& frame, bool isRequest)
{
    if (frame.size() < 4) {
        // 帧长度过短，无法解析
        qDebug() << "Frame too short to parse.";
        return QByteArray();
    }

    uint8_t address = static_cast<uint8_t>(frame.at(0));       // 提取地址
    uint8_t functionCode = static_cast<uint8_t>(frame.at(1));  // 提取功能码
    bool isError = functionCode & 0x80;                        // 检查是否是异常响应

    if (!isRequest && isError) {
        // 异常响应处理
        // 异常响应结构：[地址] [异常功能码] [异常码] [CRC]
        return frame.mid(2, 1);  // 数据域为异常码，通常只有 1 字节
    }

    if (isRequest) {
        // 处理请求帧
        switch (functionCode) {
            case 0x01: // 读线圈状态
            case 0x02: // 读离散输入状态
            case 0x03: // 读保持寄存器
            case 0x04: // 读输入寄存器
                // 请求帧包含起始地址和寄存器数量
                return frame.mid(2, 4);  // 起始地址 + 寄存器数量，共 4 字节

            case 0x05: // 写单个线圈
            case 0x06: // 写单个寄存器
                // 请求帧包含地址和写入值
                return frame.mid(2, 4);  // 地址 + 写入值，共 4 字节

            case 0x0F: // 写多个线圈
            case 0x10: // 写多个寄存器
                // 请求帧包含起始地址、寄存器数量、字节计数和数据
                return frame.mid(7, static_cast<uint8_t>(frame.at(6)));  // 数据域从字节计数之后开始

            default:
                qDebug() << "Unknown function code in request:" << functionCode;
                return QByteArray();
        }
    } else {
        // 处理响应帧
        switch (functionCode) {
            case 0x01: // 读线圈状态
            case 0x02: // 读离散输入状态
            case 0x03: // 读保持寄存器
            case 0x04: // 读输入寄存器
                // 响应帧包含字节计数和数据域
                return frame.mid(3, static_cast<uint8_t>(frame.at(2)));  // 从字节计数之后开始，长度为字节计数

            case 0x05: // 写单个线圈
            case 0x06: // 写单个寄存器
                // 响应帧通常是回显请求内容
                return frame.mid(2, 4);  // 地址 + 写入值，共 4 字节

            case 0x0F: // 写多个线圈
            case 0x10: // 写多个寄存器
                // 响应帧回显起始地址和寄存器数量
                return frame.mid(2, 4);  // 起始地址 + 寄存器数量，共 4 字节

            default:
                qDebug() << "Unknown function code in response:" << functionCode;
                return QByteArray();
        }
    }
}


//CRC-16-IBM 逐位计算法
uint16_t ModbusProtocolParser::calculateCRC16(const QByteArray &data)
{
    int len=data.size();
    uint16_t wcrc = 0XFFFF;//预置16位crc寄存器，初值全部为1
    uint8_t temp;//定义中间变量
    int i=0,j=0;//定义计数
    for(i=0;i<len;i++)//循环计算每个数据
    {
       temp=data.at(i);
       wcrc^=temp;
       for(j=0;j<8;j++){
          //判断右移出的是不是1，如果是1则与多项式进行异或。
          if(wcrc&0X0001){
              wcrc>>=1;//先将数据右移一位
              wcrc^=0XA001;//与上面的多项式进行异或
          }
          else//如果不是1，则直接移出
              wcrc>>=1;//直接移出
       }
    }
    temp = wcrc;//crc的值
    return wcrc;
}

//CRC16计算函数 查表法
quint16 ModbusProtocolParser::crc16UsingTable(const QByteArray &hexText)
{
    quint8 buf;
    quint16 crc16 = 0xFFFF;
    for ( auto i = 0; i < hexText.size(); ++i )
    {
        buf = hexText.at( i ) ^ crc16;
        crc16 >>= 8;
        crc16 ^= crc16Table[ buf ];
    }
   return  crc16;

}

bool ModbusProtocolParser::verifyCRC(const QByteArray &data, uint16_t receivedCRC)
{
    uint16_t calculatedCRC = crc16UsingTable(data);
    qDebug()<<"calculatedCRC:"<<calculatedCRC<<"receivedCRC:"<<receivedCRC;
    if (receivedCRC == calculatedCRC) {
        qDebug() << "5.CRC 校验通过(￣︶￣).";
        return true;
    } else {
        qDebug() << "5.*******CRC 检查失败(Fail),发送的和接收数据不一致!!!*******" << receivedCRC << "Calculated:" << calculatedCRC
                 <<"错误的返回数据为:"<<data.toHex()<<"后续存放到异常日志中如从机地址为[0x81]的情况";
        return true;
    }
}

float ModbusProtocolParser::toFloatData(QByteArray orgArrData)
{
    qDebug() << "接收到的数据域:" << orgArrData.toHex() << "长度：" << orgArrData.size();

    // 修改数据长度检查条件
    if(orgArrData.size() != 4){
        return -1; // 返回错误值或处理异常情况
    }

    float floatValue = 0.0;
    QDataStream stream(orgArrData);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setByteOrder(QDataStream::LittleEndian); // 协议为小端存储
    stream >> floatValue;
    qDebug() << "解析后浮点值:"  <<  floatValue << "数据:" << orgArrData.toHex();
    return floatValue;

}

int ModbusProtocolParser::toIntData(QByteArray orgData)
{
    qDebug() << "接收到的数据域:" << orgData.toHex() << "长度：" << orgData.size();

    // 修改数据长度检查条件
    if(orgData.size() != 2){
        return -1; // 返回错误值或处理异常情况
    }

    int intValue = 0; // 初始化整型值
    QDataStream stream(orgData);
    stream.setByteOrder(QDataStream::LittleEndian); // 协议为小端存储
    stream >> intValue; // 从数据流中读取整型值

    qDebug() << "解析后整型值:" << intValue << "数据:" << orgData.toHex();
    return intValue; // 返回解析后的整型值
}


