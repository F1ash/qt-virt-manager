#include "memballoon_edit.h"

MemBalloon_Edit::MemBalloon_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    MemBalloon(parent, connPtrPtr)
{
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(periodLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(period, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void MemBalloon_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _addr, _stats;
    _device = doc.firstChildElement("device")
            .firstChildElement("memballoon");
    QString _attr;
    _attr = _device.attribute("model");
    int idx = model->findData(
                _attr, Qt::UserRole, Qt::MatchExactly);
    model->setCurrentIndex( (idx<0)? 0:idx );
    _stats = _device.firstChildElement("stats");
    periodLabel->setChecked( !_stats.isNull() );
    if ( !_stats.isNull() ) {
        period->setValue( _stats.attribute("period", "0").toInt() );
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
