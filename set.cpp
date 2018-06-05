#include "set.h"

Set::Set()
{
    deleteButton->setText("Удалить");
    controllerTypeRB->setText("Базовый");
    controllerType2RB->setText("Полный");
    controllerColorRB->setText("RGB");
    controllerColor2RB->setText("Одноцветный");
    lightTypeRB->setText("Ленты");
    lightType2RB->setText("Светильники");
    addSensorPB->setText("Добавить Датчик");
    addLedPB->setText("Добавить Ленты");
    addProfilePB->setText("Добавить Профили");
    addPowerSupplyPB->setText("Добавить БП");
    addSet->setText("Добавить комплект");
    addController->setText("Добавить контроллер");
    controllerLabel->setText("Контроллеры");
    sensorLabel->setText("Датчики");
    ledLabel->setText("Ленты");
    profileLabel->setText("Профили");
    powerSupplyLabel->setText("Блоки питания");

    deleteButton->setMaximumWidth(150);
    addSensorPB->setMaximumWidth(150);
    addLedPB->setMaximumWidth(150);
    addProfilePB->setMaximumWidth(150);
    addPowerSupplyPB->setMaximumWidth(150);
    addSet->setMaximumWidth(150);
    addController->setMaximumWidth(150);
    controllerTypeRB->setMaximumWidth(150);
    controllerType2RB->setMaximumWidth(150);
    controllerColorRB->setMaximumWidth(150);
    controllerColor2RB->setMaximumWidth(150);

    controllersLayout->addWidget(controllerLabel,0,0,Qt::AlignHCenter);
    controllersLayout->addWidget(addController,1,0,Qt::AlignCenter);
    sensorsLayout->addWidget(sensorLabel,0,0,Qt::AlignHCenter);
    sensorsLayout->addWidget(addSensorPB,1,0,Qt::AlignCenter);
    profilesLayout->addWidget(profileLabel,0,0,Qt::AlignHCenter);
    profilesLayout->addWidget(addProfilePB,1,0,Qt::AlignCenter);
    ledsLayout->addWidget(ledLabel,0,0,Qt::AlignHCenter);
    ledsLayout->addWidget(addLedPB,1,0,Qt::AlignCenter);
    powerSuppliesLayout->addWidget(powerSupplyLabel,0,0,Qt::AlignHCenter);
    powerSuppliesLayout->addWidget(addPowerSupplyPB,1,0,Qt::AlignCenter);
    radioButtonsLayout->addWidget(controllerTypeRB,0,0);
    radioButtonsLayout->addWidget(controllerType2RB,1,0);
    radioButtonsLayout->addWidget(controllerColorRB,2,0);
    radioButtonsLayout->addWidget(controllerColor2RB,3,0);

    lay->addLayout(radioButtonsLayout,0,0);
    QLabel *t = new QLabel();

    QLabel *t1 = new QLabel();

    QLabel *t2 = new QLabel();

    lay->addWidget(t,1,0);
    lay->addWidget(t1,2,0);
    lay->addWidget(t2,3,0);


    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Panel | QFrame::Plain);
    frame->setLineWidth(1);
    frame->setLayout(controllersLayout);
    lay->addWidget(frame,0,1);

    QFrame *frame1 = new QFrame();
    frame1->setFrameStyle(QFrame::Panel | QFrame::Plain);
    frame1->setLineWidth(1);
    frame1->setLayout(sensorsLayout);
    lay->addWidget(frame1,0,2);

    QFrame *frame2 = new QFrame();
    frame2->setFrameStyle(QFrame::Panel | QFrame::Plain);
    frame2->setLineWidth(1);
    frame2->setLayout(ledsLayout);
    lay->addWidget(frame2,1,1);

    QFrame *frame3 = new QFrame();
    frame3->setFrameStyle(QFrame::Panel | QFrame::Plain);
    frame3->setLineWidth(1);
    frame3->setLayout(profilesLayout);
    lay->addWidget(frame3,1,2);

    QFrame *frame4 = new QFrame();
    frame4->setFrameStyle(QFrame::Panel | QFrame::Plain);
    frame4->setLineWidth(1);
    frame4->setLayout(powerSuppliesLayout);
    lay->addWidget(frame4,2,1,1,2);

    controllerTypeRB->setChecked(true);
    controllerColorRB->setChecked(true);
    controllerTypeRB->setAutoExclusive(false);
    controllerType2RB->setAutoExclusive(false);
    controllerColorRB->setAutoExclusive(false);
    controllerColor2RB->setAutoExclusive(false);
    connect(addSensorPB,&QPushButton::clicked,this,&Set::addSensorSlot);
    connect(addLedPB,&QPushButton::clicked,this,&Set::addLedSlot);
    connect(addProfilePB,&QPushButton::clicked,this,&Set::addProfileSlot);
    connect(addPowerSupplyPB,&QPushButton::clicked,this,&Set::addPowerSupplySlot);
    connect(addController,&QPushButton::clicked,this,&Set::addControllerSlot);
    connect(deleteButton,&QPushButton::clicked,this,&Set::deleteButtonSlot);
    connect(controllerTypeRB,&QRadioButton::clicked,controllerType2RB,&QRadioButton::toggle);
    //connect(controllerTypeRB,&QRadioButton::clicked,controllerTypeRB,&QRadioButton::toggle);
    connect(controllerType2RB,&QRadioButton::clicked,controllerTypeRB,&QRadioButton::toggle);
    // connect(controllerType2RB,&QRadioButton::clicked,controllerType2RB,&QRadioButton::toggle);

    connect(controllerColorRB,&QRadioButton::clicked,controllerColor2RB,&QRadioButton::toggle);
    // connect(controllerColorRB,&QRadioButton::clicked,controllerColorRB,&QRadioButton::toggle);
    connect(controllerColor2RB,&QRadioButton::clicked,controllerColorRB,&QRadioButton::toggle);
    //connect(controllerColor2RB,&QRadioButton::clicked,controllerColor2RB,&QRadioButton::toggle);

    connect(controllerTypeRB,&QRadioButton::toggled,this,&Set::baseSetSlot);
    connect(controllerColor2RB,&QRadioButton::toggled,this,&Set::nonRGBsetSlot);

}

