#include "sound_device_edit.h"

SoundDevice_Edit::SoundDevice_Edit(QWidget *parent) :
    SoundDevice(parent)
{
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(defaultICH6reg, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(duplexICH6reg, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(microICH6reg, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void SoundDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _codec, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("sound");
    QString _model = _device.attribute("model", "");
    int idx = model->findText(_model, Qt::MatchContains);
    model->setCurrentIndex( (idx<0)? 0:idx );
    _codec = _device.firstChildElement("codec");
    if ( !_codec.isNull() ) {
        QString codec = _codec.attribute("type", "");
        if ( codec.compare("micro")==0 ) {
            microICH6reg->setChecked(true);
        } else if ( codec.compare("duplex")==0 ) {
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
        if ( wdg!=Q_NULLPTR ) {
            wdg->domain->setText( _addr.attribute("domain") );
            wdg->bus->setText( _addr.attribute("bus") );
            wdg->slot->setText( _addr.attribute("slot") );
            wdg->function->setValue( _addr.attribute("function")
                                     .split("x").last().toInt() );
            if ( _addr.hasAttribute("multifunction") ) {
                wdg->multifunction->setEnabled(true);
                wdg->multifunction->setChecked(
                            _addr.attribute("multifunction").compare("on")==0 );
            };
        };
    };
}
