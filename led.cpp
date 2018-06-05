#include "led.h"

Led::Led()
{
    this->pb->setText("-");
    this->lay->addWidget(count,0,0);
    this->lay->addWidget(type,0,1);
    this->lay->addWidget(color,0,2);
    this->lay->addWidget(lenght,0,3);
    this->lay->addWidget(pb,0,4);
    color->setMaximumWidth(100);
    count->setMaximumWidth(50);
    type->setMaximumWidth(130);
    lenght->setMaximumWidth(50);
    pb->setMaximumWidth(15);
    pb->setMaximumHeight(15);
    lenght->setPlaceholderText("Длина");
    lenght->setText("1");
    connect(pb,&QPushButton::clicked,this,&Led::deleteThis);
    connect(type,&QComboBox::currentTextChanged,this,&Led::powerCalculation);
    connect(color,&QComboBox::currentTextChanged,this,&Led::powerCalculation);
    connect(color,&QComboBox::currentTextChanged,this,&Led::checkPrice);
    connect(type,&QComboBox::currentTextChanged,this,&Led::checkPrice);
    connect(count,&QComboBox::currentTextChanged,this,&Led::checkPrice);
    connect(lenght,&QLineEdit::textChanged,this,&Led::checkPrice);
    connect(type,&QComboBox::currentTextChanged,this,&Led::changeTextSlot);
}

Led::~Led()
{

}

void Led::deleteThis()
{
    emit deleteFromVector(this);
    count->deleteLater();
    type->deleteLater();
    color->deleteLater();
    lenght->deleteLater();
    lay->deleteLater();
    pb->deleteLater();
}

void Led::powerCalculation()
{
    if((type->currentText() == "Премиум")&&(color->currentText() == "RGB"))
            requiredPower = 14.4;
    else if((type->currentText() == "Люкс Arlight")&&(color->currentText() == "RGB"))
            requiredPower = 14.4;
    else if(type->currentText() == "Эконом")
            requiredPower = 4.8;
    else if(type->currentText() == "Премиум")
            requiredPower = 12;
    else if(type->currentText() == "Люкс Arlight")
            requiredPower = 9.6;
    else if(type->currentText() == "Люкс Arlight 5060")
            requiredPower = 14.4;
    else if(type->currentText() == "Люкс Arlight 5060B")
            requiredPower = 11.5;
}

void Led::changeTextSlot()
{
    emit changeTextSignal(this);
}
