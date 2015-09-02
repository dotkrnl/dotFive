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

void FiveGameTask::run()
{
    qDebug() << "five game task started";

    m_game = new FiveGame();
    m_event = new QEventLoop();


    // transfer deletion to m_game
    connect(this,   SIGNAL(shouldAddConnection(FiveConnection*)),
            m_game, SLOT(addConnection(FiveConnection*)));

    connect(m_game, SIGNAL(gameShouldTerminate()),
            this,   SLOT(gameShouldTerminate()));

    addConnection(m_creator);

    m_event->exec();

    emit gameTerminated();

    delete m_event;
    delete m_game;

    qDebug() << "five game task to terminate";
}

// transfer deletion to FiveGameTask
void FiveGameTask::addConnection(FiveConnection *con)
{
    con->setParent(this);
    emit shouldAddConnection(con);
}

void FiveGameTask::gameShouldTerminate(void)
{
    m_event->exit(0);
}
