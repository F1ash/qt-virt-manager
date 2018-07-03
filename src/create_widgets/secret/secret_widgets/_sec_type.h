#ifndef _SECT_YPE_H
#define _SECT_YPE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDomDocument>
#include "virt_objects/virt_entity_config.h"
//#include <QDebug>

class _SecType : public QWidget
{
    Q_OBJECT
public:
    explicit _SecType(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);
    QLabel                 *info;
    QVBoxLayout            *baseLayout;
    virConnectPtr*          ptr_ConnPtr;

public slots:
    virtual QDomDocument    getSecStuff() const;
};

#endif // _SEC_TYPE_H
