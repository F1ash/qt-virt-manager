#include "_block_devices.h"

_BlockDevices::_BlockDevices(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    setUsage(true);
    browse = new QPushButton(
                QIcon::fromTheme("edit-find"), "", this);
    name = new QLineEdit(this);
    name->setPlaceholderText("Block device name");
    panelLayout->insertWidget(1, browse, 1);
    panelLayout->insertWidget(2, name, 20);
    connect(browse, SIGNAL(clicked()),
            this, SLOT(setBlockDevPath()));
    setOneDeviceMode(false);
}

void _BlockDevices::addNewDevicePath(const QString &_devPath)
{
    name->setText(_devPath);
    addItem();
}

/* public slots */
QStringList _BlockDevices::getDevicesList() const
{
    QStringList _list;
    for(int i = 0; i<list->count(); i++) {
        _list.append(list->item(i)->text());
    };
    return _list;
}
void _BlockDevices::setOneDeviceMode(bool state)
{
    oneDeviceMode = state;
}

/* private slots */
void _BlockDevices::addItem()
{
    if ( !name->text().isEmpty() ) {
        QString _dev = name->text();
        if ( list->findItems(_dev, Qt::MatchExactly).isEmpty() ) {
            if ( oneDeviceMode ) list->clear();
            list->addItem(_dev);
        };
        name->clear();
    };
}
void _BlockDevices::delItem()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
    };
    list->clearSelection();
}
void _BlockDevices::setBlockDevPath()
{
    QString _dev = QFileDialog::getOpenFileName(
                this,
                "Block Device Path",
                QString("/dev"));
    if ( !_dev.isEmpty() ) name->setText(_dev);
}
