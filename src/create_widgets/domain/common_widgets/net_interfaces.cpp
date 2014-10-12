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

    info = new QStackedWidget(this);
    for (uint i=0; i<type->count(); i++) {
        setWidgets(type->itemText(i).toLower());
    };

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(typeWdg);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument NetInterfaces::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    return wdg->getDataDocument();
}

/* private slots */
void NetInterfaces::setWidgets(QString _type)
{
    if ( _type.startsWith("multicast") ) {
        info->addWidget(new MultiCast_Tunnel(this));
    } else if ( _type.startsWith("tcp") ) {
        info->addWidget(new TCP_Tunnel(this));
    } else if ( _type.startsWith("pci") ) {
        info->addWidget(new PCI_Passthrough(this, currWorkConnect));
    } else if ( _type.startsWith("direct") ) {
        info->addWidget(new DirectAttachment(this, currWorkConnect));
    } else if ( _type.startsWith("generic") ) {
        info->addWidget(new Generic_Ethernet(this, currWorkConnect));
    } else if ( _type.startsWith("userspace") ) {
        info->addWidget(new Userspace_SLIRP(this, currWorkConnect));
    } else if ( _type.startsWith("bridge") ) {
        info->addWidget(new Bridge_to_LAN(this, currWorkConnect));
    } else if ( _type.startsWith("virtual") ) {
        info->addWidget(new Virtual_Network(this, currWorkConnect));
    };
}
