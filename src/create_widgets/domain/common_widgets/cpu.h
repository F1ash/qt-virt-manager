#ifndef CPU_H
#define CPU_H

#include <QScrollArea>
#include <QMessageBox>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"
#include "cpu_widgets/cpu_allocation.h"
#include "cpu_widgets/logical_host_cpu.h"

class CPU : public _QWidget
{
    Q_OBJECT
public:
    explicit CPU(
            QWidget *parent = NULL,
            QString  _caps  = "",
            QString  _xmlDesc = QString());
    CPU_Allocation  *cpuAlloc;

signals:

private:
    const QString    xmlDesc;
    QString          capabilities;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;

    LogicalHostCPU  *logicCPULabel;
    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    QString          closeDataEdit();
    void             setMaxVCPU(QString&);

private slots:
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetCPUData();
    void             revertCPUData();
    void             saveCPUData();
};

#endif // CPU_H
