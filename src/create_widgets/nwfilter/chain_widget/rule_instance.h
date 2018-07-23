#ifndef RULE_INSTANCE_H
#define RULE_INSTANCE_H

#include <QStackedWidget>
#include <QPushButton>
#include "create_widgets/common/_qwidget.h"

class RuleInstance : public _QWidget
{
    Q_OBJECT
public:
    explicit RuleInstance(QWidget *parent = Q_NULLPTR);
    void             editRule(const QString&, int);

signals:
    void             insertRule(const QString&, int);
    void             ruleCancelled();

private:
    int              editedRow;
    QComboBox       *action, *direction;
    QSpinBox        *priority;
    QCheckBox       *stateMatch;
    QHBoxLayout     *ruleLayout;
    QWidget         *ruleWdg, *buttons;
    QStackedWidget  *attributes;
    QPushButton     *addRule, *clearRule, *cancel;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

private slots:
    void             addRuleToList();
    void             clearRuleAttrbutes();
    void             cancelEditRule();
    void             attributesTypeChanged(int);

public slots:
    void             setAttributesMapByProtocol(int);
};

#endif // RULE_INSTANCE_H
