#include "migrate_dialog.h"

mgrHelpThread::mgrHelpThread(
            QObject         *parent,
            virConnectPtr   *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{
}
void mgrHelpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    hostName.append(virConnectGetHostname(*ptr_ConnPtr));
    connType.append(virConnectGetType(*ptr_ConnPtr));
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

MigrateDialog::MigrateDialog(
            QWidget         *parent,
            QString          _domain,
            virConnectPtr   *connPtrPtr) :
    QDialog(parent), domainName(_domain)
{
    setWindowTitle(QString(tr("Migrate '%1'")).arg(domainName));
    commonLayout = new QVBoxLayout();
    Name = new QLineEdit(this);
    Name->setText(domainName);
    Name->setPlaceholderText(tr("Enter new VM name"));
    Name->setToolTip(tr("migrate as <Name>"));
    host = new QLabel(this);
    newHost = new QLabel(tr("Destination Host / Connection :"), this);
    connectList = new QComboBox(this);
    commonLayout->addWidget(host);
    commonLayout->addWidget(Name);
    commonLayout->addWidget(newHost);
    commonLayout->addWidget(connectList);

    connLayout = new QGridLayout();
    uriLabel = new QLabel(tr("Destination\nHost/URI"), this);
    uri = new QLineEdit(this);
    help = new QLabel(this);
    QIcon icon = QIcon::fromTheme("dialog-warning");
    if ( icon.isNull() ) {
        help->setText(tr("Help:"));
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

    useAdvanced = new QCheckBox(tr("Advanced"), this);
    useAdvanced->setChecked(false);
    commonLayout->addWidget(useAdvanced);
    //commonLayout->addWidget(connectivity);
    commonLayout->insertStretch(-1);

    advanced = new QWidget(this);
    advanced->setEnabled(false);
    advLayout = new QGridLayout();
    nativeMigration = new QRadioButton(
                tr("Native migration data\nover hypervisor transport\n\
(encrypt if HV support it)"),
                this);
    nativeMigration->setChecked(true);
    nativeMigration->setLayoutDirection(Qt::RightToLeft);
    tunnelMigration = new QRadioButton(
                tr("Tunnel migration data\nover libvirtd connection\n(encrypt always)"),
                this);
    tunnelMigration->setLayoutDirection(Qt::LeftToRight);
    p2pMigration = new QLabel(tr("Use peer2peer"), this);
    maxDownTimeLabel = new QLabel(
                tr("Maximum tolerable downtime\nfor live migration (ms)"),
                this);
    p2pCheck = new QCheckBox(this);
    p2pCheck->setChecked(p2p);
    maxDownCheck = new QCheckBox(this);
    maxDownCheck->setChecked(false);
    maxDownTime = new QSpinBox(this);
    maxDownTime->setEnabled(false);
    maxDownTime->setValue(30);
    bandWdthLabel = new QLabel(tr("BandWidth (MiB/s)"), this);
    bandWdthCheck = new QCheckBox(this);
    bandWdthCheck->setChecked(false);
    bandwidth = new QSpinBox(this);
    bandwidth->setEnabled(false);
    liveMigration = new QCheckBox(tr("Live migration"), this);
    liveMigration->setLayoutDirection(Qt::RightToLeft);
    persistDestMigration = new QCheckBox(
                tr("Persist the VM\non the destination"),
                this);
    persistDestMigration->setLayoutDirection(Qt::LeftToRight);
    undefineSourceMigration = new QCheckBox(
                tr("Undefine the VM\non the source"),
                this);
    undefineSourceMigration->setLayoutDirection(Qt::RightToLeft);
    pausedMigration = new QCheckBox(
                tr("Leave\nthe domain suspended\non the remote side"),
                this);
    pausedMigration->setLayoutDirection(Qt::LeftToRight);
    fullNonSharedDiskMigration = new QCheckBox(
                tr("Migration with\nnon-shared storage\nwith full disk copy"),
                this);
    fullNonSharedDiskMigration->setLayoutDirection(Qt::RightToLeft);
    incNonSharedDiskMigration = new QCheckBox(
                tr("Migration with\nnon-shared storage\nwith incremental copy"),
                this);
    incNonSharedDiskMigration->setLayoutDirection(Qt::LeftToRight);
    unsafeMigration = new QCheckBox(
                tr("Force migration even\nif it is considered unsafe"),
                this);
    unsafeMigration->setLayoutDirection(Qt::RightToLeft);
    offlineMigration = new QCheckBox(tr("Migrate offline"), this);
    offlineMigration->setLayoutDirection(Qt::LeftToRight);
    compressedMigration = new QCheckBox(
                tr("Compress data\nduring migration"),
                this);
    compressedMigration->setLayoutDirection(Qt::RightToLeft);
    abortOnMigration = new QCheckBox(
                tr("Abort migration\non I/O errors happened\nduring migration"),
                this);
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

    helpLinkLabel = new QLabel(
                tr("<a href='http://libvirt.org/migration.html'>About Migration</a>"),
                this);
    helpLinkLabel->setOpenExternalLinks(true);
    ok = new QPushButton(tr("Migrate"), this);
    ok->setIcon(QIcon::fromTheme("migrate"));
    cancel = new QPushButton(tr("Cancel"), this);
    cancel->setIcon(QIcon::fromTheme("dialog-cancel"));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(helpLinkLabel);
    buttonLayout->addWidget(cancel);
    buttonLayout->addWidget(ok);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    connect(useAdvanced, SIGNAL(toggled(bool)),
            this, SLOT(advancedVisibility(bool)));
    connect(ok, SIGNAL(clicked()),
            this, SLOT(migrateClicked()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
    connect(connectList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(connectChanged(int)));
    connect(bandWdthCheck, SIGNAL(toggled(bool)),
            this, SLOT(bandWdthVisibility(bool)));
    connect(maxDownCheck, SIGNAL(toggled(bool)),
            this, SLOT(maxDownTimeVisibility(bool)));
    connect(nativeMigration, SIGNAL(clicked()),
            this, SLOT(migrTransportChanged()));
    connect(tunnelMigration, SIGNAL(clicked()),
            this, SLOT(migrTransportChanged()));
    connect(p2pCheck, SIGNAL(clicked(bool)),
            this, SLOT(p2pChanged(bool)));
    connect(fullNonSharedDiskMigration, SIGNAL(clicked()),
            this, SLOT(migrDiskRegimeChanged()));
    connect(incNonSharedDiskMigration, SIGNAL(clicked()),
            this, SLOT(migrDiskRegimeChanged()));
    migrTransportChanged(connectList->currentIndex()==0);
    restoreGeometry(
                settings
                .value("MigrationDialogGeometry")
                .toByteArray());
    hlpThread = new mgrHelpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(fillData()));
    hlpThread->start();
}

/* public slots */
MIGR_ARGS MigrateDialog::getMigrateArgs() const
{
    return migrateArgs;
}

/* private slots */
void MigrateDialog::fillData()
{
    QStringList connList;
    connList.prepend(tr("Set Host/URI manually"));
    settings.beginGroup("Connects");
    foreach (QString conn, settings.childGroups()) {
        settings.beginGroup(conn);
        connList.append(QString("%1\t(%2)")
                    .arg(conn)
                    .arg(settings.value("Driver").toString()));
        settings.endGroup();
    };
    settings.endGroup();
    host->setText(
                QString(tr("Original Host / Connection Type : %1 / %2"))
                .arg(hlpThread->hostName)
                .arg(hlpThread->connType.toUpper()));
    connectList->addItems(connList);
    maxDownTimeLabel->setEnabled(
                hlpThread->connType.toLower()!="lxc" );
    maxDownCheck->setEnabled(
                hlpThread->connType.toLower()!="lxc" );
}
void MigrateDialog::closeEvent(QCloseEvent *ev)
{
    Q_UNUSED(ev)
    settings.setValue("MigrationDialogGeometry", saveGeometry());
    done(0);
}
void MigrateDialog::cancelClicked()
{
    done(0);
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
    // build migrate parameters
    if ( connectList->currentIndex() ) {
        migrateArgs.connName =connectList->currentText()
                .split('\t').first();
    };
    if ( uri->isEnabled() ) {
        migrateArgs.uri = uri->text();
    };
    if ( useAdvanced->isChecked() ) {
        if ( maxDownCheck->isChecked() ) {
             migrateArgs.maxDownTime = maxDownTime->text().toULongLong();
        };
        if ( bandWdthCheck->isChecked() ) {
             migrateArgs.bandwidth = bandwidth->text().toULong();
        };
    };
    migrateArgs.new_name = Name->text();
    done(1);
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
        uriLabel->setText(tr("Optional\nHost/URI"));
    } else {
        connectivity->setEnabled(useDURI);
        uriLabel->setText(tr("Destination\nHost/URI"));
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
    if ( hlpThread->connType.toLower()!="lxc" )
        maxDownTime->setEnabled(state);
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
        uri_as_seen = tr("Mandatory URI for the destination host.");
        if ( !useAdvanced->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_5());
            example.append(_HELP::EXAMPLE_URI_4());
            uri->setPlaceholderText("HV-URI");
        } else if ( tunnelMigration->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_1());
            uri_opt.append(_HELP::APPEND_URI_HELP_2());
            uri_opt.append(_HELP::APPEND_URI_HELP_6());
            example.append(_HELP::EXAMPLE_URI_6());
            uri->setPlaceholderText("[ALT-DEST-LIBVIRT-URI]");
        } else if ( nativeMigration->isChecked() && p2pCheck->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_4());
            uri_opt.append(_HELP::APPEND_URI_HELP_6());
            example.append(_HELP::EXAMPLE_URI_5());
            uri->setPlaceholderText("[ALT-DEST-LIBVIRT-URI]");
        } else {
            uri_opt.append(_HELP::APPEND_URI_HELP_5());
            example.append(_HELP::EXAMPLE_URI_4());
            uri->setPlaceholderText("HV-URI");
        };
    } else {
        uri_as_seen = tr("Destination hostname/URI as seen from the source host.");
        if ( !useAdvanced->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_3());
            example.append(_HELP::EXAMPLE_URI_1());
            uri->setPlaceholderText("[HV-URI]");
        } else if ( tunnelMigration->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_1());
            uri_opt.append(_HELP::APPEND_URI_HELP_2());
            example.append(_HELP::EXAMPLE_URI_3());
            uri->setPlaceholderText("");
        } else if ( nativeMigration->isChecked() && p2pCheck->isChecked() ) {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_1());
            uri_opt.append(_HELP::APPEND_URI_HELP_4());
            example.append(_HELP::EXAMPLE_URI_2());
            uri->setPlaceholderText("[HV-URI]");
        } else {
            uri_opt.append(_HELP::APPEND_URI_HELP_0());
            uri_opt.append(_HELP::APPEND_URI_HELP_3());
            example.append(_HELP::EXAMPLE_URI_1());
            uri->setPlaceholderText("[HV-URI]");
        };
    };
    helpText->setHtml(QString("%1<br><br>%2<br><br>%3")
                      .arg(uri_as_seen)
                      .arg(uri_opt)
                      .arg(example));
}
