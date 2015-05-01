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
    QString _name, _driver;
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
        if ( !_disk.firstChildElement("driver").isNull() ) {
            _driver = _disk.firstChildElement("driver").attribute("type");
        };
        if ( !_driver.isEmpty() ) {
            wdg->setDriverType(_driver);
        };
    };
}
QDomDocument _Disks::getElements() const
{
    QDomDocument doc;
    QDomElement _disks = doc.createElement("disks");
    doc.appendChild(_disks);
    for (uint i=0; i<disksLayout->count(); i++) {
        _DiskItem *wdg = static_cast<_DiskItem*>(
                    disksLayout->itemAt(i)->widget());
        if ( wdg!=NULL && wdg->isUsed() ) {
            QDomElement _disk = doc.createElement("disk");
            _disk.setAttribute("name", wdg->getName());
            _disk.setAttribute("snapshot", wdg->getSnapshotType());
            QDomElement _source = doc.createElement("source");
            _source.setAttribute("file", wdg->getSource());
            _disk.appendChild(_source);
            QDomElement _driver = doc.createElement("driver");
            _driver.setAttribute("type", wdg->getDriverType());
            _disk.appendChild(_driver);
            _disks.appendChild(_disk);
        };
    };
    return doc;
}
void _Disks::addStretch()
{
    disksLayout->addStretch(-1);
}
