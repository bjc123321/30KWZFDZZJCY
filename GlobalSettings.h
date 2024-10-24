#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>

// 单例的全局变量类
class GlobalSettings : public QObject
{
    Q_OBJECT

public:
    // 获取单例对象实例
    static GlobalSettings& instance() {
        static GlobalSettings instance; // 确保只创建一次实例
        return instance;
    }

    enum TEST_TYPE{
        STEADY = 0,      //稳态测试
        TUNING,          //整定测试
        SUDD_LOAD,       //突加测试
        SUDD_UNLOAD,     //突卸测试
        RECOR_DWAVE,      //录波测试
        OTHERS            //其他
    };

    // 定义结构体
    struct BASIC_INFO {

        QString testingCompany;     // 测试单位
        QString execStandard;       // 执行标准

        QString testNum;            // 测试编号
        QString testDate;           // 测试时间
        QString testPerson;         // 测试人员

        QString ratedPower;         // 额定功率
        QString ratedVoltage;       // 额定电压
        QString ratedFreq;          // 额定频率

        QString productName;        // 产品名称
        QString productModel;       // 产品型号
        QString productNum;         // 产品编号
        QString productState;       // 产品状态

        QString phaseOrLine;        // 相/线
        QString temperature;        // 环境温度℃
        QString relativeHumidity;   // 相对湿度%
        QString atmospherePressure; // 大气压力kPa

        // 构造函数
        BASIC_INFO(const QString &t_testingCompany, const QString &t_execStandard,
                   const QString &t_testNum, const QString &t_testDate, const QString &t_testPerson,
                   const QString &t_ratedPower, const QString &t_ratedVoltage, const QString &t_ratedFreq,
                   const QString &t_productName, const QString &t_productModel, const QString &t_productNum, const QString &t_productState,
                   const QString &t_phaseOrLine, const QString &t_temperature, const QString &t_relativeHumidity, const QString &t_atmospherePressure)
            : testingCompany(t_testingCompany),execStandard(t_execStandard),
              testNum(t_testNum),testDate(t_testDate),testPerson(t_testPerson),
              ratedPower(t_ratedPower),ratedVoltage(t_ratedVoltage),ratedFreq(t_ratedFreq),
              productName(t_productName),productModel(t_productModel),productNum(t_productNum),productState(t_productState),
              phaseOrLine(t_phaseOrLine),temperature(t_temperature),relativeHumidity(t_relativeHumidity),atmospherePressure(t_atmospherePressure) {}
    };

    // 设置和获取当前测试类型的值
    void setCurrentTestType(TEST_TYPE type) {
        currentTestType = type;
    }

    TEST_TYPE getCurrentTestType() const {
        return currentTestType;
    }

    // 设置和获取整型变量的值
    void setCurrentLoad(int value) {
        currentLoad = value;
    }

    int getCurrentLoad() const {
        return currentLoad;
    }

    // 设置和获取字符串变量的值
    void setFanIsOpen(bool value) {
        fanIsOpen = value;
    }

    bool getFanIsOpen() const {
        return fanIsOpen;
    }

    /**********设置和获取登录权限：1.普通用户 2.管理员**********/
    void setLoginMode(QString t_mode){
        loginMode = t_mode;
    }

    QString getLoginMode() const{
        return loginMode;
    }

private:
    // 构造函数私有化，防止外部实例化
    explicit GlobalSettings(QObject* parent = nullptr)
        : QObject(parent), currentLoad(0), fanIsOpen(false), currentTestType(STEADY),loginMode(1) {}

    // 禁止拷贝构造和赋值操作
    GlobalSettings(const GlobalSettings&) = delete;
    GlobalSettings& operator=(const GlobalSettings&) = delete;

    QString loginMode;

    // 全局变量
    int currentLoad;
    bool fanIsOpen;

    TEST_TYPE currentTestType; // 当前测试类型

};

#endif // GLOBALSETTINGS_H
