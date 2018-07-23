#ifndef GLUSTER_POOL_STUFF_H
#define GLUSTER_POOL_STUFF_H

#include "_pool_stuff.h"

class Gluster_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Gluster_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // GLUSTER_POOL_STUFF_H
