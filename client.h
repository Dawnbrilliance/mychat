#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QList>
#include <QHostAddress>

class connection;
class chatDialog;
class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    ~client();
    bool        hasConnect(const QHostAddress &senderIp, int senderPort);
    bool        sendMessage(QString name, QHostAddress senderIp, int sendPort, QString &message);
signals:

public slots:
    void            removeConnect(const QHostAddress &senderIp, quint16 senderPort);
    void            addConnect(const QHostAddress &senderIp, quint16 senderPort);
    void            newMessage(const QString &name, const QString &senderIp, quint16 sendPort, const QString &message);
private:
    connection                          *m_connect;
    chatDialog                          *m_View;
    QMultiHash<QHostAddress, int>        m_peers;
};

#endif // CLIENT_H
