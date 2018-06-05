#ifndef SET_H
#define SET_H
#include <controller.h>
#include <led.h>
#include <sensor.h>
#include <profile.h>
#include <wire.h>
#include <terminalblock.h>
#include <powersupply.h>
#include <QRadioButton>
#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QFrame>
#include <QLabel>

class Set : public QWidget
{
      Q_OBJECT
public:
    Set();
    ~Set();
    QRadioButton *controllerTypeRB = new QRadioButton();
    QRadioButton *controllerType2RB = new QRadioButton();
    QRadioButton *controllerColorRB = new QRadioButton();
    QRadioButton *controllerColor2RB = new QRadioButton();
    QRadioButton *lightTypeRB = new QRadioButton();
    QRadioButton *lightType2RB = new QRadioButton();
    QPushButton *addSensorPB = new QPushButton();
    QPushButton *addLedPB = new QPushButton();
    QPushButton *addProfilePB = new QPushButton();
    QPushButton *addPowerSupplyPB = new QPushButton();
    QPushButton *addController = new QPushButton();
    QPushButton *deleteButton = new QPushButton();
    QPushButton *addSet = new QPushButton();
    QGridLayout *lay = new QGridLayout();
    QGridLayout *sensorsLayout = new QGridLayout();
    QGridLayout *ledsLayout = new QGridLayout();
    QGridLayout *profilesLayout = new QGridLayout();
    QGridLayout *powerSuppliesLayout = new QGridLayout();
    QGridLayout *controllersLayout = new QGridLayout();
    QGridLayout *radioButtonsLayout = new QGridLayout();
    QLabel *controllerLabel = new QLabel();
    QLabel *sensorLabel = new QLabel();
    QLabel *ledLabel = new QLabel();
    QLabel *profileLabel = new QLabel();
    QLabel *powerSupplyLabel = new QLabel();
    QVector<Controller*> controllersVector;
    QVector<Sensor*> sensorsVector;
    QVector<Led*> ledsVector;
    QVector<Profile*> profilesVector;
    QVector<powerSupply*> powerSuppliesVector;
    QVector<Wire*> wiresVector;
    QVector<TerminalBlock*> terminalBlocksVector;
    QString outputText();
    bool RGB = true;
    bool baseSet = true;

    unsigned int sensorsCount = 2;
    unsigned int ledsCount = 2;
    unsigned int profilesCount = 2;
    unsigned int powerSuppliesCount = 2;
    unsigned int controllersCount = 2;
    double price = 0;

    double premiumPrice();
    double ecoPrice();
    int stepsCount();

public slots:
    void addSensorSlot();
    void addLedSlot();
    void addProfileSlot();
    void addPowerSupplySlot();
    void addControllerSlot();
    void deleteButtonSlot();
    void deleteSensor(Sensor*);
    void deleteLed(Led*);
    void deleteProfile(Profile*);
    void deletePowerSupply(powerSupply*);
    void deleteWire(Wire*);
    void deleteController(Controller*);
    void fullSetSlot();
    void baseSetSlot();
    void RGBsetSlot();
    void nonRGBsetSlot();
    void updateInfo();
    void requiredPower();
    void itemsConfiguration();
    void sensorCountConfiguration();
signals:
    void addSensorSignal(Sensor*);
    void addLedSignal(Led*);
    void addProfileSignal(Profile*);
    void addPowerSupplySignal(powerSupply*);
    void addControllerSignal(Controller*);
    void deleteButtonSignal(Set*);
    void updateText();

};

#endif // SET_H
