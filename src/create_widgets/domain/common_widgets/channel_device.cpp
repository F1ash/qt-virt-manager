#include "channel_device.h"

ChannelDevice::ChannelDevice(QWidget *parent) :
    CharDevice(parent, NULL, NULL, QString("channel"))
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

/* public slots */
QDomDocument ChannelDevice::getDevDocument() const
{
    QDomDocument doc;
    switch (devType->currentIndex()) {
    case 0:
        doc = ptyWdg->getDevDocument();
        break;
    case 1:
        doc = devWdg->getDevDocument();
        break;
    case 2:
        doc = fileWdg->getDevDocument();
        break;
    case 3:
        doc = tcpWdg->getDevDocument();
        break;
    case 4:
        doc = udpWdg->getDevDocument();
        break;
    case 5:
        doc = unixWdg->getDevDocument();
        break;
    case 6:
        doc = ptyWdg->getDevDocument();
        break;
    default:
        break;
    };
    QDomElement _target = doc.createElement("target");
    _target.setAttribute("type", "virtio");
    _target.setAttribute("name", chanType->currentText());
    doc.firstChildElement("device")
            .firstChildElement("channel")
            .appendChild(_target);
    doc.firstChildElement("device")
            .firstChildElement("channel")
            .setAttribute("type",
                          devType->itemData(
                              devType->currentIndex(),
                              Qt::UserRole).toString());
    return doc;
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
