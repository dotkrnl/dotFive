#include "linesocket.h"

#include <QtDebug>

LineSocket::LineSocket(QTcpSocket *socket,
                       int heartbeat, int timeout,
                       QObject *parent)
    : QObject(parent),
      m_disconnected(false),
      m_socket(socket),
      m_heartbeat_timer(new QTimer(this)),
      m_timeout_timer(new QTimer(this))
{
    m_socket->setParent(this);
    m_id = m_socket->socketDescriptor();

    qWarning()  << m_id << " connected";
    qDebug() << m_id << " line socket created";

    if (heartbeat != 0) {
        m_heartbeat_timer->setInterval(heartbeat);
        m_heartbeat_timer->start();
        connect(m_heartbeat_timer, SIGNAL(timeout()),
                this, SLOT(heartbeatTimeout()));
    }

    if (timeout != 0) {
        m_timeout_timer->setInterval(timeout);
        m_timeout_timer->start();
        connect(m_timeout_timer, SIGNAL(timeout()),
                this, SLOT(connectionTimeout()));
    }

    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    if (m_socket->isReadable()) readyRead();

    connect(m_socket, SIGNAL(disconnected()),
            this, SLOT(disconnectedEvent()));
    if (m_socket->state() != QAbstractSocket::ConnectedState)
        disconnectedEvent();

}

LineSocket::~LineSocket(void)
{
    qDebug() << m_id << " line socket deleted";
}

void LineSocket::sendCommand(QString command,
                             QStringList argv)
{
    QString data;
    if (!argv.size())
        data = command + "\r\n";
    else
        data = command % " " % argv.join(" ") % "\r\n";

    qDebug() << m_id << " to which send:" << data;

    m_socket->write(data.toUtf8());
}

void LineSocket::disconnect(void)
{
    m_timeout_timer->stop();
    m_socket->close();
}

void LineSocket::readyRead()
{
    m_timeout_timer->start();

    while (m_socket->canReadLine())
        processLine(QString::fromUtf8(m_socket->readLine()));
}

void LineSocket::processLine(QString line)
{
    QStringList got = line.trimmed().split(" ");

    QString command = got.front();
    got.pop_front();

    if (command == "") {
        qWarning() << m_id << " from which got nothing";
    } else if (command == "HEARTBEAT") {
        qWarning() << m_id << " from which got heartbeat";
    } else {
        qDebug() << m_id << " from which got:" << line;
        emit receivedCommand(command, got);
    }
}

void LineSocket::heartbeatTimeout()
{
    sendCommand("HEARTBEAT");
}

void LineSocket::connectionTimeout()
{
    qWarning() << m_id << " timeout";

    disconnect();
}

void LineSocket::disconnectedEvent()
{
    qWarning() << m_id << " disconnected.";

    m_timeout_timer->stop();

    m_disconnected = true;
    emit disconnected();
}
