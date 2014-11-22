#include "cpu_allocation.h"

CPU_Allocation::CPU_Allocation(QWidget *parent, QString _caps) :
    QWidget(parent), capabilities(_caps)
{
    QDomDocument doc;
    doc.setContent(capabilities);
    //qDebug()<<capabilities;
    QDomElement _domain = doc.
            firstChildElement("capabilities").
            firstChildElement("guest").
            firstChildElement("arch").
            firstChildElement("domain");
    if ( !_domain.isNull() ) {
        type = _domain.attribute("type").toLower();
    };
    vcpuLabel = new QLabel("VCPUs:", this);
    vcpu = new QSpinBox(this);
    cpusetLabel = new QCheckBox("CPU Set:", this);
    cpuset = new QLineEdit(this);
    cpuset->setEnabled(false);
    cpuset->setPlaceholderText("1-4,^3,6");
    currLabel = new QCheckBox("Current:", this);
    current = new QSpinBox(this);
    current->setEnabled(false);
    placementLabel = new QCheckBox("Placement:", this);
    // Since 0.9.11 (QEMU and KVM only)
    placement = new QComboBox(this);
    placement->addItems(QStringList()<<"static"<<"auto");
    placement->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(vcpuLabel, 0, 0);
    commonLayout->addWidget(vcpu, 0, 1);
    commonLayout->addWidget(currLabel, 0, 2);
    commonLayout->addWidget(current, 0, 3);
    commonLayout->addWidget(cpusetLabel, 1, 0);
    commonLayout->addWidget(cpuset, 1, 1);
    commonLayout->addWidget(placementLabel, 1, 2);
    commonLayout->addWidget(placement, 1, 3);
    setLayout(commonLayout);
    connect(vcpu, SIGNAL(valueChanged(int)),
            this, SLOT(vcpuValueChanged(int)));
    connect(cpusetLabel, SIGNAL(toggled(bool)),
            cpuset, SLOT(setEnabled(bool)));
    connect(currLabel, SIGNAL(toggled(bool)),
            current, SLOT(setEnabled(bool)));
    connect(placementLabel, SIGNAL(toggled(bool)),
            this, SLOT(placementStateChanged(bool)));
    // dataChanged connections
    connect(vcpu, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(current, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(cpuset, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(placement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(cpusetLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(currLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(placementLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
void CPU_Allocation::setPlacement(QString &_mode)
{
    int idx = placement->findText(_mode, Qt::MatchContains);
    placement->setCurrentIndex( (idx<0)? 0:idx );
}

/* private slots */
void CPU_Allocation::vcpuValueChanged(int i)
{
    current->setRange(1, i);
}
void CPU_Allocation::placementStateChanged(bool state)
{
    placement->setEnabled( state && (type=="qemu" || type=="kvm") );
}
