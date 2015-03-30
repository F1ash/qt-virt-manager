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
    snapshotTree = new QTreeView(this);
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
}

/* private slots */
void SnapshotActionDialog::accept()
{
    done(1);
}
void SnapshotActionDialog::reject()
{
    done(0);
}
