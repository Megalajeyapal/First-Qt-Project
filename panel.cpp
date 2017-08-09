/*****************************************************************//*
 * @file        Panel.cpp
 * @brief       This class implements the Panel UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#include "panel.h"
#include "ui_panel.h"
#include "maindialog.h"
#include <QUdpSocket>
#include <sstream>
#include <iostream>

//Static variable
std::string Panel::fileString;

/*************************************************//*
 * @brief       Panel constructor
 * @define      Initializes class objects
 *
 * *************************************************/
Panel::Panel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);

    popup = new PopUp;      //PopUp class object

    ui->module_1->hide();
    ui->module_2->hide();
    ui->module_3->hide();
    ui->module_4->hide();
    ui->module_5->hide();
    ui->module_6->hide();
    ui->module_7->hide();
    ui->module_8->hide();
    ui->module_9->hide();
    ui->module_10->hide();
    ui->module_11->hide();
    ui->module_12->hide();
    ui->module_13->hide();
    ui->module_14->hide();
    ui->module_15->hide();
    ui->module_16->hide();


    udpSocket.bind(5401);       //Socket binding with specific port
   // udpSocket.BindMode(QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);


    //Calls Update Slot when readyRead signal is emitted
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(Update()));

    //Calls Error Slot when SocketError signal is emitted
    connect(&udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error()));

}


/*******************************************************//*
 * @brief       Connecting to Host
 * @define      This method try to connect with the specified IP address
 *               if the IP address is not specified then It connects with
 *               the local IP
 *
 * @param[in]   args:   QString ipAddress
 * *******************************************************/

void Panel::ConnectingHost(QString ipAddress)
{
    qDebug()<<"Connecting to Host";
    if(ipAddress != NULL){
        udpSocket.connectToHost(ipAddress,5400);
    }else{
        udpSocket.connectToHost(QHostAddress::LocalHost,5400);
    }

    n_process = new QProcess(this);

    //n_process->startDetached("/home/megala/Downloads/HMI/HMI --so portrait");
    n_process->startDetached("./HMI --so portrait");

}

/********************************************//*
 * @brief       Destructor
 * @define      Deletes the objects
 * *********************************************/

Panel::~Panel()
{
    delete n_process;
    n_process = NULL;
    delete ui;
}


void Panel:: Error()
{
    ui->module_2->setText(udpSocket.errorString());   //Prints error message

}

/***************************************************************//*
 * @brief       Switch ON the LED based on the active module status
 * @define      This method provides 4 different LED's to show the
 *               status of the active module say
 *                  module_status 0 --> Connected(green)
 *                  module_status 1 --> Waiting for a response
 *                                          (blue)
 *                  module_status 2 --> Disconnected(red)
 *                  module_status 3 --> Fault (orange)
 *
 * @param[in]   args:  QLineEdit *led, QString moduleStatus
 *
 * *************************************************************/

void Panel :: Led(QLineEdit *led, QString moduleStatus)
{
    QString ledColor;
    QString stylesheet;
    if(moduleStatus == "0"){
        ledColor = "green";
    }else if(moduleStatus == "1"){
        ledColor = "cyan";
    }else if(moduleStatus == "2"){
        ledColor = "red";
    }else{
        ledColor = "orange";
    }

    stylesheet = "background-color:";
    stylesheet.append(ledColor);
    stylesheet.append(";");

    led->setStyleSheet(stylesheet);
}

/*****************************************************************//*
 * @brief      receives event from the module and stores its
 *              details in the form of "moduleId.csv" file
 *
 * @param[in]   args:  QString moduleId, QString data
 * ****************************************************************/

