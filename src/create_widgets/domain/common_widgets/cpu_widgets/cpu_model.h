#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDomDocument>
#include "CPU_Models.h"
#include "create_widgets/common/_changed.h"
//#include <QDebug>

class CPU_Model : public _Changed
{
    Q_OBJECT
public:
    explicit CPU_Model(QWidget *parent = nullptr);
    QCheckBox       *copyHostCPU, *allowFallback;

signals:
    //void             dataChanged();
    void             cpuUseElements(bool);

private:
    QString          currModel;
    QDomDocument     doc;
    QDomElement      _x86, _ppc64;
    QCheckBox       *useModel;
    QLabel          *matchLabel;
    QComboBox       *model, *match;
    QWidget         *baseWdg;
    QGridLayout     *baseLayout;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    void             archChanged(const QString&);
    QString          getModel() const;
    QString          getMode() const;
    QString          getMatch() const;
    void             setModel(QString&);
    void             setMatch(QString&);

private slots:
    void             copyHostCPUState(bool);
    void             modelChanged(int);
};

#endif // CPU_MODEL_H
