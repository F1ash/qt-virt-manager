#include "security_label.h"

SecurityLabel::SecurityLabel(QWidget *parent) :
    _QWidget(parent)
{
    setObjectName("Security-High");

    modelLabel = new QLabel("Model:", this);
    model = new QComboBox(this);
    model->addItems(QStringList()<<"SELinux"<<"AppArmor"<<"DAC");
    labelTypeLabel = new QComboBox(this);
    labelTypeLabel->addItems(QStringList()<<"Label"<<"BaseLabel"<<"ImageLabel");
    label = new QLineEdit(this);
    relabelLabel = new QLabel("Relabel:", this);
    relabel = new QComboBox(this);
    relabel->addItems(QStringList()<<"Default"<<"Yes"<<"No");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(modelLabel, 0, 0);
    baseLayout->addWidget(model, 0, 1);
    baseLayout->addWidget(labelTypeLabel, 1, 0);
    baseLayout->addWidget(label, 1, 1);
    baseLayout->addWidget(relabelLabel, 2, 0);
    baseLayout->addWidget(relabel, 2, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);

    useSecLabel = new QCheckBox("Use Security Label", this);
    typeLabel = new QLabel("Type:", this);
    type = new QComboBox(this);
    type->addItems(QStringList()<<"None"<<"Dynamic"<<"Static");
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);

    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    list = new QListWidget(this);
    listLayout = new QGridLayout();
    listLayout->addWidget(add, 0, 0);
    listLayout->addWidget(del, 0, 3);
    listLayout->addWidget(list, 1, 0, 2, 4);
    listWdg = new QWidget(this);
    listWdg->setLayout(listLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useSecLabel);
    commonLayout->addWidget(typeWdg);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(listWdg);
    commonLayout->insertStretch(4, -1);
    setLayout(commonLayout);
    usedStateChanged(false);
    connect(useSecLabel, SIGNAL(toggled(bool)),
            this, SLOT(usedStateChanged(bool)));
    connect(type, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(securityTypeChanged(QString)));
    connect(model, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(modelTypeChanged(QString)));
    connect(add, SIGNAL(clicked()),
            this, SLOT(addSecLabel()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delSecLabel()));
}

/* public slots */
QDomDocument SecurityLabel::getDevDocument() const
{
    QDomDocument doc;
    if ( !useSecLabel->isChecked() ) return doc;
    QDomElement _data;
    _data = doc.createElement("data");
    for (int i=0; i<list->count(); i++) {
        QDomDocument _secLabel;
        _secLabel.setContent(list->item(i)->text());
        _data.appendChild(_secLabel);
    };
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}

/* private slots */
void SecurityLabel::usedStateChanged(bool state)
{
    typeLabel->setVisible(state);
    type->setVisible(state);
    listWdg->setVisible(state);
    baseWdg->setVisible( state && type->currentText().toLower()!="none" );
}
void SecurityLabel::securityTypeChanged(QString _type)
{
    relabel->clear();
    baseWdg->setVisible( _type.toLower()!="none" );
    if ( _type.toLower()=="static" ) {
        relabel->addItems(QStringList()<<"Default"<<"Yes"<<"No");
    } else if ( _type.toLower()=="dynamic" ) {
        relabel->addItems(QStringList()<<"Yes");
    };
    label->setPlaceholderText("");
    if ( model->currentText().toLower()=="selinux" ) {
        if ( _type.toLower()=="static" ) {
            label->setPlaceholderText("system_u:system_r:svirt_t:s0:c392,c662");
        } else if ( _type.toLower()=="dynamic" ) {
            label->setPlaceholderText("system_u:system_r:svirt_t:s0");
        };
    } else if ( model->currentText().toLower()=="apparmor" ) {
        label->setPlaceholderText("an AppArmor profile");
    } else if ( model->currentText().toLower()=="dac" ) {
        label->setPlaceholderText("USER:GROUP  or  +UID:GID");
    };
}
void SecurityLabel::modelTypeChanged(QString _type)
{
    label->setPlaceholderText("");
    if ( _type.toLower()=="selinux" ) {
        if ( type->currentText().toLower()=="static" ) {
            label->setPlaceholderText("system_u:system_r:svirt_t:s0:c392,c662");
        } else if ( type->currentText().toLower()=="dynamic" ) {
            label->setPlaceholderText("system_u:system_r:svirt_t:s0");
        };
    } else if ( _type.toLower()=="apparmor" ) {
        label->setPlaceholderText("an AppArmor profile");
    } else if ( _type.toLower()=="dac" ) {
        label->setPlaceholderText("USER:GROUP  or  +UID:GID");
    };
}
QDomDocument SecurityLabel::readData()
{
    QDomText data;
    QDomDocument doc = QDomDocument();
    if ( !useSecLabel->isChecked() ) return doc;
    QDomElement _secLabel, _label;
    QString _t, _r, _m;
    _secLabel= doc.createElement("seclabel");

    _t = type->currentText().toLower();
    if ( _t!="none" ) {
        _m = model->currentText().toLower();
         _secLabel.setAttribute("model", model->currentText().toLower());
        _r = relabel->currentText().toLower();
        if ( _r!="default" ) {
            _secLabel.setAttribute("relabel", _r);
        };
        _label =doc.createElement(labelTypeLabel->currentText().toLower());
        data = doc.createTextNode(label->text());
        _label.appendChild(data);
        _secLabel.appendChild(_label);
    };
    _secLabel.setAttribute("type", _t);
    doc.appendChild(_secLabel);
    return doc;
}
void SecurityLabel::addSecLabel()
{
    QString _list = QString("%1")
            .arg(readData().toByteArray(8).data());
    if ( list->findItems(_list, Qt::MatchExactly).isEmpty() )
        list->addItem(_list);
    label->clear();
}
void SecurityLabel::delSecLabel()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
    };
    list->clearSelection();
}
