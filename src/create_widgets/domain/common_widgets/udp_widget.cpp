#include "udp_widget.h"

UdpWidget::UdpWidget(QWidget *parent) :
    _QWidget(parent)
{

    udpLayout = new QGridLayout();
}
UdpWidget::~UdpWidget()
{

}

/* public slots */
QDomNodeList UdpWidget::getNodeList() const
{
    QDomNodeList result;
    return result;
}
