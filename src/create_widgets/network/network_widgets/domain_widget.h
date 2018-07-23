#ifndef DOMAIN_WIDGET_H
#define DOMAIN_WIDGET_H

#include "create_widgets/common/_checked_widget.h"

class Domain_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Domain_Widget(
            QWidget *parent = Q_NULLPTR,
            QString  tag = "DNS Domain");

private:
    QLineEdit       *domain;
    QCheckBox       *localOnly;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // DOMAIN_WIDGET_H
