#include "_snapshot_stuff.h"

_SnapshotStuff::_SnapshotStuff(
        QWidget *parent, bool _state, bool _external) :
    QWidget(parent), state(_state), external(_external)
{
    memLabel = new QLabel(this);
    filePathLabel = new QLabel(
                tr("Absolute path of the file holding the VM memory state"),
                this);
    filePath = new QLineEdit(this);
    diskWarn = new QCheckBox(tr("Snapshot all disks"), this);
    disks = new _Disks(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(memLabel);
    commonLayout->addWidget(filePathLabel);
    commonLayout->addWidget(filePath);
    commonLayout->addWidget(diskWarn);
    commonLayout->addWidget(disks);
    //commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(diskWarn, SIGNAL(toggled(bool)),
            disks, SLOT(setDisabled(bool)));
    diskWarn->setChecked(true);
}
QDomDocument _SnapshotStuff::getElements() const
{
    return QDomDocument();
}
void _SnapshotStuff::setParameters(virConnectPtr* connPtrPtr, QString &_domName)
{
    SetDisksDataThread *setThread = new SetDisksDataThread(this);
    setThread->setCurrentWorkConnection(connPtrPtr, 0, _domName);
    connect(setThread, SIGNAL(diskData(QDomElement&)),
            this, SLOT(setDiskItem(QDomElement&)));
    connect(setThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errMsg(const QString&)));
    setThread->start();
    setThread->wait();
    disconnect(setThread, SIGNAL(diskData(QDomElement&)),
               this, SLOT(setDiskItem(QDomElement&)));
    disconnect(setThread, SIGNAL(errorMsg(const QString&, const uint)),
               this, SIGNAL(errMsg(const QString&)));
    setThread->deleteLater();
    disks->addStretch();
}
void _SnapshotStuff::setDiskItem(QDomElement &_el)
{
    disks->setDisksData(_el, external);
}
