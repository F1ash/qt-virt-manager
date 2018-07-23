#ifndef _SNAPSHOT_FLAGS_H
#define _SNAPSHOT_FLAGS_H

#include <QMenuBar>
#include <QActionGroup>
#include <math.h>
//#include <QDebug>

class _SnapshotFlags : public QMenu
{
    Q_OBJECT
public:
    explicit _SnapshotFlags(QWidget *parent = Q_NULLPTR);
    QActionGroup    *actGroup;
    uint             getCompositeFlag() const;
    virtual void     changeAvailableFlags(int);
    virtual void     changeFlagCompatibility();
};

#endif // _SNAPSHOT_FLAGS_H
