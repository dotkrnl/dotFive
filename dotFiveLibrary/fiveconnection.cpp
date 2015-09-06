#include "fiveconnection.h"

#include <QtDebug>

FiveConnection::FiveConnection(QTcpSocket *socket,
                               int heartbeat, int timeout,
                               QObject *parent)
    : QObject(parent),
      m_disconnected(false),
      m_con(new LineSocket(socket, heartbeat, timeout, this))
{
    qDebug() << m_con->m_id << "  five connection created";

    connect(this,  SIGNAL(sendCommand(QString,QStringList)),
            m_con, SLOT(sendCommand(QString,QStringList)));
    connect(m_con, SIGNAL(receivedCommand(QString,QStringList)),
            this,  SLOT(decodeCommand(QString,QStringList)));

    connect(this,  SIGNAL(shouldDisconnect()),
            m_con, SLOT(disconnect()));
    connect(m_con, SIGNAL(disconnected()),
            this,  SLOT(disconnectEvent()));
    if (m_con->m_disconnected)
        disconnectEvent();
}

FiveConnection::~FiveConnection(void)
{
    qDebug() << m_con->m_id << "  five connection deleted";
}

void FiveConnection::decodeCommand(QString command,
                                   QStringList argv)
{
    #define ERROR if (true) {\
        qWarning() << m_con->m_id << "  bad command";\
        return;\
    }
    #define SHOULD_SIZE(N) if (argv.size() != (N)) ERROR

    qDebug() << m_con->m_id << "  command:" << command;

    if (command == "TOKEN") {
        SHOULD_SIZE(1);
        emit token(argv[0]);

    } else if (command == "START") {
        SHOULD_SIZE(0);
        emit started();

    } else if (command == "COLOR") {
        SHOULD_SIZE(1);
        emit setColor(bool(argv[0].toInt()));

    } else if (command == "CHANGE") {
        SHOULD_SIZE(1);
        emit changeTurn(bool(argv[0].toInt()));

    } else if (command == "SYNC") {
        SHOULD_SIZE(4);
        emit needSync(QPoint(argv[0].toInt(),
                             argv[1].toInt()),
                      bool(argv[2].toInt()),
                      bool(argv[3].toInt()));

    } else if (command == "TIME") {
        SHOULD_SIZE(2);
        emit timeChanged(argv[0].toInt(),
                         bool(argv[1].toInt()));

    } else if (command == "UNDORQ") {
        SHOULD_SIZE(0);
        emit requestedUndo();

    } else if (command == "UNDORP") {
        SHOULD_SIZE(1);
        emit repliedUndo(bool(argv[0].toInt()));

    } else if (command == "GIVEUPRQ") {
        SHOULD_SIZE(0);
        emit requestedGiveUp();

    } else if (command == "GIVEUPRP") {
        SHOULD_SIZE(1);
        emit repliedGiveUp(bool(argv[0].toInt()));

    } else if (command == "FINISH") {
        SHOULD_SIZE(1);
        emit finished(bool(argv[0].toInt()));

    } else if (command == "CHECK") {
        SHOULD_SIZE(1);
        emit needCheck(argv[0]);

    } else if (command == "CREATE") {
        SHOULD_SIZE(0);
        emit needCreateToken();

    } else if (command == "MAGIC") {
        SHOULD_SIZE(0);
        emit needMagic();

    } else if (command == "ERROR") {
        emit error(argv.join(" "));

    } else {
        toCheck("badchecksum");
        ERROR;
    }
}

void FiveConnection::toToken(QString token)
{
    emit sendCommand("TOKEN", QStringList()
                << token);
}

void FiveConnection::toStart(void)
{
    emit sendCommand("START", QStringList());
}

void FiveConnection::toSync(QPoint loc, bool exist, bool is_white)
{
    emit sendCommand("SYNC", QStringList()
                << QString::number(loc.x())
                << QString::number(loc.y())
                << QString::number(exist)
                << QString::number(is_white));
}

void FiveConnection::toRequestUndo(void)
{
    emit sendCommand("UNDORQ", QStringList());
}

void FiveConnection::toReplyUndo(bool accepted)
{
    emit sendCommand("UNDORP", QStringList()
                << QString::number(accepted));
}

void FiveConnection::toRequestGiveUp(void)
{
    emit sendCommand("GIVEUPRQ", QStringList());
}

void FiveConnection::toReplyGiveUp(bool accepted)
{
    emit sendCommand("GIVEUPRP", QStringList()
                << QString::number(accepted));
}

void FiveConnection::toChangeTurn(bool is_white)
{
    emit sendCommand("CHANGE", QStringList()
                << QString::number(is_white));
}

void FiveConnection::toSetColor(bool is_white)
{
    emit sendCommand("COLOR", QStringList()
                << QString::number(is_white));
}

void FiveConnection::toDisconnect(void)
{
    emit disconnect();
}

void FiveConnection::toChangeTime(int time, bool is_white)
{
    emit sendCommand("TIME", QStringList()
                << QString::number(time)
                << QString::number(is_white));
}

void FiveConnection::toFinish(bool winner_is_white)
{
    emit sendCommand("FINISH", QStringList()
                << QString::number(winner_is_white));
}

void FiveConnection::toCheck(QString checksum)
{
    emit sendCommand("CHECK", QStringList()
                << checksum);
}

void FiveConnection::toCreateToken(void)
{
    emit sendCommand("CREATE", QStringList());
}

void FiveConnection::toMagic(void)
{
    emit sendCommand("MAGIC", QStringList());
}

void FiveConnection::toError(QString error)
{
    emit sendCommand("ERROR", QStringList()
                << error);
}

void FiveConnection::disconnectEvent(void)
{
    m_disconnected = true;
    emit disconnected();
}
