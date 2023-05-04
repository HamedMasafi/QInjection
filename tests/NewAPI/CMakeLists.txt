qt_add_executable(
    NewAPI
    ../common/adder.cpp ../common/adder.h
    tst_newapi.cpp
)
target_include_directories(NewAPI PRIVATE
    ../../src
    ../common
)

target_compile_definitions(NewAPI PRIVATE
    QINJECTION_LIB
)

target_link_libraries(NewAPI PRIVATE
    QInjectionStatic
    Qt::Core
    Qt::Test
)

install(TARGETS NewAPI
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
