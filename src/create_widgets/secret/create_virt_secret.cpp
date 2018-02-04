#include "create_virt_secret.h"
#include "secret_widgets/ceph_sec_type.h"
#include "secret_widgets/iscsi_sec_type.h"
#include "secret_widgets/volume_sec_type.h"
#include "secret_widgets/tls_sec_type.h"

/*
 * http://libvirt.org/formatsecret.html
 */

#define SECRET_TYPES QStringList()\
<<"VOLUME"<<"CEPH"<<"iSCSI"<<"TLS"

CreateVirtSecret::CreateVirtSecret(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    QDialog(parent), ptr_ConnPtr(connPtrPtr)
{
    setModal(true);
    setWindowTitle(tr("Create Secret"));
    settings.beginGroup("VirtSecretControl");
    restoreGeometry(settings.value("SecretCreateGeometry").toByteArray());
    bool showDesc = settings.value("SecCreateShowDesc").toBool();
    settings.endGroup();
    baseLayout = new QGridLayout();
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    showXMLDescription = new QCheckBox(
                tr("Show XML Description\nat close"), this);
    showXMLDescription->setChecked(showDesc);
    about = new QLabel(
                tr("<a href='http://libvirt.org/formatsecret.html'>About</a>"),
                this);
    about->setOpenExternalLinks(true);
    about->setToolTip("http://libvirt.org/formatsecret.html");
    ok = new QPushButton(tr("Ok"), this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton(tr("Cancel"), this);
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
    uuid->setPlaceholderText(tr("UUID generated if omitted"));
    secDesc = new QLineEdit(this);
    secDesc->setPlaceholderText(
tr("A human-readable description of the purpose of the secret"));
    secType = new QComboBox(this);
    secType->addItems(SECRET_TYPES);
    secType->setToolTip(tr("Type"));
    ephemeralAttr = new QCheckBox(tr("Ephemeral"), this);
    ephemeralAttr->setToolTip(
tr("This secret must only be kept in memory,\n\
never stored persistently"));
    privateAttr = new QCheckBox(tr("Private"), this);
    privateAttr->setToolTip(
tr("The value of the secret must not be revealed to any caller of libvirt,\n\
nor to any other node"));
    propLayout = new QHBoxLayout(this);
    propLayout->addWidget(secType);
    propLayout->addWidget(ephemeralAttr);
    propLayout->addWidget(privateAttr);
    propWdg = new QWidget(this);
    propWdg->setLayout(propLayout);
    secValue = new QLineEdit(this);
    secValue->setPlaceholderText(tr("Enter secret value/phrase"));
    stuffWdg = new QStackedWidget(this);
    stuffWdg->addWidget(new VolumeSecType(this, ptr_ConnPtr));
    stuffWdg->addWidget(new CephSecType(this, ptr_ConnPtr));
    stuffWdg->addWidget(new iSCSISecType(this, ptr_ConnPtr));
    stuffWdg->addWidget(new tlsSecType(this, ptr_ConnPtr));

    baseLayout->addWidget(uuid);
    baseLayout->addWidget(secDesc);
    baseLayout->addWidget(secValue);
    baseLayout->addWidget(propWdg);

    scrollLayout = new QVBoxLayout(this);
    scrollLayout->setContentsMargins(3, 0, 3, 0);
    scrollLayout->addWidget(stuffWdg);
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

    connect(secType, SIGNAL(currentIndexChanged(int)),
            stuffWdg, SLOT(setCurrentIndex(int)));

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
}
CreateVirtSecret::~CreateVirtSecret()
{
    secValue->clear();
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
QByteArray CreateVirtSecret::getSecretValue() const
{
    return secValue->text().toUtf8(); //.toBase64();
}
bool CreateVirtSecret::getShowing() const
{
    return showXMLDescription->isChecked();
}

/* private slots */
void CreateVirtSecret::buildXMLDescription()
{
    /* as example:
     *<secret ephemeral='no' private='yes'>
         <description>Super secret name of my first puppy</description>
         <uuid>0a81f5b2-8403-7b23-c8d6-21ccc2f80d6f</uuid>
         <usage type='volume'>
            <volume>/var/lib/libvirt/images/puppyname.img</volume>
         </usage>
      </secret>
     */
    this->setEnabled(false);
    QDomDocument doc, _usageDoc;
    _SecType* wdg = static_cast<_SecType*>(stuffWdg->currentWidget());
    QDomElement _secret, _description, _uuid, _usage;
    QDomText text;
    _secret = doc.createElement("secret");
    _description = doc.createElement("description");
    _uuid = doc.createElement("uuid");
    _secret.appendChild(_description);
    _secret.appendChild(_uuid);
    if ( wdg!=nullptr ) {
        _usageDoc = wdg->getSecStuff();
        _usage = _usageDoc.firstChildElement("usage");
    };
    _secret.appendChild(_usage);
    _secret.setAttribute(
                "ephemeral",
                (ephemeralAttr->isChecked())? "yes":"no");
    _secret.setAttribute(
                "private",
                (privateAttr->isChecked())? "yes":"no");
    doc.appendChild(_secret);
    text = doc.createTextNode(secDesc->text());
    _description.appendChild(text);
    text = doc.createTextNode(uuid->text());
    _uuid.appendChild(text);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    //qDebug()<<doc.toString();
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
