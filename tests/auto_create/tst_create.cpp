#include <QtTest>

#include "constholder.h"
#include <dependencypool.h>
#include <dependencypointer.h>

constexpr int _value = 123;

class tst_create : public QObject
{
    Q_OBJECT

public:
    tst_create();
    ~tst_create();

private slots:
    void test_inject();
    void test_create_on_param(ConstHolder *holder = Dependency::Inject);
    void test_pointer();
    void test_creatot_params();
};

ConstHolder *createConstHolder()
{
    auto c = new ConstHolder;
    c->setValue(_value);
    return c;
}
tst_create::tst_create()
{
    dep->registerCreator(createConstHolder);
}

tst_create::~tst_create()
{

}

void tst_create::test_inject()
{
    ConstHolder *holder = Dependency::Inject;
    QCOMPARE(holder->value(), _value);
}

void tst_create::test_create_on_param(ConstHolder *holder)
{
    QCOMPARE(holder->value(), _value);
}

void tst_create::test_pointer()
{
    Dependency::Pointer<ConstHolder> holder;
    QCOMPARE(holder->value(), _value);
}

void tst_create::test_creatot_params()
{
    constexpr int n = 456;
    dep->remove<ConstHolder>();
    dep->registerCreator<ConstHolder>(n);
    ConstHolder *holder = Dependency::Inject;
    QCOMPARE(holder->value(), n);
}

QTEST_APPLESS_MAIN(tst_create)

#include "tst_create.moc"
