#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QTimer>

class DataProcessor : public QObject
{
    Q_OBJECT
    public:
        explicit DataProcessor(QObject *parent = nullptr);

    // 获取单例实例的静态方法
    static DataProcessor& getInstance() {
        static DataProcessor instance; // 静态实例对象
        return instance;
    }


    public :

    //界面数据更新
    void basic_infoChanged();


    signals:
        void newData(double time, double y1, double y2, double y3);

        void drawSignal(double time, double y1, double y2, double y3);




    public slots:
        void startProcessing();
        void stopProcessing();

    public slots:
        void generateData(double U,double I,double F);
        void generateData2();

    private:
        QTimer *dataTimer;
        double currentTime = 0.0;
        double U = 1.0;
        double I = 1.0;
        double F = 1.0;

};

#endif // DATAPROCESSOR_H
