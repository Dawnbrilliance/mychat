#ifndef CONNECT_H
#define CONNECT_H

#include <QList>
#include <QTimer>
#include <QObject>
#include <QUdpSocket>
#include <QNetworkInterface>

class client;
class connection : public QUdpSocket
{
    Q_OBJECT
public:
    connection(client *parent = 0);
    ~connection();
    void        startBroadcasting();
    bool    sendMessageToUser(const QHostAddress &sendaddress, quint16 sendPort, const QString &message);
private:
    void        updateAddress();
    bool        isLocalHostAddress(const QHostAddress &address);
    QByteArray  m_username;
signals:
    void                newConnect(const QHostAddress &host, quint16 port);
    void                removeConnect(const QHostAddress &senderIp, quint16 senderPort);
    void                newMessage(const QString &name, const QString &senderIp, quint16 sendPort, const QString &message);
public slots:
    void        sendBroadcastData();
    void        readBroadcastData();
private:
    QTimer              sendBoradTime;
    client              *m_client;
    QList<QHostAddress> m_broadercastAddress;
    QList<QHostAddress> m_ipAddress;
};

#endif // CONNECT_H
