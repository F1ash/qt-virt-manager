#ifndef CHAIN_RULES_H
#define CHAIN_RULES_H

#include <QListWidget>
#include "rule_instance.h"

class ChainRules : public _QWidget
{
    Q_OBJECT
public:
    explicit ChainRules(QWidget *parent = Q_NULLPTR);
    void             setDataDescription(const QString&);
    QDomDocument     getDataDocument() const;
    QString          getChainProtocol() const;
    QString          getPriority() const;

signals:

private:
    QLabel          *prot, *prior;
    QComboBox       *chainProtocol;
    QSpinBox        *priority;
    QHBoxLayout     *paramsLayout, *ruleButtonsLayout;
    QWidget         *chainWdg, *paramsWdg, *ruleButtons;
    QPushButton     *addRule, *editRule, *delRule;
    QListWidget     *ruleList;
    RuleInstance    *ruleWdg;
    QStackedWidget  *commonWdg;
    QVBoxLayout     *chainLayout, *commonLayout;
    void             setChainIdx(const QString&);

private slots:
    void             changePriorityDefault(int);
    void             addRuleToList();
    void             editRuleInList();
    void             delRuleFromList();
    void             turnToChainWdg();
    void             insertRuleToList(const QString&, int);

public slots:
};

#endif // CHAIN_RULES_H
