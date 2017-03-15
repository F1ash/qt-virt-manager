#include "_device_list.h"

_DeviceList::_DeviceList(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    setUsage(true);
    browse = new QPushButton(
                QIcon::fromTheme("edit-find"), "", this);
    name = new QLineEdit(this);
    //name->setPlaceholderText("");
    panelLayout->insertWidget(1, browse, 1);
    panelLayout->insertWidget(2, name, 20);
    connect(browse, SIGNAL(clicked()),
            this, SLOT(setBlockDevPath()));
    setOneDeviceMode(false);
}

void _DeviceList::addNewDevicePath(const QString &_devPath)
{
    name->setText(_devPath);
    addItem();
}

/* public slots */
QStringList _DeviceList::getDevicesList() const
{
    QStringList _list;
    for(int i = 0; i<list->count(); i++) {
        _list.append(list->item(i)->text());
    };
    return _list;
}
void _DeviceList::setOneDeviceMode(bool state)
{
    oneDeviceMode = state;
}

/* private slots */
void _DeviceList::addItem()
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
void _DeviceList::delItem()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
    };
    list->clearSelection();
}
void _DeviceList::setBlockDevPath()
{
    QString _dev = QFileDialog::getOpenFileName(
                this,
                "Block Device Path",
                QString("/dev"));
    if ( !_dev.isEmpty() ) name->setText(_dev);
}
