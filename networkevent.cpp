#include "NetworkEvent.h"
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QDateTime>
//#include "EComMsg.h"

NetworkEvent::NetworkEvent()
{
    m_server = new QTcpServer();
    connect(m_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    if (m_server->listen(QHostAddress::Any, 9000))
    {
        qDebug() << "*****HMI Tcp Server Active*******";
    }
    else
    {
        qDebug() << "********HMI Tcp Server Not Created*****";
    }
}

void NetworkEvent::acceptConnection()
{
    QTcpSocket* socket = (QTcpSocket*)m_server->nextPendingConnection();

    if (socket != NULL)
    {
        qDebug() << "Connection from" << socket->peerAddress() << "accepted.";
        m_activeSockets << socket;
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(removeConnection()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(removeConnection()));
    }
    else
    {
        qDebug() << "**********Network Socket Needed********";
    }

}

void NetworkEvent::removeConnection()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    qDebug() << "Removing a failed or disconnected socket from" << socket->peerAddress();
    m_activeSockets.removeAll(socket);
    socket->deleteLater();
}

void NetworkEvent::sendEvent(eCommEvent &event, QString& text)
{

    const QDateTime now = QDateTime::currentDateTime();

    event.timestamp.second = now.time().second();
    event.timestamp.minute = now.time().minute();
    event.timestamp.hour = now.time().hour();
    event.timestamp.day = now.date().day();
    event.timestamp.month = now.date().month();
    event.timestamp.year = now.date().year() - 2000;
/**********************************************doubt***********************************************/
    //Header + Event data
    QByteArray data = QByteArray::fromHex(QString("01 81 92 FF 55 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 %1 02")
                                                .arg(sizeof(event) + text.length(), 2, 16, QChar('0')).toLatin1())+ //Header
                      QByteArray::fromRawData((char*)(&event), sizeof(event));                 //Event data

/**********************************************doubt***********************************************/
    if (text.length() > 0)
    {
        data += text;
    }
//    QString code = QByteArray::fromRawData((char*)(&event.code), sizeof(event.code)).toHex();
//    QString data_1 = QByteArray::fromRawData((char*)(&event.data1), sizeof(event.data1)).toHex();
//    QString data_2 = QByteArray::fromRawData((char*)(&event.data2), sizeof(event.data2)).toHex();
//    QString data_3 = QByteArray::fromRawData((char*)(&event.data3), sizeof(event.data3)).toHex();
//    QString data_4 = QByteArray::fromRawData((char*)(&event.data4), sizeof(event.data4)).toHex();
//    QString data_5 = QByteArray::fromRawData((char*)(&event.data5), sizeof(event.data5)).toHex();
//    QString data_6 = QByteArray::fromRawData((char*)(&event.data6), sizeof(event.data6)).toHex();
//    QString data_7 = QByteArray::fromRawData((char*)(&event.data4), sizeof(event.data4)).toHex();

    foreach (QTcpSocket* socket, m_activeSockets)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            qDebug() << "Sending event to" << socket->peerAddress();
            qDebug() << "code : "<<QByteArray::fromRawData((char*)(&event.code), sizeof(event.code)).toHex();
            qDebug() << "Data_1 : "<<QByteArray::fromRawData((char*)(&event.data1), sizeof(event.data1)).toHex();
            qDebug() << "Data_2 : "<<QByteArray::fromRawData((char*)(&event.data2), sizeof(event.data2)).toHex();
            qDebug() << "Data_3 : "<<QByteArray::fromRawData((char*)(&event.data3), sizeof(event.data3)).toHex();
            qDebug() << "Data_4 : "<<QByteArray::fromRawData((char*)(&event.data4), sizeof(event.data4)).toHex();
            qDebug() << "Data_5 : "<<QByteArray::fromRawData((char*)(&event.data5), sizeof(event.data5)).toHex();
            qDebug() << "Data_6 : "<<QByteArray::fromRawData((char*)(&event.data6), sizeof(event.data6)).toHex();
            qDebug() << "Data_7 : "<<QByteArray::fromRawData((char*)(&event.data7), sizeof(event.data7)).toHex();

            qDebug() << "data before sending to HMI is" << data;


            socket->write(data);
        }
        else
        {
            qDebug() << "Unable to send event to" << socket->peerAddress() << ", no longer connected.";
        }
    }
}

void NetworkEvent::sendEvent(quint16 code, quint16 data1, quint16 data2, quint16 data3, quint16 data4, quint16 data5, quint16 data6, quint16 data7, QString text)
{
    eCommEvent event;
    memset(&event, 0, sizeof(event));
    event.code = code;
    event.data1 = data1;
    event.data2 = data2;
    event.data3 = data3;
    event.data4 = data4;
    event.data5 = data5;
    event.data6 = data6;
    event.data7 = data7;
    sendEvent(event, text);
}

void NetworkEvent::receiveEvent()
{
    qDebug("*********Receive Event****");
    //    EComMsg eComEvent;
    /*QByteArray m_receiveBuffer;
    qint8 check=0x66;
    qDebug()<<"Bytes Availble"<<socket->bytesAvailable();
    m_receiveBuffer=socket->readAll();
    int temp=(int)m_receiveBuffer[7];

    qDebug("******In receive*****");
    qDebug()<<temp;
    if(m_receiveBuffer[7]==check)
        qDebug("********reset received********");*/
}
