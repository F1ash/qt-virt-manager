#ifndef SECURITY_LABEL_H
#define SECURITY_LABEL_H

#include "create_widgets/domain/_qwidget.h"

class SecurityLabel : public _QWidget
{
    Q_OBJECT
public:
    explicit SecurityLabel(QWidget *parent = 0);

private:
    QCheckBox       *useSecLabel;
    QLabel          *typeLabel, *modelLabel,
                    *relabelLabel;
    QComboBox       *type, *model, *relabel,
                    *labelTypeLabel;
    QLineEdit       *label;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument    getDevDocument() const;

private slots:
    void securityTypeChanged(QString);
};

#endif // SECURITY_LABEL_H
