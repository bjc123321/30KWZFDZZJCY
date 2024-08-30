#ifndef OFFICEOPT_H
#define OFFICEOPT_H

#include <QObject>
#include <QAxWidget>
#include <QDir>
#include <QFile>

class OfficeOpt : public QAxWidget
{
    Q_OBJECT
public:
    explicit OfficeOpt(QString appType,QWidget *parent = 0, QString path = QDir::currentPath());

    QString basePath ="";
public:
    bool createFile(QString name);
    bool roomFile(QString name);

    QList<QString> getDirAllFileName(QStringList AfterMost);


signals:

public slots:
};

#endif // OFFICEOPT_H
