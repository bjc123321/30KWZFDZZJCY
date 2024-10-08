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




    SerialPortManager &manager = SerialPortManager::getInstance();
    manager.addSerialPort("COM4");
    manager.configurePort("COM4", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);
    manager.openPort("COM4", QSerialPort::ReadWrite);

}


