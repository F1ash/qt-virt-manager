#include "storage_pool_index.h"

StoragePoolIndex::StoragePoolIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    Autostart = "";
    Persistent = "";
}
void StoragePoolIndex::setName(const QString &s)
{
    Name = s;
}
void StoragePoolIndex::setState(const QString &s)
{
    State = s;
}
void StoragePoolIndex::setAutostart(const QString &s)
{
    Autostart = s;
}
void StoragePoolIndex::setPersistent(const QString &s)
{
    Persistent = s;
}
QString StoragePoolIndex::getName() const
{
    return Name;
}
QString StoragePoolIndex::getState() const
{
    return State;
}
QString StoragePoolIndex::getAutostart() const
{
    return Autostart;
}
QString StoragePoolIndex::getPersistent() const
{
    return Persistent;
}
