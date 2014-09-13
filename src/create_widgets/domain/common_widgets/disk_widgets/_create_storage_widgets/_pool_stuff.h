#ifndef _POOL_STUFF_H
#define _POOL_STUFF_H

#include <QWidget>
#include "_storage_source.h"
#include "_storage_target.h"

class _Pool_Stuff : public QWidget
{
    Q_OBJECT
public:
    explicit _Pool_Stuff(QWidget *parent = 0);
    _Storage_Source *source;
    _Storage_Target *target;

private:
    QVBoxLayout     *commonLayout;

public slots:
    virtual QString getStorageXMLDescFileName() const;
};

#endif // _POOL_STUFF_H