void Set::addSensorSlot()
{
    if(!controllersVector.isEmpty())
    {
    auto *a = new Sensor();
    sensorsVector.append(a);
    sensorsLayout->addLayout(a->lay,sensorsCount,0);
    sensorsCount++;
    connect(a,&Sensor::deleteFromVector,this,&Set::deleteSensor);
    connect(a->count,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->type,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->color,&QComboBox::currentTextChanged,this,&Set::updateText);
    emit addSensorSignal(a);
    }
    else
    {
        QLabel *error = new QLabel("Добавьте контроллер");
        error->setMinimumHeight(75);
        error->show();
    }
}

void Set::addLedSlot()
{
    if(!controllersVector.isEmpty())
    {
    auto *a = new Led();
    ledsVector.append(a);
    ledsLayout->addLayout(a->lay,ledsCount,0);
    ledsCount++;
    connect(a,&Led::deleteFromVector,this,&Set::deleteLed);
    connect(a->count,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->type,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->color,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->lenght,&QLineEdit::textChanged,this,&Set::updateText);
    ledsLayout->addWidget(ledLabel,0,0,Qt::AlignHCenter);
    ledsLayout->addWidget(addLedPB,1,0,Qt::AlignHCenter);
    for(uint i =1; i<=stepsCount(); i++)
        a->count->addItem(QString::number(i));
    emit addLedSignal(a);
    }
    else
    {
        QLabel *error = new QLabel("Добавьте контроллер");
        error->setMinimumHeight(75);
        error->show();
    }
}

void Set::addProfileSlot()
{
    if(!controllersVector.isEmpty())
    {
    auto *a = new Profile();
    profilesVector.append(a);
    profilesLayout->addLayout(a->lay,profilesCount,0);
    profilesCount++;
    connect(a,&Profile::deleteFromVector,this,&Set::deleteProfile);
    connect(a->count,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->type,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->lenght,&QLineEdit::textChanged,this,&Set::updateText);
    for(uint i =1; i<=stepsCount(); i++)
        a->count->addItem(QString::number(i));
    emit addProfileSignal(a);
    }
    else
    {
        QLabel *error = new QLabel("Добавьте контроллер");
        error->setMinimumHeight(75);
        error->show();
    }
}

void Set::addPowerSupplySlot()
{
    if(!controllersVector.isEmpty())
    {
    auto *a = new powerSupply();
    powerSuppliesVector.append(a);
    powerSuppliesLayout->addLayout(a->lay,powerSuppliesCount,0);
    powerSuppliesCount++;
    connect(a,&powerSupply::deleteFromVector,this,&Set::deletePowerSupply);
    connect(a->type,&QComboBox::currentTextChanged,this,&Set::updateText);
    emit addPowerSupplySignal(a);
    }
    else
    {
        QLabel *error = new QLabel("Добавьте контроллер");
        error->setMinimumHeight(75);
        error->show();
    }
}

