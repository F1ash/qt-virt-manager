#ifndef LOGICAL_HOST_CPU_H
#define LOGICAL_HOST_CPU_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QHBoxLayout>
//#include <QDebug>

class LogicalHostCPU : public QWidget
{
    Q_OBJECT
public:
    explicit LogicalHostCPU(
            QWidget *parent = nullptr,
            uint     _cores = 0);

signals:

private:
    const uint       cores;
    QLabel          *logicCPULabel;
    QLabel          *icon;
    QLabel          *info;
    QHBoxLayout     *commonLayout;

public slots:
    void             changeInfoVisibility(int);
};

#endif // LOGICAL_HOST_CPU_H
