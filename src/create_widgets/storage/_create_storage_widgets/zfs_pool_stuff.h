#ifndef ZFS_POOL_STUFF_H
#define ZFS_POOL_STUFF_H

#include "_pool_stuff.h"

class ZFS_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit ZFS_Pool_Stuff(QWidget *parent = nullptr);

signals:

public slots:
    QDomDocument getStorageXMLDesc() const;
};

#endif // ZFS_POOL_STUFF_H
