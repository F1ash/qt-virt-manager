#include "net_interfaces_edit.h"

NetInterfaces_Edit::NetInterfaces_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    NetInterfaces(parent, connPtrPtr)
{
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<info->count(); i++) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void NetInterfaces_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QString _string;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    QString _type = _device.attribute("type");
    if ( _type.compare("mcast")==0 ) {
        _string.append("Multicast");
    } else if ( _type.compare("server")==0 || _type.compare("client")==0 ) {
        _string.append("TCP");
    } else if ( _type.compare("hostdev")==0 ) {
        _string.append("PCI");
    } else if ( _type.compare("direct")==0 ) {
        _string.append("Direct");
    } else if ( _type.compare("ethernet")==0 ) {
        _string.append("Generic");
    } else if ( _type.compare("user")==0 ) {
        _string.append("Userspace");
    } else if ( _type.compare("bridge")==0 ) {
        _string.append("Bridge");
    } else if ( _type.compare("network")==0 ) {
        _string.append("Virtual");
    };
    int idx = type->findText(_string, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
    type->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) wdg->setDataDescription(_xmlDesc);
}
