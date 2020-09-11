#ifndef TST_MAIN_H
#define TST_MAIN_H

#include <QObject>
#include "dependencypool.h"

class Adder;
class MainTest : public QObject
{
    Q_OBJECT

public:
    MainTest(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void pointer();
    void inject();
    void get();
    void macro_get();


};

#endif // TST_MAIN_H
