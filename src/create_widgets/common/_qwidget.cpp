#include "_qwidget.h"

_QWidget::_QWidget(QWidget *parent, virConnectPtr *connPtrPtr, virDomainPtr domain) :
    _Changed(parent), ptr_ConnPtr(connPtrPtr), currDomain(domain)
{
}

/* public slots */
QDomDocument _QWidget::getDataDocument() const
{
    return QDomDocument();
}
void _QWidget::setDataDescription(const QString &_xmlDesc)
{
    Q_UNUSED(_xmlDesc);
}
bool _QWidget::closeDataEdit()
{
    return true;
}
void _QWidget::setInitState()
{

}
