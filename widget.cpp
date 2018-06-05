#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QGridLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto *a = new QWidget();
    a->setLayout(lay);
    ui->scrollArea->setWidget(a);


    //DataBase Connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("server151.hosting.reg.ru");
    db.setPort(3306);
    db.setDatabaseName("u0437543_conf");
    db.setUserName("u0437543_root");
    db.setPassword("231659487aS-");
    db.open();
    //    ui->innLineEdit->setText("Connection Done");
    //     QString f;
    //        foreach (const QString &str, db.drivers())
    //    f.append(QString(" [%1] ").arg(str));
    //ui->innLineEdit->setText(f);
    if(db.open())
    {

        qDebug()<<"Connection Done";
    }
    else
    {
        ui->innLineEdit->setText(db.lastError().text());
        qDebug()<<db.lastError().text();
    }

    setsText.append("Поле информации");
    Text->setText(setsText);
    auto *le = new QGridLayout();
    le->addWidget(Text,0,0);
    ui->scrollArea_2->setWidget(Text);

    emit addSet();
    // toCSV();

}

void Widget::deleteSetSlot(Set *a)
{
    setsVector.removeOne(a);
}

void Widget::addSet()
{
    qDebug()<<"Widget::addSet()";
    auto *a = new Set();
    lay->addLayout(a->lay,setsCount,0);
    setsVector.append(a);
    connect(a->addSet,&QPushButton::clicked,this,&Widget::addSet);
    connect(a,&Set::addSensorSignal,this,&Widget::sensorConfiguration);
    connect(a,&Set::addLedSignal,this,&Widget::ledConfiguration);
    connect(a,&Set::addProfileSignal,this,&Widget::profileConfiguration);
    connect(a,&Set::addPowerSupplySignal,this,&Widget::powerSupplyConfiguration);
    connect(a,&Set::addControllerSignal,this,&Widget::controllerConfiguration);
    connect(a,&Set::updateText,this,&Widget::updateText);
    connect(a,&Set::deleteButtonSignal,this,&Widget::deleteSetSlot);
    setsCount++;
}

void Widget::sensorConfiguration(Sensor *a)
{
    QSqlQuery query;
    query.exec("SELECT type,color from sensors_table ");
    while (query.next())
    {
        a->type->addItem(query.value(0).toString());
        a->color->addItem(query.value(1).toString());
    }
    a->type->removeItem(0);
    uint count = 0;
    foreach (Set *a, setsVector)
    {
        foreach (Controller *b, a->controllersVector)
        {
            count++;
        }
    }
    for(int i = 1; i<=count*2;i++)
        a->count->addItem(QString::number(i));

    connect(a,&Sensor::checkPrice,this,&Widget::checkPrice);
    emit a->checkPrice();
}

void Widget::ledColorConfiguration(Led *a)
{
    a->color->clear();
    a->color->addItems(Ledbuffer.values(a->type->currentText()));
}

void Widget::ledConfiguration(Led *a)
{
    QSqlQuery query;
    if(Ledbuffer.isEmpty())
    {
        query.exec("SELECT type,color from leds_table");
        while(query.next())
        {
            Ledbuffer.insertMulti(query.value(0).toString(),query.value(1).toString());
        }
    }
    a->type->addItems(Ledbuffer.uniqueKeys());
    connect(a,&Led::checkPrice,this,&Widget::checkPrice);
    connect(a,&Led::changeTextSignal,this,&Widget::ledColorConfiguration);
    emit ledColorConfiguration(a);
}

void Widget::profileConfiguration(Profile *a)
{
    QSqlQuery query;
    query.exec("SELECT type from profiles_table ");
    while(query.next())
        a->type->addItem(query.value(0).toString());
    connect(a,&Profile::checkPrice,this,&Widget::checkPrice);
}

void Widget::powerSupplyConfiguration(powerSupply *a)
{
    QSqlQuery query;
    query.exec("SELECT type from powerSupply_table ");
    while(query.next())
        a->type->addItem(query.value(0).toString());
    connect(a,&powerSupply::checkPrice,this,&Widget::checkPrice);
    emit a->type->currentTextChanged(QString::number(1));
}

void Widget::controllerConfiguration(Controller *a)
{
    QSqlQuery query;
    query.exec("SELECT type from controllersPrice");
    while(query.next())
        a->type->addItem(query.value(0).toString());
    connect(a,&Controller::checkPrice,this,&Widget::checkPrice);

}

