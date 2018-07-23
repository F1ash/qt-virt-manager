#ifndef ZFS_POOL_STUFF_H
#define ZFS_POOL_STUFF_H

#include "_pool_stuff.h"

class ZFS_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit ZFS_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // ZFS_POOL_STUFF_H
