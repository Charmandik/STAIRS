#ifndef WIRE_H
#define WIRE_H
#include <QString>
#include <QVariant>
#include <QWidget>

class Wire : public QWidget
{
public:
    Wire();
    QString wireName;
    QString lenght;
    QVariant price = 0;

//signals:
//    void deleteFromVector();
};

#endif // WIRE_H
