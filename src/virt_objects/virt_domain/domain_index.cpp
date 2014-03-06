#include "domain_index.h"

DomainIndex::DomainIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    Autostart = "";
    Persistent = "";
}
void DomainIndex::setName(const QString &s)
{
    Name = s;
}
void DomainIndex::setState(const QString &s)
{
    State = s;
}
void DomainIndex::setAutostart(const QString &s)
{
    Autostart = s;
}
void DomainIndex::setPersistent(const QString &s)
{
    Persistent = s;
}
QString DomainIndex::getName() const
{
    return Name;
}
QString DomainIndex::getState() const
{
    return State;
}
QString DomainIndex::getAutostart() const
{
    return Autostart;
}
QString DomainIndex::getPersistent() const
{
    return Persistent;
}
