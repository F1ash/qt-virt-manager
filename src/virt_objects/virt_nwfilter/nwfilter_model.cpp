#include "nwfilter_model.h"

VirtNWFilterModel::VirtNWFilterModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    defined = QIcon::fromTheme("nwfilter");
    column0 = tr("Name");
    column1 = tr("UUID");
}
VirtNWFilterModel::~VirtNWFilterModel()
{
    DataList.clear();
}
Qt::ItemFlags VirtNWFilterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    NWFilter_Index *item = static_cast<NWFilter_Index *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int VirtNWFilterModel::rowCount(const QModelIndex &parent) const
{
    return DataList.count();
}
int VirtNWFilterModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}
bool VirtNWFilterModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    bool _res = true;
    if ( orientation == Qt::Horizontal ) {
        if ( role == Qt::EditRole ) {
            switch (section) {
            case 0:
                column0 = value.toString();
                break;
            case 1:
                column1 = value.toString();
                break;
            default:
                _res = false;
                break;
            }
        };
        headerDataChanged(Qt::Horizontal, 0, 1);
    };
    return _res;
}
QVariant VirtNWFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return column0;
      case 1:
        return column1;
      default:
        break;
      }
    }
  };
  return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant VirtNWFilterModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( DataList.count()<=index.row() ) {
        return res;
    };
    if ( !index.isValid() ) return res;
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
        case 0:
            res = DataList.at(index.row())->getName();
            break;
        case 1:
            res = DataList.at(index.row())->getUUID();
            break;
        default:
            break;
        }
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            res = defined;
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole ) {
        switch (index.column()) {
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool VirtNWFilterModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return false;
    };
    int editedRow = index.row();
    if ( DataList.count()<=editedRow ) {
        return false;
    };

    if ( role == Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            DataList.at(editedRow)->setName( value.toString() );
            break;
        case 1:
            DataList.at(editedRow)->setUUID( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool VirtNWFilterModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    DataList.insert(row, new NWFilter_Index);
    endInsertRows();
    return true;
}
bool VirtNWFilterModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    DataList.removeAt(row);
    endRemoveRows();
    return true;
}
