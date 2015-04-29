#include "_disks.h"

_Disks::_Disks(QWidget *parent) :
    QScrollArea(parent)
{
    scrolled = new QWidget(this);
    disksLayout = new QVBoxLayout(this);
    scrolled->setLayout(disksLayout);
    setWidget(scrolled);
    setWidgetResizable(true);
}

/* public slots */
void _Disks::setDisksData(QDomElement &_disk)
{
    QString _name;
    if ( !_disk.firstChildElement("target").isNull() ) {
        _name = _disk.firstChildElement("target").attribute("dev");
    } else if ( !_disk.firstChildElement("source").isNull() ) {
        _name = _disk.firstChildElement("source").attribute("file");
    };
    if ( !_name.isEmpty() ) {
        disksLayout->addWidget(new _DiskItem(this));
        _DiskItem *wdg = static_cast<_DiskItem*>(
                    disksLayout->itemAt(disksLayout->count()-1)->widget());
        wdg->setDiskName(_name);
    };
}
