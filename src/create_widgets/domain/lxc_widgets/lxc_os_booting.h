#ifndef LXC_OS_BOOTING_H
#define LXC_OS_BOOTING_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/common_widgets/os_boot_widgets/_arch.h"

class LXC_OSBooting : public _QWidget
{
    Q_OBJECT
public:
    explicit LXC_OSBooting(
            QWidget *parent = Q_NULLPTR,
            QString  _caps  = "");
    _Arch           *architecture;

signals:
    void             domainType(const QString&);
    void             osType(const QString&);
    void             emulatorType(const QString&);

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
    void             setDataDescription(const QString&);
    void             setInitState();

private slots:
    void             changeArch(const QString&);
};

#endif // LXC_OS_BOOTING_H
