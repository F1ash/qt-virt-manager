#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent, virConnectPtr *connPtr, virDomainPtr domain) :
    _Changed(parent), currConnPtr(connPtr), currDomain(domain)
{
}

/* public slots */
QDomDocument _QWidget::getDataDocument() const
{
    return QDomDocument();
}
void _QWidget::setDataDescription(QString &xmlDesc)
{

}
QString _QWidget::closeDataEdit()
{
    return QString();
}
void _QWidget::setInitState()
{

}
