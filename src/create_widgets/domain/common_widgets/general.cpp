#include "general.h"

General::General(QWidget *parent, QString arg1, QString arg2, QString arg3) :
    _QWidget(parent), type(arg1), arch(arg2), emulator(arg3)
{
    setObjectName("Domain");
    typeLabel = new QLabel(QString("Type: %1").arg(type.toLower()), this);
    archLabel = new QLabel(QString("Arch: %1").arg(arch), this);
    emulLabel = new QLabel(QString("Emulator: %1").arg(emulator), this);
    nameLabel = new QLabel("Name:", this);
    name = new QLineEdit(this);
    name->setPlaceholderText("Enter VM name");
    uuidLabel = new QLabel("UUID:", this);
    uuid = new QLabel("", this);
    titleLabel = new QLabel("Title:", this);
    title = new QLineEdit(this);
    title->setPlaceholderText("Enter VM title");
    descLabel = new QLabel("Description:", this);
    description = new QTextEdit(this);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(typeLabel);
    commonLayout->addWidget(archLabel);
    commonLayout->addWidget(emulLabel);
    commonLayout->addWidget(nameLabel);
    commonLayout->addWidget(name);
    commonLayout->addWidget(uuidLabel);
    commonLayout->addWidget(uuid);
    commonLayout->addWidget(titleLabel);
    commonLayout->addWidget(title);
    commonLayout->addWidget(descLabel);
    commonLayout->addWidget(description);
    setLayout(commonLayout);
}
General::~General()
{
    delete typeLabel;
    typeLabel = 0;
    delete archLabel;
    archLabel = 0;
    delete emulLabel;
    emulLabel = 0;
    delete nameLabel;
    nameLabel = 0;
    delete name;
    name = 0;
    delete uuidLabel;
    uuidLabel = 0;
    delete uuid;
    uuid = 0;
    delete titleLabel;
    titleLabel = 0;
    delete title;
    title = 0;
    delete descLabel;
    descLabel = 0;
    delete description;
    description = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomDocument General::getDevDocument() const
{
    QDomText data;
    QDomDocument doc = QDomDocument();
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
