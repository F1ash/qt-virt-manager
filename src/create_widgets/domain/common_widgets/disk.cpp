#include "disk.h"

/*
 * http://libvirt.org/formatdomain.html#elementsDisks
 */

Disk::Disk(QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    sourceLabel = new QLabel("Source:", this);
    source = new QComboBox(this);
    source->addItem(
                tr("Storage Volume to use as the disk"),
                "volume");
    source->addItem(
                tr("Host Device to serve as the disk"),
                "block");
    source->addItem(
                tr("Directory to use as the disk"),
                "dir");
    source->addItem(
                tr("File holding the disk"),
                "file");
    source->addItem(
                tr("Network volume/image to use as the disk"),
                "network");
    sourceLayout = new QHBoxLayout(this);
    sourceLayout->addWidget(sourceLabel);
    sourceLayout->addWidget(source);
    sourceWdg = new QWidget(this);
    sourceWdg->setLayout(sourceLayout);

    info = new QStackedWidget(this);
    info->addWidget(new Volume_Disk(this, ptr_ConnPtr));
    info->addWidget(new Block_Disk(this, ptr_ConnPtr));
    info->addWidget(new Dir_Disk(this, ptr_ConnPtr));
    info->addWidget(new File_Disk(this, ptr_ConnPtr));
    info->addWidget(new Network_Disk(this, ptr_ConnPtr));
    for(int i=0; i<info->count(); i++) {
        _Disk *_d = static_cast<_Disk*>(info->widget(i));
        if ( Q_NULLPTR==_d ) continue;
        connect(_d, SIGNAL(errorMsg(const QString&)),
                this, SIGNAL(errorMsg(const QString&)));
    };

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(sourceWdg);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(source, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument Disk::getDataDocument() const
{
    QDomDocument doc;
    _Disk *wdg = static_cast<_Disk*>(info->currentWidget());
    if ( Q_NULLPTR!=wdg ) doc = wdg->getDataDocument();
    return doc;
}
