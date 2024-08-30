#ifndef COM_H
#define COM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QByteArray>
#include <QMetaEnum>
class Com : public QObject
{
    Q_OBJECT
public:
    explicit Com(QObject *parent = 0);
public:
    void init(QString portName,
              QSerialPort::BaudRate baudRate,
              QSerialPort::DataBits dataBit,
              QSerialPort::Parity parity,
              QSerialPort::StopBits stopBit,
              QSerialPort::FlowControl flowControl);
    qint64 writeData(QByteArray data);

    bool isOpen();
public:
    QSerialPort* port;
    QPushButton* ui_open = NULL;
public:
    static InitUiPort(Com*com,
                      QLineEdit*ui_port,
                      QComboBox*ui_baudRate,
                      QComboBox*ui_dataBit,
                      QComboBox*ui_parity,
                      QComboBox*ui_stopBit,
                      QComboBox*ui_flowControl,
                      QPushButton *ui_open);
signals:
    void sigal_readData(QByteArray data);
public slots:
};

#endif // COM_H
