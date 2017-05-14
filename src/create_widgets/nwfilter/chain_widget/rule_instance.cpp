#include "rule_instance.h"
// Rules of this type should either go
// into the root or same named chain.
#include "attributes_widget/stp_attributes.h"
#include "attributes_widget/mac_attributes.h"
#include "attributes_widget/vlan_attributes.h"
#include "attributes_widget/ipv4_attributes.h"
#include "attributes_widget/ipv6_attributes.h"
#include "attributes_widget/arp_attributes.h"
#include "attributes_widget/rarp_attributes.h"
#include "attributes_widget/root_attributes.h"

RuleInstance::RuleInstance(QWidget *parent) :
    _QWidget(parent)
{
    editedRow = -1;
    action = new QComboBox(this);
    action->setToolTip("Action");
    action->addItems(QStringList()
                     <<"drop"<<"reject"<<"accept"
                     <<"return"<<"continue");
    direction = new QComboBox(this);
    direction->setToolTip("Direction");
    direction->addItems(QStringList()
                        <<"out"<<"in"<<"inout");
    priority = new QSpinBox(this);
    priority->setRange(-1000, 1000);
    priority->setValue(500);
    priority->setToolTip("Priority");
    stateMatch = new QCheckBox("StateMatch", this);
    stateMatch->setCheckable(true);
    stateMatch->setChecked(true);

    ruleLayout = new QHBoxLayout(this);
    ruleLayout->addWidget(action, 10);
    ruleLayout->addWidget(direction, 10);
    ruleLayout->addWidget(priority, 10);
    ruleLayout->addWidget(stateMatch, 10);
    ruleWdg = new QWidget(this);
    ruleWdg->setLayout(ruleLayout);

    attributes = new QStackedWidget(this);
    /* need to according with order in ChainRules:
    chainProtocol->addItem("STP");
    chainProtocol->addItem("MAC");
    chainProtocol->addItem("VLAN");
    chainProtocol->addItem("IPv4");
    chainProtocol->addItem("IPv6");
    chainProtocol->addItem("ARP");
    chainProtocol->addItem("RARP");
    chainProtocol->addItem("ROOT");
    */
    attributes->addWidget(new STP_Attributes(this));
    attributes->addWidget(new MAC_Attributes(this));
    attributes->addWidget(new VLAN_Attributes(this));
    attributes->addWidget(new IPv4_Attributes(this));
    attributes->addWidget(new IPv6_Attributes(this));
    attributes->addWidget(new ARP_Attributes(this));
    attributes->addWidget(new RARP_Attributes(this));
    attributes->addWidget(new ROOT_Attributes(this));

    addRule  = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                "",
                this);
    addRule->setToolTip("Rule to chain");
    clearRule = new QPushButton(
                QIcon::fromTheme("edit-clear"),
                "",
                this);
    clearRule->setToolTip("Clear all attributes data");
    cancel  = new QPushButton(
                QIcon::fromTheme("dialog-cancel"),
                "",
                this);
    cancel->setToolTip("Cancel");
    connect(addRule, SIGNAL(clicked(bool)),
            this, SLOT(addRuleToList()));
    connect(clearRule, SIGNAL(clicked(bool)),
            this, SLOT(clearRuleAttrbutes()));
    connect(cancel, SIGNAL(clicked(bool)),
            this, SLOT(cancelEditRule()));
    connect(attributes, SIGNAL(currentChanged(int)),
            this, SLOT(attributesTypeChanged(int)));
    for (int i=0; i<attributes->count(); i++) {
        connect(attributes->widget(i), SIGNAL(released(bool)),
                clearRule, SLOT(setEnabled(bool)));
    };
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(addRule);
    buttonLayout->addWidget(clearRule);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(ruleWdg);
    commonLayout->addWidget(attributes);
    commonLayout->addWidget(buttons);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
void RuleInstance::editRule(const QString &rule, int row)
{
    editedRow = row;
    QDomDocument doc;
    doc.setContent(rule);
    // parse rule for attributes and protocol parameters
    QDomElement _rule = doc.documentElement();
    int idx = action->findText(_rule.attribute("action"));
    if ( idx<0 ) idx = 0;
    action->setCurrentIndex(idx);
    idx = direction->findText(_rule.attribute("direction"));
    if ( idx<0 ) idx = 0;
    direction->setCurrentIndex(idx);
    priority->setValue(_rule.attribute("priority").toInt());
    stateMatch->setChecked(_rule.attribute("statematch", "true")=="true");
    QDomNode _n = _rule.firstChild();
    while ( !_n.isNull() ) {
        QDomElement _el = _n.toElement();
        if ( !_el.isNull() ) {
            QString protocolID = _el.tagName();
            QString match = _el.attribute("match", "yes");
            QDomNamedNodeMap m = _el.attributes();
            for (int i=0; i<m.count(); i++) {
                QDomAttr a = m.item(i).toAttr();
                //QTextStream s(stdout);
                //s<<match<<" "<<a.name()<<" "<<a.value()<<endl;
                _Attributes *_a = static_cast<_Attributes*>(
                            attributes->currentWidget());
                if ( _a!=nullptr ) {
                    QVariantMap _m;
                    _m.insert("protocolID", protocolID);
                    _m.insert("match", match);
                    _m.insert("name", a.name());
                    _m.insert("value", a.value());
                    _a->setAttrValue(_m);
                };
            };
        };
        _n = _n.nextSibling();
    };
}

/* private slots */
void RuleInstance::addRuleToList()
{
    QDomDocument doc;
    doc.setContent(QString());
    QDomElement _rule = doc.createElement("rule");
    _rule.setAttribute("action", action->currentText());
    _rule.setAttribute("direction", direction->currentText());
    _rule.setAttribute("priority", priority->text());
    if ( !stateMatch->isChecked() )
        _rule.setAttribute("statematch", "false");
    _Attributes *_a = static_cast<_Attributes*>(
                attributes->currentWidget());
    if ( _a!=nullptr ) {
        foreach (QString _attr, _a->getAttrList()) {
            QVariantMap _m = _a->getAttrValue(_attr);
            QDomElement _protocolID = doc.createElement(
                        _m.value("protocolID").toString());
            _protocolID.setAttribute(
                        _m.value("name").toString(),
                        _m.value("value").toString());
            if ( _m.contains("match") ) {
                _protocolID.setAttribute(
                            "match",
                            _m.value("match").toString());
            };
            _rule.appendChild(_protocolID);
        };
    };
    doc.appendChild(_rule);
    emit insertRule(doc.toByteArray(4).constData(), editedRow);
    clearRuleAttrbutes();
}
void RuleInstance::clearRuleAttrbutes()
{
    // clear attributes
    _Attributes *a = static_cast<_Attributes*>(
                attributes->currentWidget());
    if ( a!=nullptr ) a->clearAllAttributeData();
}
void RuleInstance::cancelEditRule()
{
    clearRuleAttrbutes();
    emit ruleCancelled();
}
void RuleInstance::attributesTypeChanged(int i)
{
    clearRule->setDisabled(i==attributes->count()-1);
}

/* public slots */
void RuleInstance::setAttributesMapByProtocol(int i)
{
    attributes->setCurrentIndex(i);
}
