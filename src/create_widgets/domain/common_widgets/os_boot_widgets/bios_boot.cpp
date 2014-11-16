#include "bios_boot.h"

BIOS_Boot::BIOS_Boot(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    architecture = new _Arch(this, capabilities);
    loaderLabel = new QLabel("Boot loader path:", this);
    loader = new Path_To_File(this);
    QString _placeHolderText = QString("/usr/lib/xen/boot/hvmloader");
    loader->setPlaceholderText(_placeHolderText);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(architecture);
    commonLayout->addWidget(loaderLabel);
    commonLayout->addWidget(loader);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(architecture, SIGNAL(domainType(QString&)),
            this, SIGNAL(domainType(QString&)));
    connect(architecture, SIGNAL(osType(QString&)),
            this, SIGNAL(osType(QString&)));
    connect(architecture, SIGNAL(emulatorType(QString&)),
            this, SIGNAL(emulatorType(QString&)));
    connect(architecture, SIGNAL(archType(QString&)),
            this, SLOT(changeArch(QString&)));
    architecture->setItems();
    // dataChanged connectins
    connect(architecture, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(loader, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument BIOS_Boot::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _os, _type, _loader, _data;
    _data = doc.createElement("data");
    _os = doc.createElement("os");
    _data.appendChild(_os);

    _type = doc.createElement("type");
    _type.setAttribute("arch", arch);
    _type.setAttribute(
                "machine", architecture->getMachine());
    _os.appendChild(_type);

    _loader = doc.createElement("loader");
    data = doc.createTextNode(loader->getPath());
    _loader.appendChild(data);
    _os.appendChild(_loader);

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void BIOS_Boot::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _os, _type, _loader;
    _os = doc
            .firstChildElement("domain")
            .firstChildElement("os");
    _type = _os.firstChildElement("type");
    _loader = _os.firstChildElement("loader");
    QString _attr;
    if ( !_type.isNull() ) {
        _attr = _type.attribute("arch");
        architecture->setArch(_attr);
        _attr = _type.attribute("machine");
        architecture->setMachine(_attr);
    };
    if ( !_loader.isNull() ) {
        _attr = _loader.firstChild().toText().data();
        loader->setPath(_attr);
    };
}
void BIOS_Boot::setInitState()
{
    if ( this->isEnabled() ) architecture->setItems();
}

/* private slots */
void BIOS_Boot::changeArch(QString &_arch)
{
    arch = _arch;
}
