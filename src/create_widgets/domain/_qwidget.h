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
#include <QDebug>

class _QWidget : public QWidget
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
    virtual QDomNodeList getNodeList() const;
    virtual QString      getDevType() const;
    virtual QString      getDevMode() const;
    virtual QString      getDevBus() const;
    virtual QString      getDevModel() const;
    virtual QString      getDevManaged() const;
    virtual QString      getDevDisplay() const;
    virtual QString      getDevXauth() const;
    virtual QString      getDevFullScreen() const;
    virtual QString      getDevPort() const;
    virtual QString      getDevAutoPort() const;
    virtual QString      getDevReplaceUser() const;
    virtual QString      getDevMultiUser() const;

};

#endif // _QWIDGET_H
