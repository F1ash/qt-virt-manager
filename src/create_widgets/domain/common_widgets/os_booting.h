#ifndef OS_BOOTING_H
#define OS_BOOTING_H

#include <QScrollArea>
#include <QStackedWidget>
#include <QMessageBox>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"
#include "create_widgets/domain/lxc_widgets/os_booting.h"
#include "os_boot_widgets/_arch.h"
#include "os_boot_widgets/bios_boot.h"
#include "os_boot_widgets/host_boot.h"
#include "os_boot_widgets/direct_kernel_boot.h"
#include "os_boot_widgets/boot_type.h"

class OS_Booting : public _QWidget
{
    Q_OBJECT
public:
    explicit OS_Booting(
            QWidget *parent = NULL,
            QString  _caps  = "",
            QString  _xmlDesc = QString());

signals:
    void             archChanged(QString&);
    void             domainType(QString&);
    void             emulatorType(QString&);

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    QString          capabilities;
    QString          type;
    QString          arch;
    QString          os_type;
    BootType        *bootType;
    QStackedWidget  *bootSet;
    RestorePanel    *restorePanel;

    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    QString          closeDataEdit();

private slots:
    void             readCapabilities();
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetBootData();
    void             revertBootData();
    void             saveBootData();
    void             changeOSType(QString&);
    void             changeBootType();
};

#endif // OS_BOOTING_H
