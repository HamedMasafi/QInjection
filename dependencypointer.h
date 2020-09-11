#ifndef DEPENDENCYPOINTER_H
#define DEPENDENCYPOINTER_H

#include "dependencyinjection.h"

#include <QVariant>
#include <QObject>

namespace Dependency {

template<class T>
class Pointer : public QObject{
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value, "QPointer's template type must not be a pointer type");

    T *_data;

public:
    Pointer() : _data(Pool::instance()->get<T>())
    {
        Dependency::Pool::instance()->registerObjectNotify<T>(this, [this](T *t) {
            _data = t;
        });
    }

    inline T *data() const
    {
        return _data;
    }
    inline T *operator->() const
    {
        return data();
    }
    inline T &operator*() const
    {
        return *data();
    }
    inline operator T *() const
    {
        return data();
    }

    inline bool isNull() const
    {
        return _data == nullptr;
    }

    inline void clear()
    {
        _data = nullptr;
    }
};

}

template<class T>
Q_DECLARE_TYPEINFO_BODY(Dependency::Pointer<T>, Q_MOVABLE_TYPE);

template<class T>
inline bool operator==(const T *o, const Dependency::Pointer<T> &p)
{
    return o == p.operator->();
}

template<class T>
inline bool operator==(const Dependency::Pointer<T> &p, const T *o)
{
    return p.operator->() == o;
}

template<class T>
inline bool operator==(T *o, const Dependency::Pointer<T> &p)
{
    return o == p.operator->();
}

template<class T>
inline bool operator==(const Dependency::Pointer<T> &p, T *o)
{
    return p.operator->() == o;
}

template<class T>
inline bool operator==(const Dependency::Pointer<T> &p1,
                       const Dependency::Pointer<T> &p2)
{
    return p1.operator->() == p2.operator->();
}

template<class T>
inline bool operator!=(const T *o, const Dependency::Pointer<T> &p)
{
    return o != p.operator->();
}

template<class T>
inline bool operator!=(const Dependency::Pointer<T> &p, const T *o)
{
    return p.operator->() != o;
}

template<class T>
inline bool operator!=(T *o, const Dependency::Pointer<T> &p)
{
    return o != p.operator->();
}

template<class T>
inline bool operator!=(const Dependency::Pointer<T> &p, T *o)
{
    return p.operator->() != o;
}

template<class T>
inline bool operator!=(const Dependency::Pointer<T> &p1,
                       const Dependency::Pointer<T> &p2)
{
    return p1.operator->() != p2.operator->();
}

#endif // DEPENDENCYPOINTER_H
