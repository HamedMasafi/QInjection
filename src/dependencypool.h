#ifndef Pool_H
#define Pool_H

#include <QMap>
#include <QObject>
#include "dependencycreator.h"

#define dep ::Dependency::Pool::instance()
#define di_new(Type, ...) ::Dependency::Pool::instance()->create<Type, __VA_ARGS__>()
#define di_get(type) ::Dependency::Pool::instance()->get<type>()
#define di_add(type) ::Dependency::Pool::instance()->add<type>()
#define CLASS_NAME(T) T::staticMetaObject.className()

namespace Dependency {

class PoolPrivate;
class Pool : public QObject
{
    Q_OBJECT

    PoolPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Pool)

    typedef std::function<QObject *()> CreatorFunc;

    QMap<QString, CreatorFunc> _creators;
    QList<SignalBase *> _signals;

public:
    explicit Pool(QObject *parent = nullptr);

    void add(QObject *object);
    void add(QObject *object, const QString &key);
    QObject *get(const QString &name);
    bool remove(const QString &name, const bool &deleteLater = true);
    bool contains(const QString &key) const;

    template<class T>
    T *add();

    template<class T>
    T *get();

    template<class T>
    T *get(const QString &key);

    template<class T>
    bool remove(const bool &deleteLater = true){
        return remove(CLASS_NAME(T), deleteLater);
    }

    template<class T>
    bool contains() {
        return contains(CLASS_NAME(T));
    }

    template<typename T, class R>
    inline void registerObjectNotify(
        R *reciver,
        void(R::*slot)(T*))
    {
        QString key = CLASS_NAME(T);
        auto s = new SignalPointer<T, R>();
        s->_key = key;
        s->slot = slot;
        s->reciver = reciver;
        _signals.append(s);

        connect(reciver, &QObject::destroyed, [this, s](QObject * = nullptr) {
            _signals.removeOne(s);
        });
    }

    template<typename T, class R>
    inline void registerObjectNotify(
        R *reciver,
        void(R::*slot)())
    {
        QString key = CLASS_NAME(T);
        auto s = new SignalPointerWithoutParam<T, R>();
        s->_key = key;
        s->slot = slot;
        s->reciver = reciver;
        _signals.append(s);

        connect(reciver, &QObject::destroyed, [this, s](QObject * = nullptr) {
            _signals.removeOne(s);
        });
    }

    template<typename T, class R>
    inline void registerObjectNotify(
        R *reciver,
        std::function<void(T*)> cb)
    {
        QString key = CLASS_NAME(T);
        auto s = new SignalPointerFunc<T, R>();
        s->_key = key;
        s->_cb = cb;
        _signals.append(s);

        connect(reciver, &QObject::destroyed, [this, s](QObject * = nullptr) {
            _signals.removeOne(s);
        });
    }

    template<class T>
    void registerCreator(CreatorFunc cb)
    {
        _creators.insert(CLASS_NAME(T), cb);
    }
    template<class T>
    void registerCreator(T *(*cb)())
    {
        _creators.insert(CLASS_NAME(T), cb);
    }
    template<class T>
    void registerCreator()
    {
        _creators.insert(CLASS_NAME(T), []() { return new T; });
    }

#if __cplusplus >= 201402L
    template<class T, typename... _Args>
    void registerCreator(_Args... args)
    {
        _creators.insert(CLASS_NAME(T), [&args...]() {
            return new T(args...);
        });
    }
#endif

    template<class _Type, class... _Args>
    _Type *create()
    {
        auto tmp = _creators.value(CLASS_NAME(_Type))();
        return new _Type(get<_Args>()...);
    }

    static Pool *_instance;
    static Pool *instance();
    static void setInctance(Pool *newInstance, bool removeOld = true);

private:
    int callSlots(QObject *o, bool sendNull = false);
Q_SIGNALS:

};


template <class T>
Q_OUTOFLINE_TEMPLATE T *Pool::add()
{
    auto o = new T;
    add(o);
    return o;
}
template<class T>
Q_OUTOFLINE_TEMPLATE T *Pool::get()
{
    return qobject_cast<T*>(get(CLASS_NAME(T)));
}

template<class T>
Q_OUTOFLINE_TEMPLATE T *Pool::get(const QString &key)
{
    return qobject_cast<T*>(get(key));
}
}

#endif // Pool_H
