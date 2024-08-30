#ifndef DATATEST_H
#define DATATEST_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QDataStream>

#include "Base/BaseFun/base.h"


// 定义一个返回数据的结构体
typedef struct ResponseData{

    QByteArray data;//参数1:仪表返回读完的数据;
    float value;//参数2:解析的浮点数;
    QString typeName; //参数3:请求命令的类型

}ResponseData;

class DataTest : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataTest)

public:

    ResponseData responseData;
    enum TYPE{
        Steady = 0,//稳态测试
        Fluctuation,//波动测试
        Tuning//整定测试
    };
    explicit DataTest(QObject *parent = 0);


    //初始化
    void init();
    //读数据请求类型初始化
    void requestTypeInit();

    //将16进制的数据位解析成浮点数
    float getFloatValue(QByteArray orgData);
    //向仪表发送请求读取数据的16进制命令
    QString hexCmdExe(QString typeStr);

    void Steady_Test();//稳态测试
    void Fluctuation_Test();//波动测试
    void Tuning_Test();//整定测试
    void resolveData(QByteArray data);



private:

    QVector<QString> steadyVector;//稳态数据类型
    QQueue<QString> readTypeQueue;
    QString cmdType = "";


    QQueue<QByteArray> readQueue;
    QQueue<QByteArray> reReadQueue;

    QTimer *timer;

signals:
    void sigal_readData(QByteArray data);

    void signal_setValue(ResponseData responseData);//发送仪表返回的数据结构



public slots:
};

#endif // DATATEST_H
