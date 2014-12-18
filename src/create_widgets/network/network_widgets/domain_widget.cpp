#include "domain_widget.h"

Domain_Widget::Domain_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    domain = new QLineEdit(this);
    domain->setPlaceholderText("example.com");
    domain->setEnabled(false);
    baseLayout->addWidget(domain);
    connect(this, SIGNAL(toggled(bool)),
            domain, SLOT(setEnabled(bool)));
}
