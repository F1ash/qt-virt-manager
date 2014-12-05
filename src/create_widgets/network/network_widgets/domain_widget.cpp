#include "domain_widget.h"

Domain_Widget::Domain_Widget(QWidget *parent) :
    _QWidget(parent)
{
    title = new QCheckBox("DNS Domain", this);
    domain = new QLineEdit(this);
    domain->setPlaceholderText("example.com");
    domain->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(title);
    commonLayout->addWidget(domain);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(title, SIGNAL(toggled(bool)),
            domain, SLOT(setEnabled(bool)));
}