void Set::addControllerSlot()
{
    auto *a = new Controller();
    controllersVector.append(a);
    controllersLayout->addLayout(a->lay,controllersCount,0);
    controllersCount++;
    emit sensorCountConfiguration();
    emit addControllerSignal(a);
    connect(a,&Controller::deleteFromVector,this,&Set::deleteController);
    connect(a->type,&QComboBox::currentTextChanged,this,&Set::updateText);
    connect(a->steps,&QLineEdit::textChanged,this,&Set::updateText);
    connect(a->expansionModule,&QCheckBox::toggled,this,&Set::updateText);
    connect(a->steps,&QLineEdit::textChanged,this,&Set::updateInfo);
}

void Set::deleteButtonSlot()
{
    qDebug()<<"delete";
    controllerTypeRB->deleteLater();
    controllerType2RB->deleteLater();
    controllerColorRB->deleteLater();
    controllerColor2RB->deleteLater();
    lightTypeRB->deleteLater();
    lightType2RB->deleteLater();
    addSensorPB->deleteLater();
    addLedPB->deleteLater();
    addProfilePB->deleteLater();
    addPowerSupplyPB->deleteLater();
    addController->deleteLater();
    deleteButton->deleteLater();
    addSet->deleteLater();
    lay->deleteLater();
    sensorsLayout->deleteLater();
    ledsLayout->deleteLater();
    profilesLayout->deleteLater();
    powerSuppliesLayout->deleteLater();
    emit deleteButtonSignal(this);

    foreach (Sensor *a, sensorsVector)
    {
        emit a->deleteThis();
        sensorsVector.removeOne(a);
    }

    foreach (Led *a, ledsVector)
    {
        emit a->deleteThis();
        ledsVector.removeOne(a);
    }

    foreach (Profile *a, profilesVector)
    {
        emit a->deleteThis();
        profilesVector.removeOne(a);
    }

    foreach (Controller* a, controllersVector)
    {
        a->type->deleteLater();
        controllersVector.removeOne(a);
    }

}

Set::~Set()
{

}

QString Set::outputText()
{
    price = 0;
    QString output;
    foreach (Controller *a, controllersVector)
    {
        output.append("\n" + a->type->currentText() + " Цена - " + QString::number(a->price));
        if(a->expansionModule->isChecked())
        {
            if((a->steps->text().toInt()>18)&&(a->steps->text().toInt()<=27))
            {
                output.append("\nМодуль расширения для лестниц от 19 до 27 ступеней \nЦена - 2900");
                price +=2900;
            }
            else if(a->steps->text().toInt()>27)
            {
                output.append("\nМодуль расширения для лестниц от 27 до 36 ступеней \nЦена - 3600");
                price += 3600;
            }
        }
        price += a->price;
    }

    foreach (Sensor *a, sensorsVector)
    {
        output.append("\n\nДатчик " + a->type->currentText()+ " " +a->color->currentText() + " Цена - " + a->price.toString()
                      + "\nИтого - " + QString::number(a->count->currentText().toInt()*a->price.toInt()));
        price += a->count->currentText().toInt()*a->price.toInt();
    }

    foreach (Led *a, ledsVector)
    {
        output.append("\n\nЛента " + a->type->currentText()+ " " +a->color->currentText()+ " " +a->lenght->text() + "м " + a->count->currentText() + "шт" + " Цена - "
                      + QString::number(a->price.toDouble())
                      + "\nИтого - " + QString::number(a->count->currentText().toInt()*a->price.toDouble()));
        price += a->count->currentText().toInt()*a->price.toDouble();
    }

    foreach (Profile *a, profilesVector)
    {
        output.append("\n\nПрофиль " + a->type->currentText() + " " +a->lenght->text() + "м " + a->count->currentText() + "шт" + " Цена - " + a->price.toString()
                      + "\nИтого - " + QString::number(a->count->currentText().toInt()*a->price.toDouble()));
        price += a->count->currentText().toInt()*a->price.toDouble();
    }

    foreach (powerSupply *a, powerSuppliesVector)
    {
        output.append("\n\nБлок питания " + a->type->currentText() + " Цена - " + a->price.toString()
                      + "\nИтого - " + QString::number(a->price.toInt()));
        price += a->price.toInt();
    }

    foreach (Wire *a, wiresVector)
    {
        output.append("\n\nПровод " + a->wireName + " " + a->lenght + "м " + " Цена - " + a->price.toString()
                      + "\nИтого - " + QString::number(a->lenght.toInt()*a->price.toInt()));
        price += a->lenght.toInt()*a->price.toInt();
    }

    foreach (TerminalBlock * a, terminalBlocksVector)
    {
        output.append("\n\nКлеммник " + a->terminalBlockName + " " + QString::number(a->count) + "шт " + "Цена - " + QString::number(a->price)
                      + "\nИтого - " + QString::number(a->count*a->price));
        price += a->count*a->price;
    }
    output.append("\n\nИтоговая сумма - " + QString::number(price));
    output.append("\n\nС премиум лентами - " + QString::number(premiumPrice()));
    output.append("\n\nС эконом лентами - " + QString::number(ecoPrice()));
    return output;
}

