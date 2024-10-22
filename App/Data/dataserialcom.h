#ifndef DATASERIALCOM_H
#define DATASERIALCOM_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

class DataSerialCom : public QObject
{
    Q_OBJECT
public:
    explicit DataSerialCom(QObject *parent = nullptr);

    // 获取单例实例的静态方法
    static DataSerialCom& getInstance() {
        static DataSerialCom instance; // 静态实例对象
        return instance;
    }

    enum TEST_TYPE{
        STEADY = 0,      //稳态测试
        TUNING,          //整定测试
        SUDD_LOAD,       //突加测试
        SUDD_UNLOAD,     //突卸测试
        RECOR_DWAVE      //录波测试

    };

    void onTabChanged(int index);



public:


    QString portName = "";
    QString portDescription = "";

    //待输出到页面的字符串队列
    QQueue<QString> dataStrQueue;

    /*********************仪表****************************/
    // 控制仪表命令队列
    QQueue<QByteArray> controlPanelQueue;
    // 每条串口收到的"控制仪表"16进制命令忙碌标志
    bool isControlPanelHexBusy = false;
    // 从队列发送下一个"控制仪表"的16进制命令
    void sendControlPanelHex();

    /*********************负载****************************/
    // 控制负载命令队列
    QQueue<QByteArray> controlLoadQueue;
    // 每条串口收到的"控制负载"16进制命令忙碌标志
    bool isControlLoadHexBusy = false;
    // 从队列发送下一个"控制负载"的16进制命令
    void sendControlLoadHex();



    //解析返回的缓存帧中的数据域
    void analyzingData(const QByteArray &data);



    void steadyRequest();

    void suddLoadRequest();

    int type = TEST_TYPE::STEADY;


private :


    QTimer *steadyTimer = nullptr;
    //请求数据的timer每隔100ms发出请求消息
    QTimer *suddLoadTimer = nullptr;


    void pageCodeRequest(int index);



signals:

    void updateSteadyPageSignal(QQueue<QString> dataStrQueue);

    void updateSuddLoadPageSignal(QQueue<QString> dataStrQueue);

private slots:

    void onDataReceived(const QSerialPort &port, const QByteArray &data);//测试

public slots:

    void StartSteadySlot();

    void StopSteadySlot();

    void startSuddIncreaseSlot();

    void stopSuddLoadSlot();

    void startRecordWaveSlot();

    void readRecordWaveSlot();


    void setFengJiSlot(bool isOpen);



};

#endif // DATASERIALCOM_H
