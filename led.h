#ifndef LED_H
#define LED_H
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QLineEdit>
#include <QObject>
class Led:public QWidget
{
     Q_OBJECT
public:
    Led();
    ~Led();
    QComboBox *count = new QComboBox;
    QComboBox *color =  new QComboBox();
    QComboBox *type = new QComboBox();
    QLineEdit *lenght = new QLineEdit();
    QGridLayout *lay = new QGridLayout;
    QPushButton *pb = new QPushButton;
    QVariant price = 0;
    double requiredPower = 0;
public slots:
    void deleteThis();
    void powerCalculation();
    void changeTextSlot();
signals:
    void deleteFromVector(Led*);
    void checkPrice();
    void changeTextSignal(Led*);
};

#endif // LED_H
