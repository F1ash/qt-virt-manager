#include "video_device_edit.h"

VideoDevice_Edit::VideoDevice_Edit(QWidget *parent) :
    VideoDevice(parent)
{
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(accel2d, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(accel3d, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(vram, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void VideoDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _model, _accel, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("video");
    _model = _device.firstChildElement("model");
    QString _type = _model.attribute("type", "Cirrus");
    int idx = model->findText(_type, Qt::MatchContains);
    model->setCurrentIndex( (idx<0)? 0:idx );
    vram->setValue(_model.attribute("vram", "0").toInt()/1024);
    _accel = _model.firstChildElement("acceleration");
    if ( !_accel.isNull() ) {
        accel2d->setChecked(
                    _accel.attribute("accel2d").compare("yes")==0);
        accel3d->setChecked(
                    _accel.attribute("accel3d").compare("yes")==0);
    } else {
        accel2d->setChecked(false);
        accel3d->setChecked(false);
    };
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        PciAddr *wdg = static_cast<PciAddr*>( addr->getCurrentAddrWidget() );
        if ( wdg!=nullptr ) {
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
