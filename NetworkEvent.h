#ifndef NETWORKEVENT_H
#define NETWORKEVENT_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QList>

/************************************************//*
 *  @class      NetworkEvent
 *  @brief      NetworkEvent class declaration
 *************************************************/


class NetworkEvent : public QObject
{
    Q_OBJECT

public:
    typedef struct
    {
        quint16 code;    /*!< Ecomm Msg Code*/
        quint16 data1;
        quint16 data2;
        quint16 data3;
        quint16 data4;
        quint16 data5;
        quint16 data6;
        quint16 data7;
        struct
        {
            quint8 second;
            quint8 minute;
            quint8 hour;
            quint8 day;
            quint8 month;
            quint8 year;
        }timestamp;
    }eCommEvent;

    NetworkEvent();

    void sendEvent(eCommEvent& event, QString& text);

    Q_INVOKABLE void sendEvent(quint16 code, quint16 data1, quint16 data2, quint16 data3,
                               quint16 data4, quint16 data5, quint16 data6, quint16 data7,
                               QString text);

private slots:
    void acceptConnection();
    void removeConnection();
    void receiveEvent();

private:
    QTcpServer* m_server;
    QList<QTcpSocket*> m_activeSockets;
};

#endif // NETWORKEVENT_H
