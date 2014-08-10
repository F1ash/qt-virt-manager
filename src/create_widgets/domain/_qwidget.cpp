#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent, virConnectPtr conn, virDomainPtr domain) :
    QWidget(parent), currWorkConnect(conn), currDomain(domain)
{
}
QDomNodeList _QWidget::getNodeList() const
{
    return QDomNodeList();
}
QString _QWidget::getDevType() const
{
    return QString();
}
