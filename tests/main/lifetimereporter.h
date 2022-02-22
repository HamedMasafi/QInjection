#ifndef LIFETIMEREPORTER_H
#define LIFETIMEREPORTER_H

#include <QObject>

class LifeTimeReporter : public QObject
{
    Q_OBJECT
    bool *_v;

public:
    explicit LifeTimeReporter(bool *value, QObject *parent = nullptr);
    ~LifeTimeReporter();

signals:
};

#endif // LIFETIMEREPORTER_H
