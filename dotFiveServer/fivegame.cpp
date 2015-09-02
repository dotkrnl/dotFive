#include "fivegame.h"

FiveGame::FiveGame(QObject *parent)
    : QObject(parent),
      m_board(new FiveBoard(this)),
      m_started(false),
      m_terminated(false),
      m_now(PLAYER_BLACK)
{
    qDebug() << "five game created";

    m_player[PLAYER_BLACK]
            = m_player[PLAYER_WHITE]
            = NULL;
    m_confirmed_start[PLAYER_BLACK]
            = m_confirmed_start[PLAYER_WHITE]
            = false;

    connect(m_board, SIGNAL(changed(QPoint,bool,bool)),
            this,    SLOT(boardChanged(QPoint,bool,bool)));
    connect(m_board, SIGNAL(won(bool)),
            this,    SLOT(boardWon(bool)));
}

FiveGame::~FiveGame(void)
{
    qDebug() << "five game deleted";
}

void FiveGame::gameStart(void)
{
    m_confirmed_start[PLAYER_BLACK]
            = m_confirmed_start[PLAYER_BLACK]
            = false;

    m_started = true;

    BOARDCAST(toStart());
    gameChangeTurn(PLAYER_FIRST);
}

void FiveGame::gameFinish(player_t winner)
{
    m_started = false;
    m_board->toClear();

    BOARDCAST(toFinish(winner == PLAYER_WHITE));
}

void FiveGame::gameSetPlayer(player_t type,
                             FiveConnection *client)
{
    m_player[type] = client;
    m_confirmed_start[type] = false;

    if (client != NULL)
        client->toSetColor(type == PLAYER_WHITE);
}

void FiveGame::gameChangeTurn(player_t type)
{
    m_now = type;
    BOARDCAST(toChangeTurn(type == PLAYER_WHITE);)
}

void FiveGame::gameSyncAllTo(FiveConnection *client)
{
    // to be implemented
    client->toError("sync all not implemented");
}

void FiveGame::boardChanged(QPoint loc, bool exist, bool is_white)
{
    BOARDCAST(toSync(loc, exist, is_white));
}

void FiveGame::boardWon(bool is_white)
{
    gameFinish(is_white ? PLAYER_WHITE : PLAYER_BLACK);
}

void FiveGame::toStart(void)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    if (m_started) {
        client->toStart();
    } else {
        m_confirmed_start[playerType(client)] = true;

        if (m_confirmed_start[PLAYER_BLACK]
                && m_confirmed_start[PLAYER_WHITE]) {
            gameStart();
        }
    }
}

void FiveGame::toSetColor(bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    if (m_started || m_confirmed_start[playerType(client)]) {
        client->toError("can't change color after started");
    }

    player_t dest = is_white ?
                PLAYER_WHITE : PLAYER_BLACK;

    if (dest == playerType(client)) {
        gameSetPlayer(dest, client);
    } else {
        // swap color
        FiveConnection *new_white = m_player[PLAYER_BLACK];
        FiveConnection *new_black = m_player[PLAYER_WHITE];
        gameSetPlayer(PLAYER_WHITE, new_white);
        gameSetPlayer(PLAYER_BLACK, new_black);
    }
}

void FiveGame::toChangeTurn(bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    player_t dest = is_white ?
                PLAYER_WHITE : PLAYER_BLACK;

    if (dest == playerType(client)) {
        client->toError("permission denied");
    } else {
        gameChangeTurn(dest);
    }
}

void FiveGame::toSync(QPoint loc, bool exist, bool is_white)
{
    GET_CLIENT_OR_DIE;
    REQUIRE_PLAYER;

    player_t dest = is_white ?
                PLAYER_WHITE : PLAYER_BLACK;

    if (!m_started) { // all permission granted
        m_board->toChange(loc, exist, is_white);

    } else if (playerType(client) != m_now) {
        client->toError("please wait for your turn");

    } else if (!exist || dest != playerType(client)) {
        client->toError("permission denied");

    } else {
        // to set $loc to be $is_white in one's turn
        if (m_board->isStatus(loc, exist, !is_white)) {
            client->toError("location used");
            client->toSync(loc, exist, !is_white);

        } else if (m_board->isStatus(loc, exist, is_white)) {
            client->toSync(loc, exist, is_white);

        } else {
            m_board->toChange(loc, exist, is_white);
            gameChangeTurn(
                playerType(client) == PLAYER_BLACK
                        ? PLAYER_WHITE : PLAYER_BLACK);
        }
    }
}

void FiveGame::toRequestUndo(int ref_id)
{
    GET_CLIENT_OR_DIE;
    client->toError("not implemented");
}

void FiveGame::toReplyUndo(int ref_id, bool accepted)
{
    GET_CLIENT_OR_DIE;
    client->toError("not implemented");
}

void FiveGame::toRequestGiveUp(void)
{
    GET_CLIENT_OR_DIE;
    client->toError("not implemented");
}

void FiveGame::toReplyGiveUp(bool accepted)
{
    GET_CLIENT_OR_DIE;
    client->toError("not implemented");
}

void FiveGame::toDisconnect(void)
{
    GET_CLIENT_OR_DIE;

    toDisconnect(client);
}

void FiveGame::toDisconnect(FiveConnection *client)
{
    m_con.removeAll(client);

    if (isPlayer(client)) {
        m_player[playerType(client)] = NULL;
        m_confirmed_start[playerType(client)] = false;

        if (m_started) {
            gameFinish(playerType(client) == PLAYER_WHITE
                       ? PLAYER_BLACK : PLAYER_WHITE);
        }
    }

    if (m_con.size() == 0) {
        m_terminated = true;
        emit gameShouldTerminate();
    }

    client->deleteLater();
}

void FiveGame::toCheck(QString checksum)
{
    GET_CLIENT_OR_DIE;
    client->toError("not implemented");
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
    connect(con,  SIGNAL(requestedUndo(int)),
            this, SLOT(toRequestUndo(int)));
    connect(con,  SIGNAL(repliedUndo(int,bool)),
            this, SLOT(toReplyUndo(int,bool)));
    connect(con,  SIGNAL(requestedGiveUp()),
            this, SLOT(toRequestGiveUp()));
    connect(con,  SIGNAL(repliedGiveUp(bool)),
            this, SLOT(toReplyGiveUp(bool)));
    connect(con,  SIGNAL(needCheck(QString)),
            this, SLOT(toCheck(QString)));

    gameSyncAllTo(con);

    if (!m_player[PLAYER_BLACK])
        gameSetPlayer(PLAYER_BLACK, con);
    else if (!m_player[PLAYER_WHITE])
        gameSetPlayer(PLAYER_WHITE, con);

    connect(con,  SIGNAL(disconnected()),
            this, SLOT(toDisconnect()));
    if (con->m_disconnected)
        toDisconnect(con);
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

bool FiveGame::playerType(FiveConnection *client)
{
    return client == m_player[PLAYER_WHITE] ?
                PLAYER_WHITE : PLAYER_BLACK;
}
