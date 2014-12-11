#include "create_virt_network.h"

/*
 * INTERFACE TYPES:
 * network
 * bridge
 * direct
 * user             ( for usermod VMs only [KVM-?] )
 * - ethernet       ( requires special knowledge the user )
 * hostdev
 * mcast
 * server/client
 */

CreateVirtNetwork::CreateVirtNetwork(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Network Settings");
    restoreGeometry(settings.value("NetCreateGeometry").toByteArray());

    netNameLabel = new QLabel("Name:", this);
    networkName = new QLineEdit(this);
    networkName->setPlaceholderText("Enter Network Name");
    uuidLabel = new QLabel("UUID:");
    uuid = new QLineEdit(this);
    uuid->setPlaceholderText("if omitted, then auto generated");
    ipv6 = new QCheckBox("Use ipv6", this);
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
    forwardWdg = new Forward_Widget(this);

    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    netDescLayout = new QVBoxLayout(this);
    netDescLayout->addWidget(baseWdg);
    netDescLayout->addWidget(bridgeWdg);
    netDescLayout->addWidget(domainWdg);
    netDescLayout->addWidget(forwardWdg);
    netDescLayout->addStretch(-1);
    netDescLayout->addWidget(buttons);
    setLayout(netDescLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
            bridgeWdg->title, SLOT(setChecked(bool)));
    connect(forwardWdg, SIGNAL(optionalsNeed(bool)),
            domainWdg->title, SLOT(setChecked(bool)));
    connect(forwardWdg->title, SIGNAL(toggled(bool)),
            this, SLOT(networkTypeChanged(bool)));
}
CreateVirtNetwork::~CreateVirtNetwork()
{
    settings.setValue("NetCreateGeometry", saveGeometry());
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

    delete forwardWdg;
    forwardWdg = NULL;

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
QString CreateVirtNetwork::getXMLDescFileName() const
{
    return xml->fileName();
}

/* private slots */
void CreateVirtNetwork::buildXMLDescription()
{

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
    bridgeWdg->title->setChecked(!state || _state);
    domainWdg->title->setChecked(!state || _state);
}
