#ifndef FIVEGLOBAL_H
#define FIVEGLOBAL_H

#include <QString>
#include <QPoint>

namespace five {

const extern int BOARD_SIZE;

const extern int SPECIAL_POINT_COUNT;
const extern QPoint SPECIAL_POINTS[];

// server defaults
const extern QString ADDRESS_ANY;
const extern QString PORT_DEFAULT;
const extern QString TIMEOUT_DEFAULT;
const extern QString HEARTBEAT_DEFAULT;

const extern int GIVEUP_TIMEOUT_CLIENT;
const extern int GIVEUP_TIMEOUT_SERVER;

}

#endif // FIVEGLOBAL_H
