#ifndef ISCSI_POOL_STUFF_H
#define ISCSI_POOL_STUFF_H

#include "_pool_stuff.h"

class iSCSI_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit iSCSI_Pool_Stuff(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);

signals:

public slots:
    QDomDocument getDataDocument() const;
};

#endif // ISCSI_POOL_STUFF_H
