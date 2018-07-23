#ifndef _QWIDGET_H
#define _QWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDomDocument>
#include <libvirt.h>
#include <virterror.h>
#include "_changed.h"

class _QWidget : public _Changed
{
    Q_OBJECT
public:
    explicit _QWidget(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr*   connPtrPtr = Q_NULLPTR,
            virDomainPtr     domain     = Q_NULLPTR);

    virConnectPtr           *ptr_ConnPtr;
    virDomain               *currDomain;

signals:
    void                     errorMsg(const QString&);
    void                     ptrIsNull();

public slots:
    virtual QDomDocument     getDataDocument() const;
    virtual void             setDataDescription(const QString&);
    virtual bool             closeDataEdit();
    virtual void             setInitState();
};

#endif // _QWIDGET_H
