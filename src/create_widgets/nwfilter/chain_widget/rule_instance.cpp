#include "rule_instance.h"
#include "attributes_widget/stp_attributes.h"
#include "attributes_widget/mac_attributes.h"
#include "attributes_widget/vlan_attributes.h"
#include "attributes_widget/ipv4_attributes.h"
#include "attributes_widget/ipv6_attributes.h"
#include "attributes_widget/arp_attributes.h"
#include "attributes_widget/rarp_attributes.h"
#include "attributes_widget/mixed_attributes.h"

RuleInstance::RuleInstance(QWidget *parent) :
    _QWidget(parent)
{
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
    /* need to according with order
    chainProtocol->addItem("STP");
    chainProtocol->addItem("MAC");
    chainProtocol->addItem("VLAN");
    chainProtocol->addItem("IPv4");
    chainProtocol->addItem("IPv6");
    chainProtocol->addItem("ARP");
    chainProtocol->addItem("RARP");
    chainProtocol->addItem("MIXED");
    */
    attributes->addWidget(new STP_Attributes(this));
    attributes->addWidget(new MAC_Attributes(this));
    attributes->addWidget(new VLAN_Attributes(this));
    attributes->addWidget(new IPv4_Attributes(this));
    attributes->addWidget(new IPv6_Attributes(this));
    attributes->addWidget(new ARP_Attributes(this));
    attributes->addWidget(new RARP_Attributes(this));
    attributes->addWidget(new MIXED_Attributes(this));

    addRule  = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                "",
                this);
    addRule->setToolTip("Rule to chain");
    clearRule = new QPushButton(
                QIcon::fromTheme("edit-clear"),
                "",
                this);
    clearRule->setToolTip("Clear attributes");
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

}
void RuleInstance::setAttributesMapByProtocol(int i)
{
    attributes->setCurrentIndex(i);
}

/* private slots */
void RuleInstance::addRuleToList()
{
    emit insertRule("", 0);
}
void RuleInstance::clearRuleAttrbutes()
{
    // clear attributes
}
void RuleInstance::cancelEditRule()
{
    clearRuleAttrbutes();
    emit ruleCancelled();
}
