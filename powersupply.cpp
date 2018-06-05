#include "powersupply.h"

powerSupply::powerSupply()
{
    pb->setText("-");
    lay->addWidget(type,0,0);
    lay->addWidget(pb,0,1);
    pb->setMaximumWidth(15);
    pb->setMaximumHeight(15);
    connect(pb,&QPushButton::clicked,this,&powerSupply::deleteThis);
    connect(type,&QComboBox::currentTextChanged,this,&powerSupply::checkPrice);
    type->setMaximumWidth(100);
}
void powerSupply::deleteThis()
{
    emit deleteFromVector(this);
    type->deleteLater();
    pb->deleteLater();
    lay->deleteLater();
}
