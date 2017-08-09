/*****************************************************************//*
 *
 * @file        MainDialog.cpp
 * @brief       This class implements the MainDialog UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/


#include "maindialog.h"
#include "ui_maindialog.h"
#include "EventGenerator.h"
#include <QApplication>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    p = new Panel;
    w = new EventGenerator;


}


void MainDialog::ConnectingHost(QString ipAddress)
{
   this->ipAddress = ipAddress;
}

MainDialog::~MainDialog()
{
    delete ui;
    delete w;
    delete p;
    w = NULL;
    p = NULL;
}

void MainDialog::on_moduleButton_clicked()
{
    w->ConnectingHost(ipAddress);
    w->show();
    this->hide();
}

void MainDialog::on_panelButton_clicked()
{
    p->ConnectingHost(ipAddress);
    p->setGeometry(QStyle::alignedRect(
                       Qt::LeftToRight,
                       Qt::AlignCenter,
                       p->size(),
                       qApp->desktop()->availableGeometry()
                   ));
    p->show();
    this->close();
}
