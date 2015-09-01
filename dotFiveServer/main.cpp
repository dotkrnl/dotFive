#include <QCoreApplication>
#include <QDebug>

#include "fiveserveroptions.h"
#include "fiveserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("dotFiveServer");
    QCoreApplication::setApplicationVersion("0.1");

    FiveServerOptions options(&app);
    FiveServer server(&options);

    QObject::connect(&server, SIGNAL(serverStopped()),
            &app, SLOT(quit()));

    if (!server.startServer()) {
        qCritical("Error while starting server, stop.");
        return 1;
    }

    return app.exec();
}