void Panel :: WriteFile(QString moduleId, QString data)
{

        bool eventCodeFound=0;
        QFile file(moduleId + ".csv");          //opens "moduleId.csv" file
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);

        QStringList sList;
        sList = data.split(",");

        //gets the hex value of the event code
        QString eventCode = sList[3];
        QString eventCodeAsHex = QString("%1").arg(eventCode.toInt(), 0, 16);

        //maps event code with its event name by parsing through eventlist.csv file


        QStringList fileParser;

        //opens file in read-only mode

        QFile f("eventlist.csv");
        qDebug()<<"Reading File " << f.fileName();
        f.open(QIODevice::ReadOnly);
        QString line;

        //file parsing

        while (!f.atEnd())
        {
            line = f.readLine();
            fileParser=line.split(",");
            if(fileParser[0] == eventCodeAsHex){
                eventCodeFound=1;
                 break;
            }
        }

       if(eventCodeFound == 1){
        qDebug() <<eventCodeAsHex<< "Event name"<<fileParser[1];

        //writes the module details with event_name into "moduleId.csv" file

        out << sList[0]<< "\t"  << sList[1] << "\t" << sList[2] <<"\t"
             << eventCodeAsHex << "\t" << fileParser[1] << "\t" << sList[4]
            <<"\t" <<sList[5] <<"\t"<< sList[6] << "\t" <<sList[7] << "\t" <<sList[8] << "\t" << sList[9]
           <<"\t" << sList[10] ;

        qDebug()<<"Module details are stored in "<<file.fileName();
        ui->serverMsgBox->setText("Module "+moduleId+" details are stored in "+file.fileName());
        }else{
            qDebug()<<"Given EventCode is not found in eventlist filefor module "<<moduleId <<", provide valid EventCode";
            ui->serverMsgBox->setText("Given EventCode is not found in eventlist file for module "+moduleId+", provide valid EventCode");
        }

        file.close();

}


/******************************************************************//*
 * @brief       receives events from the active module
 *
 * @detail      This method splits moduletype, moduleId, moduleStatus
 *               from the data received from the module and update it
 *               in the UI and calls WriteFile function
 *********************************************************************/

void Panel :: Update()
{
    qDebug()<<"Ready Read signal";
    QByteArray block;
    int i=0;
    if(udpSocket.hasPendingDatagrams()){
    while (udpSocket.hasPendingDatagrams()){
           block.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(block.data(), block.size());
            qDebug()<<"has pending datagrams"<<udpSocket.hasPendingDatagrams()<<i++;



    QStringList slist;

    QString data = block.data();
    qDebug()<<data;
    ui->serverMsgBox->setText(data);

    //splits moduletype, moduleId, moduleStatus from the data received from the module
    slist = data.split(",");
    QString moduleType = slist[0];      //gets Module Type
    QString moduleId = slist[1];        //gets Module Id
    QString moduleStatus = slist[2];    //gets Module Status



    if((slist.end() - slist.begin())>3){

        QString code = slist[3];
        QString data_1 = slist[4];
        QString data_2 = slist[5];
        QString data_3 = slist[6];
        QString data_4 = slist[7];
        QString data_5 = slist[8];
        QString data_6 = slist[9];
        QString data_7 = slist[10];

        qDebug()<<"code value in update function is "<<code;
        
        //sending Events to Network Interface
        netEvent.sendEvent(code.toUInt(), data_1.toUInt(), data_2.toUInt() , data_3.toUInt(), data_4.toUInt(),
                       data_5.toUInt() , data_6.toUInt(),data_7.toUInt() ,"Custom text");

         WriteFile(moduleId,data);       //Writes module data into a .csv file with respective moduleId as its name.

    }else{

        qDebug()<<"code value in update function is null";
    }
   switch (moduleId.toInt()) {

    case 01:

       ui->module_1->setText(moduleType);
       ui->module_1->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_1,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_1,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_1,moduleStatus);
       }else{
            Led(ui->orange_1,moduleStatus);
       }

       break;

    case 02:

       ui->module_2->setText(moduleType);
       ui->module_2->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_2,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_2,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_2,moduleStatus);
       }else{
            Led(ui->orange_2,moduleStatus);
       }

       break;

    case 03:

       ui->module_3->setText(moduleType);
       ui->module_3->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_3,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_3,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_3,moduleStatus);
       }else{
            Led(ui->orange_3,moduleStatus);
       }

       break;

    case 04:

       ui->module_4->setText(moduleType);
       ui->module_4->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_4,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_4,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_4,moduleStatus);
       }else{
            Led(ui->orange_4,moduleStatus);
       }

       break;
    case 05:

       ui->module_5->setText(moduleType);
       ui->module_5->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_5,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_5,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_5,moduleStatus);
       }else{
            Led(ui->orange_5,moduleStatus);
       }

       break;

    case 06:

       ui->module_6->setText(moduleType);
       ui->module_6->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_6,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_6,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_6,moduleStatus);
       }else{
            Led(ui->orange_6,moduleStatus);
       }

       break;

    case 07:

       ui->module_7->setText(moduleType);
       ui->module_7->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_7,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_7,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_7,moduleStatus);
       }else{
            Led(ui->orange_7,moduleStatus);
       }
       break;

    case 10:

       ui->module_10->setText(moduleType);
       ui->module_10->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_10,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_10,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_10,moduleStatus);
       }else{
            Led(ui->orange_10,moduleStatus);
       }
       break;

    case 11:

       ui->module_11->setText(moduleType);
       ui->module_11->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_11,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_11,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_11,moduleStatus);
       }else{
            Led(ui->orange_11,moduleStatus);
       }

       break;

    case 12:

       ui->module_12->setText(moduleType);
       ui->module_12->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_12,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_12,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_12,moduleStatus);
       }else{
            Led(ui->orange_12,moduleStatus);
       }

       break;

    case 13:

       ui->module_13->setText(moduleType);
       ui->module_13->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_13,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_13,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_13,moduleStatus);
       }else{
            Led(ui->orange_13,moduleStatus);
       }

       break;

    case 14:

       ui->module_14->setText(moduleType);
       ui->module_14->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_14,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_14,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_14,moduleStatus);
       }else{
            Led(ui->orange_14,moduleStatus);
       }
       break;

    case 15:

       ui->module_15->setText(moduleType);
       ui->module_15->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_15,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_15,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_15,moduleStatus);
       }else{
            Led(ui->orange_15,moduleStatus);
       }

       break;
    case 16:

       ui->module_16->setText(moduleType);
       ui->module_16->show();

       //Based on module status, Led switch on
       if(moduleStatus == "0"){
           Led(ui->green_16,moduleStatus);
       }else if(moduleStatus == "1"){
           Led(ui->blue_16,moduleStatus);
       }else if(moduleStatus == "2"){
            Led(ui->red_16,moduleStatus);
       }else{
            Led(ui->orange_16,moduleStatus);
       }

       break;

    default:
        if(moduleId=="08"){

            ui->module_8->setText(moduleType);
            ui->module_8->show();

            //Based on module status, Led switch on
            if(moduleStatus == "0"){
                Led(ui->green_8,moduleStatus);
            }else if(moduleStatus == "1"){
                Led(ui->blue_8,moduleStatus);
            }else if(moduleStatus == "2"){
                 Led(ui->red_8,moduleStatus);
            }else{
                 Led(ui->orange_8,moduleStatus);
            }

            break;
        }else if(slist[1]=="09"){

            ui->module_9->setText(moduleType);
            ui->module_9->show();

            //Based on module status, Led switch on
            if(moduleStatus == "0"){
                Led(ui->green_9,moduleStatus);
            }else if(moduleStatus == "1"){
                Led(ui->blue_9,moduleStatus);
            }else if(moduleStatus == "2"){
                 Led(ui->red_9,moduleStatus);
            }else{
                 Led(ui->orange_9,moduleStatus);
            }

            break;
        }else{
            break;
        }
    }
    }
    }else{
        qDebug()<<"No Data found from the server";
        ui->serverMsgBox->setText("Error: No Data found from the server");
    }
}

