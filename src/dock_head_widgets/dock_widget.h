#ifndef DOCK_WIDGET_H
#define DOCK_WIDGET_H

#include <QDockWidget>
#include <QTimerEvent>
#include <QPalette>
#include <QIcon>
#include <QDebug>

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent = NULL);

private:
    bool            state, _mark;
    int             timerId;
    QPalette        standard;
    QBrush          lightBrush, darkBrush, midBrush;

public slots:
    void            _setFloating(bool);
    void            changeWarningState(bool);

private slots:
    void            timerEvent(QTimerEvent*);
};

#endif // DOCK_WIDGET_H
