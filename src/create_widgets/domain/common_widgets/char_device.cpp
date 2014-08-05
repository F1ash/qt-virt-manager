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

CharDevice::CharDevice(QWidget *parent) :
    _QWidget(parent)
{
    devType = new QComboBox(this);

    ptyWdg = new PtyWidget(this);
    devWdg = new DevWidget(this);
    fileWdg = new FileWidget(this);
    tcpWdg = new TcpWidget(this);
    udpWdg = new UdpWidget(this);
    unixWdg = new UnixWidget(this);

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
CharDevice::~CharDevice()
{
    disconnect(devType, SIGNAL(currentIndexChanged(int)),
               this, SLOT(devTypeChanged(int)));
    delete devType;
    devType = 0;

    delete ptyWdg;
    ptyWdg = 0;
    delete devWdg;
    devWdg = 0;
    delete fileWdg;
    fileWdg = 0;
    delete tcpWdg;
    tcpWdg = 0;
    delete udpWdg;
    udpWdg = 0;
    delete unixWdg;
    unixWdg = 0;

    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomNodeList CharDevice::getNodeList() const
{
    QDomNodeList result;
    switch (devType->currentIndex()) {
    case 0:
        result = ptyWdg->getNodeList();
        break;
    case 1:
        result = devWdg->getNodeList();
        break;
    case 2:
        result = fileWdg->getNodeList();
        break;
    case 3:
        result = tcpWdg->getNodeList();
        break;
    case 4:
        result = udpWdg->getNodeList();
        break;
    case 5:
        result = unixWdg->getNodeList();
        break;
    default:
        break;
    }
    return result;
}
QString CharDevice::getDevType() const
{
    return devType->itemData(devType->currentIndex(), Qt::UserRole).toString();
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
