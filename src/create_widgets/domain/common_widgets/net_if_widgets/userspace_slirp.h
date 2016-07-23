#ifndef USERSPACE_SLIRP_H
#define USERSPACE_SLIRP_H

#include "create_widgets/common/_qwidget.h"
#include "mac_address.h"
#include "create_widgets/domain/device_address.h"

class Userspace_SLIRP : public _QWidget
{
    Q_OBJECT
public:
    explicit Userspace_SLIRP(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

private:
    MAC_Address     *mac;
    QLabel          *infoIcon;
    QLabel          *info;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // USERSPACE_SLIRP_H
