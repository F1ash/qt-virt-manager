#include "graphics_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsGraphics
 */

GraphicsDevice::GraphicsDevice(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    type = new QComboBox(this);
    type->addItem("Displays a window on the host desktop", "sdl");
    type->addItem("Use a VNC", "vnc");
    type->addItem("Use a Spice", "spice");
    type->addItem("Use a RDP", "rdp");
    type->addItem("Reserved for VirtualBox domains", "desktop");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    //commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
    typeChanged(0);
}

/* public slots */
QDomDocument GraphicsDevice::getDevDocument() const
{
    return info->getDevDocument();
}

/* private slots */
void GraphicsDevice::typeChanged(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    if ( _type == "sdl" ) {
        info = new SDL_Graphics(this);
    } else if ( _type == "vnc" ) {
        info = new VNC_Graphics(this, currWorkConnect);
    } else if ( _type == "spice" ) {
        info = new Spice_Graphics(this);
    } else if ( _type == "rdp" ) {
        info = new RDP_Graphics(this);
    } else if ( _type == "desktop" ) {
        info = new Desktop_Graphics(this);
    } else info = new _QWidget(this);
    commonLayout->insertWidget(1, info, -1);
}
