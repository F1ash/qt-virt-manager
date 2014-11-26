#ifndef CPU_TOPOLOGY_H
#define CPU_TOPOLOGY_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>

class CPU_Topology : public QWidget
{
    Q_OBJECT
public:
    explicit CPU_Topology(QWidget *parent = 0);

signals:
    void             dataChanged();

private:
    QCheckBox       *use;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
};

#endif // CPU_TOPOLOGY_H
