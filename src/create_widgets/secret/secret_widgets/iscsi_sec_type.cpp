#include "iscsi_sec_type.h"

/*
 * http://libvirt.org/formatsecret.html#iSCSIUsageType
 */

iSCSISecType::iSCSISecType(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
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
QDomDocument iSCSISecType::getSecStuff() const
{
    /*
     * <usage type='iscsi'>
            <target>libvirtiscsi</target>
       </usage>
     */
    QDomDocument doc;
    QDomElement _usage, _target;
    QDomText    _text = doc.createTextNode(usage->text());
    _usage = doc.createElement("usage");
    _usage.setAttribute("type", "iscsi");
    doc.appendChild(_usage);
    _target = doc.createElement("target");
    _target.appendChild(_text);
    _usage.appendChild(_target);
    //qDebug()<<doc.toByteArray(4).data();
    return doc;
}
