#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T13:56:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TTChat
TEMPLATE = app


SOURCES += main.cpp\
        chatdialog.cpp \
    client.cpp \
    connection.cpp

HEADERS  += chatdialog.h \
    client.h \
    connection.h

FORMS    += chatdialog.ui
