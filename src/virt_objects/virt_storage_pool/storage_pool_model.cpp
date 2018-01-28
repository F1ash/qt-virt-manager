#include "storage_pool_model.h"

StoragePoolModel::StoragePoolModel(QObject *parent) :
    Model(parent)
{
    defined = QIcon::fromTheme("storage");
    //created = QIcon::fromTheme("storagePool-create");
    //onViewIcon = QIcon::fromTheme("overview");
}
QVariant StoragePoolModel::data(const QModelIndex &index, int role) const
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
            /*
            if ( DataList.at(index.row())->getOnView() ) {
                res = onViewIcon;
            } else {
                res = defined;
            };
            */
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
