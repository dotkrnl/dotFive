#include "fivegamemanager.h"

FiveGameManager::FiveGameManager(QObject *parent)
    : QObject(parent),
      m_mapper(new QSignalMapper)
{
    int t_count = QThread::idealThreadCount();
    if (t_count < 2) t_count = 2;
    // should have at least 2

    m_thread_pool.resize(t_count);
    for (int i = 0; i < m_thread_pool.size(); i++) {
        m_thread_pool[i] = new QThread();
        m_thread_pool[i]->start();
    }

    this->moveToThread(m_thread_pool[0]);
    m_mapper->moveToThread(m_thread_pool[0]);
    m_mapper->setParent(this);

    connect(m_mapper, SIGNAL(mapped(int)),
            this,     SLOT(gameDeleted(int)));

    QVector<int> tokens(TOKEN_COUNT);
    for (int i = MIN_TOKEN; i <= MAX_TOKEN; i++)
        tokens[i] = i;

    std::random_shuffle(tokens.begin(), tokens.end());

    for (int i = 0; i < TOKEN_COUNT; i++)
        m_token_pool.enqueue(tokens[i]);
}

void FiveGameManager::addConnection(FiveConnection *con)
{
    connect(con,  SIGNAL(needCreateToken()),
            this, SLOT(toCreateToken()));
    connect(con,  SIGNAL(token(QString)),
            this, SLOT(toToken(QString)));
    connect(con,  SIGNAL(needMagic()),
            this, SLOT(toMagic()));
}

void FiveGameManager::managedConnection(FiveConnection *con)
{
    disconnect(con,  SIGNAL(needCreateToken()),
               this, SLOT(toCreateToken()));
    disconnect(con,  SIGNAL(token(QString)),
               this, SLOT(toToken(QString)));
    disconnect(con,  SIGNAL(needMagic()),
               this, SLOT(toMagic()));
}

QString intToToken(int t)
{
    QString token = QString::number(t);
    while (token.length() < 4)
        token = "0" + token;
    return token;
}

QString FiveGameManager::toCreateToken(void)
{
    FiveConnection *client =
            dynamic_cast<FiveConnection *>(sender());

    if (!client) {
        qWarning() << "getClient failed in toCreateToken";
        return "";
    }

    if (m_token_pool.empty()) {
        qWarning() << "no token available";
        client->toError("no token available");
        return "";
    }

    managedConnection(client);

    int t = m_token_pool.dequeue();
    qDebug() << "token" << t << "created";

    // transfer client deletion to new_game
    FiveGameTask *new_game = new FiveGameTask(client);
    // FiveGameTask delete in gameDeleted
    m_games[t] = new_game;
    m_mapper->setMapping(new_game, t);
    connect(new_game, SIGNAL(gameTerminated()),
            m_mapper, SLOT(map()));

    client->toToken(intToToken(t));

    new_game->start(&m_thread_pool);

    return intToToken(t);
}

void FiveGameManager::toToken(QString token)
{
    FiveConnection *client =
            dynamic_cast<FiveConnection *>(sender());

    if (!client) {
        qWarning() << "getClient failed in toToken";
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

    managedConnection(client);

    qDebug() << "adding to token" << t;
    client->toToken(intToToken(t));

    // transfer deletion to *game
    (*game)->addConnection(client);
    // FiveGameTask delete in gameDeleted
}

void FiveGameManager::toMagic(void)
{
    FiveConnection *client =
            dynamic_cast<FiveConnection *>(sender());

    if (!client) {
        qWarning() << "getClient failed in toToken";
        return;
    }

    if (m_magic_token != "" &&
            m_games[m_magic_token.toInt()]) {
        toToken(m_magic_token);
        m_magic_token = "";
    } else {
        m_magic_token = toCreateToken();
    }
}

void FiveGameManager::gameDeleted(int t)
{
    QMap<int, FiveGameTask *>::iterator game
            = m_games.find(t);

    if (game != m_games.end() && *game != NULL) {
        m_games[t]->deleteLater();
        m_games.remove(t);
        m_token_pool.enqueue(t);
    } else {
        qCritical() << "token" << t << "already deleted";
    }

    qDebug() << "token" << t << "deleted";
    qDebug() << "token remaining:" << m_token_pool.size();
}
