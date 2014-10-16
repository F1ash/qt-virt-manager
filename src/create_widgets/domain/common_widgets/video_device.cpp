#include "video_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsVideo
 */

VideoDevice::VideoDevice(QWidget *parent) :
    _QWidget(parent)
{
    modelLabel = new QLabel("Video Device Type:", this);
    model = new QComboBox(this);
    model->addItem("VGA");
    model->addItem("Cirrus");
    model->addItem("VMVGA");
    model->addItem("XEN");
    model->addItem("VBOX");
    model->addItem("QXL");
    vramLabel = new QLabel("VRAM (MiB):", this);
    vram = new QSpinBox(this);
    vram->setRange(8, 12800);
    accel2d = new QCheckBox("Acceleration 2D", this);
    accel3d = new QCheckBox("Acceleration 3D", this);
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("pci", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QGridLayout(this);
    commonLayout->addWidget(modelLabel, 0, 0);
    commonLayout->addWidget(model, 0, 1);
    commonLayout->addWidget(vramLabel, 1, 0);
    commonLayout->addWidget(vram, 1, 1);
    commonLayout->addWidget(accel2d, 2, 0);
    commonLayout->addWidget(accel3d, 2, 1);
    commonLayout->addWidget(addr, 3, 0, 5, 2, Qt::AlignTop);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument VideoDevice::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _model, _acceleration, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("video");
    _model = doc.createElement("model");
    _model.setAttribute("type", model->currentText().toLower());
    int _vram = vram->value()*1024;
    _model.setAttribute("vram", QString::number(_vram));
    /*
     * WARNING: possible to use more then 1 head
     */
    _model.setAttribute("heads", "1");
    _devDesc.appendChild(_model);
    if ( accel2d->isChecked() || accel3d->isChecked() ) {
        _acceleration = doc.createElement("acceleration");
        if ( accel2d->isChecked() )
            _acceleration.setAttribute("accel2d", "yes");
        if ( accel3d->isChecked() )
            _acceleration.setAttribute("accel3d", "yes");
        _model.appendChild(_acceleration);
    };
    // WARNING: address implemented experimentally
    AttrList l = addr->getAttrList();
    if ( addr->use->isChecked() && !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
