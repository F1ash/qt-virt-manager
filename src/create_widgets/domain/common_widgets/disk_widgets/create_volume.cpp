#include "create_volume.h"

/*
 * http://libvirt.org/formatstorage.html
 */

CreateVolume::CreateVolume(QWidget *parent, QString _type) :
    _CreateStorage(parent), currPoolType(_type)
{
    type->addItem(currPoolType);
    suff->setVisible(true);
    allocLabel = new QComboBox(this);
    allocLabel->addItem("Allocation (KiB):", "KiB");
    allocLabel->addItem("Allocation (MiB):", "MiB");
    allocLabel->addItem("Allocation (GiB):", "GiB");
    allocLabel->addItem("Allocation (TiB):", "TiB");
    allocLabel->addItem("Allocation (EiB):", "EiB");
    capLabel = new QComboBox(this);
    capLabel->addItem("Capacity (KiB):", "KiB");
    capLabel->addItem("Capacity (MiB):", "MiB");
    capLabel->addItem("Capacity (GiB):", "GiB");
    capLabel->addItem("Capacity (TiB):", "TiB");
    capLabel->addItem("Capacity (EiB):", "EiB");
    allocation = new QSpinBox(this);
    allocation->setRange(0, 1024);
    capacity = new QSpinBox(this);
    capacity->setRange(0, 1024);
    sizeLayout = new QGridLayout();
    sizeLayout->addWidget(allocLabel, 0, 0);
    sizeLayout->addWidget(allocation, 0, 1);
    sizeLayout->addWidget(capLabel, 1, 0);
    sizeLayout->addWidget(capacity, 1, 1);
    sizeWdg = new QWidget(this);
    sizeWdg->setLayout(sizeLayout);

    //source = new _Storage_Source(this);
    target = new _Storage_Target(this, currPoolType);
    target->formatWdg->setVisible(true);

    commonLayout->insertWidget(commonLayout->count()-1, sizeWdg);
    //commonLayout->insertWidget(commonLayout->count()-1, source);
    commonLayout->insertWidget(commonLayout->count()-1, target, -1);
    allocLabel->setCurrentIndex(1);
    capLabel->setCurrentIndex(1);
}

/* public slots */
QString CreateVolume::getStorageXMLDescFileName() const
{
    QDomDocument doc;
    QDomText _text;
    QDomElement _volume, _name, _target,
            _source, _allocation, _capacity,
            _format, _permissions, _owner,
            _group, _mode, _label;
    _volume = doc.createElement("volume");
    _name = doc.createElement("name");
    _text = doc.createTextNode(QString("%1.img").arg(stName->text()));
    _name.appendChild(_text);
    _volume.appendChild(_name);
    _allocation = doc.createElement("allocation");
    _text = doc.createTextNode(allocation->text());
    _allocation.setAttribute("unit", allocLabel->itemData(
                                 allocLabel->currentIndex(),
                                 Qt::UserRole).toString());
    _allocation.appendChild(_text);
    _volume.appendChild(_allocation);
    _capacity = doc.createElement("capacity");
    _text = doc.createTextNode(capacity->text());
    _capacity.setAttribute("unit", capLabel->itemData(
                               capLabel->currentIndex(),
                               Qt::UserRole).toString());
    _capacity.appendChild(_text);
    _volume.appendChild(_capacity);
    if ( target->usePerm->isChecked() ||
         target->format->currentText()!="default" ) {
        _target = doc.createElement("target");
        if ( target->format->currentText()!="default" ) {
            _format = doc.createElement("format");
            _format.setAttribute("type", target->format->currentText()
                                 .toLower());
            _target.appendChild(_format);
        };
        if ( target->usePerm->isChecked() ) {
            _permissions = doc.createElement("permissions");
            _owner = doc.createElement("owner");
            _text = doc.createTextNode(target->owner->text());
            _owner.appendChild(_text);
            _permissions.appendChild(_owner);
            _group = doc.createElement("group");
            _text = doc.createTextNode(target->group->text());
            _group.appendChild(_text);
            _permissions.appendChild(_group);
            _mode = doc.createElement("mode");
            _text = doc.createTextNode(target->mode->text());
            _mode.appendChild(_text);
            _permissions.appendChild(_mode);
            _label = doc.createElement("label");
            _text = doc.createTextNode(target->label->text());
            _label.appendChild(_text);
            _permissions.appendChild(_label);
            _target.appendChild(_permissions);
        };
        _volume.appendChild(_target);
    };
    doc.appendChild(_volume);
    //qDebug()<<doc.toString();

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return xml->fileName();
}
