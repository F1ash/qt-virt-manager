#ifndef GLUSTER_POOL_STUFF_H
#define GLUSTER_POOL_STUFF_H

#include "_pool_stuff.h"

class Gluster_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Gluster_Pool_Stuff(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // GLUSTER_POOL_STUFF_H
