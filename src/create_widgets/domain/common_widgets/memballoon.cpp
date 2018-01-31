#include "memballoon.h"

MemBalloon::MemBalloon(QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    modelLabel = new QLabel(tr("Model for:"), this);
    model = new QComboBox(this);
    model->addItem("NONE", "none");
    model->addItem("QEMU/KVM", "virtio");
    model->addItem("XEN", "xen");
    model->setItemIcon(0, QIcon::fromTheme(""));
    model->setItemIcon(1, QIcon::fromTheme("qemu"));
    model->setItemIcon(2, QIcon::fromTheme("xen"));
    periodLabel = new QCheckBox(tr("Period"), this);
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
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(modelChanged(int)));
    connect(periodLabel, SIGNAL(toggled(bool)),
            period, SLOT(setEnabled(bool)));
}

/* public slots */
QDomDocument MemBalloon::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _memballoon, _stats, _address;
    _device = doc.createElement("device");
    _memballoon = doc.createElement("memballoon");
    _memballoon.setAttribute(
                "model",
                model->itemData(
                    model->currentIndex(),
                    Qt::UserRole)
                .toString());
    if ( periodLabel->isChecked() ) {
        _stats = doc.createElement("stats");
        _stats.setAttribute("period", period->text());
        _memballoon.appendChild(_stats);
    };
    if ( addr->use->isChecked() ) {
        AttrList l= addr->getAttrList();
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _memballoon.appendChild(_address);
    };

    _device.appendChild(_memballoon);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void MemBalloon::modelChanged(int idx)
{
    QString _model = model->itemData(idx, Qt::UserRole).toString();
    if ( _model=="none" ) {
        periodLabel->setChecked(false);
        periodLabel->setVisible(false);
        period->setVisible(false);
        addr->use->setChecked(false);
        addr->setVisible(false);
    } else if ( _model=="virtio" ) {
        periodLabel->setVisible(true);
        period->setVisible(true);
        addr->setVisible(true);
    } else if ( _model=="xen" ) {
        periodLabel->setChecked(false);
        periodLabel->setVisible(false);
        period->setVisible(false);
        addr->setVisible(true);
    }
}
