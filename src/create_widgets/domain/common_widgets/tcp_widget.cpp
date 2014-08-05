#include "tcp_widget.h"

TcpWidget::TcpWidget(QWidget *parent) :
    _QWidget(parent)
{

    tcpLayout = new QGridLayout();
}
TcpWidget::~TcpWidget()
{

}

/* public slots */
QDomNodeList TcpWidget::getNodeList() const
{
    QDomNodeList result;
    return result;
}
