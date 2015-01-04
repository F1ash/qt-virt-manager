#include "conn_item_model.h"

ConnItemModel::ConnItemModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("run");
    no_activeIcon = QIcon::fromTheme("network-server");
    uri_logo.insert("lxc", QIcon::fromTheme("lxc"));
    uri_logo.insert("qemu", QIcon::fromTheme("qemu"));
    uri_logo.insert("vnware", QIcon::fromTheme("vnware"));
    uri_logo.insert("vbox", QIcon::fromTheme("vbox"));
    rootIdx = QModelIndex();
}
ConnItemModel::~ConnItemModel()
{
    connItemDataList.clear();
}
Qt::ItemFlags ConnItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    ConnItemIndex *item = static_cast<ConnItemIndex *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int ConnItemModel::rowCount(const QModelIndex &parent) const
{
    return connItemDataList.count();
}
int ConnItemModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}
QVariant ConnItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return QString("Name");
        break;
      case 1:
        return QString("URI");
        break;
      case 2:
        return QString("State");
        break;
      default:
        break;
      }
    } else if ( role==Qt::TextAlignmentRole ) {
        return Qt::AlignCenter;
    };
  };
  return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant ConnItemModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
        case 0:
            res = connItemDataList.at(index.row())->getName();
            break;
        case 1:
            res = connItemDataList.at(index.row())->getURI();
            break;
        case 2:
            res = connItemDataList.at(index.row())->getState();
            break;
        default:
            break;
        };
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            if ( connItemDataList.at(index.row())->getState().toLower()=="running" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 1:
            res = uri_logo.value( connItemDataList.at(index.row())->getURI()
                    .split("://").first() );
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole ) {
        QString s;
        QString a;
        switch (index.column()) {
        case 0:
            s.append(QString("Name: %1").arg(connItemDataList.at(index.row())->getName()));
            a = ( connItemDataList.at(index.row())->getData()
                  .value("availability").toBool() )? "Available":"Busy";
            s.append(QString("\nState: %1").arg(a));
            res = s;
            break;
        case 1:
            res = QString("URI: %1").arg(connItemDataList.at(index.row())->getURI());
            break;
        case 2:
            res = QString("State: %1").arg(connItemDataList.at(index.row())->getState());
            break;
        default:
            break;
        }
    };
    if ( role==Qt::TextAlignmentRole ) {
        switch (index.column()) {
        case 1:
            res = Qt::AlignCenter;
            break;
        case 2:
            res = Qt::AlignCenter;
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool ConnItemModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return false;
    };

    int editedRow = index.row();
    if ( role == Qt::EditRole ) {
        QString val = value.toString();
        switch( index.column() ) {
        case 0:
            connItemDataList.at(editedRow)->setName ( val );
            break;
        case 1:
            connItemDataList.at(editedRow)->setURI ( val );
            break;
        case 2:
            connItemDataList.at(editedRow)->setState ( val );
            break;
        default:
            break;
        };
    };
    QModelIndex topLeft;
    QModelIndex lowRight;
    if ( editedRow==0 ) {
        topLeft  = rootIdx;
    } else {
        topLeft = this->index(editedRow, 0);
    };
    if ( connItemDataList.count()>editedRow+1 ) {
        lowRight = this->index(editedRow+1, columnCount()-1);
    } else {
        lowRight = this->index(rowCount(), columnCount()-1);
    };
    emit dataChanged(topLeft, lowRight);
    return true;
}
bool ConnItemModel::insertRow(int row)
{
    if (row < 0) row = 0;
    beginInsertRows(rootIdx, rowCount(), rowCount());
    ConnItemIndex *newItem = new ConnItemIndex();
    connItemDataList.insert(row, newItem);
    endInsertRows();
    return true;
}
bool ConnItemModel::removeRow(int row)
{
    if (row < 0) row = 0;
    QModelIndex _idx = index(row,0);
    if ( !_idx.isValid() ) {
        return false;
    } else if ( connItemDataList.count() ) {
        beginRemoveRows( rootIdx, row, row );
        connItemDataList.removeAt(row);
        endRemoveRows();
        return true;
    } else return false;
}
