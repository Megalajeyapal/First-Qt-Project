/*****************************************************************//**
 *
 * @file EventGenerator.cpp
 * @brief This class implements the EventGenerator UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#include "EventGenerator.h"
#include "ui_eventgenerator.h"
#include "maindialog.h"
#include <QObject>
#include <QDebug>
#include <QTimer>

//Static Variables
int EventGenerator::repeat = 0;
int EventGenerator::delay = 0;
int EventGenerator::count;
QString EventGenerator::ip = 0;


/*********************************************************//*
 * @brief   Constructor of the EventGenerator Class
 * @define  Initializes the variables of EventGenerator Class
 * ********************************************************/

EventGenerator::EventGenerator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EventGenerator)
{
    ui->setupUi(this);

    //Sets '0' as a default value in all data boxes

    ui->data_1->setValidator(new QIntValidator);
    ui->data_2->setValidator(new QIntValidator);
    ui->data_3->setValidator(new QIntValidator);
    ui->data_4->setValidator(new QIntValidator);
    ui->data_5->setValidator(new QIntValidator);
    ui->data_6->setValidator(new QIntValidator);
    ui->data_7->setValidator(new QIntValidator);
    ui->data_1->setText("0");
    ui->data_2->setText("0");
    ui->data_3->setText("0");
    ui->data_4->setText("0");
    ui->data_5->setText("0");
    ui->data_6->setText("0");
    ui->data_7->setText("0");
    //By default sets loop_id as 1
    ui->moduleId->setValue(1);



    udpSocket.bind(5401);
    //Calls Update Slot when readyRead signal is emitted
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(Update()));

    //Calls Error Slot when SocketError signal is emitted
    connect(&udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error()));

    m_timer = new QTimer(this);
    //Calls ProcessOneThing Slot when timeout signal is emitted
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ProcessTimer()));
}

void EventGenerator::ConnectingHost(QString ipAddress)
{
    ip = ipAddress;
    if(ipAddress!=NULL){
        udpSocket.connectToHost(ipAddress,5400);
    }else{
        udpSocket.connectToHost(QHostAddress::LocalHost,5400);
    }
}



EventGenerator::~EventGenerator()
{
    delete ui;
}

/*************************************************************************//*
 * @brief      To Send a same Event Multiple times
 * ************************************************************************/

void EventGenerator::MultipleSend()
{
   if (ui->cancelRepeatButton->text() == "Cancel Repeat")       //In Repeat mode
   {
       if (ui->repeatSpinbox->text() == "0"){               //No Repeat specified
               Send();                                  //Send once
       }
       else     //If Repeat specified
       {
            delay=ui->delayBox->text().toDouble()*1000;   //Get Delay in seconds
            repeat=ui->repeatSpinbox->text().toInt();       //Get Repeat value
            m_timer->start(delay);                        //Starts the timer with regular delay
       }
   }
   else                     //In Non-Repeat mode
   {
       Send();              //Send once
   }

}

/*************************************************************************//*
 * @brief      Sends a block of data to the Server through connected socket
 * @details     Writes a block of data into the socket and send it
 *                 to the server
 * ************************************************************************/

void EventGenerator::Send()
{
    qint64 numberwriitemn;
    if (!(QIODevice::NotOpen))                      //If QIODevice is open
    {
            std::string sendData=ui->eventsBox->currentText().toStdString().substr(2,5)+",";
            sendData += (ui->data_1->text()+",").toStdString();
            sendData += (ui->data_2->text()+",").toStdString();
            sendData += (ui->data_3->text()+",").toStdString();
            sendData += (ui->data_4->text()+",").toStdString();
            sendData += (ui->data_5->text()+",").toStdString();
            sendData += (ui->data_6->text()+",").toStdString();
            sendData += (ui->data_7->text()+",").toStdString();
            sendData +=(ui->moduleId->text()).toStdString();

            numberwriitemn=udpSocket.write(sendData.c_str());


            qDebug()<<"Bytes written"<<numberwriitemn;
    }
    else                                             //If QIODevice is not_open
    {
            ui->statusBox->setText(tr("Error: QIODevice Not Open"));       //Error Message
    }
}

