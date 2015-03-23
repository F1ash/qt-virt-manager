#include "sound_device_edit.h"

SoundDevice_Edit::SoundDevice_Edit(QWidget *parent) :
    SoundDevice(parent)
{
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(defaultICH6reg, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(duplexICH6reg, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(microICH6reg, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void SoundDevice_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _codec, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("sound");
    QString _model = _device.attribute("model", "");
    int idx = model->findText(_model, Qt::MatchContains);
    model->setCurrentIndex( (idx<0)? 0:idx );
    _codec = _device.firstChildElement("codec");
    if ( !_codec.isNull() ) {
        QString codec = _codec.attribute("type", "");
        if ( codec=="micro" ) {
            microICH6reg->setChecked(true);
        } else if ( codec=="duplex" ) {
            duplexICH6reg->setChecked(true);
        } else
            defaultICH6reg->setChecked(true);
    } else {
        defaultICH6reg->setChecked(true);
    };
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        PciAddr *wdg = static_cast<PciAddr*>( addr->getCurrentAddrWidget() );
        wdg->domain->setText( _addr.attribute("domain") );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->slot->setText( _addr.attribute("slot") );
        wdg->function->setValue( _addr.attribute("function")
                                 .split("x").last().toInt() );
        if ( _addr.hasAttribute("multifunction") ) {
            wdg->multifunction->setEnabled(true);
            wdg->multifunction->setChecked(
                        _addr.attribute("multifunction")=="on" );
        };
    };
}
