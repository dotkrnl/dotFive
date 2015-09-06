#include "fivegame.h"

#include <QtDebug>

FiveGame::FiveGame(QObject *parent)
    : QObject(parent),
      m_terminated(false),
      m_board(new FiveBoard(this)),
      m_started(false),
      m_now(NOT_PLAYER),
      m_who_start(NOT_PLAYER),
      m_who_undo(NOT_PLAYER),
      m_who_giveup(NOT_PLAYER),
      m_exchange_timer(new QTimer(this))
{
    qDebug() << "five game created";

    m_player[PLAYER_BLACK]
            = m_player[PLAYER_WHITE]
            = NULL;

    connect(m_board, SIGNAL(changed(QPoint,bool,bool)),
            this,    SLOT(boardChanged(QPoint,bool,bool)));
    connect(m_board, SIGNAL(won(bool)),
            this,    SLOT(boardWon(bool)));

    m_exchange_timer->setInterval(five::GIVEUP_TIMEOUT_SERVER);
    connect(m_exchange_timer, SIGNAL(timeout()),
            this, SLOT(exchangeTimeout()));
}

FiveGame::~FiveGame(void)
{
    qDebug() << "five game deleted";
}


#define BOARDCAST(COMMAND) \
    for (QList<FiveConnection *>::iterator\
            i = m_con.begin();\
            i != m_con.end(); i++) \
        (*i)->COMMAND

void FiveGame::gameStart(void)
{
    m_now = NOT_PLAYER;
    m_who_start = NOT_PLAYER;
    m_who_undo = NOT_PLAYER;
    m_who_giveup = NOT_PLAYER;
    m_history.clear();

    m_time_used[PLAYER_BLACK]
            = m_time_used[PLAYER_WHITE]
            = 0;

    BOARDCAST(toChangeTime(0, PLAYER_BLACK));
    BOARDCAST(toChangeTime(0, PLAYER_WHITE));

    m_started = true;
    BOARDCAST(toStart());
    m_exchange_timer->start();
    gameChangeTurn(PLAYER_FIRST);
}

void FiveGame::gameFinish(player_t winner)
{
    m_started = false;

    if (m_now != NOT_PLAYER) {
        m_time_used[m_now] +=
                five::GIVEUP_TIMEOUT_SERVER -
                m_exchange_timer->remainingTime();
        BOARDCAST(toChangeTime(m_time_used[m_now], m_now));
    }

    m_board->toClear();
    m_exchange_timer->stop();

    BOARDCAST(toFinish(winner == PLAYER_WHITE));
}

void FiveGame::gameChangePlayer(player_t type,
                                FiveConnection *client)
{
    player_t orignal_type = playerType(client);

    if (orignal_type != NOT_PLAYER)
        m_player[orignal_type] = NULL;

    if (m_who_start == orignal_type)
        m_who_start = NOT_PLAYER;
    if (m_who_undo == orignal_type)
        m_who_undo = NOT_PLAYER;
    if (m_who_giveup == orignal_type)
        m_who_giveup = NOT_PLAYER;

    if (type != NOT_PLAYER)
        m_player[type] = client;

    if (client != NULL)
        client->toSetColor(type == PLAYER_WHITE);
}

void FiveGame::gameChangeTurn(player_t type)
{
    if (!m_started) return;

    if (m_now != type)
        gameSaveForUndo();

    if (m_now != NOT_PLAYER) {
        m_time_used[m_now] +=
                five::GIVEUP_TIMEOUT_SERVER -
                m_exchange_timer->remainingTime();
        BOARDCAST(toChangeTime(m_time_used[m_now], m_now));
    }

    m_now = type;

    if (m_who_undo != NOT_PLAYER) {
        m_player[m_who_undo]->toReplyUndo(false);
        m_who_undo = NOT_PLAYER;
    }

    if (m_who_giveup != NOT_PLAYER) {
        m_player[m_who_giveup]->toReplyGiveUp(false);
        m_who_giveup = NOT_PLAYER;
    }

    BOARDCAST(toChangeTurn(type == PLAYER_WHITE));
    m_exchange_timer->start();
}

void FiveGame::gameUndoAction(void)
{
    if (m_history.size() < 3) {
        BOARDCAST(toError("syserror: unable to undo"));
        return;
    }

    m_history.pop_back();
    m_history.pop_back();
    *m_board = m_history.back();

    m_exchange_timer->start();
    // recount timer after undo
}

