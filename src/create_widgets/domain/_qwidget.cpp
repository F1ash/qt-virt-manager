#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent) :
    QWidget(parent)
{
}
QDomNodeList _QWidget::getNodeList() const
{
    return QDomNodeList();
}
