#include "welcomescene.h"
#include "ui_welcomescene.h"

WelcomeScene::WelcomeScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeScene),
    m_click(new QSound(":/sounds/click.wav"))
{
    ui->setupUi(this);

    connect(ui->createButton, SIGNAL(clicked(bool)),
            this, SIGNAL(toCreate()));
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            this, SIGNAL(toToken()));
    connect(ui->autoButton, SIGNAL(clicked(bool)),
            this, SIGNAL(toMagic()));
    connect(ui->createButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
    connect(ui->autoButton, SIGNAL(clicked(bool)),
            m_click, SLOT(play()));
}

WelcomeScene::~WelcomeScene()
{
    delete ui;
}
