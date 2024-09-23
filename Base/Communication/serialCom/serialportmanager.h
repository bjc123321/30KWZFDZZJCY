#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMap>
#include <QDebug>

#include "modbusprotocolparser.h"

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例的静态方法
    static SerialPortManager& getInstance() {
        static SerialPortManager instance; // 静态实例对象
        return instance;
    }

    bool addSerialPort(const QString &portName);
    bool removeSerialPort(const QString &portName);
    QSerialPort* getSerialPort(const QString &portName);

    bool openPort(const QString &portName, QSerialPort::OpenMode mode);
    void closePort(const QString &portName);
    bool writeData(const QString &portName, const QByteArray &data);

    // 设置串口属性
    bool configurePort(const QString &portName,
                       QSerialPort::BaudRate baudRate,
                       QSerialPort::DataBits dataBits,
                       QSerialPort::Parity parity,
                       QSerialPort::StopBits stopBits,
                       QSerialPort::FlowControl flowControl);
public slots:


signals:
    void dataReceived(const QString &portName, const QByteArray &data);


private slots:
    void handleReadyRead();




private:
    SerialPortManager(QObject *parent = nullptr);  // 私有化构造函数
    ~SerialPortManager();

    SerialPortManager(const SerialPortManager&) = delete;  // 禁止拷贝构造
    SerialPortManager& operator=(const SerialPortManager&) = delete;  // 禁止赋值操作

    QMap<QString, QSerialPort*> serialPorts;

};

#endif // SERIALPORTMANAGER_H
