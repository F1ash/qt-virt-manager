#include "random.h"

/*
 * http://libvirt.org/formatdomain.html#elementsRng
 */

Random::Random(QWidget *parent) :
    _QWidget(parent)
{
    modelLabel = new QLabel(tr("Model:"), this);
    model = new QComboBox(this);
    model->setEditable(false);
    model->addItem("virtio");
    model->insertSeparator(1);
    model->addItem("Custom model");
    model->setItemIcon(0, QIcon::fromTheme("drive-virtual-io"));
    rate = new QCheckBox(tr("Use Rate limit:"), this);
    periodLabel = new QLabel(tr("Period:"), this);
    periodLabel->setEnabled(false);
    bytesLabel = new QLabel(tr("Bytes:"), this);
    bytesLabel->setEnabled(false);
    period = new QSpinBox(this);
    period->setRange(0, 65535);
    period->setValue(1000);
    period->setEnabled(false);
    bytes = new QSpinBox(this);
    bytes->setRange(0, 65535);
    bytes->setValue(1024);
    bytes->setEnabled(false);
    bkModelLabel = new QLabel(tr("Backend Model:"), this);
    bkModel = new QComboBox(this);
    bkModel->addItems(QStringList()<<"random"<<"egd");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(modelLabel, 0, 0);
    baseLayout->addWidget(model, 0, 1);
    baseLayout->addWidget(rate, 1, 0);
    baseLayout->addWidget(periodLabel, 2, 0);
    baseLayout->addWidget(period, 2, 1);
    baseLayout->addWidget(bytesLabel, 3, 0);
    baseLayout->addWidget(bytes, 3, 1);
    baseLayout->addWidget(bkModelLabel, 4, 0);
    baseLayout->addWidget(bkModel, 4, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    bkRandomLabel = new QLabel(tr("Source:"), this);
    bkRandom = new QComboBox(this);
    bkRandom->addItems(QStringList()<<"default"<<"/dev/random"<<"/dev/hwrng");
    bkRandomLayout = new QHBoxLayout(this);
    bkRandomLayout->addWidget(bkRandomLabel);
    bkRandomLayout->addWidget(bkRandom);
    bkRandomLayout->addStretch(-1);
    bkRandomWdg = new QWidget(this);
    bkRandomWdg->setLayout(bkRandomLayout);

    bkEGD = new CharDevice(this, Q_NULLPTR, Q_NULLPTR, "serial");

    bkWdg = new QStackedWidget(this);
    bkWdg->addWidget(bkRandomWdg);
    bkWdg->addWidget(bkEGD);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(bkWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(modelChanged(int)));
    connect(rate, SIGNAL(toggled(bool)),
            this, SLOT(rateChecked(bool)));
    connect(bkModel, SIGNAL(currentIndexChanged(int)),
            bkWdg, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument Random::getDataDocument() const
{
    QDomDocument doc, _charDevDoc;
    QDomElement _device, _rng, _rate, _backend;
    _device = doc.createElement("device");
    _rng = doc.createElement("rng");
    _rng.setAttribute("model", model->currentText());
    if ( rate->isChecked() ) {
        _rate = doc.createElement("rate");
        _rate.setAttribute("period", period->text());
        _rate.setAttribute("bytes", bytes->text());
        _rng.appendChild(_rate);
    };
    _backend = doc.createElement("backend");
    QString _bkModel = bkModel->currentText();
    _backend.setAttribute("model", _bkModel);
    if ( _bkModel.compare("random")==0 ) {
        QDomText _data = doc.createTextNode(
                    bkRandom->currentText());
        if ( _data.toText().data().compare("default")!=0 )
            _backend.appendChild(_data);
    } else if ( _bkModel.compare("egd")==0 ) {
        _charDevDoc = bkEGD->getDataDocument();
        QString _type = _charDevDoc
                .firstChildElement("device")
                .firstChildElement("serial")
                .attribute("type");
        _backend.setAttribute("type", _type);
        QDomNodeList list = _charDevDoc
                .firstChildElement("device")
                .firstChildElement("serial")
                .childNodes();
        int j = 0;
        int count = list.length();
        for (int i=0; i<count;i++) {
            if (!list.item(j).isNull()) {
                _backend.appendChild(list.item(j));
            } else ++j;
        };
    };
    _rng.appendChild(_backend);
    _device.appendChild(_rng);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Random::rateChecked(bool state)
{
    periodLabel->setEnabled(state);
    bytesLabel->setEnabled(state);
    period->setEnabled(state);
    bytes->setEnabled(state);
}
void Random::modelChanged(int i)
{
    model->setEditable(i==2);
    model->clearEditText();
}
