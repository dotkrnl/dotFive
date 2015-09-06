#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QSound>

enum status_t {
    TO_START,
    WIN,
    LOST,
    WAITING,
    WHITE_WIN,
    BLACK_WIN
};

namespace Ui {
class StartScene;
}

class StartScene : public QWidget
{
    Q_OBJECT

public:
    explicit StartScene(QWidget *parent = 0);
    ~StartScene();

private:
    Ui::StartScene *ui;
    status_t m_current;
    bool m_is_white;

    int m_black_time, m_white_time;

    QSound *m_click;
    QSound *m_win;
    QSound *m_lose;

signals:
    void toStart(void);
    void shouldChangeColor(bool is_white);
    void shouldLoadBoard(void);

public slots:
    void toWin(void);
    void toLost(void);
    void toFinish(bool is_white);
    void toToken(QString token);
    void toChangeColor(bool is_white);
    void toChangeTime(int time, bool is_white);

    void startClicked(void);
    void colorClicked(void);
    void updateStatus(void);
    void displayTime(void);
};

#endif // STARTSCENE_H
