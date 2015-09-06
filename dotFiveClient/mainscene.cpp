#include "mainscene.h"
#include "ui_mainscene.h"

MainScene::MainScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScene),
    m_time_counter(new QTimer(this)),
    m_who_am_i(NOT_PLAYER),
    m_click(new QSound(":/sounds/click.wav"))
{
    ui->setupUi(this);
    connect(ui->transferButton, SIGNAL(clicked(bool)),
            this, SLOT(toChangeTurn()));
    connect(ui->menuButton, SIGNAL(clicked(bool)),
            this, SIGNAL(shouldSetting()));
    connect(ui->undoButton, SIGNAL(clicked(bool)),
            this, SLOT(toSendUndo()));
    connect(ui->giveupButton, SIGNAL(clicked(bool)),
            this, SLOT(toSendGiveup()));
    connect(ui->transferButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->menuButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->undoButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->giveupButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));

    connect(this, SIGNAL(remainingTimeChanged(int)),
            this, SLOT(updateRemaining(int)));

    m_time_counter->setInterval(TIMER_INTERVAL);
    connect(m_time_counter, SIGNAL(timeout()),
            this, SLOT(reduceRemaining()));

    connect(ui->boardView, SIGNAL(won()),
            this, SIGNAL(gameWon()));
    connect(ui->boardView, SIGNAL(lost()),
            this, SIGNAL(gameLost()));
    connect(ui->boardView, SIGNAL(finished(bool)),
            this, SIGNAL(gameFinished(bool)));
    connect(ui->boardView, SIGNAL(placed(QPoint,bool,bool)),
            this, SIGNAL(shouldPlace(QPoint,bool,bool)));
}

void MainScene::gameStart(void)
{
    ui->boardView->gameStart();
}

void MainScene::gameFinish(bool is_white)
{
    ui->boardView->gameFinish(is_white);
    m_time_counter->stop();
}

void MainScene::setBoard(FiveBoard *board)
{
    m_board = board;
    ui->boardView->setBoard(m_board);
}

void MainScene::colorChanged(bool is_white)
{
    m_who_am_i = is_white ? PLAYER_WHITE : PLAYER_BLACK;

    ui->colorButton->setProperty("isWhite", is_white);
    ui->colorButton->style()->unpolish(ui->colorButton);
    ui->colorButton->style()->polish(ui->colorButton);
    ui->colorButton->update();

    ui->boardView->colorChanged(is_white);
}

void MainScene::turnChanged(bool is_white)
{
    m_remaining = five::GIVEUP_TIMEOUT_CLIENT;
    emit remainingTimeChanged(m_remaining);
    m_time_counter->start();

    m_current = is_white ? PLAYER_WHITE : PLAYER_BLACK;

    ui->transferButton->setProperty("isWhite", is_white);
    ui->transferButton->style()->unpolish(ui->transferButton);
    ui->transferButton->style()->polish(ui->transferButton);
    ui->transferButton->update();

    ui->boardView->turnChanged(is_white);

    ui->undoButton->setProperty("clickStatus", "no");
    ui->undoButton->style()->unpolish(ui->undoButton);
    ui->undoButton->style()->polish(ui->undoButton);
    ui->undoButton->update();

    ui->giveupButton->setProperty("clickStatus", "no");
    ui->giveupButton->style()->unpolish(ui->giveupButton);
    ui->giveupButton->style()->polish(ui->giveupButton);
    ui->giveupButton->update();

    if (m_current == m_who_am_i) {
        ui->undoButton->setEnabled(true);
        ui->giveupButton->setEnabled(true);
        ui->transferButton->setEnabled(true);
    } else {
        ui->undoButton->setEnabled(false);
        ui->giveupButton->setEnabled(false);
        ui->transferButton->setEnabled(false);
    }
}

void MainScene::toChangeTurn(void)
{
    bool is_white = ui->transferButton
            ->property("isWhite").toBool();

    if ((is_white ? PLAYER_WHITE : PLAYER_BLACK)
            == m_who_am_i)
        emit shouldChangeTurn(!is_white);
}

void MainScene::toSendUndo(void)
{
    if (m_who_am_i != m_current) return;

    if (ui->undoButton->property("clickStatus").toString()
            == "rejected") return;

    emit requestUndo();

    ui->undoButton->setProperty("clickStatus", "sent");
    ui->undoButton->style()->unpolish(ui->undoButton);
    ui->undoButton->style()->polish(ui->undoButton);
    ui->undoButton->update();
}

void MainScene::toSendGiveup(void)
{
    if (m_who_am_i != m_current) return;

    if (ui->giveupButton->property("clickStatus").toString()
            == "rejected") return;

    emit requestGiveup();

    ui->giveupButton->setProperty("clickStatus", "sent");
    ui->giveupButton->style()->unpolish(ui->giveupButton);
    ui->giveupButton->style()->polish(ui->giveupButton);
    ui->giveupButton->update();
}

void MainScene::undoReplied(bool accepted)
{
    if (accepted)
        ui->undoButton->setProperty("clickStatus", "no");
    else
        ui->undoButton->setProperty("clickStatus", "rejected");

    ui->undoButton->style()->unpolish(ui->undoButton);
    ui->undoButton->style()->polish(ui->undoButton);
    ui->undoButton->update();
}

void MainScene::giveupReplied(bool accepted)
{
    if (accepted)
        ui->giveupButton->setProperty("clickStatus", "no");
    else
        ui->giveupButton->setProperty("clickStatus", "rejected");

    ui->giveupButton->style()->unpolish(ui->giveupButton);
    ui->giveupButton->style()->polish(ui->giveupButton);
    ui->giveupButton->update();
}

void MainScene::showEvent(QShowEvent *event)
{
    fitTheScene();
    QWidget::showEvent(event);
}

void MainScene::resizeEvent(QResizeEvent *event)
{
    fitTheScene();
    QWidget::resizeEvent(event);
}

void MainScene::reduceRemaining(void)
{
    if (m_remaining <= 0) return;

    m_remaining -= TIMER_INTERVAL;
    emit remainingTimeChanged(m_remaining);

    if (m_remaining <= 0) toChangeTurn();
}

void MainScene::updateRemaining(int in_ms)
{
    QString second = QString::number(in_ms / TIMER_INTERVAL);
    while (second.length() < 2) second = "0" + second;
    ui->remainingTime->setText("00:"+second);
}

void MainScene::fitTheScene(void)
{
    ui->boardView->fitInView(0, 0,
                             five::BOARD_WIDTH,
                             five::BOARD_WIDTH,
                             Qt::KeepAspectRatio);
}

MainScene::~MainScene()
{
    delete ui;
}
