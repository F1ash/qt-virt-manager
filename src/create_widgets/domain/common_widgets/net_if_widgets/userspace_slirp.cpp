#include "userspace_slirp.h"

#define INFO QString("\n\
Provides a virtual LAN with NAT to the outside world.\n\
The virtual network has DHCP & DNS services and \n\
will give the guest VM addresses starting from 10.0.2.15.\n\
The default router will be 10.0.2.2 and the DNS server\n\
will be 10.0.2.3. This networking is the only option\n\
for unprivileged users who need their VMs\n\
to have outgoing access. ")

Userspace_SLIRP::Userspace_SLIRP(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    mac = new MAC_Address(this);
    info = new QLabel(this);
    info->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(32));
    info->setText(INFO);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(info);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument Userspace_SLIRP::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    if ( !mac->getMACAddress().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };
    _devDesc.setAttribute("type", "user");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
