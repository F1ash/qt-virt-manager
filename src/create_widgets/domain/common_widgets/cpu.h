#ifndef CPU_H
#define CPU_H

#include <QScrollArea>
#include "create_widgets/domain/_tab.h"
#include "cpu_widgets/cpu_allocation.h"
#include "cpu_widgets/logical_host_cpu.h"
#include "cpu_widgets/cpu_model.h"
#include "cpu_widgets/cpu_topology.h"

class CPU : public _Tab
{
    Q_OBJECT
public:
    explicit CPU(
            QWidget *parent   = Q_NULLPTR,
            QString  _caps    = "",
            QString  _xmlDesc = QString(),
            uint     cores    = 0);
    CPU_Allocation  *cpuAlloc;

signals:

private:
    const QString    xmlDesc;
    QString          capabilities;

    LogicalHostCPU  *logicCPULabel;
    CPU_Model       *cpuModel;
    CPU_Topology    *cpuTopology;
    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;

public slots:
    QDomDocument     getDataDocument() const;
    void             setMaxVCPU(const QString&);
    void             changeArch(const QString&);

private slots:
    void             readXMLDesciption();
    void             readXMLDesciption(const QString&);
};

#endif // CPU_H
