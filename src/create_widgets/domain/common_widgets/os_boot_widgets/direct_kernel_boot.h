#ifndef DIRECT_KERNEL_BOOT_H
#define DIRECT_KERNEL_BOOT_H

#include "create_widgets/domain/_qwidget.h"
#include "path_to_file.h"

class Direct_Kernel_Boot : public _QWidget
{
    Q_OBJECT
public:
    explicit Direct_Kernel_Boot(QWidget *parent = 0);
    QLineEdit       *cmdline;
    Path_To_File    *loader, *kernel, *initrd, *dtb;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QLabel          *loaderLabel, *kernelLabel,
                    *initrdLabel, *cmdlineLabel,
                    *dtbLabel;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // DIRECT_KERNEL_BOOT_H
