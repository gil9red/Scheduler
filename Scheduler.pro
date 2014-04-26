QT       += core gui

TARGET = Scheduler
TEMPLATE = app

DESTDIR = ../bin

SOURCES += main.cpp\
        mainwindow.cpp \
    winsystemcommand.cpp

HEADERS  += mainwindow.h \
    appinfo.h \
    winsystemcommand.h

FORMS    += mainwindow.ui

win32:
{
    RC_FILE = app.rc
    OTHER_FILES += app.rc
}
