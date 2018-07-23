#ifndef DIR_POOL_STUFF_H
#define DIR_POOL_STUFF_H

#include "_pool_stuff.h"

class Dir_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Dir_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // DIR_POOL_STUFF_H
