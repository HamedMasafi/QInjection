#ifndef ADDER_H
#define ADDER_H

#include <QObject>

class Adder : public QObject
{
    Q_OBJECT
public:
    explicit Adder(QObject *parent = nullptr);

    int add(int n1, int n2) const;
signals:

};

#endif // ADDER_H
