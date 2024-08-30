#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QtGlobal>

class GlobalSettings {
public:
    bool fanIsOpen = false;//风机状态

};

// 使用 Q_GLOBAL_STATIC 宏来定义全局静态变量
static GlobalSettings& globalSettingsInstance() {
    static GlobalSettings instance;
    return instance;
}


#endif // GLOBALSETTINGS_H
