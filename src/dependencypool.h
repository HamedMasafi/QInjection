#ifndef Pool_H
#define Pool_H

#include <QMap>
#include <QObject>

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

    QList<SignalBase *> _signals;

public:
    explicit Pool(QObject *parent = nullptr);

    void add(QObject *object);
    void add(QObject *object, const QString &key);
    QObject *get(const QString &name) const;
    bool remove(const QString &name, const bool &deleteLater = true);
    bool contains(const QString &key) const;

    template<class T>
    T *add() {
        auto o = new T;
        add(o);
        return o;
    }

    template<class T>
    T *get() const{
        return qobject_cast<T*>(get(CLASS_NAME(T)));
    }

    template<class T>
    T *get(const QString &key) const{
        return qobject_cast<T*>(get(key));
    }

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

    template<class _Type, class... _Args>
    _Type *create()
    {
        return new _Type(get<_Args>()...);
    }
    static Pool *_instance;
    static Pool *instance();
    static void setInctance(Pool *newInstance, bool removeOld = true);

private:
    int callSlots(QObject *o, bool sendNull = false);
signals:

};
class Injecter {
    QString _key;
public:
    Injecter()
    {}
    Injecter(const QString &key) : _key(key)
    {}
    Injecter(const Injecter &) = delete;
    Injecter(Injecter &&) = delete;
    template<class T>
    operator T *()
    {
        if (_key.isEmpty())
            return Pool::instance()->get<T>();
        else
            return Pool::instance()->get<T>(_key);
    }
};
extern Injecter Inject;

}

#endif // Pool_H
