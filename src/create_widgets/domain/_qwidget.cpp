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
QString _QWidget::getDevMode() const
{
    return QString();
}
QString _QWidget::getDevBus() const
{
    return QString();
}
QString _QWidget::getDevModel() const
{
    return QString();
}
QString _QWidget::getDevManaged() const
{
    return QString();
}
QString _QWidget::getDevDisplay() const
{
    return QString();
}
QString _QWidget::getDevXauth() const
{
    return QString();
}
QString _QWidget::getDevFullScreen() const
{
    return QString();
}
QString _QWidget::getDevPort() const
{
    return QString();
}
QString _QWidget::getDevAutoPort() const
{
    return QString();
}
QString _QWidget::getDevReplaceUser() const
{
    return QString();
}
QString _QWidget::getDevMultiUser() const
{
    return QString();
}
