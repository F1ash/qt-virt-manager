#include "char_device.h"

#define CHAR_DEV_TYPE QStringList()\
    <<"Pseudo TTY (pty)"\
    <<"Physical host character device (dev)"\
    <<"Output to a file (file)"\
    <<"TCP net console (tcp)"\
    <<"UDP net console (udp)"\
    <<"Unix socket (unix)"

#define CHAR_DEV_TYPE_LIST QStringList()\
    <<"pty"<<"dev"<<"file"<<"tcp"<<"udp"<<"unix"

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

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devType);
    commonLayout->addWidget(ptyWdg);
    commonLayout->addWidget(devWdg);
    commonLayout->addWidget(fileWdg);
    commonLayout->addWidget(tcpWdg);
    commonLayout->addWidget(udpWdg);
    commonLayout->addWidget(unixWdg);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(devType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(devTypeChanged(int)));
    devType->addItems(CHAR_DEV_TYPE);
    QStringList l = CHAR_DEV_TYPE_LIST;
    for (int i=0; i<devType->count();i++) {
        devType->setItemData(i, l.at(i), Qt::UserRole);
    }
}

/* public slots */
QDomDocument CharDevice::getDevDocument() const
{
    QDomDocument doc;
    switch (devType->currentIndex()) {
    case 0:
        doc = ptyWdg->getDevDocument();
        break;
    case 1:
        doc = devWdg->getDevDocument();
        break;
    case 2:
        doc = fileWdg->getDevDocument();
        break;
    case 3:
        doc = tcpWdg->getDevDocument();
        break;
    case 4:
        doc = udpWdg->getDevDocument();
        break;
    case 5:
        doc = unixWdg->getDevDocument();
        break;
    default:
        break;
    };
    return doc;
}

/* private slots */
void CharDevice::devTypeChanged(int i)
{
    ptyWdg->setVisible(i==0);
    devWdg->setVisible(i==1);
    fileWdg->setVisible(i==2);
    tcpWdg->setVisible(i==3);
    udpWdg->setVisible(i==4);
    unixWdg->setVisible(i==5);
}
