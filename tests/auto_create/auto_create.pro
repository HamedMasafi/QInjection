QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase cpp14
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_create.cpp \
    ../common/constholder.cpp

HEADERS += \
    ../common/constholder.h

INCLUDEPATH += ../common/

include(../../src/src.pri)
