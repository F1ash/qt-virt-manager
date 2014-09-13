#ifndef SHEEPDOG_POOL_STUFF_H
#define SHEEPDOG_POOL_STUFF_H

#include "_pool_stuff.h"

class SheepDog_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit SheepDog_Pool_Stuff(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // SHEEPDOG_POOL_STUFF_H
