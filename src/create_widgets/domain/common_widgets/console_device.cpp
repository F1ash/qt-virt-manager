#include "console_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementCharConsole
 *
 *  Console
The console element is used to represent interactive consoles.
Depending on the type of guest in use, the consoles might be paravirtualized devices,
or they might be a clone of a serial device, according to the following rules:

-- If no targetType attribute is set, then the default device type is according
to the hypervisor's rules. The default type will be added when re-querying the XML fed into libvirt.
For fully virtualized guests, the default device type will usually be a serial port.
-- If the targetType attribute is serial, then if no <serial> element exists,
the console element will be copied to the serial element. If a <serial> element does already exist,
the console element will be ignored.
-- If the targetType attribute is not serial, it will be treated normally.
-- Only the first console element may use a targetType of serial.
Secondary consoles must all be paravirtualized.
-- On s390, the console element may use a targetType of sclp or sclplm (line mode).
SCLP is the native console type for s390. There's no controller associated to SCLP consoles.
Since 1.0.2

 */

/*
 * TODO:
 * отслеживать количество консолей для выбора порта
 * отслеживать тип машины для выбора типа консоли:
 * qemu -- virtio, lxc -- lxc
 */

ConsoleDevice::ConsoleDevice(
        QWidget *parent, virConnectPtr *connPtrPtr, virDomainPtr domain) :
    CharDevice(parent, connPtrPtr, domain, QString("console"))
{
    QString connType;
    if ( NULL!=ptr_ConnPtr && NULL!=*ptr_ConnPtr ) {
        connType = QString::fromUtf8(virConnectGetType(*ptr_ConnPtr));
    } else
        emit ptrIsNull();
    devType->clear();
    devType->addItem("PseudoTTY (pty)", "pty");
    targetType = new QComboBox(this);
    if ( connType.toLower()=="lxc" ) {
        targetType->addItem("LXC", "lxc");
    } else if ( connType.toLower()=="qemu" ) {
        targetType->addItem("Default device type is according to the HV's rules", "");
        targetType->addItem("Only the first console element may use 'serial' Type", "serial");
        targetType->addItem("Secondary consoles must all be paravirtualized 'virtio'", "virtio");
        targetType->addItem("SCLP is the native console type for s390", "sclp");
    };
    commonLayout->insertWidget(1, targetType, -1);
}

/* public slots */
QDomDocument ConsoleDevice::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("console");
    _target= doc.createElement("target");
    _target.setAttribute("port", 0);
    QString _targetType = targetType->itemData(
                targetType->currentIndex(), Qt::UserRole).toString();
    if ( !_targetType.isEmpty() ) _target.setAttribute("type", _targetType);
    _devDesc.appendChild(_target);

    _device.appendChild(_devDesc);
    _devDesc.setAttribute("type", "pty");
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
