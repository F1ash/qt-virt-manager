#ifndef SHEEPDOG_POOL_STUFF_H
#define SHEEPDOG_POOL_STUFF_H

#include "_pool_stuff.h"

class SheepDog_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit SheepDog_Pool_Stuff(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR);
    void                setDataDescription(const QString&);
    QDomDocument        getDataDocument() const;
};

#endif // SHEEPDOG_POOL_STUFF_H
