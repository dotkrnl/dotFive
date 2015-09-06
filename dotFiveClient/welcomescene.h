#ifndef WELCOMESCENE_H
#define WELCOMESCENE_H

#include <QWidget>
#include <QSound>

namespace Ui {
class WelcomeScene;
}

class WelcomeScene : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScene(QWidget *parent = 0);
    ~WelcomeScene();

private:
    Ui::WelcomeScene *ui;
    QSound *m_click;

signals:
    void toCreate(void);
    void toToken(void);
    void toMagic(void);
};

#endif // WELCOMESCENE_H
