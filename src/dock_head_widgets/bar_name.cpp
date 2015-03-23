#include "bar_name.h"

BarNameLabel::BarNameLabel(QWidget *parent) : QLabel(parent)
{
    _size = this->fontInfo().pixelSize();
    setMaximumWidth(_size*2);
    setMinimumHeight(_size*10);
}

/* public slots */
void BarNameLabel::setColor(int color)
{
    _color = color;
}

/* private slots */
void BarNameLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(_color);
    painter.translate(_size, size().height());
    painter.rotate(270);
    painter.drawText(0, 0, text());
}
