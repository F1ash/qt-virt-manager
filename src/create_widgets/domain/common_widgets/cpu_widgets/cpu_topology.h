#ifndef CPU_TOPOLOGY_H
#define CPU_TOPOLOGY_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include "create_widgets/common/_qwidget.h"

class CPU_Topology : public _QWidget
{
    Q_OBJECT
public:
    explicit CPU_Topology(QWidget *parent = Q_NULLPTR);
    QSpinBox        *sockets, *cores,
                    *threads;

signals:
    void             dataChanged();
    void             valueChanged(int);
    void             useTopology(bool);

private:
    int              MaxVCPU = 1;
    bool             overload = false;
    QCheckBox       *use;
    QLabel          *socketsLabel, *coresLabel,
                    *threadsLabel, *infoIcon, *info;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QHBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    void             setMaxVCPU(int);

private slots:
    void             newValue(int);
    void             changeInfoVisibility(bool);
};

#endif // CPU_TOPOLOGY_H
