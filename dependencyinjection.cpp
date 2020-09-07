#include "dependencyinjection.h"

#include <QThread>
#include <QDebug>

DependencyInjection::DependencyInjection(QObject *parent) : QObject(parent)
{

}

DependencyInjection *DependencyInjection::instance()
{
    static DependencyInjection *instance = nullptr;
    if (!instance)
        instance = new DependencyInjection;
    return instance;
}

int DependencyInjection::callSlots(QObject *o, bool sendNull)
{
    QString key{o->metaObject()->className()};
    int ret{0};

    for (auto s : _signals)
        if (s->_key == key) {
            s->call(sendNull ? nullptr : o);
            ret++;
        }
    return ret;
}

void DependencyInjection::add(QObject *object)
{
    add(object, object->metaObject()->className());
}

void DependencyInjection::add(QObject *object, const QString &key)
{
    if (_data.contains(key))
        return;

    QThread *t = qobject_cast<QThread*>(object);
    if (t)
        connect(t, &QThread::finished, [this, key](){
            remove(key);
        });

    connect(object, &QObject::destroyed, [this, key]() {
        remove(key);
    });

    _data.insert(key, object);

    callSlots(object);
}

QObject *DependencyInjection::get(const QString &name) const
{
    if (_data.contains(name))
        return _data.value(name);
    else
        return nullptr;
}

bool DependencyInjection::remove(const QString &name, const bool &deleteLater)
{
    if (!_data.contains(name))
        return false;

    if (deleteLater)
        _data.value(name)->deleteLater();
    return _data.remove(name) > 0;
}
