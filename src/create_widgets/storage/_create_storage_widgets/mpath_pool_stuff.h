#ifndef MPATH_POOL_STUFF_H
#define MPATH_POOL_STUFF_H

#include "_pool_stuff.h"

class MPath_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit MPath_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // MPATH_POOL_STUFF_H
