#include <QtTest>

#include <QDebug>

#include "tst_main.h"
#include "dependencypool.h"
#include "dependencypointer.h"
#include "adder.h"

MainTest::MainTest(QObject *parent) : QObject(parent) {}

void MainTest::initTestCase()
{
    qDebug() << Q_FUNC_INFO;
    dep->add<Adder>();
}

void MainTest::pointer()
{
    Dependency::Pointer<Adder> a;
    QCOMPARE(a->add(2, 3), 5);
}

int run_inject(int n1, int n2, Adder *a = Dependency::Inject)
{
    return a->add(n1, n2);
}

void MainTest::inject()
{
    QCOMPARE(run_inject(7, 3), 10);
}

void MainTest::get()
{
    auto a = Dependency::Pool::instance()->get<Adder>();
    QCOMPARE(a->add(7, 3), 10);
}

void MainTest::macro_get()
{
    auto a = dep->get<Adder>();
    QCOMPARE(a->add(7, 3), 10);
}

QTEST_MAIN(MainTest)
