#include "sensor.h"

Sensor::Sensor()
{
    this->pb->setText("-");
    this->lay->addWidget(this->pb,0,3);
    this->lay->addWidget(this->color,0,2);
    this->lay->addWidget(this->type,0,1);
    this->lay->addWidget(this->count,0,0);
    pb->setMaximumWidth(15);
    pb->setMaximumHeight(15);
    count->setMaximumWidth(50);
    type->setMaximumWidth(100);
    color->setMaximumWidth(100);
    connect(pb,&QPushButton::clicked,this,&Sensor::deleteThis);
    connect(count,&QComboBox::currentTextChanged,this,&Sensor::checkPrice);
    connect(type,&QComboBox::currentTextChanged,this,&Sensor::checkPrice);
    connect(color,&QComboBox::currentTextChanged,this,&Sensor::checkPrice);
}

void Sensor::deleteThis()
{
    emit deleteFromVector(this);
    color->deleteLater();
    count->deleteLater();
    type->deleteLater();
    lay->deleteLater();
    pb->deleteLater();
}

Sensor::~Sensor()
{

}
