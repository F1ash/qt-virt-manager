#include "conn_item_model.h"

ConnItemModel::ConnItemModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    onViewIcon = QIcon::fromTheme("overview");
    activeIcon = QIcon::fromTheme("run");
    no_activeIcon = QIcon::fromTheme("network-server");
    uri_logo.insert("test", QIcon::fromTheme("user-trash"));
    uri_logo.insert("lxc", QIcon::fromTheme("lxc"));
    uri_logo.insert("qemu", QIcon::fromTheme("qemu"));
    uri_logo.insert("xen", QIcon::fromTheme("xen"));
    uri_logo.insert("vnware", QIcon::fromTheme("vnware"));
    uri_logo.insert("vbox", QIcon::fromTheme("vbox"));
    uri_logo.insert("openvz", QIcon::fromTheme("openvz"));
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
    QString _name  = connItemDataList.at(index.row())->getName();
    QString _uri   = connItemDataList.at(index.row())->getURI();
    QString _state = connItemDataList.at(index.row())->getState();
    bool ok = false;
    int percent = _state.toInt(&ok);
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
        case 0:
            res = _name;
            break;
        case 1:
            res = _uri;
            break;
        case 2:
            res = _state;
            break;
        default:
            break;
        };
    };
    if ( role==Qt::DecorationRole ) {
        DATA _data = connItemDataList.at(index.row())->getData();
        switch (index.column()) {
        case 0:
            if ( _data.value("onView").toBool() ) {
                res = onViewIcon;
            } else if ( _state.toLower()=="opened" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 1:
            foreach ( QString _key, uri_logo.keys() ) {
                if ( _uri.startsWith(_key, Qt::CaseInsensitive) ) {
                    res = uri_logo.value( _key );
                    break;
                };
            };
            break;
        default:
            break;
        };
    };
    if ( role==Qt::ToolTipRole ) {
        QString s;
        QString a;
        switch (index.column()) {
        case 0:
            s.append(QString("Name: %1").arg(_name));
            a = ( connItemDataList.at(index.row())->getData()
                  .value("availability").toBool() )? "Available":"Busy";
            s.append(QString("\nState: %1").arg(a));
            res = s;
            break;
        case 1:
            res = QString("URI: %1").arg(_uri);
            break;
        case 2:
            if ( ok && percent>=100 ) {
                res = QString("State: waiting period has expired");
            } else {
                res = QString("State: %1").arg(_state);
            };
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
    if ( role==Qt::DecorationRole ) {
        DATA _data = connItemDataList.at(index.row())->getData();
        switch (index.column()) {
        case 0:
            _data.insert("onView", value);
            connItemDataList.at(editedRow)->setData( _data );
            break;
        default:
            break;
        };
    };
    /*
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
    */
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool ConnItemModel::insertRow(int row)
{
    if (row < 0) row = 0;
    beginInsertRows(rootIdx, rowCount(), rowCount());
    ConnItemIndex *newItem = new ConnItemIndex(this);
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
