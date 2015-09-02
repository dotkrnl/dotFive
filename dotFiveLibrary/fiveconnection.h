#ifndef FIVECONNECTION_H
#define FIVECONNECTION_H

#include <QObject>
#include <QPoint>

#include "linesocket.h"

class FiveConnection : public QObject
{
    Q_OBJECT

public:
    explicit FiveConnection(LineSocket *con,
                            QObject *parent = 0);
    ~FiveConnection(void);

signals:
/* signals from remote */
    void token(QString token);
    void setColor(bool is_white);
    void changeTurn(bool is_white);
    void started(void);
    void needSync(QPoint loc, bool exist, bool is_white);
    void requestedUndo(int ref_id);
    void repliedUndo(int ref_id, bool accepted);
    void requestedGiveUp(void);
    void repliedGiveUp(bool accepted);
    void disconnected(void);
    void error(QString error);

    // to client only
    void finished(bool winner_is_white);

    // to server only
    void needCheck(QString checksum);
    void needCreateToken(void);

/* signals from local */
    void sendCommand(QString command, QStringList argv);
    void disconnect(void);

public slots:
/* slots to emit to remote */
    void toToken(QString token);
    void toSetColor(bool is_white);
    void toChangeTurn(bool is_white);
    void toStart(void);
    void toSync(QPoint loc, bool exist, bool is_white);
    void toRequestUndo(int ref_id);
    void toReplyUndo(int ref_id, bool accepted);
    void toRequestGiveUp(void);
    void toReplyGiveUp(bool accepted);
    void toDisconnect(void);
    void toError(QString error);

    // from server only
    void toFinish(bool winner_is_white);

    // from client only
    void toCreateToken(void);
    void toCheck(QString checksum);

/* slots to emit to local */
    void decodeCommand(QString command, QStringList argv);

protected:
    LineSocket *m_con;

};

#endif // FIVECONNECTION_H
