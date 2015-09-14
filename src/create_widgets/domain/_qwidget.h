#ifndef _QWIDGET_H
#define _QWIDGET_H

#include <QWidget>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QtXml/QDomDocument>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include "_changed.h"

class _QWidget : public _Changed
{
    Q_OBJECT
public:
    explicit _QWidget(
            QWidget         *parent     = NULL,
            virConnectPtr*   connPtr    = NULL,
            virDomainPtr     domain     = NULL);

    virConnectPtr           *ptr_ConnPtr;
    virDomain               *currDomain = NULL;

public slots:
    virtual QDomDocument     getDataDocument() const;
    virtual void             setDataDescription(QString&);
    virtual QString          closeDataEdit();
    virtual void             setInitState();
};

#endif // _QWIDGET_H
