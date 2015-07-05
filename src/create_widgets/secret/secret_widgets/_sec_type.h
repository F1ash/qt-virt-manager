#ifndef _SECT_YPE_H
#define _SECT_YPE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDomDocument>
#include "virt_objects/virt_entity_config.h"
#include <QDebug>

class _SecType : public QWidget
{
    Q_OBJECT
public:
    explicit _SecType(
            QWidget        *parent = NULL,
            virConnectPtr   _conn  = NULL);
    QVBoxLayout            *baseLayout;
    virConnectPtr           currWorkConnection;

signals:

public slots:
    virtual QDomDocument    getSecStuff() const;
};

#endif // _SEC_TYPE_H
