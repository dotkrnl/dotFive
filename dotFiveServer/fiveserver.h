#ifndef FIVESERVER_H
#define FIVESERVER_H

#include <QObject>
#include <QTcpServer>

#include "fiveserveroptions.h"

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

signals:
    void serverStopped(void);

public slots:
    void acceptConnection(void);

};

#endif // FIVESERVER_H