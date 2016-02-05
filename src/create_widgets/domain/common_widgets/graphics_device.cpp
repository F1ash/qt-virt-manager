#include "graphics_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsGraphics
 */

GraphicsDevice::GraphicsDevice(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    type = new QComboBox(this);
    type->addItem("Displays a window on the host desktop", "sdl");
    type->addItem("Use a VNC", "vnc");
    type->addItem("Use a Spice", "spice");
    type->addItem("Use a RDP", "rdp");
    type->addItem("Reserved for VirtualBox domains", "desktop");
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
    if ( NULL!=wdg ) doc = wdg->getDataDocument();
    return doc;
}

/* private slots */
void GraphicsDevice::setWidgets(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( _type == "sdl" ) {
        info->addWidget(new SDL_Graphics(this));
    } else if ( _type == "vnc" ) {
        info->addWidget(new VNC_Graphics(this, ptr_ConnPtr));
    } else if ( _type == "spice" ) {
        info->addWidget(new Spice_Graphics(this, ptr_ConnPtr));
    } else if ( _type == "rdp" ) {
        info->addWidget( new RDP_Graphics(this));
    } else if ( _type == "desktop" ) {
        info->addWidget(new Desktop_Graphics(this));
    };
}
