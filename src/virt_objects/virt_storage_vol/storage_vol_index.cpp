#include "storage_vol_index.h"

StorageVolIndex::StorageVolIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    Autostart = "";
    Persistent = "";
}
void StorageVolIndex::setName(const QString &s)
{
    Name = s;
}
void StorageVolIndex::setState(const QString &s)
{
    State = s;
}
void StorageVolIndex::setAutostart(const QString &s)
{
    Autostart = s;
}
void StorageVolIndex::setPersistent(const QString &s)
{
    Persistent = s;
}
QString StorageVolIndex::getName() const
{
    return Name;
}
QString StorageVolIndex::getState() const
{
    return State;
}
QString StorageVolIndex::getAutostart() const
{
    return Autostart;
}
QString StorageVolIndex::getPersistent() const
{
    return Persistent;
}
