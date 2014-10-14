#ifndef _STORAGE_SOURCE_H
#define _STORAGE_SOURCE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include "_adapter.h"
#include "_auth.h"
#include "_block_devices.h"
#include "create_widgets/domain/common_widgets/disk_widgets/_hosts.h"

class _Storage_Source : public QWidget
{
    Q_OBJECT
public:
    explicit _Storage_Source(QWidget *parent = NULL);
    QLineEdit           *dir, *named, *vendor, *product;
    QLabel              *deviceLabel, *dirLabel,
                        *hostLabel, *namedLabel,
                        *vendorLabel, *productLabel,
                        *adapterLabel, *authLabel,
                        *formatLabel;
    QComboBox           *format;
    _BlockDevices       *device;
    _Adapter            *adapter;
    _Storage_Auth       *auth;
    _Hosts              *host;

private:
    QGridLayout         *commonLayout;
};

#endif // _STORAGE_SOURCE_H
