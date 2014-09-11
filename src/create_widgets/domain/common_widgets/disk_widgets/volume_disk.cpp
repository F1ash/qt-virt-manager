#include "volume_disk.h"

Volume_Disk::Volume_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    poolLabel = new QLabel("Pool:", this);
    pool = new QLabel(this);
    volumeLabel = new QPushButton("Volume:", this);
    volume = new QLineEdit(this);
    volume->setReadOnly(true);
    modeLabel = new QLabel("LUN source mode:", this);
    modeLabel->setEnabled(false);
    mode = new QComboBox(this);
    mode->addItems(QStringList()<<"Host"<<"Direct");
    mode->setEnabled(false);

    baseLayout->addWidget(poolLabel, 0, 0);
    baseLayout->addWidget(pool, 0, 1);
    baseLayout->addWidget(volumeLabel, 1, 0);
    baseLayout->addWidget(volume, 1, 1);
    baseLayout->addWidget(modeLabel, 2, 0);
    baseLayout->addWidget(mode, 2, 1);

    connect(devType->devType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(changeModeVisibility(QString)));
    connect(volumeLabel, SIGNAL(clicked()),
            this, SLOT(getVolumeNames()));
}

/* public slots */
QDomDocument Volume_Disk::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("pool", pool->text());
    _source.setAttribute("volume", volume->text());
    if ( startupPolicy->isUsed() )
        _source.setAttribute("startupPolicy",
                             startupPolicy->getStartupPolicy());
    if ( mode->isEnabled() )
        _source.setAttribute("mode",
                             mode->currentText().toLower());
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    TargetAttrs l = target->getTargetAttrs();
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _target.setAttribute(key, l.value(key));
    };
    _devDesc.appendChild(_target);

    if ( readOnly->state() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    };

    _devDesc.setAttribute("type", "volume");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Volume_Disk::changeModeVisibility(QString _devType)
{
    modeLabel->setEnabled( _devType.toLower()=="lun" );
    mode->setEnabled( _devType.toLower()=="lun" );
}
void Volume_Disk::getVolumeNames()
{
    QStringList _ret;
    if ( volumeDialog==NULL ) {
        volumeDialog = new VirtVolumeDialog(this, currWorkConnect);
    };
    if ( volumeDialog->exec()==1 ) {
        _ret = volumeDialog->getResult();
    };
    if ( !_ret.isEmpty() ) {
        pool->setText(_ret.first());
        volume->setText(_ret.last());
    };
}
