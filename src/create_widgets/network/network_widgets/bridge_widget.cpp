#include "bridge_widget.h"

Bridge_Widget::Bridge_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter bridge name (Ex.: virbr1)");
    stp = new QCheckBox("Spanning Tree Protocol", this);
    stp->setChecked(true); // default state
    bridgeLt = new QHBoxLayout();
    bridgeLt->addWidget(bridgeName);
    bridgeLt->addWidget(stp);
    bridgeWdg = new QWidget(this);
    bridgeWdg->setLayout(bridgeLt);
    bridgeWdg->setEnabled(false);
    delayLabel = new QLabel("Delay", this);
    delay = new QSpinBox(this);
    delay->setRange(0, 360);
    macTabLabel = new QLabel("macTableManager", this);
    macTableManager = new QComboBox(this);
    macTableManager->addItems(QStringList()<<"kernel"<<"libvirt");
    macTabLt = new QHBoxLayout();
    macTabLt->addWidget(macTabLabel);
    macTabLt->addWidget(macTableManager);
    macTabLt->addSpacing(50);
    macTabLt->addWidget(delayLabel);
    macTabLt->addWidget(delay);
    macTabWdg = new QWidget(this);
    macTabWdg->setLayout(macTabLt);
    macTabWdg->setEnabled(false);
    baseLayout->addWidget(bridgeWdg);
    baseLayout->addWidget(macTabWdg);
    connect(this, SIGNAL(toggled(bool)),
            bridgeWdg, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(toggled(bool)),
            macTabWdg, SLOT(setEnabled(bool)));
}

/* public slots */
QDomDocument Bridge_Widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _bridge =
            doc.createElement("bridge");
    _bridge.setAttribute(
                "name",
                bridgeName->text());
    _bridge.setAttribute(
                "stp",
                (stp->isChecked())? "on":"off");
    _bridge.setAttribute(
                "delay",
                delay->value());
    _bridge.setAttribute(
                "macTableManager",
                macTableManager->currentText());
    doc.appendChild(_bridge);
    return doc;
}
void Bridge_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _bridge;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _bridge = _network.firstChildElement("bridge");
        if ( !_bridge.isNull() ) {
            setUsage(true);
            QString n, s, d, m;
            n = _bridge.attribute("name");
            s = _bridge.attribute("stp");
            d = _bridge.attribute("delay");
            m = _bridge.attribute("macTableManager");
            if ( !n.isEmpty() )
                bridgeName->setText(n);
            if ( !s.isEmpty() )
                stp->setChecked(
                            (s=="on")?
                                Qt::Checked : Qt::Unchecked);
            if ( !d.isEmpty() )
                delay->setValue(d.toInt());
            if ( !m.isEmpty() ) {
                int idx = macTableManager->findText(m);
                if ( idx<0 ) idx = 0;
                // "kernel" is default
                macTableManager->setCurrentIndex(idx);
            };
        };
    };
}
