#include "storage_vol_index.h"

StorageVolIndex::StorageVolIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    Path = "";
    Type = "";
    CurrSize = "";
    LogicSize = "";
}
void StorageVolIndex::setName(const QString &s)
{
    Name = s;
}
void StorageVolIndex::setPath(const QString &s)
{
    Path = s;
}
void StorageVolIndex::setType(const QString &s)
{
    Type = s;
}
void StorageVolIndex::setCurrSize(const QString &s)
{
    CurrSize = s;
}
void StorageVolIndex::setLogicSize(const QString &s)
{
    LogicSize = s;
}
QString StorageVolIndex::getName() const
{
    return Name;
}
QString StorageVolIndex::getPath() const
{
    return Path;
}
QString StorageVolIndex::getType() const
{
    return Type;
}
QString StorageVolIndex::getCurrSize() const
{
    return CurrSize;
}
QString StorageVolIndex::getLogicSize() const
{
    return LogicSize;
}
