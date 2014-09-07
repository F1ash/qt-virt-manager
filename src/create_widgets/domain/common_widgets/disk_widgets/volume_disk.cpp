#include "volume_disk.h"

Volume_Disk::Volume_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    poolLabel = new QLabel("Pool:", this);
    pool = new QLineEdit(this);
    volumeLabel = new QPushButton("Volume:", this);
    volume = new QLineEdit(this);

    baseLayout->addWidget(poolLabel, 0, 0);
    baseLayout->addWidget(pool, 0, 1);
    baseLayout->addWidget(volumeLabel, 1, 0);
    baseLayout->addWidget(volume, 1, 1);
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
