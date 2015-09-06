#ifndef DISCONNECTEDSCENE_H
#define DISCONNECTEDSCENE_H

#include <QWidget>

namespace Ui {
class DisconnectedScene;
}

class DisconnectedScene : public QWidget
{
    Q_OBJECT

public:
    explicit DisconnectedScene(QWidget *parent = 0);
    ~DisconnectedScene();

private:
    Ui::DisconnectedScene *ui;
};

#endif // DISCONNECTEDSCENE_H
