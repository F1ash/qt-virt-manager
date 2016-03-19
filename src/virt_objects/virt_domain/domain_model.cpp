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
        return DataList.at(index.row())->getState().split(":").last();
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
        case 0:
            res = defined;
            break;
        case 1:
            if ( DataList.at(index.row())->getState().split(":").first()=="active" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 2:
            if ( DataList.at(index.row())->getAutostart()=="yes" ) {
                res = activeIcon;
            } else res = no_activeIcon;
            break;
        case 3:
            if ( DataList.at(index.row())->getPersistent()=="yes" ) {
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
                             index.row())->getState()
                         .split(":").first());
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
