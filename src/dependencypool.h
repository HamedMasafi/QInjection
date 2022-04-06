#ifndef Pool_H
#define Pool_H

#include <QMap>
#include <QObject>
#include "dependencycreator.h"

#define dep ::QInjection
#define di_new(Type, ...) ::QInjection::create<Type, __VA_ARGS__>()
#define di_get(type) ::QInjection::get<type>()
#define di_add(type) ::QInjection::add<type>()
#define CLASS_NAME(T) T::staticMetaObject.className()

namespace QInjection {

namespace Impl {
void addCreator(const QString &key, CreatorBase *creator);
void addSignel(SignalBase *signal);
void removeSignel(SignalBase *signal);
int callSlots(QObject *o, bool sendNull = false);
CreatorType typeForKey(const QString &key);
void deleteObject(QObject *obj);
} // namespace Impl
/*
class PoolPrivate;
class Pool : public QObject
{
    Q_OBJECT

    PoolPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Pool)

public:
    explicit Pool(QObject *parent = nullptr);

    template<class T>
    void addSingleton();
    template<class T>
    void addSingleton(T *object);
    template<class T>
    void addSingleton(T *(*slot)());
    template<class _Owner, class T>
    void addSingleton(_Owner *owner, void (_Owner::*slot)(T *));

    template<class T>
    void addScopped(T *(*slot)());
    template<class _Owner, class T>
    void addScopped(_Owner *owner, void (_Owner::*slot)(T *));

//#if __cplusplus >= 201402L
//    template<class T, typename... _Args>
//    void registerCreator(_Args... args)
//    {
//        _creators.insert(CLASS_NAME(T), [&args...]() {
//            return new T(args...);
//        });
//    }
//#endif

    QObject *create(const QString &key);

    template <class T>
    T *create();

    bool remove(const QString &name);
    template<class T>
    bool remove(){
        return remove(CLASS_NAME(T));
    }

    bool contains(const QString &key) const;
    template<class T>
    bool contains() {
        return contains(CLASS_NAME(T));
    }

    template<typename T, class R>
    inline void registerObjectNotify(R *reciver, void (R::*slot)(T *));

    template<typename T, class R>
    inline void registerObjectNotify(R *reciver, void (R::*slot)());

    template<typename T, class R>
    inline void registerObjectNotify(R *reciver, std::function<void(T *)> cb);

    static Pool *_instance;
    static Pool *instance();
    static void setInctance(Pool *newInstance, bool removeOld = true);

private:
    void addCreator(const QString &key, CreatorBase *creator);
    void addSignel(SignalBase *signal);
    void removeSignel(SignalBase *signal);
    int callSlots(QObject *o, bool sendNull = false);
    CreatorType typeForKey(const QString &key);
Q_SIGNALS:

};*/

QObject *create(const QString &key);
bool remove(const QString &name);
bool contains(const QString &key);

template<class T>
bool remove(){
    return remove(CLASS_NAME(T));
}

template<class T>
Q_OUTOFLINE_TEMPLATE T *create()
{
    return qobject_cast<T*>(create(CLASS_NAME(T)));
}

// Add Objects
template<class T>
Q_OUTOFLINE_TEMPLATE void addSingleton()
{
    auto creator = new OnceCreator<T>(CreatorType::Singelton);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void addSingleton(T *object) {
    auto creator = new SimpleCreator<T>(CreatorType::Singelton, object);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void addSingleton(T*(*slot)() ) {
    auto creator = new FunctionCreator<T>(CreatorType::Singelton, slot);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<class _Owner, class T>
Q_OUTOFLINE_TEMPLATE void addSingleton(_Owner *owner, void (_Owner::*slot)(T *))
{
    auto creator = new ClassFunctionCreator<_Owner, T>(CreatorType::Singelton, owner, slot);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<class T>
Q_OUTOFLINE_TEMPLATE void addScopped(T*(*slot)() ) {
    auto creator = new FunctionCreator<T>(CreatorType::Scopped, slot);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<class _Owner, class T>
Q_OUTOFLINE_TEMPLATE void addScopped(_Owner *owner, void (_Owner::*slot)(T *))
{
    auto creator = new ClassFunctionCreator<_Owner, T>(CreatorType::Scopped, owner, slot);
    Impl::addCreator(CLASS_NAME(T), creator);
}

template<typename T, class R>
Q_OUTOFLINE_TEMPLATE void registerObjectNotify(
    R *reciver,
    void(R::*slot)(T*))
{
    QString key = CLASS_NAME(T);
    auto s = new SignalPointer<T, R>();
    s->_key = key;
    s->slot = slot;
    s->reciver = reciver;
    Impl::addSignel(s);

    QObject::connect(reciver, &QObject::destroyed, [s](QObject * = nullptr) {
        Impl::removeSignel(s);
    });
}

template<typename T, class R>
Q_OUTOFLINE_TEMPLATE void registerObjectNotify(
    R *reciver,
    void(R::*slot)())
{
    QString key = CLASS_NAME(T);
    auto s = new SignalPointerWithoutParam<T, R>();
    s->_key = key;
    s->slot = slot;
    s->reciver = reciver;
    Impl::addSignel(s);

    QObject::connect(reciver, &QObject::destroyed, [s](QObject * = nullptr) {
        Impl::removeSignel(s);
    });
}

template<typename T, class R>
Q_OUTOFLINE_TEMPLATE void registerObjectNotify(
    R *reciver,
    std::function<void(T*)> cb)
{
    QString key = CLASS_NAME(T);
    auto s = new SignalPointerFunc<T, R>();
    s->_key = key;
    s->_cb = cb;
    Impl::addSignel(s);

    QObject::connect(reciver, &QObject::destroyed, [s](QObject * = nullptr) {
        Impl::removeSignel(s);
    });
}

}

#endif // Pool_H
