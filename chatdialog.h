#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class chatDialog;
}

class client;
class chatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chatDialog(QWidget *parent = 0);
    void            removeConnect(const QHostAddress &senderIp, quint16 senderPort);
    void            addConnect(const QHostAddress &senderIp, quint16 senderPort);
    void            recvMessage(QString name, QString senderIp, qint16 sendPort, const QString &message);
    ~chatDialog();
private slots:
    void            sendMessage(bool checked);
private:
    Ui::chatDialog *ui;
    client          *m_client;

    void            rendRecvMessage(const QString &recvString);
    void            rendSendMessage(const QString &SendSting);
};

#endif // CHATDIALOG_H
