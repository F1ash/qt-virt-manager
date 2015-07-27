#include "virt_interface_index.h"

Interface_Index::Interface_Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    MAC  = "";
    State = "";
    Changing = "";
    onView = false;
}
void Interface_Index::setName(const QString &s)
{
    Name = s;
}
void Interface_Index::setMAC(const QString &s)
{
    MAC = s;
}
void Interface_Index::setState(const QString &s)
{
    State = s;
}
void Interface_Index::setChanging(const QString &s)
{
    Changing = s;
}
void Interface_Index::setOnView(const bool state)
{
    onView = state;
}
QString Interface_Index::getName() const
{
    return Name;
}
QString Interface_Index::getMAC() const
{
    return MAC;
}
QString Interface_Index::getState() const
{
    return State;
}
QString Interface_Index::getChanging() const
{
    return Changing;
}
bool    Interface_Index::getOnView() const
{
    return onView;
}
