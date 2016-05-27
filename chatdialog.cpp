#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "client.h"

chatDialog::chatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatDialog)
{
    ui->setupUi(this);
    m_client = new client(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &chatDialog::sendMessage);
}

chatDialog::~chatDialog()
{
    delete ui;
}

void chatDialog::removeConnect(const QHostAddress &senderIp, quint16 senderPort)
{
    for (int index=0; index < ui->listWidget->count(); index++) {
        QListWidgetItem *item = ui->listWidget->item(index);
        if (item != NULL
                && item->data(Qt::UserRole).toInt() == senderPort
                && item->data(Qt::DisplayRole).toString() == senderIp.toString())
        {
            QListWidgetItem *removeItem = ui->listWidget->takeItem(index);
            delete removeItem;
        }
    }
}

void chatDialog::addConnect(const QHostAddress &senderIp, quint16 senderPort)
{
    QVariant varIp = senderIp.toString();
    QVariant varPort = senderPort;
    QListWidgetItem *newItem = new QListWidgetItem(ui->listWidget);
    newItem->setData(Qt::UserRole, varPort);
    newItem->setData(Qt::DisplayRole, varIp);
    ui->listWidget->addItem(newItem);
}

void chatDialog::recvMessage(QString name, QString senderIp, qint16 sendPort, const QString &message)
{
    rendRecvMessage(name+":"+senderIp+":"+sendPort);
    rendRecvMessage(message);
}

void chatDialog::sendMessage(bool checked)
{
    QListWidgetItem *currentItem = ui->listWidget->currentItem();
    if (currentItem != NULL){
        QString sendMessage = ui->lineEdit->text();
        ui->lineEdit->clear();
        rendSendMessage(sendMessage);
        int sendToPort = currentItem->data(Qt::UserRole).toInt();
        QString sendToIp = currentItem->data(Qt::DisplayRole).toString();
        m_client->sendMessage("", QHostAddress(sendToIp), sendToPort, sendMessage);
    }
    return;
}

void chatDialog::rendSendMessage(const QString &text)
{
    ui->textEdit->setTextBackgroundColor(QColor(84, 192, 24));
    ui->textEdit->setTextColor(QColor(84, 192, 24));
    ui->textEdit->moveCursor(QTextCursor::Down);
    ui->textEdit->append(text);
}

void chatDialog::rendRecvMessage(const QString &text)
{
    ui->textEdit->setTextBackgroundColor(QColor(255, 255, 255));
    ui->textEdit->setTextColor(QColor(255, 255, 255));
    ui->textEdit->moveCursor(QTextCursor::Down);
    ui->textEdit->append(text);
}
