#ifndef LOGICAL_POOL_STUFF_H
#define LOGICAL_POOL_STUFF_H

#include "_pool_stuff.h"

class Logical_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit Logical_Pool_Stuff(QWidget *parent = NULL);

signals:

public slots:
    QDomDocument getStorageXMLDesc() const;
};

#endif // LOGICAL_POOL_STUFF_H
