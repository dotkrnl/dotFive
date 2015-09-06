#include "settingscene.h"
#include "ui_settingscene.h"

SettingScene::SettingScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingScene),
    m_click(new QSound(":/sounds/click.wav")),
    m_timer(new QTimer(this)),
    m_current(false)
{
    ui->setupUi(this);
    ui->closeButton->hide();
    ui->saveButton->hide();
    ui->timeDisplay->hide();

    connect(ui->token, SIGNAL(returnPressed()),
            this, SLOT(doConnect()));
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            this, SLOT(doConnect()));
    connect(ui->saveButton, SIGNAL(clicked(bool)),
            this, SLOT(doSave()));
    connect(ui->closeButton, SIGNAL(clicked(bool)),
            this, SIGNAL(shouldHide()));
    connect(ui->backToWelcome, SIGNAL(clicked(bool)),
            this, SIGNAL(shouldToWelcome()));
    connect(ui->token, SIGNAL(returnPressed()),
            m_click, SLOT(play()));
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->saveButton, SIGNAL(pressed()),
            m_click, SLOT(play()));
    connect(ui->closeButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->backToWelcome, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));

    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(increaseTime()));
}

SettingScene::~SettingScene()
{
    delete ui;
}

void SettingScene::setBoard(FiveBoard *board)
{
    m_board = board;
}

void SettingScene::token(QString t)
{
    ui->token->setText(t);
    ui->token->setEnabled(false);
    ui->connectButton->hide();
    ui->backToWelcome->hide();
    ui->timeDisplay->show();
    ui->closeButton->show();
    ui->saveButton->show();
}

void SettingScene::doConnect(void)
{
    emit shouldConnect(ui->token->text());
}

void SettingScene::doSave(void)
{
    QString name =
        QDateTime::currentDateTime()
            .toString("yyyy.MM.dd.hh.mm.ss");

    QString file_name =
            QFileDialog::getSaveFileName(
                this,
                tr("Save Board"),
                name + ".fb",
                tr("Boards (*.fb)"));

    QFile file(file_name);
    file.open(QIODevice::Truncate |
              QIODevice::WriteOnly |
              QIODevice::Text);
    m_board->toSave(&file);
    file.close();
}

void SettingScene::doLoad(void)
{
    QString file_name =
            QFileDialog::getOpenFileName(
                this,
                tr("Load Board"),
                "",
                tr("Boards (*.fb)"));

    QFile file(file_name);
    file.open(QIODevice::ReadOnly |
              QIODevice::Text);

    connect(m_board, SIGNAL(changed(QPoint,bool,bool)),
            this, SIGNAL(shouldSync(QPoint,bool,bool)));
    m_board->toLoad(&file);
    disconnect(m_board, SIGNAL(changed(QPoint,bool,bool)),
            this, SIGNAL(shouldSync(QPoint,bool,bool)));

    file.close();
}

void SettingScene::changeTime(int time, bool is_white)
{
    if (is_white) {
        m_white_time = time / 1000;
    } else {
        m_black_time = time / 1000;
    }
    displayTime();
}

void SettingScene::changePlayer(bool is_white)
{
    m_current = is_white;
    m_timer->start();
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

void SettingScene::increaseTime(void)
{
    if (m_current) m_white_time++;
    else m_black_time++;
    displayTime();
}

void SettingScene::displayTime(void)
{
    ui->blackTime->setText(toTS(m_black_time));
    ui->whiteTime->setText(toTS(m_white_time));
}
