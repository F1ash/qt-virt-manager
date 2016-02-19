#ifndef DOMAIN_WIDGET_H
#define DOMAIN_WIDGET_H

#include "_checked_widget.h"

class Domain_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Domain_Widget(
            QWidget *parent = nullptr,
            QString  tag = "DNS Domain");
    QLineEdit       *domain;
};

#endif // DOMAIN_WIDGET_H
