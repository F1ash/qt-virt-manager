#ifndef NETFS_POOL_STUFF_H
#define NETFS_POOL_STUFF_H

#include "_pool_stuff.h"

class NetFs_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit NetFs_Pool_Stuff(
            QWidget         *parent     = nullptr,
            virConnectPtr   *connPtrPtr = nullptr);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // NETFS_POOL_STUFF_H
