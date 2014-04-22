#include "memory.h"

Memory::Memory(QWidget *parent, QString arg1, QString arg2) :
    _QWidget(parent), memUnit(arg1), memValue(arg2)
{
    setObjectName("Memory");
    hostMemory = new QLabel(QString("Host Memory (%1): %2").arg(memUnit).arg(memValue), this);
    maxMemLabel = new QLabel("Maximum Memory:", this);
    maxMemValue = new QSpinBox(this);
    maxMemValue->setRange(0, memValue.toInt());
    connect(maxMemValue, SIGNAL(valueChanged(int)), this, SLOT(changeCurrentMemValue(int)));
    currMemLabel = new QLabel("Current Memeory:", this);
    currMemValue = new QSpinBox(this);
    currMemValue->setRange(0, memValue.toInt());
    connect(currMemValue, SIGNAL(valueChanged(int)), this, SLOT(changeMaximumMemValue(int)));
    memLayout = new QGridLayout();
    memLayout->addWidget(maxMemLabel, 0, 0);
    memLayout->addWidget(maxMemValue, 0, 1);
    memLayout->addWidget(currMemLabel, 1, 0);
    memLayout->addWidget(currMemValue, 1, 1);
    memWdg = new QWidget(this);
    memWdg->setLayout(memLayout);
    enableMemBacking = new QCheckBox("Enable Memory Backing", this);
    hugepages = new QCheckBox("Hugepages");
    nosharepages = new QCheckBox("No share pages");
    locked = new QCheckBox("Locked");
    memBackingLayout = new QGridLayout();
    memBackingLayout->addWidget(hugepages, 0, 1);
    memBackingLayout->addWidget(nosharepages, 1, 1);
    memBackingLayout->addWidget(locked, 2, 1);
    memBackingWdg = new QWidget(this);
    memBackingWdg->setLayout(memBackingLayout);
    memBackingWdg->setEnabled(false);
    enableMemTune = new QCheckBox("Enable Memory Tuning", this);
    hardLabel = new QLabel("Hard limit (KiB)", this);
    hard_limit = new QSpinBox(this);
    softLabel = new QLabel("Soft limit (KiB)", this);
    soft_limit = new QSpinBox(this);
    swapLabel = new QLabel("Swap hard limit (KiB)", this);
    swap_hard_limit = new QSpinBox(this);
    guaranteeLabel = new QLabel("Min guarantee (KiB)", this);
    min_guarantee = new QSpinBox(this);
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
    memTuneWdg->setEnabled(false);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(hostMemory);
    commonLayout->addWidget(memWdg);
    commonLayout->addWidget(enableMemBacking);
    commonLayout->addWidget(memBackingWdg);
    commonLayout->addWidget(enableMemTune);
    commonLayout->addWidget(memTuneWdg);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(enableMemBacking, SIGNAL(toggled(bool)), memBackingWdg, SLOT(setEnabled(bool)));
    connect(enableMemTune, SIGNAL(toggled(bool)), memTuneWdg, SLOT(setEnabled(bool)));
}
Memory::~Memory()
{
    disconnect(enableMemBacking, SIGNAL(toggled(bool)), memBackingWdg, SLOT(setEnabled(bool)));
    disconnect(enableMemTune, SIGNAL(toggled(bool)), memTuneWdg, SLOT(setEnabled(bool)));
    delete hostMemory;
    hostMemory = 0;
    delete maxMemLabel;
    maxMemLabel = 0;
    delete currMemLabel;
    currMemLabel = 0;
    delete maxMemValue;
    maxMemValue = 0;
    delete currMemValue;
    currMemValue = 0;
    delete memLayout;
    memLayout = 0;
    delete memWdg;
    memWdg = 0;
    delete enableMemBacking;
    enableMemBacking = 0;
    delete hugepages;
    hugepages = 0;
    delete nosharepages;
    nosharepages = 0;
    delete locked;
    locked = 0;
    delete memBackingLayout;
    memBackingLayout = 0;
    delete memBackingWdg;
    memBackingWdg = 0;
    delete enableMemTune;
    enableMemTune = 0;
    delete hardLabel;
    hardLabel = 0;
    delete hard_limit;
    hard_limit = 0;
    delete softLabel;
    softLabel = 0;
    delete soft_limit;
    soft_limit = 0;
    delete swapLabel;
    swapLabel = 0;
    delete swap_hard_limit;
    swap_hard_limit = 0;
    delete guaranteeLabel;
    guaranteeLabel = 0;
    delete min_guarantee;
    min_guarantee = 0;
    delete memTuneLayout;
    memTuneLayout = 0;
    delete memTuneWdg;
    memTuneWdg = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomNodeList Memory::getNodeList() const
{
    QDomText data;
    QDomDocument doc = QDomDocument();
    QDomElement _memory, _currMemory, _memTune, _memBacking, _el;
    _memory= doc.createElement("memory");
    data = doc.createTextNode(QString("%1").arg(maxMemValue->value()));
    _memory.appendChild(data);
    _memory.setAttribute("unit", memUnit);
    doc.appendChild(_memory);
    _currMemory= doc.createElement("currentMemory");
    data = doc.createTextNode(QString("%1").arg(currMemValue->value()));
    _currMemory.appendChild(data);
    _currMemory.setAttribute("unit", memUnit);
    doc.appendChild(_currMemory);

    if ( enableMemBacking->isChecked() ) {
        _memBacking= doc.createElement("memoryBacking");
        doc.appendChild(_memBacking);
        if ( hugepages->isChecked() ) {
            _el = doc.createElement("hugepages");
            _memBacking.appendChild(_el);
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
        _memTune= doc.createElement("memtune");
        doc.appendChild(_memTune);
        if ( hard_limit->value() ) {
            _el = doc.createElement("hard_limit");
            data = doc.createTextNode(QString("%1").arg(hard_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "KiB");
            _memTune.appendChild(_el);
        };
        if ( soft_limit->value() ) {
            _el = doc.createElement("soft_limit");
            data = doc.createTextNode(QString("%1").arg(soft_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "KiB");
            _memTune.appendChild(_el);
        };
        if ( swap_hard_limit->value() ) {
            _el = doc.createElement("swap_hard_limit");
            data = doc.createTextNode(QString("%1").arg(swap_hard_limit->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "KiB");
            _memTune.appendChild(_el);
        };
        if ( min_guarantee->value() ) {
            _el = doc.createElement("min_guarantee");
            data = doc.createTextNode(QString("%1").arg(min_guarantee->value()));
            _el.appendChild(data);
            _el.setAttribute("unit", "KiB");
            _memTune.appendChild(_el);
        };
    };

    //qDebug()<<doc.toString();
    return doc.childNodes();
}

/* private slots */
void Memory::changeCurrentMemValue(int i)
{
    if ( currMemValue->value()>i ) currMemValue->setValue(i);
}
void Memory::changeMaximumMemValue(int i)
{
    if ( maxMemValue->value()<i ) maxMemValue->setValue(i);
}
