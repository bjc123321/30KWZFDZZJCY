#ifndef CONFIGINI_H
#define CONFIGINI_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QDir>
#include <functional>
#include "Base/BaseFun/base.h"

class ConfigIni : public QObject
{
    Q_OBJECT
    B_INSTANCE(ConfigIni)
private:
    explicit ConfigIni(QString basePath = QDir::currentPath(), QObject *parent = 0);
    ~ConfigIni();
    QString strPath;
    QSettings config;
public:
    void read(QString group, std::function<void(QSettings &read)> lambda);
    QMap<QString,QVariant> read(QString group);
    void write(QString group,QMap<QString,QVariant> map);
public:
    static void Read(QString group, std::function<void(QSettings &read)> lambda);
    static QMap<QString,QVariant> Read(QString group);
    static void Write(QString group,QMap<QString,QVariant> map);
signals:

public slots:
};

#endif // CONFIGINI_H
