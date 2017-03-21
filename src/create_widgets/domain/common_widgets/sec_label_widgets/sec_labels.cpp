#include "sec_labels.h"

SecLabels::SecLabels(QWidget *parent) :
    _QWidget(parent)
{
    useSecLabel = new QCheckBox("Use Security Label", this);
    typeLabel = new QLabel("Type:", this);
    type = new QComboBox(this);
    type->addItems(QStringList()<<"None"<<"Dynamic"<<"Static");
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);

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

    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    list = new QListWidget(this);
    listLayout = new QGridLayout();
    listLayout->addWidget(add, 0, 0);
    listLayout->addWidget(del, 0, 3);
    listLayout->addWidget(list, 1, 0, 2, 4);
    listWdg = new QWidget(this);
    listWdg->setLayout(listLayout);
    usedStateChanged(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useSecLabel);
    commonLayout->addWidget(typeWdg);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(listWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
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
    // dataChanged connections
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(label, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(labelTypeLabel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(relabel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(useSecLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
bool SecLabels::isUsed() const
{
    return useSecLabel->isChecked();
}
void SecLabels::setUsage(bool state)
{
    useSecLabel->setChecked(state);
}
QDomDocument SecLabels::getDataDocument() const
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
void SecLabels::addSecLabel()
{
    QString _list = QString("%1")
            .arg(readData().toByteArray(8).data());
    if ( list->findItems(_list, Qt::MatchExactly).isEmpty() ) {
        list->addItem(_list);
        emit dataChanged();
    };
    label->clear();
}
void SecLabels::delSecLabel()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
        emit dataChanged();
    };
    list->clearSelection();
}
void SecLabels::modelTypeChanged(QString _type)
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
void SecLabels::usedStateChanged(bool state)
{
    typeLabel->setVisible(state);
    type->setVisible(state);
    listWdg->setVisible(state);
    baseWdg->setVisible( state && type->currentText().toLower()!="none" );
}
void SecLabels::securityTypeChanged(QString _type)
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
QDomDocument SecLabels::readData()
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
void SecLabels::readXMLDesciption(const QString &_xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _domain, _seclabel;
    _domain = doc.firstChildElement("domain");
    _seclabel = _domain.firstChildElement("seclabel");
    useSecLabel->setChecked( !_seclabel.isNull() );
    list->clear();
    if ( !_seclabel.isNull() ) {
        QDomNodeList _list = _domain.childNodes();
        uint j = 0;
        int count = _list.length();
        for (int i=0; i<count; i++) {
            if (!_list.item(j).isNull()) {
                //qDebug()<<_list.item(j).toElement().tagName();
                if ( _list.item(j).toElement().tagName()=="seclabel" ) {
                    QDomDocument _doc;
                    _doc.setContent(QString());
                    _doc.appendChild( _list.item(j) );
                    list->addItem( _doc.toByteArray(4).data() );
                } else
                    ++j;
            };
        };
    };
}
