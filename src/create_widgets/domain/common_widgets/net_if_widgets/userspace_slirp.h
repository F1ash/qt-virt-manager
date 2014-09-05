#ifndef USERSPACE_SLIRP_H
#define USERSPACE_SLIRP_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"

class Userspace_SLIRP : public _QWidget
{
    Q_OBJECT
public:
    explicit Userspace_SLIRP(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    MAC_Address     *mac;
    QLabel          *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // USERSPACE_SLIRP_H
