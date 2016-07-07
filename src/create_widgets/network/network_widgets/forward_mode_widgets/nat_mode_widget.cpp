#include "nat_mode_widget.h"

NAT_Mode_widget::NAT_Mode_widget(QWidget *parent) :
    _QWidget(parent)
{
    addrRange = new QCheckBox("Use Address Range", this);
    ipStart = new QLineEdit(this);
    ipEnd = new QLineEdit(this);
    addrLayout = new QHBoxLayout();
    addrLayout->addWidget(addrRange);
    addrLayout->addWidget(ipStart);
    addrLayout->addWidget(ipEnd);
    portRange = new QCheckBox("Use Port Range", this);
    portStart = new QSpinBox(this);
    portStart->setRange(0, 65535);
    portEnd = new QSpinBox(this);
    portEnd->setRange(0, 65535);
    portLayout = new QHBoxLayout();
    portLayout->addWidget(portRange);
    portLayout->addWidget(portStart);
    portLayout->addWidget(portEnd);
    addrWdg = new QWidget(this);
    addrWdg->setLayout(addrLayout);
    addrWdg->setEnabled(false);
    portWdg = new QWidget(this);
    portWdg->setLayout(portLayout);
    portWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(addrWdg);
    commonLayout->addWidget(portWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(addrRange, SIGNAL(toggled(bool)),
            addrWdg, SLOT(setEnabled(bool)));
    connect(portRange, SIGNAL(toggled(bool)),
            portWdg, SLOT(setEnabled(bool)));
    connect(portStart, SIGNAL(valueChanged(int)),
            this, SLOT(portStartChanged(int)));
    connect(portEnd, SIGNAL(valueChanged(int)),
            this, SLOT(portEndChanged(int)));
}

/* public slots */
QDomDocument NAT_Mode_widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _xmlDesc, _addrRange, _portRange;
    _xmlDesc = doc.createElement("nat");
    if ( addrRange->isChecked() ) {
        _addrRange = doc.createElement("address");
        _addrRange.setAttribute(
                    "start", ipStart->text());
        _addrRange.setAttribute(
                    "end", ipEnd->text());
        _xmlDesc.appendChild(_addrRange);
    };
    if ( portRange->isChecked() ) {
        _portRange = doc.createElement("port");
        _portRange.setAttribute(
                    "start", portStart->text());
        _portRange.setAttribute(
                    "end", portEnd->text());
        _xmlDesc.appendChild(_portRange);
    };
    if ( _xmlDesc.hasAttributes() || _xmlDesc.hasChildNodes() )
        doc.appendChild(_xmlDesc);
    return doc;
}
void NAT_Mode_widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _forward, _nat, _addr, _port;
    _forward = doc.documentElement();
    _nat = _forward.firstChildElement("nat");
    if ( !_nat.isNull() ) {
        _addr = _nat.firstChildElement("address");
        if ( !_addr.isNull() ) {
            addrRange->setChecked(true);
            ipStart->setText(
                        _addr.attribute("start"));
            ipEnd->setText(
                        _addr.attribute("end"));
        };
        _port = _nat.firstChildElement("port");
        if ( !_port.isNull() ) {
            portRange->setChecked(true);
            portStart->setValue(
                        _port.attribute("start").toInt());
            portEnd->setValue(
                        _port.attribute("end").toInt());
        };
    };
}

/* private slots */
void NAT_Mode_widget::portStartChanged(int _value)
{
    if ( _value>portEnd->value() ) {
        portEnd->setValue(_value);
    };
}
void NAT_Mode_widget::portEndChanged(int _value)
{
    if ( _value<portStart->value() ) {
        portStart->setValue(_value);
    };
}
