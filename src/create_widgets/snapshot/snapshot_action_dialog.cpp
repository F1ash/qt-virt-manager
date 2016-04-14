#include "snapshot_action_dialog.h"

/*
 * https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html
 */

SnapshotActionDialog::SnapshotActionDialog(
        QWidget         *parent,
        virConnectPtr   *ptr_ConnPtr,
        QString          _domName,
        QString          _conName) :
    QDialog(parent),
    ptr_ConnPtr(ptr_ConnPtr),
    domName(_domName)
{
    params.clear();
    QString winTitle = QString(
                "<%1> Snapshot Actions in [ %2 ] connection")
            .arg(domName).arg(_conName);
    setWindowTitle(winTitle);
    settings.beginGroup("SnapshotActionDialog");
    restoreGeometry( settings.value("Geometry").toByteArray() );
    int c0 = settings.value("column0", 132).toInt();
    int c1 = settings.value("column1", 64).toInt();
    settings.endGroup();
    revertFlagsMenu = new RevertSnapshotFlags(this);
    revertAction = new QAction(
                QIcon::fromTheme("document-revert"),
                "Revert To",
                this);
    revertAction->setMenu(revertFlagsMenu);
    deleteFlagsMenu = new DeleteSnapshotFlags(this);
    deleteAction = new QAction(
                QIcon::fromTheme("list-remove"),
                "Delete",
                this);
    deleteAction->setMenu(deleteFlagsMenu);
    refreshAction = new QAction(
                QIcon::fromTheme("view-refresh"),
                "Refresh",
                this);
    getXMLDescAction = new QAction(
                QIcon::fromTheme("application-xml"),
                "get XML Description",
                this);
    toolBar = new QToolBar(this);
    toolBar->addAction(revertAction);
    toolBar->addAction(deleteAction);
    toolBar->addAction(refreshAction);
    toolBar->addAction(getXMLDescAction);
    model = new SnapshotTreeModel;
    snapshotTree = new QTreeView(this);
    snapshotTree->setRootIsDecorated(true);
    snapshotTree->setModel(model);
    snapshotTree->setExpandsOnDoubleClick(true);
    snapshotTree->setColumnWidth(0, c0);
    snapshotTree->setColumnWidth(1, c1);
    info = new QLabel(
    "<a href='https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html'>About</a>",
                this);
    info->setOpenExternalLinks(true);
    info->setToolTip(
                "https://libvirt.org/html/libvirt-libvirt-domain-snapshot.html");
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    closeWdg = new QPushButton("Close", this);
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(info);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttonsLayout->addWidget(closeWdg);
    buttonsWdg = new QWidget(this);
    buttonsWdg->setLayout(buttonsLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(toolBar);
    commonLayout->addWidget(snapshotTree);
    commonLayout->addWidget(buttonsWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    connect(closeWdg, SIGNAL(clicked()), this, SLOT(reject()));
    setDomainSnapshots();
    connect(toolBar, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
    changeDialogState(false);
}
SnapshotActionDialog::~SnapshotActionDialog()
{
    if ( nullptr!=domain ) virDomainFree(domain);
    settings.beginGroup("SnapshotActionDialog");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("column0", snapshotTree->columnWidth(0));
    settings.setValue("column1", snapshotTree->columnWidth(1));
    settings.endGroup();
    settings.sync();
}

/* public slots */
QStringList SnapshotActionDialog::getParameters() const
{
    return params;
}
uint SnapshotActionDialog::getSnapshotFlags() const
{
    return flags;
}

/* private slots */
void SnapshotActionDialog::clearSnapshotTree()
{
    while ( model->rootItemChildCount()>0 ) {
        model->removeRow(0, snapshotTree->rootIndex());
    };
}
void SnapshotActionDialog::addSnapshotChild(int row, const QModelIndex &parent, char *name)
{
    // flags: extra flags; not used yet,
    // so callers should always pass 0
    virDomainSnapshotPtr snapShot =
            virDomainSnapshotLookupByName(domain, name, 0);
    char *xmlDesc = virDomainSnapshotGetXMLDesc(snapShot, 0);
    QString _desc;
    QString _data(" -- ");
    if ( nullptr!=xmlDesc ) {
        QDomDocument doc;
        doc.setContent(QString(xmlDesc));
        free(xmlDesc);
        QDomElement _el = doc
                .firstChildElement("domainsnapshot")
                .firstChildElement("creationTime");
        if ( !_el.isNull() ) {
            _data.clear();
            _data.append(
                  QDateTime::fromMSecsSinceEpoch(
                      _el.text().toULongLong()*1000)
                        .toString("yyyy-MM-dd_HH:mm:ss"));
        };
        _el = doc
                .firstChildElement("domainsnapshot")
                .firstChildElement("description");
        if ( !_el.isNull() ) {
            _desc.clear();
            _desc.append(_el.text());
        };
    };
    // flags: extra flags; not used yet,
    // so callers should always pass 0
    int current = virDomainSnapshotIsCurrent(snapShot, 0);
    if ( nullptr!=snapShot ) {
        model->insertRow(row, parent);
        model->setData(
                    model->index(row, 0, parent),
                    name,
                    Qt::DisplayRole);
        model->setData(
                    model->index(row, 0, parent),
                    (current>0),
                    Qt::DecorationRole);
        model->setData(
                    model->index(row, 0, parent),
                    _desc,
                    Qt::ToolTipRole);
        model->setData(
                    model->index(row, 1, parent),
                    _data,
                    Qt::DisplayRole);
        // By default, this command covers
        // only direct children; use flag=0
        int namesLen = virDomainSnapshotNumChildren(snapShot, 0);
        if ( namesLen>0 ) {
            char *names;
            int ret = virDomainSnapshotListChildrenNames(
                        snapShot, &names, namesLen, 0);
            if ( ret>0 ) {
                for (int i = 0; i<ret; i++) {
                    addSnapshotChild(
                                i,
                                model->index(row, 0, parent),
                                (&names)[i]);
                };
            };
        };
        virDomainSnapshotFree(snapShot);
    };
}
void SnapshotActionDialog::setDomainSnapshots()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    domain = virDomainLookupByName(
                *ptr_ConnPtr, domName.toUtf8().data());
    int namesLen = virDomainSnapshotNum(
                domain, VIR_DOMAIN_SNAPSHOT_LIST_ROOTS);
    if ( namesLen>0 ) {
        char *names;
        int ret = virDomainSnapshotListNames(
                    domain,
                    &names,
                    namesLen,
                    VIR_DOMAIN_SNAPSHOT_LIST_ROOTS);
        if ( ret>0 ) {
            for (int i = 0; i<ret; i++) {
                addSnapshotChild(
                            i,
                            snapshotTree->rootIndex(),
                            (&names)[i]);
            };
        };
    };
}
void SnapshotActionDialog::accept()
{
    TreeItem *item = static_cast<TreeItem*>(
            snapshotTree->currentIndex().internalPointer());
    if ( nullptr==item ) {
        cancelled();
    } else {
        params.append(item->data(0).toString());
        //qDebug()<<params;
        done(result());
    };
}
void SnapshotActionDialog::reject()
{
    done(0);
}
void SnapshotActionDialog::cancelled()
{
    flags = 0;
    params.clear();
    setResult(0);
    changeDialogState(false);
}
void SnapshotActionDialog::changeDialogState(bool state)
{
    snapshotTree->setEnabled(!state);
    toolBar->setEnabled(!state);
    ok->setEnabled(state);
    cancel->setEnabled(state);
    closeWdg->setEnabled(!state);
}
void SnapshotActionDialog::detectTriggeredAction(QAction *act)
{
    if ( act==revertAction ) {
        changeDialogState(true);
        params.append("revertVirtDomainSnapshot");
        flags = revertFlagsMenu->getCompositeFlag();
        setResult(REVERT_TO_DOMAIN_SNAPSHOT);
    } else if ( act==deleteAction ) {
        changeDialogState(true);
        params.append("deleteVirtDomainSnapshot");
        flags = deleteFlagsMenu->getCompositeFlag();
        setResult(DELETE_DOMAIN_SNAPSHOT);
    } else if ( act==refreshAction ) {
        snapshotTree->setEnabled(false);
        clearSnapshotTree();
        setDomainSnapshots();
        snapshotTree->setEnabled(true);
    } else if ( act==getXMLDescAction ) {
        showSnapsotXMLDesc();
    };
}
void SnapshotActionDialog::showSnapsotXMLDesc()
{
    if ( snapshotTree->currentIndex().isValid() ) {
        TreeItem *item = static_cast<TreeItem*>(
                snapshotTree->currentIndex().internalPointer());
        if ( nullptr!=item ) {
            // flags: extra flags; not used yet,
            // so callers should always pass 0
            virDomainSnapshotPtr snapShot =
                    virDomainSnapshotLookupByName(
                        domain,
                        item->data(0).toByteArray().data(),
                        0);
            char *xmlDesc =
                    virDomainSnapshotGetXMLDesc(
                        snapShot, 0);
            if ( nullptr!=xmlDesc ) {
                QTemporaryFile f;
                f.setAutoRemove(false);
                f.setFileTemplate(
                            QString("%1%2XML_Desc-XXXXXX.xml")
                            .arg(QDir::tempPath())
                            .arg(QDir::separator()));
                bool read = f.open();
                if (read) f.write(xmlDesc);
                QString xml = f.fileName();
                f.close();
                free(xmlDesc);
                QDesktopServices::openUrl(QUrl(xml));
            };
        };
    };
}
