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

private:
    // 构造函数私有化，防止外部实例化
    explicit GlobalSettings(QObject* parent = nullptr)
        : QObject(parent), currentLoad(0), fanIsOpen(false){}

    // 禁止拷贝构造和赋值操作
    GlobalSettings(const GlobalSettings&) = delete;
    GlobalSettings& operator=(const GlobalSettings&) = delete;

    // 全局变量
    int currentLoad;
    bool fanIsOpen;

};

#endif // GLOBALSETTINGS_H
