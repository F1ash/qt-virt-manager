#ifndef OS_BOOTING_H
#define OS_BOOTING_H

#include <QStackedWidget>
#include "create_widgets/domain/_tab.h"
#include "create_widgets/domain/lxc_widgets/lxc_os_booting.h"
#include "os_boot_widgets/_arch.h"
#include "os_boot_widgets/bios_boot.h"
#include "os_boot_widgets/host_boot.h"
#include "os_boot_widgets/direct_kernel_boot.h"
#include "os_boot_widgets/boot_type.h"

class OS_Booting : public _Tab
{
    Q_OBJECT
public:
    explicit OS_Booting(
            QWidget *parent = Q_NULLPTR,
            QString  _caps  = "",
            QString  _xmlDesc = QString());

signals:
    void             archChanged(const QString&);
    void             domainType(const QString&);
    void             emulatorType(const QString&);
    void             maxVCPU(const QString&);

private:
    const QString    xmlDesc;
    QString          capabilities;
    QString          type;
    QString          arch;
    QString          os_type;
    BootType        *bootType;
    QStackedWidget  *bootSet;

    //QScrollArea     *commonWdg;
    //QVBoxLayout     *scrolledLayout;
    //QWidget         *scrolled;

public slots:
    QDomDocument     getDataDocument() const;
    void             searchBootableDevices(QDomDocument&);
    BootOrderList    getBootOrder() const;
    void             initMaxVCPU();

private slots:
    void             readCapabilities();
    void             readXMLDesciption();
    void             readXMLDesciption(const QString&);
    void             changeOSType(const QString&);
    void             changeBootType();
};

#endif // OS_BOOTING_H
