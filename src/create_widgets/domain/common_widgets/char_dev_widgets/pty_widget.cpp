#include "pty_widget.h"

PtyWidget::PtyWidget(QWidget *parent) :
    _QWidget(parent)
{
    ptyLayout = new QGridLayout();
}
PtyWidget::~PtyWidget()
{
    delete ptyLayout;
    ptyLayout = 0;
}

/* public slots */
QDomNodeList PtyWidget::getNodeList() const
{
    QDomNodeList result;
    return result;
}