void Widget::checkPrice()
{
    QObject* CB = QObject::sender();
    qDebug()<<"Widget::checkPrice()";
    QSqlQuery query;
    QMap<QString,QString> buffer;
    foreach (Set *a, setsVector)
    {
        foreach (Sensor* b , a->sensorsVector)
        {
            if((b->color == CB)||(b->count == CB)||(b->type == CB)||(CB == b))
            {
                qDebug()<<"sender = sensor";
                query.exec("SELECT price,name FROM sensorsprice");
                while(query.next())
                {
                    buffer.insert(query.value(1).toString(),query.value(0).toString());
                }
                QString sensorName;
                sensorName.append(b->color->currentText());
                sensorName.append(b->type->currentText());
                if(buffer.contains(sensorName))
                    b->price = buffer.value(sensorName);
            }
        }

        buffer.clear();
        foreach (Led *b, a->ledsVector)
        {
            if((b->color == CB)||(b->count == CB)||(b->type == CB)||(b->lenght == CB)||(CB == b))
            {
                query.exec("SELECT price,name FROM ledsprice");
                while(query.next())
                {
                    buffer.insert(query.value(1).toString(),query.value(0).toString());
                }
                QString ledName;
                ledName.append(b->type->currentText());
                ledName.append(b->color->currentText());
                if(buffer.contains(ledName))
                    b->price = buffer.value(ledName);
                if((b->lenght->text().toDouble() != 1)&&(b->lenght->text().toDouble() != 1.2)&&(b->lenght->text().toDouble() != 0.8))
                    b->price = b->price.toInt()*(b->lenght->text().toDouble())*1.4;
                else
                    b->price = b->price.toInt()*b->lenght->text().toDouble();
                qDebug()<<b->price;
            }

        }

        buffer.clear();
        foreach (Profile *b, a->profilesVector)
        {
            if((b->count == CB)||(b->type == CB)||(b->lenght == CB)||(CB == b))
            {
                query.exec("SELECT price,name FROM profilesprice");
                while(query.next())
                {
                    buffer.insert(query.value(1).toString(),query.value(0).toString());
                }
                QString profileName;
                profileName.append(b->type->currentText());
                if(buffer.contains(profileName))
                    b->price = buffer.value(profileName);
                if((b->lenght->text().toDouble() != 1)&&(b->lenght->text().toDouble() != 1.2)&&(b->lenght->text().toDouble() != 0.8))
                    b->price = b->price.toInt()*b->lenght->text().toDouble()*1.4;
                else
                    b->price = b->price.toInt()*b->lenght->text().toDouble();
            }
        }
        if(dynamic_cast<Controller*>(CB) != nullptr)
        {

            qDebug()<<"sender = controller";
            buffer.clear();
            foreach (Wire *b, a->wiresVector)
            {
                query.exec("SELECT price,name FROM wiresprice");
                while(query.next())
                {
                    buffer.insert(query.value(1).toString(),query.value(0).toString());
                }
                if(buffer.contains(b->wireName))
                    b->price = buffer.value(b->wireName);
            }

            buffer.clear();
            foreach (TerminalBlock *b, a->terminalBlocksVector)
            {
                query.exec("SELECT price,name FROM terminalBlocksPrice");
                while(query.next())
                {
                    buffer.insert(query.value(1).toString(),query.value(0).toString());
                }
                if(buffer.contains(b->terminalBlockName))
                    b->price = buffer.value(b->terminalBlockName).toInt();
            }
        }

        buffer.clear();
        foreach (Controller *b, a->controllersVector)
        {
            query.exec("SELECT price,type FROM controllersPrice");
            while(query.next())
            {
                buffer.insert(query.value(1).toString(),query.value(0).toString());
            }
            if(buffer.contains(b->type->currentText()))
                b->price = buffer.value(b->type->currentText()).toInt();
        }

        buffer.clear();
        foreach (powerSupply *b, a->powerSuppliesVector)
        {
            query.exec("SELECT  price,type FROM powerSupply_table");
            while(query.next())
            {
                buffer.insert(query.value(1).toString(),query.value(0).toString());
            }
            if(buffer.contains(b->type->currentText()))
                b->price = buffer.value(b->type->currentText()).toInt();
        }
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listOfOrders_clicked()
{

}

void Widget::on_IndividualOrderPB_clicked()
{
    foreach (Set *a, setsVector)
    {
        foreach (Sensor* b , a->sensorsVector)
        {
            qDebug()<<"Sensor";
            qDebug()<<b->price;
        }


        foreach (Led *b, a->ledsVector)
        {
            qDebug()<<"Led";
            qDebug()<<b->price;
        }


        foreach (powerSupply *b, a->powerSuppliesVector)
        {
            qDebug()<<b->type->currentText();

        }

    }
}

void Widget::updateText()
{
    setsText.clear();
    foreach (Set *a, setsVector)
    {
        setsText.append(a->outputText());
    }
    Text->setText(setsText);

}

void Widget::on_physicalPersonCheckBox_stateChanged(int arg1)
{
    if(arg1 != 0)
    {
        ui->innLineEdit->show();
        ui->innLabel->show();
    }
    else
    {
        ui->innLabel->hide();
        ui->innLineEdit->hide();
        ui->innLineEdit->clear();
    }
}

void Widget::setItemsToTable(QString a, QString b, QString c, QString d, QString e, QString f)
{
    auto *tableItem1 = new QTableWidgetItem();
    auto *tableItem2 = new QTableWidgetItem();
    auto *tableItem3 = new QTableWidgetItem();
    auto *tableItem4 = new QTableWidgetItem();
    auto *tableItem5 = new QTableWidgetItem();
    auto *tableItem6 = new QTableWidgetItem();
    tableItem1->setText(a);
    tableItem2->setText(b);
    tableItem3->setText(c);
    tableItem4->setText(d);
    tableItem5->setText(e);
    tableItem6->setText(f);
    tableWidget->setItem(rowCount,0,tableItem1);
    tableWidget->setItem(rowCount,1,tableItem2);
    tableWidget->setItem(rowCount,2,tableItem3);
    tableWidget->setItem(rowCount,3,tableItem4);
    tableWidget->setItem(rowCount,4,tableItem5);
    tableWidget->setItem(rowCount,5,tableItem6);
    rowCount++;
    a.clear();
    b.clear();
    c.clear();
    d.clear();
    e.clear();
    f.clear();
}

void Widget::toCSV()
{
    fullPrice = 0;
    tableWidget->clear();
    rowCount = 0;
    tableWidget->setColumnCount(6);
    tableWidget->setRowCount(100);
    unsigned int itemCount = 1;
    setItemsToTable("Номер заказа - " + ui->orderNumberLineEdit->text(),
                    ui->personNameLabel->text() + " " + ui->personNameLineEdit->text(),
                    "","","","");

    setItemsToTable("№",
                    "Наименование, характеристика товара",
                    "Кол-во",
                    "Ед.изм",
                    "Цена",
                    "Сумма");
    foreach (Set *set, setsVector)
    {
        foreach (Controller *a, set->controllersVector)
        {
            setItemsToTable(QString::number(itemCount),
                            a->type->currentText() + " STAIRSLIGHT 18 каналов SL18-12v Дин-рейка 6 модулей",
                            "1",
                            "Шт",
                            QString::number(a->price),
                            QString::number(a->price));
            itemCount++;
            fullPrice +=a->price;

            if(a->expansionModule->isChecked())
            {
                if((a->steps->text().toInt()>18)&&(a->steps->text().toInt()<=27))
                {
                    setItemsToTable(QString::number(itemCount),
                                    "Модуль расширения для лестниц от 19 до 27 ступеней Цена - 2900",
                                    "1",
                                    "Шт",
                                    "2900",
                                    "2900");
                    itemCount++;
                    fullPrice +=2900;
                }
                else if(a->steps->text().toInt()>27)
                {
                    setItemsToTable(QString::number(itemCount),
                                    "Модуль расширения для лестниц от 27 до 36 ступеней Цена - 3600",
                                    "1",
                                    "Шт",
                                    "3600",
                                    "3600");
                    itemCount++;
                    fullPrice +=3600;
                }
            }
        }
        foreach (Sensor *a, set->sensorsVector)
        {
            setItemsToTable(QString::number(itemCount),
                            "ИК Датчик " + a->type->currentText() + " " + a->color->currentText(),
                            a->count->currentText(),
                            "Шт",
                            a->price.toString(),
                            QString::number(a->price.toInt()*a->count->currentText().toInt())
                            );
            itemCount++;
            fullPrice +=a->price.toInt()*a->count->currentText().toInt();
        }

        foreach (Led *a, set->ledsVector)
        {
            setItemsToTable(QString::number(itemCount),
                            "Светодиодная лента " + a->type->currentText() + " с подпаянными проводами длинной 1м 60 свет/м,"
                            + QString::number(a->requiredPower) + "Вт/м, 12V. Оттенок " + a->color->currentText() + " Длина ленты - " + a->lenght->text() +"м",
                            a->count->currentText(),
                            "Шт",
                            QString::number(a->price.toDouble()),
                            QString::number(a->price.toDouble() * a->count->currentText().toInt())
                            );
            itemCount++;
            fullPrice +=a->price.toDouble() * a->count->currentText().toInt();
        }

        foreach (Profile *a, set->profilesVector)
        {
            setItemsToTable(QString::number(itemCount),
                            "Алюминиевый профиль с матовым экраном " + a->type->currentText() + " 16х7мм." + " Длина профиля - " + a->lenght->text() + "м",
                            a->count->currentText(),
                            "Шт",
                            QString::number(a->price.toDouble()),
                            QString::number(a->price.toDouble() * a->count->currentText().toInt())
                            );
            itemCount++;
            fullPrice +=a->price.toDouble() * a->count->currentText().toInt();
        }

        foreach (powerSupply *a, set->powerSuppliesVector)
        {
            setItemsToTable(QString::number(itemCount),
                            "Блок питания в защитном кожухе 12v-" + a->type->currentText(),
                            "1",
                            "Шт",
                            a->price.toString(),
                            a->price.toString()
                            );
            itemCount++;
            fullPrice +=a->price.toInt();
        }

        foreach (Wire *a, set->wiresVector)
        {
            setItemsToTable(QString::number(itemCount),
                            a->wireName,
                            a->lenght,
                            "М",
                            a->price.toString(),
                            QString::number(a->price.toInt() * a->lenght.toInt())
                            );
            itemCount++;
            fullPrice +=a->price.toInt() * a->lenght.toInt();
        }

        foreach (TerminalBlock *a, set->terminalBlocksVector)
        {
            setItemsToTable(QString::number(itemCount),
                            a->terminalBlockName,
                            QString::number(a->count),
                            "Шт",
                            QString::number(a->price),
                            QString::number(a->price*a->count)
                            );
            itemCount++;
            fullPrice +=a->price*a->count;
        }
        setItemsToTable(QString::number(itemCount),
                        "Комплект инструментов для настройки и сборки: Бокорезы Sparta, Отвертка индикаторная, Кусачки, Плоскогубцы",
                        "1",
                        "Шт",
                        "0",
                        "0"
                        );
        itemCount++;

        setItemsToTable(QString::number(itemCount),
                        "Инструкция по установке и эксплуатации оборудования",
                        "1",
                        "Шт",
                        "0",
                        "0"
                        );
        itemCount++;

        setItemsToTable(QString::number(itemCount),
                        "Гарантийный талон",
                        "1",
                        "Шт",
                        "0",
                        "0"
                        );
        itemCount++;

        setItemsToTable("",
                        "",
                        "",
                        "",
                        "Итого",
                        QString::number(fullPrice)
                        );
        itemCount++;

        setItemsToTable("",
                        "",
                        "",
                        "",
                        "Скидка",
                        ""
                        );
        itemCount++;

        setItemsToTable("",
                        "",
                        "",
                        "",
                        "К оплате",
                        QString::number(fullPrice)
                        );
        itemCount++;

        if(!(ui->innLineEdit->text().isEmpty()))
        {
            setItemsToTable("",
                            "ИНН - ",
                            ui->innLineEdit->text(),
                            "",
                            "",
                            ""
                            );
            itemCount++;
        }

        setItemsToTable("",
                        "Дополнительная информация - ",
                        ui->additionInfoTextEdit->toPlainText(),
                        "",
                        "",
                        ""
                        );
        itemCount++;
    }

    //    tableWidget->show();
    QString name;
    QDateTime curTime = QDateTime::currentDateTime();
    name.append(curTime.toString("dd.MM.yyyy"));
    name.append(" Order №");
    name.append(ui->orderNumberLineEdit->text());
    name.append(".csv");
    QFile f(name); //Объявление переменной файлового класса
    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        strList << "\" \"";

        for( int c = 0; c < tableWidget->horizontalHeader()->count(); ++c )
            strList << "\""+tableWidget->model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < tableWidget->verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\""+tableWidget->model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < tableWidget->horizontalHeader()->count(); ++c )
            {
                strList << "\""+tableWidget->model()->data(tableWidget->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }
        f.close();
    }

}

void Widget::on_pushButton_clicked()
{
    toCSV();
}

void Widget::on_saveToBD_pushButton_clicked()
{
    QString output;
    foreach (Set *a, setsVector)
    {
        if(!a->controllersVector.isEmpty())
        {
            foreach (Controller *b, a->controllersVector)
            {
                output.append("__" + b->type->currentText() + "_");
                if(b->expansionModule->isChecked())
                    output.append("__Модуль расширения_НЕИЗВЕСТНЫЙ ТИП МОДУЛЯ");
            }
        }
        if(!a->sensorsVector.isEmpty())
        {
            foreach (Sensor*b, a->sensorsVector)
            {
                output.append("__Датчик_" + b->type->currentText()+"_"
                              + b->color->currentText() +"_"+ b->count->currentText() +  "_шт");
            }
        }
        if(!a->ledsVector.isEmpty())
        {
            foreach (Led*b, a->ledsVector)
            {
                output.append("__Лента_" + b->type->currentText()+"_"
                              + b->color->currentText()+"_" + b->lenght->text() + "_м_"
                              + b->count->currentText() + "_шт");
            }
        }
        if(!a->profilesVector.isEmpty())
        {
            foreach (Profile*b, a->profilesVector)
            {
                output.append("__Профиль_" + b->type->currentText() +"_"
                              + b->lenght->text() + "_м_" + b->count->currentText() + "_шт");
            }
        }
        if(!a->powerSuppliesVector.isEmpty())
        {
            foreach (powerSupply *b, a->powerSuppliesVector)
            {
                output.append("__Блок питания_" + b->type->currentText());
            }
        }
        if(!a->baseSet)
        {
            if(!a->wiresVector.isEmpty())
            {
                foreach (Wire *b, a->wiresVector)
                {
                    output.append("__Провод_" + b->wireName + "_" + b->lenght + "_м");
                }
            }

            if(!a->terminalBlocksVector.isEmpty())
                foreach (TerminalBlock *b, a->terminalBlocksVector)
                {
                    output.append("__Клеммник_" + b->terminalBlockName + "_" + QString::number(b->count) + "_шт");
                }
        }
    }
    QSqlQuery query;
    query.prepare("INSERT INTO ordersList (orderNumber, orderItems, nameOfManager, nameOfCustomer, Notes) VALUES(:number, :items, :nameOfManager, :nameOfCustomer, :notes);");
    query.bindValue(":number", ui->orderNumberLineEdit->text());
    query.bindValue(":items", output);
    query.bindValue(":nameOfManager", ui->managerNameCB->currentText());
    query.bindValue(":nameOfCustomer", ui->personNameLineEdit->text());
    query.bindValue(":notes", ui->additionInfoTextEdit->toPlainText());
    query.exec();
}


void Widget::ordersTable(QString a, QString b, QString c, QString d, QString e)
{
    auto *tableItem1 = new QTableWidgetItem();
    auto *tableItem2 = new QTableWidgetItem();
    auto *tableItem3 = new QTableWidgetItem();
    auto *tableItem4 = new QTableWidgetItem();
    auto *tableItem5 = new QTableWidgetItem();
    tableItem1->setText(a);
    tableItem2->setText(b);
    tableItem3->setText(c);
    tableItem4->setText(d);
    tableItem5->setText(e);
    tb->setItem(OrderRowCount,0,tableItem1);
    tb->setItem(OrderRowCount,1,tableItem2);
    tb->setItem(OrderRowCount,2,tableItem3);
    tb->setItem(OrderRowCount,3,tableItem4);
    tb->setItem(OrderRowCount,4,tableItem5);
    OrderRowCount++;
    a.clear();
    b.clear();
    c.clear();
    d.clear();
    e.clear();
}
void Widget::on_pushButton_3_clicked()
{


    QSqlQuery query;
    tb = new QTableWidget();
    query.exec("SELECT orderNumber, orderItems, nameOfManager, nameOfCustomer, Notes from ordersList");

    OrderRowCount = 0;
    tb->setColumnCount(5);
    tb->setRowCount(100);
    ordersTable("№ Заказа",
                "Содержание заказа",
                "Имя менеджера",
                "ФИО клиента",
                "Доп. Инфо");

    while(query.next())
    {
        qDebug()<<query.value(0).toString();
        ordersTable(query.value(0).toString(),
                    query.value(1).toString(),
                    query.value(2).toString(),
                    query.value(3).toString(),
                    query.value(4).toString());
    }

    foreach (Set *a, setsVector)
    {
        lay->removeWidget(a);
        emit a->deleteButtonSlot();
    }
    lay->addWidget(tb);



}

void Widget::on_pushButton_4_clicked()
{
    tb->deleteLater();
    foreach (Set *a, setsVector)
    {
        lay->removeWidget(a);
        emit a->deleteButtonSlot();
        deleteSetSlot(a);
    }
    emit addSet();

}
