#ifndef FIVECONNECTION_H
#define FIVECONNECTION_H

#include <QObject>
#include <QPoint>

#include "lineconnection.h"

class FiveConnection : public LineConnection
{

    Q_OBJECT

public:
    explicit FiveConnection(QTcpSocket *socket,
                            int heartbeat = 0, int timeout = 0,
                            QObject *parent = 0);

signals:
    void token(QString token);
    void started(void);
    void needSync(QPoint loc, bool exist, bool is_white);
    void requestedUndo(int ref_id);
    void repliedUndo(int ref_id, bool accepted);
    void requestedGiveUp(void);
    void repliedGiveUp(bool accepted);
    void changeTurn(bool is_white);
    void setColor(bool is_white);

    // to client only
    void finished(bool win);

    // to server only
    void needCheck(QString checksum);
    void needCreateToken(void);

public slots:
    void toToken(QString token);
    void toStart(void);
    void toSync(QPoint loc, bool exist, bool is_white);
    void toRequestUndo(int ref_id);
    void toReplyUndo(int ref_id, bool accepted);
    void toRequestGiveUp(void);
    void toReplyGiveUp(bool accepted);
    void toChangeTurn(bool is_white);
    void toSetColor(bool is_white);
    void toDisconnect(void);

    // from server only
    void toFinish(bool win);

    // from client only
    void toCheck(QString checksum);
    void toCreateToken(void);

    // utils
    void decodeCommand(QString command, QStringList argv);

};

#endif // FIVECONNECTION_H
