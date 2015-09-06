#include "fiveboardview.h"

FiveBoardView::FiveBoardView(QWidget *parent)
    : QGraphicsView(parent),
      m_who_to_play(NOT_PLAYER),
      m_who_am_i(NOT_PLAYER),
      m_click(new QSound(":/sounds/click.wav"))
{
    m_dots.resize(five::BOARD_SIZE);
    for (int i = 0; i < five::BOARD_SIZE; i++)
        m_dots[i].resize(five::BOARD_SIZE);

    int step = five::BOARD_WIDTH / five::BOARD_SIZE;
    int offset = step / 2;

    setRenderHints(
            QPainter::Antialiasing
          | QPainter::TextAntialiasing);
    translate(offset, offset);

    setScene(new QGraphicsScene(this));

    drawBoard();
}

void FiveBoardView::gameStart(void)
{
    m_click->play();
}

void FiveBoardView::gameFinish(bool is_white)
{
    player_t winner = is_white ?
                PLAYER_WHITE : PLAYER_BLACK;

    m_board->toClear();
    if (m_who_am_i == NOT_PLAYER)
        emit finished(is_white);
    else if (m_who_am_i == winner)
        emit won();
    else
        emit lost();
}

void FiveBoardView::setBoard(FiveBoard *b)
{
    m_board = b;
    connect(m_board, SIGNAL(changed(QPoint,bool,bool)),
            this,    SLOT(boardChanged(QPoint,bool,bool)));
    connect(m_board, SIGNAL(cleared()),
            this,    SLOT(boardCleared()));
    drawDots();
}

void FiveBoardView::drawBoard(void)
{
    QGraphicsScene *s = scene();
    int step = five::BOARD_WIDTH / five::BOARD_SIZE;
    int width = step * (five::BOARD_SIZE - 1);

    QPen boardPen(five::BOARD_BORDER_COLOR);
    QBrush boardBrush(five::BOARD_BORDER_COLOR);
    boardPen.setJoinStyle(Qt::RoundJoin);
    boardPen.setCapStyle(Qt::RoundCap);
    boardPen.setWidth(five::BOARD_BORDER_SIZE);

    for (int i = 0; i < five::BOARD_SIZE; i++) {
        int line_loc = i * step;
        s->addLine(0,       line_loc,
                   width,   line_loc,
                   boardPen);
    }

    for (int i = 0; i < five::BOARD_SIZE; i++) {
        int line_loc = i * step;
        s->addLine(line_loc, 0,
                   line_loc, width,
                   boardPen);
    }

    for (int i = 0; i < five::SPECIAL_POINT_COUNT; i++) {
        addRound(encodeLocation(five::SPECIAL_POINTS[i]),
                 five::SPECIAL_POINT_SIZE, boardBrush);
    }

    // tricky hack for stable display
    addRound(QPoint(-step / 2, -step / 2),
             1, Qt::NoBrush);
    addRound(QPoint(five::BOARD_WIDTH - step / 2,
                    five::BOARD_WIDTH - step / 2),
             1, Qt::NoBrush);
}

void FiveBoardView::drawDots(void)
{
    for (int i = 0; i < five::BOARD_SIZE; i++)
        for (int j = 0; j < five::BOARD_SIZE; j++) {
            QPoint location(i, j);
            boardChanged(location,
                         m_board->isExistAt(location),
                         m_board->isWhiteAt(location));
        }
}

void FiveBoardView::boardChanged(QPoint loc, bool exist, bool is_white)
{
    clearDot(loc);
    if (exist) drawDot(loc, is_white);
}

void FiveBoardView::boardCleared(void)
{
    for (int i = 0; i < five::BOARD_SIZE; i++)
        for (int j = 0; j < five::BOARD_SIZE; j++)
            clearDot(QPoint(i, j));
}

void FiveBoardView::drawDot(QPoint loc, bool is_white)
{
    if (m_dots[loc.x()][loc.y()]) return;
    m_dots[loc.x()][loc.y()] = addRound(
                encodeLocation(loc),
                five::DOTS_SIZE,
                five::DOTS_COLOR[is_white]);
}

void FiveBoardView::clearDot(QPoint loc)
{
    if (m_dots[loc.x()][loc.y()]) {
        scene()->removeItem(m_dots[loc.x()][loc.y()]);
        delete m_dots[loc.x()][loc.y()];
        m_dots[loc.x()][loc.y()] = NULL;
    }
}

void FiveBoardView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_who_am_i == NOT_PLAYER
            || m_who_to_play != m_who_am_i) return;

    QPoint loc = decodeLocation(event->pos());
    if (m_board->isExistAt(loc)) return;

    m_board->toChange(loc, true, m_who_am_i == PLAYER_WHITE);
    placed(loc, true, m_who_am_i == PLAYER_WHITE);
    m_click->play();
    turnChanged(m_who_am_i == PLAYER_WHITE
                ? PLAYER_BLACK : PLAYER_WHITE);
}

QPoint FiveBoardView::decodeLocation(QPoint mouse)
{
    double ratio_x = double(five::BOARD_WIDTH) / this->width();
    double ratio_y = double(five::BOARD_WIDTH) / this->height();

    int canvas_x = mouse.x() * ratio_x;
    int canvas_y = mouse.y() * ratio_y;

    int step = five::BOARD_WIDTH / five::BOARD_SIZE;
    int x = canvas_x / step;
    int y = canvas_y / step;
    return QPoint(x, y);
}

QPoint FiveBoardView::encodeLocation(QPoint location)
{
    int step = five::BOARD_WIDTH / five::BOARD_SIZE;
    int center_x = location.x() * step;
    int center_y = location.y() * step;
    return QPoint(center_x, center_y);
}

QGraphicsEllipseItem *
    FiveBoardView::addRound(QPoint center, int diameter, QBrush brush)
{
    QGraphicsScene *s = scene();
    if (diameter & 1) diameter++; // even number fix
    return s->addEllipse(
            center.x() - diameter / 2,
            center.y() - diameter / 2,
            diameter, diameter, Qt::NoPen, brush);
}

void FiveBoardView::colorChanged(bool is_white)
{
    m_who_am_i = is_white ? PLAYER_WHITE : PLAYER_BLACK;
}

void FiveBoardView::turnChanged(bool is_white)
{
    m_who_to_play = is_white ? PLAYER_WHITE : PLAYER_BLACK;
}
