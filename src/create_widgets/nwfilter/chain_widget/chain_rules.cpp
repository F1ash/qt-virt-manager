#include "chain_rules.h"
#include <QTextStream>

ChainRules::ChainRules(QWidget *parent) :
    _QWidget(parent)
{
    prot = new QLabel(tr("Protocol:"), this);
    prior = new QLabel(tr("Priority:"), this);
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
    addRule->setToolTip(tr("New rule"));
    editRule = new QPushButton(
                QIcon::fromTheme("configure"),
                "",
                this);
    editRule->setToolTip(tr("Edit selected rule"));
    delRule  = new QPushButton(
                QIcon::fromTheme("list-remove"),
                "",
                this);
    delRule->setToolTip(tr("Delete selected rule"));
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
    connect(chainProtocol, SIGNAL(currentIndexChanged(int)),
            ruleWdg, SLOT(setAttributesMapByProtocol(int)));

    commonWdg = new QStackedWidget(this);
    commonWdg->addWidget(chainWdg);
    commonWdg->addWidget(ruleWdg);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(commonWdg);
    setLayout(commonLayout);

    // order is template for attributes widgets
    chainProtocol->addItem("STP",   "stp");
    chainProtocol->addItem("MAC",   "mac");
    chainProtocol->addItem("VLAN",  "vlan");
    chainProtocol->addItem("IPv4",  "ip");
    chainProtocol->addItem("IPv6",  "ipv6");
    chainProtocol->addItem("ARP",   "arp");
    chainProtocol->addItem("RARP",  "rarp");
    chainProtocol->addItem("ROOT",  "root");
}
void ChainRules::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _filter;
    _filter = doc.firstChildElement("filter");
    if ( !_filter.isNull() ) {
        QString _chain = _filter.attribute("chain");
        setChainIdx(_chain);
        QString _prior = _filter.attribute("priority");
        priority->setValue(_prior.toInt());
        QDomNode _n = _filter.firstChild();
        while ( !_n.isNull() ) {
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName().compare("rule")==0 ) {
                    QListWidgetItem *item = new QListWidgetItem;
                    QString str;
                    QTextStream stream(&str);
                    _n.save(stream, QDomNode::CDATASectionNode);
                    item->setText(str);
                    item->setToolTip(str);
                    ruleList->addItem(item);
                };
            };
            _n = _n.nextSibling();
        };
    };
}
QDomDocument ChainRules::getDataDocument() const
{
    QDomDocument doc;
    for (int i=0; i<ruleList->count(); i++) {
        QListWidgetItem *item = ruleList->item(i);
        if ( item!=nullptr ) {
            QString r = item->text();
            QDomDocument _r;
            _r.setContent(r);
            QDomElement _rule = doc.createElement("rule");
            _rule = _r.documentElement();
            doc.appendChild(_rule);
        };
    };
    return doc;
}
QString ChainRules::getChainProtocol() const
{
#if QT_VERSION>=0x050200
    return chainProtocol->currentData().toString();
#else
    return chainProtocol->itemData(
                chainProtocol->currentIndex()).toString();
#endif
}
QString ChainRules::getPriority() const
{
    return priority->text();
}
void ChainRules::setChainIdx(const QString &c)
{
    int idx = chainProtocol->count()-1;
    for (int i=0; i<chainProtocol->count(); i++) {
        QString s = chainProtocol->itemData(i).toString();
        if ( c.startsWith(s) ) {
            idx = i;
            break;
        };
    };
    chainProtocol->setCurrentIndex(idx);
    chainProtocol->setEnabled(false);
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
}
void ChainRules::addRuleToList()
{
    ruleWdg->editRule("", ruleList->count());
    commonWdg->setCurrentWidget(ruleWdg);
}
void ChainRules::editRuleInList()
{
    QList<QListWidgetItem*> l = ruleList->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = ruleList->row(item);
    ruleWdg->editRule(item->text(), row);
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
    QListWidgetItem *item;
    if ( row<ruleList->count() ) {
        item = ruleList->takeItem(row);
        delete item;
        item = nullptr;
    };
    item = new QListWidgetItem;
    // item set text, data
    item->setText(_rule);
    item->setToolTip(_rule);
    ruleList->insertItem(row, item);
    commonWdg->setCurrentWidget(chainWdg);
    // check priority, move to correct place
}
