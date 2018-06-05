#include "controller.h"

Controller::Controller()
{
    steps->setPlaceholderText("Ступени");
    expansionModule->setText("Модуль Расширения");
    deleteButton->setText("-");
    lay->addWidget(steps,0,0);
    lay->addWidget(type,0,1);
    lay->addWidget(expansionModule,0,2);
    lay->addWidget(deleteButton,0,3);
    connect(deleteButton,&QPushButton::clicked,this,&Controller::deleteThis);
    deleteButton->setMaximumWidth(15);
    deleteButton->setMaximumHeight(15);
    steps->setMaximumWidth(75);
    type->setMaximumWidth(100);
    expansionModule->setMaximumWidth(150);
    connect(steps,&QLineEdit::textChanged,this,&Controller::checkPrice);
    connect(type,&QComboBox::currentTextChanged,this,&Controller::checkPrice);


}

Controller::~Controller()
{

}

void Controller::deleteThis()
{
    emit deleteFromVector(this);
    steps->deleteLater();
    expansionModule->deleteLater();
    type->deleteLater();
    deleteButton->deleteLater();
    lay->deleteLater();
}
