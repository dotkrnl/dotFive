#ifndef FIVECONNECTION_H
#define FIVECONNECTION_H

#include <QObject>
#include <QPoint>

#include "linesocket.h"

class FiveConnection : public QObject
{
    Q_OBJECT

public:
    explicit FiveConnection(QTcpSocket *socket,
                            int heartbeat = 0, int timeout = 0,
                            QObject *parent = 0);
    ~FiveConnection(void);
    bool m_disconnected;

signals:
/* signals from remote */
    void token(QString token);
    void setColor(bool is_white);
    void changeTurn(bool is_white);
    void started(void);
    void needSync(QPoint loc, bool exist, bool is_white);
    void requestedUndo(void);
    void repliedUndo(bool accepted);
    void requestedGiveUp(void);
    void repliedGiveUp(bool accepted);
    void disconnected(void);
    void error(QString error);

    // to client only
    void timeChanged(int time, bool is_white);
    void finished(bool winner_is_white);

    // to server only
    void needCheck(QString checksum);
    void needCreateToken(void);
    void needMagic(void);

/* signals from local */
    void sendCommand(QString command, QStringList argv);
    void shouldDisconnect(void);

public slots:
/* slots to emit to remote */
    void toToken(QString token);
    void toSetColor(bool is_white);
    void toChangeTurn(bool is_white);
    void toStart(void);
    void toSync(QPoint loc, bool exist, bool is_white);
    void toRequestUndo(void);
    void toReplyUndo(bool accepted);
    void toRequestGiveUp(void);
    void toReplyGiveUp(bool accepted);
    void toDisconnect(void);
    void toError(QString error);

    // from server only
    void toChangeTime(int time, bool is_white);
    void toFinish(bool winner_is_white);

    // from client only
    void toCreateToken(void);
    void toCheck(QString checksum);
    void toMagic(void);

/* slots to emit to local */
    void decodeCommand(QString command, QStringList argv);
    void disconnectEvent(void);

protected:
    LineSocket *m_con;

};

#endif // FIVECONNECTION_H
