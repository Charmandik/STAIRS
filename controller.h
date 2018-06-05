#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QString>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QObject>

class Controller:public QWidget
{
    Q_OBJECT
public:
    Controller();
    ~Controller();
    double price = 0;
    QComboBox *type = new QComboBox();
    QCheckBox *expansionModule = new QCheckBox();
    QLineEdit *steps = new QLineEdit();
    QGridLayout *lay = new QGridLayout();
    QPushButton *deleteButton = new QPushButton();
signals:
    void deleteFromVector(Controller*);
    void checkPrice();

public slots:
    void deleteThis();
};

#endif // CONTROLLER_H
