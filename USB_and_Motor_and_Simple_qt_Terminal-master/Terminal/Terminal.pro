#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui\
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Terminal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RC_FILE = icon.rc

RESOURCES += \
    terminal_res.qrc
