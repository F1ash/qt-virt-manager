#include "boot_devices.h"

Boot_Devices::Boot_Devices(QWidget *parent) :
    _Changed(parent)
{
    up = new QPushButton(QIcon::fromTheme("go-up"), "", this);
    down = new QPushButton(QIcon::fromTheme("go-down"), "", this);
    devices = new QListWidget(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(devices, 0, 1, 2, 1);
    commonLayout->addWidget(up, 0, 2);
    commonLayout->addWidget(down, 1, 2);
    setLayout(commonLayout);
    connect(up, SIGNAL(clicked()), this, SLOT(itemUp()));
    connect(down, SIGNAL(clicked()), this, SLOT(itemDown()));
    // dataChanged connections
    connect(devices, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(stateChanged()));
    connect(devices, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(orderChanged(QListWidgetItem*)));
    connect(devices, SIGNAL(currentRowChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
void Boot_Devices::addNewDevice(QDomElement &_el)
{
    QString _devName = _el.tagName();
    //qDebug()<<_devName;
    QString _devType = _el.attribute("type");
    _devName.append(" ");
    _devName.append(_devType);
    QString _dev;
    if ( _el.hasAttribute("device") ) {
        _dev.append(_el.attribute("device"));
        _devName.append(" ");
        _devName.append(_dev);
    };
    if ( !_el.firstChildElement("target").isNull() ) {
        _devName.append(" ");
        _devName.append(
                    _el.firstChildElement("target")
                    .attribute("dev"));
    };
    int _order = devices->count()+1;
    bool _used = !_el.firstChildElement("boot").isNull();
    if (_used)
        _order = _el
                .firstChildElement("boot")
                .attribute("order").toInt()-1;
    QDomDocument doc;
    doc.setContent(QString());
    doc.appendChild(_el);
    QString _data = doc.toDocument().toString();
    if ( !_el.firstChildElement("boot").isNull() ) {
        _el.removeChild(_el.firstChildElement("boot"));
    };
    //qDebug()<<doc.toDocument().toString();
    QListWidgetItem *_item = new QListWidgetItem();
    _item->setText(_devName);
    _item->setCheckState( (_used)? Qt::Checked:Qt::Unchecked );
    QString icon;
    if ( _dev.toLower().contains("cdrom") ) {
        icon.append("drive-optical");
    } else if ( _dev.toLower().contains("floppy") ) {
        icon.append("media-floppy");
    } else if ( _dev.toLower().contains("disk") ) {
        icon.append("drive-harddisk");
    } else if ( _dev.toLower().contains("lun") ) {
        icon.append("network-server-database");
    } else
        icon.append("network-wired");
    _item->setIcon(QIcon::fromTheme(icon));
    _item->setData(Qt::UserRole, _data);
    devices->insertItem(_order, _item);
}

/* private slots */
void Boot_Devices::itemUp()
{
    int idx = devices->row( devices->currentItem() );
    if ( idx>=0 ) {
        QListWidgetItem *_item = devices->takeItem(idx);
        devices->insertItem(idx-1, _item);
        devices->setCurrentItem(_item);
        orderChanged(_item);
    };
}
void Boot_Devices::itemDown()
{
    int idx = devices->row( devices->currentItem() );
    if ( idx>=0 ) {
        QListWidgetItem *_item = devices->takeItem(idx);
        devices->insertItem(idx+1, _item);
        devices->setCurrentItem(_item);
        orderChanged(_item);
    };
}
void Boot_Devices::orderChanged(QListWidgetItem *_item)
{
    bool sorted = false;
    int idx = devices->row( _item );
    if ( _item->checkState()==Qt::Checked ) {
        while (not sorted) {
            if ( idx-1<0 ) sorted = true;
            else {
                if ( devices->item(idx-1)->checkState()==Qt::Unchecked ) {
                    devices->setCurrentItem(_item);
                    itemUp();
                } else
                    sorted = true;
            };
            idx = devices->row( _item );
        };
    } else {
        while (not sorted) {
            if ( idx==devices->count()-1 ) sorted = true;
            else {
                if ( devices->item(idx+1)->checkState()==Qt::Checked ) {
                    devices->setCurrentItem(_item);
                    itemDown();
                } else
                    sorted = true;
            };
            idx = devices->row( _item );
        };
    };
}
