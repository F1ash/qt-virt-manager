#include "general.h"

General::General(QWidget *parent, QString _caps, QString _xmlDesc) :
    _Tab(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("Domain");
    readCapabilities();
    typeLabel = new QLabel(QString("VM Type: %1").arg(type.toUpper()), this);
    archLabel = new QLabel(QString("Host Arch: %1").arg(arch), this);
    nameLabel = new QLabel("Name:", this);
    name = new QLineEdit(this);
    name->setAlignment(Qt::AlignRight);
    name->setPlaceholderText("Enter VM name");
    uuidLabel = new QLabel("UUID:", this);
    uuid = new QLineEdit(this);
    uuid->setAlignment(Qt::AlignRight);
    uuid->setPlaceholderText("auto-generate if omitted");
    titleLabel = new QLabel("Title:", this);
    title = new QLineEdit(this);
    title->setAlignment(Qt::AlignRight);
    title->setPlaceholderText("Enter VM title");
    descLabel = new QLabel("Description:", this);
    description = new QTextEdit(this);
    restorePanel = new RestorePanel(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    commonLayout->addWidget(typeLabel);
    commonLayout->addWidget(archLabel);
    commonLayout->addWidget(nameLabel);
    commonLayout->addWidget(name);
    commonLayout->addWidget(uuidLabel);
    commonLayout->addWidget(uuid);
    commonLayout->addWidget(titleLabel);
    commonLayout->addWidget(title);
    commonLayout->addWidget(descLabel);
    commonLayout->addWidget(description);
    setLayout(commonLayout);
    readXMLDesciption();
    // dataChanged connections
    connect(name, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(uuid, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(title, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(description, SIGNAL(textChanged()),
            this, SLOT(stateChanged()));
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveData()));
}

/* public slots */
QDomDocument General::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _name, _uuid, _title, _desc, _data;
    _data = doc.createElement("data");
    _name= doc.createElement("name");
    data = doc.createTextNode(name->text());
    _name.appendChild(data);
    _data.appendChild(_name);

    _uuid= doc.createElement("uuid");
    data = doc.createTextNode(uuid->text());
    _uuid.appendChild(data);
    _data.appendChild(_uuid);

    _title= doc.createElement("title");
    data = doc.createTextNode(title->text());
    _title.appendChild(data);
    _data.appendChild(_title);

    _desc= doc.createElement("description");
    data = doc.createTextNode(description->toPlainText());
    _desc.appendChild(data);
    _data.appendChild(_desc);

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void General::changeArch(QString &_arch)
{
    type = _arch;
    typeLabel->setText(QString("VM Type: %1").arg(type.toUpper()));
}

/* private slots */
void General::readCapabilities()
{
    QDomDocument doc;
    doc.setContent(capabilities);
    QDomElement _domain = doc.
            firstChildElement("capabilities").
            firstChildElement("guest").
            firstChildElement("arch").
            firstChildElement("domain");
    if ( !_domain.isNull() ) {
        type = _domain.attribute("type");
    };
    arch = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("cpu").
               firstChildElement("arch").
               firstChild().toText().data();
    os_type = doc.firstChildElement("capabilities").
               firstChildElement("guest").
               firstChildElement("os_type").
               firstChild().toText().data();
}
void General::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void General::readXMLDesciption(QString &_xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _domain = doc.firstChildElement("domain");
    name->setText(
                _domain
                .firstChildElement("name")
                .firstChild().toText().data());
    uuid->setText(
                _domain
                .firstChildElement("uuid")
                .firstChild().toText().data());
    title->setText(
                _domain
                .firstChildElement("title")
                .firstChild().toText().data());
    description->setText(
                _domain
                .firstChildElement("description")
                .firstChild().toText().data());
}
