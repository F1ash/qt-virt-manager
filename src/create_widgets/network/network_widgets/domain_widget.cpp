#include "domain_widget.h"

Domain_Widget::Domain_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    domain = new QLineEdit(this);
    domain->setPlaceholderText("example.com");
    domain->setEnabled(false);
    localOnly = new QCheckBox("Local only", this);
    localOnly->setToolTip(
"DNS requests under this domain will only be resolved\n\
by the virtual network's own DNS server");
    baseLayout->addWidget(domain);
    baseLayout->addWidget(localOnly);
    baseLayout->addStretch(-1);
    connect(this, SIGNAL(toggled(bool)),
            domain, SLOT(setEnabled(bool)));
}

/* public slots */
QDomDocument Domain_Widget::getDataDocument() const
{
    QDomDocument doc;
    if ( !domain->text().isEmpty() ) {
        QDomElement _domain =
                doc.createElement("domain");
        _domain.setAttribute(
                    "name",
                    domain->text());
        if ( localOnly->isChecked() )
            _domain.setAttribute(
                        "localOnly", "yes");
        doc.appendChild(_domain);
    };
    return doc;
}
void Domain_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _domain;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _domain = _network.firstChildElement("domain");
        if ( !_domain.isNull() ) {
            setUsage(true);
            QString n, l;
            n = _domain.attribute("name");
            l = _domain.attribute("localOnly");
            if ( !n.isEmpty() ) {
                domain->setText(n);
            };
            if ( !l.isEmpty() ) {
                localOnly->setChecked(
                            (l=="on")?
                                Qt::Checked : Qt::Unchecked);
            };
        };
    };
}
