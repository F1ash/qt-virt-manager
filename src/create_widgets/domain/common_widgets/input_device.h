#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"

class InputDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit InputDevice(QWidget *parent = 0);
    QComboBox       *type;
    QComboBox       *bus;
    DeviceAddress   *addr;

signals:

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDevDocument() const;

};

#endif // INPUT_DEVICE_H
