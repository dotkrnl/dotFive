#include "fiveboard.h"

#include <QtDebug>

FiveBoard::FiveBoard(QObject *parent)
    : QObject(parent)
{
    toClear();
}

FiveBoard::FiveBoard(const FiveBoard &ot, QObject *parent)
    : QObject(parent)
{
    m_board = ot.m_board;
}

const FiveBoard &FiveBoard::operator= (const FiveBoard &ot)
{
    for (int i = 0; i < five::BOARD_SIZE; i++)
        for (int j = 0; j < five::BOARD_SIZE; j++) {
            QPoint loc(i, j);
            if (ot.isStatus(loc,
                            isExistAt(loc),
                            isWhiteAt(loc))) continue;

            bool exist = ot.isExistAt(loc);
            bool is_white = ot.isWhiteAt(loc);
            m_board[i][j] = QPair<bool, bool>(exist, is_white);
            emit changed(loc, exist, is_white);
        }

    for (int i = 0; i < five::BOARD_SIZE; i++)
        for (int j = 0; j < five::BOARD_SIZE; j++)
            checkWon(QPoint(i, j));

    return *this;
}

bool FiveBoard::isStatus(QPoint loc, bool exist, bool is_white) const
{
    int x = loc.x(), y = loc.y();
    if (!exist) return !m_board[x][y].first;
    else return m_board[x][y].first &&
            m_board[x][y].second == is_white;
}

bool FiveBoard::isInRange(QPoint loc) const
{
    int x = loc.x(), y = loc.y();
    return (x >= 0 && y >= 0 &&
            x < five::BOARD_SIZE &&
            y < five::BOARD_SIZE);
}

void FiveBoard::toChange(QPoint loc, bool exist, bool is_white)
{
    if (!isInRange(loc)) {
        qWarning() << "change outside board:" << loc;
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

void FiveBoard::checkWon(QPoint loc) const
{
    if (!isExistAt(loc)) return;

    bool is_won = sameCount(loc, QPoint(0, 1)) >= 5
            ||    sameCount(loc, QPoint(1, 0)) >= 5
            ||    sameCount(loc, QPoint(1, 1)) >= 5
            ||    sameCount(loc, QPoint(1,-1)) >= 5;

    if (is_won) emit won(isWhiteAt(loc));
}

int FiveBoard::sameCount(QPoint loc, QPoint step) const
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
