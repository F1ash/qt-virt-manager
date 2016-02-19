#ifndef SOUND_DEVICE_EDIT_H
#define SOUND_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/sound_device.h"

class SoundDevice_Edit : public SoundDevice
{
    Q_OBJECT
public:
    explicit SoundDevice_Edit(QWidget *parent = nullptr);

signals:

public slots:
    void setDataDescription(QString&);

};

#endif // SOUND_DEVICE_EDIT_H
