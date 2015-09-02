#ifndef LINECONNECTION_H
#define LINECONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <QStringBuilder>
#include <QTimer>

class LineSocket : public QObject
{
    Q_OBJECT

public:
    explicit LineSocket(QTcpSocket *socket,
                        int heartbeat = 0, int timeout = 0,
                        QObject *parent = 0);
    ~LineSocket(void);

signals:
    void receivedCommand(QString command,
                         QStringList argv);

    void disconnected(void);

public slots:
    void sendCommand(QString command,
                     QStringList argv = QStringList());
    void disconnect(void);

protected slots:
    void readyRead(void);
    void processLine(QString line);

    void heartbeatTimeout(void);
    void connectionTimeout(void);
    void disconnectedEvent(void);

public:
    qintptr m_id;

protected:
    QTcpSocket *m_socket;

    QTimer *m_heartbeat_timer;
    QTimer *m_timeout_timer;

};

#endif // LINECONNECTION_H
