#ifndef BAR_NAME_LABEL_H
#define BAR_NAME_LABEL_H

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>

class BarNameLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BarNameLabel(QWidget *parent = Q_NULLPTR);

private:
    int _size   = 0;
    uint _color = 0x000000;

public slots:
    void setColor(uint);

private slots:
    void paintEvent(QPaintEvent*);
};

#endif // BAR_NAME_LABEL_H
