#include "_snapshot_flags.h"

_SnapshotFlags::_SnapshotFlags(QWidget *parent) :
    QMenu(parent)
{
    actGroup = new QActionGroup(this);
    actGroup->setExclusive(false);
    connect(actGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(show()));
}
void _SnapshotFlags::changeAvailableFlags(int control)
{
    Q_UNUSED(control)
    return;
}
uint _SnapshotFlags::getCompositeFlag() const
{
    uint ret = 0;
    foreach (QAction *act, actGroup->actions()) {
        if ( act->isChecked() ) {
            ret += act->data().toUInt();
        };
    };
    return ret;
}
void _SnapshotFlags::changeFlagCompatibility()
{
    return;
}
