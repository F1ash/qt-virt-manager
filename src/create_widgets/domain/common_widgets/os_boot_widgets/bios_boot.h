#ifndef BIOS_BOOT_H
#define BIOS_BOOT_H

#include "create_widgets/domain/_qwidget.h"
#include "_arch.h"
#include "path_to_file.h"
#include "boot_menu.h"
#include "boot_devices.h"
#include <QList>
#include <QScrollArea>

struct BootOrder {
    QString     deviceDesc;
    bool        usage = false;
    int         order = -1;
};

typedef QList<BootOrder> BootOrderList;

class BIOS_Boot : public _QWidget
{
    Q_OBJECT
public:
    explicit BIOS_Boot(
            QWidget *parent = NULL,
            QString  _caps  = "");
    _Arch           *architecture;
    Path_To_File    *loader;
    Path_To_File    *nvram;
    Path_To_File    *nvramTemplate;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QString          arch;
    QString          capabilities;
    QLabel          *loaderLabel, *nvramLabel,
                    *templateLabel;
    BootMenu        *bootMenu;
    Boot_Devices    *bootDevices;
    QVBoxLayout     *commonLayout;

    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
    void             setInitState();
    void             searchBootableDevices(QDomDocument&);
    BootOrderList    getBootOrderData() const;

private slots:
    void             changeArch(QString&);
};

#endif // BIOS_BOOT_H
