#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent, virConnectPtr conn, virDomainPtr domain) :
    QWidget(parent), currWorkConnect(conn), currDomain(domain)
{
}
QDomDocument _QWidget::getDevDocument() const
{
    return QDomDocument();
}
