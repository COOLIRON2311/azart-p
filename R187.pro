#-------------------------------------------------
#
# Project created by QtCreator 2022-11-20T19:35:48
#
#-------------------------------------------------

QT       += core gui multimedia network 3dcore 3drender 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = R187
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 resources_big # static

# QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        modalwindow3d.cpp \
        modalwindownorm.cpp \
        modalwindowrd.cpp \
        modalwindowrules.cpp \
        protocol.cpp \
        styles.cpp

HEADERS += \
        listviewdelegate.h \
        mainwindow.h \
        modalwindow3d.h \
        modalwindownorm.h \
        modalwindowrd.h \
        modalwindowrules.h \
        protocol.h

FORMS += \
        mainwindow.ui \
        modalwindownorm.ui \
        modalwindowrd.ui \
        modalwindowrules.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
