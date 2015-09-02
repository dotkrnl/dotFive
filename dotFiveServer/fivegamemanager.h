#ifndef FIVEGAMEMANAGER_H
#define FIVEGAMEMANAGER_H

#include <QObject>
#include <QMap>
#include <QQueue>
#include <QVector>
#include <QThreadPool>
#include <QSignalMapper>

#include <algorithm>

#include "fiveconnection.h"
#include "fivegametask.h"

#define MIN_TOKEN   0
#define MAX_TOKEN   9999
#define TOKEN_COUNT (MAX_TOKEN - MIN_TOKEN + 1)

class FiveGameManager : public QObject
{
    Q_OBJECT

public:
    explicit FiveGameManager(QObject *parent = 0);

signals:

public slots:
    void addConnection(FiveConnection *con);

protected slots:
    void toCreateToken(void);
    void toToken(QString token);

    void gameDeleted(int t);

protected:
    QMap<int, FiveGameTask *> m_games;
    QQueue<int> m_token_pool;
    QSignalMapper *m_mapper;

};

#endif // FIVEGAMEMANAGER_H
