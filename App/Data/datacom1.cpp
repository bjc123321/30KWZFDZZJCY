#include "datacom1.h"

DataCom1::DataCom1(QObject *parent) : QObject(parent)
{
    connect(&DataBase::U(),DataBase::signal_reRead,this,slot_reReadData);

    devId = 0x01;

    init();
}

void DataCom1::init()
{
    mapInfoDeal["突加"] = std::tuple<std::function<void (QByteArray&)> , std::function<void ( std::function<void (QByteArray&)> )> >{
        [&](QByteArray &data){
            readAbruptAddData(data);
        },
        [&](std::function<void (QByteArray&)>fun){
            QueInfoAbruptAdd.append(fun);
        }
    };

    mapInfoDeal["突卸"] = std::tuple<std::function<void (QByteArray&)> , std::function<void ( std::function<void (QByteArray&)> )> >{
        [&](QByteArray &data){
            readAbruptUnloadData(data);
        },
        [&](std::function<void (QByteArray&)>fun){
            QueInfoAbruptUnload.append(fun);
        }
    };
}


void DataCom1::slot_reReadData(QByteArray data)
{
    auto lead = mapInfoDeal[CurName];
    auto fun = std::get<0>(lead);
    fun(data);
}

void DataCom1::startTest(char test)
{
    DataBase::U().ComRTU_writeData(devId,0x00,0x00,0x01,QVector<char>{
                                       0x00,test});
}

void DataCom1::stopTest()
{
    //切换至首页5
    DataBase::U().ComRTU_wlockPage(devId,5);
    //停止
    DataBase::U().ComRTU_writeData(devId,0x00,0x00,0x01,QVector<char>{
                                       0x00,0x20});
    DataBase::U().ComRTU_writeData(devId,0x00,0x00,0x01,QVector<char>{
                                       0x00,0x30});

    TimerPool::Stop(timerId);
}

void DataCom1::ctlInstrumentTest(char test)
{
    //切换当前 仪表显示页
    DataBase::U().ComRTU_writeData(devId,0x00,0x00,0x01,QVector<char>{
                                       0x00,test});
}

void DataCom1::readDataDeal(std::function<void ()> fun1, std::function<void (QByteArray &data)> fun2)
{
    readDataDeal(CurName,fun1,fun2);
}

void DataCom1::readDataDeal(QString CurName, std::function<void ()> fun1, std::function<void (QByteArray &)> fun2)
{
    //确保仪表工作模式切换为接收状态，后发送读取数据命令
    if(fun1 != nullptr && fun2 != nullptr){
        auto lead = mapInfoDeal[CurName];
        TimerPool::Start(50*(QueInfo.size()+1),[=](int tid){
            fun1();
        },true,false);

        auto fun = std::get<1>(lead);
        fun(fun2);
    }
}

void DataCom1::startAbruptAdd()
{
    stopTest();

    ctlInstrumentTest(0x47);//切换仪表显示页到1
    DataBase::U().ComRTU_wlockPage(devId,1);
    CurName = "突加";

    abruptAdd();
}

void DataCom1::abruptAdd()
{
    //突加时间间距
    readDataDeal([&](){
        DataBase::U().ComRTU_readData(devId,0x00,0x2B,0x01);
    },[&](QByteArray &data){
        QByteArray d = data.mid(3,data[2]);
        if(data[0] == 0x01){
            abruptAddData.timeSpace = 0;
            for(int i = 0;i<d.size();i++){
                abruptAddData.timeSpace += d[i] * (1<<(i*8));
            }
        }
    });

    startTest(0x24);

    timerId = TimerPool::Start(10000,[&](int tid){
        //线路1突加最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x02,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_1_zd =  ModbusRTU::floatData(d);
            }
        });
        //线路2突加最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x04,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_2_zd =  ModbusRTU::floatData(d);
            }
        });
        //线路3突加最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x06,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_3_zd =  ModbusRTU::floatData(d);
            }
        });
        //平均突加最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x08,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pj_zd =  ModbusRTU::floatData(d);
            }
        });
        //线路1突加最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x0A,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_1_zx =  ModbusRTU::floatData(d);
            }
        });
        //线路2突加最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x0C,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_2_zx =  ModbusRTU::floatData(d);
            }
        });
        //线路3突加最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x0E,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_3_zx =  ModbusRTU::floatData(d);
            }
        });
        //平均突加最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x10,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pj_zx =  ModbusRTU::floatData(d);
            }
        });
        //线路1瞬时电压偏差- 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x12,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_1_ss =  ModbusRTU::floatData(d);
            }
        });
        //线路2瞬时电压偏差- 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x14,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_2_ss =  ModbusRTU::floatData(d);
            }
        });
        //线路3瞬时电压偏差- 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x16,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_3_ss =  ModbusRTU::floatData(d);
            }
        });
        //平均瞬时电压偏差- 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x18,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                if(data[0] == 0x01){
                    QByteArray d = data.mid(3,data[2]);
                    abruptAddData.tj_pj_ss =  ModbusRTU::floatData(d);
                }
            }
        });
        //线路1电压恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x1A,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_1_hf =  ModbusRTU::floatData(d);
            }
        });
        //线路2电压恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x1C,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_2_hf =  ModbusRTU::floatData(d);
            }
        });
        //线路3电压恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x1E,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_3_hf =  ModbusRTU::floatData(d);
            }
        });
        //平均电压恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x20,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pj_hf =  ModbusRTU::floatData(d);
            }
        });
        //突加频率最大值 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x22,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pl_zd =  ModbusRTU::floatData(d);
            }
        });
        //突加频率最小值 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x24,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pl_zx =  ModbusRTU::floatData(d);
            }
        });
        //瞬时频率偏差- 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x26,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pl_ss =  ModbusRTU::floatData(d);
            }
        });
        //频率恢复时间
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x28,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.tj_pl_hf =  ModbusRTU::floatData(d);
            }
        });
        //突加有效曲线点数
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x2A,0x01);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
                abruptAddData.vaildNum = 0;
                for(int i = 0;i<d.size();i++){
                    abruptAddData.vaildNum += d[i] * (1<<(i*8));
                }
            }
        });
        //突加曲线X轴 时间
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x00,0x2C,0x01);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1突加曲线y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x03,0x4c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路2突加曲线y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x09,0x8c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路3突加曲线y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x0F,0xcc,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //平均突加曲线y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x16,0x0c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //频率曲线y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x1c,0x4c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路1突加曲线y轴 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x22,0x8c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路2突加曲线y轴 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x28,0xcc,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路3突加曲线y轴 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x2f,0x0c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //平均突加曲线y轴 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x35,0x4c,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
    },true,true);

}

