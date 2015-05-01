#include "_disk_item.h"

_DiskItem::_DiskItem(QWidget *parent) : QWidget(parent)
{
    usage = new QCheckBox(this);
    name = new QLineEdit(this);
    name->setReadOnly(true);
    source = new QLineEdit(this);
    source->setPlaceholderText("Source file path");
    driver = new QComboBox(this);
    snapshotType = new QComboBox(this);
    paramLayout = new QHBoxLayout(this);
    paramLayout->addWidget(driver);
    paramLayout->addWidget(snapshotType);
    params = new QWidget(this);
    params->setLayout(paramLayout);
    baseLayout = new QVBoxLayout(this);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setDisabled(true);
    baseLayout->addWidget(name);
    baseLayout->addWidget(source);
    baseLayout->addWidget(params);
    baseLayout->addStretch(-1);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setEnabled(bool)));
    connect(snapshotType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(snapshotTypeChanged(QString)));
    connect(driver, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(driverTypeChanged(QString)));
    driver->addItems(QStringList()<<"custom"<<"qcow2"<<"qcow"<<"raw"<<"qed"
                     <<"bochs"<<"vmdk"<<"dmg"<<"iso");
    snapshotType->addItems(QStringList()<<"no"<<"internal"<<"external");
}

_DiskItem::~_DiskItem()
{

}

/* public slots */
void _DiskItem::setDiskName(QString &_name)
{
    name->setText(_name);
}
void _DiskItem::setDriverType(QString &_type)
{
    int idx = driver->findText(_type);
    if ( idx<0 ) idx = 0;
    driver->setCurrentIndex(idx);
    if ( idx==0 && !_type.isEmpty() ) driver->setEditText(_type);
}
QString _DiskItem::getName() const
{
    return name->text();
}
QString _DiskItem::getSource() const
{
    return source->text();
}
QString _DiskItem::getDriverType() const
{
    return driver->currentText();
}
QString _DiskItem::getSnapshotType() const
{
    return snapshotType->currentText();
}
bool _DiskItem::isUsed() const
{
    return usage->isChecked();
}

/* private  slots */
void _DiskItem::snapshotTypeChanged(QString _text)
{
    driver->setEnabled( _text=="external" );
}
void _DiskItem::driverTypeChanged(QString _text)
{
    if ( _text=="custom" ) {
        driver->setEditable(true);
        driver->clearEditText();
    } else {
        driver->setEditable(false);
    };
}
