#ifndef LOGICAL_HOST_CPU_H
#define LOGICAL_HOST_CPU_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QDebug>

class LogicalHostCPU : public QWidget
{
    Q_OBJECT
public:
    explicit LogicalHostCPU(
            QWidget *parent = 0,
            QString  _caps = "");

signals:

private:
    QString          capabilities;
    int              cores;
    QLabel          *logicCPULabel;
    QLabel          *icon;
    QLabel          *info;
    QHBoxLayout     *commonLayout;

public slots:
    void             changeInfoVisibility(int);
};

#endif // LOGICAL_HOST_CPU_H
