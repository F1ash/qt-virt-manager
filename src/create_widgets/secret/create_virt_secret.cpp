#include "create_virt_secret.h"

/*
 * http://libvirt.org/formatsecret.html
 */

CreateVirtSecret::CreateVirtSecret(
        QWidget *parent, virConnectPtr conn, QString _uuid) :
    QDialog(parent), currConnection(conn), UUID(_uuid)
{
    setModal(true);
    setWindowTitle("Secret Settings");
    settings.beginGroup("VirtSecretControl");
    restoreGeometry(settings.value("SecretCreateGeometry").toByteArray());
    bool showDesc = settings.value("SecCreateShowDesc").toBool();
    settings.endGroup();
    baseLayout = new QGridLayout();
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    showXMLDescription = new QCheckBox("Show XML Description\nat close", this);
    showXMLDescription->setChecked(showDesc);
    about = new QLabel("<a href='http://libvirt.org/formatsecret.html'>About</a>", this);
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
    buttonLayout->addWidget(showXMLDescription);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    uuid = new QLineEdit(this);
    if ( !UUID.isEmpty() ) {
        uuid->setText(UUID);
        uuid->setReadOnly(true);
    };
    uuid->setPlaceholderText("UUID generated if omitted");
    secDesc = new QLineEdit(this);
    secDesc->setPlaceholderText(
"A human-readable description of the purpose of the secret");
    secType = new QComboBox(this);
    secType->addItems(QStringList()<<"VOLUME"<<"CEPH"<<"iSCSI");
    if ( !UUID.isEmpty() ) {
        // find type
        secType->setEnabled(false);
    };
    ephemeralAttr = new QCheckBox("Ephemeral", this);
    ephemeralAttr->setToolTip(
"This secret must only be kept in memory,\n\
never stored persistently");
    privateAttr = new QCheckBox("Private", this);
    privateAttr->setToolTip(
"The value of the secret must not be revealed to any caller of libvirt,\n\
nor to any other node");
    propLayout = new QHBoxLayout(this);
    propLayout->addWidget(secType);
    propLayout->addWidget(ephemeralAttr);
    propLayout->addWidget(privateAttr);
    propWdg = new QWidget(this);
    propWdg->setLayout(propLayout);

    baseLayout->addWidget(uuid);
    baseLayout->addWidget(secDesc);
    baseLayout->addWidget(propWdg);

    scrollLayout = new QVBoxLayout(this);
    scrollLayout->setContentsMargins(3, 0, 3, 0);
    scrollLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrollLayout);
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(scrolled);
    secDescLayout = new QVBoxLayout(this);
    secDescLayout->addWidget(baseWdg);
    secDescLayout->addWidget(scroll);
    secDescLayout->addWidget(buttons);
    setLayout(secDescLayout);

    connect(secType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(secretTypeChanged(QString)));

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
}
CreateVirtSecret::~CreateVirtSecret()
{
    settings.beginGroup("VirtSecretControl");
    settings.setValue("SecretCreateGeometry", saveGeometry());
    settings.setValue("SecCreateShowDesc", showXMLDescription->isChecked());
    settings.endGroup();
}

/* public slots */
QString CreateVirtSecret::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateVirtSecret::getShowing() const
{
    return showXMLDescription->isChecked();
}

/* private slots */
void CreateVirtSecret::buildXMLDescription()
{
    this->setEnabled(false);
    QDomDocument doc;
    //qDebug()<<doc.toString();
    QDomElement _xmlDesc, _name, _uuid, _bridge,
            _domain, _forward;
    QDomText data;

    doc.appendChild(_xmlDesc);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtSecret::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}
void CreateVirtSecret::secretTypeChanged(QString _type)
{

}
