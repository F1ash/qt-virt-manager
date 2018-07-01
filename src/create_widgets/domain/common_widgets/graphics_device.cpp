#include "graphics_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsGraphics
 */

GraphicsDevice::GraphicsDevice(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    type = new QComboBox(this);
    type->addItem(tr("Displays a window on the host desktop"), "sdl");
    type->addItem(tr("Use a VNC"), "vnc");
    type->addItem(tr("Use a Spice"), "spice");
    type->addItem(tr("Use a RDP"), "rdp");
    type->addItem(tr("Reserved for VirtualBox domains"), "desktop");
    info = new QStackedWidget(this);
    for (int i=0; i<type->count(); i++) {
        setWidgets(i);
    };
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
    // set to more useful case (SPICE) and implemented in application
    type->setCurrentIndex(2);
}

/* public slots */
QDomDocument GraphicsDevice::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) doc = wdg->getDataDocument();
    return doc;
}

/* private slots */
void GraphicsDevice::setWidgets(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( _type.compare("sdl")==0 ) {
        info->addWidget(new SDL_Graphics(this));
    } else if ( _type.compare("vnc")==0 ) {
        info->addWidget(new VNC_Graphics(this, ptr_ConnPtr));
    } else if ( _type.compare("spice")==0 ) {
        info->addWidget(new Spice_Graphics(this, ptr_ConnPtr));
    } else if ( _type.compare("rdp")==0 ) {
        info->addWidget( new RDP_Graphics(this));
    } else if ( _type.compare("desktop")==0 ) {
        info->addWidget(new Desktop_Graphics(this));
    };
}
