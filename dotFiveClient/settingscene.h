#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H

#include <QWidget>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QSound>
#include <QTimer>

#include "fiveboard.h"

namespace Ui {
class SettingScene;
}

class SettingScene : public QWidget
{
    Q_OBJECT

public:
    explicit SettingScene(QWidget *parent = 0);
    ~SettingScene();

    void setBoard(FiveBoard *board);

private:
    Ui::SettingScene *ui;
    FiveBoard *m_board;

    QSound *m_click;

    int m_black_time, m_white_time;

    QTimer *m_timer;
    bool m_current;

signals:
    void shouldConnect(QString t);
    void shouldSync(QPoint loc, bool exist, bool is_white);
    void shouldToWelcome(void);
    void shouldHide(void);

public slots:
    void token(QString t);
    void doConnect(void);
    void doSave(void);
    void doLoad(void);
    void changeTime(int time, bool is_white);
    void changePlayer(bool is_white);

    void increaseTime(void);
    void displayTime(void);
};

#endif // SETTINGSCENE_H
