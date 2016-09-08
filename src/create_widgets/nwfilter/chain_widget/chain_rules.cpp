#include "chain_rules.h"

ChainRules::ChainRules(QWidget *parent) :
    _QWidget(parent)
{
    prot = new QLabel("Protocol:", this);
    prior = new QLabel("Priority:", this);
    chainProtocol = new QComboBox(this);
    connect(chainProtocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changePriorityDefault(int)));
    priority = new QSpinBox(this);
    priority->setRange(-1000, 1000);
    priority->clear();
    paramsLayout = new QHBoxLayout(this);
    paramsLayout->addWidget(prot, 0, Qt::AlignRight);
    paramsLayout->addWidget(chainProtocol, 0, Qt::AlignLeft);
    paramsLayout->addWidget(prior, 0, Qt::AlignRight);
    paramsLayout->addWidget(priority, 0, Qt::AlignLeft);
    paramsWdg = new QWidget(this);
    paramsWdg->setLayout(paramsLayout);
    addRule  = new QPushButton(
                QIcon::fromTheme("list-add"),
                "",
                this);
    addRule->setToolTip("New rule");
    editRule = new QPushButton(
                QIcon::fromTheme("configure"),
                "",
                this);
    editRule->setToolTip("Edit selected rule");
    delRule  = new QPushButton(
                QIcon::fromTheme("list-remove"),
                "",
                this);
    delRule->setToolTip("Delete selected rule");
    ruleButtonsLayout = new QHBoxLayout(this);
    ruleButtonsLayout->addWidget(addRule);
    ruleButtonsLayout->addWidget(editRule);
    ruleButtonsLayout->addWidget(delRule);
    ruleButtons = new QWidget(this);
    ruleButtons->setLayout(ruleButtonsLayout);
    ruleList = new QListWidget(this);
    connect(addRule, SIGNAL(clicked(bool)),
            this, SLOT(addRuleToList()));
    connect(editRule, SIGNAL(clicked(bool)),
            this, SLOT(editRuleInList()));
    connect(delRule, SIGNAL(clicked(bool)),
            this, SLOT(delRuleFromList()));
    chainLayout = new QVBoxLayout(this);
    chainLayout->addWidget(paramsWdg);
    chainLayout->addWidget(ruleList);
    chainLayout->addWidget(ruleButtons);
    chainWdg = new QWidget(this);
    chainWdg->setLayout(chainLayout);

    ruleWdg = new RuleInstance(this);
    connect(ruleWdg, SIGNAL(ruleCancelled()),
            this, SLOT(turnToChainWdg()));
    connect(ruleWdg, SIGNAL(insertRule(const QString&, int)),
            this, SLOT(insertRuleToList(const QString&, int)));

    commonWdg = new QStackedWidget(this);
    commonWdg->addWidget(chainWdg);
    commonWdg->addWidget(ruleWdg);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(commonWdg);
    setLayout(commonLayout);

    chainProtocol->addItem("STP");
    chainProtocol->addItem("MAC");
    chainProtocol->addItem("VLAN");
    chainProtocol->addItem("IPv4");
    chainProtocol->addItem("IPv6");
    chainProtocol->addItem("ARP");
    chainProtocol->addItem("RARP");
    chainProtocol->addItem("NONE");
}

/* private slots */
void ChainRules::changePriorityDefault(int i)
{
    int value = 500; // default
    switch (i) {
    case 0:
        value = -810;
        break;
    case 1:
        value = -800;
        break;
    case 2:
        value = -750;
        break;
    case 3:
        value = -700;
        break;
    case 4:
        value = -600;
        break;
    case 5:
        value = -500;
        break;
    case 6:
        value = -400;
        break;
    case 7:
    default:
        break;
    };
    priority->setValue(value);
    ruleList->clear();
    ruleWdg->setAttributesMapByProtocol(i);
}
void ChainRules::addRuleToList()
{
    ruleWdg->editRule("", -1);
    commonWdg->setCurrentWidget(ruleWdg);
}
void ChainRules::editRuleInList()
{
    QList<QListWidgetItem*> l = ruleList->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = ruleList->row(item);
    ruleWdg->editRule(item->data(Qt::UserRole).toString(), row);
    commonWdg->setCurrentWidget(ruleWdg);
}
void ChainRules::delRuleFromList()
{
    QList<QListWidgetItem*> l = ruleList->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = ruleList->row(item);
    ruleList->takeItem(row);
    delete item;
    item = nullptr;
}
void ChainRules::turnToChainWdg()
{
    commonWdg->setCurrentWidget(chainWdg);
}
void ChainRules::insertRuleToList(const QString &_rule, int row)
{
}
