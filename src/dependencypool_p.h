#ifndef POOLDATA_H
#define POOLDATA_H

#include <QMap>

class QObject;

namespace Dependency {

class CreatorBase;
class Pool;
class PoolPrivate
{
    Pool *q_ptr;
    Q_DECLARE_PUBLIC(Pool)

public:
    PoolPrivate(Pool *parent);

    QList<CreatorBase*> _creators;
    QMap<QString, QObject *> data;
    QMap<QString, CreatorBase *> creators;

    static Pool *instance;
};

}

#endif // POOLDATA_H
