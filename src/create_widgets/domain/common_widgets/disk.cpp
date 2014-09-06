#include "disk.h"

/*
 * http://libvirt.org/formatdomain.html#elementsDisks
 */

#define DISK_TYPES QStringList()\
    <<"Storage Volume to use as the disk"\
    <<"Host Device to serve as the disk"\
    <<"Directory to use as the disk"\
    <<"File holding the disk"\
    <<"Network volume/image to use as the disk"


Disk::Disk(QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    sourceLabel = new QLabel("Source:", this);
    source = new QComboBox(this);
    source->addItems(DISK_TYPES);
    sourceLayout = new QHBoxLayout(this);
    sourceLayout->addWidget(sourceLabel);
    sourceLayout->addWidget(source);
    sourceWdg = new QWidget(this);
    sourceWdg->setLayout(sourceLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(sourceWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(source, SIGNAL(currentIndexChanged(int)),
            this, SLOT(sourceChanged(int)));
    sourceChanged(0);
}

/* public slots */
QDomDocument Disk::getDevDocument() const
{
    return info->getDevDocument();
}

/* private slots */
void Disk::sourceChanged(int i)
{
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    QString _source = source->currentText().toLower();
    if ( _source.startsWith("storage") ) {
        info = new Volume_Disk(this, currWorkConnect);
    } else if ( _source.startsWith("host") ) {
        info = new Block_Disk(this, currWorkConnect);
    } else if ( _source.startsWith("directory") ) {
        info = new Dir_Disk(this, currWorkConnect);
    } else if ( _source.startsWith("file") ) {
        info = new File_Disk(this, currWorkConnect);
    } else if ( _source.startsWith("network") ) {
        info = new Network_Disk(this, currWorkConnect);
    } else info = new _QWidget(this);
    commonLayout->insertWidget(1, info, -1);
}
