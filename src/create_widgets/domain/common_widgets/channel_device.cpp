#include "channel_device.h"

ChannelDevice::ChannelDevice(QWidget *parent) :
    CharDevice(parent, NULL, NULL, QString("channel"))
{
    devType->insertItem(6, "Spice Agent /spicevmc");
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
    CharDevice *wdg = static_cast<CharDevice*>(charDevWdg->currentWidget());
    doc = wdg->getDevDocument();
    QDomElement _target = doc.createElement("target");
    _target.setAttribute("type", "virtio");
    _target.setAttribute("name", chanType->currentText());
    doc.firstChildElement("device")
            .firstChildElement("channel")
            .appendChild(_target);
    QString _type = devType->currentText().split("/").last();
    doc.firstChildElement("device")
            .firstChildElement("channel")
            .setAttribute("type", _type);
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
        UnixWidget *wdg = static_cast<UnixWidget*>(charDevWdg->currentWidget());
        wdg->setPath(QString("/var/lib/libvirt/qemu/channel/target/%1")
                     .arg(chanType->currentText()));
    }
}
