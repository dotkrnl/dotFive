#ifndef REQUESTSCENE_H
#define REQUESTSCENE_H

#include <QWidget>
#include <QSound>

namespace Ui {
class RequestScene;
}

class RequestScene : public QWidget
{
    Q_OBJECT

public:
    explicit RequestScene(QWidget *parent = 0);
    ~RequestScene();

private:
    Ui::RequestScene *ui;
    bool is_requesting_undo;

    QSound *m_click;

signals:
    void replyUndo(bool accept);
    void replyGiveup(bool accept);

public slots:
    void requestUndo(void);
    void requestGiveup(void);

    void acceptOffer(void);
    void rejectOffer(void);
};

#endif // REQUESTSCENE_H
