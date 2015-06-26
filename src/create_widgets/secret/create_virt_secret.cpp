#include "create_virt_secret.h"

/*
 * http://libvirt.org/formatsecret.html
 */

CreateVirtSecret::CreateVirtSecret(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Network Settings");
    settings.beginGroup("VirtSecretControl");
    restoreGeometry(settings.value("SecretCreateGeometry").toByteArray());
    bool showDesc = settings.value("SecCreateShowDesc").toBool();
    settings.endGroup();
    baseLayout = new QGridLayout();
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    showDescription = new QCheckBox("Show XML Description\nat close", this);
    showDescription->setChecked(showDesc);
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
    buttonLayout->addWidget(showDescription);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

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
    settings.setValue("SecCreateShowDesc", showDescription->isChecked());
    settings.endGroup();
    disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));

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
    delete secDescLayout;
    secDescLayout = NULL;

    delete xml;
    xml = NULL;
}

/* public slots */
QString CreateVirtSecret::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateVirtSecret::getShowing() const
{
    return showDescription->isChecked();
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
void CreateVirtSecret::secretTypeChanged(bool state)
{
}
