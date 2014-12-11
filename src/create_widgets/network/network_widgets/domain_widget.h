#ifndef DOMAIN_WIDGET_H
#define DOMAIN_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class Domain_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit Domain_Widget(QWidget *parent = 0);
    QCheckBox       *title;
    QLineEdit       *domain;

private:
    QVBoxLayout     *commonLayout;
};

#endif // DOMAIN_WIDGET_H
