#-------------------------------------------------
#
# Project created by QtCreator 2024-06-13T13:51:31
#
#-------------------------------------------------

QT += core gui sql network printsupport serialport
QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Use Precompiled headers (PCH),improve compilation speed!
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

include(ExcelSrc/xlsx/qtxlsx.pri)

TARGET = 30KWZFDZZJCY
TEMPLATE = app

SOURCES += main.cpp\
    App/CustomWidget/recordwaveplot.cpp \
    App/CustomWidget/threephaseplot.cpp \
    App/Data/dataserialcom.cpp \
    App/Page/detailpage.cpp \
    Base/Communication/serialCom/modbusprotocolparser.cpp \
    Base/Communication/serialCom/serialportmanager.cpp \
        mainwindow.cpp \
    Base/BaseFun/base.cpp \
    Base/BaseFun/Sql/sql.cpp \
    Base/BaseFun/ConfigIni/configini.cpp \
    Base/BaseFun/TimerPool/timerpool.cpp \
    Base/Communication/Com/com.cpp \
    Base/Communication/Com/ModbusRTU/modbusrtu.cpp \
    Base/Communication/devcenter.cpp \
    App/Data/database.cpp \
    Base/SaveInfo/SaveText/officeopt.cpp \
    App/Page/page1_fzsz.cpp \
    App/Page/page3_wtcs.cpp \
    App/Page/page4_stcs.cpp \
    App/Page/page5_xbfx.cpp \
    App/Page/page6_lbfx.cpp \
    App/Data/datacom1.cpp \
    App/Data/database2.cpp \
    App/Data/datacom3.cpp \
    App/Page/user.cpp \
    App/Page/data.cpp \
    App/Page/selftest.cpp \
    App/Page/selfinfo.cpp \
    App/CustomWidget/voltageplot.cpp \
    App/Data/datatest.cpp \
    Dialog/login.cpp \
    App/Page/save.cpp \
    App/CustomWidget/frequencyplotter.cpp \
    App/Data/dataprocessor.cpp \
    qcustomplot/qcustomplot.cpp \
    App/CustomWidget/harmonicplot.cpp \
    App/CustomWidget/triangleslider.cpp \
    App/Data/dataexportmanager.cpp \
    Base/BaseFun/Sql/databasemanager.cpp

HEADERS  += mainwindow.h \
    App/CustomWidget/recordwaveplot.h \
    App/CustomWidget/threephaseplot.h \
    App/Data/dataserialcom.h \
    App/Page/detailpage.h \
    Base/BaseFun/base.h \
    Base/BaseFun/Sql/sql.h \
    Base/BaseFun/ConfigIni/ConfigIni.h \
    Base/BaseFun/TimerPool/timerpool.h \
    Base/Communication/Com/com.h \
    Base/Communication/Com/ModbusRTU/modbusrtu.h \
    Base/Communication/devcenter.h \
    App/Data/database.h \
    Base/Communication/serialCom/modbusprotocolparser.h \
    Base/Communication/serialCom/serialportmanager.h \
    Base/SaveInfo/SaveText/officeopt.h \
    App/Page/page1_fzsz.h \
    App/Page/page3_wtcs.h \
    App/Page/page4_stcs.h \
    App/Page/page5_xbfx.h \
    App/Page/page6_lbfx.h \
    App/Data/datacom1.h \
    App/Data/database2.h \
    App/Data/datacom3.h \
    App/Page/user.h \
    App/Page/data.h \
    App/Page/selftest.h \
    App/Page/selfinfo.h \
    App/CustomWidget/voltageplot.h \
    App/Data/datatest.h \
    Dialog/login.h \
    App/Page/save.h \
    App/CustomWidget/frequencyplotter.h \
    App/Data/dataprocessor.h \
    GlobalSettings.h \
    qcustomplot/qcustomplot.h \
    stable.h \
    App/CustomWidget/harmonicplot.h \
    App/CustomWidget/triangleslider.h \
    App/Data/dataexportmanager.h \
    Base/BaseFun/Sql/databasemanager.h

FORMS    += mainwindow.ui \
    App/Page/detailpage.ui \
    App/Page/page1_fzsz.ui \
    App/Page/page3_wtcs.ui \
    App/Page/page4_stcs.ui \
    App/Page/page5_xbfx.ui \
    App/Page/page6_lbfx.ui \
    App/Page/user.ui \
    App/Page/data.ui \
    App/Page/selftest.ui \
    App/Page/selfinfo.ui \
    Dialog/login.ui \
    App/Page/save.ui

RESOURCES += \
    res.qrc

DISTFILES +=
