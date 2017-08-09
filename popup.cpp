/*****************************************************************//*
 *
 * @file        PopUp.cpp
 * @brief       This class implements the PopUp UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#include "popup.h"
#include "ui_popup.h"
#include <QFile>
#include <QDebug>
#include <sstream>
#include <iostream>

int PopUp::rowCount= 0;
PopUp::PopUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopUp)
{
    ui->setupUi(this);

    ui->tableWidget->insertRow(rowCount++);
    for(int j = 11; j>=0;j--)
    {
        ui->tableWidget->insertColumn(0);
    }
    ui->tableWidget->setColumnWidth(4,200);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("Module Type")));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("Module ID")));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("Module Status")));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("Event Code")));
    ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("Event Name")));
    ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("Data 1")));
    ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("Data 2")));
    ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("Data 3")));
    ui->tableWidget->setItem(0,8,new QTableWidgetItem(tr("Data 4")));
    ui->tableWidget->setItem(0,9,new QTableWidgetItem(tr("Data 5")));
    ui->tableWidget->setItem(0,10,new QTableWidgetItem(tr("Data 6")));
    ui->tableWidget->setItem(0,11,new QTableWidgetItem(tr("Data 7")));

}

PopUp::~PopUp()
{
    delete ui;
}

void PopUp::SetText(QStringList events)
{


    m_fd = new QFileDialog;
    QString eventName;
    QStringList total;
    QString toolTipString;
    int i = 0;
    QFile f("eventlist.csv");
    qDebug()<<"Reading File " << f.fileName();
    f.open(QIODevice::ReadOnly);        //Opens the file in read-only mode
    QString line;
    while (!f.atEnd())
    {
        line = f.readLine();            //reads data from file and stores it in bytearray
        total=line.split(",");             //appends the data as a single bytearray
        if(total[0] == events[3]){
            break;
        }else{
            i++;
        }
    }

      qDebug()<<i;
      ui->tableWidget->insertRow(rowCount++);

    eventName = total[1];
    for(i=0;i<events.size();i++){

        ui->tableWidget->setItem(rowCount-1,i,new QTableWidgetItem(events[i]));

          if(eventName.contains("FIRE"))
          {
             ui->tableWidget->item(rowCount-1,i)->setBackground(Qt::red);
          }

          else if(eventName.contains("FAULT"))
          {
             ui->tableWidget->item(rowCount-1,i)->setBackground(Qt::yellow);
          }

          else
          {
             ui->tableWidget->item(rowCount-1,i)->setBackground(Qt::cyan);
          }

        if(i > 4){
            toolTipString = total[i-3];
            ui->tableWidget->item(rowCount-1,i)->setToolTip(toolTipString);
        }

    }

}

void PopUp::on_pushButton_clicked()
{
    int i;
    int totalRow = ui->tableWidget->rowCount();
    for ( i = totalRow; i >=1 ; i-- )
    {
          ui->tableWidget->removeRow(i);
    }
    rowCount = 1;
    this->close();
}
