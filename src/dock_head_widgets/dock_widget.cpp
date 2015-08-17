#include "dock_widget.h"

DockWidget::DockWidget(QWidget *parent) : QDockWidget(parent)
{
    state = false;
    _mark = false;
    standard = palette();
    lightBrush = standard.light();
    darkBrush  = standard.dark();
    midBrush   = standard.brush(
                standard.currentColorGroup(),
                QPalette::Window);
    timerId = 0;
}

void DockWidget::_setFloating(bool state)
{
    this->setFloating(state);
}
void DockWidget::changeWarningState(bool state)
{
    if ( state ) {
        if ( timerId==0 ) timerId = startTimer(1000);
    } else {
        if ( timerId>0 ) {
            killTimer(timerId);
            timerId = 0;
        };
        standard.setBrush(
                    standard.currentColorGroup(),
                    QPalette::Window,
                    midBrush);
        setPalette(standard);
        setWindowTitle("Log");
    };
}

void DockWidget::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() ) {
        standard.setBrush(
                    standard.currentColorGroup(),
                    QPalette::Window,
                    (_mark)? lightBrush : darkBrush);
        setPalette(standard);
        setWindowTitle((_mark)? "Log" : "long");
        _mark=!_mark;
    };
}
