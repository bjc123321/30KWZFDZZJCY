#include "datacom3.h"

DataCom3::DataCom3(QObject *parent) : QObject(parent)
{
    connect(&DataBase2::U(),DataBase2::signal_reRead,this,slot_reReadData);

    devId = 0x01;

    //init();
}

void DataCom3::init()
{
    mapInfoDeal["自动"] = std::tuple<std::function<void (QByteArray&)> , std::function<void ( std::function<void (QByteArray&)> )> >{
        [&](QByteArray &data){
            readInfoData(data);
        },
        [&](std::function<void (QByteArray&)>fun){
            QueInfo.append(fun);
        }
    };

    timerId = TimerPool::Start(1000,[&](int tid){
        //线路1读取风机指示灯
        readDataDeal([&](){
            DataBase2::U().ComRTU_readData(devId,0x00,0x02,0x01);
        },[&](QByteArray &data){
            QByteArray d = data.mid(3,data[2]);
            initData.fjzt = d[1];
        });

        //线路1读取超温报警
        readDataDeal([&](){
            DataBase2::U().ComRTU_readData(devId,0x00,0x03,0x01);
        },[&](QByteArray &data){
            QByteArray d = data.mid(3,data[2]);
            initData.cwbj = d[1];
        });


        //读取电阻电感状态
        for(auto it = ledPbs.begin(); it!=ledPbs.end(); it++){
            readDataDeal([&](){
                DataBase2::U().ComRTU_readData(0x01,0x00,it.value(),0x01);
            },[&](QByteArray &data){
                QByteArray d = data.mid(3,data[2]);
                *(it.key()) = d[1];
            });
        }

    });
}

void DataCom3::setFZ(bool*ptr,bool isOpen)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,ledPbs[ptr],QVector<char>{
                                          0x00,isOpen
                                    });
}

void DataCom3::readDataDeal(std::function<void ()> fun1, std::function<void (QByteArray &)> fun2)
{
    if(fun1 != nullptr && fun2 != nullptr){
        auto lead = mapInfoDeal[CurName];

        TimerPool::Start(100*(QueInfo.size()+1),[=](int tid){
            fun1();
        },true);

        auto fun = std::get<1>(lead);
        fun(fun2);
    }
}

void DataCom3::readInfoData(QByteArray data)
{
    if(!QueInfo.isEmpty()){
        std::function<void (QByteArray &data)> fun = QueInfo.dequeue();
        fun(data);

        if(QueInfo.isEmpty()){
            emit(InitDataUpdate(initData));
        }
    }
}


//设置风机开关
void DataCom3::setFJKG(bool isOpen)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,0x01,QVector<char>{
                                        0x00,isOpen
                                    });
}


//设置急停开关
void DataCom3::setJTKG(bool isOpen)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,0x05,QVector<char>{
                                        0x00,isOpen
                                    });
}

//设置加载模式,注意协议中给的是10进制,要转换为16进制传入数据
void DataCom3::setLoadIsZD(bool isZD)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,0x30,QVector<char>{
                                        0x00,isZD
                                    });
}

void DataCom3::setZDXZ(bool isXZ)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,0x35,QVector<char>{
                                        0x00,isXZ
                                    });
}

void DataCom3::setSDXZ(bool isXZ)
{
    DataBase2::U().ComRTU_writeData(devId,0x00,0x36,QVector<char>{
                                        0x00,isXZ
                                    });
}

void DataCom3::setZDZRL(int rl)
{
    char h = rl >> 8;
    char l = rl  - (h << 8);
    DataBase2::U().ComRTU_writeData(devId,0x00,0x50,QVector<char>{
                                        h,l
                                    });
}

void DataCom3::setZDGLYS(int ys)
{
    char h = ys >> 8;
    char l = ys - (h << 8);
    DataBase2::U().ComRTU_writeData(devId,0x00,51,QVector<char>{
                                        h,l
                                    });
}

void DataCom3::slot_reReadData(QByteArray data)
{
    auto lead = mapInfoDeal[CurName];
    auto fun = std::get<0>(lead);
    fun(data);
}
