#include "fiveboard.h"

#include <QDebug>

FiveBoard::FiveBoard(QObject *parent)
    : QObject(parent)
{
    toClear();
}

bool FiveBoard::isStatus(QPoint loc, bool exist, bool is_white)
{
    int x = loc.x(), y = loc.y();
    if (!exist) return !m_board[x][y].first;
    else return m_board[x][y].first &&
            m_board[x][y].second == is_white;
}

bool FiveBoard::isInRange(QPoint loc)
{
    int x = loc.x(), y = loc.y();
    return (x >= 0 && y >= 0 &&
            x < five::BOARD_SIZE &&
            y < five::BOARD_SIZE);
}

void FiveBoard::toChange(QPoint loc, bool exist, bool is_white)
{
    if (!isInRange(loc)) {
        qInfo() << "change outside board:" << loc;
        return;
    }

    if (!exist) is_white = false;

    m_board[loc.x()][loc.y()] =
            QPair<bool, bool>(exist, is_white);
    emit changed(loc, exist, is_white);

    checkWon(loc);
}

void FiveBoard::toClear(void)
{
    m_board.resize(0);
    m_board.resize(five::BOARD_SIZE);
    for (int i = 0; i < five::BOARD_SIZE; i++)
        m_board[i].resize(five::BOARD_SIZE);
    emit cleared();
}

void FiveBoard::checkWon(QPoint loc)
{
    if (!isExistAt(loc)) return;

    bool is_won = sameCount(loc, QPoint(0, 1)) >= 5
            ||    sameCount(loc, QPoint(1, 0)) >= 5
            ||    sameCount(loc, QPoint(1, 1)) >= 5
            ||    sameCount(loc, QPoint(1,-1)) >= 5;

    if (is_won) emit won(isWhiteAt(loc));
}

int FiveBoard::sameCount(QPoint loc, QPoint step)
{
    int count = -1;
    bool e = isExistAt(loc), w = isWhiteAt(loc);

    for (QPoint now = loc; isInRange(now); now += step) {
        if (!isStatus(now, e, w)) break;
        else count = count + 1;
    }
    for (QPoint now = loc; isInRange(now); now -= step) {
        if (!isStatus(now, e, w)) break;
        else count = count + 1;
    }

    return count;
}
