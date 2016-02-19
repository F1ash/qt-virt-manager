#ifndef VIDEODEVICE_EDIT_H
#define VIDEODEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/video_device.h"

class VideoDevice_Edit : public VideoDevice
{
    Q_OBJECT
public:
    explicit VideoDevice_Edit(QWidget *parent = nullptr);

signals:

public slots:
    void setDataDescription(QString&);
};

#endif // VIDEODEVICE_EDIT_H
