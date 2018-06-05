#ifndef SENSOR_H
#define SENSOR_H

#include <QComboBox>
#include "QGridLayout"
#include <QPushButton>
#include <Qwidget>
#include <QObject>
class Sensor:public QWidget
{
     Q_OBJECT
public:
    Sensor();
    ~Sensor();
    QComboBox *count = new QComboBox();
    QComboBox *color = new QComboBox();
    QComboBox *type = new QComboBox();
    QGridLayout *lay = new QGridLayout();
    QPushButton *pb = new QPushButton();
    QVariant price = 0;
public slots:
    void deleteThis();
signals:
    void deleteFromVector(Sensor*);
    void checkPrice();
};

#endif // SENSOR_H
