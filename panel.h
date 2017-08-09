/*****************************************************************//*
 *
 * @file        Panel.h
 * @brief       This class implements the Panel UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/


#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>
#include <QtNetwork>
#include <QUdpSocket>
#include <QFileDialog>
#include <QtWidgets>
#include <QProcess>
#include <QMap>
#include "popup.h"
#include "NetworkEvent.h"


namespace Ui {
class Panel;
}

/************************************************//*
 *  @class      Panel
 *  @brief      Panel class declaration
 *************************************************/

class Panel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    Panel(QString ipAddress, QWidget *parent = 0);
    ~Panel();

    //Public Methods Declaration begins
    void ConnectingHost(QString ipAddress);
    void Led(QLineEdit *t,QString moduleStatus);
    void WriteFile(QString moduleId, QString data);
    void FileRead(QString fileName);

private slots:

    //private slots declaration begins
    void Update();
    void Error();

    //button clicks
    void on_closeButton_clicked();
    void on_loadButton_clicked();
    void on_detectButton_clicked();
    void on_disConnectButton_clicked();

    //loads log files of all 16 modules
    void on_logButton_1_clicked();
    void on_logButton_2_clicked();
    void on_logButton_3_clicked();
    void on_logButton_4_clicked();
    void on_logButton_5_clicked();
    void on_logButton_6_clicked();
    void on_logButton_7_clicked();
    void on_logButton_8_clicked();
    void on_logButton_9_clicked();
    void on_logButton_10_clicked();
    void on_logButton_11_clicked();
    void on_logButton_12_clicked();
    void on_logButton_13_clicked();
    void on_logButton_14_clicked();
    void on_logButton_15_clicked();
    void on_logButton_16_clicked();

private:

    //Private Attributes declaration begins
    Ui::Panel *ui;
    QUdpSocket udpSocket;
    QFileDialog *m_fd;
    static  std::string fileString;
    PopUp *popup;
    QProcess *n_process;
    NetworkEvent netEvent;

};

#endif // PANEL_H
