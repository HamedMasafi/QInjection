qt_add_executable(
    auto_create
    ../common/constholder.cpp ../common/constholder.h
    tst_create.cpp
)
target_include_directories(auto_create PRIVATE
    ../../src
    ../common
)

target_compile_definitions(auto_create PRIVATE
    QINJECTION_LIB
)

target_link_libraries(auto_create PRIVATE
    QInjectionStatic
    Qt::Core
    Qt::Test
)

install(TARGETS auto_create
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