void DataCom1::readAbruptAddData(QByteArray data)
{
    if(!QueInfoAbruptAdd.isEmpty()){
        qDebug() << QueInfoAbruptAdd.size();

        std::function<void (QByteArray &data)> fun = QueInfoAbruptAdd.dequeue();
        fun(data);

        qDebug() << QueInfoAbruptAdd.size();
        if(QueInfoAbruptAdd.isEmpty()){
            emit(abruptAddUpdate(abruptAddData));
        }
    }
}

void DataCom1::startAbruptUnload()
{
    stopTest();

    ctlInstrumentTest(0x48);
    DataBase::U().ComRTU_wlockPage(devId,1);
    CurName = "突卸";

    abruptUnload();
}

void DataCom1::abruptUnload()
{
    //突卸曲线x轴间距T
    readDataDeal([&](){
        DataBase::U().ComRTU_readData(devId,0x3B,0xB7,0x02);
    },[&](QByteArray &data){
        if(data[0] == 0x01){
            QByteArray d = data.mid(3,data[2]);
        }
    });

    timerId = TimerPool::Start(1000,[&](int tid){
        //线路1突卸最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x8E,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2突卸最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x90,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路3突卸最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x92,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //平均突卸最大值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x94,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x96,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x98,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路3突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x9A,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //平均突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x9C,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x96,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x98,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路3突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x9A,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //平均突卸最小值 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x9C,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1瞬时偏差+ 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0x9E,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2瞬时偏差+ 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xA0,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路3瞬时偏差+ 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xA2,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //平均瞬时偏差+ 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xA4,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xA6,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xA8,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路3恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xAA,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //平均恢复时间 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xAC,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //突加频率最大值 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xAE,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //突加频率最小值 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB0,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //瞬时频率偏差+ 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB2,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //频率恢复时间 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB4,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //频率恢复时间 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB4,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //突卸曲线点数
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB6,0x01);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //突卸X轴时间
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3B,0xB8,0x01);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1突卸y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x3E,0xD8,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路2突卸y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x45,0x18,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);

            }
        });
        //线路3突卸y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x4B,0x58,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }

        });
        //平均突卸y轴 电压
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x51,0x98,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //突卸频率曲线 频率
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x57,0xd8,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }
        });
        //线路1突卸电流曲线 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x5e,0x18,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }

        });
        //线路2突卸电流曲线 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x64,0x58,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }

        });
        //线路3突卸电流曲线 电流
        readDataDeal([&](){
            DataBase::U().ComRTU_readData(devId,0x6a,0x98,0x02);
        },[&](QByteArray &data){
            if(data[0] == 0x01){
                QByteArray d = data.mid(3,data[2]);
            }

        });
    },false);
}

void DataCom1::readAbruptUnloadData(QByteArray data)
{
    if(!QueInfoAbruptUnload.isEmpty()){
        std::function<void (QByteArray &data)> fun = QueInfoAbruptUnload.dequeue();
        fun(data);

        if(QueInfoAbruptUnload.isEmpty()){
            emit(abruptUnloadUpdate(abruptUnloadData));
        }
    }
}
