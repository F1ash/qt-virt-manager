#include "char_device.h"

#define CHAR_DEV_TYPE QStringList()\
    <<"Pseudo TTY /pty"\
    <<"Physical host character device /dev"\
    <<"Output to a file /file"\
    <<"TCP net console /tcp"\
    <<"UDP net console /udp"\
    <<"Unix socket /unix"

CharDevice::CharDevice(
        QWidget *parent,
        virConnectPtr conn,
        virDomainPtr domain,
        QString _tag
        ) :
    _QWidget(parent, conn, domain), tag(_tag)
{
    devType = new QComboBox(this);

    ptyWdg  = new PtyWidget(this, tag);
    devWdg  = new DevWidget(this, tag);
    fileWdg = new FileWidget(this, tag);
    tcpWdg  = new TcpWidget(this, tag);
    udpWdg  = new UdpWidget(this, tag);
    unixWdg = new UnixWidget(this, tag);

    charDevWdg = new QStackedWidget(this);
    charDevWdg->addWidget(ptyWdg);
    charDevWdg->addWidget(devWdg);
    charDevWdg->addWidget(fileWdg);
    charDevWdg->addWidget(tcpWdg);
    charDevWdg->addWidget(udpWdg);
    charDevWdg->addWidget(unixWdg);
    charDevWdg->addWidget(new _QWidget(this));
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devType);
    commonLayout->addWidget(charDevWdg);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(devType, SIGNAL(currentIndexChanged(int)),
            charDevWdg, SLOT(setCurrentIndex(int)));
    devType->addItems(CHAR_DEV_TYPE);
}

/* public slots */
QDomDocument CharDevice::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(
                charDevWdg->currentWidget());
    return wdg->getDataDocument();
}
