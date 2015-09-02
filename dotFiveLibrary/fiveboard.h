#ifndef FIVEBOARD_H
#define FIVEBOARD_H

#include <QObject>
#include <QPoint>
#include <QPair>
#include <QVector>

#include "fiveconstants.h"

class FiveBoard : public QObject
{

    Q_OBJECT

public:
    explicit FiveBoard(QObject *parent = 0);

    bool isExistAt(QPoint loc) { return !isStatus(loc, false, false); }
    bool isBlackAt(QPoint loc) { return isStatus(loc, true, false); }
    bool isWhiteAt(QPoint loc) { return isStatus(loc, true, true); }

    bool isStatus(QPoint loc, bool exist, bool is_white);
    bool isInRange(QPoint loc);

signals:
    void cleared(void);
    void changed(QPoint loc, bool exist, bool is_white);
    void won(bool is_white);

public slots:
    void toChange(QPoint loc, bool exist, bool is_white);
    void toClear(void);

private slots:
    void checkWon(QPoint loc);

protected:
    QVector< QVector< QPair<bool, bool> > > m_board;
    int sameCount(QPoint loc, QPoint step);

};

#endif // FIVEBOARD_H
