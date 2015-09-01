#include "fiveconstants.h"

namespace five {

const int BOARD_SIZE =                  15;

const int SPECIAL_POINT_COUNT =         5;
const QPoint SPECIAL_POINTS[SPECIAL_POINT_COUNT] = {
    QPoint( 3,  3), QPoint( 3, 11),
    QPoint(11,  3), QPoint(11, 11),
    QPoint( 7,  7)
};

// server defaults
const QString ADDRESS_ANY =             "%any%";
const QString PORT_DEFAULT =            "5555";
const QString TIMEOUT_DEFAULT =         "5000";
const QString HEARTBEAT_DEFAULT =       "2000";

}
