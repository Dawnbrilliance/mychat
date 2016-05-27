#include "client.h"
#include "chatdialog.h"
#include "connection.h"

client::client(QObject *parent) : QObject(parent)
{
    m_connect = new connection(this);
    m_View = (chatDialog*)parent;
    connect(m_connect, SIGNAL(newConnect(const QHostAddress &, quint16)), this, SLOT(addConnect(const QHostAddress &, quint16)));
    connect(m_connect, SIGNAL(removeConnect(QHostAddress &host, quint16 port)), this, SLOT(removeConnect(QHostAddress &host, quint16 port))),
    connect(m_connect, SIGNAL(newMessage(const QString &, const QString &, quint16, const QString &)), this, SLOT(newMessage(const QString &, const QString &, quint16, const QString &)));
    m_connect->startBroadcasting();
}

client::~client()
{

}

bool client::hasConnect(const QHostAddress &senderIp, int senderPort)
{
    if (m_peers.contains(senderIp))
    {
        QList<int> port =  m_peers.values(senderIp);
        foreach (int indexport, port) {
            if (indexport == senderPort)
                return true;
        }
    }
    return false;
}

void client::addConnect(const QHostAddress &senderIp, quint16 senderPort)
{
    m_peers.insert(senderIp, senderPort);
    m_View->addConnect(senderIp, senderPort);
}

void client::removeConnect(const QHostAddress &senderIp, quint16 senderPort)
{
    m_peers.remove(senderIp, senderPort);
    m_View->removeConnect(senderIp, senderPort);
}

bool client::sendMessage(QString name, QHostAddress senderIp, int sendPort, QString &message)
{
    return m_connect->sendMessageToUser(senderIp, sendPort, message);
}

void client::newMessage(const QString &name, const QString &senderIp, quint16 sendPort, const QString &message)
{
    m_View->recvMessage(name, senderIp, sendPort, message);
}