/*************************************************************************//*
 * @brief      Changes the label name of Repeat Button from
 *              Repeat to Cancel_Repeat and viceversa
 * ************************************************************************/

void EventGenerator::MyRepeatClick()
{
    if (ui->cancelRepeatButton->text() == "Cancel Repeat")
    {
             ui->cancelRepeatButton->setText("Repeat");
    }
    else
    {
             ui->cancelRepeatButton->setText("Cancel Repeat");
    }
}



/************************************************************************//*
 * @brief       This slot is called when Connect button is clicked
 * @define      Once the signal is connected, inital data read from the file
 *              is sent to the server side
 ************************************************************************/
void EventGenerator :: InitialInput()
{
    if (!(QIODevice::NotOpen))                      //If QIODevice is open
    {
            std::string str = ui->customText->toPlainText().toStdString();

            udpSocket.write(str.c_str());

    }
    else                                             //If QIODevice is not_open
    {
            ui->statusBox->setText(tr("Error: QIODevice Not Open"));       //Error Message
    }

}

/*************************************************************************//*
 * @brief      Receives Data from Server and prints it in the Ui TextBox
 * ************************************************************************/

void EventGenerator :: Update()
{
    QByteArray block;                                   //To read a block of raw data from the socket
    //block = tcpSocket.readAll();                         //Reads all available data from the socket, and returns it as a QByteArray
    do {
           block.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(block.data(), block.size());
            qDebug()<<block.data();
       } while (udpSocket.hasPendingDatagrams());
    ui->logBox->setText(tr("Data Received\n")+block.data());
}


/*************************************************************************//*
 * @brief      This Slot is called whenever a SocketError signal is emitted
 * ************************************************************************/

void EventGenerator:: Error(){
    ui->statusBox->setText(udpSocket.errorString());   //Prints error message

}



/*************************************************************************//*
 * @brief      This Slot is called whenever a Timeout signal is emitted
 * ************************************************************************/

void EventGenerator::ProcessTimer()
{

    QString strVcount;
    if(repeat!=0)
    {
        Send();         //Sends the data

        /******************************************************//*
         * @details following if blocks enables sending
         *          burst of inputs to the same event
         * *****************************************************/

        if (ui->data_1Repeat->isChecked())                  //if data1 checkbox is clicked
        {
                count = ui->data_1->text().toInt();         //Gets data1 value
                strVcount = QString::number(count+1);       //Increments the data2 value by 1
                ui->data_1->setText(strVcount);             //sets the new value as data 1
         }
        if (ui->data_2Repeat->isChecked())                  //if data2 checkbox is clicked
        {
                count = ui->data_2->text().toInt();         //Gets data2 value
                strVcount = QString::number(count+1);       //Increments the data2 value by 1
                ui->data_2->setText(strVcount);             //sets the new value as data2
        }
        if (ui->data_3Repeat->isChecked())                  //if data3 checkbox is clicked
        {
                count = ui->data_3->text().toInt();         //Gets data3 value
                strVcount = QString::number(count+1);       //Increments the data3 value by 1
                ui->data_3->setText(strVcount);             //sets the new value as data3
        }
        if (ui->data_4Repeat->isChecked())                  //if data4 checkbox is clicked
        {
                count = ui->data_4->text().toInt();         //Gets data4 value
                strVcount = QString::number(count+1);       //Increments the data4 value by 1
                ui->data_4->setText(strVcount);             //sets the new value as data4
        }
        if (ui->data_5Repeat->isChecked())                  //if data5 checkbox is clicked
        {
                count = ui->data_5->text().toInt();         //Gets data5 value
                strVcount = QString::number(count+1);       //Increments the data5 value by 1
                ui->data_5->setText(strVcount);             //sets the new value as data5
        }
        if (ui->data_6Repeat->isChecked())                  //if data6 checkbox is clicked
        {
                count = ui->data_6->text().toInt();         //Gets data6 value
                strVcount = QString::number(count+1);       //Increments the data6 value by 1
                ui->data_6->setText(strVcount);             //sets the new value as data6
        }
        if (ui->data_7Repeat->isChecked())                 //if data7 checkbox is clicked
        {
                count = ui->data_7->text().toInt();         //Gets data7 value
                strVcount = QString::number(count+1);       //Increments the data7 value by 1
                ui->data_7->setText(strVcount);             //sets the new value as data7
        }
        repeat --;                  //decrements the repeat value by 1
        m_timer->start(delay);        //Again Starts the timer
    }
    else
    {
        //all repeats are processed
    }
}

                //Ui Slots definition Starts

