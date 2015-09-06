#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_board(new FiveBoard(this))
{
    ui->setupUi(this);
    setCursor(QCursor(Qt::CrossCursor));

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("xuexiao.dotkrnl.com",
                          five::PORT_DEFAULT.toInt());
    socket->waitForConnected(-1); // TODO: fix
    m_con = new FiveConnection(socket,
                               five::HEARTBEAT_DEFAULT.toInt(),
                               five::TIMEOUT_DEFAULT.toInt(),
                               this);

    ui->mainScene->setBoard(m_board);
    ui->settingScene->setBoard(m_board);

    connect(ui->welcomeScene, SIGNAL(toCreate()),
            m_con, SLOT(toCreateToken()));
    connect(ui->welcomeScene, SIGNAL(toToken()),
            this, SLOT(showSetting()));
    connect(ui->welcomeScene, SIGNAL(toMagic()),
            m_con, SLOT(toMagic()));

    connect(ui->settingScene, SIGNAL(shouldConnect(QString)),
            m_con, SLOT(toToken(QString)));
    connect(ui->settingScene, SIGNAL(shouldHide()),
            this, SLOT(showMain()));
    connect(ui->settingScene, SIGNAL(shouldSync(QPoint,bool,bool)),
            m_con, SLOT(toSync(QPoint,bool,bool)));
    connect(ui->settingScene, SIGNAL(shouldToWelcome()),
            this, SLOT(showWelcome()));

    connect(ui->mainScene, SIGNAL(shouldChangeTurn(bool)),
            m_con, SLOT(toChangeTurn(bool)));
    connect(ui->mainScene, SIGNAL(shouldSetting()),
            this, SLOT(showSetting()));
    connect(ui->mainScene, SIGNAL(gameWon()),
            ui->startScene, SLOT(toWin()));
    connect(ui->mainScene, SIGNAL(gameWon()),
            this, SLOT(showStart()));
    connect(ui->mainScene, SIGNAL(gameLost()),
            ui->startScene, SLOT(toLost()));
    connect(ui->mainScene, SIGNAL(gameLost()),
            this, SLOT(showStart()));
    connect(ui->mainScene, SIGNAL(gameFinished(bool)),
            this, SLOT(showStart()));
    connect(ui->mainScene, SIGNAL(gameFinished(bool)),
            ui->startScene, SLOT(toFinish(bool)));
    connect(ui->mainScene, SIGNAL(shouldPlace(QPoint,bool,bool)),
            m_con, SLOT(toSync(QPoint,bool,bool)));
    connect(ui->mainScene, SIGNAL(requestUndo()),
            m_con, SLOT(toRequestUndo()));
    connect(ui->mainScene, SIGNAL(requestGiveup()),
            m_con, SLOT(toRequestGiveUp()));

    connect(ui->startScene, SIGNAL(toStart()),
            m_con, SLOT(toStart()));
    connect(ui->startScene, SIGNAL(shouldChangeColor(bool)),
            m_con, SLOT(toSetColor(bool)));
    connect(ui->startScene, SIGNAL(shouldLoadBoard()),
            ui->settingScene, SLOT(doLoad()));

    connect(ui->requestScene, SIGNAL(replyUndo(bool)),
            m_con, SLOT(toReplyUndo(bool)));
    connect(ui->requestScene, SIGNAL(replyUndo(bool)),
            this, SLOT(showMain()));
    connect(ui->requestScene, SIGNAL(replyGiveup(bool)),
            m_con, SLOT(toReplyGiveUp(bool)));
    connect(ui->requestScene, SIGNAL(replyGiveup(bool)),
            this, SLOT(showMain()));

    connect(m_con, SIGNAL(needSync(QPoint,bool,bool)),
            m_board, SLOT(toChange(QPoint,bool,bool)));
    connect(m_con, SIGNAL(token(QString)),
            ui->settingScene, SLOT(token(QString)));
    connect(m_con, SIGNAL(token(QString)),
            ui->startScene, SLOT(toToken(QString)));
    connect(m_con, SIGNAL(token(QString)),
            this, SLOT(showSetting()));
    connect(m_con, SIGNAL(started()),
            this, SLOT(showMain()));
    connect(m_con, SIGNAL(started()),
            ui->mainScene, SLOT(gameStart()));
    connect(m_con, SIGNAL(finished(bool)),
            ui->mainScene, SLOT(gameFinish(bool)));
    connect(m_con, SIGNAL(setColor(bool)),
            ui->mainScene, SLOT(colorChanged(bool)));
    connect(m_con, SIGNAL(setColor(bool)),
            ui->startScene, SLOT(toChangeColor(bool)));
    connect(m_con, SIGNAL(changeTurn(bool)),
            ui->mainScene, SLOT(turnChanged(bool)));
    connect(m_con, SIGNAL(changeTurn(bool)),
            ui->settingScene, SLOT(changePlayer(bool)));
    connect(m_con, SIGNAL(disconnected()),
            this, SLOT(showDisconnected()));
    connect(m_con, SIGNAL(token(QString)),
            this, SLOT(showStart()));
    connect(m_con, SIGNAL(requestedUndo()),
            ui->requestScene, SLOT(requestUndo()));
    connect(m_con, SIGNAL(requestedUndo()),
            this, SLOT(showRequest()));
    connect(m_con, SIGNAL(requestedGiveUp()),
            ui->requestScene, SLOT(requestGiveup()));
    connect(m_con, SIGNAL(requestedGiveUp()),
            this, SLOT(showRequest()));
    connect(m_con, SIGNAL(timeChanged(int,bool)),
            ui->settingScene, SLOT(changeTime(int,bool)));
    connect(m_con, SIGNAL(timeChanged(int,bool)),
            ui->startScene, SLOT(toChangeTime(int,bool)));
    connect(m_con, SIGNAL(repliedUndo(bool)),
            ui->mainScene, SLOT(undoReplied(bool)));
    connect(m_con, SIGNAL(repliedGiveUp(bool)),
            ui->mainScene, SLOT(giveupReplied(bool)));

    showWelcome();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int width = ui->centralWidget->width();
    int height = ui->centralWidget->height();

    int contentsWidth = width;
    int contentsHeight = width + 53;

    if (height != contentsHeight)
        resize(contentsWidth, contentsHeight);
    else
        QMainWindow::resizeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWelcome(void)
{
    hideAll();
    ui->welcomeScene->show();
}

void MainWindow::showSetting(void)
{
    hideAll();
    ui->settingScene->show();
}

void MainWindow::showStart(void)
{
    hideAll();
    ui->startScene->show();
}

void MainWindow::showMain(void)
{
    hideAll();
    ui->mainScene->show();
}

void MainWindow::showRequest(void)
{
    hideAll();
    ui->requestScene->show();
}

void MainWindow::showDisconnected(void)
{
    hideAll();
    ui->disconnectedScene->show();
}

void MainWindow::hideAll(void)
{
    ui->welcomeScene->hide();
    ui->settingScene->hide();
    ui->mainScene->hide();
    ui->disconnectedScene->hide();
    ui->startScene->hide();
    ui->requestScene->hide();
}
