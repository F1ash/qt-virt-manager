#include "lxc_os_booting.h"

LXC_OSBooting::LXC_OSBooting(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    setObjectName("Booting");
    initPathLabel = new QLabel("Init path (Ex.: /sbin/init, /bin/sh, etc.) and arguments:", this);
    initPath = new QLineEdit(this);
    initPath->setPlaceholderText("Enter Init Path");
    nameSpaceEnable = new QCheckBox("Enable User Namespace", this);
    nameSpaceEnable->setChecked(false);
    uidStart = new QLabel("0", this);
    gidStart = new QLabel("0", this);
    startLabel = new QLabel("Start", this);
    targetLabel = new QLabel("Target", this);
    countLabel = new QLabel("Count", this);
    uidLabel = new QLabel("User ID", this);
    gidLabel = new QLabel("Group ID", this);
    uidTarget = new QSpinBox(this);
    uidTarget->setRange(0, 65535);
    uidTarget->setValue(1000);
    gidTarget = new QSpinBox(this);
    gidTarget->setRange(0, 65535);
    gidTarget->setValue(1000);
    uidCount = new QSpinBox(this);
    uidCount->setRange(0, 65535);
    uidCount->setValue(10);
    gidCount = new QSpinBox(this);
    gidCount->setRange(0, 65535);
    gidCount->setValue(10);
    nameSpaceLayout = new QGridLayout();
    nameSpaceLayout->addWidget(startLabel,  0, 1, Qt::AlignCenter);
    nameSpaceLayout->addWidget(targetLabel, 0, 2, Qt::AlignCenter);
    nameSpaceLayout->addWidget(countLabel,  0, 3, Qt::AlignCenter);
    nameSpaceLayout->addWidget(uidLabel,    1, 0, Qt::AlignRight );
    nameSpaceLayout->addWidget(uidStart,    1, 1, Qt::AlignCenter);
    nameSpaceLayout->addWidget(uidTarget,   1, 2, Qt::AlignCenter);
    nameSpaceLayout->addWidget(uidCount,    1, 3, Qt::AlignCenter);
    nameSpaceLayout->addWidget(gidLabel,    2, 0, Qt::AlignRight );
    nameSpaceLayout->addWidget(gidStart,    2, 1, Qt::AlignCenter);
    nameSpaceLayout->addWidget(gidTarget,   2, 2, Qt::AlignCenter);
    nameSpaceLayout->addWidget(gidCount,    2, 3, Qt::AlignCenter);
    nameSpaceWidget = new QWidget(this);
    nameSpaceWidget->setLayout(nameSpaceLayout);
    nameSpaceWidget->setEnabled(false);
    architecture = new _Arch(this, capabilities);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(architecture);
    commonLayout->addWidget(initPathLabel);
    commonLayout->addWidget(initPath);
    commonLayout->addWidget(nameSpaceEnable);
    commonLayout->addWidget(nameSpaceWidget);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(nameSpaceEnable, SIGNAL(toggled(bool)),
            nameSpaceWidget, SLOT(setEnabled(bool)));
    connect(architecture, SIGNAL(domainType(QString&)),
            this, SIGNAL(domainType(QString&)));
    connect(architecture, SIGNAL(osType(QString&)),
            this, SIGNAL(osType(QString&)));
    connect(architecture, SIGNAL(archType(QString&)),
            this, SLOT(changeArch(QString&)));
    connect(architecture, SIGNAL(emulatorType(QString&)),
            this, SIGNAL(emulatorType(QString&)));
    architecture->setItems();
    // dataChanged connectins
    connect(architecture, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(initPath, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(nameSpaceEnable, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(uidTarget, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(uidCount, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(gidTarget, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(gidCount, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument LXC_OSBooting::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _os, _type, _initElem,
            _idmap, _uid, _gid, _data;
    _data = doc.createElement("data");
    _os = doc.createElement("os");
    _data.appendChild(_os);

    _type = doc.createElement("type");
    _type.setAttribute("arch", arch);
    _os.appendChild(_type);

    QStringList cmd = initPath->text().split(" ");
    if ( cmd.length() && !cmd.first().isEmpty() ) {
        for ( int i = 0; i<cmd.length(); i++ ) {
            QString tagName;
            if ( i == 0 ) tagName = "init";
            else tagName = "initarg";
            _initElem = doc.createElement(tagName);
            data = doc.createTextNode(cmd.at(i));
            _initElem.appendChild(data);
            _os.appendChild(_initElem);
            //qDebug()<<tagName<<cmd.at(i);
        };
    };

    if ( nameSpaceEnable->isChecked() ) {
        _idmap = doc.createElement("idmap");
        _data.appendChild(_idmap);
        _uid = doc.createElement("uid");
        _uid.setAttribute("start", uidStart->text());
        _uid.setAttribute("target", QString("%1").arg(uidTarget->value()));
        _uid.setAttribute("count",QString("%1").arg( uidCount->value()));
        _idmap.appendChild(_uid);
        _gid = doc.createElement("gid");
        _gid.setAttribute("start", gidStart->text());
        _gid.setAttribute("target", QString("%1").arg(gidTarget->value()));
        _gid.setAttribute("count", QString("%1").arg(gidCount->value()));
        _idmap.appendChild(_gid);
    };
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void LXC_OSBooting::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _os, _type, _init, _initarg,
            _idmap, _uid, _gid;
    _os = doc
            .firstChildElement("domain")
            .firstChildElement("os");
    _type = _os.firstChildElement("type");
    _init = _os.firstChildElement("init");
    _idmap = doc
            .firstChildElement("domain")
            .firstChildElement("idmap");
    QString _attr;
    if ( !_init.isNull() ) {
        _attr.append(
                    _init.firstChild().toText().data());
        _initarg = _os.firstChildElement("initarg");
        while ( !_initarg.isNull() ) {
            _attr.append(" ");
            _attr.append(
                        _initarg.firstChild().toText().data());
            _initarg = _initarg.nextSiblingElement("initarg");
        };
        initPath->setText(_attr);
    };
    nameSpaceEnable->setChecked( !_idmap.isNull() );
    if ( !_idmap.isNull() ) {
        _uid = _idmap.firstChildElement("uid");
        _gid = _idmap.firstChildElement("gid");
        if ( !_uid.isNull() ) {
            uidTarget->setValue(
                        _uid.attribute("target").toInt());
            uidCount->setValue(
                        _uid.attribute("count").toInt());
        };
        if ( !_gid.isNull() ) {
            gidTarget->setValue(
                        _gid.attribute("target").toInt());
            gidCount->setValue(
                        _gid.attribute("count").toInt());
        };
    };
    if ( !_type.isNull() ) {
        _attr = _type.attribute("arch");
        architecture->setArch(_attr);
    };
}
void LXC_OSBooting::setInitState()
{
    if ( this->isEnabled() ) architecture->setItems();
}

/* private slots */
void LXC_OSBooting::changeArch(QString &_arch)
{
    arch = _arch;
}
