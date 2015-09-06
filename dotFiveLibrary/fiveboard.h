#ifndef FIVEBOARD_H
#define FIVEBOARD_H

#include <QObject>
#include <QPoint>
#include <QPair>
#include <QVector>
#include <QFile>

#include "fiveconstants.h"

class FiveBoard : public QObject
{

    Q_OBJECT

public:
    explicit FiveBoard(QObject *parent = 0);
    FiveBoard(const FiveBoard &ot, QObject *parent = 0);
    const FiveBoard &operator= (const FiveBoard &ot);

    bool isExistAt(QPoint loc) const { return !isStatus(loc, false, false); }
    bool isBlackAt(QPoint loc) const { return isStatus(loc, true, false); }
    bool isWhiteAt(QPoint loc) const { return isStatus(loc, true, true); }

    bool isStatus(QPoint loc, bool exist, bool is_white) const;
    bool isInRange(QPoint loc) const;

signals:
    void cleared(void);
    void changed(QPoint loc, bool exist, bool is_white);
    void won(bool is_white) const;

public slots:
    void toChange(QPoint loc, bool exist, bool is_white);
    void toClear(void);
    void toSave(QFile *file, bool is_white_now = false);
    void toLoad(QFile *file);

private slots:
    void checkWon(QPoint loc) const;

protected:
    QVector< QVector< QPair<bool, bool> > > m_board;
    int sameCount(QPoint loc, QPoint step) const;

};

#endif // FIVEBOARD_H
