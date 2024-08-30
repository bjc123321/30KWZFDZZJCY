#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <qt_windows.h>
#include "Base/BaseFun/base.h"
#include "Dialog/login.h"
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//支持高 DPI 显示的属性
    QApplication a(argc, argv);

    // 获取主屏幕的分辨率
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    int width = screenSize.width();
    int height = screenSize.height();

    qDebug() << "Screen resolution:" << width << "x" << height;

    // 判断是否为大屏（如1920x1080及以上）
    if (width >= 1920 && height >= 1080) {
        qDebug() << "This is a large screen.";
        qputenv("QT_SCALE_FACTOR", "2.0");//大屏时解开分辨率放大2倍的代码注释
    } else {
        qDebug() << "This is not a large screen.";
    }

    // 获取主屏幕的DPI
    qreal dpi = screen->logicalDotsPerInch();

    qDebug() << "Screen DPI:" << dpi;

    // 判断是否为高清屏幕（如超过120 DPI）
    if (dpi > 120) {
        qDebug() << "This is a high DPI display.";
    } else {
        qDebug() << "This is not a high DPI display.";
    }

    static Login& l = Login::U();

    l.exec();

    if(l.result()){
        MainWindow * w = new MainWindow;
        w->show();
    }else {
        return 0;
    }

    return a.exec();
}
