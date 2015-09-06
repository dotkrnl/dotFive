#ifndef FIVEGAME_H
#define FIVEGAME_H

#include <cstdlib>

#include <QObject>
#include <QList>
#include <QTimer>

#include "fiveconnection.h"
#include "fiveboard.h"
#include "fiveconstants.h"

#define NOT_PLAYER      (-1)
#define PLAYER_BLACK    (0)
#define PLAYER_WHITE    (1)
#define PLAYER_SIZE     (2)

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
    void gameChangePlayer(player_t type, FiveConnection *client);
    void gameChangeTurn(player_t type);
    void gameUndoAction(void);
    void gameSaveForUndo(void);
    void gameSyncAllTo(FiveConnection *client);
    void gameTerminate(void);

protected slots:
    void toStart(void);
    void toSetColor(bool is_white);
    void toChangeTurn(bool is_white);
    void toSync(QPoint loc, bool exist, bool is_white);
    void toRequestUndo(void);
    void toReplyUndo(bool accepted);
    void toRequestGiveUp(void);
    void toReplyGiveUp(bool accepted);
    void toDisconnect(void);
    void toDisconnect(FiveConnection *con);
    void toCheck(QString checksum);

    void boardChanged(QPoint loc, bool exist, bool is_white);
    void boardWon(bool is_white);

    void exchangeTimeout(void);

protected:
    FiveBoard *m_board;

    bool m_started;
    player_t m_now;

    int  m_who_start;
    int  m_who_undo;
    int  m_who_giveup;

    int  m_time_used[PLAYER_SIZE];

    QTimer *m_exchange_timer;

    QList<FiveConnection *> m_con;
    // see PLAYER_BLACK and PLAYER_WHITE
    FiveConnection *m_player[PLAYER_SIZE];

    QList<FiveBoard> m_history;

    FiveConnection *getClient(void);
    bool isPlayer(FiveConnection *client);
    player_t playerType(bool is_white);
    player_t playerType(FiveConnection *client);
    player_t oppositeType(FiveConnection *client);
    player_t oppositeType(player_t type);

};

#endif // FIVEGAME_H
