cmake_minimum_required(VERSION 3.10)

project(QInjection LANGUAGES CXX)

# Try to find Qt6 first, fallback to Qt5
find_package(Qt6 COMPONENTS Core QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    set(QT_CORE_TARGET Qt6::Core)
else()
    set(QT_VERSION_MAJOR 5)
    find_package(Qt5 REQUIRED COMPONENTS Core)
    set(QT_CORE_TARGET Qt5::Core)
endif()

# Setup automoc/uic/rcc for Qt5
if(QT_VERSION_MAJOR EQUAL 5)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
endif()

set(QInjection_Source
    dep_global.h
    dependencycreator.cpp dependencycreator.h
    dependencyinjector.cpp dependencyinjector.h
    dependencypointer.cpp dependencypointer.h
    dependencypool.cpp dependencypool.h
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_library(QInjection ${QInjection_Source})
    qt_add_library(QInjectionStatic STATIC ${QInjection_Source})
else()
    add_library(QInjection ${QInjection_Source})
    add_library(QInjectionStatic STATIC ${QInjection_Source})
endif()

target_compile_definitions(QInjection PUBLIC
    QT_DEPRECATED_WARNINGS
    SRC_LIBRARY
)
target_compile_definitions(QInjectionStatic PUBLIC
    QT_DEPRECATED_WARNINGS
    SRC_LIBRARY
)

target_link_libraries(QInjection PUBLIC ${QT_CORE_TARGET})
target_link_libraries(QInjectionStatic PUBLIC ${QT_CORE_TARGET})

target_include_directories(QInjection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(QInjectionStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS QInjection
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
