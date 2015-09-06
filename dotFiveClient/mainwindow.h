#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fiveboard.h"
#include "fiveconstants.h"
#include "fiveconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    FiveConnection *m_con;
    FiveBoard *m_board;

public slots:
    void showWelcome(void);
    void showSetting(void);
    void showStart(void);
    void showMain(void);
    void showRequest(void);
    void showDisconnected(void);
    void hideAll(void);

};

#endif // MAINWINDOW_H
