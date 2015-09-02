#include "fivegametask.h"

#include <QDebug>

// transfer deletion to FiveGameTask
FiveGameTask::FiveGameTask(FiveConnection *creator)
    : QObject(),
      m_creator(creator)
{
    qDebug() << "five game task created";
}

FiveGameTask::~FiveGameTask(void)
{
    qDebug() << "five game task deleted";
}

int randomChoose(int size)
{
    return rand() % size;
}

void FiveGameTask::start(QVector<QThread *> *thread_pool)
{
    qDebug() << "five game task started";

    int chose = randomChoose(thread_pool->size());

    m_game = new FiveGame();
    m_game->moveToThread((*thread_pool)[chose]);

    // transfer deletion to m_game
    connect(this,   SIGNAL(shouldAddConnection(FiveConnection*)),
            m_game, SLOT(addConnection(FiveConnection*)));

    connect(m_game, SIGNAL(gameShouldTerminate()),
            this,   SLOT(gameShouldTerminate()));

    addConnection(m_creator);
}

// transfer deletion to FiveGameTask
void FiveGameTask::addConnection(FiveConnection *con)
{
    con->setParent(this);
    emit shouldAddConnection(con);
}

void FiveGameTask::gameShouldTerminate(void)
{
    qDebug() << "five game task to terminate";

    emit gameTerminated();
    m_game->deleteLater();
}
