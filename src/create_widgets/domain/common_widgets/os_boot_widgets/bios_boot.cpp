#include "bios_boot.h"

BIOS_Boot::BIOS_Boot(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    architecture = new _Arch(this, capabilities);
    loaderLabel = new QLabel("Boot loader path:", this);
    loader = new Path_To_File(this);
    QString _placeHolderText = QString("/usr/lib/xen/boot/hvmloader");
    loader->setPlaceholderText(_placeHolderText);
    nvramLabel = new QLabel("NVRAM:", this);
    nvram = new Path_To_File(this);
    _placeHolderText = QString("/var/lib/libvirt/nvram/guest_VARS.fd");
    nvram->setPlaceholderText(_placeHolderText);
    templateLabel = new QLabel("NVRAM template (optional):", this);
    nvramTemplate = new Path_To_File(this);
    _placeHolderText = QString("/usr/share/OVMF/OVMF_VARS.fd");
    nvramTemplate->setPlaceholderText(_placeHolderText);
    bootMenu = new BootMenu(this);
    bootDevices = new Boot_Devices(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(architecture);
    commonLayout->addWidget(loaderLabel);
    commonLayout->addWidget(loader);
    commonLayout->addWidget(bootMenu);
    commonLayout->addWidget(bootDevices);
    commonLayout->addWidget(nvramLabel);
    commonLayout->addWidget(nvram);
    commonLayout->addWidget(templateLabel);
    commonLayout->addWidget(nvramTemplate);
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
    connect(bootMenu, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(bootDevices, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(nvram, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(nvramTemplate, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument BIOS_Boot::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _os, _type, _loader, _data,
            _boot, _bootMenu, _nvram;
    _data = doc.createElement("data");
    _os = doc.createElement("os");
    _data.appendChild(_os);

    _type = doc.createElement("type");
    _type.setAttribute("arch", arch);
    _type.setAttribute(
                "machine", architecture->getMachine());
    _os.appendChild(_type);

    if ( !loader->getPath().isEmpty() ) {
        _loader = doc.createElement("loader");
        data = doc.createTextNode(loader->getPath());
        _loader.appendChild(data);
        _os.appendChild(_loader);
    };

    /*
    if (  ) {
        _boot = doc.createElement("boot");
        _os.appendChild(_boot);
    };
    */

    _bootMenu = doc.createElement("bootmenu");
    _bootMenu.setAttribute(
                "enable",
                (bootMenu->menu->isChecked())? "yes": "no");
    _bootMenu.setAttribute(
                "timeout",
                bootMenu->timeOut->value());
    _os.appendChild(_bootMenu);

    if ( !nvram->getPath().isEmpty() && !nvramTemplate->getPath().isEmpty() ) {
        _nvram = doc.createElement("nvram");
        data = doc.createTextNode(nvram->getPath());
        _nvram.appendChild(data);
        _nvram.setAttribute("template", nvramTemplate->getPath());
        _os.appendChild(_nvram);
    };

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void BIOS_Boot::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _os, _type, _loader,
            _boot, _bootMenu, _nvram;
    _os = doc
            .firstChildElement("domain")
            .firstChildElement("os");
    _type = _os.firstChildElement("type");
    _loader = _os.firstChildElement("loader");
    _bootMenu = _os.firstChildElement("bootmenu");
    _boot = _os.firstChildElement("boot");
    _nvram = _os.firstChildElement("nvram");
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
    } else loader->clear();
    bootMenu->menu->setChecked( !_bootMenu.isNull() );
    if ( !_bootMenu.isNull() ) {
        bootMenu->menu->setChecked(
                    _bootMenu.attribute("enable")=="yes");
        bootMenu->timeOut->setValue(
                    _bootMenu.attribute("timeout").toInt());
    };
    while ( !_boot.isNull() ) {
        _boot = _boot.nextSiblingElement("boot");
    };
    if ( !_nvram.isNull() ) {
        _attr = _nvram.firstChild().toText().data();
        nvram->setPath(_attr);
        _attr = _nvram.attribute("template");
        nvramTemplate->setPath(_attr);
    } else {
        nvram->clear();
        nvramTemplate->clear();
    };
}
void BIOS_Boot::setInitState()
{
    if ( this->isEnabled() ) architecture->setItems();
}
void BIOS_Boot::searchBootableDevices(QDomDocument &_doc)
{
    //qDebug()<<_doc.toString();
    QDomNodeList _devices;
    QList<QDomElement> _bootable;
    _devices = _doc
            .firstChildElement("devices")
            .childNodes();
    /*
     * It can be tricky to configure in the desired way,
     * which is why per-device boot elements (see disks,
     * network interfaces, and USB and PCI devices) were introduced
     * and they are the preferred way providing full control over booting order.
     */
    uint count = _devices.length();
    uint j = 0;
    //search bootable devices
    for (int i=0; i<count; i++) {
        //qDebug()<<_devices.item(j).nodeName()<<i;
        if (!_devices.item(j).isNull()) {
            QDomElement _el;
            _el = _devices.item(j).toElement();
            QString _devName = _el.tagName();
            QString _devType = _el.attribute("type");
            if ( _devName=="hostdev" ) {
                if ( _devType=="pci" || _devType=="usb" ) {
                } else {
                    ++j;
                    continue;
                };
            } else if ( _devName=="disk" ) {
            } else if ( _devName=="interface" ) {
            } else {
                ++j;
                continue;
            };
            _bootable.append(_el);
        };
        ++j;
    };
    count = bootDevices->devices->count();
    QList<int> _unexist;
    // search exist bootable devices
    for (int i=0; i<count; i++) {
        QDomDocument _doc1;
        _doc1.setContent(
                    bootDevices->devices->item(i)->data(Qt::UserRole)
                    .toString());
        QDomElement _boot = _doc1.firstChildElement()
                .firstChildElement("boot");
        if ( !_boot.isNull() ) {
            _doc1.firstChildElement().removeChild(_boot);
        };
        bool exist = false;
        uint j=0;
        while ( j<_bootable.count() ) {
            QDomElement _el = _bootable.at(j);
            if ( !_el.firstChildElement("boot").isNull() ) {
                _el.removeChild(_el.firstChildElement("boot"));
            };
            QDomDocument _doc2;
            _doc2.setContent(QString());
            _doc2.appendChild(_el);
            //qDebug()<<_doc1.toString()<<_doc2.toString();
            exist = ( _doc1.toString()==_doc2.toString() );
            if (exist) {
                break;
            } else {
                ++j;
            };
        };
        if (exist) {
            //qDebug()<<_doc1.toString()<<"exist"<<i;
            _bootable.removeAt(j);
        } else {
            //qDebug()<<_doc1.toString()<<"unexist"<<i;
            _unexist.append(i);
        };
    };
    // clear unexist bootable devices
    for (int i=_unexist.count(); i>0; i--) {
        QListWidgetItem *_item = bootDevices->devices->takeItem(_unexist.at(i-1));
        delete _item;
        _item = NULL;
    };
    // append new bootable devices
    for (int i=0; i<_bootable.count(); i++) {
        QDomElement _el = _bootable.at(i);
        bootDevices->addNewDevice(_el);
        //qDebug()<<_el.attribute("type");
    };
}
BootOrderList BIOS_Boot::getBootOrderData() const
{
    BootOrderList _ret;
    int count = bootDevices->devices->count();
    for (int i=0; i<count; i++) {
        BootOrder _data;
        _data.deviceDesc = bootDevices->devices->item(i)->data(
                    Qt::UserRole).toString();
        _data.order = i+1;
        _data.usage =
                bootDevices->devices->item(i)->checkState()
                ==Qt::Checked;
        _ret.append(_data);
    };
    return _ret;
}

/* private slots */
void BIOS_Boot::changeArch(QString &_arch)
{
    arch = _arch;
}
