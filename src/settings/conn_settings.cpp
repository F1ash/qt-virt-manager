/*
 *  Remote URIs have the general form ("[...]" meaning an optional part):
 *  driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
 *  See for: http://libvirt.org/remote.html
 */

#include "settings/conn_settings.h"

// http://libvirt.org/drivers.html#hypervisor
#define HV_DRIVERS QStringList()<<"TEST"<<"LXC"<<"XEN"<<"QEMU/KVM"<<"QEMU/KVM user session"
//<<"OpenVZ"<<"UML"

// http://libvirt.org/remote.html#Remote_transports
#define TRANSPORTS QStringList()<<""<<"TLS"<<"SSH"<<"UNIX"<<"TCP"<<"LibSSH2"
//<<"EXT"


ConnSettings::ConnSettings(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Connect: <noname>");
    restoreGeometry(settings.value("SetDlgGeometry").toByteArray());
    setModal(false);
    setContentsMargins(1,1,1,1);
    initParamLayout();
    warning = new QLabel(this);
    warning->setText("<b>WARNING</b> See for: <a href='http://libvirt.org/remote.html'><b>Libvirt Remote support</b></a>");
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
    timerId = startTimer(1000);
}
ConnSettings::~ConnSettings()
{
    disconnect(ok, SIGNAL(clicked()), this, SLOT(saveConnect()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(cancelConnect()));
    disconnect(ConnName, SIGNAL(textChanged(QString)), this, SLOT(set_Title_Name(QString)));
    disconnect(Drivers, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeConnParameters(QString)));

    delete atStart;
    atStart = 0;
    delete ok;
    ok = 0;
    delete cancel;
    cancel = 0;
    delete warning;
    warning = 0;
    delete URI;
    URI = 0;
    delete buttonsLayout;
    buttonsLayout = 0;
    delete buttons;
    buttons = 0;

    delete connName;
    connName = 0;
    delete ConnName;
    ConnName = 0;
    delete driver;
    driver = 0;
    delete Drivers;
    Drivers = 0;
    delete transport;
    transport = 0;
    delete Transports;
    Transports = 0;
    delete host;
    host = 0;
    delete Host;
    Host = 0;
    delete path;
    path = 0;
    delete Path;
    Path = 0;
    delete extra;
    extra = 0;
    delete Extra;
    Extra = 0;
    delete paramLayout;
    paramLayout = 0;
    delete parameters;
    parameters = 0;
    delete commonLayout;
    commonLayout = 0;
}
void ConnSettings::initParamLayout()
{
    connName = new QLabel("Connect:", this);
    ConnName = new QLineEdit(this);
    ConnName->setPlaceholderText("Enter Connect Name");
    connect(ConnName, SIGNAL(textChanged(QString)), this, SLOT(set_Title_Name(QString)));
    driver = new QLabel("Driver:", this);
    Drivers = new QComboBox(this);
    Drivers->addItems(HV_DRIVERS);
    connect(Drivers, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeConnParameters(QString)));
    transport = new QLabel("Transport:", this);
    Transports = new QComboBox(this);
    Transports->addItems(TRANSPORTS);
    host = new QLabel("Host:", this);
    Host = new QLineEdit(this);
    Host->setPlaceholderText("[username@][hostname][:port]");
    path = new QLabel("Path:", this);
    Path = new QLineEdit(this);
    Path->setPlaceholderText("[path]");
    extra = new QLabel("Extra:", this);
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
}
void ConnSettings::initButtons()
{
    atStart = new QCheckBox("Connect at Start", this);
    atStart->setChecked(false);
    ok = new QPushButton(QIcon::fromTheme("dialog-ok"),"Ok");
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"),"Cancel");
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(atStart);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonsLayout);
    connect(ok, SIGNAL(clicked()), this, SLOT(saveConnect()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelConnect()));
}
void ConnSettings::setConnectItem(ConnItemIndex *idx)
{
    own_index = idx;
    name = idx->getName();
    previousName = name;
    set_Title_Name(name);
    if (name!=QString("<noname>")) {
        initParameters();
    };
}
void ConnSettings::saveConnect()
{
    name = ConnName->text();
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    setResult(QDialog::Accepted);
    if ( name.isEmpty() ) QMessageBox::information(this, QString("Info"), QString("Connect Name is empty."));
    else if ( groups.contains(name) && !newbe && name==previousName ) {
        saveParameters();
        close();
    } else if ( groups.contains(name) && newbe ) {
        QMessageBox::information(this, QString("Info"), QString("Same Connect Name is exist."));
    } else if ( groups.contains(name) && !newbe && name!=previousName ) {
        QMessageBox::information(this, QString("Info"), QString("Same Connect Name is exist."));
    } else if ( !groups.contains(name) && newbe ) {
        saveParameters();
        close();
    } else if ( !groups.contains(name) && !newbe ) {
        settings.beginGroup("Connects");
        settings.remove(previousName);
        settings.endGroup();
        //qDebug()<<previousName<<"included deleted";
        saveParameters();
        close();
    };
}
void ConnSettings::cancelConnect()
{
    if (newbe) {
        settings.beginGroup("Connects");
        settings.remove(name);
        settings.endGroup();
        emit creatingConnectCancelled();
    };
    setResult(QDialog::Rejected);
    close();
}
void ConnSettings::initParameters()
{
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    newbe = !groups.contains(name);
    //qDebug()<<"init"<<name<< newbe;
    settings.beginGroup(name);
    ConnName->setText(name);
    atStart->setChecked(settings.value("AtStart", QVariant(false)).toBool());
    groups = HV_DRIVERS;
    int idx = groups.indexOf(settings.value("Driver", "TEST").toString());
    if (idx<0) idx=0;
    Drivers->setCurrentIndex(idx);
    groups = TRANSPORTS;
    idx = groups.indexOf(settings.value("Transport", "").toString());
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
    settings.setValue("AtStart", QVariant(atStart->isChecked()));
    settings.setValue("Driver", Drivers->currentText());
    settings.setValue("Transport", Transports->currentText());
    settings.setValue("Host", Host->text());
    settings.setValue("Path", Path->text());
    settings.setValue("Extra", Extra->text());
    settings.endGroup();
    settings.endGroup();
    own_index->setName(name);
    if ( own_index->getData().value("isRunning").toBool() )
        QMessageBox::information(this, "Info", "New settings apply\nat next job start.");
}
void ConnSettings::closeEvent(QCloseEvent *ev)
{
    settings.setValue("SetDlgGeometry", saveGeometry());
    settings.sync();
    killTimer(timerId);
    ev->accept();
}
void ConnSettings::set_Title_Name(QString s)
{
    setWindowTitle(QString("Connect: %1").arg(s));
}
void ConnSettings::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    if ( _timerId && timerId==_timerId ) {
        // URI building
        QStringList _uri;
        _uri.append(Drivers->currentText().toLower().split("/").first());
        if ( !Transports->currentText().isEmpty() ) {
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
    };
}
void ConnSettings::changeConnParameters(QString s)
{
    if ( s.toLower().split("/").first()=="qemu" ) {
        Path->setText( (s.endsWith("session"))?"session":"system" );
    } else if ( s.toLower().split("/").first()=="test" ) {
        Path->setText( "default" );
    } else Path->clear();
}
