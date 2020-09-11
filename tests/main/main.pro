QT += testlib
QT -= gui

TEMPLATE = app
TARGET = tst_main

#CONFIG += qt console warn_on depend_includepath testcase
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    adder.cpp \
    tst_main.cpp

HEADERS += \
    adder.h \
    tst_main.h


include(../../src/src.pri)
