#ifndef _STORAGE_SOURCE_H
#define _STORAGE_SOURCE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include "_adapter.h"
#include "_auth.h"

class _Storage_Source : public QWidget
{
    Q_OBJECT
public:
    explicit _Storage_Source(QWidget *parent = 0);
    QLineEdit           *device, *dir,
                        *host, *named,
                        *vendor, *product;
    QLabel              *deviceLabel, *dirLabel,
                        *hostLabel, *namedLabel,
                        *vendorLabel, *productLabel,
                        *adapterLabel, *authLabel,
                        *formatLabel;
    QComboBox           *format;
    _Adapter            *adapter;
    _Storage_Auth       *auth;

private:
    QGridLayout         *commonLayout;
};

#endif // _STORAGE_SOURCE_H
