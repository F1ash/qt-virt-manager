#include "wait_load_label.h"
#include <QPainter>

WaitLoadLabel::WaitLoadLabel(QWidget *parent) :
    QLabel(parent)
{
    angle = 0;
    opacity = 0;
    timerId = 0;
}
void WaitLoadLabel::start()
{
    if ( timerId == 0 ) {
        opacity = 1;
        timerId = startTimer(10);
    };
}
void WaitLoadLabel::stop()
{
    if ( timerId > 0 ) {
        killTimer(timerId);
        angle = 0;
        opacity = 0;
        timerId = 0;
        update();
    };
}

/* private slot */
void WaitLoadLabel::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==timerId ) {
        angle+=3.6;
        update();
    };
    ev->accept();
}
void WaitLoadLabel::paintEvent(QPaintEvent *ev)
{
    ev->accept();
    QPainter painter(this);
    painter.setOpacity(opacity);
    if (timerId) painter.translate(frameRect().width()/2, frameRect().height()/2);
    painter.rotate(angle);
    painter.drawPixmap(
                frameRect(),
                pixmap()->copy());
}
