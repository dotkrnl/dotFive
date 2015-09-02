#include "fivegamemanager.h"

FiveGameManager::FiveGameManager(QObject *parent)
    : QObject(parent),
      m_mapper(new QSignalMapper(this))
{
    QVector<int> tokens(TOKEN_COUNT);
    for (int i = MIN_TOKEN; i <= MAX_TOKEN; i++)
        tokens[i] = i;

    std::random_shuffle(tokens.begin(), tokens.end());

    for (int i = 0; i < TOKEN_COUNT; i++)
        m_token_pool.enqueue(tokens[i]);

    m_thread_pool.resize(QThread::idealThreadCount());
    for (int i = 0; i < m_thread_pool.size(); i++) {
        m_thread_pool[i] = new QThread();
        m_thread_pool[i]->start();
    }

    connect(m_mapper, SIGNAL(mapped(int)),
            this,     SLOT(gameDeleted(int)));
}

// transfer deletion to m_manager
void FiveGameManager::addConnection(FiveConnection *con)
{
    con->setParent(this);
    connect(con,  SIGNAL(needCreateToken()),
            this, SLOT(toCreateToken()));
    connect(con,  SIGNAL(token(QString)),
            this, SLOT(toToken(QString)));
}

void FiveGameManager::toCreateToken(void)
{
    FiveConnection *client =
            dynamic_cast<FiveConnection *>(sender());

    if (!client) {
        qWarning() << "getClient failed";
        return;
    }

    if (m_token_pool.empty()) {
        qWarning() << "no token available";
        client->toError("no token available");
        return;
    }

    int t = m_token_pool.dequeue();
    qDebug() << "token" << t << "created";

    // transfer client deletion to new_game
    FiveGameTask *new_game = new FiveGameTask(client);
    // FiveGameTask delete in gameDeleted

    m_games[t] = new_game;
    client->toToken(QString::number(t));

    m_mapper->setMapping(new_game, t);
    connect(new_game, SIGNAL(gameTerminated()),
            m_mapper, SLOT(map()));

    new_game->start(&m_thread_pool);
}

void FiveGameManager::toToken(QString token)
{
    FiveConnection *client =
            dynamic_cast<FiveConnection *>(sender());

    if (!client) {
        qWarning() << "getClient failed";
        return;
    }

    bool ok = false;
    int t = token.toInt(&ok);

    QMap<int, FiveGameTask *>::iterator game
            = m_games.find(t);
    if (!ok || game == m_games.end()) {
        qDebug() << "bad token found";
        client->toError("token not found");
        return;
    }

    qDebug() << "adding to token" << t;
    client->toToken(QString::number(t));
    // transfer deletion to *game
    (*game)->addConnection(client);
}

void FiveGameManager::gameDeleted(int t)
{
    m_games[t]->deleteLater();
    m_games.remove(t);

    m_token_pool.enqueue(t);

    qDebug() << "token" << t << "deleted";
    qDebug() << "token remaining:" << m_token_pool.size();
}
