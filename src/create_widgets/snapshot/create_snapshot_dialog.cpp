#include "create_snapshot_dialog.h"

/*
 * https://libvirt.org/formatsnapshot.html
 */

#define SNAPSHOT_TYPES QStringList()\
    <<"Memory state (or VM state)"\
    <<"Disk Snapshot + internal form"\
    <<"Disk Snapshot + external form"\
    <<"System Checkpoint + internal form"\
    <<"System Checkpoint + external form"

CreateSnapshotDialog::CreateSnapshotDialog(
        QWidget *parent, QString domainName,
        bool _state, virConnectPtr *connPtr) :
    QDialog(parent)
{
    QString winTitle = QString("Create Snapshot <%1>").arg(domainName);
    setWindowTitle(winTitle);
    settings.beginGroup("CreateSnapshotDialog");
    restoreGeometry( settings.value("Geometry").toByteArray() );
    settings.endGroup();
    titleLayout = new QHBoxLayout(this);
    nameLabel = new QLabel("Name:", this);
    name = new QLineEdit(this);
    name->setMinimumWidth(100);
    addTimeSuff = new QCheckBox(this);
    addTimeSuff->setToolTip("Add Time to Snapshot Name");
    timeLabel = new QLabel(this);
    QString _date(QTime::currentTime().toString());
    _date.append("-");
    _date.append(QDate::currentDate().toString("dd.MM.yyyy"));
    timeLabel->setText(_date);
    timeLabel->setEnabled(false);
    titleLayout->addWidget(nameLabel);
    titleLayout->addWidget(name);
    titleLayout->addWidget(addTimeSuff);
    titleLayout->addWidget(timeLabel);
    titleWdg = new QWidget(this);
    titleWdg->setLayout(titleLayout);
    description = new QLineEdit(this);
    description->setPlaceholderText("Short Description");
    snapshotType = new QComboBox(this);
    snapshotType->addItems(SNAPSHOT_TYPES);
    flagsMenu = new CreateSnapshotFlags(this);
    flags = new QPushButton(QIcon::fromTheme("flag"), "", this);
    flags->setMenu(flagsMenu);
    flags->setMaximumWidth(flags->sizeHint().width());
    flags->setToolTip("Create Snapshot Flags");
    // because first item is non-actual there
    flags->setEnabled(false);
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(snapshotType);
    typeLayout->addWidget(flags);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);
    baseWdg = new QStackedWidget(this);
    baseWdg->addWidget(new MemStateSnapshot(this, _state));
    baseWdg->addWidget(new DiskSnapshot(this, _state, false));
    baseWdg->addWidget(new DiskSnapshot(this, _state, true));
    baseWdg->addWidget(new SystemCheckpoint(this, _state, false));
    baseWdg->addWidget(new SystemCheckpoint(this, _state, true));
    info = new QLabel("<a href='https://libvirt.org/formatsnapshot.html'>About</a>", this);
    info->setOpenExternalLinks(true);
    info->setToolTip("https://libvirt.org/formatsnapshot.html");
    ok = new QPushButton("Ok", this);
    // because first item is non-actual there
    ok->setEnabled(false);
    cancel = new QPushButton("Cancel", this);
    buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(info);
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    buttonsWdg = new QWidget(this);
    buttonsWdg->setLayout(buttonsLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(titleWdg);
    commonLayout->addWidget(description);
    commonLayout->addWidget(typeWdg);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(buttonsWdg);
    //commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(snapshotType, SIGNAL(currentIndexChanged(int)),
            baseWdg, SLOT(setCurrentIndex(int)));
    connect(snapshotType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(snapshotTypeChange(int)));
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(addTimeSuff, SIGNAL(toggled(bool)),
            timeLabel, SLOT(setEnabled(bool)));
    for (int i=0; i<baseWdg->count(); i++) {
        _SnapshotStuff *wdg = static_cast<_SnapshotStuff*>(
                    baseWdg->widget(i));
        if ( NULL!=wdg ) wdg->setParameters(connPtr, domainName);
        connect(wdg, SIGNAL(errMsg(QString&)),
                this, SIGNAL(errMsg(QString&)));
    };
    timerID = startTimer(1000);
}

/* public slots */
QString CreateSnapshotDialog::getSnapshotXMLDesc() const
{
    QDomDocument doc, _doc;
    QDomElement domainsnapshot, _name, _desc;
    domainsnapshot = doc.createElement("domainsnapshot");
    doc.appendChild(domainsnapshot);
    if ( !name->text().isEmpty() ) {
        _name = doc.createElement("name");
        domainsnapshot.appendChild(_name);
        QString text = name->text();
        if ( addTimeSuff->isChecked() ) {
            text.append("_");
            text.append(timeLabel->text());
        };
        QDomText _text = doc.createTextNode(text);
        _name.appendChild(_text);
    };
    if ( !description->text().isEmpty() ) {
        _desc = doc.createElement("description");
        domainsnapshot.appendChild(_desc);
        QDomText _text = doc.createTextNode(description->text());
        _desc.appendChild(_text);
    };
    _SnapshotStuff *wdg = static_cast<_SnapshotStuff*>(baseWdg->currentWidget());
    if ( NULL!=wdg ) {
        _doc = wdg->getElements();
        QDomNodeList _nodeList = _doc.childNodes();
        int count = _nodeList.count();
        for(int i=0; i<count; i++) {
            QDomNode _node = _nodeList.item(0);
            domainsnapshot.appendChild(_node);
        };
    };
    //qDebug()<<doc.toByteArray(4).data();
    return doc.toString();
}
uint CreateSnapshotDialog::getSnapshotFlags() const
{
    return flagsMenu->getCompositeFlag();
}

/* private slots */
void CreateSnapshotDialog::accept()
{
    _SnapshotStuff *wdg = static_cast<_SnapshotStuff*>(baseWdg->currentWidget());
    QDomElement _node = wdg->getElements()
            .firstChildElement("disks")
            .firstChildElement("disk");
    if ( _node.childNodes().count() ) {
        // count of disk subset not can be equal zero
        QMessageBox::warning(
                    this,
                    QString(),
                    QString("Count of disk subset not can be equal zero"));
    } else {
        killTimer(timerID);
        settings.beginGroup("CreateSnapshotDialog");
        settings.setValue("Geometry", saveGeometry());
        settings.endGroup();
        done(1);
    };
}
void CreateSnapshotDialog::reject()
{
    killTimer(timerID);
    settings.beginGroup("CreateSnapshotDialog");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();
    done(0);
}
void CreateSnapshotDialog::snapshotTypeChange(int i)
{
    ok->setDisabled( i==0 );
    flags->setDisabled( i==0 );
    flagsMenu->changeAvailableFlags(i);
}
void CreateSnapshotDialog::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==timerID ) {
        QString _date(QTime::currentTime().toString());
        _date.append("-");
        _date.append(QDate::currentDate().toString("dd.MM.yyyy"));
        timeLabel->setText(_date);
    }
}
