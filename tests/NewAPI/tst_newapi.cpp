#include <QtTest>

#include "adder.h"
#include <dependencypointer.h>
#include <dependencypool.h>

Adder *createAdder()
{
    return new Adder;
}

class NewAPI : public QObject
{
    Q_OBJECT

public:
    NewAPI();
    ~NewAPI();

private slots:
    void test_case1();
    void test_bc();
};

NewAPI::NewAPI()
{
    QInjection::addSingleton(createAdder);
}

NewAPI::~NewAPI() {}

void NewAPI::test_case1()
{
    QInjection::Pointer<Adder> adder;
    QCOMPARE(adder->add(1, 2), 3);
}

void NewAPI::test_bc()
{
    Dependency::Pointr<Adder> adder;
    QCOMPARE(adder->add(1, 2), 3);
}

QTEST_APPLESS_MAIN(NewAPI)

#include "tst_newapi.moc"
