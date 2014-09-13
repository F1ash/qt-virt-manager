#ifndef ZFS_POOL_STUFF_H
#define ZFS_POOL_STUFF_H

#include "_pool_stuff.h"

class ZFS_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit ZFS_Pool_Stuff(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // ZFS_POOL_STUFF_H
