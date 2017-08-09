/*****************************************************************//*
 *
 * @file        Eventgenerator.h
 * @brief       This class implements the EventGenerator UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <QMainWindow>
#include<QDialog>
#include<QMessageBox>
#include<QtWidgets>
#include<QFileDialog>
#include<QtNetwork>
#include <QUdpSocket>
#include<QTimer>

namespace Ui {
class EventGenerator;
}

/************************************************//*
 *  @class      EventGenerator
 *  @brief      EventGenerator class declaration
 *************************************************/

class EventGenerator : public QMainWindow
{
    Q_OBJECT

public:

    //Public Methods declaration begins

    explicit EventGenerator(QWidget *parent = 0);
    EventGenerator(QString ipAddress, QWidget *parent = 0);
    ~EventGenerator();
    void ConnectingHost(QString ipAddress);


private slots:    

    //Private Slots declaration begins

    void MyRepeatClick();
    void Send();
    void MultipleSend();
    void Update();
    void Error();
    void ProcessTimer();
    void InitialInput();

    //Slots of EventGenerator Ui Widgets

    void on_cancelRepeatButton_clicked();
    void on_moduleType_activated(const QString &labelName);
    void on_loadButton_clicked();
    void on_sendButton_clicked();
    void on_exitButton_clicked();
    void on_connectButton_clicked();

private:

    //Private Attributes declaration begins

    Ui::EventGenerator *ui;
    QFileDialog *m_fd;                //Object for QFileDialog Class
    QUdpSocket udpSocket;             //Object for QtcpSocket Class
    QTimer *m_timer;                  //Object for QTimer class
    static int delay;
    static int repeat;
    static int count;
    static QString ip;

};

#endif // EVENTGENERATOR_H
