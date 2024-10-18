#include "devcenter.h"

DevCenter::DevCenter(QObject *parent) : QObject(parent)
{
    comRtu1.init("COM1",
                 QSerialPort::Baud9600,
                 QSerialPort::Data8,
                 QSerialPort::NoParity,
                 QSerialPort::OneStop,
                 QSerialPort::NoFlowControl);

    comRtu1.port->open(QIODevice::ReadWrite);

    comRtu2.init("COM2",
                 QSerialPort::Baud9600,
                 QSerialPort::Data8,
                 QSerialPort::NoParity,
                 QSerialPort::OneStop,
                 QSerialPort::NoFlowControl);

    comRtu2.port->open(QIODevice::ReadWrite);


    listUsbSerialPorts();

}

void DevCenter::listUsbSerialPorts()
{

    // 配置和使用 SerialPortManager，设置发送端串口，打开串口等
    SerialPortManager &manager = SerialPortManager::getInstance();

    const QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

        if (serialPorts.isEmpty()) {
            qDebug() << "没有找到可用的串口设备。";
            return;
        }

        // 输出USB串口设备的信息
        for (const QSerialPortInfo &info : serialPorts) {

            if (isUsbSerialPort(info)) {
                qDebug() << "USB串口名称:" << info.portName();
                qDebug() << "描述:" << info.description();
                qDebug() << "制造商:" << info.manufacturer();
                qDebug() << "序列号:" << info.serialNumber();
                qDebug() << "系统路径:" << info.systemLocation();
                qDebug() << "是否可用:" << (info.isBusy() ? "是" : "否");
                qDebug() << "------------------------";

                manager.addSerialPort(info.portName());
                manager.configurePort(info.portName(), QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
                manager.openPort(info.portName(), QSerialPort::ReadWrite);

            }
        }

}


bool DevCenter::isUsbSerialPort(const QSerialPortInfo &portInfo)
{

    // 检查串口的名称或描述中是否包含 "USB"不区分大小写
        return portInfo.portName().contains("USB", Qt::CaseInsensitive) ||
               portInfo.description().contains("USB", Qt::CaseInsensitive);

}
