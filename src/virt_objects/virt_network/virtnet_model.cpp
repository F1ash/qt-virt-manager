#include "virtnet_model.h"

VirtNetModel::VirtNetModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("list-add");
    no_activeIcon = QIcon::fromTheme("list-remove");
    defined = QIcon::fromTheme("network-define");
    created = QIcon::fromTheme("network-create");
}
VirtNetModel::~VirtNetModel()
{
    virtNetDataList.clear();
}
Qt::ItemFlags VirtNetModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    VirtNetIndex *item = static_cast<VirtNetIndex *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int VirtNetModel::rowCount(const QModelIndex &parent) const
{
    return virtNetDataList.count();
}
int VirtNetModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}
QVariant VirtNetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return QString("Name");
        break;
      case 1:
        return QString("State");
        break;
      case 2:
        return QString("Auto");
        break;
      case 3:
        return QString("Prst");
      default:
        break;
      }
    }
  };
  return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant VirtNetModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return virtNetDataList.at(index.row())->getName();
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            if ( virtNetDataList.at(index.row())->getPersistent()=="yes" ) {
                res = defined;
            } else res = created;
            break;
        case 1:
            if ( virtNetDataList.at(index.row())->getState()=="active" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 2:
            if ( virtNetDataList.at(index.row())->getAutostart()=="yes" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 3:
            if ( virtNetDataList.at(index.row())->getPersistent()=="yes" ) {
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
            res = QString("State: %1").arg(virtNetDataList.at(index.row())->getState());
            break;
        case 2:
            res = QString("Autostart: %1").arg(virtNetDataList.at(index.row())->getAutostart());
            break;
        case 3:
            res = QString("Persistent: %1").arg(virtNetDataList.at(index.row())->getPersistent());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool VirtNetModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        qDebug()<<"index not valid";
        return false;
    };

    if ( role == Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            virtNetDataList.at(index.row())->setName ( value.toString() );
            break;
        case 1:
            virtNetDataList.at(index.row())->setState ( value.toString() );
            break;
        case 2:
            virtNetDataList.at(index.row())->setAutostart ( value.toString() );
            break;
        case 3:
            virtNetDataList.at(index.row())->setPersistent ( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool VirtNetModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    VirtNetIndex *newItem = new VirtNetIndex;
    virtNetDataList.insert(row, newItem);
    endInsertRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
bool VirtNetModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    virtNetDataList.removeAt(row);
    endRemoveRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