/*************************************************************//*
 * @brief       reads the given file and update it in the popup dialog
 *
 * @define      This method opens the file which holds the events sent
 *               by the module, read the data and copies it to the popup
 *
 * @param[in]   args:   QString fileName
 * ***************************************************************/

void Panel::FileRead(QString fileName)
{
    //opens file in read_only mode

    m_fd = new QFileDialog;
    QFile f(fileName);
    qDebug()<<"Reading File " << fileName;
    f.open(QIODevice::ReadOnly);        //Opens the file in read-only mode

    //file parsing

    QString line;
    QStringList fileParser;
    qDebug()<<f.isOpen();
    if(!f.atEnd()){
    while (!f.atEnd())
    {

        line = f.readLine();
        fileParser=line.split("\t");

    }

    //updates the file contents in the popup
    popup->SetText(fileParser);
    popup->show();
    }else{
        qDebug()<<"Module File is Empty";
        ui->serverMsgBox->setText("Error: Module File is empty");
    }

}

//loads log files of all 16 modules

void Panel::on_logButton_1_clicked()
{

    FileRead("01.csv");     //opens Module_1 file
}

void Panel::on_logButton_2_clicked()
{
    FileRead("02.csv");     //opens Module_2 file
}

void Panel::on_logButton_3_clicked()
{
    FileRead("03.csv");     //opens Module_3 file
}

void Panel::on_logButton_4_clicked()
{
    FileRead("04.csv");     //opens Module_4 file
}

void Panel::on_logButton_5_clicked()
{
    FileRead("05.csv");     //opens Module_5 file
}

