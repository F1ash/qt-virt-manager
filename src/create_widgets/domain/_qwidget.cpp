#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent, virConnectPtr conn, virDomainPtr domain) :
    _Changed(parent), currWorkConnect(conn), currDomain(domain)
{
}

/* public slots */
QDomDocument _QWidget::getDevDocument() const
{
    return QDomDocument();
}
void _QWidget::setDeviceData(QString &xmlDesc)
{

}
