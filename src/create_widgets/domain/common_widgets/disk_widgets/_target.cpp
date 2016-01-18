#include "_target.h"

#define BUS_TYPES QStringList()\
    <<"Default"<<"IDE"<<"SATA"\
    <<"SCSI"<<"USB"<<"SD"\
    <<"VirtIO"<<"XEN"

_Target::_Target(QWidget *parent) :
    QWidget(parent)
{
    busLabel = new QLabel("Bus:", this);
    bus = new QComboBox(this);
    bus->addItems(BUS_TYPES);
    bus->setItemIcon(0, QIcon::fromTheme(""));
    bus->setItemIcon(1, QIcon::fromTheme("drive-ide"));
    bus->setItemIcon(2, QIcon::fromTheme("drive-sata"));
    bus->setItemIcon(3, QIcon::fromTheme("drive-scsi"));
    bus->setItemIcon(4, QIcon::fromTheme("drive-usb"));
    bus->setItemIcon(5, QIcon::fromTheme("drive-sd"));
    bus->setItemIcon(6, QIcon::fromTheme("drive-virtual-io"));
    bus->setItemIcon(7, QIcon::fromTheme("xen"));
    devNameLabel = new QLabel("Logical Device name:", this);
    devName = new QLineEdit(this);
    devName->setPlaceholderText("sda");
    trayLabel = new QCheckBox("Tray status:", this);
    trayLabel->setLayoutDirection(Qt::RightToLeft);
    tray = new QComboBox(this);
    tray->addItems(QStringList()<<"closed"<<"open");
    tray->setEnabled(false);
    removableLabel = new QCheckBox("Removable\nstatus:", this);
    removableLabel->setLayoutDirection(Qt::RightToLeft);
    removable = new QComboBox(this);
    removable->addItems(QStringList()<<"off"<<"on");
    removable->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(busLabel, 0, 0);
    commonLayout->addWidget(bus, 0, 1);
    commonLayout->addWidget(devNameLabel, 1, 0);
    commonLayout->addWidget(devName, 1, 1);
    commonLayout->addWidget(trayLabel, 2, 0);
    commonLayout->addWidget(tray, 2, 1);
    commonLayout->addWidget(removableLabel, 3, 0);
    commonLayout->addWidget(removable, 3, 1);
    setLayout(commonLayout);
    connect(trayLabel, SIGNAL(toggled(bool)),
            tray, SLOT(setEnabled(bool)));
    connect(removableLabel, SIGNAL(toggled(bool)),
            removable, SLOT(setEnabled(bool)));
    // dataChanged connections
    connect(bus, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(devName, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(trayLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(tray, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(removableLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(removable, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
TargetAttrs _Target::getTargetAttrs() const
{
    TargetAttrs attrs;
    attrs.insert("dev", devName->text());
    if ( bus->currentText().toLower()!="default" )
        attrs.insert("bus", bus->currentText().toLower());
    if ( tray->isEnabled() )
        attrs.insert("tray", tray->currentText());
    if ( removable->isEnabled() )
        attrs.insert("removable", removable->currentText());
    return attrs;
}
