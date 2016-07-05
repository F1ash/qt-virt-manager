#include "qos_widget.h"

QoS_Widget::QoS_Widget(
        QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    // WARNING: Currently, the Linux kernel doesn't allow
    // ingress qdiscs to have any classes therefore floor
    // can be applied only on inbound and not outbound.
    inbound = new Bound_Widget(this);
    inbound->setFloorUsage(true);
    outbound = new Bound_Widget(this);
    outbound->setFloorUsage(false);
    bandWidth = new QTabWidget(this);
    bandWidth->addTab(inbound, "Inbound");
    bandWidth->addTab(outbound, "Outbound");
    baseLayout->addWidget(bandWidth);
}

/* public slots */
QDomDocument QoS_Widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _bandwidth =
            doc.createElement("bandwidth");
    QDomElement _inbound = doc.createElement("inbound");
    if ( inbound->averageL->isChecked() ) {
        _inbound.setAttribute(
                    "average",
                    inbound->average->text());
    };
    if ( inbound->peakL->isChecked() ) {
        _inbound.setAttribute(
                    "peak",
                    inbound->peak->text());
    };
    if ( inbound->burstL->isChecked() ) {
        _inbound.setAttribute(
                    "burst",
                    inbound->burst->text());
    };
    if ( inbound->floorL->isChecked() ) {
        _inbound.setAttribute(
                    "floor",
                    inbound->floor->text());
    };
    if ( _inbound.hasAttributes() )
        _bandwidth.appendChild(_inbound);
    QDomElement _outbound = doc.createElement("inbound");
    if ( outbound->averageL->isChecked() ) {
        _outbound.setAttribute(
                    "average",
                    outbound->average->text());
    };
    if ( outbound->peakL->isChecked() ) {
        _outbound.setAttribute(
                    "peak",
                    outbound->peak->text());
    };
    if ( outbound->burstL->isChecked() ) {
        _outbound.setAttribute(
                    "burst",
                    outbound->burst->text());
    };
    if ( outbound->floorL->isChecked() ) {
        _outbound.setAttribute(
                    "floor",
                    outbound->floor->text());
    };
    if ( _outbound.hasAttributes() )
        _bandwidth.appendChild(_outbound);

    doc.appendChild(_bandwidth);
    return doc;
}
void QoS_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _bandwidth, _inbound, _outbound;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _bandwidth = _network.firstChildElement("bandwidth");
        if ( !_bandwidth.isNull() ) {
            setUsage(true);
            _inbound = _bandwidth.firstChildElement("inbound");
            _outbound = _bandwidth.firstChildElement("outbound");
            if ( !_inbound.isNull() ) {
                QString a, p, b, f;
                a = _inbound.attribute("averadge");
                p = _inbound.attribute("peak");
                b = _inbound.attribute("burst");
                f = _inbound.attribute("floor");
                if ( !a.isEmpty() )
                    inbound->average->setValue(a.toInt());
                if ( !p.isEmpty() )
                    inbound->peak->setValue(p.toInt());
                if ( !b.isEmpty() )
                    inbound->burst->setValue(b.toInt());
                if ( !f.isEmpty() )
                    inbound->floor->setValue(f.toInt());
            };
            if ( !_outbound.isNull() ) {
                QString a, p, b, f;
                a = _outbound.attribute("averadge");
                p = _outbound.attribute("peak");
                b = _outbound.attribute("burst");
                f = _outbound.attribute("floor");
                if ( !a.isEmpty() )
                    outbound->average->setValue(a.toInt());
                if ( !p.isEmpty() )
                    outbound->peak->setValue(p.toInt());
                if ( !b.isEmpty() )
                    outbound->burst->setValue(b.toInt());
                if ( !f.isEmpty() )
                    outbound->floor->setValue(f.toInt());
            };
        }
    }
}
