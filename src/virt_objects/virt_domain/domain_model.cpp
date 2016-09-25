#include "domain_model.h"

DomainModel::DomainModel(QObject *parent) :
    Model(parent)
{
    defined = QIcon::fromTheme("domain");
    //created = QIcon::fromTheme("domain-create");
}
QVariant DomainModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole && index.column()==0 ) {
        return DataList.at(index.row())->getName();
    };
    if ( role==Qt::DisplayRole && index.column()==1 ) {
        return DataList.at(index.row())->getState_EXT();
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
        case 1:
            res = QString("State: %1")
                    .arg(DataList.at(
                             index.row())->getState_EXT());
            break;
        case 2:
            res = QString("Autostart: %1")
                    .arg(DataList.at(
                             index.row())->getAutostart());
            break;
        case 3:
            res = QString("Persistent: %1")
                    .arg(DataList.at(
                             index.row())->getPersistent());
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool DomainModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
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
        case 1:
            DataList.at(index.row())->setState ( value.toBool() );
            break;
        case 2:
            DataList.at(index.row())->setAutostart ( value.toBool() );
            break;
        case 3:
            DataList.at(index.row())->setPersistent ( value.toBool() );
            break;
        default:
            break;
        };
    };
    if ( role == Qt::DisplayRole && index.column()==1 ) {
        DataList.at(index.row())->setState_EXT( value.toString() );
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
