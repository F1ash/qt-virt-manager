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
void _Disks::setDisksData(QDomElement &_disk, bool external)
{
    QString _name, _driver, _type;
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
        _type = QString(external? "external" : "internal");
        wdg->setSnapshotType(_type);
        if ( !_disk.firstChildElement("driver").isNull() ) {
            _driver = _disk.firstChildElement("driver").attribute("type");
        };
        if ( !_driver.isEmpty() ) {
            wdg->setDriverType(_driver);
        };
    };
}
QDomDocument _Disks::getElements(bool all) const
{
    QDomDocument doc;
    QDomElement _disks = doc.createElement("disks");
    doc.appendChild(_disks);
    for (int i=0; i<disksLayout->count(); i++) {
        _DiskItem *wdg = static_cast<_DiskItem*>(
                    disksLayout->itemAt(i)->widget());
        if ( wdg==NULL ) continue;
        QDomElement _disk = doc.createElement("disk");
        if ( wdg->isUsed() || all ) {
            _disk.setAttribute("name", wdg->getName());
            _disk.setAttribute("snapshot", wdg->getSnapshotType());
            if ( wdg->getSnapshotType()=="external" ) {
                QString source, driver;
                source = wdg->getSource();
                if (!source.isEmpty()) {
                    QDomElement _source = doc.createElement("source");
                    _source.setAttribute("file", source);
                    _disk.appendChild(_source);
                };
                driver = wdg->getDriverType();
                if (!driver.isEmpty()) {
                    QDomElement _driver = doc.createElement("driver");
                    _driver.setAttribute("type", driver);
                    _disk.appendChild(_driver);
                };
            };
        } else {
            _disk.setAttribute("name", wdg->getName());
            _disk.setAttribute("snapshot", "no");
        };
        _disks.appendChild(_disk);
    };
    //qDebug()<<doc.toByteArray(4).data()<<"disks";
    return doc;
}
void _Disks::addStretch()
{
    disksLayout->addStretch(-1);
}
