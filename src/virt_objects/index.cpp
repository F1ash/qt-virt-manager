#include "index.h"

Index::Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = false;
    State_EXT = "";
    Autostart = false;
    Persistent = false;
    onView = false;
}
void Index::setName(const QString &s)
{
    Name = s;
}
void Index::setState(const bool state)
{
    State = state;
}
void Index::setState_EXT(const QString &s)
{
    State_EXT = s;
}
void Index::setAutostart(const bool state)
{
    Autostart = state;
}
void Index::setPersistent(const bool state)
{
    Persistent = state;
}
void Index::setOnView(const bool state)
{
    onView = state;
}
QString Index::getName() const
{
    return Name;
}
bool Index::getState() const
{
    return State;
}
QString Index::getState_EXT() const
{
    return State_EXT;
}
bool Index::getAutostart() const
{
    return Autostart;
}
bool Index::getPersistent() const
{
    return Persistent;
}
bool    Index::getOnView() const
{
    return onView;
}
