#ifndef FS_POOL_STUFF_H
#define FS_POOL_STUFF_H

#include "_pool_stuff.h"

class Fs_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Fs_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // FS_POOL_STUFF_H
