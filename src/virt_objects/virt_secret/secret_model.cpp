#include "secret_model.h"

VirtSecretModel::VirtSecretModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    defined = QIcon::fromTheme("security");
    column0 = tr("UUID");
    column1 = tr("UsageID");
}
VirtSecretModel::~VirtSecretModel()
{
    DataList.clear();
}
Qt::ItemFlags VirtSecretModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    Secret_Index *item = static_cast<Secret_Index *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int VirtSecretModel::rowCount(const QModelIndex &parent) const
{
    return DataList.count();
}
int VirtSecretModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}
bool VirtSecretModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
QVariant VirtSecretModel::headerData(int section, Qt::Orientation orientation, int role) const
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
QVariant VirtSecretModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( DataList.count()<=index.row() ) {
        return res;
    };
    if ( !index.isValid() ) return res;
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
        case 0:
            res = DataList.at(index.row())->getUUID();
            break;
        case 1:
            res = DataList.at(index.row())->getUsageID();
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
        case 0:
            res = QString(tr("Description: %1")).arg(DataList.at(index.row())->getDescription());
            break;
        case 1:
            res = QString(tr("Type: %1")).arg(DataList.at(index.row())->getType());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool VirtSecretModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return false;
    };
    int editedRow = index.row();
    if ( DataList.count()<=editedRow ) {
        return false;
    };

    if ( role==Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            DataList.at(editedRow)->setUUID( value.toString() );
            break;
        case 1:
            DataList.at(editedRow)->setUsageID( value.toString() );
            break;
        default:
            break;
        };
    };
    if ( role==Qt::ToolTipRole ) {
        switch (index.column()) {
        case 0:
            DataList.at(editedRow)->setDescription( value.toString() );
            break;
        case 1:
            DataList.at(editedRow)->setType( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool VirtSecretModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    DataList.insert(row, new Secret_Index);
    endInsertRows();
    return true;
}
bool VirtSecretModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    DataList.removeAt(row);
    endRemoveRows();
    return true;
}
