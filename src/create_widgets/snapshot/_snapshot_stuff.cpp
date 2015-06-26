#include "_snapshot_stuff.h"

_SnapshotStuff::_SnapshotStuff(QWidget *parent, bool _state) :
    QWidget(parent), state(_state)
{
    memLabel = new QLabel(this);
    filePathLabel = new QLabel("Absolute path of the file holding the VM memory state", this);
    filePath = new QLineEdit(this);
    diskWarn = new QCheckBox("Snapshot all disks", this);
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
    QDomDocument doc;
    if ( !diskWarn->isChecked() ) {
        doc = disks->getElements();
    };
    return doc;
}
void _SnapshotStuff::setParameters(virConnectPtr _conn, QString &_domName)
{
    SetDisksDataThread *setThread = new SetDisksDataThread(this);
    setThread->setCurrentWorkConnect(_conn, 0, _domName);
    connect(setThread, SIGNAL(diskData(QDomElement&)),
            disks, SLOT(setDisksData(QDomElement&)));
    connect(setThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errMsg(QString&)));
    setThread->start();
    setThread->wait();
    disconnect(setThread, SIGNAL(diskData(QDomElement&)),
               disks, SLOT(setDisksData(QDomElement&)));
    disconnect(setThread, SIGNAL(errorMsg(QString&,uint)),
               this, SIGNAL(errMsg(QString&)));
    delete setThread;
    setThread = NULL;
    disks->addStretch();
}
