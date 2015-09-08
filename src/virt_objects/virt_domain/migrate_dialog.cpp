#include "migrate_dialog.h"
#include <libvirt/libvirt.h>

#define APPEND_URI_HELP_0 QString("The destination libvirtd server \
will automatically determine the native hypervisor URI for migration, \
based off the primary hostname. ")

#define APPEND_URI_HELP_1 QString("The optional uri parameter controls \
how the source libvirtd connects to the destination libvirtd, \
in case it is not accessible using the same address that the client uses \
to connect to the destination, or a different encryption/auth scheme is required. ")

#define APPEND_URI_HELP_2 QString("The native hypervisor URI format is not used at all. ")

#define APPEND_URI_HELP_3 QString("To force migration over an alternate \
network interface the optional hypervisor specific URI must be provided. ")

#define APPEND_URI_HELP_4 QString("There is no scope for forcing \
an alternative network interface for the native migration data with this method. ")

#define APPEND_URI_HELP_5 QString("There is no use or requirement for a destination \
libvirtd instance at all. This is typically used when the hypervisor has \
its own native management daemon available to handle incoming migration attempts \
on the destination. ")

#define APPEND_URI_HELP_6 QString("The destination URI must be reachable using \
the source libvirtd credentials (which are not necessarily the same as \
the credentials of the client in connecting to the source). ")

#define EXAMPLE_URI_1 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [HV-URI]<br><br>\
<i>eg using default network interface</i>:<br><br>\
virsh migrate web1 qemu+ssh://desthost/system<br>\
virsh migrate web1 xen+tls://desthost/system<br><br>\
<i>eg using secondary network interface:</i><br><br>\
virsh migrate web1 qemu://desthost/system tcp://10.0.0.1/<br>\
virsh migrate web1 xen+tcp://desthost/system xenmigr:10.0.0.1/")

#define EXAMPLE_URI_2 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
This mode cannot be invoked from virsh ")

#define EXAMPLE_URI_3 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
This mode cannot be invoked from virsh ")

#define EXAMPLE_URI_4 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME HV-URI<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --direct web1 xenmigr://desthost/")

#define EXAMPLE_URI_5 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [ALT-DEST-LIBVIRT-URI]<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system<br><br>\
<i>eg using different libvirt URI auth scheme for peer2peer connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system qemu+tls:/desthost/system<br><br>\
<i>eg using different libvirt URI hostname for peer2peer connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system qemu+ssh://10.0.0.1/system")

#define EXAMPLE_URI_6 QString("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [ALT-DEST-LIBVIRT-URI]<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system<br><br>\
<i>eg using different libvirt URI auth scheme for peer2peer connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system qemu+tls:/desthost/system<br><br>\
<i>eg using different libvirt URI hostname for peer2peer connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system qemu+ssh://10.0.0.1/system")

