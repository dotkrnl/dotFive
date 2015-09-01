#include "fiveserver.h"

#include <QDebug>

FiveServer::FiveServer(const FiveServerOptions *options,
                       QObject *parent)
    : QTcpServer(parent),
      m_options(options)
{
    connect(this, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
}

bool FiveServer::startServer(void)
{
    QString host_v = m_options->get("host");
    QHostAddress host;
    if (host_v == five::ADDRESS_ANY)
        host = QHostAddress::Any;
    else
        host = QHostAddress(host_v);
    if (host.isNull()) {
        qCritical() << "Bad host specified";
        return false;
    }

    bool okPort;
    int port = m_options->get("port").toInt(&okPort);
    if (!okPort) {
        qCritical() << "Bad port specified";
        return false;
    }

    if (!this->listen(host, port)) {
        qCritical() << "Could not start server:"
                    << this->errorString();
        return false;
    } else {
        qDebug() << "Listening to port" << port
                 << "on" << host;
        return true;
    }
}

void FiveServer::acceptConnection(void)
{
    qDebug() << "client connecting...";

    bool okTimeout, okHeartbeat;
    int timeout = m_options->get("timeout").toInt(&okTimeout);
    int heartbeat = m_options->get("heartbeat").toInt(&okHeartbeat);

    if (!okTimeout || !okHeartbeat) {
        qFatal("Bad timeout or heart");
    }

    FiveConnection *con = new FiveConnection(
                nextPendingConnection(),
                heartbeat, timeout, this);

    connect(con, SIGNAL(disconnected()),
            con, SLOT(deleteLater()));
}
