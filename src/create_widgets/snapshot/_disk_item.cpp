#include "_disk_item.h"

_DiskItem::_DiskItem(QWidget *parent) :
    QWidget(parent)
{
    usage = new QCheckBox(this);
    name = new QLineEdit(this);
    name->setReadOnly(true);
    source = new QLineEdit(this);
    source->setPlaceholderText(tr("Source file path"));
    source->setToolTip(
tr("The absolute path to the disk snapshot image/file"));
    driver = new QComboBox(this);
    driver->setToolTip(
tr("The driver type of the new file\n\
created by the external snapshot\n(optional)"));
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
    driver->addItems(QStringList()
                     <<tr("custom")<<"qcow2"<<"qcow"<<"raw"<<"qed"
                     <<"bochs"<<"vmdk"<<"dmg"<<"iso");
    snapshotType->addItems(
                QStringList()<<tr("internal")<<tr("external"));
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
void _DiskItem::setSnapshotType(QString &_type)
{
    int idx = snapshotType->findText(_type);
    if ( idx<0 ) idx = 0;
    snapshotType->setCurrentIndex(idx);
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
    driver->setEnabled( _text==tr("external") );
    source->setEnabled( _text==tr("external") );
}
void _DiskItem::driverTypeChanged(QString _text)
{
    if ( _text==tr("custom") ) {
        driver->setEditable(true);
        driver->clearEditText();
    } else {
        driver->setEditable(false);
    };
}
