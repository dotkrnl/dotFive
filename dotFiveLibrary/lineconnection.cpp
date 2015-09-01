#include "lineconnection.h"

#include <QDebug>

LineConnection::LineConnection(QTcpSocket *socket,
                               int heartbeat, int timeout,
                               QObject *parent)
    : QObject(parent),
      m_socket(socket),
      m_heartbeat_timer(new QTimer(this)),
      m_timeout_timer(new QTimer(this))
{
    m_id = m_socket->socketDescriptor();
    qDebug() << m_id << " object created";
    qInfo()  << m_id << " connected";

    if (heartbeat) {
        m_heartbeat_timer->setInterval(heartbeat);
        m_heartbeat_timer->start();
        connect(m_heartbeat_timer, SIGNAL(timeout()),
                this, SLOT(heartbeatTimeout()));
    }

    if (timeout) {
        m_timeout_timer->setInterval(timeout);
        m_timeout_timer->start();
        connect(m_timeout_timer, SIGNAL(timeout()),
                this, SLOT(connectionTimeout()));
    }

    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()));

    connect(m_socket, SIGNAL(disconnected()),
            this, SLOT(disconnectedEvent()));
    connect(m_socket, SIGNAL(disconnected()),
            this, SIGNAL(disconnected()));
}

LineConnection::~LineConnection(void)
{
    qDebug() << m_id << " object deleted";
}

void LineConnection::sendCommand(QString command, QStringList argv)
{
    QString data = command % " " % argv.join(" ") % "\r\n";
    qDebug() << m_id << " to which send:" << data;

    m_socket->write(data.toUtf8());
}

void LineConnection::disconnect(void)
{
    m_timeout_timer->stop();
    m_socket->close();
}

void LineConnection::readyRead()
{
    m_timeout_timer->start();

    if (!m_socket->canReadLine()) return;
    emitLine(QString::fromUtf8(m_socket->readLine()));
}

void LineConnection::emitLine(QString line)
{
    QStringList got = line.trimmed().split(" ");

    QString command = got.front();
    got.pop_front();

    if (command == "") {
        qInfo() << m_id << " from which got nothing";
    } else if (command == "HEARTBEAT") {
        qInfo() << m_id << " from which got heartbeat";
    } else {
        qDebug() << m_id << " from which got:" << command;
        if (got.size()) qDebug() << m_id << " argv:" << got;
        emit receivedCommand(command, got);
    }
}

void LineConnection::heartbeatTimeout()
{
    sendCommand("HEARTBEAT");
}

void LineConnection::connectionTimeout()
{
    qInfo() << m_id << " timeout";

    disconnect();
}

void LineConnection::disconnectedEvent()
{
    qInfo() << m_id << " disconnected.";

    m_timeout_timer->stop();
    m_socket->deleteLater();
}
