#include "general.h"

General::General(QWidget *parent, QString str) :
    _QWidget(parent), type(str)
{
    setObjectName("Domain");
    typeLabel = new QLabel(QString("Type: %1").arg(type.toLower()), this);
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
QDomNodeList General::getNodeList() const
{
    QDomText data;
    QDomDocument doc = QDomDocument();
    QDomElement _name, _uuid, _title, _desc;
    _name= doc.createElement("name");
    data = doc.createTextNode(name->text());
    _name.appendChild(data);
    doc.appendChild(_name);

    _uuid= doc.createElement("uuid");
    data = doc.createTextNode(uuid->text());
    _uuid.appendChild(data);
    doc.appendChild(_uuid);

    _title= doc.createElement("title");
    data = doc.createTextNode(title->text());
    _title.appendChild(data);
    doc.appendChild(_title);

    _desc= doc.createElement("description");
    data = doc.createTextNode(description->toPlainText());
    _desc.appendChild(data);
    doc.appendChild(_desc);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
