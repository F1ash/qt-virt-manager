#ifndef LOGICAL_POOL_STUFF_H
#define LOGICAL_POOL_STUFF_H

#include "_pool_stuff.h"

class Logical_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Logical_Pool_Stuff(
            QWidget         *parent     = nullptr,
            virConnectPtr   *connPtrPtr = nullptr);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // LOGICAL_POOL_STUFF_H