void Set::deleteSensor(Sensor *a)
{
    sensorsVector.removeOne(a);
    emit updateText();
}

void Set::deleteLed(Led *a)
{
    ledsVector.removeOne(a);
    emit updateText();
}

void Set::deleteProfile(Profile *a)
{
    profilesVector.removeOne(a);
    emit updateText();
}

void Set::deletePowerSupply(powerSupply *a)
{
    powerSuppliesVector.removeOne(a);
    emit updateText();
}

void Set::deleteWire(Wire *a)
{

}

void Set::deleteController(Controller *a)
{
    qDebug()<<"delteCo";
    controllersVector.removeOne(a);
    emit sensorCountConfiguration();
    emit updateText();
}

void Set::fullSetSlot()
{

}

void Set::baseSetSlot()
{
    baseSet = !baseSet;
    if(baseSet)
    {
        qDebug()<<"Base Set";
        if(!wiresVector.isEmpty())
            foreach (Wire *a, wiresVector)
                wiresVector.removeOne(a);

        if(!terminalBlocksVector.isEmpty())
            foreach (TerminalBlock *a, terminalBlocksVector)
                terminalBlocksVector.removeOne(a);

        if(!powerSuppliesVector.isEmpty())
            foreach (powerSupply *a, powerSuppliesVector)
                emit a->pb->click();

        if(!controllersVector.isEmpty())
            foreach (Controller *a, controllersVector)
                a->deleteButton->click();

        if(!ledsVector.isEmpty())
            foreach (Led *a, ledsVector)
                a->pb->click();

        if(!profilesVector.isEmpty())
            foreach (Profile *a, profilesVector)
                a->pb->click();

        if(!sensorsVector.isEmpty())
            foreach (Sensor *a, sensorsVector)
                a->pb->click();
        emit addController->click();
        emit addSensorPB->click();
        if(!sensorsVector.isEmpty())
            foreach (Sensor *a, sensorsVector)
                a->count->setCurrentIndex(1);
    }
    else
    {
        qDebug()<<"FullSet Set";
        itemsConfiguration();
    }
    emit updateText();
}

void Set::RGBsetSlot()
{

}

void Set::nonRGBsetSlot()
{
    RGB = !RGB;
    if(RGB)
    {
        foreach (Controller *a, controllersVector)
            a->type->setCurrentIndex(1);
        foreach (Led *a, ledsVector)
            a->color->setCurrentText("RGB");
    }
    else
    {
        foreach (Controller *a, controllersVector)
            a->type->setCurrentIndex(0);
        foreach (Led *a, ledsVector)
            a->color->setCurrentText("Теплая");
    }
    emit updateText();
}

