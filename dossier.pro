#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T17:48:15
#
#-------------------------------------------------

QT       += core gui webkit network sql

TARGET = dossier
TEMPLATE = app

INCLUDEPATH += ../../qjson/include

linux* {
INCLUDEPATH += /usr/include/cppconn

LIBS += ../../qjson/build/lib/libqjson.so
LIBS += /usr/lib/libmysqlclient.so
LIBS += /usr/lib/libmysqlcppconn.so
#LIBS += /usr/lib/qt4/plugins/sqldrivers/libqsqlmysql.so
#LIBS += /home/wim/QtSDK/QtSources/4.7.3/src/plugins/sqldrivers/mysql-build-desktop-Desktop_Qt_4_7_3_for_GCC__Qt_SDK__Release/libqsqlmysql.so
LIBS += /home/wim/QtSDK/QtSources/4.7.3/src/plugins/sqldrivers/mysql/libqsqlmysql.so
}

win32-g++ {
    LIBS += ../qjson/build/lib/qjson0.dll
    LIBS += ../qjson/build/lib/libqjson.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.lib
#I don't think these ones are necessary, but just in case:
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\libqsqlmysql4.a
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\qsqlmysql4.dll
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\qsqlmysqld4.dll
}

win32-msvc* {
    LIBS += ../qjson/build/lib/qjson0.lib
    LIBS += ../qjson/build/lib/libqjson.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.lib
#I don't think these ones are necessary, but just in case:
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\libqsqlmysql4.a
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\qsqlmysql4.dll
    LIBS += C:\\Wim\\Qt\\4.8.6\\plugins\\sqldrivers\\qsqlmysqld4.dll
}

macx* {
LIBS += -F../qjson/build/lib -framework qjson
}


SOURCES += main.cpp\
        dialog.cpp \
    adddossierdialog.cpp

HEADERS  += dialog.h \
    adddossierdialog.h

FORMS    += dialog.ui \
    adddossierdialog.ui
