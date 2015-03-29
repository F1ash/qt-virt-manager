#include "_snapshot_stuff.h"

_SnapshotStuff::_SnapshotStuff(
        QWidget *parent, bool _state) :
    QWidget(parent), state(_state)
{
    memLabel = new QLabel(this);
    filePathLabel = new QLabel("Absolute path of the file holding the VM memory state", this);
    filePath = new QLineEdit(this);
    diskWarn = new QLabel("<b>Disks</b> element don't implemented yet.", this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(memLabel);
    commonLayout->addWidget(filePathLabel);
    commonLayout->addWidget(filePath);
    commonLayout->addWidget(diskWarn);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
QDomDocument _SnapshotStuff::getElements() const
{
    return QDomDocument();
}
