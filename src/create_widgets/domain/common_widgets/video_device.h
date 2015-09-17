#ifndef VIDEO_DEVICE_H
#define VIDEO_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"

class VideoDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit VideoDevice(QWidget *parent = NULL);
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
