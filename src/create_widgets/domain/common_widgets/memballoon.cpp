#include "memballoon.h"

MemBalloon::MemBalloon(QWidget *parent, virConnectPtr conn) :
    _QWidget(parent, conn)
{
    modelLabel = new QLabel("Model:", this);
    model = new QComboBox(this);
    model->addItems(QStringList()<<"NONE"<<"QEMU"<<"XEN");
    periodLabel = new QCheckBox("Period", this);
    periodLabel->setVisible(false);
    period = new QSpinBox(this);
    period->setVisible(false);
    period->setRange(0, 1024);
    period->setValue(10);
    period->setEnabled(false);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(modelLabel, 0, 0);
    baseLayout->addWidget(model, 0, 1);
    baseLayout->addWidget(periodLabel, 1, 0);
    baseLayout->addWidget(period, 1, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    addrLabel = new QCheckBox("Address:", this);
    addrLabel->setVisible(false);
    addr = new PciAddr(this);
    addr->setEnabled(false);
    addr->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(addrLabel);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(model, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(modelChanged(QString)));
    connect(periodLabel, SIGNAL(toggled(bool)),
            period, SLOT(setEnabled(bool)));
    connect(addrLabel, SIGNAL(toggled(bool)),
            addr, SLOT(setEnabled(bool)));
}

/* public slots */
QDomDocument MemBalloon::getDevDocument() const
{
    QDomDocument doc;
    QDomElement _device, _memballoon, _stats, _address;
    _device = doc.createElement("device");
    _memballoon = doc.createElement("memballoon");
    _memballoon.setAttribute("model", model->currentText().toLower());
    if ( periodLabel->isChecked() ) {
        _stats = doc.createElement("stats");
        _stats.setAttribute("period", period->text());
        _memballoon.appendChild(_stats);
    };
    if ( addrLabel->isChecked() ) {
        AttrList l= addr->getAttrList();
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _address.setAttribute("type", "pci");
        _memballoon.appendChild(_address);
    };

    _device.appendChild(_memballoon);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void MemBalloon::modelChanged(QString _model)
{
    if ( _model.toLower()=="none" ) {
        periodLabel->setVisible(false);
        period->setVisible(false);
        addrLabel->setVisible(false);
        addr->setVisible(false);
    } else if ( _model.toLower()=="qemu" ) {
        periodLabel->setVisible(true);
        period->setVisible(true);
        addrLabel->setVisible(true);
        addr->setVisible(true);
    } else if ( _model.toLower()=="xen" ) {
        periodLabel->setVisible(false);
        period->setVisible(false);
        addrLabel->setVisible(true);
        addr->setVisible(true);
    }
}
