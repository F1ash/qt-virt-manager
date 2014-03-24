#include "storage_vol_model.h"

StorageVolModel::StorageVolModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("list-add");
    no_activeIcon = QIcon::fromTheme("list-remove");
    defined = QIcon::fromTheme("storageVol-define");
    created = QIcon::fromTheme("storageVol-create");
    column0 = "Name";
    column1 = "State";
    column2 = "Auto";
    column3 = "Prst";
}
StorageVolModel::~StorageVolModel()
{
    virtStorageVolDataList.clear();
}
Qt::ItemFlags StorageVolModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    StorageVolIndex *item = static_cast<StorageVolIndex *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int StorageVolModel::rowCount(const QModelIndex &parent) const
{
    return virtStorageVolDataList.count();
}
int StorageVolModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}
bool StorageVolModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if ( orientation == Qt::Horizontal ) {
        if ( role == Qt::EditRole ) {
            switch (section) {
            case 0:
                column0 = value.toString();
                break;
            case 1:
                column1 = value.toString();
                break;
            case 2:
                column2 = value.toString();
                break;
            case 3:
                column3 = value.toString();
            default:
                break;
            }
        };
        headerDataChanged(Qt::Horizontal, 0, 3);
    };
}
QVariant StorageVolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return column0;
        break;
      case 1:
        return column1;
        break;
      case 2:
        return column2;
        break;
      case 3:
        return column3;
      default:
        break;
      }
    }
  };
  return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant StorageVolModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return virtStorageVolDataList.at(index.row())->getName();
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            if ( virtStorageVolDataList.at(index.row())->getPersistent()=="yes" ) {
                res = defined;
            } else res = created;
            break;
        case 1:
            if ( virtStorageVolDataList.at(index.row())->getState()=="active" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 2:
            if ( virtStorageVolDataList.at(index.row())->getAutostart()=="yes" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 3:
            if ( virtStorageVolDataList.at(index.row())->getPersistent()=="yes" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole && index.column() ) {
        switch (index.column()) {
        case 1:
            res = QString("State: %1").arg(virtStorageVolDataList.at(index.row())->getState());
            break;
        case 2:
            res = QString("Autostart: %1").arg(virtStorageVolDataList.at(index.row())->getAutostart());
            break;
        case 3:
            res = QString("Persistent: %1").arg(virtStorageVolDataList.at(index.row())->getPersistent());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool StorageVolModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        qDebug()<<"index not valid";
        return false;
    };

    if ( role == Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            virtStorageVolDataList.at(index.row())->setName ( value.toString() );
            break;
        case 1:
            virtStorageVolDataList.at(index.row())->setState ( value.toString() );
            break;
        case 2:
            virtStorageVolDataList.at(index.row())->setAutostart ( value.toString() );
            break;
        case 3:
            virtStorageVolDataList.at(index.row())->setPersistent ( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool StorageVolModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    StorageVolIndex *newItem = new StorageVolIndex;
    virtStorageVolDataList.insert(row, newItem);
    endInsertRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
bool StorageVolModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    virtStorageVolDataList.removeAt(row);
    endRemoveRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
