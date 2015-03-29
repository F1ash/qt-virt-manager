#include "disk_snapshot.h"

DiskSnapshot::DiskSnapshot(QWidget *parent, bool _state, bool _external) :
    _SnapshotStuff(parent, _state), external(_external)
{
    memLabel->setText("<b>Memory</b> is no");
    memLabel->setVisible(true);
    filePathLabel->setVisible(false);
    filePath->setVisible(false);
}

/* public slots */
QDomDocument DiskSnapshot::getElements() const
{
    QDomDocument doc;
    QDomElement _memory, _disks;
    _memory = doc.createElement("memory");
    _memory.setAttribute("snapshot", "no");
    doc.appendChild(_memory);
    _disks = doc.createElement("disks");
    doc.appendChild(_disks);
    return doc;
}
