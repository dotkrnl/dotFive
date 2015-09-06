#ifndef FIVEBOARDVIEW_H
#define FIVEBOARDVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QVector>
#include <QSound>

#include "fiveconstants.h"
#include "fivevisual.h"
#include "fiveboard.h"

#define NOT_PLAYER      (-1)
#define PLAYER_BLACK    (0)
#define PLAYER_WHITE    (1)

#define PLAYER_FIRST    PLAYER_BLACK

typedef int player_t;

class FiveBoardView : public QGraphicsView
{
    Q_OBJECT

public:
    FiveBoardView(QWidget *parent = 0);
    void setBoard(FiveBoard *b);

signals:
    void won();
    void lost();
    void finished(bool is_white);
    void placed(QPoint loc, bool exist, bool is_white);

public slots:
    void gameStart(void);
    void gameFinish(bool is_white);
    void colorChanged(bool is_white);
    void turnChanged(bool is_white);

protected:
    FiveBoard *m_board;

    player_t m_who_to_play;
    player_t m_who_am_i;

    QSound *m_click;

    QVector< QVector<QGraphicsEllipseItem *> > m_dots;

    void drawBoard(void);
    void drawDots(void);

    bool isWhiteAt(QPoint loc);

    void drawDot(QPoint loc, bool is_white);
    void clearDot(QPoint loc);

    void mouseReleaseEvent(QMouseEvent *event);

    QPoint decodeLocation(QPoint mouse);
    QPoint encodeLocation(QPoint location);

    QGraphicsEllipseItem *
        addRound(QPoint center, int diameter, QBrush brush);

protected slots:
    void boardChanged(QPoint loc, bool exist, bool is_white);
    void boardCleared(void);

};

#endif // FIVEBOARDVIEW_H
