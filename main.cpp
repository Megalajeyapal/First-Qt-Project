/***************************************************************************//*
 *
 * @file        main.cpp
 * @brief       This class implements the EventGenerator UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ******************************************************************************/
#include "maindialog.h"
#include "EventGenerator.h"
#include <QApplication>
#include <QtCore>
#include <QStringList>
#include <QWidget>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString ipAddress = NULL;
    MainDialog *w = new MainDialog();
    QStringList args = a.arguments();

    if(args.length()>1)
    {
        ipAddress = args.join(",");
        QString temp = ipAddress.split("1").at(0);
        ipAddress = ipAddress.remove(temp);
        qDebug()<<"ipaddress "<<ipAddress;
    }

     w->ConnectingHost(ipAddress);
     w->show();
    return a.exec();
}
