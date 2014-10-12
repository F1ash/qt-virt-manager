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
    devType->setCurrentIndex(6);
    // see for: CharDevice_Edit
    connect(devType, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    for (uint i=0; i<charDevWdg->count(); i++) {
        connect(charDevWdg->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
QDomDocument ChannelDevice::getDataDocument() const
{
    QDomDocument doc;
    CharDevice *wdg = static_cast<CharDevice*>(charDevWdg->currentWidget());
    doc = wdg->getDataDocument();
    if ( doc.isNull() ) {
        QDomElement _device, _devDesc;
        _device = doc.createElement("device");
        _devDesc = doc.createElement("channel");
        _device.appendChild(_devDesc);
        doc.appendChild(_device);
    };
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
void ChannelDevice::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _target, _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("channel");
    _target = _device.firstChildElement("target");
    QString _chanType;
    while ( !_target.isNull() ) {
        if ( _target.hasAttribute("name") ) {
            _chanType = _target.attribute("name", "com.redhat.spice.0");
            break;
        };
        _target = _target.nextSiblingElement("target");
    };
    int idx = chanType->findText(
                _chanType,
                Qt::MatchExactly);
    chanType->setCurrentIndex( (idx<0)? 0:idx );
    QString _type = _device.attribute("type", "unix");
    idx = devType->findText(_type, Qt::MatchEndsWith);
    devType->setCurrentIndex( (idx<0)? 0:idx );
    static_cast<_QWidget*>(charDevWdg->currentWidget())->setDataDescription(xmlDesc);
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
