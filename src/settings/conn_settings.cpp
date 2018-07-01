/*
 *  Remote URIs have the general form ("[...]" meaning an optional part):
 *  driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
 *  See for: http://libvirt.org/remote.html
 */

#include "settings/conn_settings.h"

// http://libvirt.org/drivers.html#hypervisor
#define HV_DRIVERS QStringList()\
<<"TEST"<<"LXC"<<"XEN"<<"QEMU/KVM"<<"QEMU/KVM user session"\
<<"VBox"<<"VMware Player"<<"VMware Workstation"<<"VMware Fusion"\
<<"VMware ESX"<<"VMware GSX"<<"VMware VPX"<<"OpenVZ"<<"HyperV"\
<<"IBM PowerVM"<<"Bhyve"//<<"UML"

// http://libvirt.org/remote.html#Remote_transports
#define TRANSPORTS QStringList()\
<<" -- "<<"TLS"<<"SSH"<<"UNIX"<<"TCP"<<"LibSSH2"<<"EXT"

ConnSettings::ConnSettings(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Connection: <noname>"));
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    restoreGeometry(settings.value("SetDlgGeometry").toByteArray());
    setModal(false);
    setContentsMargins(1,1,1,1);
    initParamLayout();
    warning = new QLabel(this);
    warning->setText(
    tr("<b>WARNING</b> See for: <a href='http://libvirt.org/remote.html'>\
<b>Libvirt Remote support</b></a>"));
    warning->setOpenExternalLinks(true);
    initButtons();
    URI = new QTextEdit(this);
    URI->setReadOnly(true);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(parameters);
    commonLayout->addWidget(warning);
    commonLayout->addWidget(URI);
    commonLayout->addWidget(buttons);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    initParameters();
}
void ConnSettings::initParamLayout()
{
    connName = new QLabel(tr("Connection:"), this);
    ConnName = new QLineEdit(this);
    ConnName->setPlaceholderText(tr("Enter Connection Name"));
    connect(ConnName, SIGNAL(textChanged(QString)),
            this, SLOT(set_Title_Name(QString)));
    driver = new QLabel(tr("Driver:"), this);
    Drivers = new QComboBox(this);
    Drivers->addItems(HV_DRIVERS);
    for (int i=0; i<Drivers->count(); i++) {
        QString _text =  Drivers->itemText(i);
        QIcon _icon = QIcon::fromTheme( getIconName( _text ) );
        Drivers->setItemIcon(i, _icon);
        Drivers->setItemData(i, getDriverName( _text ));
    };
    connect(Drivers, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(changeDriver(QString)));
    transport = new QLabel(tr("Transport:"), this);
    Transports = new QComboBox(this);
    Transports->addItems(TRANSPORTS);
    for (int i=0; i<Transports->count(); i++) {
        QString _text =  Transports->itemText(i);
        if ( _text.contains("--") ) _text.clear();
        Transports->setItemData(i, _text.toLower() );
    };
    host = new QLabel(tr("Host:"), this);
    Host = new QLineEdit(this);
    Host->setPlaceholderText("[username@][hostname][:port]");
    path = new QLabel(tr("Path:"), this);
    Path = new QLineEdit(this);
    Path->setPlaceholderText("[path]");
    extra = new QLabel(tr("Extra:"), this);
    Extra = new QLineEdit(this);
    Extra->setPlaceholderText("[?extraparameters]");
    paramLayout = new QGridLayout();
    paramLayout->addWidget(connName, 0, 0);
    paramLayout->addWidget(ConnName, 0, 1);
    paramLayout->addWidget(driver, 1, 0);
    paramLayout->addWidget(Drivers, 1, 1);
    paramLayout->addWidget(transport, 2, 0);
    paramLayout->addWidget(Transports, 2, 1);
    paramLayout->addWidget(host, 3, 0);
    paramLayout->addWidget(Host, 3, 1);
    paramLayout->addWidget(path, 4, 0);
    paramLayout->addWidget(Path, 4, 1);
    paramLayout->addWidget(extra, 5, 0);
    paramLayout->addWidget(Extra, 5, 1);
    parameters = new QWidget(this);
    parameters->setLayout(paramLayout);
    connect(Transports, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeURI()));
    connect(Host, SIGNAL(textChanged(QString)),
            this, SLOT(changeURI()));
    connect(Path, SIGNAL(textChanged(QString)),
            this, SLOT(changeURI()));
    connect(Extra, SIGNAL(textChanged(QString)),
            this, SLOT(changeURI()));
}
void ConnSettings::initButtons()
{
    atStart = new QCheckBox(tr("Connect at Start"), this);
    atStart->setChecked(false);
    ok = new QPushButton(
                QIcon::fromTheme("dialog-ok"), tr("Ok"));
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"), tr("Cancel"));
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(atStart);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonsLayout);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(saveConnection()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(cancelConnection()));
}
void ConnSettings::setConnectItem(ConnItemIndex *idx)
{
    own_index = idx;
    name = idx->getName();
    previousName = name;
    set_Title_Name(name);
    if (name.compare("<noname>")!=0) {
        initParameters();
    };
}
void ConnSettings::saveConnection()
{
    name = ConnName->text();
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    //setResult(QDialog::Accepted);
    if ( name.isEmpty() ) {
        QMessageBox::information(
                    this,
                    tr("Info"),
                    tr("Connection Name is empty."));
        return;
    } else if ( groups.contains(name)
                && !newbe && name.compare(previousName)==0 ) {
        saveParameters();
    } else if ( groups.contains(name) && newbe ) {
        QMessageBox::information(
                    this,
                    tr("Info"),
                    tr("Same Connection Name is exist."));
        return;
    } else if ( groups.contains(name) && !newbe
                && name.compare(previousName)!=0 ) {
        QMessageBox::information(
                    this,
                    tr("Info"),
                    tr("Same Connection Name is exist."));
        return;
    } else if ( !groups.contains(name) && newbe ) {
        saveParameters();
    } else if ( !groups.contains(name) && !newbe ) {
        settings.beginGroup("Connects");
        settings.remove(previousName);
        settings.endGroup();
        //qDebug()<<previousName<<"included deleted";
        saveParameters();
    };
    settings.setValue("SetDlgGeometry", saveGeometry());
    settings.sync();
    done(QDialog::Accepted);
}
void ConnSettings::cancelConnection()
{
    if (newbe) {
        settings.beginGroup("Connects");
        if ( !name.isEmpty() )
            settings.remove(name);
        settings.endGroup();
        emit creationConnCancelled();
    };
    //setResult(QDialog::Rejected);
    settings.setValue("SetDlgGeometry", saveGeometry());
    settings.sync();
    done(QDialog::Rejected);
}
void ConnSettings::initParameters()
{
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    newbe = !groups.contains(name);
    //qDebug()<<"init"<<name<< newbe;
    settings.beginGroup(name);
    ConnName->setText(name);
    atStart->setChecked(
                settings.value("AtStart", QVariant(false)).toBool());
    groups = HV_DRIVERS;
    // https://github.com/F1ash/qt-virt-manager/issues/2
    int idx = groups.indexOf(
                settings.value("Driver", "QEMU/KVM").toString());
    if (idx<0) idx=0;
    Drivers->setCurrentIndex(idx);
    // https://github.com/F1ash/qt-virt-manager/issues/2#comment
    idx = Transports->findData(
                settings.value("Transport", " -- ").toString());
    if (idx<0) idx=0;
    Transports->setCurrentIndex(idx);
    Host->setText(settings.value("Host", "").toString());
    QString path = settings.value("Path", "").toString();
    if ( !path.isEmpty() ) Path->setText(path);
    Extra->setText(settings.value("Extra", "").toString());
    settings.endGroup();
    settings.endGroup();
}
void ConnSettings::saveParameters()
{
    settings.beginGroup("Connects");
    settings.beginGroup(name);
    settings.setValue(
                "AtStart",
                QVariant(atStart->isChecked()));
    settings.setValue(
                "Driver",
                Drivers->currentText());
#if QT_VERSION>=0x050200
    settings.setValue(
                "Transport",
                Transports->currentData(Qt::UserRole).toString());
#else
    settings.setValue(
                "Transport",
                Transports->itemData(Transports->currentIndex()).toString());
#endif
    settings.setValue("Host", Host->text());
    settings.setValue("Path", Path->text());
    settings.setValue("Extra", Extra->text());
    settings.endGroup();
    settings.endGroup();
    settings.sync();
    own_index->setName(name);
    if ( own_index->getData().value("isRunning").toBool() )
        QMessageBox::information(
                    this,
                    tr("Info"),
                    tr("New settings apply\nat next job start."));
}
void ConnSettings::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    cancelConnection();
}
void ConnSettings::set_Title_Name(QString s)
{
    s.replace(" ", "_");
    ConnName->setText(s);
    setWindowTitle(QString(tr("Connection: %1")).arg(s));
}
void ConnSettings::changeURI()
{
    // URI building
    QStringList _uri;
#if QT_VERSION>=0x050200
    _uri.append(Drivers->currentData(Qt::UserRole).toString());
#else
    QString _data = Drivers->itemData(
                Drivers->currentIndex()).toString();
    _uri.append(_data);
#endif
    QString _transport;
#if QT_VERSION>=0x050200
    _transport = Transports->currentData(
                Qt::UserRole).toString();
#else
    _transport = Transports->itemData(
                Transports->currentIndex()).toString();
#endif
    if ( !_transport.isEmpty() ) {
        _uri.append("+");
        _uri.append(Transports->currentText().toLower());
    };
    _uri.append("://");
    if ( !Host->text().isEmpty() ) {
        _uri.append(Host->text());
    };
    _uri.append("/");
    if ( !Path->text().isEmpty() ) {
        _uri.append(Path->text());
    };
    if ( !Extra->text().isEmpty() ) {
        _uri.append("?");
        _uri.append(Extra->text());
    };
    URI->setText(_uri.join(""));
}
void ConnSettings::changeDriver(QString s)
{
    QString _name = getIconName(s);
    Host->setPlaceholderText("[username@][hostname][:port]");
    Transports->setEnabled(true);
    Path->setPlaceholderText("[path]");
    Path->setEnabled(true);
    Extra->setPlaceholderText("[?extraparameters]");
    Extra->setEnabled(true);
    if ( _name.compare("qemu")==0 ) {
        Path->setText( (s.endsWith("session"))?"session":"system" );
        Path->setEnabled(false);
    } else if ( _name.compare("vbox")==0 ) {
        Path->setText( "session" );
        Path->setEnabled(false);
    } else if ( _name.compare("vmware")==0 ) {
        if ( getDriverName(s).startsWith("vmware") ) {
            // VMware Player/Workstation
            Path->setText( "session" );
            Path->setEnabled(false);
        } else {
            // VMware ESX
            Transports->setCurrentIndex(0);
            Transports->setEnabled(false);
            Path->setPlaceholderText(
            "[[folder/...]datacenter/[folder/...][cluster/]server]");
            Path->clear();
        };
    } else if ( _name.compare("openvz")==0 ) {
        Path->setText( "system" );
        Path->setEnabled(false);
    } else if ( _name.compare("hyperv")==0 ) {
        Transports->setCurrentIndex(0);
        Transports->setEnabled(false);
        Path->clear();
        Path->setEnabled(false);
    } else if ( _name.compare("phyp")==0 ) {
        Host->setPlaceholderText("[username@]{hmc|ivm}");
        Transports->setCurrentIndex(0);
        Transports->setEnabled(false);
        Path->setText( "system" );
        Path->setEnabled(false);
        Extra->clear();
        Extra->setEnabled(false);
    } else if ( _name.compare("bhyve")==0 ) {
        Path->setText( "system" );
        Path->setEnabled(false);
        Extra->clear();
        Extra->setEnabled(false);
    } else if ( getDriverName(s).compare("test")==0 ) {
        Path->setText( "default" );
    } else Path->clear();
    changeURI();
}
QString ConnSettings::getIconName(QString &_text) const
{
    QString ret(_text.split("/").first().split(" ").first().toLower());
    if        ( ret.compare("test")==0 ) {
        ret = "wipe";
    } else if ( ret.compare("ibm")==0 ) {
        ret = "phyp";
    };
    return ret;
}
QString ConnSettings::getDriverName(QString &_text) const
{
    // relayted parts: ConnElement::buildURI, ConnItemModel::ConnItemModel;
    QString ret;
    QString _drv_row = _text.toLower().split("/").first();
    if        ( _drv_row.startsWith("vmware") ) {
        if ( _drv_row.endsWith("player") ) {
            ret.append("vmwareplayer");
        } else if ( _drv_row.endsWith("workstation") ) {
            ret.append("vmwarews");
        } else if ( _drv_row.endsWith("fusion") ) {
            ret.append("vmwarefusion");
        } else if ( _drv_row.endsWith("esx") ) {
            ret.append("esx");
        } else if ( _drv_row.endsWith("gsx") ) {
            ret.append("gsx");
        } else if ( _drv_row.endsWith("vpx") ) {
            ret.append("vpx");
        };
    } else if ( _drv_row.startsWith("ibm") ) {
        ret = "phyp";
    } else
        ret = _drv_row;
    return ret;
}
