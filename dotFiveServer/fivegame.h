#ifndef FIVEGAME_H
#define FIVEGAME_H

#include <cstdlib>
#include <QObject>
#include <QList>

#include <QDebug>

#include "fiveconnection.h"
#include "fiveboard.h"

#define GET_CLIENT_OR_DIE\
    FiveConnection *client = getClient();\
    if (!client) {\
        qCritical() << "getClient failed";\
        return;\
    }

#define REQUIRE_PLAYER\
    if (!isPlayer(client)) {\
        client->toError("not a player");\
        return;\
    }

#define BOARDCAST(COMMAND) \
    for (QList<FiveConnection *>::iterator\
            i = m_con.begin();\
            i != m_con.end(); i++) \
        (*i)->COMMAND

#define PLAYER_BLACK    0
#define PLAYER_WHITE    1
#define PLAYER_SIZE     2

#define PLAYER_FIRST    PLAYER_BLACK


typedef int player_t;

class FiveGame : public QObject
{

    Q_OBJECT

public:
    explicit FiveGame(QObject *parent = 0);
    ~FiveGame(void);

    bool m_terminated;

signals:
    void gameShouldTerminate(void);

public slots:
    void addConnection(FiveConnection *con);

    void gameStart(void);
    void gameFinish(player_t winner);
    void gameSetPlayer(player_t type, FiveConnection *client);
    void gameChangeTurn(player_t type);
    void gameSyncAllTo(FiveConnection *client);

protected slots:
    void toStart(void);
    void toSetColor(bool is_white);
    void toChangeTurn(bool is_white);
    void toSync(QPoint loc, bool exist, bool is_white);
    void toRequestUndo(int ref_id);
    void toReplyUndo(int ref_id, bool accepted);
    void toRequestGiveUp(void);
    void toReplyGiveUp(bool accepted);
    void toDisconnect(void);
    void toDisconnect(FiveConnection *con);
    void toCheck(QString checksum);

    void boardChanged(QPoint loc, bool exist, bool is_white);
    void boardWon(bool is_white);

protected:
    FiveBoard *m_board;
    bool m_started;
    player_t m_now;

    QList<FiveConnection *> m_con;

    // see PLAYER_BLACK and PLAYER_WHITE
    FiveConnection *m_player[PLAYER_SIZE];
    bool m_confirmed_start[PLAYER_SIZE];

    FiveConnection *getClient(void);
    bool isPlayer(FiveConnection *client);
    bool playerType(FiveConnection *client);

};

#endif // FIVEGAME_H
