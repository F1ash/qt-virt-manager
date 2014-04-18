#include "os_booting.h"

LXC_OSBooting::LXC_OSBooting(QWidget *parent) :
    _QWidget(parent)
{
    setObjectName("Booting");
    arch = new QLabel(this);
    initPathLabel = new QLabel("Init path (Ex.: /sbin/init, /bin/sh, etc.):", this);
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
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(initPathLabel);
    commonLayout->addWidget(initPath);
    commonLayout->addWidget(nameSpaceEnable);
    commonLayout->addWidget(nameSpaceWidget);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(nameSpaceEnable, SIGNAL(toggled(bool)), nameSpaceWidget, SLOT(setEnabled(bool)));
}
LXC_OSBooting::~LXC_OSBooting()
{
    disconnect(nameSpaceEnable, SIGNAL(toggled(bool)), nameSpaceWidget, SLOT(setEnabled(bool)));
    delete arch;
    arch = 0;
    delete initPathLabel;
    initPathLabel = 0;
    delete initPath;
    initPath = 0;
    delete nameSpaceEnable;
    nameSpaceEnable = 0;
    delete startLabel;
    startLabel = 0;
    delete targetLabel;
    targetLabel = 0;
    delete countLabel;
    countLabel = 0;
    delete uidStart;
    uidStart = 0;
    delete gidStart;
    gidStart = 0;
    delete uidLabel;
    uidLabel = 0;
    delete gidLabel;
    gidLabel = 0;
    delete uidTarget;
    uidTarget = 0;
    delete gidTarget;
    gidTarget = 0;
    delete uidCount;
    uidCount = 0;
    delete gidCount;
    gidCount = 0;
    delete nameSpaceLayout;
    nameSpaceLayout = 0;
    delete nameSpaceWidget;
    nameSpaceWidget = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomNodeList LXC_OSBooting::getNodeList() const
{
    QDomText data;
    QDomDocument doc = QDomDocument();
    QDomElement os, type, initElem, idmap, uid, gid;

    os= doc.createElement("os");
    doc.appendChild(os);

    type= doc.createElement("type");
    type.setAttribute("arch", arch->text());
    data = doc.createTextNode("exe");
    type.appendChild(data);
    os.appendChild(type);

    QStringList cmd = initPath->text().split(" ");
    if ( cmd.length() && !cmd.first().isEmpty() ) {
        for ( int i = 0; i<cmd.length(); i++ ) {
            QString tagName;
            if ( i == 0 ) tagName = "init";
            else tagName = "initarg";
            initElem = doc.createElement(tagName);
            data = doc.createTextNode(cmd.at(i));
            initElem.appendChild(data);
            os.appendChild(initElem);
            //qDebug()<<tagName<<cmd.at(i);
        };
    };

    if ( nameSpaceEnable->isChecked() ) {
        idmap= doc.createElement("idmap");
        doc.appendChild(idmap);
        uid= doc.createElement("uid");
        uid.setAttribute("start", uidStart->text());
        uid.setAttribute("target", QString("%1").arg(uidTarget->value()));
        uid.setAttribute("count",QString("%1").arg( uidCount->value()));
        idmap.appendChild(uid);
        gid= doc.createElement("gid");
        gid.setAttribute("start", gidStart->text());
        gid.setAttribute("target", QString("%1").arg(gidTarget->value()));
        gid.setAttribute("count", QString("%1").arg(gidCount->value()));
        idmap.appendChild(gid);
    };

    //qDebug()<<doc.toString();
    return doc.childNodes();
}

/* private slots */

