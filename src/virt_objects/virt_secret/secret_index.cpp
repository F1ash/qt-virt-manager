#include "secret_index.h"

Secret_Index::Secret_Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    onView = false;
}
void Secret_Index::setName(const QString &s)
{
    Name = s;
}
void Secret_Index::setState(const QString &s)
{
    State = s;
}
void Secret_Index::setOnView(const bool state)
{
    onView = state;
}
QString Secret_Index::getName() const
{
    return Name;
}
QString Secret_Index::getState() const
{
    return State;
}
bool    Secret_Index::getOnView() const
{
    return onView;
}
