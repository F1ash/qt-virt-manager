#ifndef RBD_POOL_STUFF_H
#define RBD_POOL_STUFF_H

#include "_pool_stuff.h"

class RBD_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit RBD_Pool_Stuff(
            QWidget         *parent = NULL,
            virConnectPtr    _conn  = NULL);

signals:

public slots:
    QDomDocument getStorageXMLDesc() const;
};

#endif // RBD_POOL_STUFF_H
