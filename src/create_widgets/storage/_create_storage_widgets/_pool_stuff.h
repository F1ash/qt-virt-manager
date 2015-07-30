#ifndef _POOL_STUFF_H
#define _POOL_STUFF_H

#include <QWidget>
#include <QtXml/QDomDocument>
#include "_storage_source.h"
#include "_storage_target.h"

class _Pool_Stuff : public QWidget
{
    Q_OBJECT
public:
    explicit _Pool_Stuff(
            QWidget         *parent = NULL,
            virConnectPtr    _conn = NULL);
    _Storage_Source         *source;
    _Storage_Target         *target;

private:
    QVBoxLayout             *commonLayout;

public slots:
    virtual QDomDocument     getStorageXMLDesc() const;
};

#endif // _POOL_STUFF_H
