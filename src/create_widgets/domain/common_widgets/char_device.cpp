#include "char_device.h"

CharDevice::CharDevice(
        QWidget *parent, virConnectPtr *connPtr,
        virDomainPtr domain, QString _tag
        ) :
    _QWidget(parent, connPtr, domain), tag(_tag)
{
    devType = new QComboBox(this);
    devType->addItem("Pseudo TTY", "pty");
    devType->addItem("Physical host character device", "dev");
    devType->addItem("Output to a file", "file");
    devType->addItem("TCP net console", "tcp");
    devType->addItem("UDP net console", "udp");
    devType->addItem("Unix socket", "unix");

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
}

/* public slots */
QDomDocument CharDevice::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(
                charDevWdg->currentWidget());
    return wdg->getDataDocument();
}
