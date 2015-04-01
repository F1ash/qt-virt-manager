#include "snapshot_tree_model.h"

SnapshotTreeModel::SnapshotTreeModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    icon = QIcon::fromTheme("camera-photo");
    column0 = "Name";
}
Qt::ItemFlags SnapshotTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    SnapshotTreeIndex *item = static_cast<SnapshotTreeIndex *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int SnapshotTreeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return DataList.count();
}
int SnapshotTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
bool SnapshotTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if ( orientation == Qt::Horizontal ) {
        if ( role == Qt::EditRole ) {
            switch (section) {
            case 0:
                column0 = value.toString();
                break;
            default:
                break;
            }
        };
        headerDataChanged(Qt::Horizontal, 0, 3);
    };
}
QVariant SnapshotTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return column0;
        break;
      default:
        break;
      }
    }
  };
  return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant SnapshotTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return DataList.at(index.row())->getName();
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            res = icon;
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole && index.column() ) {
        switch (index.column()) {
        case 0:
            res = QString("Name: %1").arg(DataList.at(index.row())->getName());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool SnapshotTreeModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return false;
    };

    if ( role == Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            DataList.at(index.row())->setName ( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool SnapshotTreeModel::insertRow(int row, const QModelIndex &parent)
{
    if ( row<0 ) return false;
    beginInsertRows(parent, row, row);
    DataList.insert(row, new SnapshotTreeIndex);
    endInsertRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
bool SnapshotTreeModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    DataList.removeAt(row);
    endRemoveRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
