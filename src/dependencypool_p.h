#ifndef POOLDATA_H
#define POOLDATA_H

#include <QMap>

class QObject;

namespace Dependency {

class Pool;
class PoolPrivate
{
    Pool *q_ptr;
    Q_DECLARE_PUBLIC(Pool)

public:
    PoolPrivate(Pool *parent);
    QMap<QString, QObject *> data;
    static Pool *instance;
};

}

#endif // POOLDATA_H
