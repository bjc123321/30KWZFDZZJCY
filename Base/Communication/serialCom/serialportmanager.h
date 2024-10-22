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

    // 防止有些串口数据返回响应帧时分段发送。则用此缓冲区为把不完整的数据帧进行拼接
    QByteArray buffer;

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


    QMap<QString, QSerialPort*> getPorts(){
        return serialPorts;
    }

public slots:


signals:
    void dataReceived(const QSerialPort &port, const QByteArray &data);
    //自定义的错误处理信号用于发送给应用层
    void errorOccurred(const QString &errorMsg);

private slots:
    void handleReadyRead();

    void handleError(QSerialPort::SerialPortError error);




private:
    SerialPortManager(QObject *parent = nullptr);  // 私有化构造函数
    ~SerialPortManager();

    SerialPortManager(const SerialPortManager&) = delete;  // 禁止拷贝构造
    SerialPortManager& operator=(const SerialPortManager&) = delete;  // 禁止赋值操作

    QMap<QString, QSerialPort*> serialPorts;


};

#endif // SERIALPORTMANAGER_H
