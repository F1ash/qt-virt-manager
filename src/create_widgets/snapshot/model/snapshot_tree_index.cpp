#include "snapshot_tree_index.h"

SnapshotTreeIndex::SnapshotTreeIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
}
void SnapshotTreeIndex::setName(const QString &s)
{
    Name = s;
}
QString SnapshotTreeIndex::getName() const
{
    return Name;
}
