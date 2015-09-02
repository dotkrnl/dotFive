#ifndef FIVESERVER_H
#define FIVESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>

#include "fiveserveroptions.h"

#include "linesocket.h"
#include "fivegamemanager.h"
#include "fiveconnection.h"
#include "fiveconstants.h"

class FiveServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit FiveServer(const FiveServerOptions *options,
                        QObject *parent = 0);
    bool startServer(void);
    // TODO: stopServer w/c+C

protected:
    const FiveServerOptions *m_options;
    FiveGameManager *m_manager;

signals:
    void serverStopped(void);

public slots:
    void acceptConnection(void);

};

#endif // FIVESERVER_H
