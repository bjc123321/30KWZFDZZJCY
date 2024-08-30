#ifndef DATACOM3_H
#define DATACOM3_H

#include <QObject>
#include <Qtimer>
#include <QQueue>
#include <QMap>
#include <functional>
#include <tuple>
#include "Base/BaseFun/base.h"
#include "App/Data/database2.h"
#include "Base/BaseFun/TimerPool/timerpool.h"
class DataCom3 : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataCom3)
public:
    explicit DataCom3(QObject *parent = 0);

    int devId = 0;

    int timerId = -1;
    QString CurName = "自动";
    QMap<QString,
    std::tuple<std::function<void (QByteArray&)> , std::function<void ( std::function<void (QByteArray&)> )> > > mapInfoDeal;

    void readDataDeal(std::function<void ()> fun1 ,
                      std::function<void (QByteArray &data)> fun2);
    QQueue<std::function<void (QByteArray &data)>> QueInfo;
    void readInfoData(QByteArray data);
public:
    //初始化
    typedef struct InitData{
        bool fjzt = false;//风机状态
        bool cwbj = false;//超温报警

        bool RV0_1 = false;
        bool RV0_2 = false;
        bool R2V0_2 = false;
        bool RV0_5 = false;
        bool RV1 = false;
        bool RV2 = false;
        bool R2V2 = false;
        bool RV5 = false;
        bool R2V5 = false;
        bool RV10 = false;
        bool R2V10 = false;

        bool VV0_5 = false;
        bool VV1 = false;
        bool VV2 = false;
        bool V2V2 = false;
        bool V3V2 = false;
        bool V4V2 = false;
        bool VV5 = false;
        bool VV10 = false;
    }InitData;

    InitData initData;

    void init();
public:
    QMap<bool*,char> ledPbs = {
        {&initData.RV0_1,0x70},
        {&initData.RV0_2,0x71},
        {&initData.R2V0_2,0x72},
        {&initData.RV0_5,0x73},
        {&initData.RV1,0x74},
        {&initData.RV2,0x75},
        {&initData.R2V2,0x76},
        {&initData.RV5,0x77},
        {&initData.R2V5,0x78},
        {&initData.RV10,0x79},
        {&initData.R2V10,0x80},

        {&initData.VV0_5,0x81},
        {&initData.VV1,0x82},
        {&initData.VV2,0x83},
        {&initData.V2V2,0x84},
        {&initData.V3V2,0x85},
        {&initData.V4V2,0x86},
        {&initData.VV5,0x87},
        {&initData.VV10,0x88}
    };
    //设置负载
    void setFZ(bool*ptr,bool isOpen);

    //风机开关
    void setFJKG(bool isOpen);
    //急停开关
    void setJTKG(bool isOpen);
    //加载模式
    void setLoadIsZD(bool isZD);
    //自动卸载
    void setZDXZ(bool isXZ);
    //手动卸载
    void setSDXZ(bool isXZ);
    //自动模式输入总容量功率 1~500
    void setZDZRL(int rl);
    //自动模式输入功率因素 1~100
    void setZDGLYS(int ys);
signals:
    void InitDataUpdate(InitData initData);
public slots:
    void slot_reReadData(QByteArray data);

};

#endif // DATACOM3_H
