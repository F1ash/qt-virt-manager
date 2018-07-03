#ifndef CPU_ALLOCATIN_H
#define CPU_ALLOCATIN_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QDomDocument>
#include "create_widgets/common/_changed.h"
//#include <QDebug>

class CPU_Allocation : public _Changed
{
    Q_OBJECT
public:
    explicit CPU_Allocation(
            QWidget *parent = nullptr,
            QString  _caps = "");
    QCheckBox       *cpusetLabel, *currLabel,
                    *placementLabel;
    QSpinBox        *vcpu, *current;
    QLineEdit       *cpuset;
    QComboBox       *placement;

signals:
    //void             dataChanged();
    void             currentVCPU(int);

private:
    QString          capabilities;
    QString          type;
    QString          cores;
    QLabel          *vcpuLabel;
    QGridLayout     *commonLayout;

public slots:
    void             setPlacement(QString&);
    void             setMaxVCPU(int);

private slots:
    void             vcpuValueChanged(int);
    void             placementStateChanged(bool);
};

#endif // CPU_ALLOCATIN_H
