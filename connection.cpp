#include <QtNetwork>
#include "connection.h"
#include "client.h"

static const qint32 BroadcastInterval = 20000;
static const unsigned broadcastPort = 45000;

connection::connection(client *parent)
    :QUdpSocket(parent), m_client(parent)
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1) {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2) {
                m_username = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    if (m_username.isEmpty())
        m_username = "unknown";

    updateAddress();
    bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                             | QUdpSocket::ReuseAddressHint);

    connect(this, SIGNAL(readyRead()),
            this, SLOT(readBroadcastData()));
    sendBoradTime.setInterval( BroadcastInterval );
    connect(&sendBoradTime, SIGNAL(timeout()),
            this, SLOT(sendBroadcastData()));
}

connection::~connection()
{

}

void connection::startBroadcasting()
{
    sendBoradTime.start();
}

void connection::sendBroadcastData()
{
    QByteArray sendData(m_username);
    sendData.append("@");
    sendData.append("LOGIN");
    sendData.append("@");
    sendData.append(QByteArray::number(broadcastPort));

    bool validBroadcastAddresses = true;
    foreach(QHostAddress address, m_broadercastAddress)
    {
        if (writeDatagram(sendData, address, broadcastPort) == -1)
        {
            validBroadcastAddresses = false;
        }
    }
    if (!validBroadcastAddresses){
        updateAddress();
    }
}

bool connection::sendMessageToUser(const QHostAddress &sendaddress, quint16 sendPort, const QString &message)
{
    QByteArray sendData(m_username);
    sendData.append("#");
    sendData.append(sendaddress.toString());
    sendData.append("#");
    sendData.append(QByteArray::number(sendPort));
    sendData.append("#");
    sendData.append(message);

    if (writeDatagram(sendData, sendaddress, sendPort) == -1)
    {
        return false;
    }
    return true;
}

void connection::readBroadcastData()
{
    while(hasPendingDatagrams()){
        quint16 senderPort;
        QHostAddress senderIp;
        QByteArray readData;
        readData.resize(pendingDatagramSize());
        if (readDatagram(readData.data(), readData.size(),
                     &senderIp, &senderPort) == -1)
            continue;

//        if (isLocalHostAddress(senderIp) && senderPort == broadcastPort)
//            continue;

        QList<QByteArray> list = readData.split('@');
        if (list.size() == 3)
        {
            if (list.at(1) == "LOGIN" && !m_client->hasConnect(senderIp, senderPort)){
                emit newConnect(senderIp, senderPort);
            }
            if (list.at(1) == "OUT" && m_client->hasConnect(senderIp, senderPort)){
                emit removeConnect(senderIp, senderPort);
            }
            continue;
        }
        QList<QByteArray> message = readData.split('#');
        if (message.size() == 4)
        {
            emit newMessage(message.at(0), senderIp.toString(), senderPort, message.at(3));
        }
    }
}

bool connection::isLocalHostAddress(const QHostAddress &address)
{
    foreach (QHostAddress localAddress, m_ipAddress) {
        if (address == localAddress)
            return true;
    }
    return false;
}

void connection::updateAddress()
{
    m_broadercastAddress.clear();
    m_ipAddress.clear();
    foreach(QNetworkInterface networkInterface, QNetworkInterface::allInterfaces()){
        foreach (QNetworkAddressEntry Entry, networkInterface.addressEntries()) {
            if(Entry.broadcast() != QHostAddress::Null && Entry.ip() != QHostAddress::LocalHost){
                m_broadercastAddress << Entry.broadcast();
                m_ipAddress << Entry.ip();
            }
        }
    }
}
