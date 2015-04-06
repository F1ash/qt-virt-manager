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
    return;
}
int _SnapshotFlags::getCompositeFlag() const
{
    int ret = 0;
    foreach (QAction *act, actGroup->actions()) {
        if ( act->isChecked() ) {
            ret += act->data().toInt();
        };
    };
    return ret;
}
void _SnapshotFlags::changeFlagCompatibility()
{
    return;
}
