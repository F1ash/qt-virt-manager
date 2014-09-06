#include "_target.h"

#define BUS_TYPES QStringList()\
    <<"Default"<<"IDE"<<"SATA"\
    <<"SCSI"<<"USB"<<"SD"\
    <<"VirtIO"<<"XEN"

_Target::_Target(QWidget *parent) :
    QWidget(parent)
{
    devNameLabel = new QLabel("Logical Device name:", this);
    devName = new QLineEdit(this);
    devName->setPlaceholderText("sda");
    busLabel = new QLabel("Bus:", this);
    bus = new QComboBox(this);
    bus->addItems(BUS_TYPES);
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
    commonLayout->addWidget(devNameLabel, 0, 0);
    commonLayout->addWidget(devName, 0, 1);
    commonLayout->addWidget(busLabel, 1, 0);
    commonLayout->addWidget(bus, 1, 1);
    commonLayout->addWidget(trayLabel, 2, 0);
    commonLayout->addWidget(tray, 2, 1);
    commonLayout->addWidget(removableLabel, 3, 0);
    commonLayout->addWidget(removable, 3, 1);
    setLayout(commonLayout);
    connect(trayLabel, SIGNAL(toggled(bool)),
            tray, SLOT(setEnabled(bool)));
    connect(removableLabel, SIGNAL(toggled(bool)),
            removable, SLOT(setEnabled(bool)));
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
