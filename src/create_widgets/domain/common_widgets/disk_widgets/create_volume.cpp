#include "create_volume.h"

/*
 * http://libvirt.org/formatstorage.html
 */

#define DIR_VOLUME_TYPES QStringList()\
    <<"raw"<<"bochs"<<"cloop"\
    <<"cow"<<"dmg"<<"iso"<<"qcow"<<"qcow2"\
    <<"qed"<<"vmdk"<<"vpc"

#define DISK_VOLUME_TYPES QStringList()\
    <<"none"<<"linux"<<"fat16"<<"fat32"\
    <<"linux-swap"<<"linux-lvm"\
    <<"linux-raid"<<"extended"

#define CUSTOMIZABLE_POOL_TYPES QStringList()\
    <<"dir"<<"fs"<<"netfs"<<"gluster"<<"disk"

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
    formatLabel = new QLabel("Format:", this);
    format = new QComboBox(this);
    format->addItem("default");
    QStringList l;
    l.append(CUSTOMIZABLE_POOL_TYPES);
    if ( l.contains(currPoolType) ) {
        if ( currPoolType=="disk" )
            format->addItems(DISK_VOLUME_TYPES);
        else format->addItems(DIR_VOLUME_TYPES);
    };
    format->insertSeparator(format->count());
    format->addItem(QIcon::fromTheme("insert-text"), "Set manually");
    sizeLayout = new QGridLayout();
    sizeLayout->addWidget(allocLabel, 0, 0);
    sizeLayout->addWidget(allocation, 0, 1);
    sizeLayout->addWidget(capLabel, 1, 0);
    sizeLayout->addWidget(capacity, 1, 1);
    sizeLayout->addWidget(formatLabel, 2, 0);
    sizeLayout->addWidget(format, 2, 1);
    sizeWdg = new QWidget(this);
    sizeWdg->setLayout(sizeLayout);
    usePerm = new QCheckBox("Use Permission", this);
    ownerLabel = new QLabel("Owner:", this);
    groupLabel = new QLabel("Group:", this);
    modeLabel = new QLabel("Mode:", this);
    labelLabel = new QLabel("Label:", this);
    owner = new QLineEdit(this);
    owner->setPlaceholderText("107");
    group = new QLineEdit(this);
    group->setPlaceholderText("107");
    mode = new QLineEdit(this);
    mode->setPlaceholderText("0744");
    label = new QLineEdit(this);
    label->setPlaceholderText("MAC (eg SELinux) label string");
    permLayout = new QGridLayout();
    permLayout->addWidget(ownerLabel, 0, 0);
    permLayout->addWidget(owner, 0, 1);
    permLayout->addWidget(groupLabel, 1, 0);
    permLayout->addWidget(group, 1, 1);
    permLayout->addWidget(modeLabel, 2, 0);
    permLayout->addWidget(mode, 2, 1);
    permLayout->addWidget(labelLabel, 3, 0);
    permLayout->addWidget(label, 3, 1);
    permissions = new QWidget(this);
    permissions->setLayout(permLayout);
    permissions->setEnabled(false);

    commonLayout->insertWidget(commonLayout->count()-1, sizeWdg);
    commonLayout->insertWidget(commonLayout->count()-1, usePerm);
    commonLayout->insertWidget(commonLayout->count()-1, permissions, -1);
    allocLabel->setCurrentIndex(1);
    capLabel->setCurrentIndex(1);
    connect(format, SIGNAL(currentIndexChanged(int)),
            this, SLOT(formatTypeChanged(int)));
    connect(usePerm, SIGNAL(toggled(bool)),
            permissions, SLOT(setEnabled(bool)));
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
    if ( usePerm->isChecked() || format->currentText()!="default" ) {
        _target = doc.createElement("target");
        if ( format->currentText()!="default" ) {
            _format = doc.createElement("format");
            _format.setAttribute("type", format->currentText().toLower());
            _target.appendChild(_format);
        };
        if ( usePerm->isChecked() ) {
            _permissions = doc.createElement("permissions");
            _owner = doc.createElement("owner");
            _text = doc.createTextNode(owner->text());
            _owner.appendChild(_text);
            _permissions.appendChild(_owner);
            _group = doc.createElement("group");
            _text = doc.createTextNode(group->text());
            _group.appendChild(_text);
            _permissions.appendChild(_group);
            _mode = doc.createElement("mode");
            _text = doc.createTextNode(mode->text());
            _mode.appendChild(_text);
            _permissions.appendChild(_mode);
            _label = doc.createElement("label");
            _text = doc.createTextNode(label->text());
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

/* private slots */
void CreateVolume::formatTypeChanged(int i)
{
    format->setEditable( i==format->count()-1 );
    format->clearEditText();
}
