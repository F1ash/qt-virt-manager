#ifndef H_NAVIGATION_LABEL_H
#define H_NAVIGATION_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>

enum DIRECT {
    PREV    = -1,
    NONE,
    NEXT
};

class HNavigationLabel : public QLabel
{
    Q_OBJECT
public:
    explicit HNavigationLabel(
            QWidget *parent     = Q_NULLPTR,
            DIRECT   direction  = NONE);
    void        setUsageStatus(bool);

signals:
    void        released();

private:
    bool        inUsage;
    qreal       opacity;
    DIRECT      direction;

private slots:
    void        mouseReleaseEvent(QMouseEvent*);
    void        wheelEvent(QWheelEvent*);
    void        enterEvent(QEvent*);
    void        leaveEvent(QEvent*);
    void        paintEvent(QPaintEvent*);
};

#endif // H_NAVIGATION_LABEL_H
