#ifndef FIVEGAMETASK_H
#define FIVEGAMETASK_H

#include <QObject>
#include <QRunnable>
#include <QEventLoop>

#include <cmath>

#include "fivegame.h"

class FiveGameTask : public QObject
{
    Q_OBJECT

public:
    FiveGameTask(FiveConnection *creator);
    ~FiveGameTask(void);

    void start(QVector<QThread *> *thread_pool);
    void addConnection(FiveConnection *con);

    FiveGame *m_game;
    FiveConnection *m_creator;

signals:
    void gameTerminated(void);

/* internal signals: */
    void shouldAddConnection(FiveConnection *con);

private slots:
    void gameShouldTerminate(void);

};

#endif // FIVEGAMETASK_H
