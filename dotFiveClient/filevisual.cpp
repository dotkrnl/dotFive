#include "fivevisual.h"

namespace five {

const int BOARD_WIDTH =                 300;

const int BOARD_BORDER_SIZE =           2;
const QColor BOARD_BORDER_COLOR =       QColor(0x55, 0x57, 0x59);

const int SPECIAL_POINT_COUNT =         5;
const QPoint SPECIAL_POINTS[SPECIAL_POINT_COUNT] = {
    QPoint( 3,  3), QPoint( 3, 11),
    QPoint(11,  3), QPoint(11, 11),
    QPoint( 7,  7)
};

const int SPECIAL_POINT_SIZE =          8;

const int DOTS_SIZE =                   16;
const QColor DOTS_COLOR[] = {
    QColor(0x15, 0x17, 0x19),
    QColor(0xD1, 0xD3, 0xD5),
};

}
