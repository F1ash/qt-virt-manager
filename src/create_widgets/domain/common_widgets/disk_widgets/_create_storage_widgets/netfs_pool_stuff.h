#ifndef NETFS_POOL_STUFF_H
#define NETFS_POOL_STUFF_H

#include "_pool_stuff.h"

class NetFs_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit NetFs_Pool_Stuff(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // NETFS_POOL_STUFF_H
