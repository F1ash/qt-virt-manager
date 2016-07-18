#include "nwfilter_index.h"

NWFilter_Index::NWFilter_Index(QObject *parent) :
    QObject(parent)
{
    Name = "";
    UUID = "";
    onView = false;
}
void NWFilter_Index::setName(const QString &s)
{
    Name = s;
}
void NWFilter_Index::setUUID(const QString &s)
{
    UUID = s;
}
void NWFilter_Index::setOnView(const bool state)
{
    onView = state;
}
QString NWFilter_Index::getName() const
{
    return Name;
}
QString NWFilter_Index::getUUID() const
{
    return UUID;
}
bool    NWFilter_Index::getOnView() const
{
    return onView;
}
