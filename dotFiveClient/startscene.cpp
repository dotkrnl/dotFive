#include "startscene.h"
#include "ui_startscene.h"

StartScene::StartScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScene),
    m_current(TO_START),
    m_is_white(false),
    m_click(new QSound(":/sounds/click.wav")),
    m_win(new QSound(":/sounds/win.wav")),
    m_lose(new QSound(":/sounds/lose.wav"))
{
    ui->setupUi(this);
    ui->colorButton->hide();
    updateStatus();
    connect(ui->statusButton, SIGNAL(clicked(bool)),
            this, SLOT(startClicked()));
    connect(ui->colorButton, SIGNAL(clicked(bool)),
            this, SLOT(colorClicked()));
    connect(ui->loadButton, SIGNAL(clicked(bool)),
            this, SIGNAL(shouldLoadBoard()));
    connect(ui->statusButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->colorButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->loadButton, SIGNAL(pressed()),
            m_click, SLOT(play()));
}

StartScene::~StartScene()
{
    delete ui;
}

void StartScene::toWin(void)
{
    m_current = WIN;
    updateStatus();
    m_win->play();
}

void StartScene::toLost(void)
{
    m_current = LOST;
    updateStatus();
    m_lose->play();
}

void StartScene::toFinish(bool is_white)
{
    m_current = is_white
            ? WHITE_WIN : BLACK_WIN;
    updateStatus();
}

void StartScene::toToken(QString token)
{
    ui->token->setText(token);
}

void StartScene::toChangeColor(bool is_white)
{
    m_is_white = is_white;

    ui->colorButton->show();
    ui->colorButton->setProperty("isWhite", is_white);
    ui->colorButton->style()->unpolish(ui->colorButton);
    ui->colorButton->style()->polish(ui->colorButton);
    ui->colorButton->update();
}

void StartScene::startClicked(void)
{
    m_current = WAITING;
    updateStatus();
    emit toStart();
}

void StartScene::colorClicked(void)
{
    emit shouldChangeColor(! m_is_white);
}

void StartScene::updateStatus(void)
{
    ui->statusButton->setStyleSheet("");
    ui->statusButton->setEnabled(true);
    if (m_current == TO_START) {
        ui->statusButton->setText(QString::fromUtf8("\uF04B"));
        ui->timeDisplay->hide();
    } else if (m_current == WIN) {
        ui->statusButton->setText(QString::fromUtf8("\uF091"));
        ui->timeDisplay->show();
    } else if (m_current == LOST) {
        ui->statusButton->setText(QString::fromUtf8("\uF119"));
        ui->timeDisplay->show();
    } else if (m_current == WAITING) {
        ui->statusButton->setText(QString::fromUtf8("\uF252"));
        ui->statusButton->setEnabled(false);
        ui->timeDisplay->hide();
    } else if (m_current == WHITE_WIN) {
        ui->statusButton->setText(QString::fromUtf8("\uF111 \uF091"));
        ui->statusButton->setStyleSheet("color: #D1D3D5;");
        ui->timeDisplay->show();
    } else if (m_current == BLACK_WIN) {
        ui->statusButton->setText(QString::fromUtf8("\uF111 \uF091"));
        ui->statusButton->setStyleSheet("color: #151719;");
        ui->timeDisplay->show();
    }
}

void StartScene::toChangeTime(int time, bool is_white)
{
    if (is_white) {
        m_white_time = time / 1000;
    } else {
        m_black_time = time / 1000;
    }
    displayTime();
}

static QString toTS(int val)
{
    QString minute = QString::number(val / 60);
    while (minute.length() < 2)
        minute = "0" + minute;
    QString second = QString::number(val % 60);
    while (second.length() < 2)
        second = "0" + second;
    return minute + ":" + second;
}

void StartScene::displayTime(void)
{
    ui->blackTime->setText(toTS(m_black_time));
    ui->whiteTime->setText(toTS(m_white_time));
}

