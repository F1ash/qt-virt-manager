#include "mem_state_snapshot.h"

MemStateSnapshot::MemStateSnapshot(QWidget *parent, bool _state) :
    _SnapshotStuff(parent, _state)
{
    memLabel->setVisible(false);
    filePathLabel->setVisible(false);
    filePath->setVisible(false);
    diskWarn->setVisible(false);
    info = new QLabel("For this use <b>Save</b> action.", this);
    commonLayout->addWidget(info);
}
