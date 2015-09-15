#ifndef GRAPHICS_DEVICE_EDIT_H
#define GRAPHICS_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/graphics_device.h"

class GraphicsDevice_Edit : public GraphicsDevice
{
    Q_OBJECT
public:
    explicit GraphicsDevice_Edit(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

signals:

public slots:
    void             setDataDescription(QString&);
};

#endif // GRAPHICS_DEVICE_EDIT_H
