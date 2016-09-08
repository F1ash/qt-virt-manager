#include "create_virt_network_adv.h"

CreateVirtNetwork_Adv::CreateVirtNetwork_Adv(
        QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Network Settings");
    settings.beginGroup("VirtNetControl");
    restoreGeometry(settings.value("NetCreateGeometry").toByteArray());
    //bool showDesc = settings.value("NetCreateShowDesc").toBool();
    settings.endGroup();

    netNameLabel = new QLabel("Name:", this);
    networkName = new QLineEdit(this);
    networkName->setPlaceholderText("Enter Network Name");
    uuidLabel = new QLabel("UUID:");
    uuid = new QLineEdit(this);
    uuid->setPlaceholderText("if omitted, then auto generated");
    /*
     * http://libvirt.org/formatnetwork.html#examplesNoGateway
     */
    noGW = new QCheckBox("Guest-to-guest", this);
    ipv6 = new QCheckBox("IPv6", this);
    ipv6->setEnabled(false);
    noGWLayout = new QHBoxLayout(this);
    noGWLayout->addWidget(noGW);
    noGWLayout->addWidget(ipv6);
    noGWWdg = new QWidget(this);
    noGWWdg->setLayout(noGWLayout);
    trustGuestRxFilters = new QCheckBox("trustGuestRxFilters", this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(netNameLabel, 0, 0);
    baseLayout->addWidget(networkName, 0, 1);
    baseLayout->addWidget(uuidLabel, 1, 0);
    baseLayout->addWidget(uuid, 1, 1);
    baseLayout->addWidget(noGWWdg, 2, 0);
    baseLayout->addWidget(trustGuestRxFilters, 2, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    bridgeWdg = new Bridge_Widget(this);
    domainWdg = new Domain_Widget(this);
    forwardWdg = new Forward_Widget(this);
    addressingWdg = new Addressing_Widget(this);
    QoSWdg = new QoS_Widget(this);
    tabs = new QTabWidget(this);
    tabs->setTabPosition(QTabWidget::West);
    tabs->addTab(bridgeWdg, "Bridge");
    tabs->addTab(domainWdg, "DNS Domain");
    tabs->addTab(forwardWdg, "Forward");
    tabs->addTab(addressingWdg, "IP");
    tabs->addTab(QoSWdg, "QoS");

    //showDescription = new QCheckBox(
    //"Show XML Description\nat close", this);
    //showDescription->setChecked(showDesc);
    about = new QLabel(
    "<a href='http://libvirt.org/formatnetwork.html'>About</a>",
                this);
    about->setOpenExternalLinks(true);
    about->setToolTip("http://libvirt.org/formatnetwork.html");
    ok = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                "Ok",
                this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"),
                "Cancel",
                this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(about);
    //buttonLayout->addWidget(showDescription);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    netDescLayout = new QVBoxLayout(this);
    netDescLayout->addWidget(baseWdg);
    netDescLayout->addWidget(tabs);
    netDescLayout->addWidget(buttons);
    setLayout(netDescLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    //connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
    //        bridgeWdg, SLOT(setUsage(bool)));
    //connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
    //        domainWdg, SLOT(setUsage(bool)));
    connect(forwardWdg, SIGNAL(QoSAvailable(bool)),
            QoSWdg, SLOT(setUsage(bool)));
    connect(forwardWdg, SIGNAL(toggled(bool)),
            this, SLOT(networkTypeChanged(bool)));
    connect(noGW, SIGNAL(toggled(bool)),
            this, SLOT(noGatewayChanged(bool)));
    connect(networkName, SIGNAL(textChanged(QString)),
            this, SIGNAL(newName(const QString&)));
}
CreateVirtNetwork_Adv::~CreateVirtNetwork_Adv()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("NetCreateGeometry", saveGeometry());
    //settings.setValue("NetCreateShowDesc", showDescription->isChecked());
    settings.endGroup();
}
void CreateVirtNetwork_Adv::readXmlDescData(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _name, _uuid, _ip, _route, _forward;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _ip = _network.firstChildElement("ip");
        _route = _network.firstChildElement("route");
        _forward = _network.firstChildElement("forward");
        if ( _network.hasAttribute("ipv6") )
            ipv6->setChecked(
                        (_network.attribute("ipv6")=="yes")?
                            Qt::Checked : Qt::Unchecked);
        if ( _network.hasAttribute("trustGuestRxFilters") )
            trustGuestRxFilters->setChecked(
                        (_network.attribute("trustGuestRxFilters")=="yes")?
                            Qt::Checked : Qt::Unchecked);
        _name = _network.firstChildElement("name");
        _uuid = _network.firstChildElement("uuid");
        networkName->setText(_name.text());
        uuid->setText(_uuid.text());
        bridgeWdg->setDataDescription(_xmlDesc);
        domainWdg->setDataDescription(_xmlDesc);
        forwardWdg->setDataDescription(_xmlDesc);
        addressingWdg->setDataDescription(_xmlDesc);
        QoSWdg->setDataDescription(_xmlDesc);
        noGW->setChecked(
                    _ip.isNull() && _route.isNull() && _forward.isNull() );
    };
}

/* public slots */
QString CreateVirtNetwork_Adv::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateVirtNetwork_Adv::getShowing() const
{
    //return showDescription->isChecked();
    return false;
}

/* private slots */
void CreateVirtNetwork_Adv::buildXMLDescription()
{
    QDomDocument doc;
    //qDebug()<<doc.toString();
    QDomElement _xmlDesc, _name, _uuid;
    QDomText data;

    _xmlDesc = doc.createElement("network");
    if ( ipv6->isChecked() ) {
        _xmlDesc.setAttribute("ipv6", "yes");
    };
    _xmlDesc.setAttribute(
                "trustGuestRxFilters",
                (trustGuestRxFilters->isChecked())? "yes":"no");
    _name = doc.createElement("name");
    data = doc.createTextNode(networkName->text());
    _name.appendChild(data);
    _uuid = doc.createElement("uuid");
    data = doc.createTextNode(uuid->text());
    _uuid.appendChild(data);
    _xmlDesc.appendChild(_name);
    _xmlDesc.appendChild(_uuid);

    if ( bridgeWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    bridgeWdg->getDataDocument());
    };
    if ( domainWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    domainWdg->getDataDocument());
    };
    if ( forwardWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    forwardWdg->getDataDocument());
    };
    if ( addressingWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    addressingWdg->getDataDocument());
    };
    if ( QoSWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    QoSWdg->getDataDocument());
    };
    doc.appendChild(_xmlDesc);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtNetwork_Adv::set_Result()
{
    this->setEnabled(false);
    if ( sender()==ok ) {
        buildXMLDescription();
        emit accepted(true);
    } else {
        emit accepted(false);
    };
}
void CreateVirtNetwork_Adv::networkTypeChanged(bool state)
{
    //bool _state =
    //        forwardWdg->getCurrentMode()=="nat" ||
    //        forwardWdg->getCurrentMode()=="route";
    //bridgeWdg->setUsage(!state || _state);
    //domainWdg->setUsage(!state || _state);
}
void CreateVirtNetwork_Adv::noGatewayChanged(bool state)
{
    ipv6->setEnabled(state);
    if ( !state ) ipv6->setChecked(state);
    bridgeWdg->setFreez(state);
    bridgeWdg->setUsage(state);
    domainWdg->setFreez(state);
    forwardWdg->setFreez(state);
    addressingWdg->noGatewayChanged(state);
}
