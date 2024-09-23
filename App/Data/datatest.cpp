#include "datatest.h"

#include "App/Data/database.h"

DataTest::DataTest(QObject *parent) : QObject(parent)
{


    requestTypeInit();
    responseData.data = QByteArray("ExampleData");
    responseData.typeName ="";
    responseData.value = 0.0;

    timer = new QTimer(this);


    connect(&DataBase::U(),DataBase::signal_reRead,this,[&](QByteArray data){

        qDebug() <<"仪表返回的\"读\"数据"<< data.toHex()<<" "<<data.size();
        // 解析浮点数（假设浮点数在数据帧的第3到第6个字节）
//        reReadQueue.enqueue(data);//将仪表返回的数据放在队列中
        float floatValue = getFloatValue(data);
        responseData.data = data;
        responseData.typeName = cmdType;
        responseData.value = floatValue;
        qDebug()<<"读取仪表返回的数据类型---------------"<<cmdType;
        emit signal_setValue(responseData);


    });
    connect(&DataBase::U(),DataBase::signal_reWrite,this,[&](QByteArray data){

        qDebug() <<"仪表返回的\"写\"数据"<< data.toHex()<<" "<<data.size();
    });

    init();


}


void DataTest::requestTypeInit()
{

    steadyVector = {"001e","0020","0022"
                   ,"002e","0030","0032"
                   ,"0038","003a","003c"
                   ,"0052","0054","0056"};
}

float DataTest::getFloatValue(QByteArray orgData)
{

    float floatValue = 0.0;
    QByteArray floatData = orgData.mid(3, 4); // 从data[3]开始,向后提取4字节
    QDataStream stream(floatData);// 创建一个QDataStream对象，绑定到floatData数组
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);// 设置QDataStream的浮点数精度为单精度（即float类型）
    stream.setByteOrder(QDataStream::LittleEndian); // 根据具体协议调整字节序:协议为小端存储
    stream >> floatValue;// 从数据流中读取一个浮点数，并存储到floatValue变量中
    qDebug() << "浮点值:" << floatValue<<"数据"<<floatData.toHex();
    return floatValue;
}


void DataTest::init()
{

    connect(timer,&QTimer::timeout,this,[=](){
        if(!readTypeQueue.isEmpty()){

            cmdType = hexCmdExe(readTypeQueue.head());
            qDebug()<<"类型："<<cmdType;
            readTypeQueue.dequeue();

        }

    });

    TYPE type = DataTest::Steady;

    if(type == DataTest::Steady){
        qDebug()<<"稳态测试";
//        Steady_Test();//2024.9.21注释

    }else if(type == DataTest::Fluctuation){
        qDebug()<<"波动测试";
    }else if(type == DataTest::Tuning){
        qDebug()<<"整定测试";
        DataBase::U().ComRTU_readData(0x01,0x13,0xD2,2);//测试前先读取仪器运行状态

        //        Tuning_Test();
        //        DataBase::U().ComRTU_readData(0x01,0x00,0x18,2);//测试前先读取仪器运行状态
        qDebug()<<"发送的数据:"<<DataBase::U().sendReadData.toHex();
    }



    //页面0切换测试
    //    DataBase::U().ComRTU_wlockPage(0x01,0);


        DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x20});//停止进行中的整定、波动、突加、突减测试等
//        DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x41});//仪表界面切换
}

void DataTest::resolveData(QByteArray data)
{
    char addressH = data[2];
    char addressL = data[3];
    //    qDebug()<<"寄存器高地址："<<addressH<<" "<<"寄存器低地址："<<addressL;
    //    qDebug()<<"数据:"<<data.begin();//会使QT卡死？？？？
    if(addressH == 0x00 && addressL == 0x1E){
        qDebug()<<"线路1电压u1";
    }else if(addressH == 0x00 && addressL == 0x20){
        qDebug()<<"线路2电压u2";
    }else if(addressH == 0x00 && addressL == 0x22){
        qDebug()<<"线路3电压u3";
    }else if(addressH == 0x00 && addressL == 0x24){
        qDebug()<<"电压uab";
    }else if(addressH == 0x00 && addressL == 0x2e){
        qDebug()<<"线路1电流I1";
    }else if(addressH == 0x00 && addressL == 0x30){
        qDebug()<<"线路2电流I2";
    }else if(addressH == 0x00 && addressL == 0x32){
        qDebug()<<"线路3电流I3";
    }else{
        qDebug()<<"其他";
    }

}


void DataTest::Steady_Test()
{
    for(int i = 0;i<steadyVector.length();i++){
        readTypeQueue.enqueue(steadyVector.at(i));
    }

    timer->start(1000);//每隔1s发送个请求数据
    if(readTypeQueue.empty()){
        qDebug()<<"全部出队后队列长度："<<readTypeQueue.length();
        timer->stop();
    }


    //页面0切换测试
    //    DataBase::U().ComRTU_wlockPage(0x01,0);

}

//波动测试
void DataTest::Fluctuation_Test()
{

    DataBase::U().ComRTU_readData(0x01,0x29,0x22,2);//读取线路一电压波动最大值信号
    qDebug()<<"发送波动数据:"<<DataBase::U().sendReadData.toHex();
    readQueue.enqueue(DataBase::U().sendReadData);
    while(!readQueue.isEmpty()){

        qDebug()<<readQueue.head().toHex().toUpper();//队头打印
        resolveData(readQueue.head());
        readQueue.dequeue();
    }
    //    DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x20});//停止进行中的整定、波动、突加、突减测试等
    //    DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x23});//进行波动测试
    //    qDebug()<<"发送的数据为："<<DataBase::U().reWriteData.toHex();
}

//整定测试
void DataTest::Tuning_Test()
{

    //    DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x20});//停止进行中的整定、波动、突加、突减测试等
    //电压整定
    DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x21});//进行中的整定测试

    //频率整定
    //    DataBase::U().ComRTU_writeData(0x01,0x00,0x00,1,QVector<char>{0x00,0x22});//进行频率整定测试
    //    qDebug()<<"发送的数据为："<<DataBase::U().reWriteData.toHex();


}


QString DataTest::hexCmdExe(QString typeStr)
{

    // 将QString(Unicode 字符串)转换为 UTF-8 编码的字节数组
    QByteArray hexData = typeStr.toUtf8();
    // 使用fromHex函数将16进制字符串转换为QByteArray
    QByteArray byteArray = QByteArray::fromHex(hexData);
    // 打印转换后的QByteArray
    //        qDebug() << "Hex String:" << hexData;
    //        qDebug() << "Decoded QByteArray:" << byteArray;
    //        qDebug() << "QByteArray大小" << byteArray.size();
    //        char H =byteArray.at(0);
    //        char L =byteArray.at(1);
    //        qDebug() << "QByteArray[0]" << QString(H)<< "QByteArray[1]" << QString(L);
    DataBase::U().ComRTU_readData(0x01,byteArray.at(0),byteArray.at(1),2);//读取示数
    qDebug()<<"请求数据类型:"<<DataBase::U().sendReadData.toHex();

    return typeStr;

}

