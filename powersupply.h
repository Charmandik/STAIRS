#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H
#include <QComboBox>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QObject>
#include <QVariant>

class powerSupply:public QWidget
{
    Q_OBJECT
public:
    powerSupply();
    QComboBox *type = new QComboBox();
    QGridLayout *lay = new QGridLayout();
    QPushButton *pb = new QPushButton();
    QVariant price;
    double power = 0;
public slots:
    void deleteThis();
signals:
    void deleteFromVector(powerSupply*);
    void checkPrice();
};

#endif // POWERSUPPLY_H
