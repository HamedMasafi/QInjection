QT += testlib
QT -= gui

TEMPLATE = app
TARGET = tst_main

#CONFIG += qt console warn_on depend_includepath testcase
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../common/adder.cpp \
    ../common/constholder.cpp \
    lifetimereporter.cpp \
    tst_main.cpp

HEADERS += \
    ../common/adder.h \
    ../common/constholder.h \
    lifetimereporter.h \
    tst_main.h

INCLUDEPATH += ../common/
include(../../src/src.pri)
