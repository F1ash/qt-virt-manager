#ifndef MEMBALLOON_H
#define MEMBALLOON_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"

class MemBalloon : public _QWidget
{
    Q_OBJECT
public:
    explicit MemBalloon(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);
    QComboBox       *model;
    QCheckBox       *periodLabel;
    QSpinBox        *period;
    DeviceAddress   *addr;

private:
    QLabel          *modelLabel;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             modelChanged(int);
};

#endif // MEMBALLOON_H