void FiveGame::gameSaveForUndo(void)
{
    m_history.append(*m_board);
    while (m_history.size() > 5)
        m_history.pop_front();
}

void FiveGame::gameSyncAllTo(FiveConnection *client)
{
    if (m_started) {
        client->toStart();
        client->toChangeTurn(m_now == PLAYER_WHITE);
    }

    if (isPlayer(client)) {
        if (m_who_undo == oppositeType(client))
            client->toRequestUndo();
        if (m_who_giveup == oppositeType(client))
            client->toRequestGiveUp();
    }

    for (int i = 0; i < five::BOARD_SIZE; i++)
        for (int j = 0; j < five::BOARD_SIZE; j++) {
            QPoint location = QPoint(i, j);
            client->toSync(location,
                           m_board->isExistAt(location),
                           m_board->isWhiteAt(location));
        }
}

void FiveGame::gameTerminate(void)
{
    if (m_terminated) return; // ensure once

    m_terminated = true;
    emit gameShouldTerminate();
}


void FiveGame::boardChanged(QPoint loc, bool exist, bool is_white)
{
    BOARDCAST(toSync(loc, exist, is_white));
}

void FiveGame::boardWon(bool is_white)
{
    gameFinish(is_white ? PLAYER_WHITE : PLAYER_BLACK);
}

void FiveGame::exchangeTimeout(void)
{
    gameChangeTurn(oppositeType(m_now));
}


#define GET_CLIENT_OR_DIE\
    FiveConnection *client = getClient();\
    if (!client) {\
        qCritical() << "get client failed";\
        return;\
    }

#define REQUIRE_PLAYER\
    if (!isPlayer(client)) {\
        client->toError("not a player");\
        return;\
    }

#define REQUIRE_CURRENT\
    if (!m_started || playerType(client) != m_now) {\
        client->toError("please wait for your turn");\
        return;\
    }

#define REQUIRE_TYPE(TYPE)\
    if (playerType(client) != TYPE) {\
        client->toError("permission denied");\
        return;\
    }


void FiveGame::toStart(void)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    // client->toAck();
    // should reply ACK

    if (m_started) {
        gameSyncAllTo(client);

    } else if (m_who_start == NOT_PLAYER) {
        m_who_start = playerType(client);

    } else if (oppositeType(m_who_start)
            == playerType(client)) {
        gameStart();
    }
}

void FiveGame::toSetColor(bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    player_t dest = playerType(is_white);

    if (m_started || m_who_start != NOT_PLAYER) {
        client->toError("can't change color after started");

    } else if (dest == playerType(client)) {
        gameChangePlayer(dest, client);

    } else { // swap color
        FiveConnection *new_white = m_player[PLAYER_BLACK];
        FiveConnection *new_black = m_player[PLAYER_WHITE];
        gameChangePlayer(PLAYER_WHITE, new_white);
        gameChangePlayer(PLAYER_BLACK, new_black);
    }
}

void FiveGame::toChangeTurn(bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    player_t dest = playerType(is_white);

    REQUIRE_TYPE(oppositeType(dest));
    gameChangeTurn(dest);
}

void FiveGame::toSync(QPoint loc, bool exist, bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    player_t dest = playerType(is_white);

    if (!m_started) { // all permission granted
        m_board->toChange(loc, exist, is_white);
        return;
    }

    REQUIRE_CURRENT;
    REQUIRE_TYPE(dest);

    if (!exist) {
        client->toError("please use undo");

        // else to set $loc to be $is_white in one's turn
    } else if (m_board->isStatus(loc, exist, !is_white)) {
        client->toError("location used");
        gameSyncAllTo(client);

    } else if (m_board->isStatus(loc, exist, is_white)) {
        client->toSync(loc, exist, is_white);

    } else {
        m_board->toChange(loc, exist, is_white);
        gameChangeTurn(oppositeType(client));
    }
}

void FiveGame::toRequestUndo(void)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;
    REQUIRE_CURRENT;

    if (m_history.size() < 3) {
        client->toError("unable to undo");
        return;
    }

    m_who_undo = playerType(client);
    m_player[oppositeType(m_who_undo)]->toRequestUndo();
}

