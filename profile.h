#ifndef PROFILE_H
#define PROFILE_H
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QObject>

class Profile:public QWidget
{
     Q_OBJECT
public:
    Profile();
    ~Profile();
    QComboBox *count = new QComboBox();
    QComboBox *type = new QComboBox();
    QLineEdit *lenght = new QLineEdit;
    QGridLayout *lay = new QGridLayout();
    QPushButton *pb = new QPushButton();
    QVariant price = 0;
public slots:
    void deleteThis();
signals:
    void deleteFromVector(Profile*);
    void checkPrice();
};

#endif // PROFILE_H
