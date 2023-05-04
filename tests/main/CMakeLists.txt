qt_add_executable(
    tst_main WIN32 MACOSX_BUNDLE
    ../common/adder.cpp ../common/adder.h
    ../common/constholder.cpp ../common/constholder.h
    lifetimereporter.cpp lifetimereporter.h
    tst_main.cpp tst_main.h
)
target_include_directories(tst_main PRIVATE
    ../../src
    ../common
)

target_compile_definitions(tst_main PRIVATE
    QINJECTION_LIB
)

target_link_libraries(tst_main PRIVATE
    QInjectionStatic
    Qt::Core
    Qt::Test
)

install(TARGETS tst_main
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
