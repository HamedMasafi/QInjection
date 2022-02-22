#ifndef DEPENDENCYINJECTOR_H
#define DEPENDENCYINJECTOR_H

#include "dependencypool.h"

namespace QInjection {

class Injecter
{
    const char *_key;

public:
    Injecter();
    Injecter(const char *key);

    Injecter(const Injecter &) = delete;
    Injecter(Injecter &&) = delete;

    template<class T>
    operator T *()
    {
        if (_key)
            return create<T>();
        else
            return qobject_cast<T *>(create(_key));
    }
};

extern Injecter Inject;
} // namespace QInjection

#endif // DEPENDENCYINJECTOR_H
