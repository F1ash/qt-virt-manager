#include "memory.h"

Memory::Memory(
        QWidget *parent, QString _caps, QString _xmlDesc) :
    _Tab(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("Memory");
    readCapabilities();
    hostMemory = new QLabel(
                QString(tr("Host Memory (%1): %2"))
                .arg(memUnit).arg(memValue), this);
    maxMemLabel = new QLabel(tr("Maximum Memory:"), this);
    maxMemValue = new QSpinBox(this);
    maxMemValue->setRange(0, memValue.toULongLong());
    maxMemValue->setSuffix(" (MiB)");
    connect(maxMemValue, SIGNAL(valueChanged(int)),
            this, SLOT(changeCurrentMemValue(int)));
    currMemLabel = new QLabel(tr("Current Memory:"), this);
    currMemValue = new QSpinBox(this);
    currMemValue->setRange(0, memValue.toULongLong());
    currMemValue->setSuffix(" (MiB)");
    connect(currMemValue, SIGNAL(valueChanged(int)),
            this, SLOT(changeMaximumMemValue(int)));
    memLayout = new QGridLayout();
    memLayout->addWidget(maxMemLabel, 0, 0);
    memLayout->addWidget(maxMemValue, 0, 1);
    memLayout->addWidget(currMemLabel, 1, 0);
    memLayout->addWidget(currMemValue, 1, 1);
    memWdg = new QWidget(this);
    memWdg->setLayout(memLayout);
    enableMemBacking = new QCheckBox(
                tr("Enable Memory Backing"), this);
    hugepages = new QCheckBox(tr("Hugepages"));
    hugePagesList = new HugePages(this);
    hugePagesList->setVisible(false);
    nosharepages = new QCheckBox(tr("No share pages"));
    locked = new QCheckBox(tr("Locked"));
    memBackingLayout = new QGridLayout();
    memBackingLayout->addWidget(hugepages, 0, 1);
    memBackingLayout->addWidget(nosharepages, 1, 1);
    memBackingLayout->addWidget(locked, 2, 1);
    memBackingLayout->addWidget(hugePagesList, 0, 3, 3, 3);
    memBackingWdg = new QWidget(this);
    memBackingWdg->setLayout(memBackingLayout);
    memBackingWdg->setVisible(false);
    enableMemTune = new QCheckBox(
                tr("Enable Memory Tuning"), this);
    hardLabel = new QLabel(tr("Hard limit"), this);
    hard_limit = new QSpinBox(this);
    hard_limit->setRange(0, memValue.toULongLong());
    hard_limit->setSuffix(" (MiB)");
    softLabel = new QLabel(tr("Soft limit"), this);
    soft_limit = new QSpinBox(this);
    soft_limit->setRange(0, memValue.toULongLong());
    soft_limit->setSuffix(" (MiB)");
    swapLabel = new QLabel(tr("Swap hard limit"), this);
    swap_hard_limit = new QSpinBox(this);
    swap_hard_limit->setRange(0, memValue.toULongLong());
    swap_hard_limit->setSuffix(" (MiB)");
    guaranteeLabel = new QLabel(tr("Min guarantee"), this);
    min_guarantee = new QSpinBox(this);
    min_guarantee->setRange(0, memValue.toULongLong());
    min_guarantee->setSuffix(" (MiB)");
    memTuneLayout = new QGridLayout();
    memTuneLayout->addWidget(hardLabel, 0, 0);
    memTuneLayout->addWidget(hard_limit, 0, 2);
    memTuneLayout->addWidget(softLabel, 1, 0);
    memTuneLayout->addWidget(soft_limit, 1, 2);
    memTuneLayout->addWidget(swapLabel, 2, 0);
    memTuneLayout->addWidget(swap_hard_limit, 2, 2);
    memTuneLayout->addWidget(guaranteeLabel, 3, 0);
    memTuneLayout->addWidget(min_guarantee, 3, 2);
    memTuneWdg = new QWidget(this);
    memTuneWdg->setLayout(memTuneLayout);
    memTuneWdg->setVisible(false);

    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(hostMemory);
    scrolledLayout->addWidget(memWdg);
    scrolledLayout->addWidget(enableMemBacking);
    scrolledLayout->addWidget(memBackingWdg);
    scrolledLayout->addWidget(enableMemTune);
    scrolledLayout->addWidget(memTuneWdg);
    scrolledLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrolledLayout);
    restorePanel = new RestorePanel(this);
    commonWdg = new QScrollArea(this);
    commonWdg->setWidget(scrolled);
    commonWdg->setWidgetResizable(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    commonLayout->addWidget(commonWdg);
    setLayout(commonLayout);
    connect(enableMemBacking, SIGNAL(toggled(bool)),
            memBackingWdg, SLOT(setVisible(bool)));
    connect(hugepages, SIGNAL(toggled(bool)),
            hugePagesList, SLOT(setVisible(bool)));
    connect(enableMemTune, SIGNAL(toggled(bool)),
            memTuneWdg, SLOT(setVisible(bool)));
    readXMLDesciption();
    // dataChanged connections
    connect(maxMemValue, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(currMemValue, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(enableMemBacking, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(hugepages, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(hugePagesList, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(nosharepages, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(locked, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(enableMemTune, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(hard_limit, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(soft_limit, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(swap_hard_limit, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(min_guarantee, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveData()));
}

/* public slots */
QDomDocument Memory::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _memory, _currMemory, _memTune, _memBacking, _el, _data;
    _data = doc.createElement("data");
    _memory= doc.createElement("memory");
    data = doc.createTextNode(
                QString("%1").arg(maxMemValue->value()));
    _memory.appendChild(data);
    _memory.setAttribute("unit", "MiB");
    _data.appendChild(_memory);
    _currMemory = doc.createElement("currentMemory");
    data = doc.createTextNode(
                QString("%1").arg(currMemValue->value()));
    _currMemory.appendChild(data);
    _currMemory.setAttribute("unit", "MiB");
    _data.appendChild(_currMemory);

    if ( enableMemBacking->isChecked() ) {
        _memBacking = doc.createElement("memoryBacking");
        _data.appendChild(_memBacking);
        QStringList pages = hugePagesList->getPagesList();
        if ( hugepages->isChecked() && !pages.isEmpty() ) {
            _el = doc.createElement("hugepages");
            _memBacking.appendChild(_el);
            foreach (QString page, pages) {
                QStringList _split = page.split(":");
                if ( _split.count()>2 ) {
                    QDomElement _page = doc.createElement("page");
                    _el.appendChild(_page);
                    _page.setAttribute("size", _split.at(0));
                    _page.setAttribute("unit", _split.at(1).at(0));
                    _page.setAttribute("nodeset", _split.at(2));
                };
            };
        };
        if ( nosharepages->isChecked() ) {
            _el = doc.createElement("nosharepages");
            _memBacking.appendChild(_el);
        };
        if ( locked->isChecked() ) {
            _el = doc.createElement("locked");
            _memBacking.appendChild(_el);
        };
    };

    if ( enableMemTune->isChecked() ) {
        _memTune = doc.createElement("memtune");
        _data.appendChild(_memTune);
        if ( hard_limit->value() ) {
            _el = doc.createElement("hard_limit");
            data = doc.createTextNode(
                        QString("%1").arg(hard_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "MiB");
            _memTune.appendChild(_el);
        };
        if ( soft_limit->value() ) {
            _el = doc.createElement("soft_limit");
            data = doc.createTextNode(
                        QString("%1").arg(soft_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "MiB");
            _memTune.appendChild(_el);
        };
        if ( swap_hard_limit->value() ) {
            _el = doc.createElement("swap_hard_limit");
            data = doc.createTextNode(
                        QString("%1")
                        .arg(swap_hard_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "MiB");
            _memTune.appendChild(_el);
        };
        if ( min_guarantee->value() ) {
            _el = doc.createElement("min_guarantee");
            data = doc.createTextNode(
                        QString("%1")
                        .arg(min_guarantee->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "MiB");
            _memTune.appendChild(_el);
        };
    };

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}

/* private slots */
void Memory::readCapabilities()
{
    QDomDocument doc;
    doc.setContent(capabilities);
    QString _memValue, _memUnit;
    _memValue = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("topology").
               firstChildElement("cells").
               firstChildElement("cell").
               firstChildElement("memory").
               firstChild().toText().data();
    _memUnit = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("topology").
               firstChildElement("cells").
               firstChildElement("cell").
               firstChildElement("memory").
               attribute("unit", "???");
    memUnit.append("MiB");
    memValue.append(
                QString::number(
                    convertNiBtoMiB(
                        _memValue.toULongLong(),
                        _memUnit)));
}
void Memory::changeCurrentMemValue(int i)
{
    if ( currMemValue->value()>i ) currMemValue->setValue(i);
}
void Memory::changeMaximumMemValue(int i)
{
    if ( maxMemValue->value()<i ) maxMemValue->setValue(i);
}
void Memory::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void Memory::readXMLDesciption(const QString &_xmlDesc)
{
    quint64 _value;
    QString _unit;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _domain = doc.firstChildElement("domain");
    _value = _domain
            .firstChildElement("memory")
            .firstChild().toText().data()
            .toULongLong();
    _unit = _domain
            .firstChildElement("memory")
            .attribute("unit");
    maxMemValue->setValue(
                convertNiBtoMiB(_value, _unit));
    _value = _domain
            .firstChildElement("currentMemory")
            .firstChild().toText().data()
            .toULongLong();
    _unit = _domain
            .firstChildElement("currentMemory")
            .attribute("unit");
    currMemValue->setValue(
                convertNiBtoMiB(_value, _unit));
    QDomElement _memoryBacking = _domain
            .firstChildElement("memoryBacking");
    enableMemBacking->setChecked( !_memoryBacking.isNull() );
    if ( !_memoryBacking.isNull() ) {
        hugepages->setChecked(
                    !_memoryBacking
                    .firstChildElement("hugepages").isNull() );
        if ( !_memoryBacking
             .firstChildElement("hugepages").isNull() ) {
            QDomElement _page = _memoryBacking
                    .firstChildElement("hugepages")
                    .firstChildElement("page");
            while ( !_page.isNull() ) {
                QString p;
                p.append(_page.attribute("size"));
                p.append(":");
                p.append(_page.attribute("unit", "M"));
                p.append("iB");
                p.append(":");
                p.append(_page.attribute("nodeset"));
                hugePagesList->setPageList(p);
                _page = _page.nextSiblingElement("page");
            };
        };
        nosharepages->setChecked( !_memoryBacking
             .firstChildElement("nosharepages").isNull() );
        locked->setChecked( !_memoryBacking
             .firstChildElement("locked").isNull() );
    };
    QDomElement _memTune = _domain.firstChildElement("memtune");
    enableMemTune->setChecked( !_memTune.isNull() );
    if ( !_memTune.isNull() ) {
        if ( !_memTune
             .firstChildElement("hard_limit").isNull() ) {
            _value = _memTune
                    .firstChildElement("hard_limit")
                    .firstChild().toText().data()
                    .toULongLong();
            _unit = _memTune
                    .firstChildElement("hard_limit")
                    .attribute("unit");
            hard_limit->setValue(
                        convertNiBtoMiB(_value, _unit));
        };
        if ( !_memTune
             .firstChildElement("soft_limit").isNull() ) {
            _value = _memTune
                    .firstChildElement("soft_limit")
                    .firstChild().toText().data()
                    .toULongLong();
            _unit = _memTune
                    .firstChildElement("soft_limit")
                    .attribute("unit");
            soft_limit->setValue(
                        convertNiBtoMiB(_value, _unit));
        };
        if ( !_memTune
             .firstChildElement("swap_hard_limit").isNull() ) {
            _value = _memTune
                    .firstChildElement("swap_hard_limit")
                    .firstChild().toText().data()
                    .toULongLong();
            _unit = _memTune
                    .firstChildElement("swap_hard_limit")
                    .attribute("unit");
            swap_hard_limit->setValue(
                        convertNiBtoMiB(_value, _unit));
        };
        if ( !_memTune
             .firstChildElement("min_guarantee").isNull() ) {
            _value = _memTune
                    .firstChildElement("min_guarantee")
                    .firstChild().toText().data()
                    .toULongLong();
            _unit = _memTune
                    .firstChildElement("min_guarantee")
                    .attribute("unit");
            min_guarantee->setValue(
                        convertNiBtoMiB(_value, _unit));
        };
    };
}
quint64 Memory::convertNiBtoMiB(quint64 _NiB, const QString &_unit)
{
    if ( _unit=="b" || _unit=="bytes" ) {
        quint64 _res = _NiB / 1048576;
        return (_res==0)? 1 : _res;
    } else if ( _unit=="K" || _unit=="KiB" ) {
        return _NiB / 1024;
    } else if ( _unit=="KB" ) {
        return convertNiBtoMiB(_NiB*1000, "b");
    } else if ( _unit=="M" || _unit=="MiB" ) {
        return _NiB;
    } else if ( _unit=="MB" ) {
        return _NiB*1000 / 1024;
    } else if ( _unit=="G" || _unit=="GiB" ) {
        return _NiB*1024;
    } else if ( _unit=="GB" ) {
        return _NiB*1000000/1024;
    } else if ( _unit=="T" || _unit=="TiB" ) {
        return _NiB*1048576;
    } else if ( _unit=="TB" ) {
        return _NiB*1000000000/1024;
    } else if ( _unit=="E" || _unit=="EiB" ) {
        return _NiB*1073741824;
    } else if ( _unit=="EB" ) {
        return _NiB*1000000000000/1024;
    } else return 0;
}
