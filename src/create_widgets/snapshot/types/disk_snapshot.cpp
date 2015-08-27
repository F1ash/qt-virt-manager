#include "disk_snapshot.h"

DiskSnapshot::DiskSnapshot(QWidget *parent, bool _state, bool _external) :
    _SnapshotStuff(parent, _state, _external)
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
    QDomElement _memory;
    _memory = doc.createElement("memory");
    _memory.setAttribute("snapshot", "no");
    doc.appendChild(_memory);
    doc.appendChild(disks->getElements(diskWarn->isChecked()));
    //qDebug()<<doc.toByteArray(4).data()<<"SystemCheckpoint";
    return doc;
}
