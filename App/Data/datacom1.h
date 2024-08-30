#ifndef DATACOM1_H
#define DATACOM1_H

#include <QObject>
#include <Qtimer>
#include <QQueue>
#include <functional>
#include <tuple>
#include "Base/BaseFun/base.h"
#include "App/Data/database.h"
#include "Base/BaseFun/TimerPool/timerpool.h"
class DataCom1 : public QObject
{
    Q_OBJECT
    B_INSTANCE(DataCom1)
    public:
        explicit DataCom1(QObject *parent = 0);
    //公共
public:
    QString CurName = "";
    int timerId = -1;
    char devId = -1;

    void init();
    void startTest(char test);
    void stopTest();
    void ctlInstrumentTest(char test);
    void readDataDeal(std::function<void ()> fun1 ,
                      std::function<void (QByteArray &data)> fun2);
    QQueue<std::function<void (QByteArray &data)>> QueInfo;

public:
    QMap<QString,
    std::tuple<std::function<void (QByteArray&)> , std::function<void ( std::function<void (QByteArray&)> )> > > mapInfoDeal;
    void readDataDeal(QString CurName, std::function<void ()> fun1 ,
                      std::function<void (QByteArray &data)> fun2);
public slots:
    void slot_reReadData(QByteArray data);

    //突加
public:

    typedef struct AbruptAdd{
        uint timeSpace = 0;
        uint vaildNum = 0;
        QList<int> lstIData;
        QVector<QList<float> > vListFData;

        float tj_1_zd = 0;
        float tj_2_zd = 0;
        float tj_3_zd = 0;
        float tj_pj_zd =0;

        float tj_1_zx = 0;
        float tj_2_zx = 0;
        float tj_3_zx = 0;
        float tj_pj_zx =0;

        float tj_1_ss = 0;
        float tj_2_ss = 0;
        float tj_3_ss = 0;
        float tj_pj_ss = 0;

        float tj_1_hf = 0;
        float tj_2_hf = 0;
        float tj_3_hf = 0;
        float tj_pj_hf = 0;

        float tj_pl_zd = 0;
        float tj_pl_zx = 0;
        float tj_pl_ss = 0;
        float tj_pl_hf = 0;


        AbruptAdd(){
            if(vListFData.isEmpty()){
                vListFData.reserve(9);
            }
        }
    }AbruptAdd;

    AbruptAdd abruptAddData;

    void startAbruptAdd();
    void abruptAdd();

    void readAbruptAddData(QByteArray data);

    QQueue<std::function<void (QByteArray &data)>> &QueInfoAbruptAdd = QueInfo;
signals:
    void abruptAddUpdate(AbruptAdd &data);


    //突卸
public:

    typedef struct AbruptUnload{
        uint timeSpace = 0;
        uint vaildNum = 0;
        QList<int> lstIData;
        QVector<QList<float> > vListFData;

        AbruptUnload(){
            if(vListFData.isEmpty()){
                vListFData.reserve(9);
            }
        }
    }AbruptUnload;

    AbruptUnload abruptUnloadData;

    void startAbruptUnload();
    void abruptUnload();

    void readAbruptUnloadData(QByteArray data);

    QQueue<std::function<void (QByteArray &data)>> &QueInfoAbruptUnload = QueInfo;
signals:
    void abruptUnloadUpdate(AbruptUnload var);
};

#endif // DATACOM1_H
