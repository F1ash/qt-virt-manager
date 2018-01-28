#include "model.h"

Model::Model(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("list-add");
    no_activeIcon = QIcon::fromTheme("list-remove");
    column0 = tr("Name");
    column1 = tr("State");
    column2 = tr("Auto");
    column3 = tr("Persistent");
}
Model::~Model()
{
    DataList.clear();
}
Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    Index *item = static_cast<Index *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int Model::rowCount(const QModelIndex &parent) const
{
    return DataList.count();
}
int Model::columnCount(const QModelIndex &parent) const
{
    return 4;
}
bool Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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
QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
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
            };
        } else if ( section && role==Qt::TextAlignmentRole ) {
            return Qt::AlignCenter;
        };
    };
    return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant Model::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( DataList.count()<=index.row() ) {
        return res;
    };
    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return DataList.at(index.row())->getName();
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            res = defined;
            break;
        case 1:
            if ( DataList.at(index.row())->getState() ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 2:
            if ( DataList.at(index.row())->getAutostart() ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 3:
            if ( DataList.at(index.row())->getPersistent() ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        default:
            break;
        }
    };
    if ( role==Qt::ToolTipRole && index.column() ) {
        switch (index.column()) {
        case 0:
            res = QString(tr("Name: %1"))
                    .arg(DataList.at(index.row())->getName());
            break;
        case 1:
            res = QString(tr("State: %1"))
                    .arg(DataList.at(index.row())->getState());
            break;
        case 2:
            res = QString(tr("Autostart: %1"))
                    .arg(DataList.at(index.row())->getAutostart());
            break;
        case 3:
            res = QString(tr("Persistent: %1"))
                    .arg(DataList.at(index.row())->getPersistent());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool Model::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
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
            DataList.at(editedRow)->setName ( value.toString() );
            break;
        case 1:
            DataList.at(editedRow)->setState ( value.toBool() );
            break;
        case 2:
            DataList.at(editedRow)->setAutostart ( value.toBool() );
            break;
        case 3:
            DataList.at(editedRow)->setPersistent ( value.toBool() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool Model::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    DataList.insert(row, new Index);
    endInsertRows();
    return true;
}
bool Model::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    DataList.removeAt(row);
    endRemoveRows();
    return true;
}
