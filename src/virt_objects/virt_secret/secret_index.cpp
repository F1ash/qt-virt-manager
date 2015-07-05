#include "secret_index.h"

Secret_Index::Secret_Index(QObject *parent) :
    QObject(parent)
{
    UsageID = "";
    UUID = "";
    Type = "";
    Description = "";
    onView = false;
}
void Secret_Index::setUsageID(const QString &s)
{
    UsageID = s;
}
void Secret_Index::setUUID(const QString &s)
{
    UUID = s;
}
void Secret_Index::setType(const QString &s)
{
    Type = s;
}
void Secret_Index::setDescription(const QString &s)
{
    Description = s;
}
void Secret_Index::setOnView(const bool state)
{
    onView = state;
}
QString Secret_Index::getUsageID() const
{
    return UsageID;
}
QString Secret_Index::getUUID() const
{
    return UUID;
}
QString Secret_Index::getType() const
{
    return Type;
}
QString Secret_Index::getDescription() const
{
    return Description;
}
bool    Secret_Index::getOnView() const
{
    return onView;
}
