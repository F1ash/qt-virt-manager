#include "smartcard_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsSmartcard
 */

SmartCardDevice::SmartCardDevice(QWidget *parent) :
    _QWidget(parent)
{
    mode = new QComboBox(this);
    mode->addItem(tr("Host"), "host");
    mode->addItem(tr("Passthrough"), "passthrough");
    channel = new ChannelDevice(this);
    channel->setVisible(false);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "ccid",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(mode);
    commonLayout->addWidget(channel);
    commonLayout->addWidget(addr);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(mode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(modeTypeChanged(int)));
}

/* public slots */
QDomDocument SmartCardDevice::getDataDocument() const
{
    QDomDocument doc, _channel;
    QDomElement _device, _devDesc, _address;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("smartcard");
    AttrList l = addr->getAttrList();
    if ( !l.isEmpty() ) {
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };

    _device.appendChild(_devDesc);
    QString _mode = mode->itemData(mode->currentIndex(), Qt::UserRole)
            .toString();
    _devDesc.setAttribute("mode", _mode);
    if ( _mode=="passthrough" ) {
        //_channel = static_cast<CharDevice*>(
        //            channel->charDevWdg->currentWidget())->getDataDocument();
        _channel = channel->getDataDocument();
        /*
         * current DomNode is removed to root-element
         * but NULL-elemens not removed
         * therefore keep to seek on not-NULL next element
         */
        QDomNodeList list = _channel
                .firstChildElement("device")
                .firstChildElement("channel")
                .childNodes();
        uint j = 0;
        int count = list.length();
        for (int i=0; i<count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if ( !list.item(j).isNull() &&
                  list.item(j).toElement().tagName()!="target" )
                _devDesc.appendChild(list.item(j));
            else ++j;
        };
        QString _type;
#if QT_VERSION>=0x050200
        _type = channel->devType->currentData(Qt::UserRole).toString();
#else
        _type = channel->devType->itemData(
                    channel->devType->currentIndex(), Qt::UserRole)
                .toString();
#endif
        _devDesc.setAttribute("type", _type);
    };
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}

/* private slots */
void SmartCardDevice::modeTypeChanged(int i)
{
    QString _mode = mode->itemData(i, Qt::UserRole).toString();
    if ( _mode=="passthrough" ) {
        channel->setVisible(true);
    } else {
        channel->setVisible(false);
    };
}
