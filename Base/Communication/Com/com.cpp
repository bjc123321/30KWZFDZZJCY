#include "com.h"

Com::Com(QObject *parent) : QObject(parent)
{
    port = new QSerialPort(this);

    connect(port,QSerialPort::readyRead,this,[&](){
        emit ( sigal_readData( port->readAll() ) );
    });
}

void Com::init(QString portName, QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBit, QSerialPort::Parity parity, QSerialPort::StopBits stopBit, QSerialPort::FlowControl flowControl)
{
    port->setPortName(portName);
    port->setBaudRate(baudRate);
    port->setDataBits(dataBit);
    port->setParity(parity);
    port->setStopBits(stopBit);
    port->setFlowControl(flowControl);
}

qint64 Com::writeData(QByteArray data)
{
    return port->write(data);
}

bool Com::isOpen()
{
    return ui_open != NULL && ui_open->isChecked();
}

Com::InitUiPort(Com *com, QLineEdit *ui_port, QComboBox *ui_baudRate, QComboBox *ui_dataBit, QComboBox *ui_parity, QComboBox *ui_stopBit, QComboBox *ui_flowControl, QPushButton *ui_open)
{
    QMetaEnum metaEnum1 = QMetaEnum::fromType<QSerialPort::BaudRate>();
    for(int i=0; i < metaEnum1.keyCount(); i++){
        ui_baudRate->addItem(metaEnum1.key(i),metaEnum1.value(i));
    }

    QMetaEnum metaEnum2 = QMetaEnum::fromType<QSerialPort::DataBits>();
    for(int i=0; i < metaEnum2.keyCount(); i++){
        ui_dataBit->addItem(metaEnum2.key(i),metaEnum2.value(i));
    }

    QMetaEnum metaEnum3 = QMetaEnum::fromType<QSerialPort::Parity>();
    for(int i=0; i < metaEnum3.keyCount(); i++){
        ui_parity->addItem(metaEnum3.key(i),metaEnum3.value(i));
    }

    QMetaEnum metaEnum4 = QMetaEnum::fromType<QSerialPort::StopBits>();
    for(int i=0; i < metaEnum4.keyCount(); i++){
        ui_stopBit->addItem(metaEnum4.key(i),metaEnum4.value(i));
    }

    QMetaEnum metaEnum5 = QMetaEnum::fromType<QSerialPort::FlowControl>();
    for(int i=0; i < metaEnum5.keyCount(); i++){
        ui_flowControl->addItem(metaEnum5.key(i),metaEnum5.value(i));
    }

    com->ui_open = ui_open;
    ui_open->setCheckable(true);
    ui_open->setChecked(false);
    ui_open->setText("打开串口");

    connect(ui_open,QPushButton::clicked,com,[&](bool checked){
        if(checked){
            com->port->setPortName(ui_port->text());
            com->port->setBaudRate(QSerialPort::BaudRate(ui_baudRate->currentData().toInt()));
            com->port->setDataBits(QSerialPort::DataBits(ui_dataBit->currentData().toInt()));
            com->port->setParity(QSerialPort::Parity(ui_parity->currentData().toInt()));
            com->port->setStopBits(QSerialPort::StopBits(ui_stopBit->currentData().toInt()));
            com->port->setFlowControl(QSerialPort::FlowControl(ui_flowControl->currentData().toInt()));

            com->port->open(QIODevice::ReadWrite);
            ui_open->setText("关闭串口");
        }else{
            com->port->close();
            ui_open->setText("打开串口");
        }
    });
}
