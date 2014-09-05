#include "net_interfaces.h"

/*
 * http://libvirt.org/formatdomain.html#elementsNICS
 */

#define LXC_NET_TYPES QStringList()<<"Multicast tunnel"<<"TCP tunnel"\
    <<"PCI Passthrough"<<"Generic ethernet connection"\
    <<"Userspace SLIRP stack"<<"Bridge to LAN"<<"Virtual network"

#define QEMU_NET_TYPES QStringList()<<"Multicast tunnel"<<"TCP tunnel"\
    <<"PCI Passthrough"<<"Direct attachment to physical interface"\
    <<"Generic ethernet connection"<<"Userspace SLIRP stack"\
    <<"Bridge to LAN"<<"Virtual network"

#define XEN_NET_TYPES QStringList()<<"None"

NetInterfaces::NetInterfaces(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    typeLabel = new QLabel("Type:", this);
    type = new QComboBox(this);
    connType = QString(virConnectGetType(currWorkConnect)).toLower();
    if ( connType=="lxc" ) {
        type->addItems(LXC_NET_TYPES);
    } else if ( connType=="qemu" ) {
        type->addItems(QEMU_NET_TYPES);
    } else if ( connType=="xen" ) {
        type->addItems(XEN_NET_TYPES);
    };
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(typeWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
    typeChanged(0);
}

/* public slots */
QDomDocument NetInterfaces::getDevDocument() const
{
    return info->getDevDocument();
}

/* private slots */
void NetInterfaces::typeChanged(int i)
{
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    QString _type = type->currentText().toLower();
    if ( _type.startsWith("multicast") ) {
        info = new MultiCast_Tunnel(this);
    } else if ( _type.startsWith("tcp") ) {
        info = new TCP_Tunnel(this);
    } else if ( _type.startsWith("pci") ) {
        info = new PCI_Passthrough(this, currWorkConnect);
    } else if ( _type.startsWith("direct") ) {
        info = new DirectAttachment(this, currWorkConnect);
    } else if ( _type.startsWith("generic") ) {
        info = new Generic_Ethernet(this, currWorkConnect);
    } else if ( _type.startsWith("userspace") ) {
        info = new Userspace_SLIRP(this, currWorkConnect);
    } else if ( _type.startsWith("bridge") ) {
        info = new Bridge_to_LAN(this, currWorkConnect);
    } else if ( _type.startsWith("virtual") ) {
        info = new Virtual_Network(this, currWorkConnect);
    } else info = new _QWidget(this);
    commonLayout->insertWidget(1, info, -1);
}
