#ifndef LINECONNECTION_H
#define LINECONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <QStringBuilder>
#include <QTimer>

class LineConnection : public QObject
{
    Q_OBJECT

public:
    explicit LineConnection(QTcpSocket *socket,
                            int heartbeat = 0, int timeout = 0,
                            QObject *parent = 0);
    ~LineConnection(void);

    void sendCommand(QString command,
                     QStringList argv = QStringList());
    void disconnect(void);

signals:
    void receivedCommand(QString command, QStringList argv);
    void disconnected(void);

public slots:
    void readyRead(void);

    void heartbeatTimeout(void);
    void connectionTimeout(void);
    void disconnectedEvent(void);

protected:
    void emitLine(QString line);

    QTcpSocket *m_socket;
    qintptr m_id;

    QTimer *m_heartbeat_timer;
    QTimer *m_timeout_timer;

};

#endif // LINECONNECTION_H
