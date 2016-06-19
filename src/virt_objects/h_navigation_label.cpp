#include "h_navigation_label.h"
#include <QPainter>
#include <QDebug>

#define L_OPACITY  0.20

HNavigationLabel::HNavigationLabel(QWidget *parent) :
    QLabel(parent)
{
    inUsage = false;
    opacity = 0;
    setMouseTracking(true);
}

void HNavigationLabel::setUsageStatus(bool state)
{
    inUsage = state;
    setAttribute(Qt::WA_TransparentForMouseEvents, !state);
    if ( !state ) {
        opacity = 0;
        hide();
    } else {
        opacity = L_OPACITY;
        show();
    };
    update();
}

/* private slots */
void HNavigationLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->accept();
    if ( !inUsage ) return;
    if ( ev->type()==QEvent::MouseButtonRelease ) {
        emit released();
    };
}
void HNavigationLabel::enterEvent(QEvent *ev)
{
    ev->accept();
    if        ( ev->type()==QEvent::Enter ) {
        opacity = 1;
        update();
    };
}
void HNavigationLabel::leaveEvent(QEvent *ev)
{
    ev->accept();
    if ( ev->type()==QEvent::Leave ) {
        opacity = L_OPACITY;
        update();
    };
}
void HNavigationLabel::paintEvent(QPaintEvent *ev)
{
    ev->accept();
    QPainter painter(this);
    if ( opacity==1 ) {
        QRadialGradient gradient(
                    rect().center().x(),
                    rect().center().y(),
                    size().width()/2);
        gradient.setColorAt(
                    1,
                    QColor::fromRgb(173, 255, 47, 25)); // green
        gradient.setColorAt(
                    0,
                    QColor::fromRgb(255, 215, 0, 255)); // gold
        QBrush b(gradient);
        painter.setBrush(b);
        painter.setPen(QPen(QBrush(), 0));
        painter.drawEllipse(rect());
    };
    painter.setOpacity(opacity);
    painter.drawPixmap(
                rect(),
                pixmap()->copy());
}
