#ifndef BCH_HOST_DEVICE_H
#define BCH_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"

class BCh_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit BCh_Host_Device(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *attention;
    QLabel          *info;
    QLabel          *typeLabel;
    QLabel          *deviceLabel;
    QComboBox       *type;
    QLineEdit       *device;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void typeChanged(int);
};

#endif // BCH_HOST_DEVICE_H
