#ifndef DEPENDENCYPOINTER_H
#define DEPENDENCYPOINTER_H

#include "dependencyinjection.h"

#include <QVariant>
#include <QObject>

template<class T>
class DependencyPointer : public QObject{
    Q_STATIC_ASSERT_X(
        __is_base_of(QObject, T),
        "DependencyPointer's template type must not be derived from QObject");

    T *_data;

public:
    DependencyPointer() : _data(DependencyInjection::instance()->get<T>())
    {
        DependencyInjection::instance()->registerObjectNotify<T>(this, [this](T *t) {
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



template<class T>
Q_DECLARE_TYPEINFO_BODY(DependencyPointer<T>, Q_MOVABLE_TYPE);

template<class T>
inline bool operator==(const T *o, const DependencyPointer<T> &p)
{
    return o == p.operator->();
}

template<class T>
inline bool operator==(const DependencyPointer<T> &p, const T *o)
{
    return p.operator->() == o;
}

template<class T>
inline bool operator==(T *o, const DependencyPointer<T> &p)
{
    return o == p.operator->();
}

template<class T>
inline bool operator==(const DependencyPointer<T> &p, T *o)
{
    return p.operator->() == o;
}

template<class T>
inline bool operator==(const DependencyPointer<T> &p1,
                       const DependencyPointer<T> &p2)
{
    return p1.operator->() == p2.operator->();
}

template<class T>
inline bool operator!=(const T *o, const DependencyPointer<T> &p)
{
    return o != p.operator->();
}

template<class T>
inline bool operator!=(const DependencyPointer<T> &p, const T *o)
{
    return p.operator->() != o;
}

template<class T>
inline bool operator!=(T *o, const DependencyPointer<T> &p)
{
    return o != p.operator->();
}

template<class T>
inline bool operator!=(const DependencyPointer<T> &p, T *o)
{
    return p.operator->() != o;
}

template<class T>
inline bool operator!=(const DependencyPointer<T> &p1,
                       const DependencyPointer<T> &p2)
{
    return p1.operator->() != p2.operator->();
}

template<class T>
inline void swap(DependencyPointer<T> &p1, DependencyPointer<T> &p2) noexcept
{
    p1.swap(p2);
}

#endif // DEPENDENCYPOINTER_H