/*************************************************************************//*
 * @brief      Repeat Button Slot
 * @details     This Slot is Called Whenever Repeat Button
 *                  is clicked
 * ************************************************************************/

void EventGenerator::on_cancelRepeatButton_clicked()
{
    if (ui->cancelRepeatButton->text() == "Cancel Repeat"){
           ui->repeatSpinbox->setDisabled(true);
           ui->delayBox->setDisabled(true);
           MyRepeatClick();
     }
    else    //if (ui->cancelRepeatButton->text() == "Repeat")
    {
           ui->repeatSpinbox->setEnabled(true);
           ui->delayBox->setEnabled(true);
           MyRepeatClick();
    }
}

/*************************************************************************//*
 * @brief       Load_Config_File Button Slot
 * @details     Once this Slot is executed, Ui will prompt to select a
 *              config file, and copies the data from the selected file
 *              to Custom_text textbox in the Ui as comma separated string
 * ************************************************************************/

void EventGenerator::on_loadButton_clicked()
{
    QString path;
    m_fd = new QFileDialog;

    //prompt to select a file and returns the path of the selected file
    path =m_fd->getOpenFileName(this,tr("Open Config File"), "/home", tr("All files (*.*);;Text files (*.txt)"));
    QFile f(path);
    f.open(QIODevice::ReadOnly);        //Opens the file in read-only mode
    QByteArray total;
    QByteArray line;
    while (!f.atEnd())
    {
        line = f.read(1024);            //reads data from file and stores it in bytearray
        total.append(line);             //appends the data as a single bytearray
    }

    /************************************************************************//*
     * @details     following line will replace all the newline characters
     *              from the total string to commas and append null character
     *              at the end of the string
     *              Finally prints it in the Custom_text textbox in the Ui
     * ************************************************************************/

    ui->customText->setText(QString(total).replace("\n", ",").left(QString(total).length()-1) + ("\0"));

    //udpSocket.write((QString(total).replace("\n", ",").left(QString(total).length()-1) + ("\0")).toStdString().c_str());

  }


/*************************************************************************//*
 * @brief      Send Button Slot
 * @details     This Slot is Called Whenever Send Button
 *                  is clicked
 * ************************************************************************/

void EventGenerator::on_sendButton_clicked()
{
    MultipleSend();         //Enables sending a Event multiple times
}


/*************************************************************************//*
 * @brief       Exit Button Slot
 * @details     This Slot is Called Whenever Exit Button
 *                  is clicked
 * ************************************************************************/

void EventGenerator::on_exitButton_clicked()
{

    udpSocket.write("Connection Closed!");

//    }
    this->close();          //Closes the Mainwindow
}



/*************************************************************************//*
 * @brief      Gets the loop id label according to the selection
 * @details     Id's may be any one of the following three
 *              - Loop Id
 *              - Network Id
 *              - I/O Module Id
 * @param[in]    args    : QString &labelName
 **************************************************************************/

void EventGenerator::on_moduleType_activated(const QString &labelName)
{
   ui->label_7->setText(labelName);
}



void EventGenerator::on_connectButton_clicked()
{
    InitialInput();
}
