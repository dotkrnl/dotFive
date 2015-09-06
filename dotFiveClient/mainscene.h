#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include <QTimer>
#include <QSound>

#include "fiveconstants.h"
#include "fiveboard.h"

#define TIMER_INTERVAL      (1000)

#define NOT_PLAYER      (-1)
#define PLAYER_BLACK    (0)
#define PLAYER_WHITE    (1)

#define PLAYER_FIRST    PLAYER_BLACK

typedef int player_t;

namespace Ui {
class MainScene;
}

class MainScene : public QWidget
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = 0);
    ~MainScene();

    void setBoard(FiveBoard *board);

signals:
    void gameWon();
    void gameLost();
    void gameFinished(bool is_white);
    void shouldPlace(QPoint loc, bool exist, bool is_white);
    void shouldSetting(void);
    void shouldChangeTurn(bool is_white);
    void requestUndo(void);
    void requestGiveup(void);

    void remainingTimeChanged(int in_ms);

public slots:
    void gameStart(void);
    void gameFinish(bool is_white);
    void colorChanged(bool is_white);
    void turnChanged(bool is_white);
    void toChangeTurn(void);
    void toSendUndo(void);
    void toSendGiveup(void);
    void undoReplied(bool accepted);
    void giveupReplied(bool accepted);

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

protected slots:
    void reduceRemaining(void);
    void updateRemaining(int in_ms);

private:
    Ui::MainScene *ui;
    FiveBoard *m_board;

    QTimer *m_time_counter;
    int m_remaining;

    player_t m_who_am_i;
    player_t m_current;

    QSound *m_click;

    void fitTheScene(void);

};

#endif // MAINSCENE_H
