INCLUDEPATH += $$PWD

DEFINES += QINJECTION_LIB

SOURCES += \
    $$PWD/dependencycreator.cpp \
    $$PWD/dependencyinjector.cpp \
    $$PWD/dependencypointer.cpp \
    $$PWD/dependencypool.cpp

HEADERS += \
    $$PWD/dep_global.h \
    $$PWD/dependencycreator.h \
    $$PWD/dependencyinjector.h \
    $$PWD/dependencypointer.h \
    $$PWD/dependencypool.h \
    $$PWD/dependencypool_p.h
