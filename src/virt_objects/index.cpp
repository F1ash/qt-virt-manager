#include "index.h"

Index::Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    Autostart = "";
    Persistent = "";
}
void Index::setName(const QString &s)
{
    Name = s;
}
void Index::setState(const QString &s)
{
    State = s;
}
void Index::setAutostart(const QString &s)
{
    Autostart = s;
}
void Index::setPersistent(const QString &s)
{
    Persistent = s;
}
QString Index::getName() const
{
    return Name;
}
QString Index::getState() const
{
    return State;
}
QString Index::getAutostart() const
{
    return Autostart;
}
QString Index::getPersistent() const
{
    return Persistent;
}
