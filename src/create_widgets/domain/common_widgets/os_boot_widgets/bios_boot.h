#ifndef BIOS_BOOT_H
#define BIOS_BOOT_H

#include "create_widgets/domain/_qwidget.h"
#include "_arch.h"

class BIOS_Boot : public _QWidget
{
    Q_OBJECT
public:
    explicit BIOS_Boot(
            QWidget *parent = NULL,
            QString  _caps  = "");
    _Arch           *architecture;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QString          capabilities;
    QVBoxLayout     *commonLayout;

public slots:
    void             setInitState();
};

#endif // BIOS_BOOT_H
