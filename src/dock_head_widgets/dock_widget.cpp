#include "dock_widget.h"

DockWidget::DockWidget(QWidget *parent) : QDockWidget(parent)
{

}

void DockWidget::_setFloating(bool state)
{
    this->setFloating(state);
}
