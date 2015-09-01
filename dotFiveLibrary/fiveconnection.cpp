#include "fiveconnection.h"

#include <QDebug>

FiveConnection::FiveConnection(QTcpSocket *socket,
                               int heartbeat, int timeout,
                               QObject *parent)
    : LineConnection(socket, heartbeat, timeout, parent)
{
    connect(this, SIGNAL(receivedCommand(QString,QStringList)),
            this, SLOT(decodeCommand(QString,QStringList)));
}

void FiveConnection::decodeCommand(QString command, QStringList argv)
{
    #define ERROR if (true) {\
        qInfo() << m_id << " bad command";\
        return;\
    }
    #define SHOULE_SIZE(N) if (argv.size() != (N)) ERROR

    if (command == "TOKEN") {
        SHOULE_SIZE(1);
        emit token(argv[0]);

    } else if (command == "SYNC") {
        SHOULE_SIZE(0);
        emit started();

    } else if (command == "SYNC") {
        SHOULE_SIZE(4);
        emit needSync(QPoint(argv[0].toInt(),
                             argv[1].toInt()),
                      bool(argv[2].toInt()),
                      bool(argv[3].toInt()));

    } else if (command == "UNDORQ") {
        SHOULE_SIZE(1);
        emit requestedUndo(argv[0].toInt());

    } else if (command == "UNDORP") {
        SHOULE_SIZE(2);
        emit repliedUndo(argv[0].toInt(),
                         bool(argv[1].toInt()));

    } else if (command == "GIVEUPRQ") {
        SHOULE_SIZE(0);
        emit requestedGiveUp();

    } else if (command == "GIVEUPRP") {
        SHOULE_SIZE(1);
        emit repliedGiveUp(bool(argv[0].toInt()));

    } else if (command == "CHANGE") {
        SHOULE_SIZE(1);
        emit changeTurn(bool(argv[0].toInt()));

    } else if (command == "COLOR") {
        SHOULE_SIZE(1);
        emit setColor(bool(argv[0].toInt()));

    } else if (command == "FINISH") {
        SHOULE_SIZE(1);
        emit finished(bool(argv[0].toInt()));

    } else if (command == "CHECK") {
        SHOULE_SIZE(1);
        emit needCheck(argv[0]);

    } else if (command == "CREATE") {
        SHOULE_SIZE(0);
        emit needCreateToken();

    } else ERROR;
}

void FiveConnection::toToken(QString token)
{
    sendCommand("TOKEN", QStringList()
                << token);
}

void FiveConnection::toStart(void)
{
    sendCommand("START");
}

void FiveConnection::toSync(QPoint loc, bool exist, bool is_white)
{
    sendCommand("SYNC", QStringList()
                << QString::number(loc.x())
                << QString::number(loc.y())
                << QString::number(exist)
                << QString::number(is_white));
}

void FiveConnection::toRequestUndo(int ref_id)
{
    sendCommand("UNDORQ", QStringList()
                << QString::number(ref_id));
}

void FiveConnection::toReplyUndo(int ref_id, bool accepted)
{
    sendCommand("UNDORP", QStringList()
                << QString::number(ref_id)
                << QString::number(accepted));
}

void FiveConnection::toRequestGiveUp(void)
{
    sendCommand("GIVEUPRQ");
}

void FiveConnection::toReplyGiveUp(bool accepted)
{
    sendCommand("GIVEUPRP", QStringList()
                << QString::number(accepted));
}

void FiveConnection::toChangeTurn(bool is_white)
{
    sendCommand("CHANGE", QStringList()
                << QString::number(is_white));
}

void FiveConnection::toSetColor(bool is_white)
{
    sendCommand("COLOR", QStringList()
                << QString::number(is_white));
}

void FiveConnection::toDisconnect(void)
{
    disconnect();
}

void FiveConnection::toFinish(bool win)
{
    sendCommand("FINISH", QStringList()
                << QString::number(win));
}

void FiveConnection::toCheck(QString checksum)
{
    sendCommand("CHECK", QStringList()
                << checksum);
}

void FiveConnection::toCreateToken(void)
{
    sendCommand("CREATE");
}
