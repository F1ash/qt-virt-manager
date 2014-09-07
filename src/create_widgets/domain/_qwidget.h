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
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "_changed.h"

class _QWidget : public _Changed
{
    Q_OBJECT
public:
    explicit _QWidget(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL);

    virConnect      *currWorkConnect = NULL;
    virDomain       *currDomain = NULL;

public slots:
    virtual QDomDocument getDevDocument() const;
    virtual void         setDeviceData(QString&);
};

#endif // _QWIDGET_H
