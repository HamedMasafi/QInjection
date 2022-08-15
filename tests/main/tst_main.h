#ifndef TST_MAIN_H
#define TST_MAIN_H

#include <QObject>
#include <adder.h>
#include "dependencypointer.h"

class Adder;
class MainTest : public QObject
{
    Q_OBJECT
    QInjection::Pointer<Adder> tmp;

public:
    MainTest(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void pointer();
    void inject();
    void get();
    void scopped();
    void scope_lifetime();
    void check_scope_lifetime();
    void scope_lifetime2();
    void check_scope_lifetime2();
};

#endif // TST_MAIN_H
