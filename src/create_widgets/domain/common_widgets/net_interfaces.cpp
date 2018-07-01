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
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget_Threaded(parent, connPtrPtr)
{
    typeLabel = new QLabel(tr("Type:"), this);
    type = new QComboBox(this);
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);

    info = new QStackedWidget(this);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(typeWdg);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    hlpThread->start();
}

/* public slots */
QDomDocument NetInterfaces::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) doc = wdg->getDataDocument();
    return doc;
}

/* private slots */
void NetInterfaces::init_wdg()
{
    if ( hlpThread->connType.toLower().compare("lxc")==0 ) {
        type->addItems(LXC_NET_TYPES);
    } else if ( hlpThread->connType.toLower().compare("qemu")==0 ) {
        type->addItems(QEMU_NET_TYPES);
    } else if ( hlpThread->connType.toLower().compare("xen")==0 ) {
        type->addItems(XEN_NET_TYPES);
    };
    for (int i=0; i<type->count(); i++) {
        setWidgets(type->itemText(i).toLower());
    };
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
    for ( int i=0; i<info->count(); i++ ) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
void NetInterfaces::setWidgets(QString _type)
{
    if ( _type.startsWith("multicast") ) {
        info->addWidget(new MultiCast_Tunnel(this));
    } else if ( _type.startsWith("tcp") ) {
        info->addWidget(new TCP_Tunnel(this));
    } else if ( _type.startsWith("pci") ) {
        info->addWidget(new PCI_Passthrough(this, ptr_ConnPtr));
    } else if ( _type.startsWith("direct") ) {
        info->addWidget(new DirectAttachment(this, ptr_ConnPtr));
    } else if ( _type.startsWith("generic") ) {
        info->addWidget(new Generic_Ethernet(this, ptr_ConnPtr));
    } else if ( _type.startsWith("userspace") ) {
        info->addWidget(new Userspace_SLIRP(this, ptr_ConnPtr));
    } else if ( _type.startsWith("bridge") ) {
        info->addWidget(new Bridge_to_LAN(this, ptr_ConnPtr));
    } else if ( _type.startsWith("virtual") ) {
        info->addWidget(new Virtual_Network(this, ptr_ConnPtr));
    };
}
