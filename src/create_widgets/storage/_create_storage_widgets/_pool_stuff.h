#ifndef _POOL_STUFF_H
#define _POOL_STUFF_H

#include <QDomDocument>
#include "_storage_source.h"
#include "_storage_target.h"

class _Pool_Stuff : public QWidget
{
    Q_OBJECT
public:
    explicit _Pool_Stuff(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);
    _Storage_Source        *source;
    _Storage_Target        *target;
    virtual void            setDataDescription(const QString&);
    virtual QDomDocument    getDataDocument() const;

private:
    QVBoxLayout            *commonLayout;
};

#endif // _POOL_STUFF_H
