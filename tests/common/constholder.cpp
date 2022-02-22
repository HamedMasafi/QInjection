#include "constholder.h"

int ConstHolder::value() const
{
    return _value;
}

void ConstHolder::setValue(int value)
{
    _value = value;
}

ConstHolder::ConstHolder(QObject *parent) : QObject(parent)
{

}

ConstHolder::ConstHolder(int n, QObject *parent) : QObject(parent), _value{n}
{

}
