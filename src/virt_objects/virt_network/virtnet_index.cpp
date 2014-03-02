#include "virtnet_index.h"

VirtNetIndex::VirtNetIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    Autostart = "";
    Persistent = "";
}
void VirtNetIndex::setName(const QString &s)
{
    Name = s;
}
void VirtNetIndex::setState(const QString &s)
{
    State = s;
}
void VirtNetIndex::setAutostart(const QString &s)
{
    Autostart = s;
}
void VirtNetIndex::setPersistent(const QString &s)
{
    Persistent = s;
}
QString VirtNetIndex::getName() const
{
    return Name;
}
QString VirtNetIndex::getState() const
{
    return State;
}
QString VirtNetIndex::getAutostart() const
{
    return Autostart;
}
QString VirtNetIndex::getPersistent() const
{
    return Persistent;
}
