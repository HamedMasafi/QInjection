#include <QtTest>

#include <QDebug>

#include "tst_main.h"
#include "dependencypool.h"
#include "dependencypointer.h"
#include "dependencyinjector.h"
#include "adder.h"
#include "constholder.h"
#include "lifetimereporter.h"

bool lifeTime;

LifeTimeReporter *createLifeTimeReporter() {
    return new LifeTimeReporter(&lifeTime);
}

ConstHolder *createRandomNumberHolder() {
    qDebug() << "Creating const helper";
    auto o = new ConstHolder;
    static int index = 1;
    o->setValue(index);
    index++;
    return o;
}

MainTest::MainTest(QObject *parent) : QObject(parent) {
    QInjection::addScopped(createRandomNumberHolder);
}

void MainTest::initTestCase()
{
    qDebug() << Q_FUNC_INFO;
    QInjection::addSingleton<Adder>();
}

void MainTest::pointer()
{
    QInjection::Pointer<Adder> a;
    QCOMPARE(a->add(2, 3), 5);
}

int run_inject(int n1, int n2, Adder *a = QInjection::Inject)
{
    return a->add(n1, n2);
}

void MainTest::inject()
{
    QCOMPARE(run_inject(7, 3), 10);
}

void MainTest::get()
{
    auto a = QInjection::create<Adder>();
    QCOMPARE(a->add(7, 3), 10);
}

void MainTest::scopped()
{
    int v1{0};
    int v2{0};
    {
        QInjection::Pointer<ConstHolder> h1;
        v1 = h1->value();
    }
    {
        QInjection::Pointer<ConstHolder> h2;
        v2 = h2->value();
    }
    QCOMPARE(v1, 1);
    QCOMPARE(v2, 2);
    QTEST_ASSERT(v1 != v2);
}

void MainTest::scope_lifetime()
{
    QInjection::addScopped(createLifeTimeReporter);

    QInjection::Pointer<LifeTimeReporter> l;
    QCOMPARE(lifeTime, true);
    qApp->processEvents();
}

void MainTest::check_scope_lifetime()
{
    QCOMPARE(lifeTime, false);
}

bool run_lifetime_inject(LifeTimeReporter *l = QInjection::Inject)
{
    Q_UNUSED(l);
    return lifeTime;
}

void MainTest::scope_lifetime2()
{
    run_lifetime_inject();
    QCOMPARE(lifeTime, true);
    qApp->processEvents();
}

void MainTest::check_scope_lifetime2()
{
    QCOMPARE(lifeTime, false);
}

QTEST_MAIN(MainTest)
