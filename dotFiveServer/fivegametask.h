#ifndef FIVEGAMETASK_H
#define FIVEGAMETASK_H

#include <QObject>
#include <QRunnable>
#include <QEventLoop>

#include "fivegame.h"

class FiveGameTask : public QObject,
                     public QRunnable
{
    Q_OBJECT

public:
    FiveGameTask(void);
    ~FiveGameTask(void);

    void run(void);
    void addConnection(FiveConnection *con);
    FiveGame *m_game;
    QEventLoop *m_event;

signals:
    void gameTerminated(void);


/* internal signals: */
    void shoudAddConnection(FiveConnection *con);

private slots:
    void gameShouldTerminate(void);

};

#endif // FIVEGAMETASK_H
