#include "requestscene.h"
#include "ui_requestscene.h"

RequestScene::RequestScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestScene),
    m_click(new QSound(":/sounds/click.wav"))
{
    ui->setupUi(this);
    connect(ui->acceptButton, SIGNAL(clicked(bool)),
            this, SLOT(acceptOffer()));
    connect(ui->rejectButton, SIGNAL(clicked(bool)),
            this, SLOT(rejectOffer()));
    connect(ui->acceptButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->rejectButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
}

RequestScene::~RequestScene()
{
    delete ui;
}

void RequestScene::requestUndo(void)
{
    is_requesting_undo = true;
    ui->content->setText(QString::fromUtf8("\uF1DA ?"));
}

void RequestScene::requestGiveup(void)
{
    is_requesting_undo = false;
    ui->content->setText(QString::fromUtf8("\uF091 ?"));
}

void RequestScene::acceptOffer(void)
{
    if (is_requesting_undo) {
        emit replyUndo(true);
    } else {
        emit replyGiveup(true);
    }
}

void RequestScene::rejectOffer(void)
{
    if (is_requesting_undo) {
        emit replyUndo(false);
    } else {
        emit replyGiveup(false);
    }
}
