#include "snapshot_tree_model.h"

SnapshotTreeModel::SnapshotTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    icon = QIcon::fromTheme("camera-photo");
    rootItem = new TreeItem("Name");
}
SnapshotTreeModel::~SnapshotTreeModel()
{
    delete rootItem;
    rootItem = NULL;
}

QModelIndex SnapshotTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
QModelIndex SnapshotTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
Qt::ItemFlags SnapshotTreeModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}
int SnapshotTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
int SnapshotTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
QVariant SnapshotTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}
QVariant SnapshotTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if (!index.isValid()) return res;

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return item->data(index.column());
    };
    if ( role==Qt::DecorationRole  && index.column()==0 ) {
        return icon;
    };
    return res;
}
bool SnapshotTreeModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    TreeItem *item;
    if (!index.isValid())
        return false;
    else
        item = static_cast<TreeItem*>(index.internalPointer());

    if ( role == Qt::EditRole ) {
        QString data = value.toString();
        item->setData( data );
        //qDebug()<<data<<"set to"<<index.row();
    };
    emit dataChanged(index.sibling(index.row()-1, 1), index.sibling(index.row()+1, 1));
    return true;
}
bool SnapshotTreeModel::insertRow(int row, const QModelIndex &parent)
{
    if ( row<0 ) return false;
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    beginInsertRows(parent, row, row);
    parentItem->appendChild(new TreeItem(QString(), parentItem));
    endInsertRows();
    emit layoutChanged();
    return true;
}
bool SnapshotTreeModel::removeRow(int row, const QModelIndex &parent)
{
    if ( row<0 ) return false;
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    beginRemoveRows(parent, row, row);
    parentItem->removeChild( parentItem->child(row) );
    endRemoveRows();
    emit layoutChanged();
    return true;
}
