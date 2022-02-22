#include "lifetimereporter.h"

#include <QDebug>
LifeTimeReporter::LifeTimeReporter(bool *value, QObject *parent)
    : QObject{parent}, _v(value)
{
    *_v = true;
}

LifeTimeReporter::~LifeTimeReporter()
{
    *_v = false;
    qDebug() << "destroyed";
}
