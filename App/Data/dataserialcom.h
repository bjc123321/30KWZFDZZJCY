#ifndef DATASERIALCOM_H
#define DATASERIALCOM_H

#include <QObject>


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

    void onTabChanged(int index);

public:
    // 控制数据命令队列
    QQueue<QByteArray> controlDataQueue;

    //待输出到页面的字符串队列
    QQueue<QString> dataStrQueue;
    // 串口控制忙碌标志
    bool isSerialControlBusy = false;
    // 从队列发送下一个控制的数据
    void sendNextControlData();
    //解析返回的缓存帧中的数据域
    void analyzingData(const QByteArray &data);





signals:
    void updatePage(QQueue<QString> dataStrQueue);

private slots:

    void onDataReceived(const QString &portName, const QByteArray &data);//测试

public slots:






};

#endif // DATASERIALCOM_H
