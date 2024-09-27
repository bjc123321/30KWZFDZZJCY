#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{

}

SerialPortManager::~SerialPortManager()
{
    // 清理所有串口
    foreach (QSerialPort *port, serialPorts.values()) {
        if (port->isOpen()) {
            port->close();
        }
        delete port;
    }
}

bool SerialPortManager::addSerialPort(const QString &portName)
{
    // 检查串口是否存在
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        bool portExists = false;

        for (const QSerialPortInfo &port : ports) {
            if (port.portName() == portName) {
                portExists = true;
                break;
            }
        }

        if (!portExists) {
            qDebug() << "串口不存在：" << portName;
            return false;  // 端口不存在
        }

        if (serialPorts.contains(portName)) {
            qDebug() << "串口已经存在：" << portName;
            return false;  // 端口已经存在
        }

        QSerialPort *serialPort = new QSerialPort(this);
        serialPort->setPortName(portName);

        // 连接信号
        connect(serialPort, &QSerialPort::readyRead, this, &SerialPortManager::handleReadyRead);
        connect(serialPort, &QSerialPort::errorOccurred, this, &SerialPortManager::handleError);

        serialPorts.insert(portName, serialPort);
        qDebug() << "串口已成功添加：" << portName;

        return true;
}

bool SerialPortManager::removeSerialPort(const QString &portName)
{
    if (!serialPorts.contains(portName))
        return false;

    QSerialPort *serialPort = serialPorts.take(portName);
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;

    return true;
}

QSerialPort* SerialPortManager::getSerialPort(const QString &portName)
{
    //找到就返回 QSerialPort*,没找到就返回nullptr;
    //value参数中的nullpte是默认值
    return serialPorts.value(portName, nullptr);
}

bool SerialPortManager::openPort(const QString &portName, QSerialPort::OpenMode mode)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort){
        qDebug()<<"没找到该串口";
        return false;
    }

    if (!serialPort->isOpen()) {
        qDebug()<<serialPort->portName()<<"打开";
        return serialPort->open(mode);
    }

    return true;  // 已经打开
}

void SerialPortManager::closePort(const QString &portName)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
}

bool SerialPortManager::writeData(const QString &portName, const QByteArray &data)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort || !serialPort->isOpen()){
        qDebug()<<"串口没打开!";
        return false;

    }

    QByteArray packet = data;
    //逐位计算法,效率较低，不适用于高性能场景
    //uint16_t crc = calculateCRC16(data);
    //查表法:高性能的场景（如网络协议实现、文件校验等）通常使用查表法.查表法的速度明显快于逐位计算法
    uint16_t crc = ModbusProtocolParser::crc16UsingTable(data);
    qDebug()<<"1.查表的CRC:"<<QString("%1").arg(crc,4,16,QLatin1Char('0')); //不足8位补0

    //假如：查表法查出来的CRC = 0xABCD，则其中高位字节是0xAB,低位字节是:0xCD
    //注意：Modbus 协议通常使用小端序(小地址存低字节,高地址存高字节),且发送的字节序要跟接收端字节序保持一致
    packet.append(crc & 0xFF);        // Append low byte of CRC,先低字节(0xCD)
    packet.append((crc >> 8) & 0xFF); // Append high byte of CRC,再高字节(0xAB)
    if (!serialPort->isOpen()) {
        qDebug() << "Serial port is not open!";
        return false;
    }

    //执行下面这条语句后,QSerialPort的缓冲区里就会有数据,有数据就会触发QSerialPort::readyRead()
    qint64 bytesWritten = serialPort->write(packet);
    //flush() 强制将缓冲区的内容立即写入到底层设备，这就保证了数据不会滞留在发送缓冲区中。
    serialPort->flush();
    if (bytesWritten == -1) {
        qDebug() << "2.Failed to write to the serial port!";
    } else if (bytesWritten < packet.size()) {
        qDebug() << "2.Only partially wrote" << bytesWritten << "bytes to the serial port.";
    } else {
        qDebug() << "2.Successfully wrote" << bytesWritten << "bytes to the serial port.";
    }
    qDebug() << "3.Sending data:" << packet.toHex()<<"Sending data大小:"<<packet.size();
    return serialPort->waitForBytesWritten(500);//等待最多500毫秒直到所有数据都写入成功。此时间内没有完成写入,返回 false。
}



void SerialPortManager::handleReadyRead()
{
    /*
     * 局部变量 serialPort: 在槽函数内部使用 serialPort 是为了访问信号的发送者（也就是具体的 QSerialPort 对象）
       通过这种方式,在处理多个串口对象时，区分哪个串口对象发出了信号，并对该对象进行相应的操作。
    */
    QSerialPort *serialPort = qobject_cast<QSerialPort *>(sender());
    if (serialPort) {

        //读取串口缓存区中数据
        QByteArray data = serialPort->readAll();
        qDebug()<<"4.ReadyRead缓存区待读数据:"<<data.toHex()<<"ReadyRead缓存区数据大小:"<<data.size()<<"串口名:"<<serialPort->portName();
        ModbusProtocolParser parser;
        if(parser.parseReponse(data)){

            QByteArray dataField = parser.getDataField();
            qDebug()<<"6.成功解析仪表返回响应帧的数据域:"<<dataField.toHex();
//            parser.floatData(dataField); // 如果 floatData 是静态函数，则可以用类名直接调用
            emit dataReceived(serialPort->portName(), data);
        }

    }
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{

    QSerialPort *serialPort = qobject_cast<QSerialPort *>(sender());
    if (error == QSerialPort::NoError) {
        // 设备意外拔出，触发了资源错误
        qDebug()<<"No problem √";

    }else{
        //处理其他错误
        if (serialPort->isOpen()) {
            serialPort->close();
            qDebug() << "Serial port closed due to disconnection.";
        }
        emit errorOccurred("Permission error while accessing the serial port.");
    }
}

bool SerialPortManager::configurePort(const QString &portName,
                                      QSerialPort::BaudRate baudRate,
                                      QSerialPort::DataBits dataBits,
                                      QSerialPort::Parity parity,
                                      QSerialPort::StopBits stopBits,
                                      QSerialPort::FlowControl flowControl)
{
    QSerialPort *serialPort = getSerialPort(portName);
    if (!serialPort)
        return false;

    bool success = true;

    success &= serialPort->setBaudRate(baudRate);
    success &= serialPort->setDataBits(dataBits);
    success &= serialPort->setParity(parity);
    success &= serialPort->setStopBits(stopBits);
    success &= serialPort->setFlowControl(flowControl);

    return success;
}

