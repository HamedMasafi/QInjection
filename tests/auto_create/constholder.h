#ifndef CONSTHOLDER_H
#define CONSTHOLDER_H

#include <QtCore/QObject>

class ConstHolder : public QObject
{
    Q_OBJECT
    int _value;

public:
    explicit ConstHolder(QObject *parent = nullptr);

    int value() const;
    void setValue(int value);

signals:

};

#endif // CONSTHOLDER_H
