#ifndef DEPENDENCYINJECTION_H
#define DEPENDENCYINJECTION_H

#include <QMap>
#include <QObject>
//#include <qobjectdefs_impl.h>

#define dep DependencyInjection::instance()
#define di_new(Type, ...) DependencyInjection::instance()->create<Type, __VA_ARGS__>()
#define CLASS_NAME(T) T::staticMetaObject.className()

class DependencyInjection : public QObject
{
    Q_OBJECT

    QMap<QString, QObject *> _data;

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
    explicit DependencyInjection(QObject *parent = nullptr);

    void add(QObject *object);
    void add(QObject *object, const QString &key);
    QObject *get(const QString &name) const;
    bool remove(const QString &name, const bool &deleteLater = true);

    template<class T>
    T *add() {
        auto o = new T;
        add(o);
        return o;
    }

    template<class T>
    T *get() const{
        return qobject_cast<T*>(get(T::staticMetaObject.className()));
    }

    template<class T>
    T *get(const QString &key) const{
        return qobject_cast<T*>(get(key));
    }

    template<class T>
    bool remove(const bool &deleteLater = true){
        return remove(T::staticMetaObject.className(), deleteLater);
    }

    template<class T>
    bool contains() {
        return _data.contains(T::staticMetaObject.className());
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
    static DependencyInjection *instance();

private:
    int callSlots(QObject *o, bool sendNull = false);
signals:

};

#endif // DEPENDENCYINJECTION_H
