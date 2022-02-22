#include "adder.h"

Adder::Adder(QObject *parent) : QObject(parent)
{

}

int Adder::add(int n1, int n2) const
{
    return n1 + n2;
}
