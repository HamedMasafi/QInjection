#ifndef DEPENDENCYCREATOR_H
#define DEPENDENCYCREATOR_H

#include <QObject>
#include <functional>

namespace Dependency {

struct SignalBase
{
    QString _key;
    void check(const QString &key, QObject *obj)
    {
        if (key == _key)
            call(obj);
    }
    virtual void call(QObject *obj) {
        Q_UNUSED(obj)
    }
};

template<typename T, class R>
struct SignalPointer : SignalBase
{
    R *reciver;
    void (R::*slot)(T *);

    void call(QObject *obj) override {
        if (obj)
            (reciver->*slot)(qobject_cast<T*>(obj));
        else
            (reciver->*slot)(nullptr);
    }
};

template<typename T, class R>
struct SignalPointerWithoutParam : SignalBase {
    R *reciver;
    void (R::*slot)();

    void call(QObject *obj) override
    {
        Q_UNUSED(obj)
        (reciver->*slot)();
    }
};

template<typename T, class R>
struct SignalPointerFunc : SignalBase {
    std::function<void(T *)> _cb;

    void call(QObject *obj) override
    {
        if (obj)
            _cb(qobject_cast<T *>(obj));
        else
            _cb(nullptr);
    }
};

} // namespace Dependency

#endif // DEPENDENCYCREATOR_H
