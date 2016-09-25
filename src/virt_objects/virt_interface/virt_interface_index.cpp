#include "virt_interface_index.h"

Interface_Index::Interface_Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    MAC  = "";
    State = false;
    Changing = false;
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
void Interface_Index::setState(const bool state)
{
    State = state;
}
void Interface_Index::setChanging(const bool state)
{
    Changing = state;
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
bool Interface_Index::getState() const
{
    return State;
}
bool Interface_Index::getChanging() const
{
    return Changing;
}
bool Interface_Index::getOnView() const
{
    return onView;
}
