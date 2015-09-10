#ifndef CONSOLED_EVICE_EDIT_H
#define CONSOLED_EVICE_EDIT_H

#include "create_widgets/domain/common_widgets/console_device.h"

class ConsoleDevice_Edit : public ConsoleDevice
{
    Q_OBJECT
public:
    explicit ConsoleDevice_Edit(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL);

signals:

public slots:
    void setDataDescription(QString&);
};

#endif // CONSOLED_EVICE_EDIT_H
