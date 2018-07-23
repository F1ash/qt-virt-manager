#ifndef WAIT_LOAD_LABEL_H
#define WAIT_LOAD_LABEL_H

#include <QLabel>
#include <QTimerEvent>
#include <QPaintEvent>

class WaitLoadLabel : public QLabel
{
    Q_OBJECT
public:
    explicit WaitLoadLabel(QWidget *parent = Q_NULLPTR);
    void        start();
    void        stop();

private:
    qreal       angle, opacity;
    int         timerId;

private slots:
    void        timerEvent(QTimerEvent*);
    void        paintEvent(QPaintEvent*);
};

#endif // WAIT_LOAD_LABEL_H
