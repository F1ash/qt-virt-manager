#include "snapshot_action_dialog.h"

SnapshotActionDialog::SnapshotActionDialog(
        QWidget         *parent,
        virConnectPtr    currConnect,
        QString          _domName) :
    QDialog(parent),
    currJobConnect(currConnect),
    domName(_domName)
{
    QString winTitle = QString("<%1> Snapshot Actions").arg(domName);
    setWindowTitle(winTitle);
    toolBar = new QToolBar(this);
    toolBar->addAction(new QAction("Revert", this));
    toolBar->addAction(new QAction("Delete", this));
    model = new SnapshotTreeModel(this);
    snapshotTree = new QTreeView(this);
    snapshotTree->setRootIndex(QModelIndex());
    snapshotTree->setRootIsDecorated(true);
    snapshotTree->setModel(model);
    snapshotTree->setExpandsOnDoubleClick(true);
    info = new QLabel("<a href='https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html'>About</a>", this);
    info->setOpenExternalLinks(true);
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(info);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttonsWdg = new QWidget(this);
    buttonsWdg->setLayout(buttonsLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(toolBar);
    commonLayout->addWidget(snapshotTree);
    commonLayout->addWidget(buttonsWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    setDomainSnapshots();
}
SnapshotActionDialog::~SnapshotActionDialog()
{
    if ( NULL!=domain ) virDomainFree(domain);
}

/* private slots */
void SnapshotActionDialog::addSnapshotChild(int row, const QModelIndex &parent, char *name)
{
    // flags: extra flags; not used yet, so callers should always pass 0
    virDomainSnapshotPtr snapShot =
            virDomainSnapshotLookupByName(domain, name, 0);
    if ( NULL!=snapShot ) {
        model->insertRow(row, parent);
        model->setData(model->index(row, 0, parent), name, Qt::EditRole);
        qDebug()<<row<<name<<"added";
        int namesLen = virDomainSnapshotNumChildren(
                    snapShot, 0);
        if ( namesLen>0 ) {
            char *names;
            int ret = virDomainSnapshotListChildrenNames(
                        snapShot, &names, namesLen, 0);
            if ( ret>0 ) {
                for (uint i = 0; i<ret; i++) {
                    int _row = model->rowCount();
                    qDebug()<<_row<<(&names)[i]<<model->index(row, 0, parent).data()<<ret;
                    addSnapshotChild(_row, model->index(row, 0, parent), (&names)[i]);
                };
            };
        };
        virDomainSnapshotFree(snapShot);
    };
}
void SnapshotActionDialog::setDomainSnapshots()
{
    domain = virDomainLookupByName(
                currJobConnect, domName.toUtf8().data());
    int namesLen = virDomainSnapshotNum(
                domain, VIR_DOMAIN_SNAPSHOT_LIST_ROOTS);
    if ( namesLen>0 ) {
        char *names;
        int ret = virDomainSnapshotListNames(
                    domain, &names, namesLen, VIR_DOMAIN_SNAPSHOT_LIST_ROOTS);
        if ( ret>0 ) {
            for (uint i = 0; i<ret; i++) {
                int row = model->rowCount();
                addSnapshotChild(row, snapshotTree->rootIndex(), (&names)[i]);
            };
        };
        qDebug()<<"tree is set";
    };
}
void SnapshotActionDialog::accept()
{
    done(1);
}
void SnapshotActionDialog::reject()
{
    done(0);
}
