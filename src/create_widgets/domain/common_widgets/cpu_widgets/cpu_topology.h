#ifndef CPU_TOPOLOGY_H
#define CPU_TOPOLOGY_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>

class CPU_Topology : public QWidget
{
    Q_OBJECT
public:
    explicit CPU_Topology(QWidget *parent = 0);
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
