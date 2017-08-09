/*****************************************************************//*
 *
 * @file        MainDialog.h
 * @brief       This class implements the MainDialog UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include "panel.h"
#include <QDialog>
#include <QTcpSocket>
#include "EventGenerator.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    MainDialog(QString ipAddress,QWidget *parent = 0);
    ~MainDialog();
    void ConnectingHost(QString ipAddress);


private slots:
    void on_moduleButton_clicked();
    void on_panelButton_clicked();

private:
    Ui::MainDialog *ui;
    Panel *p;
    EventGenerator *w;
    QString ipAddress;
};

#endif // MAINDIALOG_H
