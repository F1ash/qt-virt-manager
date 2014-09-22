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

    info = new QStackedWidget(this);
    info->addWidget(new Volume_Disk(this, currWorkConnect));
    info->addWidget(new Block_Disk(this, currWorkConnect));
    info->addWidget(new Dir_Disk(this, currWorkConnect));
    info->addWidget(new File_Disk(this, currWorkConnect));
    info->addWidget(new Network_Disk(this, currWorkConnect));

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(sourceWdg);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(source, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument Disk::getDevDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    return wdg->getDevDocument();
}
