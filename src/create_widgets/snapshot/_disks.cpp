#include "_disks.h"

_Disks::_Disks(QWidget *parent) :
    QScrollArea(parent)
{
    scrolled = new QWidget(this);
    disksLayout = new QVBoxLayout(this);
    scrolled->setLayout(disksLayout);
}

/* public slots */
void _Disks::setDisksData(QDomElement &_disk)
{
    qDebug()<<_disk.attribute("type")<<_disk.attribute("device");
    QWidget *wdg = new _DiskItem(this);
    disksLayout->addWidget(wdg);
}