void Set::updateInfo()
{
    if(!baseSet)
    {
        if(!RGB)
        {
            foreach (Wire *a, wiresVector)
            {
                if(a->wireName == "ШВПМ 2х0.2")
                {
                    if((stepsCount()%5 == 0)&&(stepsCount() >= 5)) //округление
                    {
                        a->lenght = QString::number(stepsCount()*6);
                    }
                    else
                    {
                        a->lenght = QString::number(stepsCount()*6);
                        for(int i = 0;i<15;i++)
                        {
                            if(a->lenght.toInt()%5 == 0)
                            {
                                continue;
                            }
                            else
                                a->lenght = QString::number(a->lenght.toInt() + 1);
                        }
                    }
                }
            }
            foreach (TerminalBlock *a, terminalBlocksVector)
            {
                if(a->terminalBlockName == "WAGO 2 pin")
                    a->count = stepsCount()*2;
                else if(a->terminalBlockName == "WAGO 5 pin")
                {
                    if(stepsCount()%3 == 0)
                        a->count = stepsCount()/3;
                    else if((stepsCount()+1)%3 == 0)
                        a->count =  (stepsCount()+1)/3;
                    else if((stepsCount()-1)%3 == 0)
                        a->count = (stepsCount()-1)/3;
                }
            }
        }
        else
        {
            foreach (Wire *a, wiresVector)
            {
                if(a->wireName == "ПГВА 1 мм2 красный")
                    a->lenght = QString::number(stepsCount() + 5);
                else if(a->wireName == "ПГВА 1 мм2 синий")
                    a->lenght = QString::number(stepsCount() + 5);
                else if(a->wireName == "ПГВА 1 мм2 зеленый")
                    a->lenght = QString::number(stepsCount() + 5);
                else if(a->wireName == "ПГВА 0.2 мм2")
                    a->lenght = QString::number(stepsCount()*6);
            }

            foreach (TerminalBlock *a, terminalBlocksVector)
            {
                if(a->terminalBlockName == "WAGO 2 pin")
                    a->count = stepsCount();
                else if(a->terminalBlockName == "Ответвитель")
                    a->count = stepsCount() * 3;
            }
        }
    }
    emit updateText();
}

void Set::requiredPower()
{
    double powerNeeded = 0;
    if(!ledsVector.isEmpty())
        foreach (Led *a, ledsVector)
        {
            powerNeeded+=(a->requiredPower * a->count->currentText().toInt());
        }
    qDebug()<<powerNeeded;
}

void Set::itemsConfiguration()
{
    if(!baseSet)
        if(RGB)
        {
            //WiresConfiguration BEGIN
            if(!wiresVector.isEmpty())
                foreach (Wire *a, wiresVector)
                {
                    wiresVector.removeOne(a);
                }
            if(powerSuppliesVector.isEmpty())
                emit addPowerSupplyPB->click();

            auto *wire1 = new Wire();
            wire1->wireName = "ШВПМ 2х1.5";
            wire1->lenght = "1";
            auto *wire2 = new Wire();
            wire2->wireName = "ШВПМ 2х2";
            wire2->lenght = "1";
            auto *wire3 = new Wire();
            wire3->wireName = "ПГВА 1 мм2 красный";
            wire3->lenght = QString::number(stepsCount() + 5);
            auto *wire4 = new Wire();
            wire4->wireName = "ПГВА 1 мм2 синий";
            wire4->lenght = QString::number(stepsCount() + 5);
            auto *wire5 = new Wire();
            wire5->wireName = "ПГВА 1 мм2 зеленый";
            wire5->lenght = QString::number(stepsCount() + 5);
            auto *wire6 = new Wire();
            wire6->wireName = "ПГВА 0.2 мм2";
            wire6->lenght = QString::number(stepsCount()*6);
            auto *wire7 = new Wire();
            wire7->wireName = "ШВВП 2х0.7";
            wire7->lenght = "10";

            wiresVector.append(wire1);
            wiresVector.append(wire2);
            wiresVector.append(wire3);
            wiresVector.append(wire4);
            wiresVector.append(wire5);
            wiresVector.append(wire6);
            wiresVector.append(wire7);
            //WiresConfiguration END

            //TerminalBlocksConfiguration BEGIN
            if(!terminalBlocksVector.isEmpty())
                foreach (TerminalBlock *a, terminalBlocksVector)
                {
                    terminalBlocksVector.removeOne(a);
                }
            auto *termBlock1 = new TerminalBlock();
            termBlock1->terminalBlockName = "WAGO 2 pin";
            termBlock1->count = stepsCount();
            auto *termBlock2 = new TerminalBlock();
            termBlock2->terminalBlockName = "WAGO 5 pin";
            termBlock2->count = 3;
            auto *termBlock3 = new TerminalBlock();
            termBlock3->terminalBlockName = "Ответвитель";
            termBlock3->count = stepsCount()*3;

            terminalBlocksVector.append(termBlock1);
            terminalBlocksVector.append(termBlock2);
            terminalBlocksVector.append(termBlock3);
            //TerminalBlocksConfiguration END
        }
        else
        {
            //WiresConfiguration BEGIN
            if(!wiresVector.isEmpty())
                foreach (Wire *a, wiresVector)
                {
                    wiresVector.removeOne(a);
                }
            auto *wire1 = new Wire();
            wire1->wireName = "ШВПМ 2х1.5";
            wire1->lenght = "1";
            auto *wire2 = new Wire();
            wire2->wireName = "ШВПМ 2х2";
            wire2->lenght = "1";
            auto *wire3 = new Wire();
            wire3->wireName = "ШВПМ 2х0.2";
            if((stepsCount()%5 == 0)&&(stepsCount() >= 5)) //округление
            {
                wire3->lenght = QString::number(stepsCount()*6);
            }
            else
            {
                wire3->lenght = QString::number(stepsCount()*6);
                for(int i = 0;i<15;i++)
                {
                    if(wire3->lenght.toInt()%5 == 0)
                    {
                        continue;
                    }
                    else
                        wire3->lenght = QString::number(wire3->lenght.toInt() + 1);
                }
            }
            auto *wire4 = new Wire();
            wire4->wireName = "ШВВП 2х0.7";
            wire4->lenght = "10";

            wiresVector.append(wire1);
            wiresVector.append(wire2);
            wiresVector.append(wire3);
            wiresVector.append(wire4);
            //WiresConfiguration END

            //TerminalBlocksConfiguration BEGIN
            if(!terminalBlocksVector.isEmpty())
                foreach (TerminalBlock *a, terminalBlocksVector)
                {
                    terminalBlocksVector.removeOne(a);
                }
            auto *termBlock1 = new TerminalBlock();
            termBlock1->terminalBlockName = "WAGO 2 pin";
            termBlock1->count = stepsCount()*2;
            auto *termBlock2 = new TerminalBlock();
            termBlock2->terminalBlockName = "WAGO 5 pin";
            if(stepsCount()%3 == 0)
                termBlock2->count = stepsCount()/3;
            else if((stepsCount()+1)%3 == 0)
                termBlock2->count =  (stepsCount()+1)/3;
            else if((stepsCount()-1)%3 == 0)
                termBlock2->count = (stepsCount()-1)/3;

            terminalBlocksVector.append(termBlock1);
            terminalBlocksVector.append(termBlock2);
            //TerminalBlocksConfiguration END
        }
}

