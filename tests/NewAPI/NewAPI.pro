QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_newapi.cpp \
    ../common/adder.cpp

include(../../src/src.pri)

INCLUDEPATH += ../common/

HEADERS += \
    ../common/adder.h
