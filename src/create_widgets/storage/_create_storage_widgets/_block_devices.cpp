#include "_block_devices.h"

_BlockDevices::_BlockDevices(QWidget *parent) :
    QWidget(parent)
{
    devices = new QListWidget(this);
    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    browse = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    name = new QLineEdit(this);
    name->setPlaceholderText("Block device name");
    panelLayout = new QHBoxLayout(this);
    panel = new QWidget(this);
    panelLayout->addWidget(add, 1);
    panelLayout->addWidget(browse, 1);
    panelLayout->addWidget(name, 20);
    panelLayout->addWidget(del, 1);
    panel->setLayout(panelLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devices);
    commonLayout->addWidget(panel);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(add, SIGNAL(clicked()),
            this, SLOT(addDevice()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delDevice()));
    connect(browse, SIGNAL(clicked()),
            this, SLOT(setBlockDevPath()));
    setOneDeviceMode(false);
}

/* public slots */
QStringList _BlockDevices::getDevicesList() const
{
    QStringList _list;
    for(int i = 0; i<devices->count(); i++) {
        _list.append(devices->item(i)->text());
    };
    return _list;
}
void _BlockDevices::setOneDeviceMode(bool state)
{
    oneDeviceMode = state;
}

/* private slots */
void _BlockDevices::addDevice()
{
    if ( !name->text().isEmpty() ) {
        QString _dev = name->text();
        if ( devices->findItems(_dev, Qt::MatchExactly).isEmpty() ) {
            if ( oneDeviceMode ) devices->clear();
            devices->addItem(_dev);
        };
        name->clear();
    };
}
void _BlockDevices::delDevice()
{
    QList<QListWidgetItem*> l = devices->selectedItems();
    if ( !l.isEmpty() ) {
        devices->takeItem(devices->row(l.at(0)));
    };
    devices->clearSelection();
}
void _BlockDevices::setBlockDevPath()
{
    QString _dev = QFileDialog::getOpenFileName(
                this,
                "Block Device Path",
                QString("/dev"));
    if ( !_dev.isEmpty() ) name->setText(_dev);
}
