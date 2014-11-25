#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDomDocument>
#include "CPU_Models.h"
#include <QDebug>

class CPU_Model : public QWidget
{
    Q_OBJECT
public:
    explicit CPU_Model(QWidget *parent = 0);

signals:
    void             dataChanged();

private:
    QDomDocument     doc;
    QDomElement      _x86, _ppc64;
    QCheckBox       *copyHostCPU;
    QComboBox       *model;
    QHBoxLayout     *commonLayout;

public slots:
    void             archChanged(QString&);

private slots:
    void             copyHostCPUState(bool);
};

#endif // CPU_MODEL_H
