#ifndef H_NAVIGATION_LABEL_H
#define H_NAVIGATION_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>

class HNavigationLabel : public QLabel
{
    Q_OBJECT
public:
    explicit HNavigationLabel(
            QWidget *parent = nullptr);
    void        setUsageStatus(bool);

signals:
    void        released();

private:
    bool        inUsage;
    qreal       opacity;

private slots:
    void        mouseReleaseEvent(QMouseEvent*);
    void        enterEvent(QEvent*);
    void        leaveEvent(QEvent*);
    void        paintEvent(QPaintEvent*);
};

#endif // H_NAVIGATION_LABEL_H
