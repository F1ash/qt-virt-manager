#include "_storage_target.h"

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

_Storage_Target::_Storage_Target(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _type) :
    QWidget(parent), currPoolType(_type)
{
    setAutoFillBackground(true);
    pathLabel = new QPushButton(
                QIcon::fromTheme("edit-find"), "", this);
    path = new QLineEdit(this);
    pathLayout = new QHBoxLayout(this);
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(path);
    pathWdg = new QWidget(this);
    pathWdg->setLayout(pathLayout);

    formatLabel = new QLabel(tr("Format:"), this);
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
    format->addItem(
                QIcon::fromTheme("insert-text"),
                tr("Set manually"));
    formatLayout = new QHBoxLayout(this);
    formatLayout->addWidget(formatLabel);
    formatLayout->addWidget(format);
    formatWdg = new QWidget(this);
    formatWdg->setLayout(formatLayout);

    usePerm = new QCheckBox(tr("Use Permission"), this);
    ownerLabel = new QLabel(tr("Owner:"), this);
    groupLabel = new QLabel(tr("Group:"), this);
    modeLabel = new QLabel(tr("Mode:"), this);
    labelLabel = new QLabel(tr("Label:"), this);
    owner = new QLineEdit(this);
    owner->setPlaceholderText("107");
    group = new QLineEdit(this);
    group->setPlaceholderText("107");
    mode = new QLineEdit(this);
    mode->setPlaceholderText("0744");
    label = new QLineEdit(this);
    label->setPlaceholderText(tr("MAC (eg SELinux) label string"));
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
    permissions->setVisible(false);

    encrypt = new Encryption(this, connPtrPtr);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(new QLabel(tr("<b>Target</b>")));
    commonLayout->addWidget(pathWdg);
    commonLayout->addWidget(formatWdg);
    commonLayout->addWidget(usePerm);
    commonLayout->addWidget(permissions);
    commonLayout->addWidget(encrypt);
    setLayout(commonLayout);
    pathWdg->setVisible(false);
    formatWdg->setVisible(false);
    connect(format, SIGNAL(currentIndexChanged(int)),
            this, SLOT(formatTypeChanged(int)));
    connect(usePerm, SIGNAL(toggled(bool)),
            permissions, SLOT(setVisible(bool)));
    connect(pathLabel, SIGNAL(clicked()),
            this, SLOT(setTargetDirectory()));
}

void _Storage_Target::setVolumeFormat(const QString &_fmt)
{
    int idx = format->findText(_fmt);
    if ( idx<0 ) idx = 0;
    format->setCurrentIndex(idx);
}

/* private slots */
void _Storage_Target::formatTypeChanged(int i)
{
    format->setEditable( i==format->count()-1 );
    format->clearEditText();
}
void _Storage_Target::setTargetDirectory()
{
    QString _dir = QFileDialog::getExistingDirectory(
                this,
                tr("Get Target Directory"),
                QString("/"));
    if ( !_dir.isEmpty() ) {
        path->setText(_dir);
    };
}
