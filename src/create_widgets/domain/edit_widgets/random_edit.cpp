#include "random_edit.h"

Random_Edit::Random_Edit(QWidget *parent) :
    Random(parent)
{
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(bkModel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(bkRandom, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(rate, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(period, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(bytes, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(bkEGD->devType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<bkEGD->charDevWdg->count(); i++) {
        connect(bkEGD->charDevWdg->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void Random_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _backend, _rate;
    _device = doc.firstChildElement("device")
            .firstChildElement("rng");
    QString _model, _bkModel, _bkRandom, _type, _period, _bytes;
    _rate = _device.firstChildElement("rate");
    rate->setChecked( !_rate.isNull() );
    if ( !_rate.isNull() ) {
        _period = _rate.attribute("period", "1000");
        _bytes = _rate.attribute("bytes", "1024");
        period->setValue(_period.toInt());
        bytes->setValue(_bytes.toInt());
    };
    _model = _device.attribute("model", "custom");
    int idx = model->findText(_model, Qt::MatchContains);
    idx = (idx<0)? 1:idx;
    model->setCurrentIndex(idx);
    if ( idx==1 ) model->setItemText(idx, _model);
    _backend = _device.firstChildElement("backend");
    _bkModel = _backend.attribute("model", "random");
    idx = bkModel->findText(_bkModel, Qt::MatchContains);
    bkModel->setCurrentIndex( (idx<0)? 0:idx );
    if ( _bkModel=="random" ) {
        _bkRandom = _backend.firstChild().toText().data();
        idx = bkRandom->findText(_bkRandom, Qt::MatchContains);
        bkRandom->setCurrentIndex( (idx<0)? 0:idx );
    } else if ( _bkModel=="egd" ) {
        QDomDocument _charDoc;
        QDomElement _charDevice, _serial;
        _charDoc.setContent(QString());
        _charDevice = _charDoc.createElement("device");
        _serial = _charDoc.createElement("serial");
        _charDevice.appendChild(_serial);
        _charDoc.appendChild(_charDevice);
        _type = _backend.attribute("type", "udp");
        _serial.setAttribute("type", _type);
        QDomNodeList _list = _backend.childNodes();
        uint j = 0;
        uint count = _list.length();
        for (int i=0; i<count; i++) {
            if (!_list.item(j).isNull()) {
                _serial.appendChild( _list.item(j) );
            } else
                ++j;
        };
        QString _xml = _charDoc.toString();
        //qDebug()<<_xml;
        int idx = bkEGD->devType->findData(
                    _type,
                    Qt::UserRole,
                    Qt::MatchContains);
        bkEGD->devType->setCurrentIndex( (idx<0)? 0:idx );
        _QWidget *wdg = static_cast<_QWidget*>(
                    bkEGD->charDevWdg->currentWidget() );
        if ( wdg!=nullptr ) wdg->setDataDescription(_xml);
    };
}
