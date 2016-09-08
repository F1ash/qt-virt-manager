#include "rule_instance.h"

RuleInstance::RuleInstance(QWidget *parent) :
    _QWidget(parent)
{
    setStyleSheet("QWidget {\
                  border-width: 0px;\
                  border-style: solid;\
                  border-radius: 8px;};");
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

    addRule  = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                "",
                this);
    addRule->setToolTip("Rule to chain");
    clearRule = new QPushButton(
                QIcon::fromTheme("wipe"),
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
