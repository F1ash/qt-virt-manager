#include "_disk_item.h"

_DiskItem::_DiskItem(QWidget *parent) : QWidget(parent)
{
    usage = new QCheckBox(this);
    name = new QLineEdit(this);
    name->setReadOnly(true);
    source = new QLineEdit(this);
    source->setPlaceholderText("Source path");
    driver = new QComboBox(this);
    snapshotType = new QComboBox(this);
    baseLayout = new QVBoxLayout(this);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setDisabled(true);
    baseLayout->addWidget(name);
    baseLayout->addWidget(source);
    baseLayout->addWidget(driver);
    baseLayout->addWidget(snapshotType);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setEnabled(bool)));
}

_DiskItem::~_DiskItem()
{

}

/* public slots */
void _DiskItem::setDiskName(QString &_name)
{
    name->setText(_name);
}
