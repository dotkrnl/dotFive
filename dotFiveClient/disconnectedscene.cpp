#include "disconnectedscene.h"
#include "ui_disconnectedscene.h"

DisconnectedScene::DisconnectedScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisconnectedScene)
{
    ui->setupUi(this);
}

DisconnectedScene::~DisconnectedScene()
{
    delete ui;
}
