#include "fiveserveroptions.h"

FiveServerOptions::FiveServerOptions(QCoreApplication *app,
                                     QObject *parent)
    : QObject(parent)
{
    m_parser.setApplicationDescription("Server daemon for dotFive");
    m_parser.addHelpOption();
    m_parser.addVersionOption();

    QCommandLineOption hostOption(QStringList() << "H" << "host",
            tr("Run server to listen on IP <host>."),
            tr("host"));
    hostOption.setDefaultValue(five::ADDRESS_ANY);
    m_parser.addOption(hostOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
            tr("Run server to listen on port <port>."),
            tr("port"));
    portOption.setDefaultValue(five::PORT_DEFAULT);
    m_parser.addOption(portOption);

    QCommandLineOption timeOption(QStringList() << "t" << "timeout",
            tr("Set the timeout of <timeout> ms before closing connection, 0 to disable."),
            tr("timeout"));
    timeOption.setDefaultValue(five::TIMEOUT_DEFAULT);
    m_parser.addOption(timeOption);

    QCommandLineOption heartOption(QStringList() << "b" << "heartbeat",
            tr("Set the heartbeat sending timeout to <heartbeat> ms, 0 to disable."),
            tr("heartbeat"));
    heartOption.setDefaultValue(five::HEARTBEAT_DEFAULT);
    m_parser.addOption(heartOption);

    m_parser.process(*app);
}
