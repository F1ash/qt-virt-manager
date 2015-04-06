#include "create_snapshot_flags.h"

/*
 * https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html#virDomainSnapshotCreateXML
 */

#define CREATE_SNAPSHOT_FLAGS QStringList()\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_CURRENT"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_NO_METADATA"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_HALT"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_DISK_ONLY"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_REUSE_EXT"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_QUIESCE"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_ATOMIC"\
    <<"VIR_DOMAIN_SNAPSHOT_CREATE_LIVE"

CreateSnapshotFlags::CreateSnapshotFlags(QWidget *parent) :
    _SnapshotFlags(parent)
{
    foreach (QString flag, CREATE_SNAPSHOT_FLAGS) {
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
void CreateSnapshotFlags::changeAvailableFlags(int control)
{
    switch (control) {
    case 0:
        foreach (QAction *act, actGroup->actions()) {
            act->setChecked(false);
        };
        break;
    case 1:
        actGroup->actions().at(4)->setChecked(true);
        actGroup->actions().at(4)->setEnabled(true);
        actGroup->actions().at(6)->setEnabled(true);
        break;
    case 2:
        actGroup->actions().at(4)->setChecked(true);
        actGroup->actions().at(4)->setEnabled(true);
        actGroup->actions().at(6)->setEnabled(true);
        break;
    case 3:
        actGroup->actions().at(4)->setChecked(false);
        actGroup->actions().at(4)->setEnabled(false);
        actGroup->actions().at(6)->setChecked(false);
        actGroup->actions().at(6)->setEnabled(false);
        break;
    case 4:
        actGroup->actions().at(4)->setChecked(false);
        actGroup->actions().at(4)->setEnabled(false);
        actGroup->actions().at(6)->setChecked(false);
        actGroup->actions().at(6)->setEnabled(false);
        break;
    default:
        break;
    }
}

/* private slots */
void CreateSnapshotFlags::changeFlagCompatibility()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( NULL==act ) return;
    switch ( act->data().toInt() ) {
    case 1:
        if ( !act->isChecked() ) {
            /*
             * It is an error to request the VIR_DOMAIN_SNAPSHOT_CREATE_CURRENT flag
             * without VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
             */
            if ( actGroup->actions().at(1)->isChecked() )
                actGroup->actions().at(1)->setChecked(false);
        } else {
            /*
             * VIR_DOMAIN_SNAPSHOT_CREATE_NO_METADATA flag
             * is incompatible with VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
             */
            if ( actGroup->actions().at(2)->isChecked() )
                actGroup->actions().at(2)->setChecked(false);
            /*
             * VIR_DOMAIN_SNAPSHOT_CREATE_HALT flag
             * is incompatible with VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
             */
            if ( actGroup->actions().at(3)->isChecked() )
                actGroup->actions().at(3)->setChecked(false);
        };
        break;
    case 2:
        /*
         * It is an error to request the VIR_DOMAIN_SNAPSHOT_CREATE_CURRENT flag
         * without VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
         */
        if ( act->isChecked() ) {
            if ( !actGroup->actions().at(0)->isChecked() )
                actGroup->actions().at(0)->setChecked(true);
        };
        break;
    case 4:
        /*
         * VIR_DOMAIN_SNAPSHOT_CREATE_NO_METADATA flag
         * is incompatible with VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
         */
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(0)->isChecked() )
                actGroup->actions().at(0)->setChecked(false);
        };
        break;
    case 8:
        /*
         * VIR_DOMAIN_SNAPSHOT_CREATE_HALT flag
         * is incompatible with VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE.
         */
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(0)->isChecked() )
                actGroup->actions().at(0)->setChecked(false);
        };
        break;
    case 16:
        /*
         * Moreover, VIR_DOMAIN_SNAPSHOT_CREATE_QUIESCE flag requires
         * VIR_DOMAIN_SNAPSHOT_CREATE_DISK_ONLY to be passed as well.
         */
        if ( !act->isChecked() )
            if ( actGroup->actions().at(6)->isChecked() )
                actGroup->actions().at(6)->setChecked(false);
        break;
    case 64:
        /*
         * Moreover, VIR_DOMAIN_SNAPSHOT_CREATE_QUIESCE flag requires
         * VIR_DOMAIN_SNAPSHOT_CREATE_DISK_ONLY to be passed as well.
         */
        if ( act->isChecked() ) {
            if ( actGroup->actions().at(4)->isEnabled() ) {
                actGroup->actions().at(4)->setChecked(true);
            } else
                act->setChecked(false);
        };
        break;
    default:
        break;
    }
}
