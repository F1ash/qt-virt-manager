#include "create_virt_network.h"

CreateVirtNetwork::CreateVirtNetwork(QWidget *parent, Actions _act) :
    QDialog(parent), action(_act)
{
    setModal(false);
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

    showDescription = new QCheckBox("Show XML Description\nat close", this);
    showDescription->setChecked(showDesc);
    about = new QLabel("<a href='http://libvirt.org/formatnetwork.html'>About</a>", this);
    about->setOpenExternalLinks(true);
    about->setToolTip("http://libvirt.org/formatnetwork.html");
    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(about);
    buttonLayout->addWidget(showDescription);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(bridgeWdg);
    scrollLayout->addWidget(domainWdg);
    scrollLayout->addWidget(addressingWdg);
    scrollLayout->addWidget(forwardWdg);
    scrollLayout->setContentsMargins(3, 0, 3, 0);
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
    connect(forwardWdg, SIGNAL(toggled(bool)),
            this, SLOT(networkTypeChanged(bool)));
    connect(ipv6, SIGNAL(toggled(bool)),
            this, SLOT(ipv6Changed(bool)));
}
CreateVirtNetwork::~CreateVirtNetwork()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("NetCreateGeometry", saveGeometry());
    settings.setValue("NetCreateShowDesc", showDescription->isChecked());
    settings.endGroup();
    disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    delete networkName;
    networkName = NULL;
    delete uuid;
    uuid = NULL;

    delete bridgeWdg;
    bridgeWdg = NULL;

    delete domainWdg;
    domainWdg = NULL;

    delete addressingWdg;
    addressingWdg = NULL;

    delete forwardWdg;
    forwardWdg = NULL;

    delete scrollLayout;
    scrollLayout = NULL;
    delete scrolled;
    scrolled = NULL;
    delete scroll;
    scroll = NULL;

    delete about;
    about = NULL;
    delete showDescription;
    showDescription = NULL;
    delete ok;
    ok = NULL;
    delete cancel;
    cancel = NULL;
    delete buttonLayout;
    buttonLayout = NULL;
    delete buttons;
    buttons = NULL;

    delete netDescLayout;
    netDescLayout = NULL;

    delete xml;
    xml = NULL;
}

/* public slots */
int CreateVirtNetwork::getResult() const
{
    return result();
}
Actions CreateVirtNetwork::getAction() const
{
    return action;
}
QString CreateVirtNetwork::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateVirtNetwork::getShowing() const
{
    return showDescription->isChecked();
}

/* private slots */
void CreateVirtNetwork::buildXMLDescription()
{
    this->setEnabled(false);
    QDomDocument doc;
    //qDebug()<<doc.toString();
    QDomElement _xmlDesc, _name, _uuid, _bridge,
            _domain, _forward;
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
        _bridge = doc.createElement("bridge");
        _bridge.setAttribute(
                    "name",
                    bridgeWdg->bridgeName->text());
        _bridge.setAttribute(
                    "stp",
                    (bridgeWdg->stp->isChecked())? "on":"off");
        _bridge.setAttribute(
                    "delay",
                    bridgeWdg->delay->value());
        _bridge.setAttribute(
                    "macTableManager",
                    bridgeWdg->macTableManager->currentText());
        _xmlDesc.appendChild(_bridge);
    };
    if ( domainWdg->isUsed() ) {
        if ( !domainWdg->domain->text().isEmpty() ) {
            _domain = doc.createElement("domain");
            _domain.setAttribute(
                        "name",
                        domainWdg->domain->text());
            _xmlDesc.appendChild(_domain);
        };
    };
    if ( addressingWdg->isUsed() ) {
        _xmlDesc.appendChild(
                    addressingWdg->getDataDocument());
    };
    if ( forwardWdg->isUsed() ) {
        _forward = doc.createElement("forward");
        _forward.setAttribute(
                    "mode",
                    forwardWdg->mode->currentText());
        if ( forwardWdg->devLabel->isChecked() ) {
            _forward.setAttribute(
                        "dev",
                        forwardWdg->dev->text());
        };
        _forward.appendChild(
                    forwardWdg->getDataDocument());
        _xmlDesc.appendChild(_forward);
    };
    doc.appendChild(_xmlDesc);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtNetwork::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}
void CreateVirtNetwork::networkTypeChanged(bool state)
{
    bool _state =
            forwardWdg->mode->currentText()=="nat" ||
            forwardWdg->mode->currentText()=="route";
    bridgeWdg->setUsage(!state || _state);
    domainWdg->setUsage(!state || _state);
}
void CreateVirtNetwork::ipv6Changed(bool state)
{
    bridgeWdg->setUsage(state);
    bridgeWdg->setFreez(state);
    domainWdg->setUsage(false);
    forwardWdg->setUsage(false);
    domainWdg->setDisabled(state);
    forwardWdg->setDisabled(state);
    addressingWdg->ipv6Changed(state);
}
