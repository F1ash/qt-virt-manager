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
        QWidget *parent, QString domainName, bool _state) :
    QDialog(parent)
{
    QString winTitle = QString("Create Snapshot <%1>").arg(domainName);
    setWindowTitle(winTitle);
    titleLayout = new QHBoxLayout(this);
    nameLabel = new QLabel("Name:", this);
    name = new QLineEdit(this);
    name->setMinimumWidth(100);
    addTimeSuff = new QCheckBox(this);
    addTimeSuff->setToolTip("Add Time to Snapsot Name");
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
    baseWdg = new QStackedWidget(this);
    baseWdg->addWidget(new MemStateSnapshot(this, _state));
    baseWdg->addWidget(new DiskSnapshot(this, _state, false));
    baseWdg->addWidget(new DiskSnapshot(this, _state, true));
    baseWdg->addWidget(new SystemCheckpoint(this, _state, false));
    baseWdg->addWidget(new SystemCheckpoint(this, _state, true));
    info = new QLabel("<a href='https://libvirt.org/formatsnapshot.html'>About</a>", this);
    info->setOpenExternalLinks(true);
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
    commonLayout->addWidget(snapshotType);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(buttonsWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(snapshotType, SIGNAL(currentIndexChanged(int)),
            baseWdg, SLOT(setCurrentIndex(int)));
    connect(snapshotType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(snapshotTypeChange(int)));
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(addTimeSuff, SIGNAL(toggled(bool)),
            timeLabel, SLOT(setEnabled(bool)));
    timerID = startTimer(1000);
}

/* public slots */
char* CreateSnapshotDialog::getSnapshotXMLDesc() const
{
    QDomDocument doc;
    QDomElement domainsnapshot, _name, _desc, _node;
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
    _node = wdg->getElements().firstChildElement("memory");
    if ( !_node.isNull() ) domainsnapshot.appendChild(_node);
    _node = wdg->getElements().firstChildElement("disks");
    if ( !_node.isNull() ) domainsnapshot.appendChild(_node);
    qDebug()<<doc.toByteArray(4).data();
    return doc.toByteArray(4).data();
}
QString CreateSnapshotDialog::getSnapshotType() const
{
    _SnapshotStuff *wdg = static_cast<_SnapshotStuff*>(baseWdg->currentWidget());
    return QString::number( wdg->type );
}

/* private slots */
void CreateSnapshotDialog::accept()
{
    killTimer(timerID);
    done(1);
}
void CreateSnapshotDialog::reject()
{
    killTimer(timerID);
    done(0);
}
void CreateSnapshotDialog::snapshotTypeChange(int i)
{
    ok->setDisabled( i==0 );
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