void Panel::on_logButton_6_clicked()
{
    FileRead("06.csv");     //opens Module_6 file
}

void Panel::on_logButton_7_clicked()
{
    FileRead("07.csv");     //opens Module_7 file
}

void Panel::on_logButton_8_clicked()
{
    FileRead("08.csv");     //opens Module_8 file
}

void Panel::on_logButton_9_clicked()
{
    FileRead("09.csv");     //opens Module_9 file
}

void Panel::on_logButton_10_clicked()
{
    FileRead("10.csv");     //opens Module_10 file
}

void Panel::on_logButton_11_clicked()
{
    FileRead("11.csv");     //opens Module_11 file
}

void Panel::on_logButton_12_clicked()
{
    FileRead("12.csv");     //opens Module_12 file
}

void Panel::on_logButton_13_clicked()
{
    FileRead("13.csv");     //opens Module_13 file
}

void Panel::on_logButton_14_clicked()
{
    FileRead("14.csv");     //opens Module_14 file
}

void Panel::on_logButton_15_clicked()
{
    FileRead("15.csv");     //opens Module_15 file
}

void Panel::on_logButton_16_clicked()
{
    FileRead("16.csv");     //opens Module_16 file
}

/****************************************************************//*
 * @brief       starts detecting modules
 *
 * @define      This method sends the content of config_file(initially loaded)
 *               to the server and starts receiving the modules status
 * ****************************************************************/

void Panel::on_detectButton_clicked()
{

    if(udpSocket.isOpen()){
    udpSocket.write(fileString.c_str());
    std::string delimiter = ",";
    int i=0;
    size_t pos = 0;

    //parses the config_file and updates the panel number
    while ((pos = fileString.find(delimiter)) != std::string::npos) {
        i++;
        if(i==2){
        ui->panelNumber->setAlignment(Qt::AlignCenter);
        ui->panelNumber->setText(fileString.substr(0, pos).c_str());
        }
        fileString.erase(0, pos + delimiter.length());
    }

    ui->detectButton->setStyleSheet("background: grey; font-size: 16px");
    ui->detectButton->setEnabled(false);
    }else{
        qDebug()<<"Socket not open";
    }

}

/****************************************************************//*
 * @brief       close button slot
 *
 * @define      This method closes the UI, thereby ends the connection
 *                  with the server
 * ***************************************************************/

void Panel::on_closeButton_clicked()
{
    //closes HMI application
    n_process->terminate();
    n_process->close();

    //closes current application
    this->close();

}

/*************************************************************************//*
 * @brief       Load_Config_File Button Slot
 *
 * @details     This method will prompt to select a
 *              config file, and copies the data from the selected file
 *              to Custom_text textbox in the Ui as a comma separated string
 * ************************************************************************/

void Panel::on_loadButton_clicked()
{
    QString path;
    QByteArray fileData;

    //prompt to select a file and returns the path of the selected file

    m_fd = new QFileDialog;
    path =m_fd->getOpenFileName(this,tr("Open Config File"), "/home", tr("All files (*.*);;Text files (*.txt)"));
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QByteArray line;


    //file parsing
    if(!f.atEnd()){
    while (!f.atEnd())
    {
        line = f.readAll();
        fileData.append(line);             //appends the data as a single bytearray
    }

     fileString = (QString(fileData).replace("\n", ",").left(QString(fileData).length()-1) + ("\0")).toStdString();

     if(fileString != ""){
        qDebug()<<"Config File Successfully loaded";
        ui->serverMsgBox->setText("Config file loaded successfully");
     }else{
         qDebug()<<"No data found in config file";
         ui->serverMsgBox->setText("Error: No data found in config file");
     }

    }else{
        qDebug()<<"Config File is empty";
        ui->serverMsgBox->setText("Error: Config file is empty");
    }

}

/*********************************************************************//*
 * @brief       Disconnect button slot
 *
 * @define      This method sends disconnect/connect message to the server
 *               when the button is clicked
 * *******************************************************************/
void Panel::on_disConnectButton_clicked()
{

    if(ui->disConnectButton->text() == "Disconnect")
    {
        //sends disconnect message and toggles to connect button
        udpSocket.write("Disconnect");
        ui->disConnectButton->setText("Connect");
    }
    else if(ui->disConnectButton->text() == "Connect")
    {
        //sends connect message and toggles to disconnect button
        udpSocket.write("Connect");
        ui->disConnectButton->setText("Disconnect");
    }

}

