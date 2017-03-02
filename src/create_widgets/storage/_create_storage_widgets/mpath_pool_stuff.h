#ifndef MPATH_POOL_STUFF_H
#define MPATH_POOL_STUFF_H

#include "_pool_stuff.h"

class MPath_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit MPath_Pool_Stuff(QWidget *parent = nullptr);

signals:

public slots:
    QDomDocument getDataDocument() const;
};

#endif // MPATH_POOL_STUFF_H
