#include "channel_device.h"

ChannelDevice::ChannelDevice(QWidget *parent) :
    CharDevice(parent)
{
    devType->insertItem(6, "Spice Agent (spicevmc)", "spicevmc");
    chanType = new QComboBox(this);
    chanType->setEditable(true);
    chanType->addItem("com.redhat.spice.0");
    chanType->addItem("org.qemu.guest_agent.0");
    chanType->addItem("org.libguestfs.channel.0");
    commonLayout->insertWidget(0, chanType, -1);
    connect(chanType, SIGNAL(editTextChanged(const QString&)),
            this, SLOT(chanNameChanged(const QString&)));
    //connect(chanType, SIGNAL(currentIndexChanged(const QString&)),
    //        this, SLOT(chanNameChanged(const QString&)));
    devType->setCurrentIndex(6);
}
ChannelDevice::~ChannelDevice()
{
    disconnect(chanType, SIGNAL(editTextChanged(const QString&)),
               this, SLOT(chanNameChanged(const QString&)));
    delete chanType;
    chanType = 0;
}
/* public slots */
QDomNodeList ChannelDevice::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement device, _target;
    QDomNodeList result, list;
    switch (devType->currentIndex()) {
    case 0:
        list = ptyWdg->getNodeList();
        break;
    case 1:
        list = devWdg->getNodeList();
        break;
    case 2:
        list = fileWdg->getNodeList();
        break;
    case 3:
        list = tcpWdg->getNodeList();
        break;
    case 4:
        list = udpWdg->getNodeList();
        break;
    case 5:
        list = unixWdg->getNodeList();
        break;
    case 6:
        /* nothing there */
        break;
    default:
        break;
    };
    device = doc.createElement("device");
    uint j = 0;
    uint count = list.length();
    for (uint i=0; i<count;i++) {
        //qDebug()<<list.item(j).nodeName()<<i;
        /* target element create later */
        if (!list.item(j).isNull() && list.item(j).nodeName()!="target")
            device.appendChild(list.item(j));
        else ++j;
    };
    _target = doc.createElement("target");
    _target.setAttribute("type", "virtio");
    _target.setAttribute("name", chanType->currentText());
    device.appendChild(_target);
    doc.appendChild(device);
    result = doc.firstChildElement("device").childNodes();
    return result;
}

/* private slots */
void ChannelDevice::chanNameChanged(const QString &text)
{
    //qDebug()<<text;
    if ( text.contains("spice") ) {
        devType->setCurrentIndex(6);
    } else {
        devType->setCurrentIndex(5);
        unixWdg->setPath(QString("/var/lib/libvirt/qemu/channel/target/%1").arg(chanType->currentText()));
    }
}
