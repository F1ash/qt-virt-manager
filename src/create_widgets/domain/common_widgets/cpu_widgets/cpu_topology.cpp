#include "cpu_topology.h"

CPU_Topology::CPU_Topology(QWidget *parent) :
    QWidget(parent)
{
    use = new QCheckBox("Use Topology", this);
    socketsLabel = new QLabel("Sockets", this);
    coresLabel = new QLabel("Cores", this);
    threadsLabel = new QLabel("Threads", this);
    sockets = new QSpinBox(this);
    sockets->setMinimum(1);
    cores = new QSpinBox(this);
    cores->setMinimum(1);
    threads = new QSpinBox(this);
    threads->setMinimum(1);
    infoIcon = new QLabel(this);
    infoIcon->setPixmap(
                QIcon::fromTheme("dialog-warning")
                .pixmap(this->font().SizeResolved));
    info = new QLabel("<b>Number of CPU overload</b>", this);
    changeInfoVisibility(false);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(socketsLabel, 0, 0);
    baseLayout->addWidget(sockets, 1, 0);
    baseLayout->addWidget(coresLabel, 0, 1);
    baseLayout->addWidget(cores, 1, 1);
    baseLayout->addWidget(threadsLabel, 0, 2);
    baseLayout->addWidget(threads, 1, 2);
    baseLayout->addWidget(infoIcon, 0, 3, Qt::AlignHCenter);
    baseLayout->addWidget(info, 1, 3);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setEnabled(false);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(use);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(use, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setEnabled(bool)));
    connect(use, SIGNAL(toggled(bool)),
            this, SIGNAL(useTopology(bool)));
    connect(sockets, SIGNAL(valueChanged(int)),
            this, SLOT(newValue(int)));
    connect(cores, SIGNAL(valueChanged(int)),
            this, SLOT(newValue(int)));
    connect(threads, SIGNAL(valueChanged(int)),
            this, SLOT(newValue(int)));
    // dataChanged connections
    connect(use, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(sockets, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(cores, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(threads, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
bool CPU_Topology::isUsed() const
{
    // if overload, then topology not valid
    return use->isChecked() && !overload;
}
void CPU_Topology::setUsage(bool state)
{
    use->setChecked(state);
    use->setEnabled(state);
}
void CPU_Topology::setMaxVCPU(int i)
{
    MaxVCPU = i;
    if ( use->isChecked() ) newValue(i);
}

/* private slots */
void CPU_Topology::newValue(int i)
{
    uint _value = sockets->value()*cores->value()*threads->value();
    overload = MaxVCPU<_value;
    if ( overload ) {
        //QSpinBox *wdg = static_cast<QSpinBox*>(sender());
        //wdg->setValue(i-1);
    } else {
        emit valueChanged(_value);
    };
    changeInfoVisibility(overload);
}
void CPU_Topology::changeInfoVisibility(bool state)
{
    infoIcon->setVisible(state);
    info->setVisible(state);
}
