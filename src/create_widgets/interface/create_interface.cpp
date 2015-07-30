#include "create_interface.h"

CreateInterface::CreateInterface(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Interface Settings");
    settings.beginGroup("VirtIfaceControl");
    restoreGeometry(settings.value("IfaceCreateGeometry").toByteArray());
    bool showDesc = settings.value("IfaceCreateShowDesc").toBool();
    settings.endGroup();

    showXMLDescription = new QCheckBox("Show XML Description\nat close", this);
    showXMLDescription->setChecked(showDesc);
    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(showXMLDescription);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    name = new QLineEdit(this);
    name->setPlaceholderText("the public name for interface");
    type = new QComboBox(this);
    type->addItems(QStringList()<<"ethernet");
    type->setToolTip("Type");
    ip4 = new QLabel("ip4", this);
    ip4Addr = new QLineEdit(this);
    ip4Addr->setPlaceholderText("inet address");
    ip4Pref = new QSpinBox(this);
    ip4Pref->setRange(0, 1024);
    ip4Pref->setValue(8);
    ip4Pref->setToolTip("Prefix");
    ip6 = new QLabel("ip6", this);
    ip6Addr = new QLineEdit(this);
    ip6Addr->setPlaceholderText("inet address");
    ip6Pref = new QSpinBox(this);
    ip6Pref->setRange(0, 1024);
    ip6Pref->setValue(128);
    ip6Pref->setToolTip("Prefix");
    ipLayout = new QGridLayout(this);
    ipLayout->addWidget(name, 0, 1);
    ipLayout->addWidget(type, 0, 2);
    ipLayout->addWidget(ip4, 1, 0);
    ipLayout->addWidget(ip4Addr, 1, 1);
    ipLayout->addWidget(ip4Pref, 1, 2);
    ipLayout->addWidget(ip6, 2, 0);
    ipLayout->addWidget(ip6Addr, 2, 1);
    ipLayout->addWidget(ip6Pref, 2, 2);

    protocols = new QWidget(this);
    protocols->setLayout(ipLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(protocols);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
}
CreateInterface::~CreateInterface()
{
    settings.beginGroup("VirtIfaceControl");
    settings.setValue("IfaceCreateGeometry", saveGeometry());
    settings.setValue("IfaceCreateShowDesc", showXMLDescription->isChecked());
    settings.endGroup();
}

/* public slots */
QString CreateInterface::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateInterface::getShowing() const
{
    return showXMLDescription->isChecked();
}

/* private slots */
void CreateInterface::buildXMLDescription()
{
    /* as example:
     *<interface type='ethernet' name='lo'>
          <start mode='onboot'/>
          <protocol family='ipv4'>
              <ip address='127.0.0.1' prefix='8'/>
          </protocol>
          <protocol family='ipv6'>
              <ip address='::1' prefix='128'/>
          </protocol>
      </interface>
     */
    this->setEnabled(false);
    QDomDocument doc;
    QDomElement _iface, _protocol, _ip, _start;
    _iface = doc.createElement("interface");
    _iface.setAttribute("type", type->currentText());
    _iface.setAttribute("name", name->text());
    _start = doc.createElement("start");
    _start.setAttribute("mode", "onboot");
    _iface.appendChild(_start);
    _protocol = doc.createElement("protocol");
    _protocol.setAttribute("family", "ipv4");
    _ip = doc.createElement("ip");
    _ip.setAttribute("address", ip4Addr->text());
    _ip.setAttribute("prefix", ip4Pref->text());
    _protocol.appendChild(_ip);
    _iface.appendChild(_protocol);
    _protocol = doc.createElement("protocol");
    _protocol.setAttribute("family", "ipv6");
    _ip = doc.createElement("ip");
    _ip.setAttribute("address", ip6Addr->text());
    _ip.setAttribute("prefix", ip6Pref->text());
    _protocol.appendChild(_ip);
    _iface.appendChild(_protocol);
    doc.appendChild(_iface);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    //qDebug()<<doc.toString();
    xml->close();
}
void CreateInterface::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}
