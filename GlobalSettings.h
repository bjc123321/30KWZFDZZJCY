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
    struct BasicInfo {

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
        BasicInfo(): testingCompany("XXX厂"),execStandard("GJB"),
              testNum(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")),testDate(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")),testPerson("某某某"),
              ratedPower("30"),ratedVoltage("380.0"),ratedFreq("50"),
              productName("XXX大电机"),productModel("型号A"),productNum("FDJCY20241024"),productState("冷态"),
              phaseOrLine("相电压"),temperature("27"),relativeHumidity("30"),atmospherePressure("102") {}
    };

    static BasicInfo BASIC_INFO;  // 静态成员声明


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
