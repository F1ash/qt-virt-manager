#include "volume_disk.h"

Volume_Disk::Volume_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
}

/* public slots */
QDomDocument Volume_Disk::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _devDesc.appendChild(_source);

    _devDesc.setAttribute("type", "volume");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
