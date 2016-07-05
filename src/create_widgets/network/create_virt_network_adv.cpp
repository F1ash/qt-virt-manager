#include "create_virt_network_adv.h"

CreateVirtNetwork_Adv::CreateVirtNetwork_Adv(
        QWidget *parent, Actions _act) :
    QWidget(parent), action(_act)
{
    setWindowTitle("Network Settings");
    settings.beginGroup("VirtNetControl");
    restoreGeometry(settings.value("NetCreateGeometry").toByteArray());
    bool showDesc = settings.value("NetCreateShowDesc").toBool();
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
    ipv6 = new QCheckBox("For guest-to-guest IPv6", this);
    trustGuestRxFilters = new QCheckBox("trustGuestRxFilters", this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(netNameLabel, 0, 0);
    baseLayout->addWidget(networkName, 0, 1);
    baseLayout->addWidget(uuidLabel, 1, 0);
    baseLayout->addWidget(uuid, 1, 1);
    baseLayout->addWidget(ipv6, 2, 0);
    baseLayout->addWidget(trustGuestRxFilters, 2, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    bridgeWdg = new Bridge_Widget(this);
    domainWdg = new Domain_Widget(this);
    addressingWdg = new Addressing_Widget(this);
    forwardWdg = new Forward_Widget(this);
    QoSWdg = new QoS_Widget(this);

    //showDescription = new QCheckBox(
    //"Show XML Description\nat close", this);
    //showDescription->setChecked(showDesc);
    about = new QLabel(
    "<a href='http://libvirt.org/formatnetwork.html'>About</a>",
                this);
    about->setOpenExternalLinks(true);
    about->setToolTip("http://libvirt.org/formatnetwork.html");
    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
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

    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(bridgeWdg);
    scrollLayout->addWidget(domainWdg);
    scrollLayout->addWidget(addressingWdg);
    scrollLayout->addWidget(forwardWdg);
    scrollLayout->addWidget(QoSWdg);
    scrollLayout->setContentsMargins(4, 0, 4, 0);
    scrollLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrollLayout);
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(scrolled);
    netDescLayout = new QVBoxLayout(this);
    netDescLayout->addWidget(baseWdg);
    netDescLayout->addWidget(scroll);
    netDescLayout->addWidget(buttons);
    setLayout(netDescLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
            bridgeWdg, SLOT(setUsage(bool)));
    connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
            domainWdg, SLOT(setUsage(bool)));
    connect(forwardWdg, SIGNAL(QoSAvailable(bool)),
            QoSWdg, SLOT(setUsage(bool)));
    connect(forwardWdg, SIGNAL(toggled(bool)),
            this, SLOT(networkTypeChanged(bool)));
    connect(ipv6, SIGNAL(toggled(bool)),
            this, SLOT(ipv6Changed(bool)));
    connect(networkName, SIGNAL(textChanged(QString)),
            this, SIGNAL(newName(QString)));
}
CreateVirtNetwork_Adv::~CreateVirtNetwork_Adv()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("NetCreateGeometry", saveGeometry());
    //settings.setValue("NetCreateShowDesc", showDescription->isChecked());
    settings.endGroup();
}

/* public slots */
int CreateVirtNetwork_Adv::getResult() const
{
    return 1; //result();
}
Actions CreateVirtNetwork_Adv::getAction() const
{
    return action;
}
void CreateVirtNetwork_Adv::readXmlDescData(QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _name, _uuid;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
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
        addressingWdg->setDataDescription(_xmlDesc);
        forwardWdg->setDataDescription(_xmlDesc);
        QoSWdg->setDataDescription(_xmlDesc);
    };
}
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
    this->setEnabled(false);
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
    if ( addressingWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    addressingWdg->getDataDocument());
    };
    if ( forwardWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    forwardWdg->getDataDocument());
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
    if ( sender()==ok ) {
        //setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        //setResult(QDialog::Rejected);
    };
    //done(result());
}
void CreateVirtNetwork_Adv::networkTypeChanged(bool state)
{
    bool _state =
            forwardWdg->getCurrentMode()=="nat" ||
            forwardWdg->getCurrentMode()=="route";
    bridgeWdg->setUsage(!state || _state);
    domainWdg->setUsage(!state || _state);
}
void CreateVirtNetwork_Adv::ipv6Changed(bool state)
{
    bridgeWdg->setUsage(state);
    bridgeWdg->setFreez(state);
    domainWdg->setUsage(false);
    forwardWdg->setUsage(false);
    domainWdg->setDisabled(state);
    forwardWdg->setDisabled(state);
    addressingWdg->ipv6Changed(state);
}