MigrateDialog::MigrateDialog(QWidget *parent,
                             const QString arg1,
                             const QString arg2,
                             const QString arg3,
                             const QStringList list) :
    QDialog(parent), domainName(arg1), hostName(arg2),
    connType(arg3), connList(list)
{
    setWindowTitle(QString("Migrate '%1'").arg(domainName));
    commonLayout = new QVBoxLayout();
    Name = new QLineEdit(this);
    Name->setText(domainName);
    Name->setPlaceholderText("Enter new VM name");
    commonLayout->addWidget(Name);
    host = new QLabel(QString("Original Host / Connection Type : %1 / %2")
                      .arg(hostName).arg(connType.toUpper()), this);
    commonLayout->addWidget(host);
    newHost = new QLabel("Destination Host / Connection :", this);
    commonLayout->addWidget(newHost);
    connectList = new QComboBox(this);
    connList.prepend("Set Host/URI manually");
    connectList->addItems(connList);
    commonLayout->addWidget(connectList);

    connLayout = new QGridLayout();
    uriLabel = new QLabel("Destination\nHost/URI", this);
    uri = new QLineEdit(this);
    help = new QLabel(this);
    QIcon icon = QIcon::fromTheme("dialog-warning");
    if ( icon.isNull() ) {
        help->setText("Help:");
    } else {
        help->setPixmap(icon.pixmap(this->fontInfo().pixelSize()));
    };
    helpText = new QTextBrowser(this);
    connLayout->addWidget(uriLabel, 0, 0, Qt::AlignRight);
    connLayout->addWidget(uri, 0, 2);
    connLayout->addWidget(help, 1, 0, Qt::AlignRight|Qt::AlignTop);
    connLayout->addWidget(helpText, 1, 2);
    connectivity = new QWidget(this);
    connectivity->setLayout(connLayout);

    useAdvanced = new QCheckBox("Advanced", this);
    useAdvanced->setChecked(false);
    commonLayout->addWidget(useAdvanced);
    //commonLayout->addWidget(connectivity);
    commonLayout->insertStretch(-1);

    advanced = new QWidget(this);
    advanced->setEnabled(false);
    advLayout = new QGridLayout();
    nativeMigration = new QRadioButton("Native migration data\nover hypervisor transport\n(encrypt if HV support it)", this);
    nativeMigration->setChecked(true);
    nativeMigration->setLayoutDirection(Qt::RightToLeft);
    tunnelMigration = new QRadioButton("Tunnel migration data\nover libvirtd connection\n(encrypt always)", this);
    tunnelMigration->setLayoutDirection(Qt::LeftToRight);
    p2pMigration = new QLabel("Use peer2peer", this);
    maxDownTimeLabel = new QLabel("Maximum tolerable downtime\nfor live migration (ms)", this);
    maxDownTimeLabel->setEnabled( connType.toLower()!="lxc" );
    p2pCheck = new QCheckBox(this);
    p2pCheck->setChecked(p2p);
    maxDownCheck = new QCheckBox(this);
    maxDownCheck->setChecked(false);
    maxDownCheck->setEnabled( connType.toLower()!="lxc" );
    maxDownTime = new QSpinBox(this);
    maxDownTime->setEnabled(false);
    maxDownTime->setValue(30);
    bandWdthLabel = new QLabel("BandWidth (MiB/s)", this);
    bandWdthCheck = new QCheckBox(this);
    bandWdthCheck->setChecked(false);
    bandwidth = new QSpinBox(this);
    bandwidth->setEnabled(false);
    liveMigration = new QCheckBox("Live migration", this);
    liveMigration->setLayoutDirection(Qt::RightToLeft);
    persistDestMigration = new QCheckBox("Persist the VM\non the destination", this);
    persistDestMigration->setLayoutDirection(Qt::LeftToRight);
    undefineSourceMigration = new QCheckBox("Undefine the VM\non the source", this);
    undefineSourceMigration->setLayoutDirection(Qt::RightToLeft);
    pausedMigration = new QCheckBox("Leave\nthe domain suspended\non the remote side", this);
    pausedMigration->setLayoutDirection(Qt::LeftToRight);
    fullNonSharedDiskMigration = new QCheckBox("Migration with\nnon-shared storage\nwith full disk copy", this);
    fullNonSharedDiskMigration->setLayoutDirection(Qt::RightToLeft);
    incNonSharedDiskMigration = new QCheckBox("Migration with\nnon-shared storage\nwith incremental copy", this);
    incNonSharedDiskMigration->setLayoutDirection(Qt::LeftToRight);
    unsafeMigration = new QCheckBox("Force migration even\nif it is considered unsafe", this);
    unsafeMigration->setLayoutDirection(Qt::RightToLeft);
    offlineMigration = new QCheckBox("Migrate offline", this);
    offlineMigration->setLayoutDirection(Qt::LeftToRight);
    compressedMigration = new QCheckBox("Compress data\nduring migration", this);
    compressedMigration->setLayoutDirection(Qt::RightToLeft);
    abortOnMigration = new QCheckBox("Abort migration\non I/O errors happened\nduring migration", this);
    abortOnMigration->setLayoutDirection(Qt::LeftToRight);
    advLayout->addWidget(nativeMigration, 0, 0);
    advLayout->addWidget(tunnelMigration, 0, 2);
    advLayout->addWidget(p2pMigration, 1, 0, Qt::AlignRight);
    advLayout->addWidget(p2pCheck, 1, 1, Qt::AlignBottom|Qt::AlignCenter);
    advLayout->addWidget(maxDownTimeLabel, 2, 0, Qt::AlignRight);
    advLayout->addWidget(maxDownCheck, 2, 1, Qt::AlignBottom|Qt::AlignCenter);
    advLayout->addWidget(maxDownTime, 2, 2, Qt::AlignBottom);
    advLayout->addWidget(bandWdthLabel, 3, 0, Qt::AlignRight);
    advLayout->addWidget(bandWdthCheck, 3, 1, Qt::AlignBottom|Qt::AlignCenter);
    advLayout->addWidget(bandwidth, 3, 2, Qt::AlignBottom);
    advLayout->addWidget(liveMigration, 4, 0);
    advLayout->addWidget(persistDestMigration, 4, 2);
    advLayout->addWidget(undefineSourceMigration, 5, 0);
    advLayout->addWidget(pausedMigration, 5, 2);
    advLayout->addWidget(fullNonSharedDiskMigration, 6, 0);
    advLayout->addWidget(incNonSharedDiskMigration, 6, 2);
    advLayout->addWidget(unsafeMigration, 7, 0);
    advLayout->addWidget(offlineMigration, 7, 2);
    advLayout->addWidget(compressedMigration, 8, 0);
    advLayout->addWidget(abortOnMigration, 8, 2);
    advanced->setLayout(advLayout);
    flags = new QScrollArea(this);
    flags->setWidget(advanced);
    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(connectivity);
    splitter->addWidget(flags);
    commonLayout->addWidget(splitter);

    helpLinkLabel = new QLabel("<a href='http://libvirt.org/migration.html'>About Migration</a>", this);
    helpLinkLabel->setOpenExternalLinks(true);
    ok = new QPushButton("Migrate", this);
    ok->setIcon(QIcon::fromTheme("migrate"));
    cancel = new QPushButton("Cancel", this);
    cancel->setIcon(QIcon::fromTheme("dialog-cancel"));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(helpLinkLabel);
    buttonLayout->addWidget(cancel);
    buttonLayout->addWidget(ok);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    connect(useAdvanced, SIGNAL(toggled(bool)), this, SLOT(advancedVisibility(bool)));
    connect(ok, SIGNAL(clicked()), this, SLOT(migrateClicked()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(connectList, SIGNAL(currentIndexChanged(int)), this, SLOT(connectChanged(int)));
    connect(bandWdthCheck, SIGNAL(toggled(bool)), this, SLOT(bandWdthVisibility(bool)));
    connect(maxDownCheck, SIGNAL(toggled(bool)), this, SLOT(maxDownTimeVisibility(bool)));
    connect(nativeMigration, SIGNAL(clicked()), this, SLOT(migrTransportChanged()));
    connect(tunnelMigration, SIGNAL(clicked()), this, SLOT(migrTransportChanged()));
    connect(p2pCheck, SIGNAL(clicked(bool)), this, SLOT(p2pChanged(bool)));
    connect(fullNonSharedDiskMigration, SIGNAL(clicked()), this, SLOT(migrDiskRegimeChanged()));
    connect(incNonSharedDiskMigration, SIGNAL(clicked()), this, SLOT(migrDiskRegimeChanged()));
    migrTransportChanged(connectList->currentIndex()==0);
    restoreGeometry(settings.value("MigrationDialogGeometry").toByteArray());
}

/* public slots */
MIGR_ARGS MigrateDialog::getMigrateArgs() const
{
    return migrateArgs;
}

/* private slots */
void MigrateDialog::closeEvent(QCloseEvent *ev)
{
    settings.setValue("MigrationDialogGeometry", saveGeometry());
    done(exitCode);
}
void MigrateDialog::cancelClicked()
{
    exitCode = 0;
    close();
}
void MigrateDialog::migrateClicked()
{
    if ( useAdvanced->isChecked() ) {
        if ( liveMigration->isChecked() )
            m_flags |= VIR_MIGRATE_LIVE;
        if ( p2pCheck->isChecked() )
            m_flags |= VIR_MIGRATE_PEER2PEER;
        if ( tunnelMigration->isChecked() )
            m_flags |= VIR_MIGRATE_TUNNELLED;
        if ( persistDestMigration->isChecked() )
            m_flags |= VIR_MIGRATE_PERSIST_DEST;
        if ( undefineSourceMigration->isChecked() )
            m_flags |= VIR_MIGRATE_UNDEFINE_SOURCE;
        if ( pausedMigration->isChecked() )
            m_flags |= VIR_MIGRATE_PAUSED;
        if ( fullNonSharedDiskMigration->isChecked() )
            m_flags |= VIR_MIGRATE_NON_SHARED_DISK;
        if ( incNonSharedDiskMigration->isChecked() )
            m_flags |= VIR_MIGRATE_NON_SHARED_INC;
        if ( unsafeMigration->isChecked() )
            m_flags |= VIR_MIGRATE_UNSAFE;
        if ( offlineMigration->isChecked() )
            m_flags |= VIR_MIGRATE_OFFLINE;
        if ( compressedMigration->isChecked() )
            m_flags |= VIR_MIGRATE_COMPRESSED;
        if ( abortOnMigration->isChecked() )
            m_flags |= VIR_MIGRATE_ABORT_ON_ERROR;
    };
    migrateArgs.flags = m_flags;
    exitCode = 1;
    // build migrate parameters
    if ( connectList->currentIndex() ) {
        migrateArgs.connName =connectList->currentText().split('\t').first();
    };
    if ( uri->isEnabled() ) {
        migrateArgs.uri = uri->text();
    };
    if ( useAdvanced->isChecked() ) {
        if ( maxDownCheck->isChecked() ) {
             migrateArgs.maxDownTime = maxDownTime->value();
        };
        if ( bandWdthCheck->isChecked() ) {
             migrateArgs.bandwidth = bandwidth->value();
        };
    };
    migrateArgs.new_name = Name->text();
    //qDebug()<<migrateArgs;
    close();
}
void MigrateDialog::advancedVisibility(bool state)
{
    if ( connectList->currentIndex() ) {
        connectivity->setEnabled(state);
    } else {
        connectivity->setEnabled(true);
    };
    advanced->setEnabled(state);
    migrTransportChanged(connectList->currentIndex()==0);
}
void MigrateDialog::connectChanged(int i)
{
    // http://libvirt.org/html/libvirt-libvirt.html#virDomainMigrate
    // http://libvirt.org/html/libvirt-libvirt.html#virDomainMigrateToURI
    bool useDURI = i==0;
    if ( !useDURI ) {
        connectivity->setEnabled(advanced->isEnabled());
        uriLabel->setText("Optional\nHost/URI");
    } else {
        connectivity->setEnabled(useDURI);
        uriLabel->setText("Destination\nHost/URI");
    };
    migrTransportChanged(useDURI);
}
void MigrateDialog::p2pChanged(bool state)
{
    p2p = state;
    migrTransportChanged(connectList->currentIndex()==0);
}
void MigrateDialog::bandWdthVisibility(bool state)
{
    bandwidth->setEnabled(state);
}
void MigrateDialog::maxDownTimeVisibility(bool state)
{
    if ( connType.toLower()!="lxc" ) maxDownTime->setEnabled(state);
}
void MigrateDialog::migrDiskRegimeChanged()
{
    if ( sender()==incNonSharedDiskMigration ) {
        if ( incNonSharedDiskMigration->isChecked() )
            fullNonSharedDiskMigration->setChecked(false);
    } else {
        if ( fullNonSharedDiskMigration->isChecked() )
            incNonSharedDiskMigration->setChecked(false);
    };
}
void MigrateDialog::migrTransportChanged()
{
    if ( tunnelMigration->isChecked() ) {
        p2pCheck->setCheckState(Qt::Checked);
        p2pCheck->setEnabled(false);
    } else {
        p2pCheck->setChecked(p2p);
        p2pCheck->setEnabled(true);
    };
    migrTransportChanged(connectList->currentIndex()==0);
}
void MigrateDialog::migrTransportChanged(bool useDURI)
{
    QString uri_opt, uri_as_seen, example;
    if ( useDURI ) {
        uri_as_seen = "Mandatory URI for the destination host.";
        if ( !useAdvanced->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_5);
            example.append(EXAMPLE_URI_4);
            uri->setPlaceholderText("HV-URI");
        } else if ( tunnelMigration->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_1);
            uri_opt.append(APPEND_URI_HELP_2);
            uri_opt.append(APPEND_URI_HELP_6);
            example.append(EXAMPLE_URI_6);
            uri->setPlaceholderText("[ALT-DEST-LIBVIRT-URI]");
        } else if ( nativeMigration->isChecked() && p2pCheck->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_4);
            uri_opt.append(APPEND_URI_HELP_6);
            example.append(EXAMPLE_URI_5);
            uri->setPlaceholderText("[ALT-DEST-LIBVIRT-URI]");
        } else {
            uri_opt.append(APPEND_URI_HELP_5);
            example.append(EXAMPLE_URI_4);
            uri->setPlaceholderText("HV-URI");
        };
    } else {
        uri_as_seen = "Destination hostname/URI as seen from the source host.";
        if ( !useAdvanced->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_3);
            example.append(EXAMPLE_URI_1);
            uri->setPlaceholderText("[HV-URI]");
        } else if ( tunnelMigration->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_1);
            uri_opt.append(APPEND_URI_HELP_2);
            example.append(EXAMPLE_URI_3);
            uri->setPlaceholderText("");
        } else if ( nativeMigration->isChecked() && p2pCheck->isChecked() ) {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_1);
            uri_opt.append(APPEND_URI_HELP_4);
            example.append(EXAMPLE_URI_2);
            uri->setPlaceholderText("[HV-URI]");
        } else {
            uri_opt.append(APPEND_URI_HELP_0);
            uri_opt.append(APPEND_URI_HELP_3);
            example.append(EXAMPLE_URI_1);
            uri->setPlaceholderText("[HV-URI]");
        };
    };
    helpText->setHtml(QString("%1<br><br>%2<br><br>%3")
                      .arg(uri_as_seen)
                      .arg(uri_opt)
                      .arg(example));
}
