#include "delete_snapshot_flags.h"

/*
 * https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html#virDomainSnapshotDelete
 */

#define DELETE_SNAPSHOT_FLAGS QStringList()\
    <<"VIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN"\
    <<"VIR_DOMAIN_SNAPSHOT_DELETE_METADATA_ONLY"\
    <<"VIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN_ONLY"

DeleteSnapshotFlags::DeleteSnapshotFlags(QWidget *parent) :
    _SnapshotFlags(parent)
{
    foreach (QString flag, DELETE_SNAPSHOT_FLAGS) {
        actGroup->addAction(new QAction(flag, this));
    };
    int i = 0;
    foreach (QAction *act, actGroup->actions()) {
        act->setCheckable(true);
        act->setChecked(false);
        connect(act, SIGNAL(changed()),
                this, SLOT(changeFlagCompatibility()));
        act->setData(QVariant(pow(2,i)));
        //qDebug()<<act->text()<<act->data().toInt();
        i++;
    };
    addActions(actGroup->actions());
}

/* public slots */
void DeleteSnapshotFlags::changeAvailableFlags(int control)
{
    switch (control) {
    case 0:
        foreach (QAction *act, actGroup->actions()) {
            act->setChecked(false);
        };
        break;
    default:
        break;
    }
}

/* private slots */
void DeleteSnapshotFlags::changeFlagCompatibility()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( Q_NULLPTR==act ) return;
    switch ( act->data().toInt() ) {
    case 1:
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(2)->isChecked() )
                actGroup->actions().at(2)->setChecked(false);
        };
        break;
    case 4:
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(0)->isChecked() )
                actGroup->actions().at(0)->setChecked(false);
        };
        break;
    default:
        break;
    }
}
