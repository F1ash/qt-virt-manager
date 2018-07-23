#ifndef VIDEO_DEVICE_H
#define VIDEO_DEVICE_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"

class VideoDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit VideoDevice(QWidget *parent = Q_NULLPTR);
    QSpinBox        *vram;
    QCheckBox       *accel2d;
    QCheckBox       *accel3d;
    QComboBox       *model;
    DeviceAddress   *addr;

private:
    QLabel          *modelLabel;
    QLabel          *vramLabel;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // VIDEO_DEVICE_H
