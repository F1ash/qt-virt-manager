#ifndef LXC_OS_BOOTING_H
#define LXC_OS_BOOTING_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/common_widgets/os_boot_widgets/_arch.h"

class LXC_OSBooting : public _QWidget
{
    Q_OBJECT
public:
    explicit LXC_OSBooting(
            QWidget *parent = nullptr,
            QString  _caps  = "");
    _Arch           *architecture;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QString          capabilities;
    QString          arch;
    QLabel          *initPathLabel;
    QLineEdit       *initPath;

    QLabel          *startLabel;
    QLabel          *targetLabel;
    QLabel          *countLabel;
    QLabel          *uidStart;
    QLabel          *gidStart;
    QLabel          *uidLabel;
    QLabel          *gidLabel;
    QSpinBox        *uidTarget;
    QSpinBox        *gidTarget;
    QSpinBox        *uidCount;
    QSpinBox        *gidCount;
    QCheckBox       *nameSpaceEnable;
    QGridLayout     *nameSpaceLayout;
    QWidget         *nameSpaceWidget;

    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
    void             setInitState();

private slots:
    void             changeArch(QString&);
};

#endif // LXC_OS_BOOTING_H
