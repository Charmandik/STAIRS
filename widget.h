#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QSqlError>
#include <set.h>
#include <QLabel>
#include <QTableWidget>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT


public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    unsigned int setsCount = 0;
    unsigned int rowCount = 0;
    unsigned int OrderRowCount = 0;
    double fullPrice = 0;
    QVector<Set*> setsVector;
    QGridLayout *lay = new QGridLayout();
    QLabel *Text = new QLabel();
    QString setsText;
    QMap<QString,QString> Ledbuffer;
    QTableWidget *tableWidget = new QTableWidget();
    QTableWidget *tb = new QTableWidget();
    QTabWidget *tab = new QTabWidget();
    QGridLayout * lay1 = new QGridLayout();
    QGridLayout * lay2 = new QGridLayout();

    void setItemsToTable(QString, QString, QString, QString, QString, QString);
    void ordersTable(QString a, QString b, QString c, QString d, QString e);
    void toCSV();
public slots:
    void addSet();
    void checkPrice();
    void updateText();
    void sensorConfiguration(Sensor*);
    void ledConfiguration(Led*);
    void ledColorConfiguration(Led*);
    void profileConfiguration(Profile*);
    void powerSupplyConfiguration(powerSupply*);
    void controllerConfiguration(Controller*);
    void deleteSetSlot(Set*);
private slots:
    void on_listOfOrders_clicked();
    void on_IndividualOrderPB_clicked();
    void on_physicalPersonCheckBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_saveToBD_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
