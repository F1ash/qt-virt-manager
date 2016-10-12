#include "virt_interface_model.h"

VirtIfaceModel::VirtIfaceModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("list-add");
    no_activeIcon = QIcon::fromTheme("list-remove");
    defined = QIcon::fromTheme("network-wired");
    changing = QIcon::fromTheme("dialog-warning");
    column0 = "Name";
    column1 = "MAC";
    column2 = "State";
    column3 = "Changing";
}
VirtIfaceModel::~VirtIfaceModel()
{
    DataList.clear();
}
Qt::ItemFlags VirtIfaceModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    Interface_Index *item = static_cast<Interface_Index *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int VirtIfaceModel::rowCount(const QModelIndex &parent) const
{
    return DataList.count();
}
int VirtIfaceModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}
bool VirtIfaceModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
            case 2:
                column2 = value.toString();
                break;
            case 3:
                column3 = value.toString();
                break;
            default:
                _res = false;
                break;
            }
        };
        headerDataChanged(Qt::Horizontal, 0, 3);
    };
    return _res;
}
QVariant VirtIfaceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal ) {
        if ( role == Qt::DisplayRole ) {
            switch (section) {
            case 0:
                return column0;
            case 1:
                return column1;
            case 2:
                return column2;
            case 3:
                return column3;
            default:
                break;
            }
        } else if ( section && role==Qt::TextAlignmentRole ) {
            return Qt::AlignCenter;
        }
    };
    return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant VirtIfaceModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( DataList.count()<=index.row() ) {
        return res;
    };
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
        case 0:
            res = DataList.at(index.row())->getName();
            break;
        case 1:
            res = DataList.at(index.row())->getMAC();
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
        case 2:
            if ( DataList.at(index.row())->getState() ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 3:
            if ( DataList.at(index.row())->getChanging() ) {
                res = changing;
            };
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole ) {
        switch (index.column()) {
        case 0:
            res = QString("Name: %1").arg(DataList.at(index.row())->getName());
            break;
        case 1:
            res = QString("MAC: %1").arg(DataList.at(index.row())->getMAC());
            break;
        case 2:
            res = QString("State: %1").arg(DataList.at(index.row())->getState());
            break;
        case 3:
            if ( DataList.at(index.row())->getChanging() ) {
                res = QString("Changing Began");
            };
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool VirtIfaceModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
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
            DataList.at(editedRow)->setMAC( value.toString() );
            break;
        case 2:
            DataList.at(editedRow)->setState( value.toBool() );
            break;
        case 3:
            if ( value.toBool() )
                DataList.at(editedRow)->setChanging( value.toBool() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool VirtIfaceModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    DataList.insert(row, new Interface_Index);
    endInsertRows();
    return true;
}
bool VirtIfaceModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    DataList.removeAt(row);
    endRemoveRows();
    return true;
}
