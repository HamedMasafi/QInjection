INCLUDEPATH += $$PWD

DEFINES += QINJECTION_LIB

SOURCES += \
    $$PWD/dependencypointer.cpp \
    $$PWD/dependencypool.cpp

HEADERS += \
    $$PWD/dep_global.h \
    $$PWD/dependencypointer.h \
    $$PWD/dependencypool.h \
    $$PWD/dependencypool_p.h
