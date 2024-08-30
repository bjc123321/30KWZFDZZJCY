#include "ConfigIni.h"
ConfigIni::ConfigIni(QString basePath, QObject *parent) :
    QObject(parent),
    strPath(basePath + "/Depend/Base/config.ini"),
    config(strPath, QSettings::IniFormat)
{
}

ConfigIni::~ConfigIni()
{
}

void ConfigIni::read(QString group, std::function<void (QSettings &)> lambda)
{
    config.beginGroup(group);

    lambda(config);

    config.endGroup();
}

QMap<QString, QVariant> ConfigIni::read(QString group)
{
    config.beginGroup(group);
    QStringList keys = config.allKeys();

    QMap<QString, QVariant> map;

    for(QString & key:keys){
        map[key] = config.value(key);
    }

    config.endGroup();

    return map;
}

void ConfigIni::write(QString group, QMap<QString, QVariant> map)
{
    config.beginGroup(group);

    for(auto it = map.begin(); it!= map.end();it++){
        config.setValue(it.key(),it.value().toString());
        config.sync();
    }

    config.endGroup();
}

void ConfigIni::Read(QString group, std::function<void (QSettings &)> lambda)
{
    ConfigIni::U().read(group,lambda);
}

QMap<QString, QVariant> ConfigIni::Read(QString group)
{
    ConfigIni::U().read(group);
}

void ConfigIni::Write(QString group, QMap<QString, QVariant> map)
{
    ConfigIni::U().write(group,map);
}
