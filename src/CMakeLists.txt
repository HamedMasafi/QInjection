set(
    QInjection_Source
    dep_global.h
    dependencycreator.cpp dependencycreator.h
    dependencyinjector.cpp dependencyinjector.h
    dependencypointer.cpp dependencypointer.h
    dependencypool.cpp dependencypool.h
)

qt_add_library(
    QInjection
    ${QInjection_Source}
)
qt_add_library(
    QInjectionStatic
    STATIC
    ${QInjection_Source}
)

target_compile_definitions(QInjection PUBLIC
    QT_DEPRECATED_WARNINGS
    SRC_LIBRARY
)
target_compile_definitions(
    QInjectionStatic
    PUBLIC
    QT_DEPRECATED_WARNINGS
    SRC_LIBRARY
)

target_link_libraries(
    QInjection
    PUBLIC
    Qt::Core
)
target_link_libraries(
    QInjectionStatic
    PUBLIC
    Qt::Core
)

target_include_directories(
    QInjection
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    QInjectionStatic
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS QInjection
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