void FiveGame::toReplyUndo(bool accepted)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;
    REQUIRE_TYPE(oppositeType(m_who_undo));

    m_player[m_who_undo]->toReplyUndo(accepted);
    if (accepted) gameUndoAction();

    m_who_undo = NOT_PLAYER;
}

void FiveGame::toRequestGiveUp(void)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;
    REQUIRE_CURRENT;

    m_who_giveup = playerType(client);
    m_player[oppositeType(m_who_giveup)]->toRequestGiveUp();
}

void FiveGame::toReplyGiveUp(bool accepted)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;
    REQUIRE_TYPE(oppositeType(m_who_giveup));

    m_player[m_who_giveup]->toReplyGiveUp(accepted);
    if (accepted) gameFinish(oppositeType(m_who_giveup));

    m_who_giveup = NOT_PLAYER;
}

void FiveGame::toDisconnect(void)
{
    GET_CLIENT_OR_DIE;

    toDisconnect(client);
}

void FiveGame::toDisconnect(FiveConnection *client)
{
    if (m_started && isPlayer(client))
        gameFinish(oppositeType(client));
    gameChangePlayer(NOT_PLAYER, client);

    m_con.removeAll(client);
    client->deleteLater();

    if (m_con.empty()) gameTerminate();
}

void FiveGame::toCheck(QString checksum)
{
    GET_CLIENT_OR_DIE;
    gameSyncAllTo(client);
}


// transfer deletion to FiveGame, delete in toDisconnect
void FiveGame::addConnection(FiveConnection *con)
{
    if (m_terminated) return;

    m_con.append(con);

    connect(con,  SIGNAL(started()),
            this, SLOT(toStart()));
    connect(con,  SIGNAL(setColor(bool)),
            this, SLOT(toSetColor(bool)));
    connect(con,  SIGNAL(changeTurn(bool)),
            this, SLOT(toChangeTurn(bool)));
    connect(con,  SIGNAL(needSync(QPoint,bool,bool)),
            this, SLOT(toSync(QPoint,bool,bool)));
    connect(con,  SIGNAL(requestedUndo()),
            this, SLOT(toRequestUndo()));
    connect(con,  SIGNAL(repliedUndo(bool)),
            this, SLOT(toReplyUndo(bool)));
    connect(con,  SIGNAL(requestedGiveUp()),
            this, SLOT(toRequestGiveUp()));
    connect(con,  SIGNAL(repliedGiveUp(bool)),
            this, SLOT(toReplyGiveUp(bool)));
    connect(con,  SIGNAL(needCheck(QString)),
            this, SLOT(toCheck(QString)));
    gameSyncAllTo(con);

    if (!m_player[PLAYER_BLACK])
        gameChangePlayer(PLAYER_BLACK, con);
    else if (!m_player[PLAYER_WHITE])
        gameChangePlayer(PLAYER_WHITE, con);

    connect(con,  SIGNAL(disconnected()),
            this, SLOT(toDisconnect()));
    if (con->m_disconnected) toDisconnect(con);
}


FiveConnection *FiveGame::getClient(void)
{
    if (!sender()) qCritical() << "slot without sender";
    return dynamic_cast<FiveConnection *>(sender());
}

bool FiveGame::isPlayer(FiveConnection *client)
{
    return client == m_player[PLAYER_BLACK] ||
           client == m_player[PLAYER_WHITE];
}

player_t FiveGame::playerType(bool is_white) {
    return is_white ? PLAYER_WHITE : PLAYER_BLACK;
}

player_t FiveGame::playerType(FiveConnection *client)
{
    if (!isPlayer(client)) return NOT_PLAYER;
    return client == m_player[PLAYER_WHITE]
            ? PLAYER_WHITE : PLAYER_BLACK;
}

player_t FiveGame::oppositeType(FiveConnection *client)
{
    if (!isPlayer(client)) return NOT_PLAYER;
    return client == m_player[PLAYER_WHITE]
            ? PLAYER_BLACK : PLAYER_WHITE;
}

player_t FiveGame::oppositeType(player_t type)
{
    if (type == NOT_PLAYER) return NOT_PLAYER;
    return type == PLAYER_WHITE
            ? PLAYER_BLACK : PLAYER_WHITE;
}
