#ifndef BIOS_BOOT_H
#define BIOS_BOOT_H

#include "create_widgets/domain/_qwidget.h"
#include "_arch.h"
#include "path_to_file.h"
#include "boot_menu.h"
#include "boot_devices.h"

class BIOS_Boot : public _QWidget
{
    Q_OBJECT
public:
    explicit BIOS_Boot(
            QWidget *parent = NULL,
            QString  _caps  = "");
    _Arch           *architecture;
    Path_To_File    *loader;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QString          arch;
    QString          capabilities;
    QLabel          *loaderLabel;
    BootMenu        *bootMenu;
    Boot_Devices    *bootDevices;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
    void             setInitState();
    void             searchBootableDevices(QDomDocument&);

private slots:
    void             changeArch(QString&);
};

#endif // BIOS_BOOT_H
