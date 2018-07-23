#ifndef _ARCH_H
#define _ARCH_H

#include "create_widgets/common/_qwidget.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QDomDocument>

class _Arch : public _QWidget
{
    Q_OBJECT
public:
    explicit _Arch(QWidget *parent = Q_NULLPTR,
                   QString  _caps  = "");

signals:
    void             domainType(const QString&);
    void             archType(const QString&);
    void             osType(const QString&);
    void             emulatorType(const QString&);
    void             maxVCPU(const QString&);

private:
    QString          capabilities;
    QDomDocument     doc;
    QLabel          *archLabel, *machLabel;
    QComboBox       *arch, *machines;
    QGridLayout     *commonLayout;

public slots:
    void             setItems();
    void             setArch(const QString&);
    void             setMachine(const QString&);
    QString          getArch() const;
    QString          getMachine() const;
    void             machineChanged(const QString&);
    void             archChanged(const QString&);
};

#endif // _ARCH_H
