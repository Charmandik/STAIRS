#include "profile.h"

Profile::Profile()
{
    pb->setText("-");
    this->lay->addWidget(count,0,0);
    this->lay->addWidget(type,0,1);
    this->lay->addWidget(lenght,0,2);
    this->lay->addWidget(pb,0,3);
    lenght->setPlaceholderText("Длина м.");
    lenght->setText("1");
    pb->setMaximumWidth(15);
    pb->setMaximumHeight(15);
    connect(pb,&QPushButton::clicked,this,&Profile::deleteThis);
    connect(count,&QComboBox::currentTextChanged,this,&Profile::checkPrice);
    connect(type,&QComboBox::currentTextChanged,this,&Profile::checkPrice);
    connect(lenght,&QLineEdit::textChanged,this,&Profile::checkPrice);
    lenght->setMaximumWidth(100);
    count->setMaximumWidth(50);
    type->setMaximumWidth(100);

}

Profile::~Profile()
{

}

void Profile::deleteThis()
{
    emit deleteFromVector(this);
    count->deleteLater();
    type->deleteLater();
    lenght->deleteLater();
    pb->deleteLater();
    lay->deleteLater();
}
