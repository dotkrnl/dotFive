#include "fivegametask.h"

#include <QDebug>

FiveGameTask::FiveGameTask(void)
    : QObject()
{
    qDebug() << "five game task created";
}

FiveGameTask::~FiveGameTask(void)
{
    qDebug() << "five game task deleted";
}

void FiveGameTask::run()
{
    qDebug() << "five game task started";

    m_game = new FiveGame();
    m_event = new QEventLoop();

    connect(this,   SIGNAL(shoudAddConnection(FiveConnection*)),
            m_game, SLOT(addConnection(FiveConnection*)));

    connect(m_game, SIGNAL(gameShouldTerminate()),
            this,   SIGNAL(gameTerminated()));
    connect(m_game, SIGNAL(gameShouldTerminate()),
            this,   SLOT(gameShouldTerminate()));

    m_event->exec();
    delete m_event;
    delete m_game;

    qDebug() << "five game task to terminate";
}

void FiveGameTask::addConnection(FiveConnection *con)
{
    emit shoudAddConnection(con);
}

void FiveGameTask::gameShouldTerminate(void)
{
    m_event->exit(0);
}
