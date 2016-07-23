#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"

class InputDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit InputDevice(QWidget *parent = nullptr);
    QComboBox       *type;
    QComboBox       *bus;
    DeviceAddress   *addr;

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // INPUT_DEVICE_H
