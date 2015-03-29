#include "system_checkpoint.h"

SystemCheckpoint::SystemCheckpoint(QWidget *parent, bool _state, bool _external) :
    _SnapshotStuff(parent, _state), external(_external)
{
    QString _memAttr("<b>Memory</b> is ");
    if ( !state ) {
        _memAttr.append("no (Domain inactive)");
    } else
        _memAttr.append(external? "external" : "internal");
    memLabel->setText(_memAttr);
    filePathLabel->setVisible(state && external);
    filePath->setVisible(state && external);
}

/* public slots */
QDomDocument SystemCheckpoint::getElements() const
{
    QDomDocument doc;
    QDomElement _memory, _disks;
    _memory = doc.createElement("memory");
    if ( !state ) {
        _memory.setAttribute("snapshot", "no");
    } else if ( external ) {
        _memory.setAttribute("snapshot", "external");
    } else
        _memory.setAttribute("snapshot", "internal");
    doc.appendChild(_memory);
    _disks = doc.createElement("disks");
    doc.appendChild(_disks);
    return doc;
}