void Set::sensorCountConfiguration()
{
    uint count = 0;
    foreach (Controller *a, controllersVector)
    {
        count++;
    }
    foreach (Sensor *a, sensorsVector)
    {
        a->count->clear();
        for(int i = 1;i<=count*2;i++)
            a->count->addItem(QString::number(i));
    }
}

int Set::stepsCount()
{
    unsigned int count = 0;
    foreach (Controller *a, controllersVector)
    {
        count += a->steps->text().toInt();
    }
    return count;
}

double Set::premiumPrice()
{
    qDebug()<<"begin";
    double temporaryPrice = price;
    foreach (Led *a, ledsVector)
    {
        if(a->color->currentText() == "RGB")
        {
            temporaryPrice -=a->price.toDouble()*a->count->currentText().toInt();
            if((a->lenght->text().toDouble() != 1)&&(a->lenght->text().toDouble() != 1.2)&&(a->lenght->text().toDouble() != 0.8))
                temporaryPrice += 490*a->count->currentText().toInt()*(a->lenght->text().toDouble())*1.4;
            else
                temporaryPrice += 490*a->count->currentText().toInt()*a->lenght->text().toDouble();
        }
        else
        {
            temporaryPrice -=a->price.toDouble()*a->count->currentText().toInt();
            if((a->lenght->text().toDouble() != 1)&&(a->lenght->text().toDouble() != 1.2)&&(a->lenght->text().toDouble() != 0.8))
                temporaryPrice += 360*a->count->currentText().toInt()*(a->lenght->text().toDouble())*1.4;
            else
                temporaryPrice += 360*a->count->currentText().toInt()*a->lenght->text().toDouble();
        }
    }
    qDebug()<<temporaryPrice;
    return temporaryPrice;


}

double Set::ecoPrice()
{
    qDebug()<<"begin";
    double temporaryPrice = price;
    foreach (Led *a, ledsVector)
    {
        temporaryPrice -=a->price.toDouble()*a->count->currentText().toInt();
        if((a->lenght->text().toDouble() != 1)&&(a->lenght->text().toDouble() != 1.2)&&(a->lenght->text().toDouble() != 0.8))
            temporaryPrice += 190*a->count->currentText().toInt()*(a->lenght->text().toDouble())*1.4;
        else
            temporaryPrice += 190*a->count->currentText().toInt()*a->lenght->text().toDouble();
    }
    qDebug()<<temporaryPrice;
    return temporaryPrice;
}
