#include "_fstype.h"

_FsType::_FsType(QWidget *parent,
                 QString _type) :
    _QWidget(parent), connType(_type)
{
    driverLabel = new QLabel("Driver:", this);
    wrPolicyLabel = new QLabel("WrPolicy:", this);
    formatLabel = new QLabel("Format:", this);
    accessModeLabel = new QLabel("AccessMode:", this);
    sourceLabel = new QPushButton("Source:", this);
    targetLabel = new QLabel("Target:", this);
    driver = new QComboBox(this);
    if ( connType=="lxc" ) {
        driver->addItems(LXC_DRIVER_TYPES);
    } else if ( connType=="qemu" ) {
        driver->addItems(QEMU_DRIVER_TYPES);
    };
    wrPolicy = new QComboBox(this);
    wrPolicy->addItem("default");
    wrPolicy->addItem("immediate");
    format = new QComboBox(this);
    formatLabel->setVisible(false);
    format->setVisible(false);
    accessMode = new QComboBox(this);
    accessMode->addItem("default");
    accessMode->addItem("passthrough");
    accessMode->addItem("mapped");
    accessMode->addItem("squash");
    driverAttrLayout = new QGridLayout();
    driverAttrLayout->addWidget(wrPolicyLabel, 0, 0);
    driverAttrLayout->addWidget(wrPolicy, 0, 1);
    driverAttrLayout->addWidget(formatLabel, 1, 0);
    driverAttrLayout->addWidget(format, 1, 1);
    driverAttrLayout->addWidget(accessModeLabel, 2, 0, Qt::AlignTop);
    driverAttrLayout->addWidget(accessMode, 2, 1, Qt::AlignTop);
    driverAttrWdg = new QWidget(this);
    driverAttrWdg->setLayout(driverAttrLayout);
    driverAttrWdg->setVisible(false);
    source = new QLineEdit(this);
    target = new QLineEdit(this);
    readOnly = new QCheckBox("Export filesystem readonly", this);

    commonLayout = new QGridLayout();
    commonLayout->addWidget(driverLabel, 0, 0);
    commonLayout->addWidget(driver, 0, 1);
    commonLayout->addWidget(driverAttrWdg, 1, 0, 2, 2);
    commonLayout->addWidget(sourceLabel, 4, 0);
    commonLayout->addWidget(source, 4, 1);
    commonLayout->addWidget(targetLabel, 5, 0, Qt::AlignCenter);
    commonLayout->addWidget(target, 5, 1, Qt::AlignTop);
    commonLayout->addWidget(readOnly, 6, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(driver, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(driverTypeChanged(QString)));
}

/* virtual */
QDomDocument _FsType::getDataDocument() const
{
    return QDomDocument();
}

/* private slots */
void _FsType::driverTypeChanged(QString _type)
{
    format->clear();
    driverAttrWdg->setVisible( _type!="default" );
    /*
     * Currently this only works with type='mount' for the QEMU/KVM driver.
     */
    accessModeLabel->setVisible( connType=="qemu" );
    accessMode->setVisible( connType=="qemu" );
    /*
     * LXC supports a type of "loop", with a format of "raw" or "nbd" with any format.
     * QEMU supports a type of "path" or "handle", but no formats.
     */
    formatLabel->setVisible( connType=="lxc" );
    format->setVisible( connType=="lxc" );
    if ( _type=="loop" ) {
        format->addItem("raw");
    } else if ( _type=="nbd" ) {
        format->addItems(FORMAT_TYPES);
    }
}
