#ifndef BCH_HOST_DEVICE_H
#define BCH_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"

class BCh_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit BCh_Host_Device(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);
    QComboBox       *type;
    QLineEdit       *device;

private:
    QLabel          *attention;
    QLabel          *info;
    QLabel          *typeLabel;
    QLabel          *deviceLabel;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             typeChanged(int);
};

#endif // BCH_HOST_DEVICE_H
