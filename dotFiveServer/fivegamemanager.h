#ifndef FIVEGAMEMANAGER_H
#define FIVEGAMEMANAGER_H

#include <QObject>
#include <QMap>
#include <QQueue>
#include <QVector>
#include <QThread>
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
    QString toCreateToken(void);
    void toToken(QString token);
    void toMagic(void);

    void managedConnection(FiveConnection *con);

    void gameDeleted(int t);

protected:
    QMap<int, FiveGameTask *> m_games;
    QVector<QThread *> m_thread_pool;
    QQueue<int> m_token_pool;
    QSignalMapper *m_mapper;

    QString m_magic_token;

};

#endif // FIVEGAMEMANAGER_H
