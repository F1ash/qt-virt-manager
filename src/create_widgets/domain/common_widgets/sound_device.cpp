#include "sound_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsSound
 */

SoundDevice::SoundDevice(QWidget *parent) :
    _QWidget(parent)
{
    modelLabel = new QLabel("Sound Device Model:", this);
    model = new QComboBox(this);
    model->addItem("es1370");
    model->addItem("sb16");
    model->addItem("ac97");
    model->addItem("ich6");
    model->addItem("usb");
    defaultICH6reg = new QRadioButton("Allow playback\nrecording", this);
    defaultICH6reg->setChecked(true);
    duplexICH6reg = new QRadioButton("Line-in\nline-out", this);
    microICH6reg = new QRadioButton("Speaker\nmicrophone", this);
    regWdgLayout = new QHBoxLayout(this);
    regWdgLayout->addWidget(defaultICH6reg);
    regWdgLayout->addWidget(duplexICH6reg);
    regWdgLayout->addWidget(microICH6reg);
    regWdg = new QWidget(this);
    regWdg->setLayout(regWdgLayout);
    regWdg->setVisible(false);
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("pci", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QGridLayout(this);
    commonLayout->addWidget(modelLabel, 0, 0);
    commonLayout->addWidget(model, 0, 1);
    commonLayout->addWidget(regWdg, 1, 0, 2, 2);
    commonLayout->addWidget(addr, 3, 0, 4, 2, Qt::AlignTop);
    setLayout(commonLayout);
    connect(model, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(modelChanged(QString)));
}

/* public slots */
QDomDocument SoundDevice::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("sound");
    if ( model->currentText()=="ich6" && !defaultICH6reg->isChecked() ) {
        QDomElement _codec = doc.createElement("codec");
        if ( duplexICH6reg->isChecked() )
            _codec.setAttribute("type", "duplex");
        if ( microICH6reg->isChecked() )
            _codec.setAttribute("type", "micro");
        _devDesc.appendChild(_codec);
    };
    // WARNING: address implemented experimentally
    AttrList l = addr->getAttrList();
    if ( !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _device.appendChild(_devDesc);
    _devDesc.setAttribute("model", model->currentText());
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void SoundDevice::modelChanged(QString _model)
{
    regWdg->setVisible( ( _model=="ich6" ) );
}
