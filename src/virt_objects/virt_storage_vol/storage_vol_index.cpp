#include "storage_vol_index.h"

StorageVolIndex::StorageVolIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    Path = "";
    Type = "";
    Size = "";
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
void StorageVolIndex::setSize(const QString &s)
{
    Size = s;
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
QString StorageVolIndex::getSize() const
{
    return Size;
}
