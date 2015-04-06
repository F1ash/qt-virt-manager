#include "revert_snapshot_flags.h"

/*
 * https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html#virDomainRevertToSnapshot
 */

#define REVERT_SNAPSHOT_FLAGS QStringList()\
    <<"VIR_DOMAIN_SNAPSHOT_REVERT_RUNNING"\
    <<"VIR_DOMAIN_SNAPSHOT_REVERT_PAUSED"\
    <<"VIR_DOMAIN_SNAPSHOT_REVERT_FORCE"

RevertSnapshotFlags::RevertSnapshotFlags(QWidget *parent) :
    _SnapshotFlags(parent)
{
    foreach (QString flag, REVERT_SNAPSHOT_FLAGS) {
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
void RevertSnapshotFlags::changeAvailableFlags(int control)
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
void RevertSnapshotFlags::changeFlagCompatibility()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( NULL==act ) return;
    switch ( act->data().toInt() ) {
    case 1:
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(1)->isChecked() )
                actGroup->actions().at(1)->setChecked(false);
        };
        break;
    case 2:
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(0)->isChecked() )
                actGroup->actions().at(0)->setChecked(false);
        };
        break;
    default:
        break;
    }
}
