#include "dependencypool.h"
#include "dependencypool_p.h"

#include <QThread>
#include <QDebug>

namespace Dependency {

Injecter Inject;

Pool *Pool::_instance = nullptr;
Pool *PoolPrivate::instance = nullptr;

PoolPrivate::PoolPrivate(Dependency::Pool *parent) : q_ptr(parent)
{}


Pool::Pool(QObject *parent) : QObject(parent), d_ptr(new PoolPrivate(this))
{

}

Pool *Pool::instance()
{
    if (!_instance)
        _instance = new Pool;
    return _instance;
}

void Pool::setInctance(Pool *newInstance, bool removeOld)
{
    if (!newInstance)
        return;

    if (_instance == newInstance)
        return;

    if (removeOld)
        _instance->deleteLater();

    _instance = newInstance;
}

int Pool::callSlots(QObject *o, bool sendNull)
{
    QString key{o->metaObject()->className()};
    int ret{0};

    for (auto &s : _signals)
        if (s->_key == key) {
            s->call(sendNull ? nullptr : o);
            ret++;
        }
    return ret;
}

void Pool::add(QObject *object)
{
    add(object, object->metaObject()->className());
}

void Pool::add(QObject *object, const QString &key)
{
    Q_D(Pool);

    if (d->data.contains(key))
        return;

    QThread *t = qobject_cast<QThread*>(object);
    if (t)
        connect(t, &QThread::finished, [this, key](){
            remove(key);
        });

    connect(object, &QObject::destroyed, [this, key]() {
        remove(key);
    });

    d->data.insert(key, object);

    callSlots(object);
}

QObject *Pool::get(const QString &name)
{
    Q_D(const Pool);

    if (d->data.contains(name)) {
#ifdef QT_DEBUG
        if (!d->data.contains(name))
            qDebug() << "Object for key" << name << "storred as nullptr";
#endif
        return d->data.value(name);
    } else if (_creators.contains(name)) {
        auto tmp = _creators.value(name)();
        add(tmp);
        return tmp;
    }  else {
        return nullptr;
    }
}

bool Pool::remove(const QString &name, const bool &deleteLater)
{
    Q_D(Pool);

    if (!d->data.contains(name))
        return false;

    if (deleteLater)
        d->data.value(name)->deleteLater();
    return d->data.remove(name) > 0;
}

bool Pool::contains(const QString &key) const
{
    Q_D(const Pool);
    return d->data.contains(key);
}

}
