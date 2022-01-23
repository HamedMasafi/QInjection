#ifndef DEPENDENCYINJECTOR_H
#define DEPENDENCYINJECTOR_H

#include "dependencypool.h"

namespace Dependency {

class Injecter {
    const char* _key;

public:
    Injecter();
    Injecter(const char *key);

    Injecter(const Injecter &) = delete;
    Injecter(Injecter &&) = delete;

    template<class T>
    operator T *()
    {
        if (_key)
            return Pool::instance()->get<T>();
        else
            return Pool::instance()->get<T>(_key);
    }
};

extern Injecter Inject;
}

#endif // DEPENDENCYINJECTOR_H
