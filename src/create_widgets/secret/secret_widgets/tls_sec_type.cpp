#include "tls_sec_type.h"

/*
 * http://libvirt.org/formatsecret.html#tlsUsageType
 */

tlsSecType::tlsSecType(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _SecType(parent, connPtrPtr)
{
    usage = new QLineEdit(this);
    usage->setPlaceholderText(tr("Enter usage name"));
    usageLayout = new QHBoxLayout();
    usageLayout->addWidget(usage);
    usageWdg = new QWidget(this);
    usageWdg->setLayout(usageLayout);
    baseLayout->addWidget(usageWdg);
    info->setText(tr("Using for authentification"));
}
QDomDocument tlsSecType::getSecStuff() const
{
    /*
     * <usage type='tls'>
            <name>tls_example</name>
       </usage>
     */
    QDomDocument doc;
    QDomElement _usage, _name;
    QDomText    _text = doc.createTextNode(usage->text());
    _usage = doc.createElement("usage");
    _usage.setAttribute("type", "tls");
    doc.appendChild(_usage);
    _name = doc.createElement("name");
    _name.appendChild(_text);
    _usage.appendChild(_name);
    //qDebug()<<doc.toByteArray(4).data();
    return doc;
}
