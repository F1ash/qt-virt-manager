#include "security_label.h"

SecurityLabel::SecurityLabel(QWidget *parent) :
    _QWidget(parent)
{
    setObjectName("Security-High");
    useSecLabel = new QCheckBox("Use Security Label", this);
    typeLabel = new QLabel("Type:", this);
    modelLabel = new QLabel("Model:", this);
    type = new QComboBox(this);
    type->addItems(QStringList()<<"None"<<"Dynamic"<<"Static");
    model = new QComboBox(this);
    model->addItem("SELinux");
    labelTypeLabel = new QComboBox(this);
    labelTypeLabel->addItems(QStringList()<<"Label"<<"BaseLabel"<<"ImageLabel");
    label = new QLineEdit(this);
    relabelLabel = new QLabel("Relabel:", this);
    relabel = new QComboBox(this);
    relabel->addItems(QStringList()<<"Default"<<"Yes"<<"No");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(typeLabel, 0, 0);
    baseLayout->addWidget(type, 0, 1);
    baseLayout->addWidget(modelLabel, 1, 0);
    baseLayout->addWidget(model, 1, 1);
    baseLayout->addWidget(labelTypeLabel, 2, 0);
    baseLayout->addWidget(label, 2, 1);
    baseLayout->addWidget(relabelLabel, 3, 0);
    baseLayout->addWidget(relabel, 3, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useSecLabel);
    commonLayout->addWidget(baseWdg);
    commonLayout->insertStretch(2, -1);
    setLayout(commonLayout);
    connect(useSecLabel, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(type, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(securityTypeChanged(QString)));
}

/* public slots */
QDomDocument SecurityLabel::getDevDocument() const
{
    QDomText data;
    QDomDocument doc = QDomDocument();
    if ( !useSecLabel->isChecked() ) return doc;
    QDomElement _secLabel, _label, _data;
    QString _t, _r, _m;
    _data = doc.createElement("data");
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
        data = doc.createTextNode(label->text().toLower());
        _label.appendChild(data);
        _secLabel.appendChild(_label);
    };
    _secLabel.setAttribute("type", _t);
    _data.appendChild(_secLabel);

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}

/* private slots */
void SecurityLabel::securityTypeChanged(QString _type)
{
    if ( _type.toLower()=="static" ) {
        label->setPlaceholderText("system_u:system_r:svirt_t:s0:c392,c662");
    } else if ( _type.toLower()=="dynamic" ) {
        label->setPlaceholderText("system_u:system_r:svirt_t:s0");
    } else {
        label->setPlaceholderText("");
    };
}
